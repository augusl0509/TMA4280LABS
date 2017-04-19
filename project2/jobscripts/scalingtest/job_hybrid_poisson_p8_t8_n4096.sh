#!/bin/bash

#PBS -N mpi_poisson_p8_t8_n4096
#PBS -A imf_tma4280
#PBS -l walltime=00:10:00
#PBS -l select=8:ncpus=20:mpiprocs=8:ompthreads=8

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project2/p_poisson/hybrid_poisson/build/poisson 4096
