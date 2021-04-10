#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "wave.h"
#include "array_2d.h"
 
/*
 * Computes the time history of the error between
 * the simulate solution and the true solution.
 *
 * Arguments:
 * 		n: Number of grid points in the x and y dimensions.
 * 		mx: Number of standing wave nodes in the x dimension.
 * 		my: Number of standing wave nodes in the y dimension.
 * 		alpha: CFL safety factor
 * 		T: Simulation time
 *
 * Returns:
 * 		
 */  
int main(int argc, char** argv) {i
	
	// Error checks...

	// Convert cmd args to numeric data types
	int n = atoi(argv[1]);
	int mx = atoi(argv[2]);
	int my = atoi(argv[3]);
	int alpha = atoi(argv[4]);
	int T = atoi(argv[5]);

	// calculate nt with given args
	float dx = 1.0 / (n - 1.0);
	float dt = (alpha * dx) / sqrt(2.0);
	int nt = round(T / dt);

	// initialize, allocate, setup data structures
	Array2D_f u_sim;
	Array2D_f u_true;
	
	allocate_Array2D_f(&u_sim, n, n);
	allocate_Array2D_F(&u_true, n, n);
	
	initialize_Array2D_f(&u_sim);
	initialize_Array2D_f(&u_true);
		
	// loop through nt times to find errors and store in file
	for (int k=0; k<nt; ++k) {
			
	}


	// deallocate
}

