#ifndef	alarmtimer_h
#define	alarmtimer_h

static const char alarmtimer_h_rcsid[]="$Id: alarmtimer.h,v 1.1 1998/04/17 00:08:53 mrsam Exp $";

#include	"alarm.h"

///////////////////////////////////////////////////////////////////////////
//
//  This is mainly used by DotLock to implement a dotlock timeout.
//
///////////////////////////////////////////////////////////////////////////

class AlarmTimer: public Alarm {

	void	handler();
	int	flag;
public:
	AlarmTimer();
	void	Set(unsigned);
	~AlarmTimer();
	int	Expired() { return (flag); }
} ;

#endif
