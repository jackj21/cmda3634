#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct Grid_tag {
	unsigned int n_y;	// unsigned int for number of rows
	unsigned int n_x;	// unsigned int for number of columns
	float* data;

} Grid;

int allocate(Grid* a, unsigned int ny, unsigned int nx);
int deallocate(Grid* a);
int initialize(Grid* a);
Grid* copy(Grid* a);
int save(Grid* a, char** file_name);
Grid* wave_eq(Grid* a, int t, int m_x, int m_y);
Grid* timestep(unsigned int n_y, unsigned int n_x, Grid* prev, Grid* curr, float dt);


#endif
