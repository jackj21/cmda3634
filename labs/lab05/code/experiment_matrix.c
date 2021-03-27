#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

#include "vector.h"
#include "matrix.h"

int experiment(int N, int n_trials, int n_threads){

    omp_set_num_threads(n_threads);

    printf("matrix, %4d, %4d, %4d", n_threads, n_trials, N);

    Matrix A;
    Vector x;
    Vector b;

    allocate_Matrix(&A, N, N);
    allocate_Vector(&x, N);
    allocate_Vector(&b, N);

    rand_fill_Matrix(&A);
    rand_fill_Vector(&x);
    zero_fill_Vector(&b);

    // set up number of repeat runs, and declare timers
    double t_start, t_end;
    double t_total, t_avg;

    // time matvec
    t_total = 0.0;
    t_avg = 0.0;
    for(int i=0; i<n_trials; i++){
        t_start = omp_get_wtime();
        matvec(&A, &x, &b);
        t_end = omp_get_wtime();
        t_total += t_end - t_start;
    }
    t_avg = t_total / n_trials;

    printf(", %.10f", t_avg);

    // time matvec_triangular
    t_total = 0.0;
    t_avg = 0.0;
    for(int i=0; i<n_trials; i++){
        t_start = omp_get_wtime();
        matvec_triangular(&A, &x, &b);
        t_end = omp_get_wtime();
        t_total += t_end - t_start;
    }
    t_avg = t_total / n_trials;

    printf(", %.10f", t_avg);

    // time matvec_triangular_guided
    t_total = 0.0;
    t_avg = 0.0;
    for(int i=0; i<n_trials; i++){
        t_start = omp_get_wtime();
        matvec_triangular_guided(&A, &x, &b);
        t_end = omp_get_wtime();
        t_total += t_end - t_start;
    }
    t_avg = t_total / n_trials;

    printf(", %.10f", t_avg);

    printf("\n");

    // cleanup
    deallocate_Matrix(&A);
    deallocate_Vector(&x);
    deallocate_Vector(&b);

    return 0;
}


int main(int argc, char *argv[]){

    if(argc < 5){
        printf("WARNING: Missing arguments. \n");
        printf("Usage: experiment_matrix N n_trials \n");
        printf("    N: Matrices are NxN and vectors are Nx1\n");
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
        N = floor(N*sqrt((float)n_threads));
    }

    experiment(N, n_trials, n_threads);


    return 0;
}
