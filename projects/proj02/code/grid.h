#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef __GRID_H__
#define __GRID_H__

typedef struct Grid_tag {
	unsigned int n_y;	// unsigned int for number of rows
	unsigned int n_x;	// unsigned int for number of columns
	float* data;

} Grid;

int allocate(Grid* a, unsigned int ny, unsigned int nx);
int deallocate(Grid* a);
int initialize(Grid* a);
int copy(Grid* a, Grid* aCopy);
int save(Grid* a, char* file_name);
int wave_eq(Grid* a, int t, int m_x, int m_y);
int timestep(unsigned int n_y, unsigned int n_x, Grid* prev, Grid* curr, Grid* next, float dt);
int simulate(unsigned int T, unsigned int n_y, unsigned int n_x, int m_x, int m_y);

#endif
