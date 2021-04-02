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

int axpy(float alpha, Vector* vx, Vector* vy, Vector* out);

int scatter_Vector(Vector* v, Vector* v_local);

int distributed_print_Vector(Vector* v_local);

int gather_Vector(Vector* v, Vector* v_local);

int distributed_axpy(float alpha, Vector* vx, Vector* vy);


#endif
