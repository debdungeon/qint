#include	"alarmtimer.h"

static const char rcsid[]="$Id: alarmtimer.C,v 1.1 1998/04/16 23:53:22 mrsam Exp $";


AlarmTimer::AlarmTimer() : flag(0)
{
}

AlarmTimer::~AlarmTimer()
{
}

void AlarmTimer::handler()
{
	flag=1;
}

void AlarmTimer::Set(unsigned nseconds)
{
	Cancel();
	flag=0;
	Alarm::Set(nseconds);
}
