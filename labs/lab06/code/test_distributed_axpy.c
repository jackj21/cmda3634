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

    Vector x, y;

    if(rank == 0){
        allocate_Vector(&x, N);
        allocate_Vector(&y, N);
        rand_fill_Vector(&x);
        rand_fill_Vector(&y);
    }

    float alpha = 1.5;

    // on rank 0, do serial axpy
    Vector serial_out;
    if(rank == 0){
        allocate_Vector(&serial_out, N);
        zero_fill_Vector(&serial_out);
        axpy(alpha, &x, &y, &serial_out);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Call parallel axpy in-place operation, which overwrites y with alpha * x + y
    distributed_axpy(alpha, &x, &y);

    // check correctness (relative Frobenius norm of error) between serial and parallel in-place version, just on rank 0
    if(rank == 0){
        float relErr = 0;
        float normSer = 0; // serial output frobenius norm
        float normPar = 0; // parallel output frobenius norm
        for(int i=0; i<N; ++i){
            normSer += (serial_out.data[i])*(serial_out.data[i]);
            normPar += (y.data[i])*(y.data[i]);
            relErr += (serial_out.data[i]-y.data[i])*(serial_out.data[i]-y.data[i]);
        }

        normSer = sqrt(normSer);
        normPar = sqrt(normPar);

        if(normSer > 0){
                relErr = sqrt(relErr)/normSer;
        }

        printf("test for correctness with %4d entries and %4d procs\n"
               "   Relative error %.10f\n"
               "   Serial norm %.10f\n"
               "   Parallel norm %.10f\n", N, n_procs, relErr, normSer, normPar);
    }

    // cleanup
    if(rank == 0){
        deallocate_Vector(&x);
        deallocate_Vector(&y);
        deallocate_Vector(&serial_out);
    }

    MPI_Finalize();

    return 0;
}
