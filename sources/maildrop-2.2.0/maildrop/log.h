#ifndef	log_h
#define	log_h

static const char log_h_rcsid[]="$Id: log.h,v 1.1 1998/04/17 00:08:53 mrsam Exp $";

////////////////////////////////////////////////////////////////////////
//
// Log file support is too lame to be encapsulate.  Just two function
// calls will do.
//
////////////////////////////////////////////////////////////////////////

void log(const char *mailbox, int status, class FormatMbox &);
void log_line(const class Buffer &);

#endif
