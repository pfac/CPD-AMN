#include "distaux.h"

void random_dislikes (unsigned long * dislikes, unsigned long nstudents, unsigned long min, unsigned long max) {
	unsigned long i, j;
	for (i = 0; i < nstudents; ++i)
	{
		dislikes[ i * nstudents + i ] = ULONG_MAX;
		for ( j = i + 1 ; j < nstudents ; ++j )
			  dislikes[ i * nstudents + j ]
			= dislikes[ j * nstudents + i ]
			= randomul_lim( min , max );
	}
}

unsigned long distribute_random (unsigned long * dislikes, unsigned long * rooms, unsigned long * assigned, unsigned long nstudents) {
	unsigned long cost;
	unsigned long i;
	unsigned long laststudent = nstudents - 1;
	unsigned long nrooms = nstudents / 2;
	unsigned long s1;
	unsigned long s2;

	for ( i = 0 ; i < nstudents ; ++i )
		assigned[i] = ULONG_MAX;

	cost = 0;
	for ( i = 0 ; i < nrooms ; ++i )
	{
		do
		{
			s1 = randomul_max(laststudent);
		}
		while (assigned[ s1 ] < ULONG_MAX);
		assigned[s1] = i;
		do
		{
			s2 = randomul_max(laststudent);
		}
		while (assigned[s2] < ULONG_MAX);
		assigned[s2] = i;
		rooms[i * 2    ] = s1;
		rooms[i * 2 + 1] = s2;
		cost += dislikes[s1 * nstudents + s2];
	}

	return cost;
}