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

    float val = 1.0;
    float redval;

    log_sum_reduction(&val, &redval, MPI_COMM_WORLD);

    float redval_mpi;
    MPI_Reduce(&val, &redval_mpi, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0){
        if(redval != redval_mpi)
            printf("Error in reduction! %f vs %f\n", redval, redval_mpi);
        else
            printf("Success for n_procs=%d!\n", n_procs);
    }

    MPI_Finalize();

    return 0;
}
