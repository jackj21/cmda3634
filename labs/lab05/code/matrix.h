#include "vector.h"

#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct Matrix_tag {
    // This defines a matrix stored in row major order so
    // the (row i, column j) entry
    float *data;
    int M;
    int N;
} Matrix;

int allocate_Matrix(Matrix* A, int M, int N);

int deallocate_Matrix(Matrix* A);

int zero_fill_Matrix(Matrix* A);

int rand_fill_Matrix(Matrix* A);

int matvec(Matrix* A, Vector* x, Vector* b);

int matvec_triangular(Matrix* A, Vector* x, Vector* b);

int matvec_triangular_guided(Matrix* A, Vector* x, Vector* b);

#endif
