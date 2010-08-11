#include "sig.h"
#ifndef TLS
#include "readwrite.h"
#endif
#include "stralloc.h"
#include "substdio.h"
#include "alloc.h"
#include "auto_qmail.h"
#include "control.h"
#include "received.h"
#include "constmap.h"
#include "error.h"
#include "ipme.h"
#include "ip.h"
#include "qmail.h"
#include "str.h"
#include "fmt.h"
#include "scan.h"
#include "byte.h"
#include "case.h"
#include "env.h"
#include "now.h"
#include "exit.h"
#include "rcpthosts.h"
#ifndef TLS
#include "timeoutread.h"
#include "timeoutwrite.h"
#endif
#include "commands.h"
#include "qregex.h"
#include "strerr.h"
#include "wait.h"
#include "fd.h"
#include "dns.h"
#include "spf.h"
#include "cdb.h"
#include "auto_break.h"
#include "qmail-spp.h"


#define BMCHECK_BMF 0
#define BMCHECK_BMFNR 1
#define BMCHECK_BRT 2
#define BMCHECK_BRTNR 3
#define BMCHECK_BHELO 4

#define _XOPEN_SOURCE
#include <unistd.h>
#ifdef TLS
#include <openssl/ssl.h>
SSL *ssl = NULL;

stralloc clientcert = {0};
stralloc tlsserverciphers = {0};
#endif

int spp_val;

#define MAXHOPS 100
unsigned int databytes = 0;
unsigned int greetdelay = 0;
unsigned int drop_pre_greet = 0;
unsigned int mfchk = 0;
int timeout = 1200;
int rcptcounter = 0;
int maxrcpt = -1;
unsigned int spfbehavior = 0;
int useauth = 0;
int useauth_cl = 0;
int useauth_cdb = 0;
int auth_cdb_fd = -1;
char *auth_cdb_file;
static stralloc authcdb_vars = {0};
int usecram = 0;
unsigned int essl = 0;
char unique[FMT_ULONG + FMT_ULONG + 3];
static stralloc authin = {0};
static stralloc user = {0};
static stralloc pass = {0};
static stralloc chal = {0};
static stralloc slop = {0};
char *hostname;
char **childargs;
substdio ssup;
char upbuf[128];
int authd = 0;
unsigned int allow_insecure_auth = 0;
unsigned int require_auth = 0;
char pid_buf[FMT_ULONG];
stralloc title = {0};
int log_mail = 0;
int log_rcpt = 0;
unsigned long pw_expire = 0;
char rcptcheck_err[1024];

#ifdef TLS
unsigned int force_tls = 0;
unsigned int deny_tls = 0;
int flagtimedout = 0;
char *servercert = "control/servercert.pem";
void sigalrm()
{
 flagtimedout = 1;
}

int ssl_timeoutread(timeout,fd,buf,n) int timeout; int fd; char *buf; int n;
{
 int r; int saveerrno;
  if (flagtimedout) { errno = error_timeout; return -1; }
  alarm(timeout);
  if (ssl) {
    while(((r = SSL_read(ssl,buf,n)) <= 0)
          && (SSL_get_error(ssl, r) == SSL_ERROR_WANT_READ));
  }else r = read(fd,buf,n);
 saveerrno = errno;
 alarm(0);
 if (flagtimedout) { errno = error_timeout; return -1; }
 errno = saveerrno;
 return r;
}

int ssl_timeoutwrite(timeout,fd,buf,n) int timeout; int fd; char *buf; int n;
{
 int r; int saveerrno;
 if (flagtimedout) { errno = error_timeout; return -1; }
 alarm(timeout);
 if (ssl) {
    while(((r = SSL_write(ssl,buf,n)) <= 0)
          && (SSL_get_error(ssl, r) == SSL_ERROR_WANT_WRITE));
 }else r = write(fd,buf,n);
 saveerrno = errno;
 alarm(0);
 if (flagtimedout) { errno = error_timeout; return -1; }
 errno = saveerrno;
 return r;
}
#endif

int safewrite(fd,buf,len) int fd; char *buf; int len;
{
  int r;
#ifdef TLS
  r = ssl_timeoutwrite(timeout,fd,buf,len);
#else
  r = timeoutwrite(timeout,fd,buf,len);
#endif
  if (r <= 0) _exit(1);
  return r;
}

char ssoutbuf[512];
substdio ssout = SUBSTDIO_FDBUF(safewrite,1,ssoutbuf,sizeof ssoutbuf);

void flush() { substdio_flush(&ssout); }
void out(s) char *s; { substdio_puts(&ssout,s); }

void die_read() { _exit(1); }
void die_alarm() { out("451 timeout (#4.4.2)\r\n"); flush(); _exit(1); }
void die_nomem() { out("421 out of memory (#4.3.0)\r\n"); flush(); _exit(1); }
void die_control() { out("421 unable to read controls (#4.3.0)\r\n"); flush(); _exit(1); }
void die_ipme() { out("421 unable to figure out my IP addresses (#4.3.0)\r\n"); flush(); _exit(1); }
void die_fork() { out("421 unable to fork (#4.3.0)\r\n"); flush(); _exit(1); }
void die_rcpt() { out("421 unable to verify recipient (#4.3.0)\r\n"); flush(); _exit(1); }
void die_rcpt2() { out("421 unable to execute recipient check (#4.3.0)\r\n"); flush(); _exit(1); }
void straynewline() { out("451 See http://pobox.com/~djb/docs/smtplf.html.\r\n"); flush(); _exit(1); }
void die_cannot_auth() { out("421 REQUIRE_AUTH set without valid AUTH program.\r\n"); flush(); _exit(1); }
void die_cannot_cram() { out("421 ALLOW_CRAM not available\r\n"); flush(); _exit(1); }
void die_auth_cdb() { out("421 cannot read AUTH_CDB file\r\n"); flush(); _exit(1); }
void die_pre_greet() { out("554 SMTP protocol violation\r\n"); flush(); _exit(1); }

void err_bmf() { out("553 sorry, your envelope sender has been denied (#5.7.1)\r\n"); }
void err_brt() { out("553 sorry, your envelope recipient has been denied (#5.7.1)\r\n"); }
void err_bhelo() { out("553 sorry, your HELO host name has been denied (#5.7.1)\r\n"); }
void err_hmf() { out("553 sorry, your envelope sender domain must exist (#5.7.1)\r\n"); }
void err_smf() { out("451 DNS temporary failure (#4.3.0)\r\n"); }
void err_nogateway() { out("553 sorry, that domain isn't in my list of allowed rcpthosts (#5.7.1)\r\n"); }
#ifdef TLS
void err_nogwcert() { out("553 no valid cert for gatewaying (#5.7.1)\r\n"); }
void err_tlsfirst() { out("503 STARTTLS first (#5.5.1)\r\n"); }
void die_forcedenytls() { out("421 FORCE_TLS and DENY_TLS both found (#4.3.0)\r\n"); flush(); _exit(1); }
#endif
void err_unimpl() { out("502 unimplemented (#5.5.1)\r\n"); }
void err_syntax() { out("555 syntax error (#5.5.4)\r\n"); }
void err_relay() { out("553 we don't relay (#5.7.1)\r\n"); }
void err_wantmail() { out("503 MAIL first (#5.5.1)\r\n"); }
void err_seenmail() { out("503 only one MAIL command allowed (#5.5.1)\r\n"); }
void err_wantrcpt() { out("503 RCPT first (#5.5.1)\r\n"); }
void err_noop() { out("250 ok\r\n"); }
void err_vrfy() { out("252 send some mail, i'll try my best\r\n"); }
void err_qqt() { out("451 qqt failure (#4.3.0)\r\n"); }
void err_badrcpt() { out("553 sorry, no mailbox here by that name. (#5.1.1)\r\n"); }

int err_child() { out("454 oops, problem with child and I can't auth (#4.3.0)\r\n"); return -1; }
int err_fork() { out("454 oops, child won't start and I can't auth (#4.3.0)\r\n"); return -1; }
int err_pipe() { out("454 oops, unable to open pipe and I can't auth (#4.3.0)\r\n"); return -1; }
int err_write() { out("454 oops, unable to write pipe and I can't auth (#4.3.0)\r\n"); return -1; }
void err_authd() { out("503 you're already authenticated (#5.5.0)\r\n"); }
void err_authmail() { out("503 no auth during mail transaction (#5.5.0)\r\n"); }
void err_noauthavail() { out("503 auth not available (#5.3.3)\r\n"); }
int err_noauthtype() { out("504 auth type not available (#5.5.1)\r\n"); return -1; }
int err_authabrt() { out("501 auth exchange cancelled (#5.0.0)\r\n"); return -1; }
int err_input() { out("501 malformed auth input (#5.5.4)\r\n"); return -1; }
int err_authfirst() { out("503 AUTH first (#5.5.1)\r\n"); }
int err_authmethod() { out("454 oops, unknown AUTH back-end (#4.3.0)\r\n"); return -1; }

void err_vrt() { out("553 sorry, this recipient is not in my validrcptto list (#5.7.1)\r\n"); }
void die_vrt() { out("421 too many invalid addresses, goodbye (#4.3.0)\r\n"); flush(); _exit(1); }

stralloc greeting = {0};
stralloc spflocal = {0};
stralloc spfguess = {0};
stralloc spfexp = {0};
int spf_log = 0;
int help_version = 0;

void smtp_greet(code) char *code;
{
  substdio_puts(&ssout,code);
  substdio_put(&ssout,greeting.s,greeting.len);
}
void smtp_help()
{
  if(help_version)
    out("214-qmail home page: http://pobox.com/~djb/qmail.html\r\n"
        "214 jms1 combined patch v7.08 http://qmail.jms1.net/patches/combined.shtml\r\n");
  else
    out("214 qmail home page: http://pobox.com/~djb/qmail.html\r\n");
}
void smtp_quit()
{
  smtp_greet("221 "); out("\r\n"); flush(); _exit(0);
}

char *remoteip;
char *remotehost;
char *remoteinfo;
char *local;
char *relayclient;
static char *rcptcheck[2] = { 0, 0 };
#ifdef TLS
char *tlsciphers;
#endif

void err_size() {
  out("552 sorry, that message size exceeds my databytes limit (#5.3.4)\r\n");
  strerr_warn4(title.s,"DATABYTES exceeded [",remoteip,"]",0);
}

stralloc helohost = {0};
char *fakehelo; /* pointer into helohost, or 0 */

void dohelo(arg) char *arg; {
  if (!stralloc_copys(&helohost,arg)) die_nomem(); 
  if (!stralloc_0(&helohost)) die_nomem(); 
  fakehelo = case_diffs(remotehost,helohost.s) ? helohost.s : 0;
}

int liphostok = 0;
stralloc liphost = {0};

int bmfok = 0;
stralloc bmf = {0};

int bmfnrok = 0;
stralloc bmfnr = {0};

int brtok = 0;
stralloc brt = {0};

int brtnrok = 0;
stralloc brtnr = {0};

int bhelook = 0;
stralloc bhelo = {0};

int vrtfd = -1;
int vrtcount = 0;
int vrtlimit = 10;
int vrtlog_do = 0;

int relayrej = 0;

void readenv()
{
  char *x;
  unsigned long u;

  x = env_get("MFCHECK");
  if (x) { scan_ulong(x,&u); mfchk = u; }

  x = env_get("MAXRCPT");
  if (x) { scan_ulong(x,&u); maxrcpt = u; }

  /* RFC 2821 section 4.5.3.1 "recipients buffer" */
  if(maxrcpt<1) maxrcpt=-1;
  else if(maxrcpt<100) maxrcpt=100 ;

  x = env_get("DATABYTES");
  if (x) { scan_ulong(x,&u); databytes = u; }
  if (!(databytes + 1)) --databytes;

  x = env_get("SPFBEHAVIOR");
  if (x) { scan_ulong(x,&u); spfbehavior = u; }

  x = env_get("VALIDRCPTTO_LIMIT");
  if(x) { scan_ulong(x,&u); vrtlimit = (int) u; }

  x = env_get("VALIDRCPTTO_LOG");
  if(x) { scan_ulong(x,&u); vrtlog_do = (int) u; }

  x = env_get("SPF_LOG");
  if(x) { scan_ulong(x,&u); spf_log = (int) u; }

  x = env_get("RELAYREJ");
  if(x) { scan_ulong(x,&u); relayrej = (int) u; }

  x = env_get("VALIDRCPTTO_CDB");
  if(x) {
    if (-1 != vrtfd) { close(vrtfd); vrtfd = -1; }
    if(*x) {
      vrtfd = open_read(x);
      if (-1 == vrtfd) die_control();
    }
  }
  else if (-1 != vrtfd) { close(vrtfd); vrtfd = -1; }

  x = env_get("QMAILSMTPD_LOG_MAIL");
  if(x) { scan_ulong(x,&u); log_mail = (int) u; }

  x = env_get("QMAILSMTPD_LOG_RCPT");
  if(x) { scan_ulong(x,&u); log_rcpt = (int) u; }

  x = env_get("QMAILSMTPD_HELP_VERSION");
  if(x) { scan_ulong(x,&u); help_version = (int) u; }

  rcptcheck[0] = env_get("RCPTCHECK");
}

int logregex = 0;
stralloc matchedregex = {0};

void setup()
{
  char *x;
  unsigned long u;
#ifdef TLS
  char *tlsciphers;
#endif
 
  if (control_init() == -1) die_control();
  if (control_rldef(&greeting,"control/smtpgreeting",1,(char *) 0) != 1)
    die_control();
  x=env_get("SMTPGREETING");
  if(x) { if(!stralloc_copys(&greeting,x)) die_nomem(); }
  liphostok = control_rldef(&liphost,"control/localiphost",1,(char *) 0);
  if (liphostok == -1) die_control();
  if (control_readint(&timeout,"control/timeoutsmtpd") == -1) die_control();
  if (timeout <= 0) timeout = 1;
  if (control_readint(&maxrcpt,"control/maxrcpt") == -1) die_control();

  if (rcpthosts_init() == -1) die_control();
  if (spp_init() == -1) die_control();

  if (control_readint(&mfchk,"control/mfcheck") == -1) die_control();

  bmfok = control_readfile(&bmf,"control/badmailfrom",0);
  if (bmfok == -1) die_control();

  bmfnrok = control_readfile(&bmfnr,"control/badmailfromnorelay",0);
  if (bmfnrok == -1) die_control();

  brtok = control_readfile(&brt,"control/badrcptto",0);
  if (brtok == -1) die_control();

  brtnrok = control_readfile(&brtnr,"control/badrcpttonorelay",0);
  if (brtnrok == -1) die_control();

  bhelook = control_readfile(&bhelo,"control/badhelo",0);
  if(bhelook == -1) die_control() ;
  if(env_get("NOBADHELO")) bhelook = 0;

  if(env_get("LOGREGEX")) logregex = 1;

  auth_cdb_file = env_get("AUTH_CDB");
  if(auth_cdb_file) {
    if ( ! access(auth_cdb_file,R_OK) ) { useauth_cdb = 1; }
    else die_auth_cdb() ;
  }

  if (useauth_cdb) {
    auth_cdb_fd = open_read(auth_cdb_file);
    if(-1 == auth_cdb_fd) die_control();
  }
 
  if (control_readint(&databytes,"control/databytes") == -1) die_control();
 
  if (control_readint(&spfbehavior,"control/spfbehavior") == -1)
    die_control();

  if (control_readline(&spflocal,"control/spfrules") == -1) die_control();
  if (spflocal.len && !stralloc_0(&spflocal)) die_nomem();
  if (control_readline(&spfguess,"control/spfguess") == -1) die_control();
  if (spfguess.len && !stralloc_0(&spfguess)) die_nomem();
  if (control_rldef(&spfexp,"control/spfexp",0,SPF_DEFEXP) == -1)
    die_control();
  if (!stralloc_0(&spfexp)) die_nomem();

  x = env_get("GREETDELAY");
  if(x) { scan_ulong(x,&u); greetdelay = u; }

  x = env_get("DROP_PRE_GREET");
  if(x) { scan_ulong(x,&u); drop_pre_greet = u; }

  remoteip = env_get("TCPREMOTEIP");
  if (!remoteip) remoteip = "unknown";
  local = env_get("TCPLOCALHOST");
  if (!local) local = env_get("TCPLOCALIP");
  if (!local) local = "unknown";
  remotehost = env_get("TCPREMOTEHOST");
  if (!remotehost) remotehost = "unknown";
  remoteinfo = env_get("TCPREMOTEINFO");
  relayclient = env_get("RELAYCLIENT");
#ifdef TLS
  if (tlsciphers = env_get("TLSCIPHERS")){
    if (!stralloc_copys(&tlsserverciphers,tlsciphers)) die_nomem();
  }
  else {
    if (control_rldef(&tlsserverciphers,"control/tlsserverciphers",0,"DEFAULT") != 1)
      die_control();
  }
  if (!stralloc_0(&tlsserverciphers)) die_nomem();

  x=env_get("TLS_SERVER_CERT");
  if(x) servercert=x;
#endif
  readenv();
  dohelo(remotehost);
}


stralloc addr = {0}; /* will be 0-terminated, if addrparse returns 1 */

int addrparse(arg)
char *arg;
{
  int i;
  char ch;
  char terminator;
  struct ip_address ip;
  int flagesc;
  int flagquoted;
 
  terminator = '>';
  i = str_chr(arg,'<');
  if (arg[i])
    arg += i + 1;
  else { /* partner should go read rfc 821 */
    terminator = ' ';
    arg += str_chr(arg,':');
    if (*arg == ':') ++arg;
    while (*arg == ' ') ++arg;
  }

  /* strip source route */
  if (*arg == '@') while (*arg) if (*arg++ == ':') break;

  if (!stralloc_copys(&addr,"")) die_nomem();
  flagesc = 0;
  flagquoted = 0;
  for (i = 0;ch = arg[i];++i) { /* copy arg to addr, stripping quotes */
    if (flagesc) {
      if (!stralloc_append(&addr,&ch)) die_nomem();
      flagesc = 0;
    }
    else {
      if (!flagquoted && (ch == terminator)) break;
      switch(ch) {
        case '\\': flagesc = 1; break;
        case '"': flagquoted = !flagquoted; break;
        default: if (!stralloc_append(&addr,&ch)) die_nomem();
      }
    }
  }
  /* could check for termination failure here, but why bother? */
  if (!stralloc_append(&addr,"")) die_nomem();

  if (liphostok) {
    i = byte_rchr(addr.s,addr.len,'@');
    if (i < addr.len) /* if not, partner should go read rfc 821 */
      if (addr.s[i + 1] == '[')
        if (!addr.s[i + 1 + ip_scanbracket(addr.s + i + 1,&ip)])
          if (ipme_is(&ip)) {
            addr.len = i + 1;
            if (!stralloc_cat(&addr,&liphost)) die_nomem();
            if (!stralloc_0(&addr)) die_nomem();
          }
  }

  if (addr.len > 900) return 0;
  return 1;
}

int sizelimit(arg) /* modified SIZELIMIT function by Erwin Hoffmann (tx Uwe Ohse) */
char *arg;   
{
  int i;
  long r;
  char terminator;
  unsigned long sizebytes = 0;

  terminator = '>';
  i = str_chr(arg,'<');
  if (arg[i])
    arg += i + 1;
  else {
    terminator = ' ';
    arg += str_chr(arg,':');
    if (*arg == ':') ++arg;
    while (*arg == ' ') ++arg;
  }

  arg += str_chr(arg,terminator);
  if (*arg && terminator == '>' ) ++arg; /* end of adddress */

  while (*++arg) {
    i = str_chr(arg,'=');
    arg[i] = 0;
    if (case_equals(arg,"SIZE")) {
      arg += i;
      while (*++arg && *arg > 47 && *arg < 58) {
        sizebytes *= 10;
        sizebytes += *arg - 48; }
      r = databytes - sizebytes;
      if (r < 0) return 0;
    }
    else
      ++arg;
  }
  return 1;
}

int bmcheck(which) int which;
{
  int i = 0;
  int j = 0;
  int x = 0;
  int negate = 0;
  static stralloc bmb = {0};
  static stralloc curregex = {0};

  if (which == BMCHECK_BMF) {
    if (!stralloc_copy(&bmb,&bmf)) die_nomem();
  } else if (which == BMCHECK_BMFNR) {
    if (!stralloc_copy(&bmb,&bmfnr)) die_nomem();
  } else if (which == BMCHECK_BRT) {
    if (!stralloc_copy(&bmb,&brt)) die_nomem();
  } else if (which == BMCHECK_BRTNR) {
    if (!stralloc_copy(&bmb,&brtnr)) die_nomem();
  } else if (which == BMCHECK_BHELO) {
    if (!stralloc_copy(&bmb,&bhelo)) die_nomem();
  } else {
    die_control();
  }

  while (j < bmb.len) {
    i = j;
    while ((bmb.s[i] != '\0') && (i < bmb.len)) i++;
    if (bmb.s[j] == '!') {
      negate = 1;
      j++;
    }
    if (!stralloc_copyb(&curregex,bmb.s + j,(i - j))) die_nomem();
    if (!stralloc_0(&curregex)) die_nomem();
    if (which == BMCHECK_BHELO) {
      x = matchregex(helohost.s, curregex.s);
    } else {
      x = matchregex(addr.s, curregex.s);
    }
    if ((negate) && (x == 0)) {
      if (!stralloc_copyb(&matchedregex,bmb.s + j - 1,(i - j + 1))) die_nomem();
      if (!stralloc_0(&matchedregex)) die_nomem();      
      return 1;
    }
    if (!(negate) && (x > 0)) {
      if (!stralloc_copyb(&matchedregex,bmb.s + j,(i - j))) die_nomem();
      if (!stralloc_0(&matchedregex)) die_nomem();
      return 1;
    }
    j = i + 1;
    negate = 0;
  }
  return 0;
}

int mfcheck()
{
  stralloc sa = {0};
  ipalloc ia = {0};
  unsigned int random;
  int j;

  if (str_equal(addr.s,"#@[]")) return 0;
  if (!mfchk) return 0;
  random = now() + (getpid() << 16);
  j = byte_rchr(addr.s,addr.len,'@') + 1;
  if (addr.len == 1) return 0;
  if (j < addr.len) {
    stralloc_copys(&sa, addr.s + j);
    dns_init(0);
    j = dns_mxip(&ia,&sa,random);
    stralloc_0(&sa);
    if (j < 0) {
      if(mfchk>1) strerr_warn5(title.s,"MFCHECK fail [",remoteip,"] ",sa.s,0);
      return j;
    }
    if(mfchk>2) strerr_warn5(title.s,"MFCHECK pass [",remoteip,"] ",sa.s,0);
    return 0;
  }
  if(mfchk>1) strerr_warn5(title.s,"MFCHECK invalid [",remoteip,"] ",addr.s,0);
  return DNS_HARD;
}

void vrtlog(l,a,b)
int l;
const char *a;
const char *b;
{
  if (l <= vrtlog_do)
    strerr_warn6(title.s,"validrcptto [",remoteip,"] ",a,b,0);
}

int vrtcheck()
{
  static char *rcptto = "RCPT TO: ";
  static char *trying = "trying: ";
  static char *found  = "found: ";
  static char *reject = "reject: ";
  char *f = 0;
  int j,k,r;
  uint32 dlen;
  stralloc laddr = {0};

  stralloc user = {0};
  stralloc adom = {0};
  stralloc utry = {0};
  stralloc dval = {0};

  if (-1 == vrtfd) return 1;

  /* lowercase whatever we were sent */
  if (!stralloc_copy(&laddr,&addr)) die_nomem() ;
  case_lowerb(laddr.s,laddr.len);

  if ( !log_rcpt ) vrtlog ( 1 , rcptto , laddr.s );

  /* exact match? */
  vrtlog ( 2 , trying , laddr.s );
  r = cdb_seek(vrtfd,laddr.s,laddr.len-1,&dlen) ;
  if (r>0) f=laddr.s ;
  else
  {
    j = byte_rchr(laddr.s,laddr.len,'@');
    if (j < laddr.len)
    {
      /* start "-default" search loop */
      stralloc_copyb(&user,laddr.s,j) ;
      stralloc_copyb(&adom,laddr.s+j,laddr.len-j-1);
  
      while(1)
      {
        k = byte_rchr(user.s,user.len,*auto_break);
        if (k >= user.len) break ;
  
        user.len = k+1;
        stralloc_cats(&user,"default");
  
        stralloc_copy(&utry,&user);
        stralloc_cat (&utry,&adom);
        stralloc_0(&utry);
  
        vrtlog ( 2 , trying , utry.s );
        r = cdb_seek(vrtfd,utry.s,utry.len-1,&dlen);
        if (r>0) { f=utry.s; break; }
  
        user.len = k ;
      }

      /* try "@domain" */
      if(!f) {
        vrtlog ( 2 , trying , laddr.s+j );
        r = cdb_seek(vrtfd,laddr.s+j,laddr.len-j-1,&dlen) ;
        if (r>0) f=laddr.s+j ;
      }
    }
  }

  if(f) {
    if(dlen) {
      if(!stralloc_ready(&dval,dlen)) die_nomem();
      dval.len = read(vrtfd,dval.s,dlen);
      if(dval.len>0) if(dval.s[0] == '-') {
        vrtlog ( 1 , reject , f ) ;
        return 0;
      }
    }
    vrtlog ( 1 , found , f ) ;
    return 1;
  }

  return 0;
}

int addrallowed()
{
  int r;
  r = rcpthosts(addr.s,str_len(addr.s));
  if (r == -1) die_control();
  return r;
}

int addrrelay()
{
  int j;
  if(relayrej) {
    j = addr.len;
    while(--j >= 0)
      if (addr.s[j] == '@') break;
    if (j < 0) j = addr.len;
    while(--j >= 0) {
      if (addr.s[j] == '@') return 1;
      if (addr.s[j] == '%') return 1;
      if (addr.s[j] == '!') return 1;
    }
  }
  return 0;
}

int seenmail = 0;
int flagbarf; /* defined if seenmail */
int flagbarfbmf; /* defined if seenmail */
int flagbarfbrt;
int flagbarfbhelo;
int flagbarfspf;
stralloc spfbarfmsg = {0};
stralloc mailfrom = {0};
stralloc rcptto = {0};
int allowed;

int addrvalid()
{
  int pid;
  int wstat;
  int pierr[2] ;
  substdio ss;
  char ssbuf[sizeof(rcptcheck_err)];
  int len = 0 ;
  char ch;

  if (!rcptcheck[0]) return 1;
  if (pipe(pierr) == -1) die_rcpt2();

  switch(pid = fork()) {
    case -1:
      close(pierr[0]);
      close(pierr[1]);
      die_fork();
    case 0:
      if (!env_put2("SENDER",mailfrom.s)) die_nomem();
      if (!env_put2("RECIPIENT",addr.s)) die_nomem();
      if (!env_put2("HELO",helohost.s)) die_nomem();
      if (!env_put2("USE_FD4","1")) die_nomem();
      close(1);
      dup2(2,1);
      close(pierr[0]);
      if (fd_move(4,pierr[1]) == -1) die_rcpt2();
      execv(*rcptcheck,rcptcheck);
      _exit(120);
  }

  close(pierr[1]);
  if (wait_pid(&wstat,pid) == -1) die_rcpt2();
  if (wait_crashed(wstat)) die_rcpt2();

  substdio_fdbuf(&ss,read,pierr[0],ssbuf,sizeof(ssbuf));
  while ( substdio_bget(&ss,&ch,1) && len < (sizeof(ssbuf)-3) )
    rcptcheck_err[len++] = ch;
  close(pierr[0]);

  while (len&&((rcptcheck_err[len-1]=='\n')||(rcptcheck_err[len-1]=='\r')))
    len -- ;
  if (len) {
    rcptcheck_err[len] = '\0';
    strerr_warn3(title.s,"RCPTCHECK error: ",rcptcheck_err,0);
    rcptcheck_err[len++] = '\r';
    rcptcheck_err[len++] = '\n';
  }
  rcptcheck_err[len] = '\0';

  switch(wait_exitcode(wstat)) {
    case 100:
      if (!len) {
        len = str_copy(rcptcheck_err,"553 no mailbox here by that name");
        rcptcheck_err[len] = '\0' ;
      }
    case 111:
      if (!len) {
        len = str_copy(rcptcheck_err,"450 unable to verify recipient");
        rcptcheck_err[len] = '\0' ;
      }
      return 0;
    case 120: die_rcpt2();
  }
  return 1;
}

void smtp_helo(arg) char *arg;
{
  if(!spp_helo(arg)) return;
  smtp_greet("250 "); out("\r\n");
  seenmail = 0; dohelo(arg);
  if (bhelook) flagbarfbhelo = bmcheck(BMCHECK_BHELO);
}
char size_buf[FMT_ULONG];
void smtp_size()
{
  size_buf[fmt_ulong(size_buf,(unsigned long) databytes)] = 0;
  out("\r\n250-SIZE "); out(size_buf);
}
void smtp_ehlo(arg) char *arg;
{
  if(!spp_helo(arg)) return;
  smtp_greet("250-");
  if (useauth)
  {
    if ( essl || allow_insecure_auth )
    {
      if (usecram)
      {
        out("\r\n250-AUTH LOGIN CRAM-MD5 PLAIN");
        out("\r\n250-AUTH=LOGIN CRAM-MD5 PLAIN");
      }
      else
      {
        out("\r\n250-AUTH LOGIN PLAIN");
        out("\r\n250-AUTH=LOGIN PLAIN");
      }
    }
    else if (usecram)
    {
        out("\r\n250-AUTH CRAM-MD5");
        out("\r\n250-AUTH=CRAM-MD5");
    }
  }
#ifdef TLS
  if (!(essl||deny_tls)) out("\r\n250-STARTTLS");
#endif
  smtp_size();
  out("\r\n250-PIPELINING\r\n250 8BITMIME\r\n");
  seenmail = 0; dohelo(arg);
  if (bhelook) flagbarfbhelo = bmcheck(BMCHECK_BHELO);
}
void smtp_rset()
{
  spp_rset();
  seenmail = 0;
  out("250 flushed\r\n");
}
void smtp_mail(arg) char *arg;
{
  int r;
  if (seenmail) { err_seenmail(); return; }
#ifdef TLS
  if (force_tls) if (!ssl) { err_tlsfirst(); return; }
#endif
  if (require_auth) if (!authd) { err_authfirst(); return; }
  if (!addrparse(arg)) { err_syntax(); return; }
  if (!(spp_val = spp_mail())) return;
  if (spp_val == 1)
  if (databytes && !sizelimit(arg)) { err_size(); return; }
  flagbarfbmf = 0; /* bmcheck is skipped for empty envelope senders */
  if((bmfok) && (addr.len != 1)) flagbarfbmf = bmcheck(BMCHECK_BMF);
  if ((!flagbarfbmf) && (bmfnrok) && (addr.len != 1) && (!relayclient) && (!authd)) {
    flagbarfbmf = bmcheck(BMCHECK_BMFNR);
  }
  switch(mfcheck()) { /* make sure MAIL FROM domain has an MX record */
    case DNS_HARD: err_hmf(); return;
    case DNS_SOFT: err_smf(); return;
    case DNS_MEM: die_nomem();
  }
  flagbarfspf = 0;
  if (spfbehavior && !relayclient)
  {
    switch (r = spfcheck())
    {
      case SPF_OK: env_put2("SPFRESULT","pass"); break;
      case SPF_NONE: env_put2("SPFRESULT","none"); break;
      case SPF_UNKNOWN: env_put2("SPFRESULT","unknown"); break;
      case SPF_NEUTRAL: env_put2("SPFRESULT","neutral"); break;
      case SPF_SOFTFAIL: env_put2("SPFRESULT","softfail"); break;
      case SPF_FAIL: env_put2("SPFRESULT","fail"); break;
      case SPF_ERROR: env_put2("SPFRESULT","error"); break;
    }
    if (spf_log)
    {
      stralloc si = {0};
      if (!spfinfo(&si)) die_nomem();
      if (!stralloc_0(&si)) die_nomem();
      strerr_warn3(title.s,"Received-SPF: ",si.s,0);
    }
    switch (r)
    {
      case SPF_NOMEM:
        die_nomem();
      case SPF_ERROR:
        if (spfbehavior < 2) break ;
        out ("451 SPF lookup failure (#4.3.0)\r\n");
        return;
      case SPF_NONE:
      case SPF_UNKNOWN:
        if (spfbehavior < 6) break ;
      case SPF_NEUTRAL:
        if (spfbehavior < 5) break ;
      case SPF_SOFTFAIL:
        if (spfbehavior < 4) break ;
      case SPF_FAIL:
        if (spfbehavior < 3) break ;
        if (!spfexplanation(&spfbarfmsg)) die_nomem();
        if (!stralloc_0(&spfbarfmsg)) die_nomem();
        flagbarfspf = 1;
    }
  }
  else
    env_unset("SPFRESULT");
  seenmail = 1;
  if (!stralloc_copys(&rcptto,"")) die_nomem();
  if (!stralloc_copys(&mailfrom,addr.s)) die_nomem();
  if (!stralloc_0(&mailfrom)) die_nomem();
  if (log_mail) { strerr_warn4(title.s,"MAIL FROM:<",mailfrom.s,">",0); }
  rcptcounter = 0 ;
  out("250 ok\r\n");
}

void err_spf()
{
  int i,j;

  for( i=0 ; i < spfbarfmsg.len ; i=j+1 ) {
    j = byte_chr(spfbarfmsg.s + i,spfbarfmsg.len - i, '\n') + i;
    if (j < spfbarfmsg.len){
      out("550-");
      spfbarfmsg.s[j] = 0;
      out(spfbarfmsg.s);
      spfbarfmsg.s[j] = '\n';
    } else {
      out("550 ");
      out(spfbarfmsg.s);
      out(" (#5.7.1)\r\n");
    }
  }
}

#ifdef TLS
static int verify_cb(int ok, X509_STORE_CTX * ctx)
{
  return(1);
}
#endif

void smtp_rcpt(arg) char *arg; {
  rcptcounter++;
  if (!seenmail) { err_wantmail(); return; }
  if (checkrcptcount() == 1) { err_syntax(); return; }
  if (!addrparse(arg)) { err_syntax(); return; }
  if (addrrelay()) { err_relay(); return; }
  if (log_rcpt) { strerr_warn4(title.s,"RCPT TO:<",addr.s,">",0); }
  if (flagbarfbhelo) {
    if (logregex) {
      strerr_warn7(title.s,"badhelo: <",helohost.s,"> at ",remoteip," matches pattern: ",matchedregex.s,0);
    } else {
      strerr_warn5(title.s,"badhelo: <",helohost.s,"> at ",remoteip,0);
    }
    err_bhelo();
    return;
  }
  if (flagbarfbmf) {
    if (logregex) {
      strerr_warn7(title.s,"badmailfrom: <",mailfrom.s,"> at ",remoteip," matches pattern: ",matchedregex.s,0);
    } else {
      strerr_warn5(title.s,"badmailfrom: <",mailfrom.s,"> at ",remoteip,0);
    }
    err_bmf();
    return;
  }
  if (brtok) flagbarfbrt = bmcheck(BMCHECK_BRT);
  if ((!flagbarfbrt) && (brtnrok) && (!relayclient) && (!authd)) {
    flagbarfbrt = bmcheck(BMCHECK_BRTNR);
  }
  if (flagbarfbrt) {
    if (logregex) {
      strerr_warn7(title.s,"badrcptto: <",addr.s,"> at ",remoteip," matches pattern: ",matchedregex.s,0);
    } else {
      strerr_warn5(title.s,"badrcptto: <",addr.s,"> at ",remoteip,0);
    }
    err_brt();
    return;
  }

  if (flagbarfspf) { err_spf(); return; }
  if (!relayclient) allowed = addrallowed();
  else allowed = 1;
  if (!(spp_val = spp_rcpt(allowed))) return;

  if (relayclient) {
    --addr.len;
    if (!stralloc_cats(&addr,relayclient)) die_nomem();
    if (!stralloc_0(&addr)) die_nomem();
  }
  else
#ifndef TLS
  if (spp_val == 1) {
    if (!allowed) { err_nogateway(); return; }
  }
//    if (!addrallowed()) { err_nogateway(); return; }
#else
   if (spp_val == 1) {
//    if (!addrallowed())
     if (!allowed)
     {
      	if (ssl)
      	{ 
		STACK_OF(X509_NAME) *sk;
        	X509 *peercert;
	        stralloc tlsclients = {0};
	        struct constmap maptlsclients;
	        int r;

	        SSL_set_verify(ssl,
                       SSL_VERIFY_PEER|SSL_VERIFY_CLIENT_ONCE,
                       verify_cb);
	        if ((sk = SSL_load_client_CA_file("control/clientca.pem")) == NULL)
		         { err_nogateway(); return; }
	        SSL_set_client_CA_list(ssl, sk);
	        if((control_readfile(&tlsclients,"control/tlsclients",0) != 1) ||
	           !constmap_init(&maptlsclients,tlsclients.s,tlsclients.len,0))
	          { err_nogateway(); return; }

        	SSL_renegotiate(ssl);
	        SSL_do_handshake(ssl);
	        ssl->state = SSL_ST_ACCEPT;
	        SSL_do_handshake(ssl);
	        if ((r = SSL_get_verify_result(ssl)) != X509_V_OK)
	        {
		     out("553 no valid cert for gatewaying: ");
	            out(X509_verify_cert_error_string(r));
	            out(" (#5.7.1)\r\n");
	            return;
         	}

	        if (peercert = SSL_get_peer_certificate(ssl))
	           {
			char emailAddress[256];

		          X509_NAME_get_text_by_NID(X509_get_subject_name(
                                     SSL_get_peer_certificate(ssl)),
                                     NID_pkcs9_emailAddress, emailAddress, 256);
		          if (!stralloc_copys(&clientcert, emailAddress)) die_nomem();
	        	  if (!constmap(&maptlsclients,clientcert.s,clientcert.len))
		              { err_nogwcert(); return; }
		          relayclient = "";
             	   }
    	      	   else { err_nogwcert(); return; }
       	   }
           else { err_nogateway(); return; }
       }
     }
#endif

  if (!relayclient && !vrtcheck()) {
    strerr_warn5(title.s,"validrcptto [",remoteip,"] not found: ",
      addr.s,0);
    if(vrtlimit && (++vrtcount >= vrtlimit)) {
      strerr_warn4(title.s,"validrcptto [",remoteip,
        "] excessive violations, hanging up",0);
      die_vrt();
    }
    err_vrt();
    return;
  }

  if (!addrvalid()) {
    if (rcptcheck_err[0])
      out (rcptcheck_err);
    else
      err_badrcpt();
    return;
  }

  spp_rcpt_accepted();
  if (!stralloc_cats(&rcptto,"T")) die_nomem();
  if (!stralloc_cats(&rcptto,addr.s)) die_nomem();
  if (!stralloc_0(&rcptto)) die_nomem();
  out("250 ok\r\n");
}


int saferead(fd,buf,len) int fd; char *buf; int len;
{
  int r;
  flush();
#ifdef TLS
  r = ssl_timeoutread(timeout,fd,buf,len);
#else
  r = timeoutread(timeout,fd,buf,len);
#endif
  if (r == -1) if (errno == error_timeout) die_alarm();
  if (r <= 0) die_read();
  return r;
}

char ssinbuf[1024];
substdio ssin = SUBSTDIO_FDBUF(saferead,0,ssinbuf,sizeof ssinbuf);

struct qmail qqt;
unsigned int bytestooverflow = 0;

void put(ch)
char *ch;
{
  if (bytestooverflow)
    if (!--bytestooverflow)
      qmail_fail(&qqt);
  qmail_put(&qqt,ch,1);
}

void blast(hops)
int *hops;
{
  char ch;
  int state;
  int flaginheader;
  int pos; /* number of bytes since most recent \n, if fih */
  int flagmaybex; /* 1 if this line might match RECEIVED, if fih */
  int flagmaybey; /* 1 if this line might match \r\n, if fih */
  int flagmaybez; /* 1 if this line might match DELIVERED, if fih */
 
  state = 1;
  *hops = 0;
  flaginheader = 1;
  pos = 0; flagmaybex = flagmaybey = flagmaybez = 1;
  for (;;) {
    substdio_get(&ssin,&ch,1);
    if (flaginheader) {
      if (pos < 9) {
        if (ch != "delivered"[pos]) if (ch != "DELIVERED"[pos]) flagmaybez = 0;
        if (flagmaybez) if (pos == 8) ++*hops;
        if (pos < 8)
          if (ch != "received"[pos]) if (ch != "RECEIVED"[pos]) flagmaybex = 0;
        if (flagmaybex) if (pos == 7) ++*hops;
        if (pos < 2) if (ch != "\r\n"[pos]) flagmaybey = 0;
        if (flagmaybey) if (pos == 1) flaginheader = 0;
        ++pos;
      }
      if (ch == '\n') { pos = 0; flagmaybex = flagmaybey = flagmaybez = 1; }
    }
    switch(state) {
      case 0:
        if (ch == '\n') straynewline();
        if (ch == '\r') { state = 4; continue; }
        break;
      case 1: /* \r\n */
        if (ch == '\n') straynewline();
        if (ch == '.') { state = 2; continue; }
        if (ch == '\r') { state = 4; continue; }
        state = 0;
        break;
      case 2: /* \r\n + . */
        if (ch == '\n') straynewline();
        if (ch == '\r') { state = 3; continue; }
        state = 0;
        break;
      case 3: /* \r\n + .\r */
        if (ch == '\n') return;
        put(".");
        put("\r");
        if (ch == '\r') { state = 4; continue; }
        state = 0;
        break;
      case 4: /* + \r */
        if (ch == '\n') { state = 1; break; }
        if (ch != '\r') { put("\r"); state = 0; }
    }
    put(&ch);
  }
}

void spfreceived()
{
  stralloc sa = {0};
  stralloc rcvd_spf = {0};

  if (!spfbehavior || relayclient) return;

  if (!stralloc_copys(&rcvd_spf, "Received-SPF: ")) die_nomem();
  if (!spfinfo(&sa)) die_nomem();
  if (!stralloc_cat(&rcvd_spf, &sa)) die_nomem();
  if (!stralloc_append(&rcvd_spf, "\n")) die_nomem();
  if (bytestooverflow) {
    bytestooverflow -= rcvd_spf.len;
    if (bytestooverflow <= 0) qmail_fail(&qqt);
  }
  qmail_put(&qqt,rcvd_spf.s,rcvd_spf.len);
}


char accept_buf[FMT_ULONG];
void acceptmessage(qp) unsigned long qp;
{
  datetime_sec when;
  when = now();
  out("250 ok ");
  accept_buf[fmt_ulong(accept_buf,(unsigned long) when)] = 0;
  out(accept_buf);
  out(" qp ");
  accept_buf[fmt_ulong(accept_buf,qp)] = 0;
  out(accept_buf);
  out("\r\n");
}

/* addvars() - find and add environment variables after encrypted password
   in cdb data. if doit=0, only PASSWORD_EXPIRES is searched for. this way
   expired passwords don't get their extra vars if password is expired. */
void addvars(s,doit)
char *s;
int doit;
{
  char *n;
  char *v;
  int x;
  int y;

  n = s;

  while (*n)
  {
    if (','==*n) n++ ;
    x = str_chr(n,'=');
    if (!n[x]) return ;
    if (n[x+1]!='\"') return ;
    v = n+x+2 ;
    y = str_chr(v,'\"');
    if (!v[y]) return ;
    if(!str_diff(n,"PASSWORD_EXPIRES")) scan_ulong(v,&pw_expire);
    if(doit) {
      n[x]=0;
      v[y]=0;
      env_put2(n,v);
    }
    n = v+y+1;
  }
}

void auth_fixenv()
{
  int i,f;
  char *envi,*eq;
  stralloc work = {0};

  do
  {
    f=0;
    for (i=0;envi=environ[i];++i)
    {
      if (!str_diffn("AUTH_SET_",envi,9))
      {
        stralloc_copys(&work,envi);
        stralloc_0(&work);
        eq = env_findeq(work.s);
        *eq=0;
        env_unset(work.s);
        if(authd)
        {
          env_unset(work.s+9);
          *eq='=';
          env_put(work.s+9);
        }
        f=1;
        break;
      }
      if (!str_diffn("AUTH_UNSET_",envi,11))
      {
        stralloc_copys(&work,envi);
        stralloc_0(&work);
        eq = env_findeq(work.s);
        *eq=0;
        env_unset(work.s);
        if(authd) env_unset(work.s+11);
        f=1;
        break;
      }
    }
  } while (f);

  if(authcdb_vars.s)
    addvars(authcdb_vars.s,1);
}

void smtp_data() {
  int hops;
  unsigned long qp;
  char *qqx;
#ifdef TLS
  stralloc protocolinfo = {0};
#endif
 
  if (!seenmail) { err_wantmail(); return; }
  if (!rcptto.len) { err_wantrcpt(); return; }
  if (!spp_data()) return;
  seenmail = 0;
  if (databytes) bytestooverflow = databytes + 1;
  if (qmail_open(&qqt) == -1) { err_qqt(); return; }
  qp = qmail_qp(&qqt);
  out("354 go ahead\r\n");

#ifdef TLS
  if(ssl){
   if (!stralloc_copys(&protocolinfo, SSL_CIPHER_get_name(SSL_get_current_cipher(ssl)))) die_nomem();
   if (!stralloc_catb(&protocolinfo, " encrypted SMTP", 15)) die_nomem();
   if (clientcert.len){
     if (!stralloc_catb(&protocolinfo," cert ", 6)) die_nomem();
     if (!stralloc_catb(&protocolinfo,clientcert.s, clientcert.len)) die_nomem();
   }
   if (!stralloc_0(&protocolinfo)) die_nomem();
  } else if (!stralloc_copyb(&protocolinfo,"SMTP",5)) die_nomem();
  received(&qqt,protocolinfo.s,local,remoteip,remotehost,remoteinfo,case_diffs(remotehost,helohost.s) ? helohost.s : 0);
#else
  received(&qqt,"SMTP",local,remoteip,remotehost,remoteinfo,fakehelo);
#endif
  qmail_put(&qqt,sppheaders.s,sppheaders.len); /* set in qmail-spp.c */
  spp_rset(); 

  spfreceived();
  blast(&hops);
  hops = (hops >= MAXHOPS);
  if (hops) qmail_fail(&qqt);
  qmail_from(&qqt,mailfrom.s);
  qmail_put(&qqt,rcptto.s,rcptto.len);
 
  qqx = qmail_close(&qqt);
  if (!*qqx) { acceptmessage(qp); return; }
  if (hops) { out("554 too many hops, this message is looping (#5.4.6)\r\n"); return; }
  if (databytes) if (!bytestooverflow) { err_size(); return; }
  if (*qqx == 'I') out("250 ok "); else if (*qqx == 'D') out("554 "); else out("451 ");
  out(qqx + 1);
  out("\r\n");
}
#ifdef TLS
static RSA *tmp_rsa_cb(ssl,export,keylength) SSL *ssl; int export; int keylength; 
{
  RSA* rsa;
  BIO* in;

  if (!export || keylength == 512)
   if (in=BIO_new(BIO_s_file_internal()))
    if (BIO_read_filename(in,"control/rsa512.pem") > 0)
     if (rsa=PEM_read_bio_RSAPrivateKey(in,NULL,NULL,NULL))
      return rsa;
  return (RSA_generate_key(export?keylength:512,RSA_F4,NULL,NULL));
}

void smtp_tls(arg) char *arg;
{
  SSL_CTX *ctx;

  if (*arg)
   {out("501 Syntax error (no parameters allowed) (#5.5.4)\r\n");
    return;}

  if (ssl)
   {out("454 TLS not available: TLS already active (4.3.0)\r\n");
    return;}

  if (essl)
   {out("454 TLS not available: already SSL secured (4.3.0)\r\n");
    return;}

  if (deny_tls)
   {out("454 TLS not available (4.3.0)\r\n");
    return;}

  SSL_library_init();
  if(!(ctx=SSL_CTX_new(SSLv23_server_method())))
   {out("454 TLS not available: unable to initialize ctx (#4.3.0)\r\n");
    return;}
  if(!SSL_CTX_use_RSAPrivateKey_file(ctx, servercert, SSL_FILETYPE_PEM))
   {out("454 TLS not available: missing RSA private key (#4.3.0)\r\n");
    return;}
  if(!SSL_CTX_use_certificate_chain_file(ctx, servercert))
   {out("454 TLS not available: missing certificate (#4.3.0)\r\n");
    return;}
  SSL_CTX_set_tmp_rsa_callback(ctx, tmp_rsa_cb);
  SSL_CTX_set_cipher_list(ctx,tlsserverciphers.s);
  SSL_CTX_load_verify_locations(ctx, "control/clientca.pem",NULL);
  SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, verify_cb);

  out("220 ready for tls\r\n"); flush();
  essl=1;

  if(!(ssl=SSL_new(ctx))) die_read();
  SSL_set_rfd(ssl,0);
  SSL_set_wfd(ssl,1);
  if(SSL_accept(ssl)<=0) die_read();
  substdio_fdbuf(&ssout,SSL_write,ssl,ssoutbuf,sizeof(ssoutbuf));

  remotehost = env_get("TCPREMOTEHOST");
  if (!remotehost) remotehost = "unknown";
  dohelo(remotehost);
}
#endif


int authgetl(void) {
  int i;

  if (!stralloc_copys(&authin, "")) die_nomem();

  for (;;) {
    if (!stralloc_readyplus(&authin,1)) die_nomem(); /* XXX */
    i = substdio_get(&ssin,authin.s + authin.len,1);
    if (i != 1) die_read();
    if (authin.s[authin.len] == '\n') break;
    ++authin.len;
  }

  if (authin.len > 0) if (authin.s[authin.len - 1] == '\r') --authin.len;
  authin.s[authin.len] = 0;

  if (*authin.s == '*' && *(authin.s + 1) == 0) { return err_authabrt(); }
  if (authin.len == 0) { return err_input(); }
  return authin.len;
}

int authenticate_cl(void)
{
  int child;
  int wstat;
  int pi[2];

  if (!stralloc_0(&user)) die_nomem();
  if (!stralloc_0(&pass)) die_nomem();
  if (!stralloc_0(&chal)) die_nomem();

  if (pipe(pi) == -1) return err_pipe();
  switch(child = fork()) {
    case -1:
      return err_fork();
    case 0:
      close(pi[1]);
      if(fd_copy(3,pi[0]) == -1) return err_pipe();
      sig_pipedefault();
      execvp(*childargs, childargs);
      _exit(1);
  }
  close(pi[0]);

  substdio_fdbuf(&ssup,write,pi[1],upbuf,sizeof upbuf);
  if (substdio_put(&ssup,user.s,user.len) == -1) return err_write();
  if (substdio_put(&ssup,pass.s,pass.len) == -1) return err_write();
  if (substdio_put(&ssup,chal.s,chal.len) == -1) return err_write();
  if (substdio_flush(&ssup) == -1) return err_write();

  close(pi[1]);
  byte_zero(pass.s,pass.len);
  byte_zero(upbuf,sizeof upbuf);
  if (wait_pid(&wstat,child) == -1) return err_child();
  if (wait_crashed(wstat)) return err_child();
  if (wait_exitcode(wstat)) {
    strerr_warn5(title.s,"AUTH failed [",remoteip,"] ",user.s,0);
    sleep(5);
    return 1; /* no */
  }
  strerr_warn5(title.s,"AUTH successful [",remoteip,"] ",user.s,0);
  return 0; /* yes */
}

int authenticate_cdb(void)
{
  int r,x;
  uint32 dlen ;
  stralloc epw = {0} ;
  char *cr ;

  if (!stralloc_0(&user)) die_nomem();
  if (!stralloc_0(&pass)) die_nomem();

  r = cdb_seek(auth_cdb_fd,user.s,user.len-1,&dlen);
  if (r<=0) {
    strerr_warn5(title.s,"AUTH failed (unknown user) [",
      remoteip,"] ",user.s,0);
    return 1 ;
  }

  if (!stralloc_ready(&epw,dlen+1)) die_nomem() ;
  epw.len = dlen ;
  if (-1 == cdb_bread(auth_cdb_fd,epw.s,epw.len)) die_auth_cdb() ;
  if (!stralloc_0(&epw)) die_nomem();

  x = str_chr(epw.s,',') ;
  epw.s[x] = 0 ;

  if ( str_diff ( crypt ( pass.s , epw.s ) , epw.s ) ) {
    strerr_warn5(title.s,"AUTH failed (bad password) [",
      remoteip,"] ",user.s,0);
    return 1 ;
  }

  if ( x < dlen ) {
    stralloc_copyb(&authcdb_vars,epw.s+x+1,dlen-x-1);
    stralloc_0(&authcdb_vars);
    addvars(authcdb_vars.s,0);
    if(pw_expire && pw_expire <= time(0)) {
      strerr_warn5(title.s,"AUTH failed (password expired) [",
        remoteip,"] ",user.s,0);
      return 1 ;
    }
  }

  strerr_warn5(title.s,"AUTH successful [",remoteip,"] ",user.s,0);
  return 0 ;
}

int authenticate(void)
{
  if(useauth_cdb) return authenticate_cdb();
  if(useauth_cl)  return authenticate_cl();
  /* future auth back-end modules here */
  return err_authmethod();
}

int auth_login(arg) char *arg;
{
  int r;

  if ( ! ( essl || allow_insecure_auth ) ) return err_noauthtype() ;

  if (*arg) {
    if (r = b64decode(arg,str_len(arg),&user) == 1) return err_input();
  }
  else {
    out("334 VXNlcm5hbWU6\r\n"); flush(); /* Username: */
    if (authgetl() < 0) return -1;
    if (r = b64decode(authin.s,authin.len,&user) == 1) return err_input();
  }
  if (r == -1) die_nomem();

  out("334 UGFzc3dvcmQ6\r\n"); flush(); /* Password: */

  if (authgetl() < 0) return -1;
  if (r = b64decode(authin.s,authin.len,&pass) == 1) return err_input();
  if (r == -1) die_nomem();

  if (!user.len || !pass.len) return err_input();
  return authenticate();  
}

int auth_plain(arg) char *arg;
{
  int r, id = 0;

  if ( ! ( essl || allow_insecure_auth ) ) return err_noauthtype() ;

  if (*arg) {
    if (r = b64decode(arg,str_len(arg),&slop) == 1) return err_input();
  }
  else {
    out("334 \r\n"); flush();
    if (authgetl() < 0) return -1;
    if (r = b64decode(authin.s,authin.len,&slop) == 1) return err_input();
  }
  if (r == -1 || !stralloc_0(&slop)) die_nomem();
  while (slop.s[id]) id++; /* ignore authorize-id */

  if (slop.len > id + 1)
    if (!stralloc_copys(&user,slop.s + id + 1)) die_nomem();
  if (slop.len > id + user.len + 2)
    if (!stralloc_copys(&pass,slop.s + id + user.len + 2)) die_nomem();

  if (!user.len || !pass.len) return err_input();
  return authenticate();
}

int auth_cram()
{
  int i, r;
  char *s;

  if ( ! usecram ) return err_noauthtype();

  s = unique;
  s += fmt_uint(s,getpid());
  *s++ = '.';
  s += fmt_ulong(s,(unsigned long) now());
  *s++ = '@';
  *s++ = 0;

  if (!stralloc_copys(&chal,"<")) die_nomem();
  if (!stralloc_cats(&chal,unique)) die_nomem();
  if (!stralloc_cats(&chal,hostname)) die_nomem();
  if (!stralloc_cats(&chal,">")) die_nomem();
  if (b64encode(&chal,&slop) < 0) die_nomem();
  if (!stralloc_0(&slop)) die_nomem();

  out("334 ");
  out(slop.s);
  out("\r\n");
  flush();

  if (authgetl() < 0) return -1;
  if (r = b64decode(authin.s,authin.len,&slop) == 1) return err_input();
  if (r == -1 || !stralloc_0(&slop)) die_nomem();

  i = str_chr(slop.s,' ');
  s = slop.s + i;
  while (*s == ' ') ++s;
  slop.s[i] = 0;
  if (!stralloc_copys(&user,slop.s)) die_nomem();
  if (!stralloc_copys(&pass,s)) die_nomem();

  if (!user.len || !pass.len) return err_input();
  return authenticate();
}

struct authcmd {
  char *text;
  int (*fun)();
} authcmds[] = {
  { "login", auth_login }
, { "plain", auth_plain }
, { "cram-md5", auth_cram }
, { 0, err_noauthtype }
};

void smtp_auth(arg)
char *arg;
{
  int i;
  char *cmd = arg;

  if (!useauth) { err_noauthavail(); return; }
  if (authd) { err_authd(); return; }
  if (seenmail) { err_authmail(); return; }

  if (!stralloc_copys(&user,"")) die_nomem();
  if (!stralloc_copys(&pass,"")) die_nomem();
  if (!stralloc_copys(&chal,"")) die_nomem();
  if (!stralloc_copys(&authcdb_vars,"")) die_nomem();
  pw_expire = 0 ;

  i = str_chr(cmd,' ');   
  arg = cmd + i;
  while (*arg == ' ') ++arg;
  cmd[i] = 0;

  for (i = 0;authcmds[i].text;++i)
    if (case_equals(authcmds[i].text,cmd)) break;

  switch (authcmds[i].fun(arg)) {
    case 0:
      authd = 1;
      relayclient = "";
      auth_fixenv();
      readenv();
      remoteinfo = user.s;
      if (!env_unset("TCPREMOTEINFO")) die_read();
      if (!env_put2("TCPREMOTEINFO",remoteinfo)) die_nomem();
      if (!env_unset("SMTP_AUTH_USER")) die_read();
      if (!env_put2("SMTP_AUTH_USER",remoteinfo)) die_nomem();
      out("235 ok, go ahead (#2.0.0)\r\n");
      break;
    case 1:
      out("535 authorization failed (#5.7.0)\r\n");
  }
}

struct commands smtpcommands[] = {
  { "rcpt", smtp_rcpt, 0 }
, { "mail", smtp_mail, 0 }
, { "data", smtp_data, flush }
, { "auth", smtp_auth, flush }
, { "quit", smtp_quit, flush }
, { "helo", smtp_helo, flush }
, { "ehlo", smtp_ehlo, flush }
, { "rset", smtp_rset, 0 }
, { "help", smtp_help, flush }
#ifdef TLS
, { "starttls", smtp_tls, flush }
#endif
, { "noop", err_noop, flush }
, { "vrfy", err_vrfy, flush }
, { 0, err_unimpl, flush }
} ;

void main(argc,argv)
int argc;
char **argv;
{
  char *x ;
  unsigned long u ;

  if (argc>3)
  {
    hostname = argv[1];
    childargs = argv + 2;
    useauth_cl = 1;
  }

  pid_buf[fmt_ulong(pid_buf,getpid())]=0;
  if (!stralloc_copys(&title,"qmail-smtpd[")) die_nomem();
  if (!stralloc_cats(&title,pid_buf)) die_nomem();
  if (!stralloc_cats(&title,"]: ")) die_nomem();
  if (!stralloc_0(&title)) die_nomem();

#ifdef TLS
  sig_alarmcatch(sigalrm);
#endif
  sig_pipeignore();
  if (chdir(auto_qmail) == -1) die_control();

  setup();

#ifdef TLS
  if(access(servercert,R_OK)) { deny_tls = 1; }
  x = env_get("DENY_TLS");
  if(x) { scan_ulong(x,&u); if (u>0) deny_tls = 1; }
  x = env_get("FORCE_TLS");
  if(x) { scan_ulong(x,&u); if (u>0) force_tls = 1; }
#endif

  x = env_get("SSL");
  if(x) { scan_ulong(x,&u); if (u>0) essl = 1; }
#ifdef TLS
  if (essl) { force_tls = 0; }
  if (force_tls && deny_tls) die_forcedenytls() ;
#endif

  x = env_get("ALLOW_INSECURE_AUTH");
  if(x) { scan_ulong(x,&u); if (u>0) allow_insecure_auth = 1; }

  x = env_get("REQUIRE_AUTH");
  if(x) { scan_ulong(x,&u); if (u>0) require_auth = 1; }

  if ( useauth_cl || useauth_cdb ) { useauth = 1 ; }

  if (require_auth && (!useauth)) die_cannot_auth() ;

  if (useauth)
  {
    x = env_get("ALLOW_CRAM");
    if(x) { scan_ulong(x,&u); usecram = (int) u; }
    if(useauth_cdb && usecram) die_cannot_cram() ;
  }

  if (ipme_init() != 1) die_ipme();
  if (greetdelay||drop_pre_greet) {
    x = timeoutread(greetdelay?greetdelay:1,0,ssinbuf,sizeof ssinbuf);
    if(-1 == x) {
      if(errno != error_timeout) strerr_die4sys(1,
        title.s,"before greeting: [", remoteip, "] ");
    } else if ( 0 == x ) {
      strerr_die4x(1,title.s,"before greeting: [", remoteip,
        "] client disconnected");
    } else if(drop_pre_greet) {
      strerr_warn4(title.s,"before greeting: [", remoteip,
        "] client sent data",0);
      die_pre_greet();
    }
  }
  if (spp_connect()) {
  smtp_greet("220 ");
  out(" ESMTP\r\n");
  }
  if (commands(&ssin,&smtpcommands) == 0) die_read();
  die_nomem();
}

int checkrcptcount() {
  if (maxrcpt == -1) { return 0;}
  else if (rcptcounter > maxrcpt ) {
    strerr_warn4(title.s,"MAXRCPT fail [",remoteip,"]",0);
    return 1;
  }
  return 0;
}
