#!/bin/bash

#PBS -N estimatepi_zeta1_16p
#PBS -A imf_tma4280
#PBS -l walltime=00:01:00
#PBS -l select=1:ncpus=20:mpiprocs=16

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project1/zeta1/estimatepi 1000000
mpiexec ./TMA4280LABS/project1/zeta1/zetavtest
