/*
 * qregex (v2)
 * $Id: qregex.c,v 2.1 2001/12/28 07:05:21 evan Exp $
 *
 * Author  : Evan Borgstrom (evan at unixpimps dot org)
 * Created : 2001/12/14 23:08:16
 * Modified: $Date: 2001/12/28 07:05:21 $
 * Revision: $Revision: 2.1 $
 *
 * Do POSIX regex matching on addresses for anti-relay / spam control.
 * It logs to the maillog
 * See the qregex-readme file included with this tarball.
 * If you didn't get this file in a tarball please see the following URL:
 *  http://www.unixpimps.org/software/qregex
 *
 * qregex.c is released under a BSD style copyright.
 * See http://www.unixpimps.org/software/qregex/copyright.html
 *
 * Note: this revision follows the coding guidelines set forth by the rest of
 *       the qmail code and that described at the following URL.
 *       http://cr.yp.to/qmail/guarantee.html
 * 
 */

#include <sys/types.h>
#include <regex.h>
#include "qregex.h"

#define REGCOMP(X,Y)    regcomp(&X, Y, REG_EXTENDED|REG_ICASE)
#define REGEXEC(X,Y)    regexec(&X, Y, (size_t)0, (regmatch_t *)0, (int)0)

int matchregex(char *text, char *regex) {
  regex_t qreg;
  int retval = 0;


  /* build the regex */
  if ((retval = REGCOMP(qreg, regex)) != 0) {
    regfree(&qreg);
    return(-retval);
  }

  /* execute the regex */
  if ((retval = REGEXEC(qreg, text)) != 0) {
    /* did we just not match anything? */
    if (retval == REG_NOMATCH) {
      regfree(&qreg);
      return(0);
    }
    regfree(&qreg);
    return(-retval);
  }

  /* signal the match */
  regfree(&qreg);
  return(1);
}
