CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g


all: parserv2

force: clean all
	
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

parserv2: parserv2.o scanner.o mystring.o
	$(CC) -o $@ $^ $(CFLAGS)

run: parserv2
	./parserv2 <test.php

clean:
	rm -f *.o parserv2