#!/usr/bin/env bash

for BLOCKS in 64
do
export BLOCKS

for ROUND_TRIPS in 4 8 16 32 64
do
export ROUND_TRIPS

### compile and execute
g++ -std=c++11 examples/entropy.cpp -Isource -o pp && ./pp

done
done
