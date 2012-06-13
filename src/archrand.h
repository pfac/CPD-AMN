#ifndef ___ARCH_H___
#define ___ARCH_H___

#ifndef _LP64
typedef float real;
#else
typedef double real;
#endif

void randomseed (unsigned int seed);

real randomr ();

unsigned long randomul_lim (unsigned long min ,unsigned long max);

#define randomul_max(max) (randomul_lim(0,(max)))

#endif//___ARCH_H___
