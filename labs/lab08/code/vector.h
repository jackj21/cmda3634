#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <mpi.h>

typedef struct Vector_tag {

	// Local subset of the data
    float* data;

    // Size of the ghost padding
    int padding;

    // Size of the (unpadded) local part of the vector
    int N_local;

    // Size of the padded local part of the vector
    int N_padded;

    // Size of the global vector
    int N_global;

    // Starting index of the unpadded local vector in global coordinates
    int r0;

    // The communicator over which the global array is partitioned
    MPI_Comm comm;
} Vector;

int compute_subvector_size(int N, int P, int k, int* Nk, int* r0);
int allocate_Vector(Vector* v, int N, int padding, MPI_Comm comm);
int initialize_Vector(Vector* v);
int deallocate_Vector(Vector* v);
int print_Vector(const Vector* v);

void inject_unpadded_data_into_Vector(float* data, Vector* v);
void extract_unpadded_data_from_Vector(float* data, Vector* v);

int update_padding_mirror_Vector(Vector* v);
int update_padding_ghost_exchange_Vector(Vector* v);

int average(Vector* v_in, Vector* v_out, int k);

#endif
