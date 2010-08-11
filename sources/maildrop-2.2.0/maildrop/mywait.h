#ifndef	mywait_h
#define	mywait_h

static const char mywait_h_rcsid[]="$Id: mywait.h,v 1.2 1999/03/31 07:30:03 mrsam Exp $";

#include	"config.h"
#include	<sys/types.h>
#if HAVE_SYS_WAIT_H
#include	<sys/wait.h>
#endif
#ifndef WEXITSTATUS
#define	WEXITSTATUS(stat_val)	((unsigned)(stat_val) >> 8)
#endif
#ifndef	WIFEXITED
#define	WIFEXITED(stat_val)	(((stat_val) & 255) == 0)
#endif

#endif
