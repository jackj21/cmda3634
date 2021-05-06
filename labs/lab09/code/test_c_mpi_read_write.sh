#!/bin/bash

mkdir -p temp
mpirun -np 1 ./test_c_mpi_read_write temp
rm -rf temp

mkdir -p temp
mpirun -np 2 ./test_c_mpi_read_write temp
rm -rf temp

mkdir -p temp
mpirun -np 3 ./test_c_mpi_read_write temp
rm -rf temp

mkdir -p temp
mpirun -np 4 ./test_c_mpi_read_write temp
rm -rf temp