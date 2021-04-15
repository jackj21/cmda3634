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
    Vector x_global, y_global, y_global_copy;
    Vector x_local, y_local;

    // Every rank gets a variable but only rank 0 maintains
    // the global state
    if(rank == 0){
        allocate_Vector(&x_global, N, MPI_COMM_SELF);
        allocate_Vector(&y_global, N, MPI_COMM_SELF);
        allocate_Vector(&y_global_copy, N, MPI_COMM_SELF);

        rand_fill_Vector(&x_global);
        rand_fill_Vector(&y_global);
        copy_Vector(&y_global, &y_global_copy);
    }

    // Every rank has a local segment to work on
    allocate_Vector(&x_local, N, MPI_COMM_WORLD);
    allocate_Vector(&y_local, N, MPI_COMM_WORLD);

    // Scatter global vectors
    scatter_Vector(&x_global, &x_local);
    scatter_Vector(&y_global, &y_local);

    // Do the work
    axpy(1.5, &x_local, &y_local);

    // Gather distributed vectors
    gather_Vector(&y_local, &y_global);

    if (rank == 0){
        axpy(1.5, &x_global, &y_global_copy);

        printf("Serial:\n");
        print_Vector(&y_global);
        printf("\n");
        printf("Distributed:\n");
        print_Vector(&y_global);
    }

    // cleanup
    if(rank == 0){
        deallocate_Vector(&x_global);
        deallocate_Vector(&y_global);
        deallocate_Vector(&y_global_copy);
    }

    deallocate_Vector(&x_local);
    deallocate_Vector(&y_local);

    MPI_Finalize();

    return 0;
}
