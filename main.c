#include <stdint.h>
#include <stdlib.h>
#ifdef _LP64
typedef uint64_t uint;
typedef double real;

#define UMAX UINT64_MAX

union _32to64
{
	uint32_t u32[2];
	uint64_t u64;
};

uint randgen_max( uint max )
{
	union _32to64 r;
	union _32to64 s;
	s.u64 = max;
	r.u32[0] = arc4random_uniform(s.u32[0]);
	r.u32[1] = arc4random_uniform(s.u32[1]);
	return r.u64;
}

real randgenR()
{
	union _32to64 r;
	r.u32[0] = arc4random_uniform( UINT32_MAX );
	r.u32[1] = arc4random_uniform( UINT32_MAX );
	real f = (real) r.u64 / (real) UMAX;
	return f;
}

#else
typedef uint32_t uint;
typedef float real;
#define UMAX UINT32_MAX

uint randgen_max( uint max )
{
	return arc4random_uniform(max);
}

real randgenR()
{
	uint r = arc4random_uniform( UINT32_MAX );
	return (real) r / (real) UMAX;
}

#endif

uint randgen_lim( uint min , uint max )
{
	return randgenR() * ( max - min ) + min;
}



#define PERFECT_SOLUTION

#include <stdio.h>
#include <time.h>

#include <limits.h>
#include <string.h>

#define MAX_RATIO 1000


void debug_room_matrix( uint *rooms , uint nrooms )
{
	uint i, j;
	printf("Current rooms matrix:\n");
	for ( i = 0 ; i < nrooms ; ++i )
	{
		printf("\t");
		for ( j = 0 ; j < 2 ; ++j )
#ifdef	_LP64
			printf("%llu ", rooms[ i * 2 + j ]);
#else
			printf("%lu ", rooms[ i * 2 + j ]);
#endif
		printf("\n");
	}
	getchar();
}

void debug_assigned_vector( uint *assigned , uint nstudents )
{
	uint i;
	printf("Current assigned vector:\n");
	for ( i = 0 ; i < nstudents ; ++i )
	{
#ifdef	_LP64
		printf("\t%llu\n", assigned[ i ]);
#else
		printf("\t%lu\n", assigned[ i ]);
#endif
	}
	getchar();
}

#include "distribute.h"
#define SIMULATED_ANNEALING
#include "distribute.h"

void debug_dislike_matrix( uint *dislikes , uint nstudents )
{
	uint i, j;
	printf("Generated dislike matrix:\n");
	for ( i = 0 ; i < nstudents ; ++i )
	{
		printf("\t");
		for ( j = 0 ; j < nstudents ; ++j )
#ifdef	_LP64
			printf("%llu ", dislikes[ i * nstudents + j ]);
#else
			printf("%lu ", dislikes[ i * nstudents + j ]);
#endif	
		printf("\n");
	}
	getchar();
}

/**
 * \param	n	Number of students.
 */
int main ( int argc , char *argv[] )
{
	uint nstudents;
	uint laststudent;
	uint *dislikes;
	uint *rooms;
	uint i, j;
	uint max_iterations;

	if ( argc > 1 )
		nstudents = strtol( argv[1] , NULL , 10 );
	else
	{
		fprintf( stderr , "Error: no dimension specified.\n" );
		exit(-1);
	}
	laststudent = nstudents - 1;
	max_iterations = MAX_RATIO * nstudents;

	//	allocate dynamic memory
	dislikes = (uint*) calloc( nstudents * nstudents , sizeof(uint) );
	rooms = (uint*) calloc( nstudents , sizeof(uint) );
	//assigned = (uint*) malloc( nstudents * sizeof(uint) );

	//	generate dislike matrix
	for ( i = 0 ; i < nstudents ; ++i )
	{
		dislikes[ i * nstudents + i ] = UMAX;
		for ( j = i + 1 ; j < nstudents ; ++j )
			  dislikes[ i * nstudents + j ]
			= dislikes[ j * nstudents + i ]
			= randgen_max(10) + 1;
	}

#ifdef	PERFECT_SOLUTION
	//	add perfect solution
	for ( i = 0 ; i < laststudent ; i += 2 )
	{
		j = i + 1;
		  dislikes[ i * nstudents + j ]
		= dislikes[ j * nstudents + i ]
		= 0;
	}
#endif

	//debug_dislike_matrix( dislikes , nstudents );


	//	standard version
	for ( i = 0 ; i < nstudents ; ++i )
	{
		rooms[ i ] = UMAX;
	}
	uint c1 = alg1( dislikes , rooms , nstudents , max_iterations );
	//	simulated annealing
	for ( i = 0 ; i < nstudents ; ++i )
	{
		rooms[ i ] = UMAX;
	}
	uint c2 = alg2( dislikes , rooms , nstudents , max_iterations );

#ifdef	_LP64
	printf("%llu;%llu;%llu\n", nstudents , c1, c2);
#else
	printf("%lu;%lu;%lu\n", nstudents , c1, c2);
#endif

	free(rooms);
	free(dislikes);

	return 0;
}
