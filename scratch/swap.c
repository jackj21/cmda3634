#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Container_tag{

    int* array;
    int number;

} Container;

void setupC(Container* a, Container* b){

    a->number = 5;
    a->array = malloc(5*sizeof(int));
    for(int i=0; i<5;i++) a->array[i] = 5;

    b->number = 7;
    b->array = malloc(7*sizeof(int));
    for(int i=0; i<7;i++) b->array[i] = 7;
}

void teardownC(Container* a, Container* b){

    free(a->array);
    free(b->array);

}

void swap(Container* a, Container* b){

    // This doesn't work, why?
    Container* temp;
    temp = a;
    a = b;
    b = temp;
}

int main(){
    
    printf("\n\n------------------------------------------\n");
    // A) Inline, variable
    // Swap the values stored by these variables.
    int a1 = 5;
    int a2 = 7;

    printf("A) Before: a1 = %d; a2 = %d\n", a1, a2);

    int tempA;
    tempA = a1;
    a1 = a2;
    a2 = tempA;

    printf("A) After:  a1 = %d; a2 = %d\n", a1, a2);


    printf("\n\n------------------------------------------\n");
    // B) Inline, pointer
    // Swap the integers referenced (pointed to) by these pointers.  Do not modify the referenced variables.
    int b_ref_1 = 5;
    int b_ref_2 = 7;

    int* b1 = &b_ref_1;
    int* b2 = &b_ref_2;

    printf("B) Before: b1 = %d; b2 = %d\n", *b1, *b2);

    // Swap here
	b1 = &b_ref_2;
	b2 = &b_ref_1;
    printf("B) After:  b1 = %d; b2 = %d\n", *b1, *b2);


    printf("\n\n------------------------------------------\n");
    // C) Inline, struct variable
    // Swap the data inside the containers.  Do not reallocate anything.
    Container c1;
    Container c2;
	
    setupC(&c1, &c2);

    printf("C) Before: c1.number = %d; c1.array = [%d %d %d %d %d]\n", c1.number, 
            c1.array[0], c1.array[1], c1.array[2], c1.array[3], c1.array[4]);

    printf("C) Before: c2.number = %d; c2.array = [%d %d %d %d %d %d %d]\n", c2.number, 
            c2.array[0], c2.array[1], c2.array[2], c2.array[3], c2.array[4], c2.array[5], c2.array[6]);

    // Swap here
	Container c1_temp;
	c1_temp = c1;
	c1 = c2;
	c2 = c1_temp;


    printf("C) After:  c1.number = %d; c1.array = [%d %d %d %d %d %d %d]\n", c1.number, 
            c1.array[0], c1.array[1], c1.array[2], c1.array[3], c1.array[4], c1.array[5], c1.array[6]);

    printf("C) After:  c2.number = %d; c2.array = [%d %d %d %d %d]\n", c2.number, 
            c2.array[0], c2.array[1], c2.array[2], c2.array[3], c2.array[4]);

    teardownC(&c1, &c2);


    printf("\n\n------------------------------------------\n");
    // D) Inline, struct pointer
    // Swap the data inside the containers.  Do not reallocate anything.
    Container d_ref_1;
    Container d_ref_2;
    setupC(&d_ref_1, &d_ref_2);

    Container* d1 = &d_ref_1;
    Container* d2 = &d_ref_2;


    printf("D) Before: d1->number = %d; d1->array = [%d %d %d %d %d]\n", d1->number, 
            d1->array[0], d1->array[1], d1->array[2], d1->array[3], d1->array[4]);

    printf("D) Before: d2->number = %d; d2->array = [%d %d %d %d %d %d %d]\n", d2->number, 
            d2->array[0], d2->array[1], d2->array[2], d2->array[3], d2->array[4], d2->array[5], d2->array[6]);

    // Swap here

    printf("D) After:  d1->number = %d; d1->array = [%d %d %d %d %d %d %d]\n", d1->number, 
            d1->array[0], d1->array[1], d1->array[2], d1->array[3], d1->array[4], d1->array[5], d1->array[6]);

    printf("D) After:  d2->number = %d; d2->array = [%d %d %d %d %d]\n", d2->number, 
            d2->array[0], d2->array[1], d2->array[2], d2->array[3], d2->array[4]);

    teardownC(&d_ref_1, &d_ref_2);


    printf("\n\n------------------------------------------\n");
    // E) Inside a function
    // Swap the data inside the containers using the swap() function.  Do not reallocate anything.
    Container e1;
    Container e2;
    setupC(&e1, &e2);

    printf("E) Before: e1.number = %d; e1.array = [%d %d %d %d %d]\n", e1.number, 
            e1.array[0], e1.array[1], e1.array[2], e1.array[3], e1.array[4]);

    printf("E) Before: e2.number = %d; e2.array = [%d %d %d %d %d %d %d]\n", e2.number, 
            e2.array[0], e2.array[1], e2.array[2], e2.array[3], e2.array[4], e2.array[5], e2.array[6]);

    // Fix this function
    swap(&e1, &e2);

    printf("E) After:  e1.number = %d; e1.array = [%d %d %d %d %d %d %d]\n", e1.number, 
            e1.array[0], e1.array[1], e1.array[2], e1.array[3], e1.array[4], e1.array[5], e1.array[6]);

    printf("E) After:  e2.number = %d; e2.array = [%d %d %d %d %d]\n", e2.number, 
            e2.array[0], e2.array[1], e2.array[2], e2.array[3], e2.array[4]);

    teardownC(&e1, &e2);


}
