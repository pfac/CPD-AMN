#ifndef ___DISTAUX_H___
#define ___DISTAUX_H___

void random_dislikes (unsigned long * dislikes, unsigned long nstudents, unsigned long min, unsigned long max);

unsigned long distribute_random (unsigned long * dislikes, unsigned long * rooms, unsigned long * assigned, unsigned long nstudents);

#endif//___DISTAUX_H___