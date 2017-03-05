#!/bin/bash

#PBS -N estimatepi
#PBS -A imf_tma4280
#PBS -l walltime=00:01:00
#PBS -l select=1:ncpus=32:mpiprocs=16

cd $PBS_O_WORKDIR
./TMA4280LABS/project1/zeta0/machutest
./TMA4280LABS/project1/zeta0/estimatepi 1000000
./TMA4280LABS/project1/zeta0/machvtest
