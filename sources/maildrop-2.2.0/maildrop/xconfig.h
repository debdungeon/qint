#ifndef	config_h
#define	config_h

static const char xconfig_h_rcsid[]="$Id: xconfig.h.in,v 1.8 2008/05/08 15:38:46 mrsam Exp $";

/*

  These variables are set by the configure script - DO NOT EDIT

  Directory for temporary files.
*/

#define	TEMPDIR "use tmpfile()"

/*  Temporary files are used for messages larger than SMALLMSG bytes. */

#define	SMALLMSG 8192

/*  Watchdog timer */

#define	GLOBAL_TIMEOUT	300

/*  Terminate line with a CRLF pair. */

#define	CRLF_TERM	0

/*  Initial values for corresponding variables */

#define	LOCKEXT_DEF	".lock"
#define	LOCKSLEEP_DEF	"5"
#define	LOCKTIMEOUT_DEF	"60"
#define	LOCKREFRESH_DEF	"15"

/*  Set to 1 to restrict -d option to trusted users only */

#define	RESTRICT_TRUSTED	1

/*  Set to 1 to keep the original From_ line in message (if any).
    Otherwise, the original From_ line is kept only if invoked by root. */

#define	KEEP_FROMLINE		1

/*  Define the DEFAULT extension for enhanced embedded mode. */

#define	DEFAULTEXT	"-default"

/*  Use flock/lockf locking */

#define	USE_FLOCK	1

/*  Use dotlock locking */

#define	USE_DOTLOCK	0

/*  Automatic version */

#define VERSION		"2.1.0"

#endif
