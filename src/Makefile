CC		=	mpicc
CFLAGS	=	-Wall -Wextra -std=c99 -pedantic -g

.PHONY:	clean search

all:	distribute

profile: LDFLAGS+=-L/share/apps/mpip-openmpi-gnu/lib -lmpiP
profile: clean all

search:
	module load gnu/mpich2-mx;
	export;
	$(MAKE)

clean:
	$(RM) *.o distribute

distribute.o: distribute.h
