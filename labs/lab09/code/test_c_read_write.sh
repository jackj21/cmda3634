#!/bin/bash

mkdir -p temp
mpirun -np 1 ./test_c_read_write temp

rm -rf temp