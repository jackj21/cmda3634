#include "grid.h"

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
	a->data = malloc(n_y*n_x*sizeof(float));
	
	if (a->data == NULL) 
		return 1;
		
	u->n_y = ny;
	u->n_x = nx;

	return 0;	
}

/**
 * Deallocates a Grid object and resets its elements to default.
 *
 * Parameters:
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
 *
 */
int initialize(Grid* a) {
	
	
}

/**
 *
 *
 *
 */
int copy(Grid* a) {
	
	
}

/**
 *
 */
int save(Grid* a) {
	
	
}
