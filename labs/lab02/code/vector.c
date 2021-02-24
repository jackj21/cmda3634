#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "vector.h"

/**
 * Dynamically allocates a vector and its array.
 *
 * Returns:
 * 		 1 if allocation did not work.
 * 		 0 if allocation did work.
 *
 */
int allocate(Vector* u, int N) {
	u->data = malloc(N*sizeof(float));

	if (u->data == NULL)
		return 1;
	
	u->N = N;

	return 0;

}

/**
 * Deallocates a vector and empties its contents.
 */
int deallocate(Vector* u) {
	
	if (u->data == NULL){
		u->N = 0;
		return 0;	
	}
	
	free(u->data);
	u->data = NULL;
	u->N = 0;

	return 0;
}


/**
 * Initializes the vector being pointed to to 0.
 */  
int initialize(Vector* u) {
	for (int i = 0; i < u->N; ++i) {
		u->data[i] = 0.0;
	}
	
	return 0;
}

/**
 * Returns the inner dot product of the 2 vectors
 * being pointed to.
 */ 
int inner_product(Vector* u, Vector* v, float* sum) {
	if (u->N != v->N)
		return 1;
	// iterate through vectors x,y,z components and multiplies and stores value
	*sum = 0.0;
	for (int i = 0; i < u->N; ++i) {
		*sum += u->data[i] * v->data[i];
	}
	

	return 0;
}

/**
 * Returns the Euclidian norm of a vector
 */ 
float norm(Vector* u) {
	float ip;
	inner_product(u, u, &ip);

	return sqrt(ip);
}

/**
 * Modifies a given vector to have length 1, in place
 */ 
int normalize(Vector* u) {
	float length = norm(u);

	if (length < 1e-7)
		return 1;

	// divides vectors x, y, and z components by length of vector to normalize it 
	for (int i = 0; i < u->N; ++i){
		u->data[i] /= length;	
	}
	
	return 0;
}

/**
 * Returns the result of the operation z = alpha * x + y 
 */
int axpy(float a, Vector* u, Vector* v, Vector* z) {
	if (u->N != v->N || u->N != z->N || v->N != z->N)
		return 1; 
	for (int i = 0; i < u->N; ++i){
		z->data[i] = a*u->data[i] + v->data[i];
	}
	
	return 0;
} 

/**
 * Prints the contents of a vector in a useful way
 */ 
void print(Vector* u) {
	
	printf("[");
	for (int i = 0; i < (u->N - 1); ++i) {
		printf("%f, ", u->data[i]);
	}
	printf("%f]\n", u->data[u->N - 1]);

}


