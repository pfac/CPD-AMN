CC		=	mpicc
CFLAGS	=	-Wall -Wextra -std=c99 -pedantic
LDFLAGS	=	-lm


%: %.o
	$(CC) -o $@ $^ $(LOADLIBES) $(LDLIBS) $(LDFLAGS)


.PHONY:	clean search

all:	CFLAGS+=-O3
all:	distribute

profile: CFLAGS+=-g
profile: LDFLAGS+=-L/share/apps/mpip-openmpi-gnu/lib -lmpiP -lbfd -liberty
profile: all

search:
	module load gnu/openmpi;
	export;
	$(MAKE)

clean:
	$(RM) *.o distribute

distribute.o: distribute.h
