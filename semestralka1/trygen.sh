#!/bin/bash

WORKDIR="/media/synology/School/MI/PAR/semestralka1"
GEN="${WORKDIR}/bin/par_gen"
SOLVE="${WORKDIR}/bin/par_solve"
ARGS="10 10"

OUTPUTFILE="${WORKDIR}/runs/"$(date +%Y%m%d_%H%M%S)
INPUTFILE="${WORKDIR}/inst/"$(date +%Y%m%d_%H%M%S)

# Generate
$GEN $INPUTFILE $ARGS

# Solve
echo "Command: $SOLVE $INPUTFILE" > $OUTPUTFILE
echo "---" >> $OUTPUTFILE
(time $SOLVE $INPUTFILE) 1>> $OUTPUTFILE 2>>$OUTPUTFILE
echo "---" >> $OUTPUTFILE
