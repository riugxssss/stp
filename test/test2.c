#include <stdio.h>

#include "stp_threadpool.h"

void aa(void *a){

    int *var = (int *)(a);
    printf("aa function\n");
    printf("%d\n", *var + 0);
}
void bb(void *a){
    int *var = (int *)(a);
    printf("bb function\n");
    printf("%d\n", *var + 1);
}
void cc(void *a){
    int *var = (int *)(a);
    printf("%d\n", *var + 2);
}
void dd(void *a){
    int *var = (int *)(a);
    printf("dd function\n");
    printf("%d\n", *var + 3);
}
void ee(void *a){
    int *var = (int *)(a);
    printf("%d\n", *var + 4);
}
void ff(void *a){
    int *var = (int *)(a);
    printf("%d\n", *var + 5);
}
void gg(void *a){
    int *var = (int *)(a);
    printf("%d\n", *var + 6);
}
void zz(void *a){
    int *var = (int *)(a);
    printf("%d\n", *var + 8);
}
void yy(void *a){
    int *var = (int *)(a);
    printf("%d\n", *var + 9);
}
void xx(void *a){
    int *var = (int *)(a);
    printf("xx function\n");
    printf("%d\n", *var + 10);
}
void qq(void *a){
    int *var = (int *)a;
    printf("%d\n", *var + 11);
}
void ss(void *a){
    int *var = (int *)(a);

    printf("ss function\n");
    printf("%d\n", *var + 13);
}


int main(void){

    stp_tp_t *pool = stp_create_pool(7, 256);
    int
        a = 1,
        c = 2,
        d = 3,
        e = 5,
        f = 7;

    stp_tp_add(pool, ss, &a, 1);
    stp_tp_add(pool, aa, &c, 7);
    stp_tp_add(pool, bb, &d, 3);
    stp_tp_add(pool, dd, &e, 2);
    stp_tp_add(pool, xx, &f, 6);

    stp_tp_start(pool);
    stp_tp_free(&pool, GRC_MODE);

    return 0;
}
