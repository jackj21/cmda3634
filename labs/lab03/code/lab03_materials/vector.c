#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "vector.h"

int allocate(Vector* v, int dimension) {

    v->dimension = dimension;
    v->data = malloc(sizeof(float)*dimension);

    // malloc failed
    if (v->data == NULL) return 1;

    memset(v->data, 0, sizeof(float)*dimension); 

    return 0;
}

int deallocate(Vector* v) {

    v->dimension = 0;

    free(v->data);
    v->data = NULL;

    return 0;
}

int random_fill(Vector* v){
    // seed with the current time
    srand(time(NULL));
    // check dimension of vector
    int dim = v->dimension;
    // loop through and fill vector with random floats between 0 and 1
    for(int i=0; i<dim; i++){
        // rand() generates ints between 0 and RAND_MAX, so scale by RAND_MAX
        v->data[i] = (float)rand() / RAND_MAX; 
    }

    return 0;
}


int axpy(float alpha, Vector* vx, Vector* vy, Vector* vz) {

    // Sanity check on array dimensions
    if ((vx->dimension != vy->dimension) ||
        (vx->dimension != vz->dimension)){
        return 1;
    }

    for(int i=0; i < vx->dimension; i++){
        vz->data[i] = alpha*vx->data[i] + vy->data[i];
    }

    return 0;
}


int axpy_random(float alpha, Vector* vx, Vector* vy, Vector* vz, int* indexOrdering) {

    // Sanity check on array dimensions
    if ((vx->dimension != vy->dimension) ||
        (vx->dimension != vz->dimension)){
        return 1;
    }

    // actually do the axpy
    for(int i=0; i < vx->dimension; i++){
        int randID = indexOrdering[i];
        vz->data[randID] = alpha*vx->data[randID] + vy->data[randID];
    }

    return 0;
}