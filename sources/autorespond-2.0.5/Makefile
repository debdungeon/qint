CC=gcc
OPTS=-O2
LIBS=
CFLAGS=-Wall -g
DESTDIR=
PREFIX=$(DESTDIR)/usr

all: autorespond

autorespond: autorespond.c
	$(CC) $(OPTS) $(CFLAGS) $(LIBS) $< -o $@

distclean: clean

clean:
	-rm -f autorespond autorespond.o

install: autorespond
	install -d $(PREFIX)/bin $(PREFIX)/share/man/man1
	install autorespond $(PREFIX)/bin
	install autorespond.1 $(PREFIX)/share/man/man1

