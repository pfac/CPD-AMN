UNAME	=	$(shell uname)
CC		=	gcc
CFLAGS	=	-Wall -Wextra -std=c99 -pedantic
LDFLAGS	=	-lm

GEN		?=	rand

LIMIT	?=	5000

ifeq ($(GEN),arc4)
ifeq ($(UNAME),Linux)
LDFLAGS	+=	-lbsd
endif
endif

%: %.o
	$(CC) -o $@ $^ $(LOADLIBES) $(LDLIBS) $(LDFLAGS)

EXECS	=	seq mpi
OPTXS	=	lim

.PHONY:	clean search

default all:	CFLAGS+=-O3
default:	$(EXECS)

all:		$(EXECS) $(OPTXS)

profile: CFLAGS+=-g
profile: LDFLAGS+=-L/share/apps/mpip-openmpi-gnu/lib -lmpiP -lbfd -liberty
profile: all

seq mpi lim:	distaux.o $(GEN).o

mpi:	CC=mpicc

lim.o:	CFLAGS+=	-D LIMITED=$(LIMIT)
lim.o:	seq.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(EXECS:%=%.o): distribute.h distaux.h archrand.h

clean:
	$(RM) *.o $(EXECS) $(OPTXS)
