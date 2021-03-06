#ifndef	exittrap_h
#define	exittrap_h

static const char exittrap_h_rcsid[]="$Id: exittrap.h,v 1.1 1998/04/17 00:08:53 mrsam Exp $";

//////////////////////////////////////////////////////////////////////////
//
// ExitTrap implements exit traps - cleanup functions that must be called
// in case of an abnormal program termination.
//
// This class does NOT do anything like trap signals, etcetera.  The main
// program should do that, and call onexit() in order to call the cleanup()
// virtual function.
//
//////////////////////////////////////////////////////////////////////////

class ExitTrap {

	ExitTrap	*next, *prev;
	virtual void	cleanup()=0;
	virtual void	forked()=0;
	int	callcleanup;
protected:
	void	destroying() { callcleanup=0; }
	void	constructed() { callcleanup=1; }
public:
	ExitTrap();
	virtual ~ExitTrap();

static	void	onexit();
static	void	onfork();
} ;
#endif
