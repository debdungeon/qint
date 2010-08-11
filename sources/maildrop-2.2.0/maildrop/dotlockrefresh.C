#include	"dotlockrefresh.h"
#include	"dotlock.h"
#include	"xconfig.h"

static const char rcsid[]="$Id: dotlockrefresh.C,v 1.2 1999/03/31 07:30:03 mrsam Exp $";

DotLockRefresh::DotLockRefresh(DotLock *p) : dotlock(p)
{
}

DotLockRefresh::~DotLockRefresh()
{
	Cancel();
}

void DotLockRefresh::handler()
{
	dotlock->dorefresh();
}
