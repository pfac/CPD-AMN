#!/bin/bash
#
#PBS -N distribute
#PBS -l nodes=3:r601:ppn=24
#PBS -l walltime=2:00:00
#PBS -m bea
#PBS -V
#

EXEC="distribute"

if [ ! "$NFIRST" ]; then NFIRST="40"; fi;
if [ ! "$NLAST"  ]; then NLAST="200"; fi;
if [ ! "$NSTEP"  ]; then NSTEP="4";   fi;

if [ ! "$TFIRST" ]; then TFIRST="1"; fi;
if [ ! "$TLAST"  ]; then TLAST="1"; fi;
if [ ! "$TSTEP"  ]; then TSTEP="1"; fi;

echo "N from ${NFIRST} to ${NLAST} by ${NSTEP}";
echo "T from ${TFIRST} to ${TLAST} by ${TSTEP}";

cd "$PBS_O_WORKDIR";
PROCESSES=`cat "$PBS_NODEFILE" | wc -l`;

N="$NFIRST";
while [ "$N" -le "$NLAST" ];
do
	T="$TFIRST";
	while [ "$T" -le "$TLAST" ];
	do
		echo -n "(${N},${T})..." 1>&2;
		mpirun -np "$PROCESSES" -machinefile "$PBS_NODEFILE" -loadbalance "$EXEC" "$N" "$T";
		echo "DONE" 1>&2;
		T=$(( $T + $TSTEP ));
	done;
	N=$(( $N + $NSTEP ));
done;
