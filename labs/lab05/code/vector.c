#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <omp.h>

#include "vector.h"

int allocate_Vector(Vector *v, int N){

    v->N = N;
    v->data = malloc(sizeof(float)*N);

    // in case malloc failed
    if(v->data == NULL)
        return 1;

    // if malloc succeeded, set entries to 0
    memset(v->data, 0, sizeof(float)*N);

    return 0;
}

int deallocate_Vector(Vector *v){

    v->N = 0;

    free(v->data);
    v->data = NULL;

    return 0;
}

int rand_fill_Vector(Vector *v){

    srand(time(NULL));

    // check that N of vector is reasonable
    if(v->N < 1)
        return 1;

    // loop through and fill vector with random numbers 0 to 1
    for(int i=0; i < v->N; i++){
        v->data[i] = (float)rand() / (float)RAND_MAX;
    }
    return 0;
}

int zero_fill_Vector(Vector *v){

    // check that N of vector is reasonable
    if(v->N < 1)
        return 1;

    memset(v->data, 0, sizeof(float)*v->N);

    return 0;
}

int axpy(float alpha, Vector* vx, Vector* vy, Vector* vz) {

    int i;
    int N = vx->N;

    float* vx_data = vx->data;
    float* vy_data = vy->data;
    float* vz_data = vz->data;

    // Sanity check on array dimensions
    if ((vx->N != vy->N) ||
        (vx->N != vz->N)){
        return 1;
    }

    for(i=0; i < N; i++){
        vz_data[i] = alpha*vx_data[i] + vy_data[i];
    }

    return 0;
}

int inner_product(Vector* vx, Vector* vy, float* ip) {

    int i;
    int N = vx->N;

    float* vx_data = vx->data;
    float* vy_data = vy->data;

    float result = 0.0;

    // Sanity check on array dimensions
    if (vx->N != vy->N) return 1;

    for(i=0; i < N; i++){
        result += vx_data[i]*vy_data[i];
    }

    *ip = result;

    return 0;
}
