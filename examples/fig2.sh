#!/usr/bin/env bash

for BLOCKS in 4 8 16 24 32 40 48 56 64
do
export BLOCKS

for ROUND_TRIPS in 4 8 16 32 64
do
export ROUND_TRIPS

### compile and execute
g++ -std=c++11 examples/round_trip_wl.cpp -Isource -o pp && ./pp

done
done
