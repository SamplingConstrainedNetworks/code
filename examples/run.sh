#!/usr/bin/env bash

export TARGET=generic
export METHOD=CanonicSampling
export BLOCKS=4

### compile and execute
g++ -std=c++11 examples/generic.cpp -Isource -o pp && ./pp
