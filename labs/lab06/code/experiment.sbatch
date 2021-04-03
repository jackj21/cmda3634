#!/bin/bash

#SBATCH --nodes=2
#SBATCH --ntasks-per-node=128
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh

cd $SLURM_SUBMIT_DIR

#Load modules
module load gompi

make clean
make time_distributed_axpy 

bash experiment_strong_scalability.sh
bash experiment_weak_scalability.sh
