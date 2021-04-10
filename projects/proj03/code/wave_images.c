/*
* Authors:
*   Russell J. Hewett (rhewett@vt.edu)
*
* Not licensed for external distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "array_2d.h"
#include "wave.h"

int main(int argc, char** argv){

    if(argc < 7){
        printf("Incorrect number of parameters.  Correct usage:\n./wave_images n Mx My alpha t_plot0 t_plot1 n_threads ...\n");
        return 1;
    }

    // Setup the timers.
    clock_t start, stop;
    double simulation_time = 0.0;

    // Extract command line arguments.
    unsigned int n = atoi(argv[1]);
    unsigned int Mx = atoi(argv[2]);
    unsigned int My = atoi(argv[3]);

    float alpha = atof(argv[4]);
	
	int n_threads = atoi(argv[7]);
	omp_set_num_threads(n_threads);

    // Perform the task.
    start = clock();

    // Specification allows us to assume that nx == ny
    unsigned int nx = n;
    unsigned int ny = n;

    // Compute grid spacing
    float dx = 1.0 / (nx - 1);
    float dy = dx;

    // Compute time step size
    float dt = alpha * dx / sqrt(2.0);

    int error = 0;

    Array2D_f u_prev;
    Array2D_f u_curr;
    Array2D_f u_next;
    Array2D_f u_true;

    // This was not required by the spec but it is an additional safety against using an
    // unallocated array.
    nullify_Array2D_f(&u_prev);
    nullify_Array2D_f(&u_curr);
    nullify_Array2D_f(&u_next);
    nullify_Array2D_f(&u_true);

    // Allocate the required arrays.
    allocate_Array2D_f(&u_prev, ny, nx);
    if (error) return 1;
    allocate_Array2D_f(&u_curr, ny, nx);
    if (error) return 1;
    allocate_Array2D_f(&u_next, ny, nx);
    if (error) return 1;
    allocate_Array2D_f(&u_true, ny, nx);
    if (error) return 1;

    // Initialize the required arrays.
    initialize_Array2D_f(&u_prev);
    initialize_Array2D_f(&u_curr);
    initialize_Array2D_f(&u_next);
    initialize_Array2D_f(&u_true);

    char outfile[50];

    // Loop over the remaining times and run the simulation to that time.
    // We have to reset every time because we don't enforce that the inputs are
    // sorted.  If we had them sorted, we could perform the simulation incrementally.
    for(int k=5; k<argc; k++){

        float t_plot = atof(argv[k]);
        int nt = floor(t_plot / dt);

        // Evaluate the initial conditions at t=-dt and t=0
        evaluate_standing_wave(&u_prev, Mx, My, dx, dy, -1.0*dt);    
        evaluate_standing_wave(&u_curr, Mx, My, dx, dy, 0.0);
        
        // Perform nt steps of the simulation
        standing_wave_simulation_nsteps(&u_prev, &u_curr, &u_next, dt, dx, nt);

        // Evaluate the true solution at that time point.  Note, this is not exactly
        // the same as the request final time
        evaluate_standing_wave(&u_true, Mx, My, dx, dy, nt*dt);
 
        // Save the simulated solutution
        memset(outfile, 50*sizeof(char), 0);
        sprintf(outfile, "computed_%8f.arr", nt*dt);
        write_Array2D_f(&u_curr, outfile);

        // Save the true solution
        memset(outfile, 50*sizeof(char), 0);
        sprintf(outfile, "true_%8f.arr", nt*dt);
        write_Array2D_f(&u_true, outfile);

        printf("Saving images from t=%f, closest index %d to %f.\n", nt*dt, nt, t_plot);
    }

    // Clean up the memory
    deallocate_Array2D_f(&u_prev);
    deallocate_Array2D_f(&u_curr);
    deallocate_Array2D_f(&u_next);
    deallocate_Array2D_f(&u_true);

    return 0;
}
