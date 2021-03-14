#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh

module reset
module load gcc/8.2.0

make main_def

echo "Starting main_def..."
echo "n, time" > main1.csv

./main_def 11 3 4 1 2.5 >> main1.csv
./main_def 51 3 4 1 2.5 >> main1.csv
./main_def 101 3 4 1 2.5 >> main1.csv
./main_def 201 3 4 1 2.5 >> main1.csv
./main_def 301 3 4 1 2.5 >> main1.csv
./main_def 501 3 4 1 2.5 >> main1.csv
./main_def 1001 3 4 1 2.5 >> main1.csv
./main_def 2501 3 4 1 2.5 >> main1.csv
echo "Ending main_def"
