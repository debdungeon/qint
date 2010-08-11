#ifndef	rematchstr_h
#define	rematchstr_h

static const char rematchstr_h_rcsid[]="$Id: rematchstr.h,v 1.2 1999/03/31 07:30:03 mrsam Exp $";

#include	<sys/types.h>
#include	"config.h"
#include	"rematch.h"

////////////////////////////////////////////////////////////////////////////
//
// ReMatchStr - derive from ReMatch when text matched against a regular
// expression comes from the message body itself.
//
////////////////////////////////////////////////////////////////////////////

class ReMatchStr : public ReMatch {

	const char *str;
	const char *pos;

public:
	ReMatchStr(const char *p) : str(p), pos(p)	{}
	~ReMatchStr();

	int	NextChar();
	int	CurrentChar();
	off_t GetCurrentPos();
	void	SetCurrentPos(off_t);
} ;
#endif
