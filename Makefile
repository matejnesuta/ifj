CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g
file=test.php


all: parser

force: clean all
	
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

parser: parser.o scanner.o mystring.o
	$(CC) -o $@ $^ $(CFLAGS)

run: all
	./parser < $(file) || printf 'exit val: %d\n' $$?

clean:
	rm -f *.o parser