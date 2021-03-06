#ifndef	funcs_h
#define	funcs_h

static const char funcs_h_rcsid[]="$Id: funcs.h,v 1.2 1999/03/31 07:30:03 mrsam Exp $";

#include	"config.h"
#include	"maildrop.h"

#define	SLASH_CHAR	'/'

// Miscellaneous functions that do not fit anywhere else.

void	memorycopy(void *dst, void *src, int cnt);
			// Function copies block of memory that may overlap

void	outofmem();	// Throw an out of memory error message, and die.
void	seekerr();	// Throw a bad seek error message, and die.

extern int verbose_level;
#define	VerboseLevel()	maildrop.verbose_level
const char *GetDefaultMailbox(const char *);
int	delivery(const char *);
int	filter(const char *);
void	subshell(const char *);
const char *TempName();	// Return temporary filename
const char *TempName(const char *, unsigned=0);	// ... with this prefix.

int backslash_char(int);
#endif
