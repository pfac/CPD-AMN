#include "archrand.h"

#include <stdio.h>
#include <stdlib.h>

void randomseed (unsigned int seed) {srand(seed);}

real randomr () {
	real f = (real) rand() / (real) RAND_MAX;
	return f;
}

unsigned long randomul_lim (unsigned long min ,unsigned long max) {
	real f = randomr();
	return f * ( max - min + 1 ) + min;
}
