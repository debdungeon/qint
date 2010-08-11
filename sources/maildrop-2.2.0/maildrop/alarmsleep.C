#include	<signal.h>
#include	"alarmsleep.h"

static const char rcsid[]="$Id: alarmsleep.C,v 1.1 1998/04/16 23:53:22 mrsam Exp $";

AlarmSleep::AlarmSleep(unsigned nseconds) : flag(0)
{
	Set(nseconds);
	do
	{
		sigpause(0);
	} while (!flag);
	Cancel();
}

AlarmSleep::~AlarmSleep()
{
}

void AlarmSleep::handler()
{
	flag=1;
	Set(5);	// A possibility of a race condition - try again.
}
