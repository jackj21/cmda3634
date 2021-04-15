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
    Vector x_global, y_global;
    Vector x_local, y_local;

    // Every rank gets a variable but only rank 0 maintains
    // the global state
    if(rank == 0){
        allocate_Vector(&x_global, N, MPI_COMM_SELF);
        allocate_Vector(&y_global, N, MPI_COMM_SELF);

        rand_fill_Vector(&x_global);
        rand_fill_Vector(&y_global);
    }

    // Every rank has a local segment to work on
    allocate_Vector(&x_local, N, MPI_COMM_WORLD);
    allocate_Vector(&y_local, N, MPI_COMM_WORLD);

    // Scatter global vectors
    scatter_Vector(&x_global, &x_local);
    scatter_Vector(&y_global, &y_local);

    // Do the work
    float ip_man_distributed, ip_mpi_distributed;
    inner_product_manual(&x_local, &y_local, &ip_man_distributed);
    inner_product_mpi(&x_local, &y_local, &ip_mpi_distributed);

    if (rank == 0){
        float ip_man_global, ip_mpi_global;
        inner_product_manual(&x_global, &y_global, &ip_man_global);
        inner_product_mpi(&x_global, &y_global, &ip_mpi_global);

        // You may observe differences in the distributed and serial solutions
        // Floating point arithmetic is not associative!
        printf("Serial manual:      %f\n", ip_man_global);
        printf("Serial mpi:         %f\n", ip_mpi_global);
        printf("Distributed manual: %f\n", ip_man_distributed);
        printf("Distributed mpi:    %f\n", ip_mpi_distributed);
    }

    // cleanup
    if(rank == 0){
        deallocate_Vector(&x_global);
        deallocate_Vector(&y_global);
    }

    deallocate_Vector(&x_local);
    deallocate_Vector(&y_local);

    MPI_Finalize();

    return 0;
}
