#!/bin/bash

echo "weak scalability"
echo "=================="
echo "experiment, n_procs, n_trials, N, avg_time"
mpirun -np 1 ./time_distributed_axpy 10000000 5

echo ""
echo ""
