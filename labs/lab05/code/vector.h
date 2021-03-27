#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector_tag {
    float* data;
    int N;
} Vector;

int allocate_Vector(Vector *v, int N);

int deallocate_Vector(Vector *v);

int rand_fill_Vector(Vector *v);

int zero_fill_Vector(Vector *v);

float norm(Vector* v);

int normalize(Vector* v);

int axpy(float alpha, Vector* vx, Vector* vy, Vector* vz);

int inner_product(Vector* vx, Vector* vy, float* ip);

#endif
