#include <math.h>

#ifndef	SIMULATED_ANNEALING
uint distribute1 ( uint * dislikes , uint * rooms , uint nstudents )
#else
uint distribute2 ( uint * dislikes , uint * rooms , uint nstudents )
#endif
{
	uint * assigned;
	uint   cost;
	long   dcost;
	uint   i;
	uint   laststudent = nstudents - 1;
	uint   max = 100 * nstudents;
	uint   p1;
	uint   p2;
	uint   r1;
	uint   r2;
	uint   sa;
	uint   sb;
	uint   s1;
	uint   s2;
	uint   s3;
	uint   s4;
#ifdef	SIMULATED_ANNEALING
	real   t;
#endif

	//0. Create the assigned vector
	assigned = (uint *) malloc( nstudents * sizeof( uint ) );
	if ( ! assigned )
	{
		fprintf( stderr , "Failed to allocate memory for the assigned vector!\n" );
		fprintf( stderr , "\tERROR %d : %s\n" , errno , strerror( errno ) );
		exit(errno);
	}

	//1. Assign rooms randomly & compute initial cost
	cost = distribute_random( dislikes , rooms , assigned , nstudents );

	//3. While no MAX_ITERATIONS have passed since the last accepted swap
#ifdef	SIMULATED_ANNEALING
	t = 10.0;
#endif
	i = max;
	while ( i )
	{
	//3.1. Find two students which are not roommates
		do
		{
			s1 = random_uint_max(laststudent);
			s2 = random_uint_max(laststudent);
		}
		while ( assigned[s1] == assigned[s2] );

	//3.2. Find their roommates
		//3.2.1 Room of the first student
		r1 = assigned[s1];
		sa = rooms[ r1 * 2     ];
		sb = rooms[ r1 * 2 + 1 ];
		p1 = ( s1 == sa );
		s3 = p1 ? sb : sa;

		//3.2.2 Room of the second student
		r2 = assigned[s2];
		sa = rooms[ r2 * 2     ];
		sb = rooms[ r2 * 2 + 1 ];
		p2 = ( s2 == sa );
		s4 = p2 ? sb : sa;

	//3.3. Get the cost in swapping them
		dcost = dislikes[ s1 * nstudents + s4 ]
		      + dislikes[ s2 * nstudents + s3 ]
			  - dislikes[ s1 * nstudents + s3 ]
			  - dislikes[ s2 * nstudents + s4 ]
			  ;

	//3.4. Accept or discard
#ifndef	SIMULATED_ANNEALING
		if ( dcost < 0 )
#else
		real r = random_real();
		real f = (double) (- dcost) / (double) t;
		real e = exp(f);
		if ( dcost < 0 || ( e >= r ) )
#endif
		{
	//3.4.1. IF accepted
	//3.4.1.1. Swap the roommates
			assigned[s3] = r2;
			assigned[s4] = r1;
			rooms[ r1 * 2 + p1 ] = s4;
			rooms[ r2 * 2 + p2 ] = s3;
	//3.4.1.2. Update the total cost
			cost += dcost;
	//3.4.1.3. Reset iterations
			i = max;
		}
		else
		{
	//3.4.2. OTHERWISE
	//3.4.2.1. Count one more boring iteration
			--i;
		}
#ifdef	SIMULATED_ANNEALING
		t *= 0.999;
#endif
	}

	//99. Cleanup
	free(assigned);

	return cost;
}

