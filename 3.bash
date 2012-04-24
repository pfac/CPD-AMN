#!/bin/bash

EXEC=$1
OUTFILE=$2

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

i=80;
while [ "$i" -le "400" ];
do
	$EXEC $i >> "$OUTFILE";
	echo "$i";
	i=$(( $i + 8 ));
done;
