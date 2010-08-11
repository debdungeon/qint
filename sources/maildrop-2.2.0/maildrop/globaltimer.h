#ifndef	globaltimer_h
#define	globaltimer_h

static const char globaltimer_h_rcsid[]="$Id: globaltimer.h,v 1.1 1998/04/17 00:08:53 mrsam Exp $";

#include	"alarm.h"

///////////////////////////////////////////////////////////////////////////
//
//  This is the global timer used to terminate maildrop
//
///////////////////////////////////////////////////////////////////////////

class GlobalTimer: public Alarm {

	void	handler();
public:
	GlobalTimer();
	~GlobalTimer();
} ;

#endif
