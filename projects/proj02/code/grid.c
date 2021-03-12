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
	a->data = malloc((ny)*(nx)*sizeof(float));
	
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
 * 		0 to indicate completition of function..
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
		a->data[index] = 0;	
		}
	}
	return 0;
}

/**
 * Makes a deep copy of the grid data structure.
 *
 * Parameter:
 * 		a: Pointer to Grid object to be copied.
 * 		aCopy: Pointer to Grid object where deep copy is stored.
 *
 * Returns:
 * 		0 if successfully copied Grid a.
 * 		1 if a or aCopy are passed in as a NULL value.
 */
int copy(Grid* a, Grid* aCopy) {
	if (a == NULL || aCopy == NULL)
		return 1;
	unsigned int length = (aCopy->n_y) * (aCopy->n_x);
	for (int i =0; i < length; ++i) {
		aCopy->data[i] = a->data[i];
	}

	return 0;	
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
int save(Grid* a, char* file_name) {
	if (a == NULL) {
		return 1;	
	}
	
	FILE* fp;	
	fp = fopen(file_name, "wb");

	if (fp = NULL)
		return 1;
	
	char n_x[a->n_x];
	char n_y[a->n_y];
	itoa(a->n_x, n_x, 10);
	itoa(a->n_y, n_y, 10);
	
	fwrite(n_y, sizeof(unsigned int), 1, fp);
	fwrite("\n", sizeof(char), 1, fp);
	fwrite(n_x, sizeof(unsigned int), 1, fp);
	fwrite("\n", sizeof(char), 1, fp);
	fwrite(a->data, sizeof(a), 1, fp);
	
	fclose(fp);
	
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
 * 		0 if wave equation successfully evaluated and stored into Grid.
 * 		1 if evaluation runs into error.
 */
int wave_eq(Grid* a, int t, int m_x, int m_y) {
	float omega = PI * sqrt((pow(m_x, 2.0)) + (pow(m_y, 2.0)));
	float d_x = 1 / (a->n_x - 1);
	float d_y = 1 / (a->n_y - 1);
		
	for (int j=0; j<(a->n_y); ++j) {
		for (int i=0; i<(a->n_x); ++i) {
			float y = j * d_y;
			float x = i * d_x;
			unsigned int ind = get_1D_index(j, i, a->n_x);
			*(a->data) + ind = sin(m_x * PI * x) * sin(m_y * PI * y) * cos(omega * t);
			// a->data[ind] = ^^
		}
	}
	return 0;
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
	if (j == 0 || j == (n_y - 1) || i == 0 || i == (n_x - 1))
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
 * 		next: Next time step.
 * 		dt: delta t.
 *
 * Returns:
 * 		0 if timestep was properly computed.
 * 		1 if timestep ran into an error.
 */ 
int timestep(unsigned int n_y, unsigned int n_x, Grid* prev, Grid* curr, Grid* next, float dt) {
	if (n_y == 0 || n_x == 0 || prev == NULL || curr == NULL || next == NULL) 
		return 1;
	
	float d_x = 1 / (n_x - 1);
	float d_y = 1 / (n_y - 1);

	for (int j=0; j<(next->n_y); ++j) {
		for (int i=0; i<(next->n_x); ++i) {
			unsigned int ji = get_1D_index(j, i, next->n_x);
			if (boundary_check(next->n_y, next->n_x, j, i))
				next->data[ji] = 0;
			else {
				unsigned int jm1i = get_1D_index(j-1, i, next->n_x);
				unsigned int jim1 = get_1D_index(j, i-1, next->n_x);
				unsigned int jp1i = get_1D_index(j+1, i, next->n_x);
				unsigned int jip1 = get_1D_index(j, i+1, next->n_x);
	
				//           -4*curr[j][i]               curr[j-1][i]
				lap = ((-4 * curr->data[ind])) + (curr->data[jm1i])
				//			  curr[j][i-1]             curr[j+1][i]
						+ (curr->data[jim1]) + (curr->data[jp1i])
				//            curr[j][i+1]           dx^2
						+ (curr->data[jip1]) / pow(d_x, 2);

				//   next[j][i]        -prev [j][i]            2*curr[j][i]
				next->data[ji] = (-(prev->data[ji]) + (2*(curr->data[ji]))
					+ (pow(dt, 2) * lap));
			}
		}
	}

	return 0;
}

/**
 * Simulates the wave equation with initial conditions u_m1 and u_0
 * and saves data onto the disk in separate files according
 * to iteration # with corresponding file names. 
 *
 * Parameters:
 * 		T: Final simulation time.
 * 		n_y: Number of rows.
 * 		n_x: Number of columns.
 * 		m_x: Number of standing nodes.
 * 		m_y: Number of standing nodes.
 *
 * Returns:
 * 		0 if simulation ran correctly with no errors.
 * 		1 if simulation ran into an error.
 */
int simulate(unsigned int T, unsigned int n_y, unsigned int n_x, int m_x, int m_y) {	
	double alpha = 1.0;
	float d_xy = 1 / (a->n - 1);
	float dt = (alpha * d_xy) / sqrt(2);
	int n_t = round(T / dt);
	int check = 0;

	Grid* prev;
	Grid* curr;
	Grid* next;
	
	check = allocate(prev, n_y, n_x); 	
	if (check == 1)
		return 1;	
	
	check = allocate(curr, n_y, n_x);
	if (check == 1)
		return 1;
	
	check = allocate(next, n_y, n_x); 
	if (check == 1)
		return 1;

	// Initialize u_m1 and u_0
	initialize(prev);
	initialize(curr);
	initialize(next);
	
	// Get initial conditions with t=-dt and t=0
	wave_eq(prev, -dt, m_x, m_y);
	wave_eq(curr, 0, m_x, m_y);

	// SAVE PREV AND CURR TO FILES HERE...
	// save(prev, n_t) n_t = -1 and 0?
	// save(curr, n_t)
	char* file_name_m1 = calloc(13, sizeof(char));
	char* file_name_0 = calloc(12, sizeof(char));
	sprintf(file_name_m1, "Iteration#%d.bin", -1);
	sprintf(file_name_0, "Iteration#%d.bin", 0);
	save(prev, file_name_m1);
	save(curr, file_name_0);

	free(file_name_m1);
	free(file_name_0);

	// Run simulation...
	// IMPLEMENT CLOCK IN TIME.H HERE? look at analysis...
	//   start time
	for (int i =0; i < n_t; ++i) {
		timestep(n_y, n_x, prev, curr, next, dt);
			
		char* file_name = calloc(11 + n_t, sizeof(char)); 
		sprintf(file_name, "Iteration#%d.bin", i + 1);
		save(next, file_name);		

		copy(curr, prev);
		copy(next, curr);

		free(file_name);
	}
	
	deallocate(next); 
	deallocate(curr);
	deallocate(prev);
	
		

}


