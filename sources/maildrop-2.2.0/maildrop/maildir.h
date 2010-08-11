#ifndef	maildir_h
#define	maildir_h

static const char maildir_h_rcsid[]="$Id: maildir.h,v 1.4 2004/01/15 03:12:13 mrsam Exp $";

///////////////////////////////////////////////////////////////////////
//
//  Message delivery to maildir directories.
//
///////////////////////////////////////////////////////////////////////

#include	"buffer.h"
#include	<sys/types.h>

class	Mio;

class Maildir {
	int	is_open;
	int	is_afs;
	Buffer	maildirRoot;
public:
	Buffer	tmpname;
	Buffer	newname;

	Maildir();
	virtual ~Maildir();

static	int	IsMaildir(const char *);	// Is this a Maildir directory?
	int	MaildirOpen(const char *, Mio &, off_t);
	void	MaildirSave();
	void	MaildirAbort();
} ;
#endif

