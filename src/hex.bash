#!/bin/bash
#
#PBS -N distribute
#PBS -l nodes=3:r601:ppn=24
#PBS -l walltime=2:00:00
#PBS -m bea
#PBS -V
#

EXEC="distribute"

NSTART=40
NSTOP=200
NSTEP=4

cd "$PBS_O_WORKDIR";
PROCESSES=`cat "$PBS_NODEFILE" | wc -l`;

N=$NSTART;
while [ "$N" -le "$NSTOP" ];
do
	#mpirun -np $PROCESSES -machinefile $PBS_NODEFILE -loadbalance $EXEC "$N";
	mpirun -np 4 -machinefile $PBS_NODEFILE -loadbalance $EXEC "$N";
	N=$(( $N + $NSTEP ));
done;
