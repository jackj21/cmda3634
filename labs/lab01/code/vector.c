#include <stdio.h>
#include <math.h>

typedef struct Vector_tag {
	float data[3];
} Vector;

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
	for (int i=0; i<3; ++i) {
		*sum += u->data[i] * u->data[i];
	}

	return 0;
}

/**
 * Returns the Euclidian norm of a vector
 */ 
int norm(Vector* u, Vector* v, float* norm) {
	inner_product(u, u, norm);
	*norm = sqrt(*norm);
	return 0;
}

/**
 * Modifies a given vector to have length1, in place
 */ 
int normalize(Vector* u, float* length) {
	norm(u, u, length);

	if (*length == 0)
		return 1;

	// 	
	u->data[0] = u->data[0] / *length;
	u->data[1] = u->data[1] / *length;
	u->data[2] = u->data[2] / *length;		

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

/**
 * Runs program.
 */ 
int main() {
	// initialize vectors being pointed to
	Vector u;
	Vector v;
	initialize(&u);
	initialize(&v);

	// store 2.0, 1.2, and 5.3 as values into vector u
	u.data[0] = 2.0;
	u.data[1] = 1.2;
	u.data[2] = 5.3;

	// store 0.0001, 34.5, and 1717.17 as values into vector v
	v.data[0] = 0.0001;
	v.data[1] = 34.5;
	v.data[2] = 1717.17;
	
	// i)
	// find m, the length of vector u, with norm function
	float m;
	norm(&u, &u, &m); 
	
	// print length of vector u
	printf("Length of vector u is: %f\n", m);

	
	// ii)
	// find w1 = alpha * u + v, where alpha = 0.45
	float alpha = 0.45;
	Vector w_1;
	axpy(alpha, &u, &v, &w_1);
	printf("Vector w_1 = ");
	print(&w_1);
	printf("\n");
	
	// iii)
	// find w2 = beta * v + v, where beta = 0.65
	float beta = 0.65;
	Vector w_2;
	axpy(beta, &v, &v, &w_2);
	printf("Vector w_2 = ");
	print(&w_2);
	printf("\n");

	// iv)
	// find a = <u, v>, inner product of u and v
	

	return 0;
}
