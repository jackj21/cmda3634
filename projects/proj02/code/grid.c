#include "grid.h"
#include <math.h>
#define PI 3.14159265358979323846

/**
 * Dynamically allocates a two dimensional Grid data structure.
 *
 * Parameters:
 * 		a: Pointer to a Grid object to be allocated.
 * 		n_y: Number of rows for the Grid object.
 *		n-x: Number of columns for the Grid object.
 *
 * Returns:
 * 		0 if allocation was successful.
 * 		1 if allocation was not successful.
 */
int allocate(Grid* a, unsigned int ny, unsigned int nx) {
	a->data = malloc((a->n_y)*(a->n_x)*sizeof(float));
	
	if (a->data == NULL) 
		return 1;
		
	a->n_y = ny;
	a->n_x = nx;

	return 0;	
}

/**
 * Deallocates a Grid object and resets its elements to default.
 *
 * Parameter:
 * 		a: Pointer to a Grid object to be deallocated.
 *
 * Returns:
 * 		0 if deallocation was successful.
 * 		1 if deallocation was not successful.
 */
int deallocate(Grid* a) {
	if (a->data == NULL) {
		a->n_y = 0;
		a->n_x = 0;
		return 0;	
	}

	free(a->data);
	a->data = NULL;
	a->n_y = 0;
	a->n_x = 0;

	return 0;
}

/**
 * Gets the 1 dimensional index of a linear array
 * from a 2 dimensional array.
 *
 * Parameters:
 * 		j: indicates the specific row of a point in the 2D array.
 * 		i: indicates the specific column of a point in the 2D array.
 * 		n_x: the length/number of columns of the 2D array.
 * 
 * Returns:
 * 		1D index of a linear array.
 */
unsigned int get_1D_index(unsigned int j, unsigned int i, unsigned int n_x) {
	return j * n_x + i;
		
}
 
/**
 * Initializes the Grid to 0.
 *
 * Parameters:
 * 		a: Pointer to Grid object to be initialized.
 *
 * Returns:
 * 		0 to signify initialization was successful.
 */
int initialize(Grid* a) {
	for (int i = 0; i<a->n_y; ++i) {
		for (int j = 0; j<a->n_x; ++i) {
		unsigned int index = get_1D_index(j, i, a->n_x);
		*a->data + index = 0;	
		}
	}
	return 0;
}

/**
 * Makes a deep copy of the grid data structure.
 *
 * Parameter:
 * 		a: Pointer to Grid object to be copied.
 *
 * Returns:
 * 		A new array representing a deep copy of
 * 		the array passed in.
 */
Grid* copy(Grid* a) {
	Grid* a_new;
	allocate(a_new, a->n_y, a->nx);
	unsigned int length = (a->n_y) * (a->n_x);
	for (int i =0; i < length; ++i) {
		*(a_new->data) + i = *(a->data) + i;
	}

	return a_new;	
}

/**
 * Saves grid to a file.
 *
 * Paramters:
 * 		a: Pointer to a Grid object to be saved.
 * 		file_name: Name of file to which Grid object is being saved to.
 *
 * Returns:
 * 		0 if save is successful.
 * 		1 is save is unsuccessful due to Grid object or file pointer  being null.
 */
int save(Grid* a, char** file_name) {
	if (a == NULL) {
		return 1;	
	}
	
	FILE* f;
	f = fopen(file_name, "wb");

	if (f = NULL)
		return 1;
	
	fwrite(a, sizeof(a), 1, file_name);
	
	fclose(f);

	return 0;
	
}

/**
 * Evaluates the standing wave solution to the wave equation
 * on a grid and stores data into the Grid.
 *
 * Paramters:
 * 		a: Pointer to Grid object to which wave equation and data
 * 			will be stored.
 * 		t: Current time in the simulation.
 * 		m_x: Number of stationary nodes on x-axis.
 * 		m_y: Number of stationary nodes on y-axis.
 *
 * Returns:
 * 		Pointer to Grid object that has wave equation 
 * 		evaluated and data stored.
 */
Grid* wave_eq(Grid* a, int t, int m_x, int m_y) {
	float omega = PI * sqrt((pow(m_x, 2.0)) + (pow(m_y, 2.0)));
	float d_x = 1 / (a->n_x - 1);
	float d_y = 1 / (a->n_y - 1);

	initialize(a);
	
	for (int j=0; j<(a->n_y); ++j) {
		for (int i=0; i<(a->n_x); ++i) {
			y = j * d_y;
			x = i * d_x;
			unsigned int ind = get_1d_index(j, i, a->n_x);
			*(a->data) + ind = sin(m_x * PI * x) * sin(m_y * PI * y) * cos(omega * t);
			// a->data[ind] = ^^
		}
	}
	return a;

}

/**
 * Private helper function that checks if grid point 
 * is a boundary point.
 *
 * Parameters:
 * 		n_y: Number of rows.
 * 		n_x: Number of columns.
 * 		j: x value of grid point.
 * 		i: y value of grid point.
 *
 * Returns:
 * 		True if grid point is a boundary point.
 * 		False if grid point is not a boundary point.
 */
bool boundary_check(unsigned int n_y, unsigned int n_x, int j, int i) {
	if (j == 0 || j == (n_y - 1) || i == 0 || i == (n_x - 1)
		return true;
	return false;	
	
}

/**
 * Computes one time step of the wave equation simulation on the Grid.
 *
 * Parameters:
 * 		n_y: Number of rows.
 * 		n_x: Number of columns:
 * 		prev: Previous time step.
 * 		curr: Current time step.
 * 		dt: delta t.
 *
 * Returns:
 * 		The a pointer to next time step of the wave equation simulation,
 * 		stored in a Grid object.
 */ 
Grid* timestep(unsigned int n_y, unsigned int n_x, Grid* prev, Grid* curr, float dt) {
	Grid* next = curr;
	
	float d_x = 1 / (a->n_x - 1);
	float d_y = 1 / (a->n_y - 1);

	for (int j=0; j<(next->n_y); ++j) {
		for (int i=0; i<(next->n_x); ++i) {
			unsigned int ji = get_1d_index(j, i, next->n_x);
			if (boundary_check(next->n_y, next->n_x, j, i)
				*(next->data) + ji = 0;
			else {
				unsigned int jm1i = get_1d_index(j-1, i, next->n_x);
				unsigned int jim1 = get_1d_index(j, i-1, next->n_x);
				unsigned int jp1i = get_1d_index(j+1, i, next->n_x);
				unsigned int jip1 = get_1d_index(j, i+1, next->n_x);
	
				//           -4*curr[j][i]               curr[j-1][i]
				lap = ((-4 * (*curr->data + ind)) + (*curr->data + jm1i)
				//			  curr[j][i-1]             curr[j+1][i]
						+ (*curr->data + jim1) + (*curr->data + jp1i)
				//            curr[j][i+1]           dx^2
						+ (*curr->data + jip1)) / pow(d_x, 2);

				//   next[j][i]        -prev [j][i]            2*curr[j][i]
				*next->data + ji = (-(*prev->data + ji) + (2*(*curr->data + ji))
					+ (pow(dt, 2) * lap));
			}
		}
	}

	return next;
}

/**
 *
 */
Grid* simulate(unsigned int T, unsigned int n, int m_x, int m_y) {	
	double alpha = 1.0;
	float d_xy = 1 / (a->n - 1);
	float dt = (alpha * d_xy) / sqrt(2);
	
	Grid prev;
	Grid curr;
	Grid next;

	allocate(&prev, n, n); 		// ***Double check ny=nx=n for this task***
	allocate(&curr, n, n);
	nt = ...
	//   start time
	for (int i =0; i < nt; ++i)
	
	deallocate(&curr);
	deallocate(&prev);
	


}


/**
 * QUESTIONS
 * 		1. How to check if functions are working as intended?
 *		2. Good framework?
 *		3. For save, to write as binary, do you just do fwrite(..."wb")?
 *		4. In the "Produce images of some solutions", when saving data files,
 *		is it implying we use our save method? And for the iteration number,
 *		should we have a global variable of some sort to keep track?
 *		5. Will simulate (task 4) return list of iterations, n_t, or something else?
 *		6. Task 4 - Do I allocate only 2 Grid data structs and then change their values
 *		for the simulation instead of creating new Grid data structs?
 *
 */ 
