#include	"token.h"

static const char rcsid[]="$Id: token.C,v 1.6 2004/01/15 03:12:13 mrsam Exp $";

static const char *names[]={
		"error",
		"eof",
		"\"...\"",
		"'...'",
		"`...`",
		"=",
		"{",
		"}",
		";",
		"/.../",
                "+",
		"-",
		"*",
		"/",
		"<",
		"<=",
		">",
		">=",
		"==",
		"!=",
		"||",
		"&&",
		"|",
		"&",
		"(",
		")",
		"!",
		"~",
		"lt",
		"le",
		"gt",
		"ge",
		"eq",
		"ne",
		"length",
		"substr",
		",",
		"=~",
		"if",
		"else",
		"while",
		"to",
		"cc",
		"exception",
		"echo",
		"xfilter",
		"dotlock",
		"flock",
		"logfile",
		"log",
		"include",
		"exit",
		"foreach",
		"getaddr",
		"lookup",
		"escape",
		"tolower",
		"toupper",
		"hasaddr",
		"gdbmopen",
		"gdbmclose",
		"gdbmfetch",
		"gdbmstore",
		"time",
		"import",
		"unset"
		} ;

static Buffer namebuf;

const char *Token::Name()
{
	if (type == qstring)
	{
		namebuf="string: \"";
		namebuf += buf;
		namebuf += "\"";
		namebuf.push(0);
		return (namebuf);
	}

	if (type == sqstring)
	{
		namebuf="string: '";
		namebuf += buf;
		namebuf += "'";
		namebuf.push(0);
		return (namebuf);
	}

	if (type == btstring)
	{
		namebuf="string: `";
		namebuf += buf;
		namebuf += "`";
		namebuf.push(0);
		return (namebuf);
	}

	if (type == regexpr)
	{
		namebuf="regexp: ";
		namebuf += buf;
		namebuf.push(0);
		return (namebuf);
	}

unsigned	t=(unsigned)type;

	if (t >= sizeof(names)/sizeof(names[0]))	t=0;
	return (names[t]);
}
