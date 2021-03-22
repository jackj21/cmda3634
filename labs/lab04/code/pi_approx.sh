#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --partition=interactive_q
#SBATCH --account=cmda3634_rjh

module reset
module load gcc/8.2.0

make pi_omp
