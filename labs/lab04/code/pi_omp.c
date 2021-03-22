#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv){

    long long int n_tests = atoll(argv[1]);

    // Take number of Ope threads as a command line argument
    int n_threads = atoi(argv[2]);

    double tic = omp_get_wtime();

    //Number of points landing inside quarter of unit circle in the first quadrant
    long long int n_inside = 0;
    long long n;

    double estpi = 0;


    // Parallelize random number generation
    // TODO
    struct drand48_data rand_buffer;
    int seed = time(NULL);
    srand48_r(seed, &rand_buffer);

    // Parallelize the loop
    // TODO
    for(n=0; n < n_tests; ++n){
        double x;
        double y;

        drand48_r(&rand_buffer, &x);
        drand48_r(&rand_buffer, &y);

        // Check to see if radius is less than 1
        // TODO
        if(1){
            n_inside++;
        }
    }

    // Divide area of circle by area of square
    // TODO
    estpi = 0;

    double toc = omp_get_wtime();
    double elapsed = toc - tic;
    
    printf("estPi = %lf\n", estpi);
    printf("dt = %f\n", elapsed);


    return 0;
}

