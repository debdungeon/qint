/*
** Copyright 1998 - 1999 Double Precision, Inc.
** See COPYING for distribution information.
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include	"maildirquota.h"
#include	<stdlib.h>
#include	<string.h>

static const char rcsid[]="$Id: qmail-maildir++.patch,v 1.4 2007-05-22 03:59:04 rwidmer Exp $";

int maildir_parsequota(const char *n, unsigned long *s)
{
const char *o;
int	yes;

	if ((o=strrchr(n, '/')) == 0)	o=n;

	for (; *o; o++)
		if (*o == ':')	break;
	yes=0;
	for ( ; o >= n; --o)
	{
		if (*o == '/')	break;

		if (*o == ',' && o[1] == 'S' && o[2] == '=')
		{
			yes=1;
			o += 3;
			break;
		}
	}
	if (yes)
	{
		*s=0;
		while (*o >= '0' && *o <= '9')
			*s= *s*10 + (*o++ - '0');
		return (0);
	}
	return (-1);
}
