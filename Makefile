CC		=	gcc
CFLAGS	=	-Wall -Wextra -std=c99 -pedantic -g

.PHONY:	clean

all:	distribute

clean:
	$(RM) *.o distribute

distribute.o: distribute.h
