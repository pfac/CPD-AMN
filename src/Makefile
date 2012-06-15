UNAME	=	$(shell uname)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -std=c99 -pedantic
LDFLAGS	=	-lm

GEN		?=	rand

ifeq ($(GEN),arc4)
ifeq ($(UNAME),Linux)
LDFLAGS	+=	-lbsd
endif
endif

%: %.o
	$(CC) -o $@ $^ $(LOADLIBES) $(LDLIBS) $(LDFLAGS)

EXECS	=	seq mpi

.PHONY:	clean search

all:	CFLAGS+=-O3
all:	$(EXECS)

profile: CFLAGS+=-g
profile: LDFLAGS+=-L/share/apps/mpip-openmpi-gnu/lib -lmpiP -lbfd -liberty
profile: all

seq mpi:	distaux.o $(GEN).o



mpi:	CC=mpicc

$(EXECS:%=%.o): distribute.h distaux.h archrand.h

clean:
	$(RM) *.o $(EXECS)
