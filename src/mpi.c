#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <mpi.h>

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
	unsigned long   lc1;
	unsigned long   lc2;
	unsigned long   nstudents;
	unsigned long * rooms;
	real   t0;
	int    mpi_rank;
	int    mpi_size;

	MPI_Init(&argc , &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);

	{//debug
		int r;
		int mpi_size;
		MPI_Comm_size (MPI_COMM_WORLD, &mpi_size);
		for (r = 0; r < mpi_size; ++r)
		{
			if (r == mpi_rank)
				fprintf (stderr, "%d\tProcess %d/%d\n", getpid(), mpi_rank, mpi_size);
			MPI_Barrier (MPI_COMM_WORLD);
		}
	}

	//0. Read arguments
	if ( argc > 1 )
		nstudents = parse_uint( argv[1] );

	MPI_Init( &argc , &argv );
	MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
	MPI_Comm_size (MPI_COMM_WORLD, &mpi_size);

	if (!mpi_rank)
	{
		//0. Read arguments
		if ( argc > 1 )
			nstudents = strtoul(argv[1], NULL, 10);
		if (argc > 2)
			t0 = atof(argv[2]);
		else
			t0 = 1.0;
	}
	MPI_Bcast (&nstudents, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
#ifndef _LP64
	MPI_Bcast(&t0, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
#else
	MPI_Bcast(&t0, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
	laststudent = nstudents - 1;

	dislikes = (unsigned long *) malloc( nstudents * nstudents * sizeof(unsigned long));

	randomseed(time(NULL) * (mpi_rank + 1));

	if (!mpi_rank)
	{
		//0. Generate dislike matrices
		random_dislikes(dislikes , nstudents , 1 , 10);

		//0. Add perfect solution
		for (i = 0 ; i < laststudent ; i += 2)
		{
			j = i + 1;
			dislikes[i * nstudents + j] = 0;
			dislikes[j * nstudents + i] = 0;
		}
	}
	MPI_Bcast (dislikes, nstudents * nstudents, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

	//0. Create arrays
	rooms = (unsigned long*) malloc (nstudents * sizeof (unsigned long));

		// //debug
		// for (int r = 0; r < mpi_size; ++r)
		// {
		// 	MPI_Barrier (MPI_COMM_WORLD);
		// 	if (r == mpi_rank)
		// 	{
		// 		fprintf (stderr, "%lu\n", nstudents);
		// 		for (i = 0; i < nstudents; ++i)
		// 		{
		// 			for (j = 0; j < nstudents; ++j)
		// 				fprintf (stderr, "%lu ", dislikes[i * nstudents + j]);
		// 			fprintf (stderr, "\n");
		// 		}
		// 	}
		// 	MPI_Barrier (MPI_COMM_WORLD);
		// }

	//1. No simulated annealing
	lc1 = distribute1(dislikes, rooms, nstudents);

	//2. Now with simulated annealing
	lc2 = distribute2(dislikes, rooms, nstudents, t0);

		//debug
		// for (int r = 0; r < mpi_size; ++r)
		// {
		// 	MPI_Barrier (MPI_COMM_WORLD);
		// 	if (r == mpi_rank)
		// 	{
		// 		fprintf (stderr, "%ld %ld\n", lc1, lc2);
		// 	}
		// 	MPI_Barrier (MPI_COMM_WORLD);
		// }

	MPI_Reduce (&lc1, &c1, 1, MPI_UNSIGNED_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce (&lc2, &c2, 1, MPI_UNSIGNED_LONG, MPI_MIN, 0, MPI_COMM_WORLD);

	if (!mpi_rank) {
		//98. Output
		printf("%lu;%lu\n", c1, c2);
	}

	// 99. Cleanup
	free( rooms );
	free( dislikes );

	MPI_Finalize();

	return 0;
}
