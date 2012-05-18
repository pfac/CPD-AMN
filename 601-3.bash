#!/bin/bash
#
#PBS -N distribute
#PBS -l nodes=3:r601:ppn=24
#PBS -l walltime=05:00
#
#
#PBS -m bea
#PBS -e 601-3.err
#PBS -o 601-3.out
#
#PBS -V
#

EXEC="distribute"

NSTART=40
NSTOP=200
NSTEP=4

cd "$PBS_O_WORKDIR";

N=$NSTART;
while [ "$N" -le "$NSTOP" ];
do
	mpirun -np 72 -machinefile $PBS_NODEFILE -loadbalance $EXEC "$N";
	N=$(( $N + $NSTEP ));
done;
