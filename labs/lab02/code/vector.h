#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector_tag {
	float data[3];
} Vector;

int initialize(Vector* u);
int inner_product(Vector* u, Vector* v, float* sum);
int norm(Vector* u, float* norm);
int normalize(Vector* u);
int axpy(float a, Vector* u, Vector* v, Vector* z);
void print(Vector* u);

#endif
