#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "vector.h"


int main(int argc, char** argv){

    MPI_Init(&argc, &argv);

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
        allocate_Vector(&v, N, 0, MPI_COMM_SELF);

        rand_fill_Vector(&v);

        char fname[500];
        sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);
        c_write_Vector(&v, fname);

        Vector u;

        c_read_Vector(&u, fname);

        int result = compare_Vector(&v, &u);

        if(result)
            printf("read/write match for N=%d\n", N);
        else
            printf("read/write failure for N=%d\n", N);

        deallocate_Vector(&v);
        deallocate_Vector(&u);
    }
}
