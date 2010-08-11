# Don't edit Makefile! Use conf-* for configuration.

SHELL=/bin/sh

default: it

alloc.0: \
alloc.3
	nroff -man alloc.3 > alloc.0

alloc.a: \
makelib alloc.o alloc_re.o
	./makelib alloc.a alloc.o alloc_re.o

alloc.o: \
compile alloc.c alloc.h error.h
	./compile alloc.c

alloc_re.o: \
compile alloc_re.c alloc.h byte.h
	./compile alloc_re.c

auto-ccld.sh: \
conf-cc conf-ld warn-auto.sh
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\' \
	) > auto-ccld.sh

auto-str: \
load auto-str.o substdio.a error.a str.a
	./load auto-str substdio.a error.a str.a 

auto-str.o: \
compile auto-str.c substdio.h readwrite.h exit.h
	./compile auto-str.c

auto_home.c: \
auto-str conf-home
	./auto-str auto_home `head -1 conf-home` > auto_home.c

auto_home.o: \
compile auto_home.c
	./compile auto_home.c

byte_chr.o: \
compile byte_chr.c byte.h
	./compile byte_chr.c

byte_copy.o: \
compile byte_copy.c byte.h
	./compile byte_copy.c

byte_cr.o: \
compile byte_cr.c byte.h
	./compile byte_cr.c

case.0: \
case.3
	nroff -man case.3 > case.0

case.a: \
makelib case_lowers.o
	./makelib case.a case_lowers.o

case_lowers.o: \
compile case_lowers.c case.h
	./compile case_lowers.c

check: \
it instcheck
	./instcheck

columnt: \
load columnt.o slurpclose.o strerr.a substdio.a stralloc.a alloc.a \
error.a str.a
	./load columnt slurpclose.o strerr.a substdio.a stralloc.a \
	alloc.a error.a str.a 

columnt.0: \
columnt.1
	nroff -man columnt.1 > columnt.0

columnt.o: \
compile columnt.c stralloc.h gen_alloc.h alloc.h slurpclose.h \
strerr.h substdio.h readwrite.h exit.h
	./compile columnt.c

compile: \
make-compile warn-auto.sh systype
	( cat warn-auto.sh; ./make-compile "`cat systype`" ) > \
	compile
	chmod 755 compile

ddist: \
warn-auto.sh ddist.sh conf-home
	cat warn-auto.sh ddist.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> ddist
	chmod 755 ddist

deferrals: \
warn-auto.sh deferrals.sh conf-home
	cat warn-auto.sh deferrals.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> deferrals
	chmod 755 deferrals

error.0: \
error.3
	nroff -man error.3 > error.0

error.a: \
makelib error.o error_str.o
	./makelib error.a error.o error_str.o

error.o: \
compile error.c error.h
	./compile error.c

error_str.0: \
error_str.3
	nroff -man error_str.3 > error_str.0

error_str.o: \
compile error_str.c error.h
	./compile error_str.c

failures: \
warn-auto.sh failures.sh conf-home
	cat warn-auto.sh failures.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> failures
	chmod 755 failures

find-systype: \
find-systype.sh auto-ccld.sh
	cat auto-ccld.sh find-systype.sh > find-systype
	chmod 755 find-systype

fmt_str.o: \
compile fmt_str.c fmt.h
	./compile fmt_str.c

fmt_strn.o: \
compile fmt_strn.c fmt.h
	./compile fmt_strn.c

fmt_uint.o: \
compile fmt_uint.c fmt.h
	./compile fmt_uint.c

fmt_uint0.o: \
compile fmt_uint0.c fmt.h
	./compile fmt_uint0.c

fmt_ulong.o: \
compile fmt_ulong.c fmt.h
	./compile fmt_ulong.c

fmt_ushort.o: \
compile fmt_ushort.c fmt.h
	./compile fmt_ushort.c

fs.a: \
makelib fmt_str.o fmt_strn.o fmt_uint.o fmt_uint0.o fmt_ulong.o \
fmt_ushort.o scan_uint.o scan_ulong.o scan_ushort.o
	./makelib fs.a fmt_str.o fmt_strn.o fmt_uint.o fmt_uint0.o \
	fmt_ulong.o fmt_ushort.o scan_uint.o scan_ulong.o \
	scan_ushort.o

getln.0: \
getln.3
	nroff -man getln.3 > getln.0

getln.a: \
makelib getln.o getln2.o
	./makelib getln.a getln.o getln2.o

getln.o: \
compile getln.c substdio.h byte.h stralloc.h gen_alloc.h getln.h
	./compile getln.c

getln2.0: \
getln2.3
	nroff -man getln2.3 > getln2.0

getln2.o: \
compile getln2.c substdio.h stralloc.h gen_alloc.h byte.h getln.h
	./compile getln2.c

hier.o: \
compile hier.c auto_home.h
	./compile hier.c

install: \
load install.o hier.o auto_home.o strerr.a substdio.a open.a error.a \
str.a
	./load install hier.o auto_home.o strerr.a substdio.a \
	open.a error.a str.a 

install.o: \
compile install.c substdio.h strerr.h error.h open.h readwrite.h \
exit.h
	./compile install.c

instcheck: \
load instcheck.o hier.o auto_home.o strerr.a substdio.a error.a str.a
	./load instcheck hier.o auto_home.o strerr.a substdio.a \
	error.a str.a 

instcheck.o: \
compile instcheck.c strerr.h error.h readwrite.h exit.h
	./compile instcheck.c

it: \
man prog install instcheck

load: \
make-load warn-auto.sh systype
	( cat warn-auto.sh; ./make-load "`cat systype`" ) > load
	chmod 755 load

make-compile: \
make-compile.sh auto-ccld.sh
	cat auto-ccld.sh make-compile.sh > make-compile
	chmod 755 make-compile

make-load: \
make-load.sh auto-ccld.sh
	cat auto-ccld.sh make-load.sh > make-load
	chmod 755 make-load

make-makelib: \
make-makelib.sh auto-ccld.sh
	cat auto-ccld.sh make-makelib.sh > make-makelib
	chmod 755 make-makelib

makelib: \
make-makelib warn-auto.sh systype
	( cat warn-auto.sh; ./make-makelib "`cat systype`" ) > \
	makelib
	chmod 755 makelib

man: \
matchup.0 columnt.0 xqp.0 xsender.0 xrecipient.0 alloc.0 case.0 \
error.0 error_str.0 getln2.0 getln.0 stralloc.0

matchup: \
load matchup.o strerr.a getln.a substdio.a stralloc.a alloc.a error.a \
str.a fs.a case.a
	./load matchup strerr.a getln.a substdio.a stralloc.a \
	alloc.a error.a str.a fs.a case.a 

matchup.0: \
matchup.1
	nroff -man matchup.1 > matchup.0

matchup.o: \
compile matchup.c stralloc.h gen_alloc.h gen_alloc.h gen_allocdefs.h \
strerr.h getln.h substdio.h subfd.h substdio.h readwrite.h exit.h \
str.h fmt.h scan.h case.h
	./compile matchup.c

open.a: \
makelib open_read.o open_trunc.o
	./makelib open.a open_read.o open_trunc.o

open_read.o: \
compile open_read.c open.h
	./compile open_read.c

open_trunc.o: \
compile open_trunc.c open.h
	./compile open_trunc.c

prog: \
matchup columnt zoverall zsendmail xqp xsender xrecipient ddist \
deferrals failures successes rhosts recipients rxdelay senders suids \
zddist zdeferrals zfailures zsuccesses zrhosts zrecipients zrxdelay \
zsenders zsuids

recipients: \
warn-auto.sh recipients.sh conf-home
	cat warn-auto.sh recipients.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> recipients
	chmod 755 recipients

rhosts: \
warn-auto.sh rhosts.sh conf-home
	cat warn-auto.sh rhosts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rhosts
	chmod 755 rhosts

rxdelay: \
warn-auto.sh rxdelay.sh conf-home
	cat warn-auto.sh rxdelay.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rxdelay
	chmod 755 rxdelay

scan_uint.o: \
compile scan_uint.c scan.h
	./compile scan_uint.c

scan_ulong.o: \
compile scan_ulong.c scan.h
	./compile scan_ulong.c

scan_ushort.o: \
compile scan_ushort.c scan.h
	./compile scan_ushort.c

senders: \
warn-auto.sh senders.sh conf-home
	cat warn-auto.sh senders.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> senders
	chmod 755 senders

setup: \
it install
	./install

slurpclose.o: \
compile slurpclose.c stralloc.h gen_alloc.h readwrite.h slurpclose.h \
error.h
	./compile slurpclose.c

str.a: \
makelib str_len.o str_diff.o str_start.o byte_copy.o byte_cr.o \
byte_chr.o
	./makelib str.a str_len.o str_diff.o str_start.o \
	byte_copy.o byte_cr.o byte_chr.o

str_diff.o: \
compile str_diff.c str.h
	./compile str_diff.c

str_len.o: \
compile str_len.c str.h
	./compile str_len.c

str_start.o: \
compile str_start.c str.h
	./compile str_start.c

stralloc.0: \
stralloc.3
	nroff -man stralloc.3 > stralloc.0

stralloc.a: \
makelib stralloc_eady.o stralloc_pend.o stralloc_copy.o \
stralloc_opys.o stralloc_opyb.o stralloc_cat.o stralloc_cats.o \
stralloc_catb.o
	./makelib stralloc.a stralloc_eady.o stralloc_pend.o \
	stralloc_copy.o stralloc_opys.o stralloc_opyb.o \
	stralloc_cat.o stralloc_cats.o stralloc_catb.o

stralloc_cat.o: \
compile stralloc_cat.c byte.h stralloc.h gen_alloc.h
	./compile stralloc_cat.c

stralloc_catb.o: \
compile stralloc_catb.c stralloc.h gen_alloc.h byte.h
	./compile stralloc_catb.c

stralloc_cats.o: \
compile stralloc_cats.c byte.h str.h stralloc.h gen_alloc.h
	./compile stralloc_cats.c

stralloc_copy.o: \
compile stralloc_copy.c byte.h stralloc.h gen_alloc.h
	./compile stralloc_copy.c

stralloc_eady.o: \
compile stralloc_eady.c alloc.h stralloc.h gen_alloc.h \
gen_allocdefs.h
	./compile stralloc_eady.c

stralloc_opyb.o: \
compile stralloc_opyb.c stralloc.h gen_alloc.h byte.h
	./compile stralloc_opyb.c

stralloc_opys.o: \
compile stralloc_opys.c byte.h str.h stralloc.h gen_alloc.h
	./compile stralloc_opys.c

stralloc_pend.o: \
compile stralloc_pend.c alloc.h stralloc.h gen_alloc.h \
gen_allocdefs.h
	./compile stralloc_pend.c

strerr.a: \
makelib strerr_sys.o strerr_die.o
	./makelib strerr.a strerr_sys.o strerr_die.o

strerr_die.o: \
compile strerr_die.c substdio.h subfd.h substdio.h exit.h strerr.h
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c error.h strerr.h
	./compile strerr_sys.c

subfderr.o: \
compile subfderr.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfderr.c

subfdin.o: \
compile subfdin.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfdin.c

subfdout.o: \
compile subfdout.c readwrite.h substdio.h subfd.h substdio.h
	./compile subfdout.c

substdi.o: \
compile substdi.c substdio.h byte.h error.h
	./compile substdi.c

substdio.a: \
makelib substdio.o substdi.o substdo.o subfderr.o subfdin.o \
subfdout.o substdio_copy.o
	./makelib substdio.a substdio.o substdi.o substdo.o \
	subfderr.o subfdin.o subfdout.o substdio_copy.o

substdio.o: \
compile substdio.c substdio.h
	./compile substdio.c

substdio_copy.o: \
compile substdio_copy.c substdio.h
	./compile substdio_copy.c

substdo.o: \
compile substdo.c substdio.h str.h byte.h error.h
	./compile substdo.c

successes: \
warn-auto.sh successes.sh conf-home
	cat warn-auto.sh successes.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> successes
	chmod 755 successes

suids: \
warn-auto.sh suids.sh conf-home
	cat warn-auto.sh suids.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> suids
	chmod 755 suids

systype: \
find-systype trycpp.c
	./find-systype > systype

xqp: \
warn-auto.sh xqp.sh conf-home
	cat warn-auto.sh xqp.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> xqp
	chmod 755 xqp

xqp.0: \
xqp.1
	nroff -man xqp.1 > xqp.0

xrecipient: \
warn-auto.sh xrecipient.sh conf-home
	cat warn-auto.sh xrecipient.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> xrecipient
	chmod 755 xrecipient

xrecipient.0: \
xrecipient.1
	nroff -man xrecipient.1 > xrecipient.0

xsender: \
warn-auto.sh xsender.sh conf-home
	cat warn-auto.sh xsender.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> xsender
	chmod 755 xsender

xsender.0: \
xsender.1
	nroff -man xsender.1 > xsender.0

zddist: \
warn-auto.sh zddist.sh conf-home
	cat warn-auto.sh zddist.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zddist
	chmod 755 zddist

zdeferrals: \
warn-auto.sh zdeferrals.sh conf-home
	cat warn-auto.sh zdeferrals.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zdeferrals
	chmod 755 zdeferrals

zfailures: \
warn-auto.sh zfailures.sh conf-home
	cat warn-auto.sh zfailures.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zfailures
	chmod 755 zfailures

zoverall: \
warn-auto.sh zoverall.sh conf-home
	cat warn-auto.sh zoverall.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zoverall
	chmod 755 zoverall

zrecipients: \
warn-auto.sh zrecipients.sh conf-home
	cat warn-auto.sh zrecipients.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zrecipients
	chmod 755 zrecipients

zrhosts: \
warn-auto.sh zrhosts.sh conf-home
	cat warn-auto.sh zrhosts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zrhosts
	chmod 755 zrhosts

zrxdelay: \
warn-auto.sh zrxdelay.sh conf-home
	cat warn-auto.sh zrxdelay.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zrxdelay
	chmod 755 zrxdelay

zsenders: \
warn-auto.sh zsenders.sh conf-home
	cat warn-auto.sh zsenders.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsenders
	chmod 755 zsenders

zsendmail: \
warn-auto.sh zsendmail.sh conf-home
	cat warn-auto.sh zsendmail.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsendmail
	chmod 755 zsendmail

zsuccesses: \
warn-auto.sh zsuccesses.sh conf-home
	cat warn-auto.sh zsuccesses.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsuccesses
	chmod 755 zsuccesses

zsuids: \
warn-auto.sh zsuids.sh conf-home
	cat warn-auto.sh zsuids.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsuids
	chmod 755 zsuids
