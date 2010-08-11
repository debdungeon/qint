#include	"setgroupid.h"

static const char rcsid[]="$Id: setgroupid.c,v 1.1 1998/04/16 23:53:22 mrsam Exp $";

void	setgroupid(gid_t grpid)
{
gid_t g=grpid;

#if	HAVE_SETGROUPS
	setgroups(1, &g);
#endif

	setgid(g);
}
