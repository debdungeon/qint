#ifndef	messageinfo_h
#define	messageinfo_h

static const char messageinfo_h_rcsid[]="$Id: messageinfo.h,v 1.2 1999/03/31 07:30:03 mrsam Exp $";

#include	<sys/types.h>
#include	"config.h"
#include	"buffer.h"

class	Message;

///////////////////////////////////////////////////////////////////////////
//
//  The MessageInfo class collects information about a message - namely
//  it calculates where the message headers actually start in the Message
//  class.  We ignore blank lines and "From " lines at the beginning of
//  the message
//
///////////////////////////////////////////////////////////////////////////

class	MessageInfo {
public:
	off_t msgoffset;	// Skip leading blank lines and From header
	Buffer fromname;	// Envelope sender

	MessageInfo() : msgoffset(0)	{}
	~MessageInfo()			{}

	void	info(Message &);
	void	filtered() { msgoffset=0; }
} ;
#endif
