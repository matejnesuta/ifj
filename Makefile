CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g


all: parser

force: clean all
	
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

parser: parser.o scanner.o mystring.o
	$(CC) -o $@ $^ $(CFLAGS)

run: force
	./parser <test.php

clean:
	rm -f *.o parser