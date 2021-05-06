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


    int rank, n_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);


    srand(time(0));

    char* path = argv[1];

    int N = atoi(argv[1]);

    const int n_sizes = 5;
    int sizes[5] = {512, 1024, 32768, 1048576, 33554432};
    int logsizes[5] = {5, 10, 15, 20, 25};

    int n_trials = 5;

    if((rank == 0) && (n_procs == 1)){

        double c_write_start[n_sizes], c_write_stop[n_sizes], c_write_time[n_sizes];

        for (int k=0; k<n_trials; k++){
            for(int i=0; i<n_sizes; i++){
                Vector v;
                int N = sizes[i];
                allocate_Vector(&v, N, 0, MPI_COMM_SELF);

                rand_fill_Vector(&v);

                char fname[500];
                sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);

                c_write_start[i] = MPI_Wtime();
                c_write_Vector(&v, fname);
                c_write_stop[i] = MPI_Wtime();
                c_write_time[i] += c_write_stop[i] - c_write_start[i];

                deallocate_Vector(&v);
            }
        }

        printf("c write,     %d ", n_procs);
        for(int i=0; i<n_sizes; i++){
            c_write_time[i] /= n_trials;
            printf("%f,\t", c_write_time[i]);
        }
        printf("\n");

        double c_read_start[n_sizes], c_read_stop[n_sizes], c_read_time[n_sizes];

        for (int k=0; k<n_trials; k++){
            for(int i=0; i<n_sizes; i++){
                Vector v;
                int N = sizes[i];

                char fname[500];
                sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);

                c_read_start[i] = MPI_Wtime();
                c_read_Vector(&v, fname);
                c_read_stop[i] = MPI_Wtime();
                c_read_time[i] += c_read_stop[i] - c_read_start[i];

                deallocate_Vector(&v);
            }
        }

        printf("c read,       %d ", n_procs);
        for(int i=0; i<n_sizes; i++){
            c_read_time[i] /= n_trials;
            printf("%f,\t", c_read_time[i]);
        }
        printf("\n");
    }


//    if(n_procs > 1){

//    double c_mpi_write_start[n_sizes], c_mpi_write_stop[n_sizes], c_mpi_write_time[n_sizes];

  //  for (int k=0; k<n_trials; k++){
    //    for(int i=0; i<n_sizes; i++){
      //      Vector v;
        //    int N = sizes[i];
          //  allocate_Vector(&v, N, 0, MPI_COMM_WORLD);

//            rand_fill_Vector(&v);

  //          char fname[500];
    //        sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);

      //      MPI_Barrier(MPI_COMM_WORLD);
        //    c_mpi_write_start[i] = MPI_Wtime();
        //    c_mpi_write_Vector(&v, fname);
        //    MPI_Barrier(MPI_COMM_WORLD);
        //    c_mpi_write_stop[i] = MPI_Wtime();
        //    c_mpi_write_time[i] += c_mpi_write_stop[i] - c_mpi_write_start[i];

        //    deallocate_Vector(&v);
        //}
    //}

    //if(rank == 0){
    //    printf("c-mpi write,  %d ", n_procs);
    //    for(int i=0; i<n_sizes; i++){
    //        c_mpi_write_time[i] /= n_trials;
    //        printf("%f,\t", c_mpi_write_time[i]);
    //    }
    //    printf("\n");
    //}

    //double c_mpi_read_start[n_sizes], c_mpi_read_stop[n_sizes], c_mpi_read_time[n_sizes];

    //for (int k=0; k<n_trials; k++){
    //    for(int i=0; i<n_sizes; i++){
    //        Vector v;
    //        int N = sizes[i];

      //      char fname[500];
      //      sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);

        //    MPI_Barrier(MPI_COMM_WORLD);
        //    c_mpi_read_start[i] = MPI_Wtime();
        //    c_mpi_read_Vector(&v, fname, MPI_COMM_WORLD);
        //    MPI_Barrier(MPI_COMM_WORLD);
        //    c_mpi_read_stop[i] = MPI_Wtime();
        //    c_mpi_read_time[i] += c_mpi_read_stop[i] - c_mpi_read_start[i];

        //    deallocate_Vector(&v);
       // }
    //}

    //if(rank == 0){
    //    printf("c-mpi read,   %d ", n_procs);
    //    for(int i=0; i<n_sizes; i++){
    //        c_mpi_read_time[i] /= n_trials;
    //        printf("%f,\t", c_mpi_read_time[i]);
    //    }
    //    printf("\n");
   // }




    double mpi_write_start[n_sizes], mpi_write_stop[n_sizes], mpi_write_time[n_sizes];

    for (int k=0; k<n_trials; k++){
        for(int i=0; i<n_sizes; i++){
            Vector v;
            int N = sizes[i];
            allocate_Vector(&v, N, 0, MPI_COMM_WORLD);

            rand_fill_Vector(&v);

            char fname[500];
            sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);

            MPI_Barrier(MPI_COMM_WORLD);
            mpi_write_start[i] = MPI_Wtime();
            mpi_write_Vector(&v, fname);
            MPI_Barrier(MPI_COMM_WORLD);
            mpi_write_stop[i] = MPI_Wtime();
            mpi_write_time[i] += mpi_write_stop[i] - mpi_write_start[i];

            deallocate_Vector(&v);
        }
    }

    if(rank == 0){
        printf("mpi write,    %d ", n_procs);
        for(int i=0; i<n_sizes; i++){
            mpi_write_time[i] /= n_trials;
            printf("%f,\t", mpi_write_time[i]);
        }
        printf("\n");
    }

    double mpi_read_start[n_sizes], mpi_read_stop[n_sizes], mpi_read_time[n_sizes];

    for (int k=0; k<n_trials; k++){
        for(int i=0; i<n_sizes; i++){
            Vector v;
            int N = sizes[i];

            char fname[500];
            sprintf(fname, "%s/baseline_%d.data", path, logsizes[i]);

            MPI_Barrier(MPI_COMM_WORLD);
            mpi_read_start[i] = MPI_Wtime();
            mpi_read_Vector(&v, fname, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
            mpi_read_stop[i] = MPI_Wtime();
            mpi_read_time[i] += mpi_read_stop[i] - mpi_read_start[i];

            deallocate_Vector(&v);
        }
    }

    if(rank == 0){
        printf("mpi read,     %d ", n_procs);
        for(int i=0; i<n_sizes; i++){
            mpi_read_time[i] /= n_trials;
            printf("%f,\t", mpi_read_time[i]);
        }
        printf("\n");
    }
	
    }
//}
