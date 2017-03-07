#!/bin/bash

#PBS -N estimatepi_zeta0
#PBS -A imf_tma4280
#PBS -l walltime=00:01:00
#PBS -l select=1:ncpus=20

cd $PBS_O_WORKDIR
./TMA4280LABS/project1/zeta0/zetavtest
