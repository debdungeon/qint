/* 
 * $Id: forward.c,v 1.2.2.5 2005/01/23 17:35:11 tomcollins Exp $
 * Copyright (C) 1999-2004 Inter7 Internet Technologies, Inc. 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <vpopmail.h>
#include <vauth.h>
#include "alias.h"
#include "config.h"
#include "forward.h"
#include "qmailadmin.h"
#include "qmailadminx.h"
#include "template.h"
#include "show.h"
#include "util.h"

int show_forwards(char *user, char *dom, time_t mytime)
{
  if (AdminType != DOMAIN_ADMIN) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[142]);
    vclose();
    exit(0);
  }

  count_forwards();

  if(CurForwards == 0 && CurBlackholes == 0) {
    snprintf (StatusMessage, sizeof(StatusMessage), "%s", html_text[232]);
    show_menu(Username, Domain, Mytime);
    vclose();
    exit(0);
  } else {
    send_template("show_forwards.html");
  }

  return 0;
}

void count_forwards()
{
 char *alias_line;
 char alias_name[MAX_FILE_NAME];
 char this_alias[MAX_FILE_NAME];
 char *p1, *p2;
 int isforward;

  CurForwards = 0;
  CurBlackholes = 0;

  alias_line = valias_select_all (alias_name, Domain);
  while (alias_line != NULL) {
    strcpy (this_alias, alias_name);
    if (*alias_line == '#') { CurBlackholes++; }
    else {
      isforward = 1;
      while (isforward && (alias_line != NULL) 
        && (strcmp (this_alias, alias_name) == 0)) {
        p1 = strstr (alias_line, "/ezmlm-");
        p2 = strchr (alias_line, ' ');
        if ( (p1 != NULL) && (p2 == NULL || p1 < p2) ) isforward = 0;
        if (strstr (alias_line, "/autorespond ")) isforward = 0;
        alias_line = valias_select_all_next(alias_name);
      }
      if (isforward) CurForwards++;
    }

    /* burn through remaining lines for this alias, if necessary */
    while ((alias_line != NULL) && (strcmp (this_alias, alias_name) == 0)) {
      alias_line = valias_select_all_next(alias_name);
    }
  }

  return;
}

