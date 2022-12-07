CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g
file=test.php
BINS=ifj22.o parser.o scanner.o mystring.o symtable.o ASTree.o ASTreeGraphGenerator.o expressionParser.o LList.o symbol.o logger.o error.o codegen.o


all: ifj22

force: clean all
	
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

ifj22: $(BINS)
	$(CC) -o $@ $^ $(CFLAGS)
	
clean:
	rm -f *.o ifj22 vgcore.*
	