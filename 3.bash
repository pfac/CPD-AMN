#!/bin/bash

EXEC=$1
OUTFILE=$2

NSTART=40
NSTOP=200
NSTEP=4

if [ -f "$OUTFILE" ];
then
	if [ -f "${OUTFILE}.all" ];
	then
		cat "$OUTFILE" >> "${OUTFILE}.all";
	else
		cp "$OUTFILE" "${OUTFILE}.all";
	fi;
	echo > "$OUTFILE";
else
	touch "$OUTFILE";
fi;

N=$NSTART;
while [ "$N" -le "$NSTOP" ];
do
	$EXEC "$N" >> "$OUTFILE";
	echo "$N";
	N=$(( $N + $NSTEP ));
done;
