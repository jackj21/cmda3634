#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "vector.h"


int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(argc < 2){
        printf("Should call as executable <path> \n");
        return 1;
    }

    srand(time(0));

    char* path = argv[1];

    int N = atoi(argv[1]);

    int sizes[3] = {10, 1000, 1000000};
    int logsizes[3] = {1, 3, 6};

    for(int i=0; i<3; i++){
        Vector v;
        int N = sizes[i];
        allocate_Vector(&v, N, 0, MPI_COMM_WORLD);

        rand_fill_Vector(&v);

        char fname[500];
        sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);
        c_mpi_write_Vector(&v, fname);

        Vector u;

        c_mpi_read_Vector(&u, fname, v.comm);

        int result = compare_Vector(&v, &u);

        if(result)
            printf("rank %d: read/write match for N=%d\n", rank, N);
        else
            printf("rank %d: read/write failure for N=%d\n", rank, N);

        deallocate_Vector(&v);
        deallocate_Vector(&u);
    }

    MPI_Finalize();
}
