#!/bin/bash
#$ -S /bin/bash
#$ -cwd
#$ -V

#$ -l h_rss=100M,h_fsize=100M,h_cpu=120:00:00

hostname

## force any error to stop the script
set -e

## starting directory is always the one we are at
startDir=`pwd`
## the directory that stores results
resultsPath=results

## End directory created on scratch; JOB_ID avoids collisions.
endDir=/scratch/$USER/$JOB_ID

echo Creating directory ${endDir}...
mkdir -p ${endDir}

## move to the server
cd $endDir

## clone the repository
echo Cloning ${startDir} to ${endDir}...
git clone ${startDir} .

## copy dependencies
ln -s ${startDir}/dependencies dependencies
mkdir -p ${resultsPath}

## compile the source
echo Compiling...
cmake . && make ${TARGET}

## run the executable
echo Running...
./${TARGET}

## compress results, move them back, clear the server
echo Compressing results...
tar -cf temp${JOB_ID}.tar $resultsPath/ && mv temp${JOB_ID}.tar $startDir && rm -rf $endDir

## return home and extract results
echo Sending results home...
cd $startDir && tar -xf temp${JOB_ID}.tar && rm temp${JOB_ID}.tar
