#!/usr/bin/env bash

export TARGET=main

for METHOD in CanonicSampling WangLandau
do
export METHOD

for BLOCKS in 4 8 16 24 32 40 48 56 64
do
export BLOCKS

### execute
if [ $(uname) == Linux ]; then
qsub -N ${TARGET}_${METHOD}_${BLOCKS} ./run.sh
else
cmake . && make ${TARGET} && ./${TARGET}
fi

done
done
