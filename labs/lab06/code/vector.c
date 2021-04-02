#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <stdio.h>
#include "vector.h"

int allocate_Vector(Vector *v, int N){

    v->N = N;
    v->data = malloc(sizeof(float)*N);

    // in case malloc failed
    if(v->data == NULL)
        return 1;

    // if malloc succeeded, set entries to 0
    memset(v->data, 0, sizeof(float)*N);

    return 0;
}

int deallocate_Vector(Vector *v){

    v->N = 0;

    free(v->data);
    v->data = NULL;

    return 0;
}

int rand_fill_Vector(Vector *v){

    srand(time(NULL));

    // check that N of vector is reasonable
    if(v->N < 1)
        return 1;
	
    // loop through and fill vector with random numbers 0 to 1
    for(int i=0; i < v->N; i++){
        v->data[i] = (float)rand() / (float)RAND_MAX;
    }
    return 0;
}

int zero_fill_Vector(Vector *v){

    // check that N of vector is reasonable
    if(v->N < 1)
        return 1;

    memset(v->data, 0, sizeof(float)*v->N);

    return 0;
}

int axpy(float alpha, Vector* vx, Vector* vy, Vector* out) {
    // in serial, overwrite out with alpha*vx + vy

    int i;
    int N = vx->N;

    float* vx_data = vx->data;
    float* vy_data = vy->data;
    float* out_data = out->data;

    // Sanity check on array dimensions
    if ((vx->N != vy->N) || (vx->N != out->N)){
        return 1;
    }

    for(i=0; i < N; i++){
        out_data[i] = alpha*vx_data[i] + vy_data[i];
    }

    return 0;
}


// Distribute a Vector into smaller subvectors on distinct MPI ranks.
// Each subvector will have the same number of entries, except the last,
// which will have the remainder.
int scatter_Vector(Vector* v, Vector* v_local){


    // MPI Variables
    MPI_Status status;
    int tag = 0;
    int n_procs;
    int rank;

    int N_global;
    int N_local;

    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // All ranks need to know the total vector size.  Only the master rank has that information.
    if(rank == 0){
        N_global = v->N;
        for(int dest_rank=1; dest_rank<n_procs; ++dest_rank){
            MPI_Send(&N_global, 1, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(&N_global, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    }

    // We do a close to even distribution of labor.  All ranks get the same amount, but the last
    // rank gets any remainder.
    N_local = (N_global/n_procs);
    if (rank == n_procs-1) N_local += N_global%n_procs;
    allocate_Vector(v_local, N_local);

    // Scatter the data
    if(rank == 0){
        // Make the local copy of the master data
        for(int i=0; i<N_local; ++i){
            v_local->data[i] = v->data[i];
        }

        // Must be careful, this code should work in serial too.
        if(n_procs > 1){
            // Send relevant data to each rank
            int start_idx;
            int N_remote;
            for(int dest_rank=1; dest_rank<n_procs; ++dest_rank){
                start_idx = dest_rank*N_local;

                N_remote = N_local;
                // Account for the last rank being special
                if (dest_rank == n_procs-1) N_remote += N_global%n_procs;

                MPI_Send(&(v->data[start_idx]), N_remote, MPI_FLOAT, dest_rank, tag, MPI_COMM_WORLD);
            }
        }
    }
    else if(rank > 0){
        // All other ranks receive from the master rank.
        MPI_Recv(&(v_local->data[0]), N_local, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
    }

    return 0;
}


// Print the vector entries, in order, even though it is distributed.
int distributed_print_Vector(Vector* v_local){

    // ************************ STUDENT CODE GOES HERE **************************
		// Error check
	if (v_local == NULL || v_local->data == NULL) {
		printf("Vector passed or its data was null.");
		return 1;
	}
	
	int rank, n_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

	int n = v_local->N;
	
	for (int i = 0; i<n_procs; i++) {
	
		if (rank == i) {
			printf("Rank %d has %d entries\n", rank, v_local->N);
			for (int idx = 0; idx < v_local->N; idx++) {
				printf("%f\n", v_local->data[idx]);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
    // *************************STUDENT CODE STOPS HERE ***************************

    return 0;
}


// Collect a Vector from smaller subvectors on distinct MPI ranks.
// Assumes that scatter_Vector was used to distribute the original vector.
int gather_Vector(Vector* v, Vector* v_local){

    // ***************STUDENT CODE GOES HERE******************
	MPI Status status;
	int tag = 0;
	int n_procs;
	int rank;

	MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	for (int i = 0; i<n_procs; i++) {
		if (rank == i) {
			for (int idx = 0; idx < v_local->N; idx++) {
				MPI_Send(&(v_local->data[idx]), v_local->N, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
			}
			MPI_Recv(&(v->data[i*v_local->N]), v_local->N, MPI_FLOAT, rank, tag, MPI_COMM_WORLD, &status);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
    // **************STUDENT CODE STOPS HERE*******************

    return 0;
}


// Performs an axpy in parallel by scattering a vector,
// performing the operation in-place, and gathering the result.
int distributed_axpy(float alpha, Vector* vx, Vector* vy){

    // split up the array into its subsets, assuming only rank 0 has access to the correct vx overall
    Vector vx_local;
    Vector vy_local;
    int N_local;

    scatter_Vector(vx, &vx_local);
    scatter_Vector(vy, &vy_local);

    // check array dims on this subset of vectors
    int Nx = vx_local.N;
    int Ny = vy_local.N;
    if(Nx != Ny){
        return 1;
    }
    N_local = Nx;

    // do the axpy on the data subset of interest
    // ******STUDENT CODE GOES HERE*************
	
    // ******STUDENT CODE STOPS HERE*************

    // collect the updated information from all the processes into vx and vy on rank 0
    gather_Vector(vx, &vx_local);
    gather_Vector(vy, &vy_local);

    // cleanup vx_local, vy_local
    deallocate_Vector(&vx_local);
    deallocate_Vector(&vy_local);
    return 0;
}

