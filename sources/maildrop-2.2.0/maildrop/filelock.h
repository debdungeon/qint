#ifndef	filelock_h
#define	filelock_h

static const char filelock_h_rcsid[]="$Id: filelock.h,v 1.2 1998/08/31 03:54:50 mrsam Exp $";

#include	"exittrap.h"

/////////////////////////////////////////////////////////////////////////////
//
// Encapsulate the flock() system call.  By encapsulating the system call
// in a class, this allows automatic cleanup if, for some reason, an
// exception is thrown while a lock is being held.
//
/////////////////////////////////////////////////////////////////////////////

class	FileLock : public ExitTrap {

	void	cleanup();
	void	forked();

	int	fd;
public:
	FileLock();
	virtual ~FileLock();
	void	Lock(const char *);
	void	UnLock();

static	void	do_filelock(int);
} ;

#endif
