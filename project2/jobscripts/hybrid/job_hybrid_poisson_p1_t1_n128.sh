#!/bin/bash

#PBS -N mpi_poisson_p1_t1_n128
#PBS -A imf_tma4280
#PBS -l walltime=00:02:00
#PBS -l select=1:ncpus=20:mpiprocs=1:ompthreads=1

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project2/p_poisson/hybrid_poisson/build/poisson 128
