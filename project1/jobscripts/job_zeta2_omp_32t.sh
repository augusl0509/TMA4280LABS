#!/bin/bash

#PBS -N estimatepi_zeta2_omp_32t
#PBS -A imf_tma4280
#PBS -l walltime=00:01:00
#PBS -l select=2:ncpus=20:ompthreads=32

cd $PBS_O_WORKDIR
module load gcc
./TMA4280LABS/project1/zeta2/omp/zetavtest
