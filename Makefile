SRC = *.c
OBJ = $(SRC:.c=.o)
BIN = scanner
CFLAGS =-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g

all: $(BIN)

$(BIN): $(OBJ)
	gcc $^ $(CFLAGS) -o $@

%.o: %.c
	gcc -c $^ $(CFLAGS) -o $@