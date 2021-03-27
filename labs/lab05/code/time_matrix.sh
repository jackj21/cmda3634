#!/bin/bash

echo "strong scalability"
echo "=================="
echo "experiment, n_threads, n_trials, N, matvec, matvec_triangular, matvec_triangular guided" > matrix_strong.csv

N=5000
NTRIALS=3

for i in {1..8}
do
	./bin/experiment_matrix $N $NTRIALS $i 0 >> matrix_strong.csv
done

echo ""
echo ""

echo "weak scalability"
echo "=================="
echo "experiment, n_threads, n_trials, N, matvec, matvec_triangular, matvec_triangular guided" > matrix_weak.csv

for i in {1..8}
do
	./bin/experiment_matrix $N $NTRIALS $i 1 >> matrix_weak.csv
done
