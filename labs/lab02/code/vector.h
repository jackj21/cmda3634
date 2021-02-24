#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector_tag {
	int N;
	float* data;
} Vector;

int allocate(Vector* u, int N);
int deallocate(Vector* u);
int initialize(Vector* u);
int inner_product(Vector* u, Vector* v, float* sum);
float norm(Vector* u);
int normalize(Vector* u);
int axpy(float a, Vector* u, Vector* v, Vector* z);
void print(Vector* u);

#endif
