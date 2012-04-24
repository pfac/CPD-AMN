CC		=	gcc
CFLAGS	+=	-Wall -Wextra -std=c99 -pedantic -O3

ifeq ($(MODE),dbg)
CFLAGS	+=	-g
endif

.PHONY: clean

%.s: %.c
	$(CC) $(CPPFLAGS) $(TARGET_ARCH) -S -o $@ $<

all: main

main.o: distribute.h

clean:
	$(RM) *.o main