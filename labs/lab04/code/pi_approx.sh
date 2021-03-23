#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=64
#SBATCH --time=00:30:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh

module reset
module load gcc/8.2.0

make pi_omp

echo "Starting pi_omp..."
echo "T, N, PI, Time" > pi_omp_script.csv

./pi_omp 1 $((2**10)) >> pi_omp_script.csv
./pi_omp 1 $((2**13)) >> pi_omp_script.csv
./pi_omp 1 $((2**16)) >> pi_omp_script.csv
./pi_omp 1 $((2**19)) >> pi_omp_script.csv
./pi_omp 1 $((2**22)) >> pi_omp_script.csv
./pi_omp 1 $((2**25)) >> pi_omp_script.csv
./pi_omp 1 $((2**28)) >> pi_omp_script.csv
./pi_omp 1 $((2**31)) >> pi_omp_script.csv

./pi_omp 2 $((2**10)) >> pi_omp_script.csv
./pi_omp 2 $((2**13)) >> pi_omp_script.csv
./pi_omp 2 $((2**16)) >> pi_omp_script.csv
./pi_omp 2 $((2**19)) >> pi_omp_script.csv
./pi_omp 2 $((2**22)) >> pi_omp_script.csv
./pi_omp 2 $((2**25)) >> pi_omp_script.csv
./pi_omp 2 $((2**28)) >> pi_omp_script.csv
./pi_omp 2 $((2**31)) >> pi_omp_script.csv

./pi_omp 4 $((2**10)) >> pi_omp_script.csv
./pi_omp 4 $((2**13)) >> pi_omp_script.csv
./pi_omp 4 $((2**16)) >> pi_omp_script.csv
./pi_omp 4 $((2**19)) >> pi_omp_script.csv
./pi_omp 4 $((2**22)) >> pi_omp_script.csv
./pi_omp 4 $((2**25)) >> pi_omp_script.csv
./pi_omp 4 $((2**28)) >> pi_omp_script.csv
./pi_omp 4 $((2**31)) >> pi_omp_script.csv

./pi_omp 8 $((2**10)) >> pi_omp_script.csv
./pi_omp 8 $((2**13)) >> pi_omp_script.csv
./pi_omp 8 $((2**16)) >> pi_omp_script.csv
./pi_omp 8 $((2**19)) >> pi_omp_script.csv
./pi_omp 8 $((2**22)) >> pi_omp_script.csv
./pi_omp 8 $((2**25)) >> pi_omp_script.csv
./pi_omp 8 $((2**28)) >> pi_omp_script.csv
./pi_omp 8 $((2**31)) >> pi_omp_script.csv

./pi_omp 16 $((2**10)) >> pi_omp_script.csv
./pi_omp 16 $((2**13)) >> pi_omp_script.csv
./pi_omp 16 $((2**16)) >> pi_omp_script.csv
./pi_omp 16 $((2**19)) >> pi_omp_script.csv
./pi_omp 16 $((2**22)) >> pi_omp_script.csv
./pi_omp 16 $((2**25)) >> pi_omp_script.csv
./pi_omp 16 $((2**28)) >> pi_omp_script.csv
./pi_omp 16 $((2**31)) >> pi_omp_script.csv

./pi_omp 32 $((2**10)) >> pi_omp_script.csv
./pi_omp 32 $((2**13)) >> pi_omp_script.csv
./pi_omp 32 $((2**16)) >> pi_omp_script.csv
./pi_omp 32 $((2**19)) >> pi_omp_script.csv
./pi_omp 32 $((2**22)) >> pi_omp_script.csv
./pi_omp 32 $((2**25)) >> pi_omp_script.csv
./pi_omp 32 $((2**28)) >> pi_omp_script.csv
./pi_omp 32 $((2**31)) >> pi_omp_script.csv

./pi_omp 48 $((2**10)) >> pi_omp_script.csv
./pi_omp 48 $((2**13)) >> pi_omp_script.csv
./pi_omp 48 $((2**16)) >> pi_omp_script.csv
./pi_omp 48 $((2**19)) >> pi_omp_script.csv
./pi_omp 48 $((2**22)) >> pi_omp_script.csv
./pi_omp 48 $((2**25)) >> pi_omp_script.csv
./pi_omp 48 $((2**28)) >> pi_omp_script.csv
./pi_omp 48 $((2**31)) >> pi_omp_script.csv

./pi_omp 64 $((2**10)) >> pi_omp_script.csv
./pi_omp 64 $((2**13)) >> pi_omp_script.csv
./pi_omp 64 $((2**16)) >> pi_omp_script.csv
./pi_omp 64 $((2**19)) >> pi_omp_script.csv
./pi_omp 64 $((2**22)) >> pi_omp_script.csv
./pi_omp 64 $((2**25)) >> pi_omp_script.csv
./pi_omp 64 $((2**28)) >> pi_omp_script.csv
./pi_omp 64 $((2**31)) >> pi_omp_script.csv

