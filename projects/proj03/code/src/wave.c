/*
* Project 2 Provided Solution
* Authors:
*   Russell J. Hewett (rhewett@vt.edu) 
* Adapted for Project 3 by:
* 	Jack Jiang (jackj21@vt.edu)
*
* Not licensed for external distribution.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "wave.h"
#include "array_2d.h"


/*
* Helper function to the linear index given array indices.
*
* Arguments:
*   j: current row index
*   i: current col index
*   nx: number of cols
*
* Returns:
*   linear index
*/

int ji_to_idx(int j, int i, int nx) {
    return j*nx + i;
}


/*
* Evaluates the standing wave solution on an ny x nx grid with openMP.
*
* Arguments:
*   u: Pointer to the array structure that will store the output
*   Mx: Number of standing wave nodes in the x dimension
*   My: Number of standing wave nodes in the y dimension
*   dx: grid spacing in the x dimension
*   dy: grid spacing in the y dimension
*   t: time at which to evaluate the function
*
* Returns:
*   Error Code:
*      * 1: array is improperly allocated.
*/

int evaluate_standing_wave(Array2D_f* u, unsigned int Mx, unsigned int My, float dx, float dy, float t) {

    // Error checks
    if (u->data == NULL) {
        fprintf(stderr, "Error: u is null in evaluate_standing_wave.\n");
        return 1;
    }

    unsigned int ny = u->ny;
    unsigned int nx = u->nx;
	
    float w = M_PI*sqrt((float)(Mx*Mx + My*My));

	
#pragma omp parallel for default(none) shared(ny,nx,dy,dx,Mx,My,w,u,t)
    for(int j=0; j<ny; ++j) {

        float y = j*dy;

        for(int i=0; i<nx; ++i) {

            int kr = ji_to_idx(j, i, nx);
            float x = i*dx;

            u->data[kr] = sin(Mx*x*M_PI)*sin(My*y*M_PI)*cos(w*t);
        }        
    }

    return 0;
}


/*
* Evaluates a single timestep of a wave solver with openMP.
*
* Arguments:
*   u_prev: Pointer to the array structure containing previous state
*   u_curr: Pointer to the array structure containing current state
*   u_next: Pointer to the array structure containing next state, contains the output
*   dx: grid spacing in the x dimension
*   dt: size of temporal time step
*
* Returns:
*   Error Code:
*      * 1: an array is improperly allocated.
*      * 2: dimension mismatch in arrays.
*/

int wave_timestep(Array2D_f* u_prev, Array2D_f* u_curr, Array2D_f* u_next, float dt, float dx) {

    // Error checks
    if ((u_prev->data == NULL) || (u_curr->data == NULL) || (u_next->data == NULL)) {
        fprintf(stderr, "Error: u_prev (%p), u_curr (%p), or u_next (%p) are null in wave_timestep.\n", 
                 u_prev->data, u_curr->data, u_next->data);
        return 1;
    }
    if ((u_prev->nx != u_curr->nx) || (u_prev->nx != u_next->nx) ||
        (u_prev->ny != u_curr->ny) || (u_prev->ny != u_next->ny)) {
        fprintf(stderr, "Error: u_prev (%u, %u), u_curr (%u, %u), or u_next (%u, %u) dimension mismatch.\n", 
                u_prev->nx, u_prev->ny, u_curr->nx, u_curr->ny, u_next->nx, u_next->ny);
        return 2;
    }

    // Convenience variables for shortening code
    unsigned int ny = u_curr->ny;
    unsigned int nx = u_curr->nx;

    // Loop over both spatial dimensions
#pragma omp parallel for default(none) shared(ny,nx,dx,dt,u_prev,u_curr,u_next)
    for(int j=0; j<ny; ++j) {
        for(int i=0; i<nx; ++i) {

            // Map the two dimensions back to the linear index
            int kr = ji_to_idx(j, i, nx);

            // If the point is on the boundary, zero it and move on to the next point
            if ((j == 0) || (j == ny-1) || (i == 0) || (i == nx-1)) {
                u_next->data[kr] = 0.0;
                continue;
            }

            // Precompute the indices of the surrounding points
            int kr_up    = ji_to_idx(j-1, i, nx);
            int kr_down  = ji_to_idx(j+1, i, nx);
            int kr_left  = ji_to_idx(j, i-1, nx);
            int kr_right = ji_to_idx(j, i+1, nx);

            // Compute the Laplacian
            float lap = -4*u_curr->data[kr] + u_curr->data[kr_up] + u_curr->data[kr_down] + u_curr->data[kr_left] + u_curr->data[kr_right];
            lap /= (dx*dx);

            // Compute the time step update for this point
            u_next->data[kr] = -1*u_prev->data[kr] + 2*u_curr->data[kr] + dt*dt*lap;
        }        
    }

    return 0;
}


/*
* Performs a simulation of the wave equation on a standing-wave input.
*
* Arguments:
*   T: Final time, in seconds, of the simulation
*   N: Number of grid points in the x and y dimensions 
*   Mx: Number of standing wave nodes in the x dimension
*   My: Number of standing wave nodes in the y dimension
*   alpha: CFL safety factor
*
* Returns:
*   Error Code:
*      * 1: Error allocating u_prev.
*      * 2: Error allocating u_curr.
*      * 3: Error allocating u_next.
*/

int standing_wave_simulation(int nt, unsigned int N, unsigned int Mx, unsigned int My, float alpha) {

    // Specification allows us to assume that nx == ny
    unsigned int nx = N;
    unsigned int ny = N;

    // Compute grid spacing
    float dx = 1.0 / (nx - 1);
    float dy = dx;

    // Compute time step size
    float dt = alpha * dx / sqrt(2.0);

    // Compute number of timesteps

    int error = 0;

    Array2D_f u_prev;
    Array2D_f u_curr;
    Array2D_f u_next;

    // This was not required by the spec but it is an additional safety against using an
    // unallocated array.
    nullify_Array2D_f(&u_prev);
    nullify_Array2D_f(&u_curr);
    nullify_Array2D_f(&u_next);

    // Allocate the required arrays.
    error = allocate_Array2D_f(&u_prev, ny, nx);
    if (error) return 1;
    error = allocate_Array2D_f(&u_curr, ny, nx);
    if (error) return 2;
    error = allocate_Array2D_f(&u_next, ny, nx);
    if (error) return 3;

    // Initialize the required arrays.
    initialize_Array2D_f(&u_prev);
    initialize_Array2D_f(&u_curr);
    initialize_Array2D_f(&u_next);

    // Evaluate the initial conditions at t=-dt and t=0
    evaluate_standing_wave(&u_prev, Mx, My, dx, dy, -1*dt);    
    evaluate_standing_wave(&u_curr, Mx, My, dx, dy, 0);
    
    // Perform nt steps of the simulation
    standing_wave_simulation_nsteps(&u_prev, &u_curr, &u_next, dt, dx, nt);

    // Clean up the memory
    deallocate_Array2D_f(&u_prev);
    deallocate_Array2D_f(&u_curr);
    deallocate_Array2D_f(&u_next);
}


/*
* Evaluates multiple timesteps of a wave solver.
*
* On output, u_curr contains the solution after nt timesteps.
*
* Arguments:
*   u_prev: Pointer to the array structure containing previous state
*   u_curr: Pointer to the array structure containing current state
*   u_next: Pointer to the array structure containing next state
*   dx: grid spacing in the x dimension
*   dt: size of temporal time step
*   nt: number of timesteps to evaluate
*
* Returns:
*   Error Code:
*      * 1: an array is improperly allocated.
*      * 2: dimension mismatch in arrays.
*/

int standing_wave_simulation_nsteps(Array2D_f* u_prev, Array2D_f* u_curr, Array2D_f* u_next, float dt, float dx, int nt) {

    // Error checks
    if ((u_prev->data == NULL) || (u_curr->data == NULL) || (u_next->data == NULL)) {
        fprintf(stderr, "Error: u_prev (%p), u_curr (%p), or u_next (%p) are null in wave_timestep.\n", 
                 u_prev->data, u_curr->data, u_next->data);
        return 1;
    }
    if ((u_prev->nx != u_curr->nx) || (u_prev->nx != u_next->nx) ||
        (u_prev->ny != u_curr->ny) || (u_prev->ny != u_next->ny)) {
        fprintf(stderr, "Error: u_prev (%u, %u), u_curr (%u, %u), or u_next (%u, %u) dimension mismatch.\n", 
                u_prev->nx, u_prev->ny, u_curr->nx, u_curr->ny, u_next->nx, u_next->ny);
        return 2;
    }


    // Perform nt timesteps of the simulation and swap the outputs so that u_curr and u_prev are ready for the next step.
    for(int k=0; k<nt; ++k) {

        wave_timestep(u_prev, u_curr, u_next, dt, dx);

        // swap the pointers around, no reallocation
        // This is a space to be very careful.  This function takes pointers to our _types_ as arguments.
        // The pointers themselves are passed-by-copy.  Therefore, we cannot swap these pointers.
        // However, we can swap their contents!  (We've already tested that the dimensions match.)
        float* temp;
        temp = u_prev->data;
        u_prev->data = u_curr->data;
        u_curr->data = u_next->data;
        u_next->data = temp;
    }
}

/**
 * Computes the norm of the error between 2 wave equation solutions.
 *
 * Arguments:
 * 		u_sim: Pointer to the array structure containing the simulated solution.
 * 		u_true: Pointer to the array structure containing the true solution.
 * 		nx: Number of grid points in the x-dimension.
 * 		ny: Number of grid points in the y-dimension.
 *
 * Returns:
 * 		* NULL: if pointer to u_sim or u_true is null.
 *		Norm of the error between 2 wave equation solutions as a float.
 * 		
 */
float norm_error(Array2D_f* u_sim, Array2D_f* u_true, unsigned int nx, unsigned int ny) {
	
	// Error checks
	if (u_sim == NULL || u_true == NULL) {
		return -1.0;
	}
	
	float error = 0.0;
	float c = 1.0 / (float)(nx * ny);

#pragma omp parallel for default(none) shared(u_sim,u_true,nx,ny) reduction(+:error) 
	for (int j=0; j<ny-1; j++) {
		for (int i=0; i<nx-1; i++) {
			int idx = ji_to_idx(j, i, nx);
			error = error + ((u_sim->data[idx] - u_true->data[idx]) * (u_sim->data[idx] - u_true->					  data[idx]));
		}
	}

	error = c * sqrt(error);
	return error;
}
