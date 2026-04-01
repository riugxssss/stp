/*
Thread Pool with priority queue

Copyright (C) 2026 R riugxss riugxs@gmail.com


This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "../include/stp_threadpool.h"
#include "../include/stp_queue.h"
#include "../include/stp_helper.h"

#define _DEBUG_

#ifdef _DEBUG_
# define PRINT_MSG(x)           fpritnf(stdout, "%s\n", x )
# define PRINT_INFO(f1, f2)     fprintf(stdout, "%s | %d\n", f1, f2)

#endif


typedef enum {
    immediate_shutdown = 10,
    graceful_shutdown = 11,
    threadpool_err_mutex = 20,
    threadpool_err_cond = 21,
    threadpool_err_free = 22
}stp_tpenum_t;

typedef struct stp_tp{
    pthread_mutex_t lock; /*Mutex variable*/
    pthread_cond_t working; /*Cond variable*/
    pthread_cond_t done; /*Cond variable*/ 
    pthread_t *threads; /*Actual threads*/
    stp_queue_t *pq; /*Priority queue*/
    long total_threads; /*Total threads used*/
}stp_tp_t;


stp_tp_t *stp_tp_create(uint16_t qsize_input){
    stp_tp_t *pool = (stp_tp_t *) malloc(sizeof(*pool));
    if (pool == NULL) return NULL;
    
    pool->pq = stp_pq_create(qsize_input);
    assert(pool->pq != NULL);

    pool->total_threads = GET_TOTSYS_THREAD;
    
    pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * pool->total_threads);

    if (pthread_mutex_init(&pool->lock, NULL) != 0 ||
        pthread_cond_init(&pool->working, NULL) != 0||
        pthread_cond_init(&pool->done, NULL) != 0){
        return NULL;
    }
    
    return pool;
}

void stp_tp_work(stp_tp_t *pool){
    if (pool == NULL) return;


}


int stp_tp_add(stp_tp_t *pool,void (*task)(void *), void *args,  uint16_t priority){
    
    if (task == NULL || args == NULL || pool == NULL){
        return -1;
    }


    pthread_mutex_lock(&pool->lock);

    PRINT_MSG("[DEBUG] Adding a task to the queue");

    if (pool->pq->tot_task == 0){
        pthread_cond_broadcast(&pool->working);
    }

    if (stp_pq_insert(pool->pq, task, args, priority) == STP_I_FAIL){
        return -1;
    }

    PRINT_INFO("[DEBUG] Inserted in the queue task with priority: ", priority);

    pthread_mutex_unlock(&pool->lock);

}

void stp_tp_wait(stp_tp_t *pool){
    if(pool == NULL) return;
    PRINT_MSG("[DEBUG] Waiting execution of all threads");

    pthread_mutex_lock(&pool->lock);
    while (pool->pq->tot_task > 0){
        pthread_cond_wait(&pool->done, &pool->lock);
    }
    pthread_mutex_unlock(&pool->lock);
    PRINT_MSG("[DEBUG] Tasks completed.");
}

int stp_tp_free(stp_tp_t **pool){
    if (pool == NULL || *pool == NULL) return -1;
    
    stp_tp_wait(*pool);

    (*pool)->total_threads = 0;
    stp_destroyq(&(*pool)->pq);
    free((*pool)->threads);
    (*pool)->threads = NULL;
    
    if (pthread_mutex_destroy(&(*pool)->lock) != 0)
        return threadpool_err_mutex;
    if (pthread_cond_destroy(&(*pool)->working) != 0)
        return threadpool_err_cond;
    if (pthread_cond_destroy(&(*pool)->done) != 0)
        return threadpool_err_cond;
    
    return 1;
}

