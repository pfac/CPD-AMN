CC		=	gcc
CFLAGS	+=	-Wall -Wextra -std=c99 -pedantic -O3

ifeq ($(MODE),dbg)
CFLAGS	+=	-g
endif

.PHONY: clean

all: main

main.o: distribute.h

clean:
	$(RM) *.o main