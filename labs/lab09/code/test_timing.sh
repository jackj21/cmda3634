#!/bin/bash


pid=`jackj21`

work_path=/work/$pid/temp

echo "Testing timing to $work_path"

mkdir -p $work_path
mpirun -np 1 ./timing $work_path
rm -rf $work_path

mkdir -p $work_path
mpirun -np 2 ./timing $work_path
rm -rf $work_path

mkdir -p $work_path
mpirun -np 3 ./timing $work_path
rm -rf $work_path

mkdir -p $work_path
mpirun -np 4 ./timing $work_path
rm -rf $work_path
