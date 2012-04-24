#include <stdio.h>
#include <time.h>

#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_ITERATIONS 1000
#define RANDGEN(a,b)	\
	( ( (float)rand() / (float)RAND_MAX ) * ( (b) - (a) ) + (a) )


void debug_room_matrix( unsigned int *rooms , unsigned int nrooms )
{
	unsigned int i, j;
	printf("Current rooms matrix:\n");
	for ( i = 0 ; i < nrooms ; ++i )
	{
		printf("\t");
		for ( j = 0 ; j < 2 ; ++j )
			printf("%u ", rooms[ i * 2 + j ]);
		printf("\n");
	}
	getchar();
}

void debug_assigned_vector( unsigned int *assigned , unsigned int nstudents )
{
	unsigned int i;
	printf("Current assigned vector:\n");
	for ( i = 0 ; i < nstudents ; ++i )
	{
		printf("\t%u\n", assigned[ i ]);
	}
	getchar();
}

#include "distribute.h"
#define SIMULATED_ANNEALING
#include "distribute.h"

void debug_dislike_matrix( unsigned int *dislikes , unsigned int nstudents )
{
	unsigned int i, j;
	printf("Generated dislike matrix:\n");
	for ( i = 0 ; i < nstudents ; ++i )
	{
		printf("\t");
		for ( j = 0 ; j < nstudents ; ++j )
			printf("%u ", dislikes[ i * nstudents + j ]);
		printf("\n");
	}
	getchar();
}

int main ( int argc , char *argv[] )
{
	unsigned int nstudents;
	unsigned int *dislikes;
	unsigned int *rooms;
	unsigned int i, j;
	time_t seed = time(NULL);

	if ( argc > 1 )
		nstudents = strtol( argv[1] , NULL , 10 );
	else
	{
		fprintf( stderr , "Error: no dimension specified.\n" );
		exit(-1);
	}

	//	allocate dynamic memory
	dislikes = (unsigned int*) calloc( nstudents * nstudents , sizeof(unsigned int) );
	rooms = (unsigned int*) calloc( nstudents , sizeof(unsigned int) );

	//	generate dislike matrix
	srand( time(NULL) );
	for ( i = 0 ; i < nstudents ; ++i )
	{
		dislikes[ i * nstudents + i ] = 0;
		for ( j = i + 1 ; j < nstudents ; ++j )
			  dislikes[ i * nstudents + j ]
			= dislikes[ j * nstudents + i ]
			= RANDGEN(1,10);
	}

	//debug_dislike_matrix( dislikes , nstudents );

	//	standard version
	memset( rooms , 0 , nstudents * sizeof(unsigned int) );
	srand(seed);
	unsigned int c1 = alg1( dislikes , rooms , nstudents );
	//	simulated annealing
	memset( rooms , 0 , nstudents * sizeof(unsigned int) );
	srand(seed);
	unsigned int c2 = alg2( dislikes , rooms , nstudents );

	printf("%u;%u\n", c1, c2);

	free(rooms);
	free(dislikes);

	return 0;
}
