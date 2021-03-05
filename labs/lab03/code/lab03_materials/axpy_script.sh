#!/bin/bash

#Part 1: Tell the scheduler what resources we need
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --partition=normal_q
#SBATCH --account=cmda3634_rjh

#Part 2: Tell the scheduler what to do when it finds those resources

#Maybe change directories if you want (the deault is the directory
#from which the job was submitted)

#Load modules
module reset
module load gcc/8.2.0

#Compile (not strictly necessary if this has already been done)
make axpy_random
make axpy_normal

#Run

echo "Starting axpy_random..."
echo "n, time" > axpy_random_script.csv
for i in $(seq 0 19); do
	n=$((2**i * 100))
	./bin/axpy_random $n >> axpy_random_script.csv
done
echo "Ending axpy_random"

echo "Starting axpy_normal..."
echo "n, time" > axpy_normal_script.csv
for i in $(seq 0 19); do
	n=$((2**i * 100))
	./bin/axpy_normal $n >> axpy_normal_script.csv
done
echo "Ending axpy_normal"


