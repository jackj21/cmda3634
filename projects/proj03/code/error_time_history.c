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
int main(int argc, char** argv) {
	
	// Error checks...
	if(argc != 7) {
		printf("Incorrect number of parameters. Correct usage:\n./wave_error n Mx My alpha T n_threads\n");
		return 1;
	}

	// Convert cmd args to numeric data types
	int n = atoi(argv[1]);
	int mx = atoi(argv[2]);
	int my = atoi(argv[3]);
	int alpha = atoi(argv[4]);
	int T = atoi(argv[5]);
	int n_threads = atoi(argv[6]);

	omp_set_num_threads(n_threads);
	// calculate nt with given args
	float dx = 1.0 / (n - 1.0);
	float dy = dx;
	float dt = (alpha * dx) / sqrt(2.0);
	int nt = round(T / dt);

	// initialize, allocate, setup data structures
	Array2D_f u_prev;
	Array2D_f u_curr;
	Array2D_f u_next;
	Array2D_f u_true;
	
	allocate_Array2D_f(&u_prev, n, n);
	allocate_Array2D_f(&u_curr, n, n);
	allocate_Array2D_f(&u_next, n, n);
	allocate_Array2D_f(&u_true, n, n);
	
	initialize_Array2D_f(&u_prev);
	initialize_Array2D_f(&u_curr);
	initialize_Array2D_f(&u_next);
	initialize_Array2D_f(&u_true);
	
	// Evaluate initial conditions at t=-dt and t=0
	evaluate_standing_wave(&u_prev, mx, my, dx, dy, -1.0*dt);
	evaluate_standing_wave(&u_curr, mx, my, dx, dy, 0.0);
	
	char file[10] = "error.csv";
	FILE* fp;
	fp = fopen(file, "w+");
	fprintf(fp, "Timestep    Error\n");
	

	// loop through nt times to find errors and store in file
	for (int k=1; k<nt; ++k) {
		standing_wave_simulation_nsteps(&u_prev, &u_curr, &u_next, dt, dx, 1);
		evaluate_standing_wave(&u_true, mx, my, dx, dy, k*dt);
		
		float error = norm_error(&u_curr, &u_true, n, n);
			
		fprintf(fp, "%d    %f\n", k, nt);

	}
	
	fclose(fp);

	// deallocate
	deallocate_Array2D_f(&u_prev);
	deallocate_Array2D_f(&u_curr);
	deallocate_Array2D_f(&u_next);
	deallocate_Array2D_f(&u_true);

	return 0;
}

