#include	"rematchstr.h"

static const char rcsid[]="$Id: rematchstr.C,v 1.1 1998/04/16 23:53:22 mrsam Exp $";


ReMatchStr::~ReMatchStr()
{
}

int ReMatchStr::NextChar()
{
	return (*pos == 0 ? -1: (int)(unsigned char)*pos++);
}

int ReMatchStr::CurrentChar()
{
	return (*pos == 0 ? -1: (int)(unsigned char)*pos);
}

off_t ReMatchStr::GetCurrentPos()
{
	return (pos-str);
}

void	ReMatchStr::SetCurrentPos(off_t p)
{
	pos=str+p;
}
