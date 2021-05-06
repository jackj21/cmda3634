#!/bin/bash

pid=`whoami`

work_path=/work/$pid/temp

mkdir -p $work_path
mpirun -np 1 ./test_mpi_read_write $work_path
rm -rf $work_path

mkdir -p $work_path
mpirun -np 2 ./test_mpi_read_write $work_path
rm -rf $work_path

mkdir -p $work_path
mpirun -np 3 ./test_mpi_read_write $work_path
rm -rf $work_path

mkdir -p $work_path
mpirun -np 4 ./test_mpi_read_write $work_path
rm -rf $work_path
