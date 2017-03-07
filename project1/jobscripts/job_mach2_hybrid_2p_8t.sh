#!/bin/bash

#PBS -N estimatepi_mach2_hybrid_2p_8t.sh
#PBS -A imf_tma4280
#PBS -l walltime=00:02:00
#PBS -l select=1:ncpus=20:mpiprocs=2:ompthreads=8

cd $PBS_O_WORKDIR
module load gcc openmpi
mpiexec ./TMA4280LABS/project1/mach2/hybrid/machvtest
