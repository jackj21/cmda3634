/*
* Authors:
*   Russell J. Hewett (rhewett@vt.edu)
* Adapted for Project 3 By:
* 	Jack Jiang (jackj21)
*
* Not licensed for external distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "array_2d.h"
#include "wave.h"


int main(int argc, char** argv){

    if(argc != 7){
        printf("Incorrect number of parameters.  Correct usage:\n./wave_timing n Mx My alpha nt n_threads\n");
        return 1;
    }
	
	int n_threads = atoi(argv[6]);
	omp_set_num_threads(n_threads);

    // Setup the timers.
	double t_start, t_end;
	double t_total;

    // Extract command line arguments.
    unsigned int n = atoi(argv[1]);
    unsigned int Mx = atoi(argv[2]);
    unsigned int My = atoi(argv[3]);

    float alpha = atof(argv[4]);
    int nt = atoi(argv[5]); 

    // Perform the task.
	t_total = 0.0;
	
	t_start = omp_get_wtime();
    standing_wave_simulation(nt, n, Mx, My, alpha);

    t_end = omp_get_wtime();

	t_total = t_end - t_start;

    printf("%d %d %lf %d\n", n, nt, t_total, n_threads);
    
    return 0;
}
