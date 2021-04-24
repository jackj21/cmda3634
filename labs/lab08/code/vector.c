#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>
#include "vector.h"


// M = total number of entries in global array
// P = number of processors in communicator over which to distribute the array
// k = rank of this process
// Mk will be overwritten with local number of entries
// r0 will be overwritten with start index of local subset


/*
* Computes properties of the local subvector of a specific rank
*
* Arguments:
*   N: Global vector size
*   P: Total processors
*   k: rank to get the size of
*   Nk: (return) Size of local subvector on rank k
*   r0: (return) Index of first local entry in global coordinates
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int compute_subvector_size(int N, int P, int k, int* Nk, int* r0){

    *Nk = N/P;
    *r0 = (*Nk)*k;

    if (k == P-1){
        *Nk += N%P;
    }


    return 0;
}


/*
* Allocator for a vector
*
* Arguments:
*   v: Vector to allocate
*   N: Global vector size
*   padding: Width of ghost region
*   comm: Communicator over which the vector is partitioned
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int allocate_Vector(Vector* v, int N, int padding, MPI_Comm comm){

    int rank, size;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int N_local, r0;
    int flag = 0;
    flag = compute_subvector_size(N, size, rank, &N_local, &r0);

    v->padding = padding;

    v->N_local = N_local;
    v->N_padded = N_local + 2*padding;
    v->N_global = N;

    v->r0 = r0;

    v->comm = comm;

    v->data = (float*)malloc((v->N_padded)*sizeof(float));
    if(v->data == NULL){
        fprintf(stderr, "Error allocating image data. \n");
    return 1;
    }

    return flag;
}


/*
* Initializer for a vector
*
* Arguments:
*   v: Vector to initialize
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int initialize_Vector(Vector* v){

    memset(v->data, 0, v->N_padded*sizeof(float));

    return 0;
}


/*
* Deallocator for a vector
*
* Arguments:
*   v: Vector to deallocate
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int deallocate_Vector(Vector* v){

    v->padding = 0;

    v->N_local = 0;
    v->N_padded = 0;
    v->N_global = 0;

    v->r0 = -1;

    v->comm = MPI_COMM_NULL;

    free(v->data);
    v->data = NULL;

    return 0;
}


/*
* Pritner for a vector
*
* Arguments:
*   v: Vector to print
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int print_Vector(const Vector* v){

    int rank, n_procs;

    MPI_Comm_rank(v->comm, &rank);
    MPI_Comm_size(v->comm, &n_procs);

    char output[50000];
    memset(output, 0, 50000);

    int k = 0;
    for(int j=0; j < v->N_padded; j++){
        k += sprintf(output+k, "%.10f\n", v->data[j]);
    }
    sprintf(output+k, "\n");

    MPI_Barrier(v->comm);

    for(int i=0; i<n_procs; i++){
        if (rank == i){
            printf("%s", output);
        }
        MPI_Barrier(v->comm);
    }

    usleep(300);
    fflush(stdout);
    MPI_Barrier(v->comm);
}


/*
* Copies data into unpadded portion of local vector
*
* Arguments:
*   data: data to copy in to Vector
*   v: Vector to copy data in to
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
void inject_unpadded_data_into_Vector(float* data, Vector* v){

    int idx_unpadded;
    int idx_padded;

    // Think about how you can do this in one line.
    for(int j=0; j<v->N_local; ++j){
        idx_unpadded = j;
        idx_padded = j + v->padding;
        v->data[idx_padded] = data[idx_unpadded];
    }
}


/*
* Copies main data from padded Vector to an output vector
*
* Arguments:
*   data: data to copy from Vector in to
*   v: Vector to copy data from
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
void extract_unpadded_data_from_Vector(float* data, Vector* v){
	if (data == NULL || v == NULL) {
		printf("Null pointer passed in.\n");
		return;
	}

	int pad = v->padding;
	int r0 = v->r0;
	int start = 0;

	for (int i=pad; i<(v->N_padded-pad); i++) {
		data[start] = v->data[i];
		start++;
	}
}


/*
* Fills the padding region with a mirror condition.
*
* For example, if padding is sized 3, the vector
*     - - - a b c d - - -
* becomes
*     c b a a b c d d c b
*
* Arguments:
*   v: Vector to apply boundary condition to
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int update_padding_mirror_Vector(Vector* v){
	if (v == NULL) {
		printf("Null pointer passed in.\n"); 
		return 1;
	}
	
	// v structure fields
	// float* data, int padding, int N_local, int N_padded, int N_global, int r0
	int pad = v->padding;
	int start = 0;
	int end = v->N_padded - 1;
	
	float values[v->N_local];
	for (int i=0; i<v->N_local; i++) {
		values[i] = v->data[i+pad];
	}
	
	for (int i=pad; i>0; i--) {
		v->data[start] = values[i-1];
		v->data[end] = values[v->N_local-i];
		start++;
		end--;
	}

    return 0;
}


/*
* Perform a ghost/halo exchange on a Vector.
*
* Arguments:
*   v: Vector to perform ghost exchange on
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int update_padding_ghost_exchange_Vector(Vector* v){
    int p = v->padding;
    int h = v->N_padded;
    int offset;

    int rank, size;
    MPI_Comm_size(v->comm, &size);
    MPI_Comm_rank(v->comm, &rank);

    MPI_Status statuses[4];
    MPI_Request requests[4] = {MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL};
    // <student>
	if (v->data == NULL) {
		printf("Vector's data was null.\n");
		return 1;
	}
    // Initialize the requests
	//requests = {MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL};
    
	// pointer to location to fill using data from previous rank
	float* prev_fill = v->data;
    
	// pointer to location to send data from to previous rank
	float* prev_send = v->data+p;
    
	// pointer to location to send data from to next rank
	float* next_send = v->data+(h-p-p);
    
	// pointer to location to fill using data from next rank
	float* next_fill = v->data+(h-p);
    
	// Protect the sequential case
	if (size  == 1) return 0;
    
	// receive from previous (up) rank
	if (rank > 0) {
		printf("recv from prev up rank\n");
		MPI_Irecv(prev_fill, p, MPI_FLOAT, rank-1, 0, v->comm, &requests[0]); 
    }
	// receive from next (down) rank
	if (rank < size-1) {
		printf("recv from next down rank\n");
		MPI_Irecv(next_fill, p, MPI_FLOAT, rank+1, 1, v->comm, &requests[1]);
    }
	// send to previous (up) rank
	if (rank > 0) {
		printf("send to prev up rank\n");
		MPI_Isend(prev_send, p, MPI_FLOAT, rank-1, 1, v->comm, &requests[2]);
    }
	// send to next (down)rank
	if (rank < size-1) {
		printf("send to down rank\n");
		MPI_Isend(next_send, p, MPI_FLOAT, rank+1, 0, v->comm, &requests[3]);
    }
	// make sure all the requests are done
	MPI_Waitall(4, requests, statuses);
    // </student>
    return 0;
}


/*
* Compute a k-window running average
*
* Arguments:
*   v_in: Vector to perform operation on
*   v_out: Vector to return operation in
*   k: Size of window
*
* Returns:
*   error code, 0 for success, 1 for failure
*/
int average(Vector* v_in, Vector* v_out, int k){

    int q = (k-1) / 2;

    if(q > v_in->padding){
        printf("Error in average: requested averaging window is wider than padding \n");
    return 1;
    }
    if(v_in->padding != v_out->padding){
        printf("Error in average: v_in and v_out are not padded by the same size \n");
        return 1;
    }

    int p = v_in->padding;
    int N = v_in->N_local;

    for(int j=p; j<p+N; ++j){

        v_out->data[j] = 0.0;

        for(int i=j-q; i<=j+q; ++i){
            v_out->data[j] += v_in->data[i];
        }

        v_out->data[j] = (v_out->data[j])/(float)(k);
    }

    return 0;
}


