all: experiment_vector experiment_matrix

experiment_vector:
	mkdir -p bin
	gcc experiment_vector.c vector.c matrix.c -fopenmp -o bin/experiment_vector -lm

experiment_matrix:
	mkdir -p bin
	gcc experiment_matrix.c vector.c matrix.c -fopenmp -o bin/experiment_matrix -lm

clean:
	rm -f cachegrind.*
	rm -f bin/*
