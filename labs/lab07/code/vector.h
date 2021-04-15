#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <mpi.h>

typedef struct Vector_tag {
    int N;
    int N_global;

    MPI_Comm comm;

    float* data;
} Vector;

int local_vector_size(int rank, int n_procs, int N);

int allocate_Vector(Vector* v, int N, MPI_Comm comm);
int deallocate_Vector(Vector* v);

int rand_fill_Vector(Vector* v);
int zero_fill_Vector(Vector* v);

int copy_Vector(Vector* src, Vector* dest);

int print_Vector(const Vector* v);

int scatter_Vector(const Vector* vg, Vector* vl);
int gather_Vector(const Vector* vl, Vector* vg);

int log_sum_reduction(const float* val, float* reducedval, MPI_Comm comm);
int log_broadcast(float* val, MPI_Comm comm);

int axpy(float alpha, Vector* vx, Vector* vy);

int inner_product_manual(Vector* vx, Vector* vy, float* ip);
int inner_product_mpi(Vector* vx, Vector* vy, float* ip);

float norm(Vector* v);

int normalize_serial(Vector* v);
int normalize_manual(Vector* v);
int normalize_mpi2(Vector* v);
int normalize_mpi1(Vector* v);


#endif
