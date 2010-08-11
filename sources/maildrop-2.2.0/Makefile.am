# $Id: Makefile.am,v 1.75 2009/06/27 19:13:36 mrsam Exp $
#
# Copyright 1998 - 2009 Double Precision, Inc.  See COPYING for
# distribution information.

@SET_MAKE@

AUTOMAKE_OPTIONS=dist-bzip2

DISTCHECK_CONFIGURE_FLAGS=--enable-maildirquota --enable-userdb

HTML2TXT=`which lynx 2>/dev/null && echo "lynx -dump -nolist" && exit; echo "links -dump"`

SUBDIRS=numlib liblock unicode rfc822 rfc2045 gdbmobj bdbobj makedat maildir maildrop

EXTRA_DIST=README.html INSTALL.html UPGRADE.html README.postfix\
	maildroptips.txt COPYING.GPL manpage.css pgpkeys.txt

if HAVE_SGML
manpage.css: docbook/manpage.css
	cp -f $(srcdir)/docbook/manpage.css manpage.css
endif

scriptdir=$(pkgdatadir)/scripts
htmldir=$(pkgdatadir)/html

DELIVERQUOTAHTMLSOURCE=maildirquota.html deliverquota.html

if INSTALL_DELIVERQUOTA

DELIVERQUOTAHTML=$(DELIVERQUOTAHTMLSOURCE)
DELIVERQUOTAMAN=maildirquota.7 deliverquota.8

#  Install deliverquota into libdir/bin
#
#  maildrop < 0.74 installed bindir/maildrop.deliverquota and created a
#  soft link in bindir from deliverquota to maildrop.deliverquota
#
#  remove maildrop 0.74 installlation, if present.  Starting with maildrop 0.75
#  it is expected that the default login shell will add libdir/bin to PATH,
#  where deliverquota will be installed.
#  Create a soft link from bindir/deliverquota to libdir/bin/deliverquota

install-deliverquota:
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	$(INSTALL_PROGRAM) maildir/deliverquota $(DESTDIR)$(bindir)/deliverquota
	cd $(DESTDIR)$(bindir); rm -f maildrop.deliverquota

uninstall-deliverquota:
	rm -f $(DESTDIR)$(bindir)/deliverquota

else
DELIVERQUOTAHTML=
DELIVERQUOTAMAN=

install-deliverquota:

uninstall-deliverquota:

endif

MAILDROPHTML=lockmail.html maildirmake.html maildropex.html \
	maildir.html \
	maildropfilter.html maildropgdbm.html maildrop.html mailbot.html \
	makemime.html reformail.html reformime.html

MAILDROPMAN=lockmail.1 maildirmake.1 maildrop.1 mailbot.1 maildropex.7 \
	maildir.5 \
	maildropfilter.7 maildropgdbm.7 makemime.1 reformail.1 reformime.1

EXTRA_LTLIBRARIES=librfc822.la librfc2045.la

if DEVEL
DEVEL_MAN=rfc822.3 rfc2045.3
endif

sysincludedir=$(includedir)
sysinclude_DATA=@DEVEL_INCLUDES@

lib_LTLIBRARIES=@DEVEL_LIBS@

htmldataclean=$(DELIVERQUOTAHTML) \
	$(MAILDROPHTML) rfc822.html rfc2045.html makedat.html

html_DATA=$(htmldataclean) manpage.css

man_MANS=$(DELIVERQUOTAMAN) \
	$(MAILDROPMAN) $(DEVEL_MAN)
DISTCLEANFILES=$(htmldataclean) $(man_MANS)

librfc822_la_SOURCES=
librfc2045_la_SOURCES=

librfc822_la_LIBADD=rfc822/librfc822.la rfc822/libencode.la
librfc2045_la_LIBADD=rfc2045/librfc2045.la
librfc822_la_LDFLAGS=-rpath $(libdir)
librfc2045_la_LDFLAGS=-rpath $(libdir)

install-data-local: install-man install-deliverquota install-maildrop

uninstall-local: uninstall-deliverquota uninstall-maildrop

deliverquota.8: maildir/deliverquota.8
	cp -f maildir/deliverquota.8 deliverquota.8

deliverquota.html: maildir/deliverquota.html
	cp -f maildir/deliverquota.html deliverquota.html

#  In maildrop 0.74, maildirmake was installed as bindir/maildrop.maildirmake
#  and a soft link was created from maildirmake to maildrop.maildirmake
#
#  Starting with maildrop 0.75, maildirmake gets installed into libdir/bin,
#  and install-exec-local will remove maildrop 0.74's maildirmake, and create
#  a soft link.
#  It is expected that the default login script will add libdir/bin to PATH
#
#  Same thing goes for deliverquota and maildrop.deliverquota

install-maildrop:
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	for f in maildrop mailbot reformail ; do \
		$(INSTALL_PROGRAM) maildrop/$$f $(DESTDIR)$(bindir)/$$f ; \
	done
	$(INSTALL_PROGRAM) liblock/lockmail $(DESTDIR)$(bindir)/lockmail
	$(INSTALL_PROGRAM) maildir/maildirmake $(DESTDIR)$(bindir)/maildirmake
	$(INSTALL_PROGRAM) rfc2045/reformime $(DESTDIR)$(bindir)/reformime
	$(INSTALL_PROGRAM) rfc2045/makemime $(DESTDIR)$(bindir)/makemime
	$(INSTALL_PROGRAM) makedat/makedatprog $(DESTDIR)$(bindir)/makedatprog
	$(INSTALL_SCRIPT)  makedat/makedat     $(DESTDIR)$(bindir)/makedat
	. maildrop/uidgid ; test -z "$$gid" && exit 0; test -w /etc || exit 0; cd $(DESTDIR)$(bindir) && chgrp $$gid maildrop lockmail
	. maildrop/uidgid ; test -z "$$uid" && exit 0; test -w /etc || exit 0; cd $(DESTDIR)$(bindir) && chown $$uid maildrop lockmail && chmod 755 maildrop lockmail

uninstall-maildrop:
	for f in maildrop mailbot maildirmake reformail lockmail makemime reformime makedat makedatprog; do \
		rm -f $(DESTDIR)$(bindir)/$$f ; \
	done


all-recursive: all-make-check

all-make-check:
	$(MAKE) all-gmake-check FOO=BAR

all-gmake-check:
	@test "$(FOO)" = "BAR" && exit 0 ; echo "" ; \
		echo "ERROR: Something's wrong.  If you're using xBSD, you must use gmake" ; \
		echo "instead of make, as you were told by the INSTALL file (you did read the" ; \
		echo "INSTALL file, did you?)" ; echo "" ; exit 1

makedat.html: maildrop/makedat.html
	cp -f $(srcdir)/maildrop/makedat.html makedat.html

makedat.1: maildrop/makedat.1
	cp -f $(srcdir)/maildrop/makedat.1 makedat.1

reformime.html: $(srcdir)/rfc2045/reformime.html
	cp -f $(srcdir)/rfc2045/reformime.html reformime.html

reformime.1: $(srcdir)/rfc2045/reformime.1
	cp -f $(srcdir)/rfc2045/reformime.1 reformime.1

makemime.html: $(srcdir)/rfc2045/makemime.html
	cp -f $(srcdir)/rfc2045/makemime.html makemime.html

makemime.1: $(srcdir)/rfc2045/makemime.1
	cp -f $(srcdir)/rfc2045/makemime.1 makemime.1

lockmail.html: liblock/lockmail.html
	cp -f $(srcdir)/liblock/lockmail.html lockmail.html

lockmail.1: liblock/lockmail.1
	cp -f $(srcdir)/liblock/lockmail.1 lockmail.1

maildir.html: maildir/maildir.html
	cp -f $(srcdir)/maildir/maildir.html maildir.html

maildir.5: maildir/maildir.5
	cp -f $(srcdir)/maildir/maildir.5 maildir.5

maildirmake.html: maildir/maildirmake.html
	cp -f maildir/maildirmake.html maildirmake.html

maildirmake.1: maildir/maildirmake.1
	cp -f maildir/maildirmake.1 .

maildropex.html: maildrop/maildropex.html
	cp -f $(srcdir)/maildrop/maildropex.html maildropex.html

maildropex.7: maildrop/maildropex.7
	cp -f $(srcdir)/maildrop/maildropex.7 maildropex.7

maildropfilter.html: maildrop/maildropfilter.html
	cp -f maildrop/maildropfilter.html maildropfilter.html

maildropfilter.7: maildrop/maildropfilter.7
	cp -f maildrop/maildropfilter.7 maildropfilter.7

maildropgdbm.html: maildrop/maildropgdbm.html
	cp -f $(srcdir)/maildrop/maildropgdbm.html maildropgdbm.html

maildropgdbm.7: maildrop/maildropgdbm.7
	cp -f $(srcdir)/maildrop/maildropgdbm.7 maildropgdbm.7

maildrop.html: maildrop/maildrop.html
	cp -f maildrop/maildrop.html maildrop.html

maildrop.1: maildrop/maildrop.1
	cp -f maildrop/maildrop.1 maildrop.1

mailbot.html: maildrop/mailbot.html
	cp -f $(srcdir)/maildrop/mailbot.html mailbot.html

mailbot.1: maildrop/mailbot.1
	cp -f $(srcdir)/maildrop/mailbot.1 mailbot.1

reformail.html: maildrop/reformail.html
	cp -f $(srcdir)/maildrop/reformail.html reformail.html

reformail.1: maildrop/reformail.1
	cp -f $(srcdir)/maildrop/reformail.1 reformail.1

maildirquota.html: maildir/maildirquota.html
	cp -f $(srcdir)/maildir/maildirquota.html maildirquota.html

maildirquota.7: maildir/maildirquota.7
	cp -f $(srcdir)/maildir/maildirquota.7 maildirquota.7

rfc822.h: rfc822/rfc822.h
	cp -f $(srcdir)/rfc822/rfc822.h .

rfc2047.h: rfc822/rfc2047.h
	cp -f $(srcdir)/rfc822/rfc2047.h .

rfc2045.h: rfc2045/rfc2045.h
	cp -f $(srcdir)/rfc2045/rfc2045.h .

rfc2045.3: rfc2045/rfc2045.3
	cp -f $(srcdir)/rfc2045/rfc2045.3 .

rfc822.html: rfc822/rfc822.html
	cp -f $(srcdir)/rfc822/rfc822.html .

rfc822.3: rfc822/rfc822.html
	cp -f $(srcdir)/rfc822/rfc822.3 .

rfc2045.html: rfc2045/rfc2045.html
	cp -f $(srcdir)/rfc2045/rfc2045.html .

CLEANFILES=INSTALL.txt UPGRADE.txt

INSTALL.txt: INSTALL.html
	$(HTML2TXT) $(srcdir)/INSTALL.html >INSTALL.txt

UPGRADE.txt: UPGRADE.html
	$(HTML2TXT) $(srcdir)/UPGRADE.html >UPGRADE.txt

maildroptips.txt: maildrop/maildroptips.html
	$(HTML2TXT) maildrop/maildroptips.html >maildroptips.txt

dist-hook: INSTALL.txt UPGRADE.txt maildrop.spec maildrop.lpspec
	rm -f $(distdir)/INSTALL $(distdir)/UPGRADE $(distdir)/README
	$(HTML2TXT) $(srcdir)/README.html >$(distdir)/README
	cp INSTALL.txt $(distdir)/INSTALL
	cp UPGRADE.txt $(distdir)/UPGRADE
	cp maildrop.spec maildrop.lpspec $(distdir)
#
#  For maintainer's use only
#
distrelease:
	$(MAKE) distcheck
	$(MAKE) rpm
	socksify cvs commit
	socksify cvs -Q tag -F maildrop-latest
	$(MAKE)
	$(MAKE)
	$(MAKE) dist
	$(MAKE) htmldoc

dollar=$$
id=$(dollar)Id

htmldoc: $(DELIVERQUOTAHTMLSOURCE) $(htmldataclean) maildrop/maildroptips.html
	for f in $^; do xsltproc --nonet $$HOME/www/www.courier-mta.org/xsl/maildrop.xsl $$f | perl -ne 's~\$(id):.*\$$~\$(id):'"$$h"'\$$~; print' >html/`basename $$f`; done
	cp manpage.css html

rpm: rpm-build

rpm-build:
	$(MAKE) -$(MAKEFLAGS) bump.rpm.release
	$(MAKE) -$(MAKEFLAGS) dist
	$(MAKE) -$(MAKEFLAGS) dorpm

dorpm:
	rm -rf rpm/BUILD/*
	rm -f rpm/RPMS/*/*
	rm -f rpm/SOURCES/*
	rm -f rpm/SPECS/*
	rm -f rpm/SRPMS/*
	rpmbuild -ta --clean \
		--define "_topdir `pwd`/rpm" \
		--define '_rpmdir %{_topdir}/RPMS' \
		--define '_srcrpmdir %{_topdir}/SRPMS' \
		--define '_sourcedir %{_topdir}/SOURCES' \
		--define '_specdir %{_topdir}/SPECS' \
		--define '_builddir %{_topdir}/BUILD' \
		--define '_build_name_fmt %%{ARCH}/%%{NAME}-%%{VERSION}-%%{RELEASE}.%%{ARCH}.rpm' \
		--define '_tmppath %{_var}/tmp' \
		--define '__spec_prep_pre %{___build_pre}' \
		@PACKAGE@-@VERSION@.tar.bz2

bump.rpm.release: maildrop.spec maildrop.lpspec
	VERSION="x"; \
	test -f $(srcdir)/rpm.release && . $(srcdir)/rpm.release; \
	NEXT_RELEASE=0; \
	test "$$VERSION" = "@VERSION@" && NEXT_RELEASE="$$RELEASE"; \
	RELEASE=`expr $$NEXT_RELEASE + 1`; \
	echo "VERSION=@VERSION@" >$(srcdir)/rpm.release; \
	echo "RELEASE=$$RELEASE" >>$(srcdir)/rpm.release; \
	sed 's/^Release: .*/Release: '$$RELEASE'%{courier_release}/' \
		<maildrop.spec >maildrop.spec.new; \
	mv maildrop.spec.new maildrop.spec; \
	sed 's/^Release: .*/Release: '$$RELEASE'/' \
		<maildrop.lpspec >maildrop.lpspec.new; \
	mv maildrop.lpspec.new maildrop.lpspec

lpm: lpm-build

lpm-build:
	$(MAKE) -$(MAKEFLAGS) bump.rpm.release
	$(MAKE) -$(MAKEFLAGS) dist
	rm -rf lpm
	mkdir lpm
	lpbuild `test "@REPOSITORY@" == "" || echo --define "repository=1"` --srcbuild -o lpm @PACKAGE@-@VERSION@.tar.bz2

publish: publish-@REPOSITORY@

publish-dev:
	$(MAKE) -$(MAKEFLAGS) dopublish SUBDIR=dev

publish-prod:
	$(MAKE) -$(MAKEFLAGS) dopublish SUBDIR=prod

dopublish:
	lpbuild --resign lpm/*
	rm -rf lpmtmp
	mkdir lpmtmp
	ln lpm/* lpmtmp
	rm -f lpmtmp/*debuginfo*
	mv lpmtmp/*.src.lp repository-$(SUBDIR)/`lpm -q --distribution`/sources
	mv lpmtmp/*.lp repository-$(SUBDIR)/`lpm -q --distribution`/packages
	rm -rf lpmtmp
	ls -t repository-$(SUBDIR)/`lpm -q --distribution`/sources | tail +6 | xargs -r -n 1 lpm --erase repository-$(SUBDIR)
	lpm --sync repository-$(SUBDIR)
	rsync -a -v --delete-after repository-$(SUBDIR)/`lpm -q --distribution`/. headache:lpmtool.com/www/repositories/$(SUBDIR)/maildrop/`lpm -q --distribution`

upload-beta:
	test -z "$(SFPASSWD)" && exit 1; exit 0
	rm -f @PACKAGE@-@VERSION@.tar.bz2.sig; gpg --detach-sign --default-key mrsam@courier-mta.com @PACKAGE@-@VERSION@.tar.bz2; test -f @PACKAGE@-@VERSION@.tar.bz2.sig || exit 1
	socksify ssh grace.speakeasy.net echo @PACKAGE@-@VERSION@.tar.bz2 `ls -l @PACKAGE@-@VERSION@.tar.bz2 | awk ' { print $$5 } '` \>courier-mta.org/@PACKAGE@.beta
	rsync -aP --progress @PACKAGE@-@VERSION@.tar.bz2 @PACKAGE@-@VERSION@.tar.bz2.sig frs.sourceforge.net:uploads/
	../sftools/release.pl mrsam $(SFPASSWD) courier maildrop-devel `echo @VERSION@|sed 's/.*\(........\)/\1/'` "" "" @PACKAGE@-@VERSION@.tar.bz2 @PACKAGE@-@VERSION@.tar.bz2.sig

upload-dist:
	rm -f @PACKAGE@-@VERSION@.tar.bz2.sig; gpg --detach-sign --default-key mrsam@courier-mta.com @PACKAGE@-@VERSION@.tar.bz2; test -f @PACKAGE@-@VERSION@.tar.bz2.sig || exit 1
	socksify cvs tag -F -r courier-latest \
               `echo @PACKAGE@-@VERSION@ | tr '.' '-'`
	rsync -aP --progress @PACKAGE@-@VERSION@.tar.bz2 @PACKAGE@-@VERSION@.tar.bz2.sig frs.sourceforge.net:uploads/

release-dist:
	test -z "$(SFPASSWD)" && exit 1; exit 0
	./changelog.pl >changelog.txt
	emacs releasenotes.txt
	test -f releasenotes.txt || exit 1
	socksify ../sftools/release.pl mrsam $(SFPASSWD) courier maildrop @VERSION@ releasenotes.txt changelog.txt @PACKAGE@-@VERSION@.tar.bz2 @PACKAGE@-@VERSION@.tar.bz2.sig
	socksify ssh grace.speakeasy.net rm -f courier-mta.org/maildrop.beta

