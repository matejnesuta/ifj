CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g
file=test.php
BINS=parser.o scanner.o mystring.o symtable.o ASTree.o ASTreeGraphGenerator.o expressionParser.o LList.o symbol.o logger.o


all: parser

force: clean all
	
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

parser: $(BINS)
	$(CC) -o $@ $^ $(CFLAGS)

run: all
	./parser < $(file) || printf "exit val: %d\n" $$?

clean:
	rm -f *.o parser