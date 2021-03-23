#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv){

    long long int n_tests = atoll(argv[2]);

    // Take number of Ope threads as a command line argument
    int n_threads = atoi(argv[1]);
	omp_set_num_threads(n_threads);
	// **Number of sample points N and number of threads T

    double tic = omp_get_wtime();

    //Number of points landing inside quarter of unit circle in the first quadrant
    long long int n_inside = 0;
    long long n;

    double estpi = 0;


    // Parallelize random number generation
	struct drand48_data rand_buffer;
	int seed = time(NULL);

#pragma omp parallel default(none) private(rand_buffer,n) shared(n_tests,n_inside)
	{
		int thread_id = omp_get_thread_num();
		int seed = time(NULL) + thread_id;		
    	srand48_r(seed, &rand_buffer);
	
    // Parallelize the loop
#pragma omp for reduction(+:n_inside)
    for(n=0; n < n_tests; ++n){
        double x;
        double y;

        drand48_r(&rand_buffer, &x);
        drand48_r(&rand_buffer, &y);

        // Check to see if radius is less than 1
        if((pow(x, 2) + pow(y, 2)) < 1){
            n_inside++;
        }
    }

	} // end parallel region
    // Divide area of circle by area of square
    estpi = 4 * (double)n_inside / (double)n_tests;

    double toc = omp_get_wtime();
    double elapsed = toc - tic;
    
	printf("%d, %llu, %lf, %lf\n", n_threads, n_tests, estpi, elapsed);
	


    return 0;
}

