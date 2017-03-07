#!/bin/bash

#PBS -N estimatepi_mach0
#PBS -A imf_tma4280
#PBS -l walltime=00:05:00
#PBS -l select=1:ncpus=20

cd $PBS_O_WORKDIR
./TMA4280LABS/project1/mach0/machvtest
