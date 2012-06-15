#include "archrand.h"

#ifdef BSD
#include <stdlib.h>
#else
typedef unsigned char u_char;
#include <bsd/stdlib.h>
#endif

#include <stdint.h>
#include <limits.h>

void randomseed (unsigned int seed) {}

#ifndef _LP64
real randomr () {
	real f = (real) arc4random() / (real) UINT32_MAX;
	return f;
}
#else
union _32to64 {
	uint32_t u32[2];
	uint64_t u64;
};

real randomr () {
	union _32to64 r;
	r.u32[0] = arc4random();
	r.u32[1] = arc4random();
	real f = (real) r.u64 / (real) UINT64_MAX;
	return f;
}
#endif

unsigned long randomul_lim (unsigned long min ,unsigned long max) {
	real f = randomr();
	return f * ( max - min + 1 ) + min;
}
