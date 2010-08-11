/*
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc, char *argv)
{
 FILE *fs;
 DIR *mydir;
 struct dirent *mydirent;
 struct stat mystatbuf;
 int BigTodo = 0;
 int ConfSplit = 0;

    fs = fopen("qmailmrtg7.h", "w+");
    chdir("/var/qmail/queue/todo");
    mydir = opendir(".");
    while((mydirent=readdir(mydir))!=NULL) {
      if ( strcmp(mydirent->d_name,".")==0 || 
           strcmp(mydirent->d_name,"..")==0 ) continue;

      stat( mydirent->d_name, &mystatbuf );
      if ( S_ISDIR(mystatbuf.st_mode) ) { 
        BigTodo = 1;
      }
    }
    closedir(mydir);
    fprintf(fs, "int BigTodo=%d;\n", BigTodo);

    chdir("/var/qmail/queue/mess");
    mydir = opendir(".");
    while((mydirent=readdir(mydir))!=NULL) {
      if ( strcmp(mydirent->d_name,".")==0 || 
           strcmp(mydirent->d_name,"..")==0 ) continue;

      stat( mydirent->d_name, &mystatbuf );
      if ( S_ISDIR(mystatbuf.st_mode) ) { 
        ++ConfSplit; 
      }
    }
    closedir(mydir);
    fprintf(fs, "int ConfSplit=%d;\n", ConfSplit);

    fclose(fs);

    return(0);
}          
