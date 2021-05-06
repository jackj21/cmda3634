#!/bin/bash

#SBATCH --nodes=4
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=00:60:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh

#Change to the directory from which the job was submitted
cd $SLURM_SUBMIT_DIR

#Load modules
module reset
module load gompi

make clean
make all

bash timing_study.sh
