EXE      = labyrinthe

CC       = gcc
CFLAGS   = -std=c99 -pedantic-errors -Wall -Wextra

LD       = gcc
# LDLIBS   = -lncurses
LDLIBS   =
LDFLAGS  =

SRC     := $(wildcard *.c)
HDR     := $(SRC:.c=.h)
OBJ     := $(SRC:.c=.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(LD) $(LDLIBS) $^ -o $(EXE)

# assumses that every source file depends on all the headers
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(EXE)

.PHONY: clean all $(EXE)
