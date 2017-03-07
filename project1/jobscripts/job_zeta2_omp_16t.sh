#!/bin/bash

#PBS -N estimatepi_zeta2_omp_16t
#PBS -A imf_tma4280
#PBS -l walltime=00:01:00
#PBS -l select=1:ncpus=20:ompthreads=16

cd $PBS_O_WORKDIR
module load gcc
./TMA4280LABS/project1/zeta2/omp/zetavtest
