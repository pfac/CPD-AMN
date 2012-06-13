#!/bin/bash
#
#PBS -l walltime=2:00:00
#
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

if [ ! "$RUNS" ]; then RUNS="1"; fi;

cd "$PBS_O_WORKDIR";
PROCESSES=`cat "$PBS_NODEFILE" | wc -l`;

T="$TSTART";
while [ "$T" -le "$TSTOP" ];
do
	echo "$T";
	N="$NSTART";
	while [ "$N" -le "$NSTOP" ];
	do
		R="1";
		RESULTS="$N";
		while [ "$R" -le "$RUNS" ];
		do
			echo -n "(${N},${T})..." 1>&2;
			RESTMP=`$EXEC "$N" "$T"`;
			RESULTS="${RESULTS};${RESTMP}";
			echo "DONE" 1>&2;
			R=$(( $R + 1 ));
		done;
		echo "$RESULTS";
		unset RESULTS;
		N=$(( $N + $NSTEP ));
	done;
	echo;
	T=$(( $T + $TSTEP ));
done;
