#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * Runs program.
 */ 
int main(int argc, char* argv[]) {
	// initialize vectors being pointed to
	Vector u;
	Vector v;
	
	if (argc > 1)
		printf("Too many arguments.\n");
	int N = *argv[1]; 

	allocate(&u, N);
	allocate(&v, N);

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
	norm(&u, &m); 
	
	// print length of vector u
	printf("Length of vector u is: %f\n", m);

	
	// ii)
	// find w1 = alpha * u + v, where alpha = 0.45
	float alpha = 0.45;
	Vector w_1;
	allocate(&w_1, N);
	axpy(alpha, &u, &v, &w_1);
	printf("Vector w_1 = ");
	print(&w_1);
	printf("\n");
	deallocate(&w_1);
	
	// iii)
	// find w2 = beta * v + v, where beta = 0.65
	float beta = 0.65;
	Vector w_2;
	allocate(&w_2, N);
	axpy(beta, &v, &v, &w_2);
	printf("Vector w_2 = ");
	print(&w_2);
	printf("\n");
	deallocate(&w_2);

	// iv)
	// find a = <u, v>, inner product of u and v
	float a;
	inner_product(&u, &v, &a);
	printf("Inner product of vectors u and v:\n");
	printf("a = %f\n", a);	

	// v)
	// u_hat = u / ||u||, a normalized version of vector u
	normalize(&u);
	printf("Normalized version of vector u:\n");
	print(&u);

	// vi)
	// v_hat = v / ||v||, a normalized version of vector v
	normalize(&v);
	printf("Normalized version of vector v:\n");
	print(&v);
	
	deallocate(&v);
	deallocate(&u);

	return 0;
}
