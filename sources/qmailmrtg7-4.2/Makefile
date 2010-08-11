all:
	cc checkq.c -o checkq
	./checkq
	cc -s -O qmailmrtg7.c -o qmailmrtg7

install:
	strip qmailmrtg7
	cp qmailmrtg7 /usr/local/bin

clean:
	rm -rf *.o qmailmrtg7 checkq core
