#!/bin/bash

pid=`whoami`

#home_path=/home/$pid/temp
work_path=/work/$pid/temp

# Do not run this experment!
# MPI write to home is currently unstable.
# echo "Testing operations on $home_path"
# for P in 1 2 4 8 16 32 64
# do
# 	mkdir -p $home_path
# 	mpirun -np $P ./timing $home_path
# 	rm -rf $home_path
# done

echo "Testing operations on $work_path"
for P in 1 2 4 8 16 32 64
do
	mkdir -p $work_path
	mpirun -np $P ./timing $work_path
	rm -rf $work_path
done
