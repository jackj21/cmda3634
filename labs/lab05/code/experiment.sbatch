#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --time=00:30:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR

#Load modules
module load gompi

bash time_vector.sh
bash time_matrix.sh
