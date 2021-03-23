#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv){

    long long int n_tests = atoll(argv[1]);

    clock_t tic = clock();

    //Number of points landing inside quarter of unit circle in the first quadrant
    long long int n_inside = 0;
    long long n;

    double estpi = 0;

    srand48(time(NULL));

    for(n=0; n < n_tests; ++n){
        double x = drand48();
        double y = drand48();

        // Check to see if radius is less than 1
        if((pow(x,2) + pow(y,2)) < 1){
            n_inside++;
        }
    }

    // Divide area of circle by area of square
    estpi = 4*(double) n_inside / (double) n_tests;

    clock_t toc = clock();
    double elapsed = (double)(toc-tic)/CLOCKS_PER_SEC;

    printf("estPi = %lf\n", estpi);
    printf("dt = %f\n", elapsed);


    return 0;
}

