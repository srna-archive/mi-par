#!/bin/bash

WORKDIR="/media/synology/School/MI/PAR/semestralka1"

for i in $(seq 1 8); do
	sleep 2
	$WORKDIR/trygen.sh &
done
