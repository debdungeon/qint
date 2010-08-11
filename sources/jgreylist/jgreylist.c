/******************************************************************************

jgreylist.c
John Simpson <jms1@jms1.net> 2007-07-27

2007-07-29 jms1 (v2) - fixed two non-critical warnings, neither of which
  changes the program (it worked before and it works now.) Also using atoi()
  in the getenv_int() function, instead of manually converting the string to
  an int. Thanks to Ed Neville for reminding me to use "-Wall" when compiling
  it, and suggesting both changes.

2007-07-29 jms1 (v3) - fixed a REAL bug... it did work before, but it was
  using a different filename scheme than the perl version. Changed the logic
  to use the same filenames that the perl version uses. Thanks to Patrick
  "marlowe" McDonald for letting me know about the problem.

2007-07-30 jms1 (v4) - fixed a minor bug... the initial banner still had the
  original hard-coded "jgreylist" instead of using BANNER... thanks to "Egor
  SBP" for pointing out the fact that Samuel Adams doesn't write the best
  code in the world.

2007-07-30 jms1 (v5) - Samuel Adams strikes again. i really should have looked
  more closely at what the perl script was doing. the IP octets need to be
  three digits (i.e. "___/001/002/003" instead of "___/1/2/3".)

2007-07-30 jms1 (v6) - apparently we sometimes need to fflush() after sending
  a packet back to the client.

2007-08-19 jms1 (v7) - calling sigaction() after setting up the SIGALRM
  handler to clear the SA_RESTART flag, which makes read() return EINTR
  rather than restarting.

2007-08-21 jms1 (v8) - adding a JGREYLIST_LIMIT variable... if a client
  sends too many RCPT commands without taking the hint, hang up on them.
  Thanks to Egor Fisher for the suggestion.

*******************************************************************************

Copyright (C) 2007 John Simpson.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 or version 3 of the
license, at your option.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/types.h>

/******************************************************************************

hard-coded messages

******************************************************************************/

#define BANNER	"jgreylist v8"
#define GREY1	"Try again later."
#define GREY2	"I said to try later."
#define TOOMANY "Too many RCPT commands, goodbye."

/******************************************************************************

environment variable stuff

JGREYLIST		- if empty, connection is allowed without checks
			- if not empty, connection is denied and this value is
			  used as the error message
			- if not present, normal checks are done.

JGREYLIST_DIR		- root directory where files are stored. REQUIRED.

JGREYLIST_NOREV		- if not empty, connections from IP addresses with no
			  reverse DNS will be denied, and this value will be
			  used as the error message for those connections.

JGREYLIST_BY_IP		- if non-zero, greylist entries are done for each IP
			  address, rather than for each class-c block.

JGREYLIST_HOLDTIME	- number of seconds a client must wait after the first
			  connection before they can send mail. default 120.
			  if explicitly set to zero, client can re-connect
			  immediately.

JGREYLIST_LOG		- if non-zero, log the disposition of each connection
			  (i.e. allowed, denied, greylisted.) default 1.

JGREYLIST_LOG_PID	- if non-zero, any log entries will include the PID of
			  the jgreylist process. default 1.

JGREYLIST_LOG_SMTP	- if non-zero, the actual SMTP commands and responses
			  will be sent to the log. default 0.

JGREYLIST_TIMEOUT	- maximum number of seconds each connection is allowed
			  to waste in a fake SMTP conversation before jgreylist
			  forcibly disconnects them. range 5-300, default 60.

JGREYLIST_LIMIT		- if non-zero, maximum number of RCPT commands which
			  can be sent before forcibly hanging up. default 0.

******************************************************************************/

const char *JGREYLIST ;
const char *JGREYLIST_DIR ;
const char *JGREYLIST_NOREV ;
const char *TCPREMOTEHOST ;
const char *TCPREMOTEIP ;
int JGREYLIST_BY_IP ;
int JGREYLIST_HOLDTIME ;
int JGREYLIST_LOG ;
int JGREYLIST_LOG_PID ;
int JGREYLIST_LOG_SMTP ;
int JGREYLIST_TIMEOUT ;
int JGREYLIST_LIMIT ;

int getenv_int ( const char *name , int def )
{
	char *x ;

	x = getenv ( name ) ;

	return ( x && *x ) ? atoi ( x ) : def ;
}

void read_env ( void )
{
	JGREYLIST	= getenv ( "JGREYLIST"		) ;
	JGREYLIST_DIR	= getenv ( "JGREYLIST_DIR"	) ;
	JGREYLIST_NOREV	= getenv ( "JGREYLIST_NOREV"	) ;
	TCPREMOTEHOST	= getenv ( "TCPREMOTEHOST"	) ;
	TCPREMOTEIP	= getenv ( "TCPREMOTEIP"	) ;

	JGREYLIST_BY_IP    = getenv_int ( "JGREYLIST_BY_IP"    ,   0 ) ;
	JGREYLIST_HOLDTIME = getenv_int ( "JGREYLIST_HOLDTIME" , 120 ) ;
	JGREYLIST_LOG      = getenv_int ( "JGREYLIST_LOG"      ,   1 ) ;
	JGREYLIST_LOG_PID  = getenv_int ( "JGREYLIST_LOG_PID"  ,   1 ) ;
	JGREYLIST_LOG_SMTP = getenv_int ( "JGREYLIST_LOG_SMTP" ,   0 ) ;
	JGREYLIST_TIMEOUT  = getenv_int ( "JGREYLIST_TIMEOUT"  ,  60 ) ;
	JGREYLIST_LIMIT    = getenv_int ( "JGREYLIST_LIMIT"    ,   0 ) ;

	if ( JGREYLIST_TIMEOUT < 5 )
		JGREYLIST_TIMEOUT = 5 ;
	if ( JGREYLIST_TIMEOUT > 300 )
		JGREYLIST_TIMEOUT = 300 ;
}

/******************************************************************************

error and log handlers

******************************************************************************/

char title[32] = "jgreylist: " ;

void logit4 ( const char *m1 , const char *m2 , const char *m3 , const char *m4 )
{
	fputs ( title , stderr ) ;

	          fputs ( m1    , stderr ) ;
	if ( m2 ) fputs ( m2    , stderr ) ;
	if ( m3 ) fputs ( m3    , stderr ) ;
	if ( m4 ) fputs ( m4    , stderr ) ;

	fputs ( "\n"  , stderr ) ;
}

#define logit3(x1,x2,x3) logit4(x1,x2,x3,NULL)
#define logit2(x1,x2)    logit4(x1,x2,NULL,NULL)
#define logit(x1)        logit4(x1,NULL,NULL,NULL)

void die4 ( const char *m1 , const char *m2 , const char *m3 , const char *m4 )
{
	logit4 ( m1 , m2 , m3 , m4 ) ;
	_exit ( 1 ) ;
}

#define die3(x1,x2,x3) die4(x1,x2,x3,NULL)
#define die2(x1,x2)    die4(x1,x2,NULL,NULL)
#define die(x1)        die4(x1,NULL,NULL,NULL)

void pdie ( const char *msg )
{
	fputs ( title , stderr ) ;
	perror ( msg ) ;
	_exit ( 1 ) ;
}

/******************************************************************************

alarm stuff - handles the timeout for a fake conversation

******************************************************************************/

static int alarm_received = 0 ;

void handle_alarm ( int signum )
{
	alarm_received = 1 ;
	alarm ( 0 ) ;
}

#define BUFLEN 32
char cmdbuf[BUFLEN] = "" ;
void getcmd ( void )
{
	int bufcount , rv ;
	char ch ;

	bufcount = 0 ;

	while ( ! alarm_received )
	{
		rv = read ( 0 , &ch , 1 ) ;
		if ( -1 == rv )
		{
			if ( errno == EINTR )
				break ;
			pdie ( "read() failed" ) ;
		}
		else if ( 0 == rv )
			break ;

		if ( '\n' == ch )
			break ;

		if ( (bufcount+1) < BUFLEN )
			cmdbuf[bufcount++] = ch ;
	}

	cmdbuf[bufcount] = '\0' ;
}

/******************************************************************************

send a message to the client

******************************************************************************/

void out3 ( const char *m1 , const char *m2 , const char *m3 )
{
	if ( JGREYLIST_LOG_SMTP )
		logit4 ( ">>> " , m1 , m2 , m3 ) ;

	          fputs ( m1 , stdout ) ;
	if ( m2 ) fputs ( m2 , stdout ) ;
	if ( m3 ) fputs ( m3 , stdout ) ;

	fputs ( "\r\n" , stdout ) ;
	fflush ( stdout ) ;
}

#define out2(x1,x2) out3(x1,x2,NULL)
#define out(x1) out3(x1,NULL,NULL)

/******************************************************************************

fake SMTP conversation

is_grey: 0 for deny, 1 for greylist

******************************************************************************/

void fake ( int is_grey , const char *pmsg )
{
	int ml , didsub , rcpt_count ;
	char *emsg , *e ;
	const char *p , *t ;
	struct sigaction sa ;

	/***************************************
	* build "emsg", the message which will be sent to clients
	* in response to any commands they may send. this is built
	* from the parameter, with the first '%' replaced with the
	* value from client's IP address.
	*/

	if ( ! TCPREMOTEIP )
		TCPREMOTEIP = "(?)" ;

	ml = strlen ( pmsg ) + strlen ( TCPREMOTEIP ) + 1 ;
	emsg = malloc ( ml ) ;
	if ( ! emsg )
		die ( "out of memory" ) ;

	e = emsg ;
	p = pmsg ;
	t = TCPREMOTEIP ;
	didsub = 0 ;
	while ( *p )
	{
		if ( !didsub )
		{
			if ( *p == '%' )
			{
				while ( *t )
					*e++ = *t++ ;
				didsub = 1 ;
				p++ ;
			}
			else
				*e++ = *p++ ;
		}
		else
			*e++ = *p++ ;
	}
	*e = '\0' ;

	/***************************************
	* set an alarm for the conversation's timeout
	*/

	signal ( SIGALRM , handle_alarm ) ;
	alarm ( JGREYLIST_TIMEOUT ) ;

	/***************************************
	* make read() return EINTR when SIGALRM strikes
	*/

	sigaction ( SIGALRM , NULL , &sa ) ;
	sa.sa_flags &= ~SA_RESTART ;
	sigaction ( SIGALRM , &sa , NULL ) ;

	/***************************************
	* start the fake conversation
	*/

	rcpt_count = 0 ;

	out ( "220 " BANNER ) ;

	while ( 1 )
	{
		getcmd() ;

		if ( alarm_received )
			break ;

		if ( JGREYLIST_LOG_SMTP )
			logit2 ( "<<< " , cmdbuf ) ;

		/***************************************
		* we only need the first word
		* also convert to uppercase for later comparison
		*/

		e = cmdbuf ;
		while ( *e && ! isspace ( *e ) )
		{
			*e = toupper ( *e ) ;
			e++ ;
		}
		*e = '\0' ;

		/***************************************
		* if the command is QUIT, do so
		*/

		if ( ! strcmp ( cmdbuf , "QUIT" ) )
		{
			alarm ( 0 ) ;
			alarm_received = 0 ;
			out ( "221 " BANNER ) ;
			break ;
		}

		/***************************************
		* if the command is on this list, show the banner again
		*/

		if (    ( ! strcmp ( cmdbuf , "HELO" ) )
		     || ( ! strcmp ( cmdbuf , "EHLO" ) )
		     || ( ! strcmp ( cmdbuf , "MAIL" ) )
		     || ( ! strcmp ( cmdbuf , "RSET" ) ) )
		{
			out ( "250 " BANNER ) ;
			continue ;
		}

		/***************************************
		* possible special handling of RCPT commands
		*/

		if ( JGREYLIST_LIMIT && ( ! strcmp ( cmdbuf , "RCPT" ) ) )
		{
			rcpt_count ++ ;
			if ( rcpt_count >= JGREYLIST_LIMIT )
			{
				logit2 ( TCPREMOTEIP ,
					": HANGUP too many RCPT commands" ) ;
				out ( "421 " TOOMANY ) ;
				break ;
			}
		}

		/***************************************
		* otherwise we show the error message
		* note this includes RCPT, if we're not counting
		*   or if the limit was not reached yet
		*/

		char *e = is_grey ? "450 GREYLIST " : "553 DENIED " ;
		out2 ( e , emsg ) ;
	}

	alarm ( 0 ) ;

	/***************************************
	* outta here
	*/

	if ( alarm_received )
		out ( "421 timeout" ) ;

	free ( emsg ) ;
	_exit ( 0 ) ;
}

/******************************************************************************

run the next program listed on the command line

******************************************************************************/

void okay ( int argc , char **argv )
{
	execvp ( argv[1] , &(argv[1]) ) ;

	perror ( "jgreylist: exec failed" ) ;
	_exit ( 1 ) ;
}

/******************************************************************************

file modification functions

******************************************************************************/

void file_create ( const char *filename )
{
	int fh ;

	fh = creat ( filename , 0600 ) ;
	if ( -1 == fh )
		pdie ( "file_create(): creat() failed" ) ;
	close ( fh ) ;
}

void file_touch ( const char *filename , time_t newtime )
{
	struct stat sb ;
	struct utimbuf ut ;

	if ( stat ( filename , &sb ) )
		pdie ( "file_touch(): stat() failed" ) ;

	ut.actime = newtime ;
	ut.modtime = sb.st_mtime ;

	if ( utime ( filename , &ut ) )
		pdie ( "file_touch(): utime() failed" ) ;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************

this is where it all begins

*******************************************************************************
*******************************************************************************
******************************************************************************/

int main ( int argc , char *argv[] )
{
	int now , a , b , c , d ;
	struct stat sb_dir ;
	struct stat sb_file ;
	char *filename , *ip1 , *ip2 , *ip3 , *ip4 ;
	char ipbuf[16] ;

	read_env() ;

	if ( JGREYLIST_LOG_PID )
		snprintf ( title , 31 , "jgreylist[%d]: " , getpid() ) ;

	/***************************************
	* make sure we have a command to run
	*  if we decide to allow the connection
	*/

	if ( argc < 2 )
		die ( "no command specified on command line" ) ;

	/***************************************
	* make sure we have an IP address to check
	*/

	if ( ! ( TCPREMOTEIP && *TCPREMOTEIP ) )
		die ( "TCPREMOTEIP is empty or missing" ) ;

	/***************************************
	* if JGREYLIST is set, either approve or deny automatically
	*/

	if ( JGREYLIST )
	{
		if ( *JGREYLIST )
		{
			if ( JGREYLIST_LOG )
				logit2 ( TCPREMOTEIP , ": DENY blacklisted" ) ;
			fake ( 0 , JGREYLIST ) ;
		}
		if ( JGREYLIST_LOG )
			logit2 ( TCPREMOTEIP , ": OK whitelisted" ) ;
		okay ( argc , argv ) ;
	}

	if ( JGREYLIST_NOREV && *JGREYLIST_NOREV )
		if ( ! ( TCPREMOTEHOST && *TCPREMOTEHOST ) )
		{
			if ( JGREYLIST_LOG )
				logit2 ( TCPREMOTEIP , ": DENY no reverse DNS" ) ;
			fake ( 0 , JGREYLIST_NOREV ) ;
		}

	/***************************************
	* make sure the JGREYLIST_DIR directory exists and is writable
	*/

	if ( ! ( JGREYLIST_DIR && *JGREYLIST_DIR ) )	/* defined */
		die ( "JGREYLIST_DIR is not set" ) ;

	if ( stat ( JGREYLIST_DIR , &sb_dir ) )		/* exists */
		pdie ( JGREYLIST_DIR ) ;

	if ( ! ( sb_dir.st_mode & S_IFDIR ) )		/* is a directory */
		die2 ( JGREYLIST_DIR , " is not a directory" ) ;

	if ( access ( JGREYLIST_DIR , R_OK | W_OK | X_OK ) )
		pdie ( JGREYLIST_DIR ) ;

	/***************************************
	* copy the IP, break it into four separate segments
	*/

	strncpy ( ipbuf , TCPREMOTEIP , 16 ) ;
	ipbuf[15] = '\0' ; /* just in case */
	ip1 = ip2 = ipbuf ;
	while ( *ip2 && *ip2 != '.' ) ip2++ ;
	if ( *ip2 ) *ip2++ = '\0' ;
	ip3 = ip2 ;
	while ( *ip3 && *ip3 != '.' ) ip3++ ;
	if ( *ip3 ) *ip3++ = '\0' ;
	ip4 = ip3 ;
	while ( *ip4 && *ip4 != '.' ) ip4++ ;
	if ( *ip4 ) *ip4++ = '\0' ;

	/***************************************
	* change "1.2.3.4" to "001.002.003.004"
	*/

	a = atoi ( ip1 ) ;
	b = atoi ( ip2 ) ;
	c = atoi ( ip3 ) ;
	d = atoi ( ip4 ) ;

	snprintf ( ipbuf , 16 , "%03d.%03d.%03d.%03d" , a , b , c , d ) ;

	/***************************************
	* break it into separate strings for each octet
	*/

	ipbuf[3] = ipbuf[7] = ipbuf[11] = '\0' ;
	ip2 = &(ipbuf[4]) ;
	ip3 = &(ipbuf[8]) ;
	ip4 = &(ipbuf[12]) ;

	/***************************************
	* walk into the directory structure, creating directories as we go.
	* then reassemble the portions of the IP address which will become
	* the actual filename within the directory, and build the final
	* filename whose timestamps we need.
	*/

	filename = malloc ( strlen ( JGREYLIST_DIR ) + 32 ) ;
	if ( ! filename )
		die ( "out of memory" ) ;

	strcpy ( filename , JGREYLIST_DIR ) ;
	strcat ( filename , "/" ) ;
	strcat ( filename , ip1 ) ;
	if ( mkdir ( filename , 0700 ) && errno != EEXIST )
		pdie ( filename ) ;
	strcat ( filename , "/" ) ;
	strcat ( filename , ip2 ) ;
	if ( mkdir ( filename , 0700 ) && errno != EEXIST )
		pdie ( filename ) ;
	strcat ( filename , "/" ) ;
	strcat ( filename , ip3 ) ;

	if ( JGREYLIST_BY_IP )
	{
		if ( mkdir ( filename , 0700 ) && errno != EEXIST )
			pdie ( filename ) ;
		strcat ( filename , "/" ) ;
		strcat ( filename , ip4 ) ;
	}

	/***************************************
	* try to get the file's timestamps
	* if not exist, create it and fake conversation first time
	* if other error, die
	*/

	if ( stat ( filename , &sb_file ) )
	{
		if ( errno == ENOENT )
		{
			if ( JGREYLIST_LOG )
				logit2 ( TCPREMOTEIP , ": GREY first time" ) ;
			file_create ( filename ) ;
			fake ( 1 , GREY1 ) ;
		}
		pdie ( filename ) ;
	}

	/***************************************
	* is it too soon to let them in?
	*/

	now = time ( 0 ) ;

	if ( ( sb_file.st_mtime + JGREYLIST_HOLDTIME ) > now )
	{
		if ( JGREYLIST_LOG )
			if ( JGREYLIST_LOG )
				logit2 ( TCPREMOTEIP , ": GREY too soon" ) ;
		file_touch ( filename , now ) ;
		fake ( 2 , GREY2 ) ;
	}

	/***************************************
	* they're okay
	*/

	if ( JGREYLIST_LOG )
		logit2 ( TCPREMOTEIP , ": OK known" ) ;
	file_touch ( filename , now ) ;
	okay ( argc , argv ) ;

	return 0 ;
}
