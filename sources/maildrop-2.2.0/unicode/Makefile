# Makefile.in generated by automake 1.11 from Makefile.am.
# Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
# 2003, 2004, 2005, 2006, 2007, 2008, 2009  Free Software Foundation,
# Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



# $Id: Makefile.am,v 1.25 2008/07/20 16:24:52 mrsam Exp $
#
# Copyright 2000-2008 Double Precision, Inc.  See COPYING for
# distribution information.




pkgdatadir = $(datadir)/unicode
pkgincludedir = $(includedir)/unicode
pkglibdir = $(libdir)/unicode
pkglibexecdir = $(libexecdir)/unicode
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
noinst_PROGRAMS = unicode-info$(EXEEXT)
subdir = .
DIST_COMMON = README $(am__configure_deps) $(srcdir)/../config.guess \
	$(srcdir)/../config.sub $(srcdir)/../depcomp \
	$(srcdir)/../install-sh $(srcdir)/../ltmain.sh \
	$(srcdir)/../missing $(srcdir)/Makefile.am \
	$(srcdir)/Makefile.in $(srcdir)/unicode_config.h.in \
	$(top_srcdir)/configure
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/configure.in
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
am__CONFIG_DISTCLEAN_FILES = config.status config.cache config.log \
 configure.lineno config.status.lineno
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = unicode_config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
LTLIBRARIES = $(noinst_LTLIBRARIES)
libunicode_la_LIBADD =
am__objects_1 = iso88591.lo iso88592.lo iso88593.lo iso88594.lo \
	iso88595.lo iso88596.lo iso88597.lo iso88598.lo iso88599.lo \
	iso885910.lo iso885913.lo iso885914.lo iso885915.lo
am__objects_2 = windows1250.lo windows1251.lo windows1252.lo \
	windows1253.lo windows1254.lo windows1255.lo windows1256.lo \
	windows1257.lo windows1258.lo windows874.lo
am__objects_3 = ibm437.lo ibm775.lo ibm850.lo ibm852.lo ibm855.lo \
	ibm857.lo ibm860.lo ibm861.lo ibm862.lo ibm863.lo ibm864.lo \
	ibm865.lo ibm866.lo ibm869.lo
am_libunicode_la_OBJECTS = iso8859.lo windows874u.lo unicode.lo \
	unicode2.lo iso8859_convert.lo ibm864convert.lo utf7imap.lo \
	xtest.lo unicode_ultcase.lo utf8.lo utf8_chset.lo utf7.lo \
	koi8r.lo koi8u.lo unicode_ultcasetab.lo $(am__objects_1) \
	$(am__objects_2) $(am__objects_3) big5.lo gb2312.lo \
	iso2022jp.lo shiftjis.lo eucjp.lo ksx1001.lo \
	unicode_wcwidth.lo charsetlist.lo
libunicode_la_OBJECTS = $(am_libunicode_la_OBJECTS)
PROGRAMS = $(noinst_PROGRAMS)
am_unicode_info_OBJECTS = unicodeinfo.$(OBJEXT)
unicode_info_OBJECTS = $(am_unicode_info_OBJECTS)
unicode_info_LINK = $(LIBTOOL) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(unicode_info_LDFLAGS) $(LDFLAGS) -o $@
SCRIPTS = $(noinst_SCRIPTS)
DEFAULT_INCLUDES = -I.
depcomp = $(SHELL) $(top_srcdir)/../depcomp
am__depfiles_maybe = depfiles
am__mv = mv -f
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --tag=CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
	--mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --tag=CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
	--mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) \
	$(LDFLAGS) -o $@
SOURCES = $(libunicode_la_SOURCES) $(unicode_info_SOURCES)
DIST_SOURCES = $(libunicode_la_SOURCES) $(unicode_info_SOURCES)
ETAGS = etags
CTAGS = ctags
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)
am__remove_distdir = \
  { test ! -d "$(distdir)" \
    || { find "$(distdir)" -type d ! -perm -200 -exec chmod u+w {} ';' \
         && rm -fr "$(distdir)"; }; }
DIST_ARCHIVES = $(distdir).tar.gz
GZIP_ENV = --best
distuninstallcheck_listfiles = find . -type f -print
distcleancheck_listfiles = find . -type f -print
ACLOCAL = ${SHELL} /opt/qfd-0.1/sources/maildrop/missing --run aclocal-1.11
AMTAR = ${SHELL} /opt/qfd-0.1/sources/maildrop/missing --run tar
AR = ar
AUTOCONF = ${SHELL} /opt/qfd-0.1/sources/maildrop/missing --run autoconf
AUTOHEADER = ${SHELL} /opt/qfd-0.1/sources/maildrop/missing --run autoheader
AUTOMAKE = ${SHELL} /opt/qfd-0.1/sources/maildrop/missing --run automake-1.11
AWK = gawk
CC = gcc
CCDEPMODE = depmode=gcc3
CFLAGS = -g -O2 -Wall
CPP = gcc -E
CPPFLAGS = 
CYGPATH_W = echo
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DSYMUTIL = 
DUMPBIN = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /bin/grep -E
EXEEXT = 
FGREP = /bin/grep -F
GREP = /bin/grep
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
LD = /usr/bin/ld
LDFLAGS = 
LIBOBJS = 
LIBS = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIPO = 
LN_S = ln -s
LTLIBOBJS = 
MAKEINFO = ${SHELL} /opt/qfd-0.1/sources/maildrop/missing --run makeinfo
MKDIR_P = /bin/mkdir -p
NM = /usr/bin/nm -B
NMEDIT = 
OBJDUMP = objdump
OBJEXT = o
OTOOL = 
OTOOL64 = 
PACKAGE = unicode
PACKAGE_BUGREPORT = courier-maildrop@lists.sourceforge.net
PACKAGE_NAME = unicode
PACKAGE_STRING = unicode 0.10
PACKAGE_TARNAME = unicode
PACKAGE_VERSION = 0.10
PATH_SEPARATOR = :
PERL = /usr/bin/perl
RANLIB = ranlib
SED = /bin/sed
SET_MAKE = 
SHELL = /bin/bash
STRIP = strip
VERSION = 0.10
abs_builddir = /opt/qfd-0.1/sources/maildrop/unicode
abs_srcdir = /opt/qfd-0.1/sources/maildrop/unicode
abs_top_builddir = /opt/qfd-0.1/sources/maildrop/unicode
abs_top_srcdir = /opt/qfd-0.1/sources/maildrop/unicode
ac_ct_CC = gcc
ac_ct_DUMPBIN = 
am__include = include
am__leading_dot = .
am__quote = 
am__tar = ${AMTAR} chof - "$$tardir"
am__untar = ${AMTAR} xf -
bindir = ${exec_prefix}/bin
build = i686-pc-linux-gnu
build_alias = 
build_cpu = i686
build_os = linux-gnu
build_vendor = pc
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = /usr/local
host = i686-pc-linux-gnu
host_alias = 
host_cpu = i686
host_os = linux-gnu
host_vendor = pc
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /opt/qfd-0.1/sources/maildrop/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
lt_ECHO = echo
mandir = ${datarootdir}/man
mkdir_p = /bin/mkdir -p
oldincludedir = /usr/include
pdfdir = ${docdir}
prefix = /usr/local
program_transform_name = s,x,x,
psdir = ${docdir}
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target_alias = 
top_build_prefix = 
top_builddir = .
top_srcdir = .
unicode = no
unicode_xtest = no
CLEANFILES = charsetlist.c
noinst_SCRIPTS = mkiso8859.pl mkkoi8r.pl mkkoi8u.pl mkibm864.pl big5.pl gb2312.pl \
	update.sh \
	mkultcase.pl unicodefilter.pl charsetlist.pl mkiso8859.pl \
	iso2022jp.pl ksx1001.pl mkeastasianwidth.pl

unicode_info_SOURCES = unicodeinfo.c
unicode_info_DEPENDENCIES = libunicode.la
unicode_info_LDADD = libunicode.la
unicode_info_LDFLAGS = -static
ISO8859CFILES = iso88591.c iso88592.c iso88593.c iso88594.c iso88595.c\
	iso88596.c iso88597.c iso88598.c iso88599.c iso885910.c iso885913.c\
	iso885914.c iso885915.c

WINDOWSCFILES = windows1250.c windows1251.c windows1252.c windows1253.c\
	windows1254.c windows1255.c windows1256.c windows1257.c windows1258.c\
	windows874.c

IBMCFILES = ibm437.c ibm775.c ibm850.c ibm852.c ibm855.c ibm857.c ibm860.c \
	ibm861.c ibm862.c ibm863.c ibm864.c ibm865.c ibm866.c ibm869.c

noinst_LTLIBRARIES = libunicode.la
libunicode_la_SOURCES = unicode.h iso8859.c windows874u.c unicode.c unicode2.c \
			iso8859_convert.c ibm864convert.c \
			utf7imap.c xtest.c \
			unicode_ultcase.c utf8.c utf8_chset.c utf7.c koi8r.c koi8u.c \
			unicode_ultcasetab.c $(ISO8859CFILES) $(WINDOWSCFILES) \
			$(IBMCFILES) big5.c big5.h gb2312.c gb2312.h \
			iso2022jp.c iso2022jp.h shiftjis.c eucjp.c \
			ksx1001.h ksx1001.c \
			unicode_wcwidth.c eastasianwidth.h \
			charsetlist.c

EXTRA_DIST = $(noinst_SCRIPTS) $(ISO8859CFILES) charsetlist.txt
#MAKEISO8859CFILES = dummy1
#MAKEKOIR8 = dummy2
#MAKEKOIU8 = dummy3
all: unicode_config.h
	$(MAKE) $(AM_MAKEFLAGS) all-am

.SUFFIXES:
.SUFFIXES: .c .lo .o .obj
am--refresh:
	@:
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      echo ' cd $(srcdir) && $(AUTOMAKE) --foreign'; \
	      $(am__cd) $(srcdir) && $(AUTOMAKE) --foreign \
		&& exit 0; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --foreign Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --foreign Makefile
.PRECIOUS: Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    echo ' $(SHELL) ./config.status'; \
	    $(SHELL) ./config.status;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck

$(top_srcdir)/configure:  $(am__configure_deps)
	$(am__cd) $(srcdir) && $(AUTOCONF)
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	$(am__cd) $(srcdir) && $(ACLOCAL) $(ACLOCAL_AMFLAGS)
$(am__aclocal_m4_deps):

unicode_config.h: stamp-h1
	@if test ! -f $@; then \
	  rm -f stamp-h1; \
	  $(MAKE) $(AM_MAKEFLAGS) stamp-h1; \
	else :; fi

stamp-h1: $(srcdir)/unicode_config.h.in $(top_builddir)/config.status
	@rm -f stamp-h1
	cd $(top_builddir) && $(SHELL) ./config.status unicode_config.h
$(srcdir)/unicode_config.h.in:  $(am__configure_deps) 
	($(am__cd) $(top_srcdir) && $(AUTOHEADER))
	rm -f stamp-h1
	touch $@

distclean-hdr:
	-rm -f unicode_config.h stamp-h1

clean-noinstLTLIBRARIES:
	-test -z "$(noinst_LTLIBRARIES)" || rm -f $(noinst_LTLIBRARIES)
	@list='$(noinst_LTLIBRARIES)'; for p in $$list; do \
	  dir="`echo $$p | sed -e 's|/[^/]*$$||'`"; \
	  test "$$dir" != "$$p" || dir=.; \
	  echo "rm -f \"$${dir}/so_locations\""; \
	  rm -f "$${dir}/so_locations"; \
	done
libunicode.la: $(libunicode_la_OBJECTS) $(libunicode_la_DEPENDENCIES) 
	$(LINK)  $(libunicode_la_OBJECTS) $(libunicode_la_LIBADD) $(LIBS)

clean-noinstPROGRAMS:
	@list='$(noinst_PROGRAMS)'; test -n "$$list" || exit 0; \
	echo " rm -f" $$list; \
	rm -f $$list || exit $$?; \
	test -n "$(EXEEXT)" || exit 0; \
	list=`for p in $$list; do echo "$$p"; done | sed 's/$(EXEEXT)$$//'`; \
	echo " rm -f" $$list; \
	rm -f $$list
unicode-info$(EXEEXT): $(unicode_info_OBJECTS) $(unicode_info_DEPENDENCIES) 
	@rm -f unicode-info$(EXEEXT)
	$(unicode_info_LINK) $(unicode_info_OBJECTS) $(unicode_info_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c

include ./$(DEPDIR)/big5.Plo
include ./$(DEPDIR)/charsetlist.Plo
include ./$(DEPDIR)/eucjp.Plo
include ./$(DEPDIR)/gb2312.Plo
include ./$(DEPDIR)/ibm437.Plo
include ./$(DEPDIR)/ibm775.Plo
include ./$(DEPDIR)/ibm850.Plo
include ./$(DEPDIR)/ibm852.Plo
include ./$(DEPDIR)/ibm855.Plo
include ./$(DEPDIR)/ibm857.Plo
include ./$(DEPDIR)/ibm860.Plo
include ./$(DEPDIR)/ibm861.Plo
include ./$(DEPDIR)/ibm862.Plo
include ./$(DEPDIR)/ibm863.Plo
include ./$(DEPDIR)/ibm864.Plo
include ./$(DEPDIR)/ibm864convert.Plo
include ./$(DEPDIR)/ibm865.Plo
include ./$(DEPDIR)/ibm866.Plo
include ./$(DEPDIR)/ibm869.Plo
include ./$(DEPDIR)/iso2022jp.Plo
include ./$(DEPDIR)/iso8859.Plo
include ./$(DEPDIR)/iso88591.Plo
include ./$(DEPDIR)/iso885910.Plo
include ./$(DEPDIR)/iso885913.Plo
include ./$(DEPDIR)/iso885914.Plo
include ./$(DEPDIR)/iso885915.Plo
include ./$(DEPDIR)/iso88592.Plo
include ./$(DEPDIR)/iso88593.Plo
include ./$(DEPDIR)/iso88594.Plo
include ./$(DEPDIR)/iso88595.Plo
include ./$(DEPDIR)/iso88596.Plo
include ./$(DEPDIR)/iso88597.Plo
include ./$(DEPDIR)/iso88598.Plo
include ./$(DEPDIR)/iso88599.Plo
include ./$(DEPDIR)/iso8859_convert.Plo
include ./$(DEPDIR)/koi8r.Plo
include ./$(DEPDIR)/koi8u.Plo
include ./$(DEPDIR)/ksx1001.Plo
include ./$(DEPDIR)/shiftjis.Plo
include ./$(DEPDIR)/unicode.Plo
include ./$(DEPDIR)/unicode2.Plo
include ./$(DEPDIR)/unicode_ultcase.Plo
include ./$(DEPDIR)/unicode_ultcasetab.Plo
include ./$(DEPDIR)/unicode_wcwidth.Plo
include ./$(DEPDIR)/unicodeinfo.Po
include ./$(DEPDIR)/utf7.Plo
include ./$(DEPDIR)/utf7imap.Plo
include ./$(DEPDIR)/utf8.Plo
include ./$(DEPDIR)/utf8_chset.Plo
include ./$(DEPDIR)/windows1250.Plo
include ./$(DEPDIR)/windows1251.Plo
include ./$(DEPDIR)/windows1252.Plo
include ./$(DEPDIR)/windows1253.Plo
include ./$(DEPDIR)/windows1254.Plo
include ./$(DEPDIR)/windows1255.Plo
include ./$(DEPDIR)/windows1256.Plo
include ./$(DEPDIR)/windows1257.Plo
include ./$(DEPDIR)/windows1258.Plo
include ./$(DEPDIR)/windows874.Plo
include ./$(DEPDIR)/windows874u.Plo
include ./$(DEPDIR)/xtest.Plo

.c.o:
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(COMPILE) -c $<

.c.obj:
	$(COMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
	$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
#	source='$<' object='$@' libtool=no \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(COMPILE) -c `$(CYGPATH_W) '$<'`

.c.lo:
	$(LTCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
	$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Plo
#	source='$<' object='$@' libtool=yes \
#	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) \
#	$(LTCOMPILE) -c -o $@ $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool config.lt

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	mkid -fID $$unique
tags: TAGS

TAGS:  $(HEADERS) $(SOURCES) unicode_config.h.in $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	set x; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS) unicode_config.h.in $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: CTAGS
CTAGS:  $(HEADERS) $(SOURCES) unicode_config.h.in $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	list='$(SOURCES) $(HEADERS) unicode_config.h.in $(LISP) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '{ files[$$0] = 1; nonempty = 1; } \
	      END { if (nonempty) { for (i in files) print i; }; }'`; \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

distdir: $(DISTFILES)
	$(am__remove_distdir)
	test -d "$(distdir)" || mkdir "$(distdir)"
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
	$(MAKE) $(AM_MAKEFLAGS) \
	  top_distdir="$(top_distdir)" distdir="$(distdir)" \
	  dist-hook
	-test -n "$(am__skip_mode_fix)" \
	|| find "$(distdir)" -type d ! -perm -777 -exec chmod a+rwx {} \; -o \
	  ! -type d ! -perm -444 -links 1 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -400 -exec chmod a+r {} \; -o \
	  ! -type d ! -perm -444 -exec $(install_sh) -c -m a+r {} {} \; \
	|| chmod -R a+r "$(distdir)"
dist-gzip: distdir
	tardir=$(distdir) && $(am__tar) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	$(am__remove_distdir)

dist-bzip2: distdir
	tardir=$(distdir) && $(am__tar) | bzip2 -9 -c >$(distdir).tar.bz2
	$(am__remove_distdir)

dist-lzma: distdir
	tardir=$(distdir) && $(am__tar) | lzma -9 -c >$(distdir).tar.lzma
	$(am__remove_distdir)

dist-xz: distdir
	tardir=$(distdir) && $(am__tar) | xz -c >$(distdir).tar.xz
	$(am__remove_distdir)

dist-tarZ: distdir
	tardir=$(distdir) && $(am__tar) | compress -c >$(distdir).tar.Z
	$(am__remove_distdir)

dist-shar: distdir
	shar $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).shar.gz
	$(am__remove_distdir)

dist-zip: distdir
	-rm -f $(distdir).zip
	zip -rq $(distdir).zip $(distdir)
	$(am__remove_distdir)

dist dist-all: distdir
	tardir=$(distdir) && $(am__tar) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	$(am__remove_distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	case '$(DIST_ARCHIVES)' in \
	*.tar.gz*) \
	  GZIP=$(GZIP_ENV) gunzip -c $(distdir).tar.gz | $(am__untar) ;;\
	*.tar.bz2*) \
	  bunzip2 -c $(distdir).tar.bz2 | $(am__untar) ;;\
	*.tar.lzma*) \
	  unlzma -c $(distdir).tar.lzma | $(am__untar) ;;\
	*.tar.xz*) \
	  xz -dc $(distdir).tar.xz | $(am__untar) ;;\
	*.tar.Z*) \
	  uncompress -c $(distdir).tar.Z | $(am__untar) ;;\
	*.shar.gz*) \
	  GZIP=$(GZIP_ENV) gunzip -c $(distdir).shar.gz | unshar ;;\
	*.zip*) \
	  unzip $(distdir).zip ;;\
	esac
	chmod -R a-w $(distdir); chmod a+w $(distdir)
	mkdir $(distdir)/_build
	mkdir $(distdir)/_inst
	chmod a-w $(distdir)
	test -d $(distdir)/_build || exit 0; \
	dc_install_base=`$(am__cd) $(distdir)/_inst && pwd | sed -e 's,^[^:\\/]:[\\/],/,'` \
	  && dc_destdir="$${TMPDIR-/tmp}/am-dc-$$$$/" \
	  && am__cwd=`pwd` \
	  && $(am__cd) $(distdir)/_build \
	  && ../configure --srcdir=.. --prefix="$$dc_install_base" \
	    $(DISTCHECK_CONFIGURE_FLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) uninstall \
	  && $(MAKE) $(AM_MAKEFLAGS) distuninstallcheck_dir="$$dc_install_base" \
	        distuninstallcheck \
	  && chmod -R a-w "$$dc_install_base" \
	  && ({ \
	       (cd ../.. && umask 077 && mkdir "$$dc_destdir") \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" install \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" uninstall \
	       && $(MAKE) $(AM_MAKEFLAGS) DESTDIR="$$dc_destdir" \
	            distuninstallcheck_dir="$$dc_destdir" distuninstallcheck; \
	      } || { rm -rf "$$dc_destdir"; exit 1; }) \
	  && rm -rf "$$dc_destdir" \
	  && $(MAKE) $(AM_MAKEFLAGS) dist \
	  && rm -rf $(DIST_ARCHIVES) \
	  && $(MAKE) $(AM_MAKEFLAGS) distcleancheck \
	  && cd "$$am__cwd" \
	  || exit 1
	$(am__remove_distdir)
	@(echo "$(distdir) archives ready for distribution: "; \
	  list='$(DIST_ARCHIVES)'; for i in $$list; do echo $$i; done) | \
	  sed -e 1h -e 1s/./=/g -e 1p -e 1x -e '$$p' -e '$$x'
distuninstallcheck:
	@$(am__cd) '$(distuninstallcheck_dir)' \
	&& test `$(distuninstallcheck_listfiles) | wc -l` -le 1 \
	   || { echo "ERROR: files left after uninstall:" ; \
	        if test -n "$(DESTDIR)"; then \
	          echo "  (check DESTDIR support)"; \
	        fi ; \
	        $(distuninstallcheck_listfiles) ; \
	        exit 1; } >&2
distcleancheck: distclean
	@if test '$(srcdir)' = . ; then \
	  echo "ERROR: distcleancheck can only run from a VPATH build" ; \
	  exit 1 ; \
	fi
	@test `$(distcleancheck_listfiles) | wc -l` -eq 0 \
	  || { echo "ERROR: files left in build directory after distclean:" ; \
	       $(distcleancheck_listfiles) ; \
	       exit 1; } >&2
check-am: all-am
check: check-am
all-am: Makefile $(LTLIBRARIES) $(PROGRAMS) $(SCRIPTS) \
		unicode_config.h
installdirs:
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libtool clean-noinstLTLIBRARIES \
	clean-noinstPROGRAMS mostlyclean-am

distclean: distclean-am
	-rm -f $(am__CONFIG_DISTCLEAN_FILES)
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
distclean-am: clean-am distclean-compile distclean-generic \
	distclean-hdr distclean-libtool distclean-tags

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am:

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -f $(am__CONFIG_DISTCLEAN_FILES)
	-rm -rf $(top_srcdir)/autom4te.cache
	-rm -rf ./$(DEPDIR)
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am:

.MAKE: all install-am install-strip

.PHONY: CTAGS GTAGS all all-am am--refresh check check-am clean \
	clean-generic clean-libtool clean-noinstLTLIBRARIES \
	clean-noinstPROGRAMS ctags dist dist-all dist-bzip2 dist-gzip \
	dist-hook dist-lzma dist-shar dist-tarZ dist-xz dist-zip \
	distcheck distclean distclean-compile distclean-generic \
	distclean-hdr distclean-libtool distclean-tags distcleancheck \
	distdir distuninstallcheck dvi dvi-am html html-am info \
	info-am install install-am install-data install-data-am \
	install-dvi install-dvi-am install-exec install-exec-am \
	install-html install-html-am install-info install-info-am \
	install-man install-pdf install-pdf-am install-ps \
	install-ps-am install-strip installcheck installcheck-am \
	installdirs maintainer-clean maintainer-clean-generic \
	mostlyclean mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool pdf pdf-am ps ps-am tags uninstall \
	uninstall-am


update-www:
	@$(MAKE) update-www-unicode
	@$(MAKE) update-www-iso8859
	@$(MAKE) update-www-koi8r
	@$(MAKE) update-www-koi8u
	@$(MAKE) update-www-windows
	@$(MAKE) update-www-dos
	@$(MAKE) update-www-eastasia

update-www-unicode:
	/bin/bash $(srcdir)/update.sh UnicodeData.txt http://www.unicode.org/Public/UNIDATA/UnicodeData.txt

update-www-iso8859:
	for n in 1 2 3 4 5 6 7 8 9 10 13 14 15 ; do \
		/bin/bash $(srcdir)/update.sh 8859-$$n.txt \
		http://www.unicode.org/Public/MAPPINGS/ISO8859/8859-$$n.TXT \
		|| exit 1 ; done

update-www-koi8r:
	/bin/bash $(srcdir)/update.sh koi8-r.txt \
		http://www.unicode.org/Public/MAPPINGS/VENDORS/MISC/KOI8-R.TXT

update-www-koi8u:
	/bin/bash $(srcdir)/update.sh koi8-u.txt \
		http://svn.python.org/projects/python/trunk/Tools/unicode/python-mappings/KOI8-U.TXT

update-www-windows:
	for n in 874 1250 1251 1252 1253 1254 1255 1256 1257 1258 ; do \
		/bin/bash $(srcdir)/update.sh windows-$$n.txt \
			http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP$$n.TXT \
			|| exit 1 ; done

update-www-dos:
	for n in 437 775 850 852 855 857 860 861 862 863 864 865 866 869 ; do \
		/bin/bash $(srcdir)/update.sh ibm-$$n.txt \
			http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/PC/CP$$n.TXT \
			|| exit 1 ; done

update-www-eastasia:
	/bin/bash $(srcdir)/update.sh Unihan-3.2.0.txt.gz http://www.unicode.org/Public/3.2-Update/Unihan-3.2.0.txt.gz
	/bin/bash $(srcdir)/update.sh CP949.TXT http://www.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP949.TXT
	/bin/bash $(srcdir)/update.sh big5-iso.txt http://www.info.gov.hk/digital21/chi/hkscs/download/big5-iso.txt
	/bin/bash $(srcdir)/update.sh EastAsianWidth.txt http://www.unicode.org/Public/UNIDATA/EastAsianWidth.txt

charsetlist.c: charsetlist.txt charsetlist.pl Makefile config.status
	/usr/bin/perl $(srcdir)/unicodefilter.pl "no" \
			"no" <${srcdir}/charsetlist.txt | \
		/usr/bin/perl $(srcdir)/charsetlist.pl >charsetlist.c

dist-hook:
	rm -f $(distdir)/charsetlist.c

#update-csrc:
#	if test -f UnicodeData.txt ; then $(MAKE) "MAKEISO8859CFILES=$(ISO8859CFILES)" ISO8859OFILES=dummy MAKEKOIR8=koi8r.c MAKEKOIU8=koi8u.c $(ISO8859CFILES) koi8r.c koi8u.c ; fi

#$(ISO8859CFILES): UnicodeData.txt mkiso8859.pl
#	@n=`echo $@ | cut -c8- | sed 's/..$$//'` ; \
#		echo /usr/bin/perl $(srcdir)/mkiso8859.pl 8859-$${n}.txt iso8859$${n}_unicode iso8859$${n}_uc iso8859$${n}_lc iso8859$${n}_tc unicode_ISO8859_$${n} ISO-8859-$${n} '>'$@; \
#		/usr/bin/perl $(srcdir)/mkiso8859.pl 8859-$${n}.txt iso8859$${n}_unicode iso8859$${n}_uc iso8859$${n}_lc iso8859$${n}_tc unicode_ISO8859_$${n} ISO-8859-$${n} >$@ && exit 0; rm -f $@

#$(WINDOWSCFILES): UnicodeData.txt mkiso8859.pl
#	@n=`echo $@ | tr -d -c '[0-9]'` ; \
#		echo /usr/bin/perl $(srcdir)/mkiso8859.pl windows-$${n}.txt windows$${n}_unicode windows$${n}_uc windows$${n}_lc windows$${n}_tc unicode_WINDOWS_$${n} WINDOWS-$${n} '>'$@; \
#		/usr/bin/perl $(srcdir)/mkiso8859.pl windows-$${n}.txt windows$${n}_unicode windows$${n}_uc windows$${n}_lc windows$${n}_tc unicode_WINDOWS_$${n} WINDOWS-$${n} >$@ && exit 0; rm -f $@

#$(IBMCFILES): UnicodeData.txt mkiso8859.pl mkibm864.pl
#	@n=`echo $@ | cut -c4-6` ; s=$(srcdir)/mkiso8859.pl; \
#		if test "$$n" = 864 ; then s=$(srcdir)/mkibm864.pl; fi; \
#		echo /usr/bin/perl $$s ibm-$${n}.txt ibm$${n}_unicode ibm$${n}_uc ibm$${n}_lc ibm$${n}_tc unicode_IBM_$${n} IBM$${n} '>'$@; \
#		/usr/bin/perl $$s ibm-$${n}.txt ibm$${n}_unicode ibm$${n}_uc ibm$${n}_lc ibm$${n}_tc unicode_IBM_$${n} IBM$${n} >$@ && exit 0; rm -f $@

#$(MAKEKOIR8): UnicodeData.txt mkkoi8r.pl
#	/usr/bin/perl $(srcdir)/mkkoi8r.pl >koi8r.c

#$(MAKEKOIU8): UnicodeData.txt mkkoi8u.pl
#	/usr/bin/perl $(srcdir)/mkkoi8u.pl >koi8u.c

#unicode_ultcasetab.c: UnicodeData.txt mkultcase.pl
#	/usr/bin/perl $(srcdir)/mkultcase.pl >unicode_ultcasetab.c

#big5.h: Unihan-3.2.0.txt.gz big5.pl big5-iso.txt
#	/usr/bin/perl $(srcdir)/big5.pl >big5.h

#gb2312.h: Unihan-3.2.0.txt.gz gb2312.pl
#	/usr/bin/perl $(srcdir)/gb2312.pl >gb2312.h

#iso2022jp.h: Unihan-3.2.0.txt.gz iso2022jp.pl
#	/usr/bin/perl $(srcdir)/iso2022jp.pl > iso2022jp.h

#ksx1001.h: CP949.TXT ksx1001.pl
#	/usr/bin/perl $(srcdir)/ksx1001.pl > ksx1001.h

#eastasianwidth.h: EastAsianWidth.txt mkeastasianwidth.pl
#	/usr/bin/perl $(srcdir)/mkeastasianwidth.pl >eastasianwidth.h

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
