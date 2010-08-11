#ifndef	deliverdotlock_h
#define	deliverdotlock_h

static const char deliverdotlock_h_rcsid[]="$Id: deliverdotlock.h,v 1.3 1999/03/31 07:30:03 mrsam Exp $";

////////////////////////////////////////////////////////////////////////////
//
// When delivering to a mailbox, and there's an exception, the mailbox
// must be truncated BEFORE the dot lock is released.  Override the cleanup()
// virtual function to do the truncation first.
//
////////////////////////////////////////////////////////////////////////////

#include	"dotlock.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"config.h"

class DeliverDotLock : public DotLock {

	void	cleanup();

	int	truncate_fd;
	off_t	truncate_size;
public:
	DeliverDotLock();
	~DeliverDotLock();

	void	trap_truncate(int f, off_t size)
		{
			truncate_size=size;
			truncate_fd=f;
		}
	void	remove_trap() { truncate_fd= -1; }
	void	truncate();
} ;
#endif
