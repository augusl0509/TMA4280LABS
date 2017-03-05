#!/bin/bash

#PBS -N estimatepi_zeta2_omp
#PBS -A imf_tma4280
#PBS -l walltime=00:01:00
#PBS -l select=1:ncpus=32:ompthreads:16

export OMP_NUM_THREADS=16

cd $PBS_O_WORKDIR
./TMA4280LABS/project1/zeta2/omp/estimatepi 1000000
./TMA4280LABS/project1/zeta2/omp/zetavtest
