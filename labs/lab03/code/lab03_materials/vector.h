#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector_tag {
    float* data;
    int dimension;
} Vector;

int allocate(Vector* v, int dimension);

int deallocate(Vector* v);

int random_fill(Vector* v);

int axpy(float alpha, Vector* vx, Vector* vy, Vector* vz);

int axpy_random(float alpha, Vector* vx, Vector* vy, Vector* vz, int* indexOrdering);

#endif