/*
** Copyright 2001-2007 Double Precision, Inc.
** See COPYING for distribution information.
*/

#include "config.h"
#include "dbobj.h"
#include "liblock/config.h"
#include "liblock/liblock.h"
#include "numlib/numlib.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#if HAVE_LOCALE_H
#include <locale.h>
#endif
#if HAVE_STRINGS_H
#include <strings.h>
#endif
#include <ctype.h>
#include "rfc822/rfc822.h"
#include "rfc2045/rfc2045charset.h"
#include <sys/types.h>
#include "mywait.h"
#include <signal.h>
#if HAVE_SYSEXITS_H
#include <sysexits.h>
#endif

#ifndef EX_TEMPFAIL
#define EX_TEMPFAIL	75
#endif

static const char *txtfile=0, *mimefile=0;
static const char *recips=0;
static const char *dbfile=0;
static const char *subj=0;
static const char *charset=RFC2045CHARSET;
static const char *mimedsn=0;
static unsigned interval=1;
static char *sender;

struct header {
	struct header *next;
	char *buf;
} ;

static struct header *header_list;

static struct header *extra_headers=0;

static void usage()
{
	fprintf(stderr,
		"Usage: mailbot [ options ] [ $MAILER arg arg... ]\n"
		"\n"
		"    -t filename        - text autoresponse\n"
		"    -c charset         - text MIME character set (default " RFC2045CHARSET ")\n"
		"    -m filename        - text autoresponse with a MIME header\n"
		"    -r addr1,addr2...  - any 'addr' required in a To/Cc header\n");

	fprintf(stderr,
		"    -d $pathname       - database to prevent duplicate autoresponses\n"
		"    -D x               - at least 'x' days before dupes (default: 1)\n"
		"    -s subject         - Subject: on autoresponses\n"
		"    -A \"Header: stuff\" - Additional header on the autoresponse\n"
		"    -M recipient       - format autoresponse as a DSN from 'recipient'\n"
		"    $MAILER arg arg... - run $MAILER (sendmail) to mail the autoresponse\n"
		);
	exit(EX_TEMPFAIL);
}

static void read_headers()
{
	char buf[BUFSIZ];
	struct header **lasthdr= &header_list, *prevhdr=0;

	while (fgets(buf, sizeof(buf), stdin))
	{
		size_t l=strlen(buf);

		if (l > 0 && buf[l-1] == '\n')
			--l;
		if (l > 0 && buf[l-1] == '\r')
			--l;
		buf[l]=0;

		if (l == 0)
		{
			/* Eat rest of message from stdin */

			while (getc(stdin) != EOF)
				;
			break;
		}

		if (isspace((int)(unsigned char)buf[0]) && prevhdr)
		{
			if ( (prevhdr->buf=
			     realloc( prevhdr->buf,
				      strlen (prevhdr->buf)+2+strlen(buf)))
			     == NULL)
			{
				perror("malloc");
				exit(EX_TEMPFAIL);
			}
			strcat(strcat( prevhdr->buf, "\n"), buf);
		}
		else
		{
			if ((*lasthdr=(struct header *)
			     malloc(sizeof(struct header))) == NULL ||
			    ((*lasthdr)->buf=strdup(buf)) == NULL)
			{
				perror("malloc");
				exit(EX_TEMPFAIL);
			}

			prevhdr= *lasthdr;
			lasthdr= &(*lasthdr)->next;
		}
	}

	*lasthdr=NULL;
}

const char *hdr(const char *hdrname)
{
	struct header *h;
	size_t l=strlen(hdrname);

	for (h=header_list; h; h=h->next)
	{
		if (strncasecmp(h->buf, hdrname, l) == 0 &&
		    h->buf[l] == ':')
		{
			const char *p=h->buf+l+1;

			while (*p && isspace((int)(unsigned char)*p))
				++p;
			return (p);
		}
	}

	return ("");
}

/*
** Get the sender's address
*/

static void check_sender()
{
	const char *h=hdr("reply-to");
	struct rfc822t *t;
	struct rfc822a *a;

	if (!h || !*h)
		h=hdr("from");

	if (!h || !*h)
		exit(0);

	t=rfc822t_alloc_new(h, NULL, NULL);

	if (!t || !(a=rfc822a_alloc(t)))
	{
		perror("malloc");
		exit(EX_TEMPFAIL);
	}

	if (a->naddrs <= 0)
		exit (0);
	sender=rfc822_getaddr(a, 0);
	rfc822a_free(a);
	rfc822t_free(t);

	if (!sender || !*sender)
		exit(0);
}

/*
** Do not autorespond to DSNs
*/

static void check_dsn()
{
	static const char ct[]="multipart/report;";

	const char *p=hdr("content-type");

	if (strncasecmp(p, ct, sizeof(ct)-1) == 0)
		exit(0);

	p=hdr("precedence");

	if (strncasecmp(p, "junk", 4) == 0 ||
	    strncasecmp(p, "bulk", 4) == 0 ||
	    strncasecmp(p, "list", 4) == 0)
		exit(0);	/* Just in case */

	p=hdr("auto-submitted");

	if (*p && strcmp(p, "no"))
		exit(0);

	p=hdr("list-id");

	if (*p)
		exit(0);
}

/*
** Check for a required recipient
*/

static void check_recips()
{
	char *buf;
	struct rfc822t *t;
	struct rfc822a *a;
	struct header *h;

	if (!recips || !*recips)
		return;

	buf=strdup(recips);
	if (!buf)
	{
		perror("strdup");
		exit(EX_TEMPFAIL);
	}

	for (h=header_list; h; h=h->next)
	{
		int i;

		if (strncasecmp(h->buf, "to:", 3) &&
		    strncasecmp(h->buf, "cc:", 3))
			continue;

		t=rfc822t_alloc_new(h->buf+3, NULL, NULL);
		if (!t || !(a=rfc822a_alloc(t)))
		{
			perror("malloc");
			exit(EX_TEMPFAIL);
		}

		for (i=0; i<a->naddrs; i++)
		{
			char *p=rfc822_getaddr(a, i);
			char *q;

			strcpy(buf, recips);

			for (q=buf; (q=strtok(q, ", ")) != 0; q=0)
			{
				if (strcasecmp(p, q) == 0)
				{
					free(p);
					free(buf);
					rfc822a_free(a);
					rfc822t_free(t);
					return;
				}
			}

			free(p);
		}
		rfc822a_free(a);
		rfc822t_free(t);
	}
	free(buf);
	exit(0);
}

/*
** Check the dupe database.
*/

#ifdef DbObj
static void check_db()
{
	char *dbname;
	char *lockname;
	int lockfd;
	struct dbobj db;
	time_t now;
	char *sender_key, *p;

	size_t val_len;
	char *val;

	if (!dbfile || !*dbfile)
		return;

	sender_key=strdup(sender);
	dbname=malloc(strlen(dbfile)+ sizeof( "." DBNAME));
	lockname=malloc(strlen(dbfile)+ sizeof(".lock"));

	for (p=sender_key; *p; p++)
		*p=tolower((int)(unsigned char)*p);

	if (!dbname || !lockname || !sender)
	{
		perror("malloc");
		exit(EX_TEMPFAIL);
	}

	strcat(strcpy(dbname, dbfile), "." DBNAME);
	strcat(strcpy(lockname, dbfile), ".lock");

	lockfd=open(lockname, O_RDWR|O_CREAT, 0666);

	if (lockfd < 0 || ll_lock_ex(lockfd))
	{
		perror(lockname);
		exit(EX_TEMPFAIL);
	}

	dbobj_init(&db);

	if (dbobj_open(&db, dbname, "C") < 0)
	{
		perror(dbname);
		exit(EX_TEMPFAIL);
	}

	time(&now);

	val=dbobj_fetch(&db, sender_key, strlen(sender_key), &val_len, "");

	if (val)
	{
		time_t t;

		if (val_len >= sizeof(t))
		{
			memcpy(&t, val, sizeof(t));

			if (t >= now - interval * 60 * 60 * 24)
			{
				free(val);
				dbobj_close(&db);
				close(lockfd);
				exit(0);
			}
		}
		free(val);
	}

	dbobj_store(&db, sender_key, strlen(sender_key),
		    (void *)&now, sizeof(now), "R");
	dbobj_close(&db);
	close(lockfd);
}
#endif

static FILE *opensendmail(int argn, int argc, char **argv)
{
	char **newargv;
	int i;
	int pipefd[2];
	pid_t p;

	if (argn >= argc)
	{
		static char *sendmail_argv[]={"sendmail", "-f", ""};

		argn=0;
		argc=3;
		argv=sendmail_argv;
	}

	newargv=(char **)malloc( sizeof(char *)*(argc-argn+1));
	if (!newargv)
	{
		perror("malloc");
		exit(EX_TEMPFAIL);
	}

	for (i=0; argn+i < argc; i++)
		newargv[i]=argv[argn+i];
	newargv[i]=0;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		exit(EX_TEMPFAIL);
	}

	signal(SIGCHLD, SIG_DFL);

	p=fork();
	if (p < 0)
	{
		perror("fork");
		exit(EX_TEMPFAIL);
	}

	if (p)	/* Parent execs sendmail */
	{
		pid_t p2;
		int waitstat;

		/* Wait for first child to exit */

		while ( (p2=wait(&waitstat)) >= 0 && p2 != p)
			;

		if (p2 < 0)
			exit(EX_TEMPFAIL);

		if (!WIFEXITED(waitstat))
			exit(EX_TEMPFAIL);

		waitstat=WEXITSTATUS(waitstat);
		if (waitstat)
			exit(waitstat);

		dup2(pipefd[0], 0);
		close(pipefd[0]);
		close(pipefd[1]);

		execvp(newargv[0], newargv);
		perror(newargv[0]);
		exit(EX_TEMPFAIL);
	}

	/* Child - fork again, so that parent has no direct child procs */

	p=fork();

	if (p < 0)
	{
		perror("fork");
		exit(EX_TEMPFAIL);
	}

	if (p)
		exit (0);

	close(pipefd[0]);
	signal(SIGPIPE, SIG_DFL);

	return (fdopen(pipefd[1], "w"));
}

static char *mkboundary()
{
	char    hostnamebuf[256];
	pid_t   mypid;
	char    pidbuf[NUMBUFSIZE];
	time_t  mytime;
	char    timebuf[NUMBUFSIZE];
	static size_t   cnt=0;
	char    cntbuf[NUMBUFSIZE];
	char    *p;
	char	tempbuf[NUMBUFSIZE];

        hostnamebuf[sizeof(hostnamebuf)-1]=0;
        if (gethostname(hostnamebuf, sizeof(hostnamebuf)-1))
                hostnamebuf[0]=0;
        mypid=getpid();
        time(&mytime);
        libmail_str_pid_t(mypid, pidbuf);
        libmail_str_time_t(mytime, timebuf);
	libmail_str_size_t(++cnt, tempbuf);
	sprintf(cntbuf, "%4s", tempbuf);
	for (p=cntbuf; *p == ' '; *p++ = '0')
		;
	p=malloc(strlen(hostnamebuf)+strlen(pidbuf)
		 +strlen(timebuf)+strlen(cntbuf)+11);
	if (!p)
	{
		return (NULL);
	}

	sprintf(p, "=_%s-%s-%s-%s", hostnamebuf,
		pidbuf, timebuf, cntbuf);
	return (p);
}

static int find_boundary_fp(const char *boundary, FILE *fp)
{
	int l=strlen(boundary);
	char buf[BUFSIZ];

	clearerr(fp);
	if (fseek(fp, 0L, SEEK_SET) < 0)
		return (0);

	while (fgets(buf, sizeof(buf), fp))
	{
		if (buf[0] == '-' && buf[1] == '-' &&
		    strncasecmp(buf, boundary, l) == 0)
			return (-1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	int argn;
	FILE *outf;
	FILE *infp;
	char *boundary;

#if HAVE_SETLOCALE
	setlocale(LC_ALL, "C");
#endif

	sender=NULL;
	for (argn=1; argn < argc; argn++)
	{
		char optc;
		char *optarg;

		if (argv[argn][0] != '-')
			break;

		if (strcmp(argv[argn], "--") == 0)
		{
			++argn;
			break;
		}

		optc=argv[argn][1];
		optarg=argv[argn]+2;

		if (!*optarg)
			optarg=NULL;

		switch (optc) {
		case 'c':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			if (optarg && *optarg)
				charset=optarg;
			continue;
		case 't':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			txtfile=optarg;
			continue;
		case 'm':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			mimefile=optarg;
			continue;
		case 'r':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			recips=optarg;
			continue;
		case 'M':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			mimedsn=optarg;
			continue;
		case 'd':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			dbfile=optarg;
			continue;
		case 'D':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			interval=optarg ? atoi(optarg):1;
			continue;
		case 'A':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			if (optarg)
			{
				struct header **h;

				for (h= &extra_headers; *h;
				     h= &(*h)->next)
					;

				if ((*h=malloc(sizeof(struct header))) == 0 ||
				    ((*h)->buf=strdup(optarg)) == 0)
				{
					perror("malloc");
					exit(EX_TEMPFAIL);
				}
				(*h)->next=0;
			}
			continue;
		case 's':
			if (!optarg && argn+1 < argc)
				optarg=argv[++argn];

			subj=optarg;
			continue;

		case 'f':
			if (optarg && *optarg)
			{
				sender=strdup(optarg);
			}
			else
			{
				sender=getenv("SENDER");
				if (!sender)
					continue;
				sender=strdup(sender);
			}
			if (sender == NULL)
			{
				perror("malloc");
				exit(1);
			}
			continue;
		default:
			usage();
		}
	}

	if (!txtfile && !mimefile)
		usage();

	read_headers();

	if (sender == NULL || *sender == 0)
		check_sender();

	check_dsn();
	check_recips();
#ifdef DbObj
	check_db();
#endif
	if (mimefile)
	{
		infp=fopen(mimefile, "r");
		if (!infp)
		{
			perror(mimefile);
			exit(EX_TEMPFAIL);
		}
	}
	else
	{
		infp=fopen(txtfile, "r");
		if (!infp)
		{
			perror(txtfile);
			exit(EX_TEMPFAIL);
		}
	}

	boundary=mkboundary();

	while (boundary && find_boundary_fp(boundary, infp))
	{
		free(boundary);
		boundary=mkboundary();
	}

	if (!boundary)
	{
		perror("malloc");
		exit(EX_TEMPFAIL);
	}
	clearerr(infp);
	fseek(infp, 0L, SEEK_SET);

	outf=opensendmail(argn, argc, argv);

	{
		struct header *h;

		for (h=extra_headers; h; h=h->next)
			fprintf(outf, "%s\n", h->buf);
	}

	if (!subj || !*subj)
	{
		char *p;

		p=rfc822_coresubj_nouc(hdr("subject"), NULL);
		fprintf(outf, "Subject: Re: %s\n", p);
		free(p);
	}
	else
		fprintf(outf, "Subject: %s\n", subj);

	fprintf(outf,
		"To: %s\n"
		"Precedence: junk\n"
		"Auto-Submitted: auto-replied\n"
		"Mime-Version: 1.0\n",
		sender);

	if (mimedsn && *mimedsn)
	{
		fprintf(outf,
			"Content-Type: multipart/report;"
			" report-type=delivery-status;\n"
			"    boundary=\"%s\"\n"
			"\n"
			RFC2045MIMEMSG
			"\n"
			"--%s\n",
			boundary, boundary);
	}

	fprintf(outf, "%s",
		mimefile ? "":
		"Content-Transfer-Encoding: quoted-printable\n"
		);

	if (txtfile)
		fprintf(outf, "Content-Type: text/plain; charset=\"%s\"\n\n",
			charset);

	/* Convert text autoresponse to quoted-printable */

	{
		int c;
		int l=0;

		while ((c=getc(infp)) != EOF)
		{
			if (c == '\r')
				continue;

			if (c == '\n' || mimefile)
			{
				putc(c, outf);
				l=0;
				continue;
			}

			if (l > 75)
			{
				fprintf(outf, "=\n");
				l=0;
			}

			if (c != '\t' &&
			    (c == '=' || c < ' ' || c >= 0x7f))
			{
				fprintf(outf, "=%02X",
					(int)(unsigned char)c);
				l += 3;
			}
			else
			{
				putc(c, outf);
				++l;
			}
		}
	}

	if (mimedsn && *mimedsn)
	{
		time_t now;
		struct header *h;

		time(&now);

		fprintf(outf,
			"\n--%s\n"
			"Content-Type: message/delivery-status\n"
			"Content-Transfer-Encoding: 7bit\n\n"
			"Arrival-Date: %s\n"
			"\n"
			"Final-Recipient: rfc822; %s\n"
			"Action: delivered\n"
			"Status: 2.0.0\n"
			"\n--%s\n"
			"Content-Type: text/rfc822-headers; charset=us-ascii\n"
			"Content-Disposition: attachment\n"
			"Content-Transfer-Encoding: 7bit\n\n",
			boundary,
			rfc822_mkdate(now),
			mimedsn,
			boundary
			);

		for (h=header_list; h; h=h->next)
		{
			const char *p=h->buf;

			while (*p)
			{
				putc ( (*p) & 0x7F, outf);
				++p;
			}
			putc('\n', outf);
		}
		fprintf(outf, "\n--%s--\n", boundary);
	}

	fflush(outf);

	exit(0);
}
