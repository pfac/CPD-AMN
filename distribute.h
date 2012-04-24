//uint *assigned;

#ifndef	SIMULATED_ANNEALING
uint alg1 ( uint *dislikes, uint *rooms , uint
nstudents , uint max_iterations )
#else
#include <math.h>
uint alg2 ( uint *dislikes, uint *rooms , uint
nstudents , uint max_iterations )
#endif
{
	uint *assigned;
	uint i;
	uint nrooms = nstudents / 2;
	uint s1, s2;
	uint s3, s4;
	uint sa, sb;
	uint r1, r2;
	uint p1, p2;
	uint cost;
	long dcost;
#ifdef	SIMULATED_ANNEALING
	real t;
#endif

	//	create the assigned array
	assigned = (uint*) malloc( nstudents * sizeof( uint ) );
	//assigned = (uint*) calloc( nstudents , sizeof(uint) );

	//	clear rooms
	for ( i = 0 ; i < nstudents ; ++i )
		assigned[ i ] = UMAX;

	//debug_assigned_vector( assigned , nstudents );


	//	randomly assign rooms & compute total cost
	cost = 0;
	for ( i = 0 ; i < nrooms ; ++i )
	{
		do
		{
			s1 = randgen_max(nstudents);
		}
		while ( assigned[ s1 ] < UMAX );
		assigned[ s1 ] = i;
		rooms[ i * 2     ] = s1;

		do
		{
			s2 = randgen_max(nstudents);
		}
		while ( assigned[ s2 ] < UMAX );
		assigned[ s2 ] = i;
		rooms[ i * 2 + 1 ] = s2;

		cost += dislikes[ s1 * nstudents + s2 ];
	}

#ifdef	SIMULATED_ANNEALING
	t = 0.0;
#endif
	i = max_iterations;
	while ( i )
	{
		//	find two students which are not roommates
		do
		{
			s1 = randgen_max(nstudents);
			s3 = randgen_max(nstudents);
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
		if ( dcost < 0 || exp( -dcost / t ) >= randgenR() )
#endif
		{
			//	swap s2 and s4
			rooms[ r1 * 2 + p1 ] = s4;
			rooms[ r2 * 2 + p2 ] = s2;
			assigned[ s2 ] = r2;
			assigned[ s4 ] = r1;
			cost += dcost;
			i = max_iterations;
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
