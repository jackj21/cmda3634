#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <math.h>

#include <mpi.h>

#include "vector.h"

int main(int argc, char *argv[]){

    MPI_Init(&argc, &argv);

    int rank, n_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float val, val_mpi;

    if (rank == 0){
        val = 100.123;
        val_mpi = val;
    }

    log_broadcast(&val, MPI_COMM_WORLD);

    MPI_Bcast(&val_mpi, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if(val != val_mpi)
        printf("Error in broadcast! %f vs %f on rank %d\n", val, val_mpi, rank);
    else
        printf("Success for n_procs=%d on rank %d!\n", n_procs, rank);

    MPI_Finalize();

    return 0;
}
