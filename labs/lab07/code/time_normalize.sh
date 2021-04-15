#!/bin/bash

echo "strong scalability"
echo "=================="
echo "experiment, n_procs, manual, mpi, mpi2" > normalize_strong.csv

mpirun -np 1     ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 2     ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 4     ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 8     ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 16    ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 32    ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 64    ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 96    ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 128   ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 192   ./bin/time_normalize 1000000 >> normalize_strong.csv
mpirun -np 256   ./bin/time_normalize 1000000 >> normalize_strong.csv

echo ""
echo ""

echo "weak scalability"
echo "=================="
echo "experiment, n_procs, manual, mpi, mpi2"  > normalize_weak.csv

mpirun -np 1     ./bin/time_normalize 100000 >> normalize_weak.csv
mpirun -np 2     ./bin/time_normalize 200000 >> normalize_weak.csv
mpirun -np 4     ./bin/time_normalize 400000 >> normalize_weak.csv
mpirun -np 8     ./bin/time_normalize 800000 >> normalize_weak.csv
mpirun -np 16    ./bin/time_normalize 1600000 >> normalize_weak.csv
mpirun -np 32    ./bin/time_normalize 3200000 >> normalize_weak.csv
mpirun -np 64    ./bin/time_normalize 6400000 >> normalize_weak.csv
mpirun -np 96    ./bin/time_normalize 9600000 >> normalize_weak.csv
mpirun -np 128   ./bin/time_normalize 12800000 >> normalize_weak.csv
mpirun -np 192   ./bin/time_normalize 19200000 >> normalize_weak.csv
mpirun -np 256   ./bin/time_normalize 25600000 >> normalize_weak.csv
