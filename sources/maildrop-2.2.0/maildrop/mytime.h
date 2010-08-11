#ifndef	mytime_h
#define	mytime_h

static const char mytime_h_rcsid[]="$Id: mytime.h,v 1.1 1998/04/17 00:08:53 mrsam Exp $";

#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#endif
