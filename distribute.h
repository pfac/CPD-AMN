#ifndef	SIMULATED_ANNEALING
unsigned int alg1 ( unsigned int *dislikes, unsigned int *rooms , unsigned int
nstudents )
#else
#include <math.h>
unsigned int alg2 ( unsigned int *dislikes, unsigned int *rooms , unsigned int
nstudents )
#endif
{
	unsigned int *assigned;
	unsigned int i;
	unsigned int nrooms = nstudents / 2;
	unsigned int laststudent = nstudents - 1;
	unsigned int s1, s2;
	unsigned int s3, s4;
	unsigned int sa, sb;
	unsigned int r1, r2;
	unsigned int p1, p2;
	unsigned int cost;
	int dcost;
#ifdef	SIMULATED_ANNEALING
	float t;
#endif

	//	create the assigned array
	assigned = (unsigned int*) malloc( nstudents * sizeof( unsigned int ) );

	//	clear rooms
	memset( assigned , UINT_MAX , nstudents * sizeof( unsigned int ) );


	//	randomly assign rooms & compute total cost
	cost = 0;
	for ( i = 0 ; i < nrooms ; ++i )
	{
		do
		{
			s1 = RANDGEN(0,laststudent);
		}
		while ( assigned[ s1 ] < UINT_MAX );
		assigned[ s1 ] = i;
		rooms[ i * 2     ] = s1;

		do
		{
			s2 = RANDGEN(0,laststudent);
		}
		while ( assigned[ s2 ] < UINT_MAX );
		assigned[ s2 ] = i;
		rooms[ i * 2 + 1 ] = s2;

		cost += dislikes[ s1 * nstudents + s2 ];
	}

#ifdef	SIMULATED_ANNEALING
	t = 0.0f;
#endif
	i = MAX_ITERATIONS;
	while ( i )
	{
		//printf("%d\n", i);
		//debug_room_matrix(rooms,nrooms);
		//debug_assigned_vector(assigned,nstudents);
		//printf("\tCost: %d\n" , cost);
		//getchar();


		//	find two students which are not roommates
		do
		{
			s1 = RANDGEN(0,laststudent);
			s3 = RANDGEN(0,laststudent);
		}
		while ( s1 == s3 || assigned[ s1 ] == assigned[ s3 ] );

		//	get the roommates
		r1 = assigned[ s1 ];
		sa = rooms[ r1 * 2     ];
		sb = rooms[ r1 * 2 + 1 ];
		p1 = ( s1 == sa );
		s2 = p1 ? sb : sa;

		r2 = assigned[ s3 ];
		sa = rooms[ r2 * 2     ];
		sb = rooms[ r2 * 2 + 1 ];
		p2 = ( s3 == sa );
		s4 = p2 ? sb : sa;

		//	compute the difference in cost
		dcost = dislikes[ s1 * nstudents + s4 ]
		      + dislikes[ s2 * nstudents + s3 ]
		      - dislikes[ s1 * nstudents + s2 ]
		      - dislikes[ s3 * nstudents + s4 ]
		      ;

		//	accept or discard
#ifndef SIMULATED_ANNEALING
		if ( dcost < 0 )
#else
		if ( dcost < 0 || exp( -dcost / t ) >= ( (float)rand() / (float)RAND_MAX ) )
#endif
		{
			//	swap s2 and s4
			rooms[ r1 * 2 + p1 ] = s4;
			rooms[ r2 * 2 + p2 ] = s2;
			assigned[ s2 ] = r2;
			assigned[ s4 ] = r1;
			cost += dcost;
			i = MAX_ITERATIONS;
		}
		else
			--i;
#ifdef	SIMULATED_ANNEALING
		t *= 0.999;
#endif
	}

	free(assigned);

	return cost;
}
