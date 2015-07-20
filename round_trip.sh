#!/usr/bin/env bash

export TARGET=round_trip_wl

for BLOCKS in 4 8 16 24 32 40 48 56 64
do
export BLOCKS

for ROUND_TRIPS in 4 8 16 32 64
do
export ROUND_TRIPS

### execute
if [ $(uname) == Linux ]; then
qsub -N ${TARGET}_${BLOCKS}_${ROUND_TRIPS} ./run.sh
else
cmake . && make ${TARGET} && ./${TARGET}
fi

done
done
