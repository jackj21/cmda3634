#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <math.h>

#include <mpi.h>

#include "vector.h"

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("WARNING: Missing arguments. \n");
        printf("Usage: mpirun -np #PROCS axpy_vector_test N  \n");
        printf("    N: vectors are Nx1\n");
        return 1;
    }

    MPI_Init(&argc, &argv);

    int rank, n_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = atoi(argv[1]);
    Vector x_global, x_global_manual, x_global_mpi2, x_global_mpi1;
    Vector x_local_manual, x_local_mpi2, x_local_mpi1;

    // Every rank gets a variable but only rank 0 maintains
    // the global state
    if(rank == 0){
        allocate_Vector(&x_global, N, MPI_COMM_SELF);
        allocate_Vector(&x_global_manual, N, MPI_COMM_SELF);
        allocate_Vector(&x_global_mpi2, N, MPI_COMM_SELF);
        allocate_Vector(&x_global_mpi1, N, MPI_COMM_SELF);

        rand_fill_Vector(&x_global);
        zero_fill_Vector(&x_global_manual);
        zero_fill_Vector(&x_global_mpi2);
        zero_fill_Vector(&x_global_mpi1);
    }

    // Every rank has a local segment to work on
    allocate_Vector(&x_local_manual, N, MPI_COMM_WORLD);
    allocate_Vector(&x_local_mpi2, N, MPI_COMM_WORLD);
    allocate_Vector(&x_local_mpi1, N, MPI_COMM_WORLD);

    // Scatter global vectors
    scatter_Vector(&x_global, &x_local_manual);
    scatter_Vector(&x_global, &x_local_mpi2);
    scatter_Vector(&x_global, &x_local_mpi1);

    // Do the work

    int n_trials = 10;
    double tt_manual = 0.0;
    double tt_mpi2 = 0.0;
    double tt_mpi1 = 0.0;
    double time_manual = 0.0;
    double time_mpi2 = 0.0;
    double time_mpi1 = 0.0;


    MPI_Barrier(MPI_COMM_WORLD);
    for (int i=0; i<n_trials; i++){
        tt_manual = MPI_Wtime();
        normalize_manual(&x_local_manual);
        time_manual += MPI_Wtime() - tt_manual;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    for (int i=0; i<n_trials; i++){
        tt_mpi2 = MPI_Wtime();
        normalize_mpi2(&x_local_mpi2);
        time_mpi2 += MPI_Wtime() - tt_mpi2;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    for (int i=0; i<n_trials; i++){
        tt_mpi1 = MPI_Wtime();
        normalize_mpi1(&x_local_mpi1);
        time_mpi1 += MPI_Wtime() - tt_mpi1;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0){

        printf("%d, %d, %.10f, %.10f, %.10f\n", N, n_procs, time_manual, time_mpi2, time_mpi1);

    }

    // cleanup
    if(rank == 0){
        deallocate_Vector(&x_global);
        deallocate_Vector(&x_global_manual);
        deallocate_Vector(&x_global_mpi2);
        deallocate_Vector(&x_global_mpi1);
    }

    deallocate_Vector(&x_local_manual);
    deallocate_Vector(&x_local_mpi2);
    deallocate_Vector(&x_local_mpi1);

    MPI_Finalize();

    return 0;
}
