#include "scan.h"

unsigned int scan_ushort(s,u) register char *s; register unsigned short *u;
{
  register unsigned int pos; unsigned long result;
  pos = scan_ulong(s,&result);
  *u = result; return pos;
}
