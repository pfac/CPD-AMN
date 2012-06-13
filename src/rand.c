#include "archrand.h"

#include <stdio.h>
#include <stdlib.h>

void randomseed (unsigned int seed) {srand(seed);}

real randomr () {return (real) rand() / (real) RAND_MAX;}

unsigned long randomul_lim (unsigned long min ,unsigned long max) {
	real f = randomr();
	return f * ( max - min + 1 ) + min;
}