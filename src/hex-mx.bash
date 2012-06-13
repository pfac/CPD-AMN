#!/bin/bash
#
#PBS -l walltime=6:00:00
#
#PBS -m bea
#PBS -V
#

EXEC="distribute"

NSTART=40
NSTOP=200
NSTEP=4

cd "$PBS_O_WORKDIR";
NODES=`cat "$PBS_NODEFILE" | uniq | wc -l`;
PROCESSES=`cat "$PBS_NODEFILE" | wc -l`;
PROCESSES=1;

sort "$PBS_NODEFILE" | uniq > "mpd.hosts";
mpdboot --totalnum="$NODES" -f "$PBS_NODEFILE" --file="mpd.hosts";
N=$NSTART;
while [ "$N" -le "$NSTOP" ];
do
	echo -n "${N}..." 1>&2;
	mpiexec -n "$PROCESSES" "$EXEC" "$N";
	echo "DONE" 1>&2;
	N=$(( $N + $NSTEP ));
done;
rm "mpd.hosts";
