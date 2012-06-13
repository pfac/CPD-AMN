#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <mpi.h>

//#define MAX_ITERATIONS 1000

typedef long unsigned uint;
#define UMAX ULONG_MAX

#ifndef	_LP64
//	32 bits build
typedef float real;

real random_real () {
	// return (real) arc4random() / (real) UMAX;
	return (real) rand() / (real) RAND_MAX;
}



#define fscan_real(f,r)      ( fscanf( (f) , "%f" , (r) ) )



#else
//	64 bits build
#include <stdint.h>

//typedef uint64_t uint;
//#define UMAX UINT64_MAX
typedef double real;

union _32to64
{
	uint32_t u32[2];
	uint64_t u64;
};



real random_real ()
{
	// union _32to64 r;
	// r.u32[0] = arc4random();
	// r.u32[1] = arc4random();
	// return (real) r.u64 / (real) UMAX;
	return (real) rand() / (real) RAND_MAX;
}



#define fscan_real(f,r)      ( fscanf( (f) , "%lf" , (r) ) )

#endif//	_LP64


#define print_uint(n)        ( printf(  "%lu" , (n) ) )
#define fprint_uint(f,n)     ( fprintf( (f) , "%lu" , (n) ) )
#define fscan_uint(f,n)      ( fscanf( (f) , "%lu" , (n) ) )
#define parse_uint(s)        ( strtoul( (s) , NULL , 10 ) )





void print_uint_matrix ( uint * matrix , uint rows , uint cols ) {
	uint i;
	uint j;
	for ( i = 0 ; i < rows ; ++i )
	{
		printf("\t");
		for ( j = 0 ; j < cols ; ++j )
		{
			print_uint( matrix[ i * cols + j ] );
			printf(" ");
		}
		printf("\n");
	}
}



uint random_uint_lim ( uint min , uint max ) {
	real f = random_real();
	return f * ( max - min + 1 ) + min;
}



uint random_uint_max ( uint max ) {
	return random_uint_lim(0,max);
}



void random_matrix_uint_lim ( uint * matrix , uint rows , uint cols , uint min , uint max ) {
	uint i;
	uint j;
	for ( i = 0 ; i < rows ; ++i )
		for ( j = 0 ; j < cols ; ++j )
			matrix[ i * cols + j ] = random_uint_lim( min , max );
}



void random_dislikes ( uint * dislikes , uint nstudents, uint min , uint max ) {
	uint i;
	uint j;
	for ( i = 0 ; i < nstudents ; ++i )
	{

		dislikes[ i * nstudents + i ] = UMAX;
		for ( j = i + 1 ; j < nstudents ; ++j )
			  dislikes[ i * nstudents + j ]
			= dislikes[ j * nstudents + i ]
			= random_uint_lim( min , max );
	}
}






uint distribute_random ( uint * dislikes , uint * rooms , uint * assigned , uint nstudents ) {
	uint cost;
	uint i;
	uint laststudent = nstudents - 1;
	uint nrooms = nstudents / 2;
	uint s1;
	uint s2;

	for ( i = 0 ; i < nstudents ; ++i )
		assigned[i] = UMAX;

	cost = 0;
	for ( i = 0 ; i < nrooms ; ++i )
	{
		do
		{
			s1 = random_uint_max(laststudent);
		}
		while ( assigned[ s1 ] < UMAX );
		assigned[ s1 ] = i;
		do
		{
			s2 = random_uint_max(laststudent);
		}
		while ( assigned[ s2 ] < UMAX );
		assigned[ s2 ] = i;
		rooms[ i * 2     ] = s1;
		rooms[ i * 2 + 1 ] = s2;
		cost += dislikes[ s1 * nstudents + s2 ];
	}

	return cost;
}


#include "distribute.h"
#define	SIMULATED_ANNEALING
#include "distribute.h"





/**
 * \param nstudents Number of students.
 */
int main ( int argc , char * argv[] ) {
	uint * dislikes;
	uint   c1;
	uint   c2;
	uint   i;
	uint   j;
	uint   laststudent;
	uint   lc1;
	uint   lc2;
	uint   nstudents;
	uint * rooms;
	real   t0;
	int    mpi_rank;
	int    mpi_size;

	MPI_Init( &argc , &argv );
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
			nstudents = parse_uint( argv[1] );
		if (argc > 2)
			t0 = atof(argv[2]);
		else
			t0 = 1.0;
	}
	MPI_Bcast (&nstudents, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
	laststudent = nstudents - 1;

	dislikes = (uint *) malloc( nstudents * nstudents * sizeof( uint ) );

	srand(time(NULL) * mpi_rank);

	if (!mpi_rank)
	{
		//0. Generate dislike matrices
		random_dislikes( dislikes , nstudents , 1 , 10 );

		//0. Add perfect solution
		for ( i = 0 ; i < laststudent ; i += 2 )
		{
			j = i + 1;
			dislikes[ i * nstudents + j ] = 0;
			dislikes[ j * nstudents + i ] = 0;
		}
	}
	MPI_Bcast (dislikes, nstudents * nstudents, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

	//0. Create arrays
	rooms = (uint*) malloc (nstudents * sizeof (uint));

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

	if ( ! mpi_rank ) {
		//98. Output
		print_uint( nstudents );
		printf(";");
		print_uint( c1 );
		printf(";");
		print_uint( c2 );
		printf("\n");
	}

	// 99. Cleanup
	free( rooms );
	free( dislikes );

	MPI_Finalize();

	return 0;
}
