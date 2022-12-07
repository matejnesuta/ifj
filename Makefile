CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g
BINS=ifj22.o scanner.o mystring.o symtable.o ASTree.o expressionParser.o LList.o symbol.o logger.o error.o codegen.o parser.o

all: ifj22

pack:
	zip -r xstipe02.zip *.c *.h *.dot Makefile grammar dokumentace.pdf
	
%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

ifj22: $(BINS)
	$(CC) -o $@ $^ $(CFLAGS)
	
clean:
	rm -f *.o ifj22 vgcore.*
