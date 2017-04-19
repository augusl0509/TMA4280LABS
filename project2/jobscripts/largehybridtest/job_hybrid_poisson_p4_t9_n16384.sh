#!/bin/bash

#PBS -N mpi_poisson_p4_t9_n16384
#PBS -A imf_tma4280
#PBS -l walltime=00:10:00
#PBS -l select=6:ncpus=20:mpiprocs=4:ompthreads=9

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project2/p_poisson/hybrid_poisson/build/poisson 16384
