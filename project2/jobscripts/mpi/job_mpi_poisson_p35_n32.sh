#!/bin/bash

#PBS -N mpi_poisson_p35_n32
#PBS -A imf_tma4280
#PBS -l walltime=00:02:00
#PBS -l select=4:ncpus=20:mpiprocs=35

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project2/p_poisson/mpi_poisson/build/poisson 32
