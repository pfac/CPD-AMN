#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "archrand.h"
#include "distaux.h"


#include "distribute.h"
#define	SIMULATED_ANNEALING
#include "distribute.h"





/**
 * \param nstudents Number of students.
 */
int main ( int argc , char * argv[] ) {
	unsigned long * dislikes;
	unsigned long   c1;
	unsigned long   c2;
	unsigned long   i;
	unsigned long   j;
	unsigned long   laststudent;
	unsigned long   nstudents;
	unsigned long * rooms;
	real t0;

	//0. Read arguments
	if ( argc > 1 )
		nstudents = strtoul(argv[1], NULL, 10);
	else
		nstudents = 0;
	if (argc > 2)
		t0 = atof(argv[2]);
	else
		t0 = 1.0;

	//0. Preparation
	laststudent = nstudents - 1;
	randomseed(time(NULL));

	//0. Create arrays
	dislikes = (unsigned long *) malloc(nstudents * nstudents * sizeof(unsigned long));
	rooms    = (unsigned long *) malloc(nstudents             * sizeof(unsigned long));

	//0. Generate dislike matrices
	random_dislikes(dislikes, nstudents, 1, 10);

	//0. Add perfect solution
	for (i = 0; i < laststudent; i += 2)
	{
		j = i + 1;
		dislikes[i * nstudents + j] = 0;
		dislikes[j * nstudents + i] = 0;
	}
	
	//1. No simulated annealing
	c1 = distribute1(dislikes, rooms, nstudents);

	//2. Now with simulated annealing
	c2 = distribute2(dislikes, rooms, nstudents, t0);

	//98. Output
	printf("%lu;%lu;%lu\n", nstudents, c1, c2);

	// 99. Cleanup
	free(dislikes);
	free(rooms);
	
	return 0;
}