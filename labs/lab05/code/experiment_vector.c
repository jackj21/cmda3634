#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

#include "vector.h"

int experiment(int N, int n_trials, int n_threads){

    omp_set_num_threads(n_threads);

    printf("vector, %4d, %4d, %4d", n_threads, n_trials, N);

    Vector x, y;
    Vector b;

    allocate_Vector(&x, N);
    allocate_Vector(&y, N);
    allocate_Vector(&b, N);

    float retval;

    rand_fill_Vector(&x);
    rand_fill_Vector(&y);
    zero_fill_Vector(&b);

    // set up number of repeat runs, and declare timers
    double t_start, t_end;
    double t_total, t_avg;


    // time axpy
    t_total = 0.0;
    t_avg = 0.0;

#pragma omp parallel default(none) private(t_start,t_end) shared(n_trials,t_total,x,y,b) 
	{
#pragma omp for reduction(+:t_total)
    for(int i=0; i<n_trials; i++){
        t_start = omp_get_wtime();
        axpy(0.5, &x, &y, &b);
        t_end = omp_get_wtime();
        t_total += (t_end - t_start);
    }
	
	} //end parallel region

    t_avg = t_total / n_trials;

    printf(", %.10f", t_avg);

    // time inner_product
    t_total = 0.0;
    t_avg = 0.0;

#pragma omp parallel default(none) private(t_start,t_end) shared(n_trials,t_total,x,y,retval) 
	{
#pragma omp for reduction(+:t_total)
    for(int i=0; i<n_trials; i++){
        t_start = omp_get_wtime();
        inner_product(&x, &y, &retval);
        t_end = omp_get_wtime();
        t_total += t_end - t_start;
    }

	} // end parallel region

    t_avg = t_total / n_trials;

    printf(", %.10f", t_avg);

    printf("\n");

    // cleanup
    deallocate_Vector(&x);
    deallocate_Vector(&y);
    deallocate_Vector(&b);

    return 0;
}


int main(int argc, char *argv[]){

    if(argc < 5){
        printf("WARNING: Missing arguments. \n");
        printf("Usage: experiment_vector N n_trials \n");
        printf("    N: vectors are Nx1\n");
        printf("    n_trials: Number of experiment trials to run\n");
        printf("    n_threads: Number of OpenMP threads to use\n");
        printf("    weak: 0 if strong scaling, 1 if week scaling\n");
        return 1;
    }

    // read input arguments to get number of rows and columns
    int N = atoi(argv[1]);
    int n_trials = atoi(argv[2]);
    int n_threads = atoi(argv[3]);
    int weak = atoi(argv[4]);

    if(weak){
        N *= n_threads;
    }

    experiment(N, n_trials, n_threads);

    return 0;
}
