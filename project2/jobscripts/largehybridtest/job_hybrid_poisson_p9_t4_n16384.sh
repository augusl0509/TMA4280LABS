#!/bin/bash

#PBS -N mpi_poisson_p9_t4_n16384
#PBS -A imf_tma4280
#PBS -l walltime=00:10:00
#PBS -l select=5:ncpus=20:mpiprocs=9:ompthreads=4

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project2/p_poisson/hybrid_poisson/build/poisson 16384
