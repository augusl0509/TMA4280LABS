#!/bin/bash

#PBS -N estimatepi_mach0
#PBS -A imf_tma4280
#PBS -l walltime=00:01:00
#PBS -l select=1:ncpus=32:mpiprocs=16

cd $PBS_O_WORKDIR
./TMA4280LABS/project1/mach0/machutest
./TMA4280LABS/project1/mach0/estimatepi 1000000
./TMA4280LABS/project1/mach0/machvtest
