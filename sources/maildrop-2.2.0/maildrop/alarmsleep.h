#ifndef	alarmsleep_h
#define	alarmsleep_h

static const char alarmsleep_h_rcsid[]="$Id: alarmsleep.h,v 1.1 1998/04/17 00:08:53 mrsam Exp $";

#include	"alarm.h"

///////////////////////////////////////////////////////////////////////////
//
//  When using the Alarm objects, sleep() can't be used because it mucks
//  around with the ALRM signal by itself.  Instead, provide the AlarmSleep()
//  function (an object, really), in order to be able to sleep for a defined
//  number of seconds (an approximation, really).
//
///////////////////////////////////////////////////////////////////////////

class AlarmSleep: public Alarm {

	void	handler();
	int	flag;
public:
	AlarmSleep(unsigned);
	~AlarmSleep();
} ;

#endif
