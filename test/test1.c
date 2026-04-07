#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*Thread pool library*/
#include "../include/stp_threadpool.h"

int main(void){

    int tot = 8;
    uint16_t qsize = 256;

    stp_tp_t *pool = stp_create_pool(tot, qsize);
    printf("Create and destroy\n");
    stp_tp_free(&pool, GRC_MODE);

    return 0;
}
