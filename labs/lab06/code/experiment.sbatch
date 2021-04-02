#!/bin/bash

#SBATCH --ntasks=??
#SBATCH --ntasks-per-node=??
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh

cd $SLURM_SUBMIT_DIR

#Load modules
module load gompi

bash experiment_strong_scalability.sh
bash experiment_weak_scalability.sh
