#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#include "vector.h"

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Usage: mpirun -np #PROCS test_print_vector N \n");
        printf("    N: vectors are Nx1\n");
        return 1;
    }

    MPI_Init(&argc, &argv);

    int n_procs;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get test vector size
    int N = atoi(argv[1]);

    // Note: each rank will have this global vector, but only rank 0's is meaningful
    Vector x;

    // Create a random vector
    if(rank == 0){
        allocate_Vector(&x, N);
        rand_fill_Vector(&x);

        // For verification, print the results serially
        printf("Serial printout of vector with %d entries \n",N);
        for(int i=0; i<N; ++i)
            printf("%.10f \n", x.data[i]);
    }

    // split up the overall rank 0 vector into smaller vectors
    Vector x_local;
    scatter_Vector(&x, &x_local);

    if(rank == 0){
        printf("MPI printout of vector with %d entries over %d processes \n", N, n_procs);
    }

    // Sync for safety
    MPI_Barrier(MPI_COMM_WORLD);
    distributed_print_Vector(&x_local);

    // Clean up
    if(rank == 0){
        deallocate_Vector(&x);
    }
    deallocate_Vector(&x_local);

    MPI_Finalize();

    return 0;
}
