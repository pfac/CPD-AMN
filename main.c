#include <stdio.h>
#include <time.h>

#include "distribute.h"
//#define SIMULATED_ANNEALING
//#include "distribute.h"

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

	debug_dislike_matrix( dislikes , nstudents );

	//	standard version
	unsigned int c1 = alg1( dislikes , rooms , nstudents );
	//	simulated annealing
	//c2 = alg2();

	printf("Solution:");
	debug_room_matrix( rooms , nstudents / 2 );
	printf("%u\n", c1);

	free(rooms);
	free(dislikes);

	return 0;
}
