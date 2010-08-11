/* 
 * $Id: dotqmail.c,v 1.2.2.3 2004/11/20 01:10:41 tomcollins Exp $
 * Copyright (C) 1999-2004 Inter7 Internet Technologies 
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
#include <vpopmail.h>
#include <vpopmail_config.h>
/* undef some macros that get redefined in config.h below */
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#include "config.h"
#include "dotqmail.h"
#include "qmailadmin.h"
#include "qmailadminx.h"

int dotqmail_delete_files(char *user) 
{
	return (! valias_delete (user, Domain));
}
int dotqmail_add_line(char *user, char *line)
{
	return (valias_insert (user, Domain, line));
}
#ifdef VALIAS
int dotqmail_del_line(char *user, char *line)
{
	return (valias_remove (user, Domain, line));
}
#else
#define _(String) gettext(String)

#define MAX_LINE		512

FILE *fr, *fw;
int lr,lw;
char LineBuf[MAX_LINE];
char *mydotqmail_name;
char *dotqmail_bak_name;

int dotqmail_open_files(char *user)
{
 int i,j;

  lr = 0;
  lw = 0;

  if (!user || !strlen(user)) return(1);

  i = strlen(user);
  mydotqmail_name = strcat(strcpy(malloc(8 + i), ".qmail-"), user);
  for(j=8;mydotqmail_name[j]!=0;j++) {
    if (mydotqmail_name[j]=='.') {
      mydotqmail_name[j] = ':';
    }
  }
  dotqmail_bak_name = strcat(strcpy(malloc(8 + i + 4), 
    mydotqmail_name), ".new");

  fr = fopen(mydotqmail_name, "r");
	

  if (!(fw = fopen(dotqmail_bak_name, "w"))) {
    free(mydotqmail_name);
    free(dotqmail_bak_name);
    fclose(fr);
    return(1);
  }
  return(0);
}

void dotqmail_close_files(char *user, int keep)
{
  if (fr) fclose(fr);
  if (fw) fclose(fw);
  if (!keep) {
    rename(dotqmail_bak_name, mydotqmail_name);
    lr = lw;
  } else {
    unlink(dotqmail_bak_name);
  }

  if (!lr) unlink(mydotqmail_name);

  free(mydotqmail_name);
  free(dotqmail_bak_name);
}

int dotqmail_del_line(char *user, char *line)
{
 int exist = 1;
 int i;

  if (dotqmail_open_files(user)) return(1);

  if (fr) {
    while (fgets(LineBuf, sizeof(LineBuf), fr)) {
      i = strlen(line);	

      if (!strncmp(LineBuf, line, i)) {
        exist = 0;
      } else {
        fputs(LineBuf, fw);
        lw++;
      }
      lr++;
    }
  }
  dotqmail_close_files(user, exist);
  return(0);
}

int dotqmail_cleanup(char *user, char *line)
{
 int exist = 0;

  if (dotqmail_open_files(user)) return(1);

  if (fr) {
    while (fgets(LineBuf, sizeof(LineBuf), fr)) {
      if (strncmp(LineBuf, line, sizeof(LineBuf))) {
        exist = 1;
        lw++;
        fputs(LineBuf, fw);
      }
      lr++;
    }
  }
  dotqmail_close_files(user, exist);
  return(0);
}

int dotqmail_count(char *user)
{
  if (dotqmail_open_files(user)) return(1);

  if (fr) {
    while (fgets(LineBuf, sizeof(LineBuf), fr)) {
      lr++;
    }
  }

  dotqmail_close_files(user, 1);
  return(lr);
}
#endif
