CC		=	gcc
CFLAGS	=	-Wall -Wextra -std=c99 -pedantic
LDFLAGS	=	-lm


%: %.o
	$(CC) -o $@ $^ $(LOADLIBES) $(LDLIBS) $(LDFLAGS)

EXECS	=	seq mpi

.PHONY:	clean search

all:	CFLAGS+=-O3
all:	$(EXECS)

profile: CFLAGS+=-g
profile: LDFLAGS+=-L/share/apps/mpip-openmpi-gnu/lib -lmpiP -lbfd -liberty
profile: all

mpi:	CC=mpicc

$(EXECS:%=%.o): distribute.h

clean:
	$(RM) *.o $(EXECS)
