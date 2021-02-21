#include <stdio.h>
#include <math.h>
#include "vector.h"

/**
 * Initializes the vector being pointed to to 0.
 */  
int initialize(Vector* u) {
	for (int i=0; i<3; ++i) {
		u->data[i] = 0.0;
	}
	
	return 0;
}

/**
 * Returns the inner dot product of the 2 vectors
 * being pointed to.
 */ 
int inner_product(Vector* u, Vector* v, float* sum) {
	// iterate through vectors x,y,z components and multiplies and stores value
	float temp;
	for (int i=0; i<3; ++i) {
		temp += (u->data[i] * v->data[i]);
	}
	*sum = temp;

	return 0;
}

/**
 * Returns the Euclidian norm of a vector
 */ 
int norm(Vector* u, float* norm) {
	inner_product(u, u, norm);
	*norm = sqrt(*norm);
	return 0;
}

/**
 * Modifies a given vector to have length 1, in place
 */ 
int normalize(Vector* u) {
	float length = 0.0;
	norm(u, &length);

	if (length == 0)
		return 1;

	// divides vectors x, y, and z components by length of vector to normalize it 	
	u->data[0] = u->data[0] / length;
	u->data[1] = u->data[1] / length;
	u->data[2] = u->data[2] / length;		
	
	return 0;
}

/**
 * Returns the result of the operation z = alpha * x + y 
 */
int axpy(float a, Vector* u, Vector* v, Vector* z) {
	// Compute ax + y for x component of vector	
	z->data[0] = a*u->data[0] + v->data[0];

	// Compute ax + y for y component of vector
	z->data[1] = a*u->data[1] + v->data[1];

	// Compute ax + y for z component of vector
	z->data[2] = a*u->data[2] + v->data[2];
	
	return 0;
} 

/**
 * Prints the contents of a vector in a useful way
 */ 
void print(Vector* u) {
	int n = 3;	
	
	printf("[");
	for (int i=0; i<n-1; ++i) {
		printf("%f, ", u->data[i]);
	}
	printf("%f]\n", u->data[n-1]);

}


