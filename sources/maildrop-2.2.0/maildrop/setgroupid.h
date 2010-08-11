#ifndef	setgroupid_h
#define	setgroupid_h

static const char setgroupid_h_rcsid[]="$Id: setgroupid.h,v 1.2 1999/03/31 07:30:03 mrsam Exp $";

#include	"config.h"
#include	<sys/types.h>
#if	HAVE_UNISTD_H
#include	<unistd.h>
#endif

#define	__USE_BSG
#include	<grp.h>

#ifdef  __cplusplus

extern "C"

#endif

	void	setgroupid(gid_t grpid);

#endif
