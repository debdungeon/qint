#ifndef	varlist_h
#define	varlist_h

static const char varlist_h_rcsid[]="$Id: varlist.h,v 1.2 2004/01/15 03:12:13 mrsam Exp $";

//
// Quick hack to implement variables - get them and set them.
//

class Buffer;

void UnsetVar(const Buffer &);
void SetVar(const Buffer &, const Buffer &);
const Buffer *GetVar(const Buffer &);
const char *GetVarStr(const Buffer &);
char **ExportEnv();
#endif
