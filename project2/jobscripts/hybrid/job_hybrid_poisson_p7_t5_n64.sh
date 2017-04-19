#!/bin/bash

#PBS -N mpi_poisson_p7_t5_n64
#PBS -A imf_tma4280
#PBS -l walltime=00:02:00
#PBS -l select=4:ncpus=20:mpiprocs=7:ompthreads=5

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project2/p_poisson/hybrid_poisson/build/poisson 64
