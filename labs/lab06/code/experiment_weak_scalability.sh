#!/bin/bash

echo "weak scalability"
echo "=================="
echo "experiment, n_procs, n_trials, N, avg_time" > time_axpy_weak.csv
for i in $(seq 0 8); do
	n=$((2**i))
	mpirun -np $n ./time_distributed_axpy 10000000*$n 5 >> time_axpy_weak.csv
done
echo ""
echo ""
