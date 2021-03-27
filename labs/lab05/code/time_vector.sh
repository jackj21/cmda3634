#!/bin/bash

echo "strong scalability"
echo "=================="
echo "experiment, n_threads, n_trials, N, axpy, inner_product" > vector_strong.csv

N=10000000
NTRIALS=10

for i in {1..8}
do
	./bin/experiment_vector $N $NTRIALS $i 0 >> vector_strong.csv
done

echo ""
echo ""

echo "weak scalability"
echo "=================="
echo "experiment, n_threads, n_trials, N, axpy, inner_product" > vector_weak.csv

for i in {1..8}
do
	./bin/experiment_vector $N $NTRIALS $i 1 >> vector_weak.csv
done

