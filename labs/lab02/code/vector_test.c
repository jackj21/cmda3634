#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "vector.h"


int test_vector_equal(Vector* v1, Vector* v2){
    int pass = 1;

    if (v1->N != v2->N){
        pass = 0;
        return pass;
    }

    for(int i=0; i < v1->N; i++){
        pass = pass && (fabs(v1->data[i] - v2->data[i]) < 1e-5);
    }
    return pass;
}

void setup_norm_test(Vector* vx, float* ans){
    allocate(vx, 100);
    for(int i=0;i < 100;i++) vx->data[i] = 2.1;
    *ans = 21.0;
}

void teardown_norm_test(Vector* vx, float* ans){
    deallocate(vx);
    *ans = 0.0;
}

void setup_normalize_test(Vector* vx, Vector* vy,
                          Vector* v_ans, Vector* v_zero){
    allocate(vx, 5);
    allocate(v_ans, 5);

    vx->data[0] = 3.0;
    vx->data[1] = 3.0;
    vx->data[2] = 4.0;
    vx->data[3] = 4.0;
    vx->data[4] = 5.0;

    v_ans->data[0] = 0.34641016;
    v_ans->data[1] = 0.34641016;
    v_ans->data[2] = 0.46188022;
    v_ans->data[3] = 0.46188022;
    v_ans->data[4] = 0.57735027;

    allocate(vy, 37);
    allocate(v_zero, 37);
}

void teardown_normalize_test(Vector* vx, Vector* vy,
                             Vector* v_ans, Vector* v_zero){
    deallocate(vx);
    deallocate(v_ans);
    deallocate(vy);
    deallocate(v_zero);
}

void setup_axpy_test(Vector* vx, Vector* vy, float* a,
                     Vector* vz, Vector* v_ans) {

    int n = 555;

    allocate(vx, n);
    allocate(vy, n);
    allocate(v_ans, n);
    allocate(vz, n);

    *a = 0.137;

    for(int i=0; i<n; i++){
        vx->data[i] = 0.5*i;
        vy->data[i] = 0.11*i + 0.1123;
        v_ans->data[i] = 0.137*0.5*i + 0.11*i + 0.1123;
    }
}

void teardown_axpy_test(Vector* vx, Vector* vy, float* a,
                        Vector* vz, Vector* v_ans) {
    deallocate(vx);
    deallocate(vy);
    deallocate(v_ans);
    *a = 0.0;
    deallocate(vz);
}

void setup_inner_product_test(Vector* vx, Vector* vy, float* s_ans){

    int n = 654;

    allocate(vx, n);
    allocate(vy, n);

    *s_ans = 0.0;

    for(int i=0; i<n; i++){
        vx->data[i] = 0.5*i;
        vy->data[i] = 0.11*i + 0.1123;
        *s_ans += (0.5*i)*(0.11*i + 0.1123);
    }
}

void teardown_inner_product_test(Vector* vx, Vector* vy, float* s_ans){
    deallocate(vx);
    deallocate(vy);
    *s_ans = 0.0;
}

int main()
{
    int all_tests = 1;
    int tests[100];
    int test_counter = 0;

    int err = 0;

    Vector vx;
    Vector vy;
    Vector vz;
    float alpha;

    Vector v_ans;
    Vector v_zero;
    float s_ans;

    for(int i=0;i<100;i++) tests[i] = 1;

    // Test allocate for success
    err = allocate(&vx, 13);

    // Test 1
    tests[test_counter] = !err;
    test_counter++;

    // Test 2
    tests[test_counter] = (vx.data != NULL);
    test_counter++;

    // Test 3
    tests[test_counter] = (vx.N == 13);
    test_counter++;

    // Test initialize for 0
    initialize(&vx);

    // Test 4
    for(int i=0; i<13;i++){
        tests[test_counter] = tests[test_counter] && (((int)vx.data[i]) == 0);
    }
    test_counter++;

    // Test allocate for failure on SIZE_MAX
    // We fake this by giving malloc a negative number
    err = allocate(&vy, -1);

    // Test 5
    tests[test_counter] = err;
    test_counter++;

    // Test deallocate for success
    err = deallocate(&vx);

    // Test 6
    tests[test_counter] = !err;
    test_counter++;

    // Test deallocate for NULL ptr return
    // Test 7
    tests[test_counter] = (vx.data == NULL);
    test_counter++;

    err = deallocate(&vx);

    // Test norm for return value
    setup_norm_test(&vx, &s_ans);

    // Test 8
    tests[test_counter] = (fabs(s_ans - norm(&vx)) < 1e-5);
    test_counter++;

    teardown_norm_test(&vx, &s_ans);

    // Test normalize for success
    // Test normalize for 0 vector
    setup_normalize_test(&vx, &vy, &v_ans, &v_zero);

    // Test 9
    normalize(&vx);
    tests[test_counter] = test_vector_equal(&vx, &v_ans);
    test_counter++;

    // Test 10
    normalize(&vy);
    tests[test_counter] = test_vector_equal(&vy, &v_zero);
    test_counter++;

    teardown_normalize_test(&vx, &vy, &v_ans, &v_zero);

    // Test axpy for success

    setup_axpy_test(&vx, &vy, &alpha, &vz, &v_ans);

    // Test 11
    tests[test_counter] = !axpy(alpha, &vx, &vy, &vz);
    test_counter++;

    // Test 12
    tests[test_counter] = test_vector_equal(&vz, &v_ans);
    test_counter++;

    teardown_axpy_test(&vx, &vy, &alpha, &vz, &v_ans);

    // Test axpy for wrong size x,y
    // Test axpy for wrong size x,z
    // Test axpy for wrong size y,z

    allocate(&vx, 12);
    allocate(&vy, 123);
    allocate(&vz, 12);

    // Test 13
    tests[test_counter] = axpy(1.0, &vx, &vy, &vz);
    test_counter++;

    deallocate(&vy);
    deallocate(&vz);

    allocate(&vy, 12);
    allocate(&vz, 123);

    // Test 14
    tests[test_counter] = axpy(1.0, &vx, &vy, &vz);
    test_counter++;

    deallocate(&vx);
    deallocate(&vy);
    deallocate(&vz);

    // Test inner_product for success
    setup_inner_product_test(&vx, &vy, &s_ans);

    // Test 15
    tests[test_counter] = !inner_product(&vx, &vy, &alpha);
    test_counter++;

    // Test 16
    tests[test_counter] = (fabs(alpha - s_ans) < 1e-5);
    test_counter++;

    teardown_inner_product_test(&vx, &vy, &s_ans);

    // Test inner_product for wrong size x, y

    allocate(&vx, 12);
    allocate(&vy, 123);

    // Test 17
    tests[test_counter] = inner_product(&vx, &vy, &alpha);
    test_counter++;

    deallocate(&vx);
    deallocate(&vy);

    all_tests = 1;

    for(int i=0; i < test_counter; i++){
        all_tests = all_tests && tests[i];
        if (tests[i]){
            printf("Test %d: Pass\n", i+1);
        }
        else{
            printf("Test %d: Fail\n", i+1);
        }
    }
    if (all_tests){
        printf("All Tests: Pass\n");
    }
    else{
        printf("Some Tests: Fail\n");
    }
    return all_tests;
}
