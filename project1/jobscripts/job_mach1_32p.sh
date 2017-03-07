#!/bin/bash

#PBS -N estimatepi_mach1_32p
#PBS -A imf_tma4280
#PBS -l walltime=00:02:00
#PBS -l select=2:ncpus=20:mpiprocs=32

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project1/mach1/machvtest
