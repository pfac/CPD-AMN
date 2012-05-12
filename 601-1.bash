#!/bin/bash
#
#PBS -N distribute
#PBS -l nodes=1:r601:ppn=24
#PBS -l walltime=05:00
#
#
#PBS -m bea
#PBS -e 601-1.err
#PBS -o 601-1.out
#

EXEC="distribute"

NSTART=40
NSTOP=200
NSTEP=4

cd "$PBS_O_WORKDIR";

N=$NSTART;
while [ "$N" -le "$NSTOP" ];
do
	mpirun -np 24 -machinefile $PBS_NODEFILE $EXEC "$N";
	N=$(( $N + $NSTEP ));
done;
