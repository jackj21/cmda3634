#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

#include "vector.h"

int main(int argc, char *argv[]){

    if(argc < 3){
        printf("Usage: mpirun -np #PROCS axpy_vector_test N n_trials \n");
        printf("    N: vectors are Nx1\n");
        printf("    n_trials: Number of experiment trials to run\n");
        return 1;
    }

    MPI_Init(&argc, &argv);

    int n_procs;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get test vector size and number of trials
    int N = atoi(argv[1]);
    int n_trials = atoi(argv[2]);

    Vector x, y;

    // create a couple of vectors and fill with random values
    if(rank == 0){
        printf("axpy vector, %4d, %4d, %4d", n_procs, n_trials, N);
        allocate_Vector(&x, N);
        allocate_Vector(&y, N);
        rand_fill_Vector(&x);
        rand_fill_Vector(&y);
    }


    // create alpha, the scaling on one of the vectors
    float alpha= 1.5;

    // set up number of repeat runs, and declare timers
    double t_start, t_end;
    double t_total;

    // time the parallel axpy in-place operation
    // note: will be different y vector each iteration
    t_total = 0.0;

    MPI_Barrier(MPI_COMM_WORLD);

    for(int i=0; i<n_trials; i++){
        if(rank == 0){
            t_start = MPI_Wtime();
        }

        distributed_axpy(alpha, &x, &y);

        if(rank == 0){
            t_end = MPI_Wtime();
            t_total += t_end - t_start;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0){
        double t_avg = t_total / n_trials;
        printf(", %.10f \n", t_avg);
    }

    // cleanup
    if(rank == 0){
        deallocate_Vector(&x);
        deallocate_Vector(&y);
    }

    MPI_Finalize();

    return 0;
}
