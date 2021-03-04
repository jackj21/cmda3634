#include <stdlib.h> // for random number generation
#include <time.h> // for clock timing
#include <stdio.h> // for input/output
#include "vector.h"

void random_index_shuffle(int *orig, int N){
    // This function randomly permutes (in place) the entries of the int array pointed to by orig
    srand(time(NULL)); // see random number generator   

    for(int i = 0; i < N; ++i){
        // randomly choose an entry of origIndices 
        int new_idx = rand() % N;
        // swap the entries at i and new_idx
        int temp = orig[i];
        orig[i] = orig[new_idx];
        orig[new_idx] = temp;
    }
}


int main(int argc, char *argv[]){

    // check that a command line argument specifying the dimension is present
    if(argc < 2){
        printf("WARNING: Missing one input argument. \n");
        printf("Run code as > executable_name dimension_of_test (filename_for_timing_data) \n");
        return 1;
    } 
    // argv[1] holds the dimension of the test vectors as a string
    // so use atoi function (part of stdlib.h) to change to an interger
    int dim = atoi(argv[1]);

    float a = 1.5;
    Vector x;
    Vector y;
    Vector z;

    // allocate the vectors and fill x and y with random numbers
    allocate(&x, dim);
    allocate(&y, dim);
    allocate(&z, dim);
    random_fill(&x);
    random_fill(&y);

    // number of runs, and declare timers
    int n_trials = 100; 
    clock_t beginning, ending;

    // First, choose the random order (so not part of timing)
    int *idx_order = (int*)calloc(dim, sizeof(int));
    for(int i = 0; i < dim; ++i) {
        idx_order[i] = i; // start with indices in order
    }
    random_index_shuffle(idx_order, dim); // randomly permute the indices

    // run the axpy in random order n_trials times
    beginning = clock();
    for(int i = 0; i < n_trials; ++i) {
        axpy_random(a, &x, &y, &z, idx_order);
    }
    ending = clock();

    double secondsToRun = (double)(ending-beginning) / (double)CLOCKS_PER_SEC;
    double avgSecPerRunRand = secondsToRun/(double)n_trials;

    printf("%d, %17.15f\n", dim, avgSecPerRunRand);

    // cleanup
    free(idx_order);
    deallocate(&x);
    deallocate(&y);
    deallocate(&z);

    return 0;
}
