#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct Grid_tag {
	unsigned int n_y;	// unsigned int for number of rows
	unsigned int n_x;	// unsigned int for number of columns
	float* data;

} Grid;

int allocate(Grid* a, unsigned int n_y, unsigned int n_x);
int deallocate(Grid* a);
int initialize(Grid* a);
int copy(Grid* a);
int save(Grid* a);


#endif
