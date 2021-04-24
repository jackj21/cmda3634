#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "vector.h"

int print_data(float* data, int N){

    int rank, n_procs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

    char output[50000];
    memset(output, 0, 50000);

    int k = 0;
    for(int j=0; j < N; j++){
        k += sprintf(output+k, "%.10f\n", data[j]);
    }
    sprintf(output+k, "\n");


    MPI_Barrier(MPI_COMM_WORLD);

    for(int i=0; i<n_procs; i++){
        if (rank == i){
            printf("%s", output);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    usleep(300);
    fflush(stdout);
    MPI_Barrier(MPI_COMM_WORLD);

}



int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    if(argc < 2){
        printf("Should call as executable <number-of-entries-in-global-array> <window-size> \n");
        return 1;
    }

    int N = atoi(argv[1]);
    int k = atoi(argv[2]);
    if (!(k%2)){
        printf("Window size must be odd.\n");
        return 1;
    }
    int padding = (k-1) / 2;

    int rank, n_procs;
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Number of entries in local array (unpadded)
    int N_local;

    // Index of local array in global array
    int r0;

    // Sanity check / debugging help
    compute_subvector_size(N, n_procs, rank, &N_local, &r0);
    printf("Rank %d of %d processes gets %d entries of a global %d-entry array starting with the %d entry \n", rank, n_procs, N_local, N, r0);
    fflush(stdout);

    Vector v;
    Vector avg_vec;

    allocate_Vector(&v, N, padding, MPI_COMM_WORLD);
    allocate_Vector(&avg_vec, N, padding, MPI_COMM_WORLD);

    initialize_Vector(&v);
    initialize_Vector(&avg_vec);

    // Fill the vector with increasing values
    float *data = (float*)malloc(N_local*sizeof(float));
    for(int j=0; j<N_local; ++j) {
        data[j] = r0+j;
    }

    // Print the initial data
    if (rank == 0)
        printf("Before raw data\n--------------\n");
    print_data(data, N_local);

    MPI_Barrier(MPI_COMM_WORLD);

    // Copy data into vector
    inject_unpadded_data_into_Vector(data, &v);

    // Apply mirror boundary condition
    update_padding_mirror_Vector(&v);

    if (rank == 0)
        printf("Before Exchange\n-----------\n");
    print_Vector(&v);

    // Apply ghost exchange
    update_padding_ghost_exchange_Vector(&v);

    if (rank == 0)
        printf("After Exchange\n-----------\n");
    print_Vector(&v);

    // Apply windowed average operation
    average(&v, &avg_vec, k);

    // Copy data from average vector
    extract_unpadded_data_from_Vector(data, &avg_vec);

    MPI_Barrier(MPI_COMM_WORLD);

    // Print the average data
    if (rank == 0)
        printf("Average data\n-----------\n");
    print_data(data, N_local);

    // Clean up
    free(data);
    deallocate_Vector(&v);
    deallocate_Vector(&avg_vec);

    MPI_Finalize();
}
