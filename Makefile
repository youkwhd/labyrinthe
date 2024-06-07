EXE      = labyrinthe

CC       = gcc
CFLAGS   = -std=c99 -pedantic-errors -Wall -Wextra

LD       = gcc
LDLIBS   =
LDFLAGS  =

SRC     := $(wildcard *.c)

ifneq ($(GUI),1)
	SRC		:= $(filter-out gui.c gui.h,$(SRC))
endif

HDR     := $(SRC:.c=.h)
OBJ     := $(SRC:.c=.o)

ifeq ($(GUI),1)
	CFLAGS  += -DGUI
	LDLIBS  += -lraylib
endif

all: $(EXE)

$(EXE): $(OBJ)
	$(LD) $(LDLIBS) $^ -o $(EXE)

# assumses that every source file depends on all the headers
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(EXE) *.o

.PHONY: clean all $(EXE)
