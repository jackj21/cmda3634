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
int allocate_Vector(Vector* localVec, int N, int padding, MPI_Comm comm);
int initialize_Vector(Vector* localVec);
int deallocate_Vector(Vector* localVec);
int rand_fill_Vector(Vector *v);
int print_Vector(const Vector* v);

int compare_Vector(const Vector* v, const Vector* u);

int c_read_Vector(Vector* v, const char* filename);
int c_write_Vector(Vector* v, const char* filename);

int c_mpi_read_Vector(Vector* v, const char* filename, MPI_Comm comm);
int c_mpi_write_Vector(Vector* v, const char* filename);

int mpi_read_Vector(Vector* v, const char* filename, MPI_Comm comm);
int mpi_write_Vector(Vector* v, const char* filename);


#endif
