#!/bin/bash

#PBS -N mpi_poisson_p4_t4_n64
#PBS -A imf_tma4280
#PBS -l walltime=00:02:00
#PBS -l select=2:ncpus=20:mpiprocs=4:ompthreads=4

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project2/p_poisson/hybrid_poisson/build/poisson 64
