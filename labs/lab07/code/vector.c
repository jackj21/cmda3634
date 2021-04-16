#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <math.h>

#include <mpi.h>

#include "vector.h"


int local_vector_size(int rank, int n_procs, int N_global){

    int N_local = (int)floor(N_global/n_procs);
    int remainder = N_global - (N_local*n_procs);

    if (rank == n_procs-1) N_local += remainder;

    return N_local;
}


int allocate_Vector(Vector* v, int N, MPI_Comm comm){

    int rank, n_procs;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &n_procs);

    v->N = local_vector_size(rank, n_procs, N);;
    v->N_global = N;

    v->comm = comm;

    v->data = malloc(sizeof(float)*N);

    // in case malloc failed
    if(v->data == NULL)
        return 1;

    // if malloc succeeded, set entries to 0
    memset(v->data, 0, sizeof(float)*N);


    return 0;
}


int deallocate_Vector(Vector* v){

    v->N = 0;

    free(v->data);
    v->data = NULL;

    v->N_global = 0;
    v->comm = MPI_COMM_NULL;

    return 0;
}

int rand_fill_Vector(Vector* v){

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

int zero_fill_Vector(Vector* v){

    // check that N of vector is reasonable
    if(v->N < 1)
        return 1;

    memset(v->data, 0, sizeof(float)*v->N);

    return 0;
}

int copy_Vector(Vector* src, Vector* dest){

    dest->N = src->N;
    memcpy(dest->data, src->data, dest->N*sizeof(float));

    return 0;
}


int print_Vector(const Vector* v){

    int rank, n_procs;

    MPI_Comm_rank(v->comm, &rank);
    MPI_Comm_size(v->comm, &n_procs);

    MPI_Status status;

    int token = 0;
    int start_rank = n_procs-1;

    if (rank == start_rank)
        MPI_Send(&token, 1, MPI_INT, (rank+1)%n_procs, 0, v->comm);

    MPI_Recv(&token, 1, MPI_INT, (rank-1)%n_procs, 0, v->comm, &status);

    for(int i=0; i < v->N; i++)
        printf("%.10f\n", v->data[i]);

    if (rank != start_rank)
        MPI_Send(&token, 1, MPI_INT, (rank+1)%n_procs, 0, v->comm);
}

int scatter_Vector(const Vector* vg, Vector* vl){

    int rank, n_procs;
    int tag = 0;
    int offset, data_size;

    MPI_Status status;

    MPI_Comm_rank(vl->comm, &rank);
    MPI_Comm_size(vl->comm, &n_procs);

    if (rank == 0){
        // copy the first N_local records
        memcpy(vl->data, vg->data, vl->N*sizeof(float));
        offset = local_vector_size(0, n_procs, vg->N);

        // scatter the remainder
        for(int i=1; i<n_procs; i++){
            data_size = local_vector_size(i, n_procs, vg->N);
            MPI_Send((vg->data + offset), data_size, MPI_FLOAT, i, tag, vl->comm);
            offset += data_size;

        }
    }
    else{
        MPI_Recv(vl->data, vl->N, MPI_FLOAT, 0, tag, vl->comm, &status);
    }

    return 0;
}

int gather_Vector(const Vector* vl, Vector* vg){

    int rank, n_procs;
    int tag = 0;
    int offset, data_size;

    MPI_Status status;

    MPI_Comm_rank(vl->comm, &rank);
    MPI_Comm_size(vl->comm, &n_procs);

    if (rank == 0){
        // copy the first N_local records
        memcpy(vg->data, vl->data, vl->N*sizeof(float));
        offset = local_vector_size(0, n_procs, vg->N);

        // scatter the remainder
        for(int i=1; i<n_procs; i++){
            data_size = local_vector_size(i, n_procs, vg->N);
            MPI_Recv((vg->data+offset), data_size, MPI_FLOAT, i, tag, vl->comm, &status);
            offset += data_size;

        }
    }
    else{
        MPI_Send(vl->data, vl->N, MPI_FLOAT, 0, tag, vl->comm);
    }

    return 0;
}


int axpy(float alpha, Vector* vx, Vector* vy) {

    // Sanity check on array dimensions
    if (vx->N != vy->N) return 1;

    for(int i=0; i < vx->N; i++){
        vy->data[i] = alpha*vx->data[i] + vy->data[i];
    }

    return 0;
}


int log_sum_reduction(const float* val, float* reducedval, MPI_Comm comm){

	
	int tag = 0;
	int rank, n_procs;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &n_procs);
	MPI_Status status;
	
	//float redval = *reducedval;
	float normval = *val;
	float recvval;
	
	int stride, level;
	stride = pow(2, (int)floor(log2(n_procs)));
	level = (int)floor(log2(n_procs)) + 1;

	int partner;

	while (stride > 0) {
		if (rank < 2*stride) {
			if (rank < stride) {
				partner = rank + stride;
				if (partner < n_procs) {
					MPI_Recv(&recvval, 1, MPI_FLOAT, partner, tag, comm, &status);
					normval += recvval;
				}
			}
			else {
				partner = rank - stride;	
				MPI_Send(&normval, 1, MPI_FLOAT, partner, tag, comm);
			}
		}

		stride /= 2;
		level -= 1;
	}
	*reducedval = normval;	
	
	return 0;
}


int log_broadcast(float* val, MPI_Comm comm){
	
	int tag = 0;
	int rank, n_procs;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &n_procs);
	MPI_Status status;
	
	float data;

	if (rank == 0) data = *val;
	//printf("Before Bcast Rank %d: Value = %f.\n", rank, data);
	
	int level, partner, stride;
	level = 1;
	stride = 1;
	
	while (stride < n_procs) {
		if (rank < 2*stride) {
			if (rank < stride) {
				partner = rank + stride;
				if (partner < n_procs) {
					//printf("Level %d (%d): %d sends to %d\n", level, stride, rank, partner);	
					MPI_Send(&data, 1, MPI_FLOAT, partner, 0, comm);
				}
			}
			else {
				partner = rank - stride;
				//printf("Level %d (%d): %d recv from %d\n", level, stride, rank, partner);
				MPI_Recv(&data, 1, MPI_FLOAT, partner, 0, comm, &status);
			}
		}
		stride *= 2;
		level += 1;
	}
	//printf("After Bcast Rank %d: value = %f.\n", rank, data);
	*val = data;

    return 0;
}


int inner_product_manual(Vector* vx, Vector* vy, float* ip) {

    int i;
    int N = vx->N;

    float* vx_data = vx->data;
    float* vy_data = vy->data;

    float result = 0.0;

    // Sanity check on array dimensions
    if (vx->N != vy->N) return 1;

    for(i=0; i < N; i++){	
        result += vx_data[i]*vy_data[i];
    }

	log_sum_reduction(&result, ip, vx->comm);
    return 0;
}


int inner_product_mpi(Vector* vx, Vector* vy, float* ip) {

    int i;
    int N = vx->N;

    float* vx_data = vx->data;
    float* vy_data = vy->data;

    float result = 0.0;

    // Sanity check on array dimensions
    if (vx->N != vy->N) return 1;

    for(i=0; i < N; i++){
    	result += vx_data[i]*vy_data[i];

    }
	MPI_Reduce(&result, ip, 1, MPI_FLOAT, MPI_SUM, 0, vx->comm);

    return 0;
}

float norm(Vector* v) {

    int i;
    int N = v->N;
    float* v_data = v->data;

    float length = 0.0;

    for(i=0; i < N; i++){
        length += pow(v_data[i], 2.0);
    }

    length = sqrt(length);

    return length;
}

int normalize_serial(Vector* v) {

    int i;
    int N = v->N;
    float* v_data = v->data;
	
    float length = norm(v);

    // Zero vector, nothing to do.
    if( abs(length) < 1e-5 ) return 0;

    for(i=0; i < N; i++){
        v_data[i] /= length;
    }

    return 0;
}

int normalize_manual(Vector* v) {

    int i;
    int N = v->N;
    float* v_data = v->data;

    float local_length;
    float length;
	local_length = pow(norm(v), 2);

	log_sum_reduction(&local_length, &length, MPI_COMM_WORLD);  
	log_broadcast(&length, MPI_COMM_WORLD);	
	length = sqrt(length);
    // Zero vector, nothing to do.
    if( abs(length) < 1e-5 ) return 0;

    for(i=0; i < N; i++){
        v_data[i] /= length;
    }

	
	

    return 0;
}

int normalize_mpi2(Vector* v) {

    int i;
    int N = v->N;
    float* v_data = v->data;

    float local_length;
    float length;
	
	local_length = pow(norm(v), 2);
	
	MPI_Reduce(&local_length, &length, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Bcast(&length, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	length = sqrt(length);
    
	// Zero vector, nothing to do.
    if( abs(length) < 1e-5 ) return 0;

    for(i=0; i < N; i++){
        v_data[i] /= length;
    }

    return 0;
}

int normalize_mpi1(Vector* v) {

    int i;
    int N = v->N;
    float* v_data = v->data;

    float local_length;
    float length;
	
	local_length = pow(norm(v), 2);

	MPI_Allreduce(&local_length, &length, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

	length = sqrt(length);

    // Zero vector, nothing to do.
    if( abs(length) < 1e-5 ) return 0;

    for(i=0; i < N; i++){
        v_data[i] /= length;
    }

    return 0;
}
