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
./bin/axpy_random 100 
./bin/axpy_normal 200

echo "Starting axpy_random..."
echo "n, time" > axpy_random.csv
n=100
for i in $(seq 8); do
	./bin/axpy_random $n >> axpy_random.csv
	n=$(($n*2))
done
echo "Ending axpy_random"

echo "Starting axpy_normal..."
echo "n, time" > axpy_normal.csv
n=100
for i in $(seq 8); do
	./bin/axpy_normal $n >> axpy_normal.csv
	n=$(($n*2))
done
echo "Ending axpy_normal"



