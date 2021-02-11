#include <stdlib.h>
#include <stdio.h>

/**
 * Function to find coordinates of x 
 * depending on the number of x's, nx,
 * in a domain of [0, 1] using addition.
 *
 * Parameters: 
 *     int nx: The number of x's.
 *
 * Return:
 *     Returns the coordinate of the last grid point for a nx value.
 **/ 
void addition(int nx) {
	float x = 0;
	float dx = 1 / (nx - 1);
	printf("Addition method for nx = %d\n:", nx);
	for (int i = 0; i <= nx; i += 1) {
		printf("Last grid point for nx = %d: %f\n", nx, x);
    	x += dx;
    }
	printf("End of addition method for nx = %d: %d\n", nx);
}

/**
 * Function to find coordinates of x
 * depending on the number of x's, nx,
 * in a domain of [0, 1] using multiplication.
 *
 * Parameters:
 *     int nx: The number of x's.
 *
 * Return:
 *     Returns the coordinate of the last grid point for a nx value.
 **/ 
void multiplication(int nx) {
	float x = 0;
	int n = 1;
	float dx = 1 / (nx - 1);
	printf("Multiplication method for nx = %d\n:", nx);
	for (int i = 0; i <= nx; i += 1) {
		printf("Last grid point for nx = %d: %d\n", nx, x);
		x *= n;
		n += 1;
		x += dx;
	} 
	printf("End of multiplication method for nx = %d: \n", nx);
}

int main() {
	addition(11);
	addition(101);
	addition(1001);
	addition(10001);
	addition(100001);
	addition(1000001);
	addition(10000001);

	multiplication(11);
	multiplication(101);
	multiplication(1001);
	multiplication(10001);
	multiplication(100001);
	multiplication(1000001);
	multiplication(10000001);

}
