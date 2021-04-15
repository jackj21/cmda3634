#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --time=00:15:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh
#SBATCH --exclusive

#Change to the directory from which the job was submitted
cd $SLURM_SUBMIT_DIR

#Load modules
module reset
module load gompi

make clean
make all

bash time_normalize.sh
