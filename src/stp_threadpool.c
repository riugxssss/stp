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
#ifdef _DEBUG
# define PRINT_MSG(x)           fprintf(stdout, "%s\n", x )
# define PRINT_INFO(f1, f2)     fprintf(stdout, "%s | %d\n", f1, f2)
#define __DEBUG_PQ
#else
#define PRINT_MSG(x)            ((void)0)
#define PRINT_INFO(f1, f2)      ((void)0)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <stdatomic.h>

#include "../include/stp_threadpool.h"
#include "../include/stp_queue.h"
#include "../include/stp_helper.h"


/*The Thread Pool struct with the total_threads that will
indicate the max threads. Two condition variable and one mutex
Used to lock and notify, corrispectively a mutex is used in critical
sections and the cond var it will be used to notify the a job is in the 
priority queue and one when all job are done*/

typedef struct stp_tp{
    pthread_mutex_t lock; /*Mutex variable*/
    pthread_cond_t working; /*Cond variable*/
    pthread_cond_t done; /*Cond variable*/ 
    pthread_t *threads; /*Actual threads*/
    stp_queue_t *pq; /*Priority queue*/
    long total_threads; /*Total threads used*/
}stp_tp_t;

/*
@function name stp_tp_work

The usage of this function is to execute
a scheduled task in the queue*/

static void *stp_tp_work(void *arg){

    stp_tp_t *pool = (stp_tp_t*)arg;

    while (1){
    pthread_mutex_lock(&pool->lock);

    while (pool->pq->tot_task == 0){
        pthread_cond_wait(&pool->working, &pool->lock);
    }

    stp_item_t *ptr = stp_pq_peek(pool->pq);
    stp_pq_remove(pool->pq);

    if (pool->pq->tot_task == 0){
        pthread_cond_signal(&pool->done);
    }

    pthread_mutex_unlock(&pool->lock);

    ptr->task(ptr->args);
    }
    return NULL;
}

/*
@function name: stp_create_pool

The purpose of this function is to initialize a thread pool with parameters
passed by input to make it flexible and custom
*/

stp_tp_t *stp_create_pool(int total_threads, uint16_t queue_size){

    static atomic_int called = 0;
    static stp_tp_t *singleton_pool = NULL;

    /*From  https://en.cppreference.com/w/c/atomic/atomic_load.html
    Atomically loads and returns the current value of the atomic variable pointed to by obj. 
    The operation is atomic read operation.

    The purpose of doing that is to create only one thread pool at a time
    */
   
    if (atomic_load(&called)){
        return singleton_pool;
    }

    //Allocating memory for the struct
    stp_tp_t *pool = (stp_tp_t *) malloc(sizeof(*pool));
    CHECK_ALLOC(pool);

    //Assigning total threads section
    
    int systot_threads = (int) GET_TOTSYS_THREAD;

    //Setting a limit
    systot_threads *= 2;
    
    /*If the input is greater than the limit
    we set a proper default value*/
    if (total_threads > systot_threads){
        #ifdef _DEBUG
        #warning "[!] The input variable for the threads can be inefficient"
        #endif
        PRINT_INFO("[!] Changed total threads in ", systot_threads);
        total_threads = systot_threads;
    }

    /*Initializating the thread pool*/

    //Tot threads
    pool->total_threads = total_threads;

    //Creating the priority queue
    pool->pq = stp_pq_create(queue_size);

    //Allocating space for threads
    pool->threads = (pthread_t *) malloc(sizeof(pthread_t *) * total_threads);
    CHECK_ALLOC(pool->threads);

    //Creating threads
    for (int i = 0; i < total_threads;i++){
        pthread_create(&pool->threads[i], NULL, stp_tp_work, pool);
    }

    //Allocating the syncro variables
    if (pthread_mutex_init(&pool->lock, NULL) != 0)
        return (void*)STP_FAIL;
    if (pthread_cond_init(&pool->done, NULL) != 0 ||
        pthread_cond_init(&pool->working, NULL) != 0){
            return (void*)STP_FAIL;
    }

    /*From https://en.cppreference.com/w/c/atomic/atomic_store.html
    Atomically replaces the value of the atomic variable pointed to by obj with desired.
    The operation is atomic write operation.

    The purpose of doing that is to set the obj (called) value, to the desired (1)
    */
    atomic_store(&called, 1);
    singleton_pool = pool;


    return pool;
}

/*
@functio name: stp_tp_add

The purpose of this function is to add a task that a thread will execute
*/

int stp_tp_add(stp_tp_t *pool, void (*task)(void *), void *args, uint16_t priority){

    if (pool == NULL) return STP_FAIL;

    //Crit section
    pthread_mutex_lock(&pool->lock);

    /*Inserting in the priority queue and waking the threads on the
    condition variable to execute scheduled jobs*/
    if (stp_pq_insert(pool->pq, task, args, priority) == STP_I_SUCC){
        pthread_cond_broadcast(&pool->working);
    }

    pthread_mutex_unlock(&pool->lock);
    return STP_SUCC;
}

/*
@function name: stp_tp_wait

The purpose of this function is to wait until all threads are completed*/

int stp_tp_wait(stp_tp_t *pool){
    if (pool == NULL) return STP_FAIL;

    pthread_mutex_lock(&pool->lock);

    while (pool->pq->tot_task > 0){
        pthread_cond_wait(&pool->done, &pool->lock);
    }

    PRINT_MSG("[!] All threads finished to execute the task");

    pthread_mutex_unlock(&pool->lock);
    return STP_SUCC;
}

/*
@function name: stp_tp_free

The usage of this function is to free the entire thread pool*/

void stp_tp_free(stp_tp_t **pool){
    if (pool == NULL || *pool == NULL)
        return;
    
    stp_tp_wait(*pool);

    PRINT_MSG("FREEING ALL");

    for (int i = 0;i < (*pool)->total_threads;i++){
        pthread_cancel((*pool)->threads[i]);
        pthread_join((*pool)->threads[i], NULL);
    }

    /*Setting total threads field to 0*/
    (*pool)->total_threads = 0;
    
    /*Calling internal function to destroy the queue*/
    stp_pq_destroyq(&(*pool)->pq);

    /*Setting the threads block allocated to free (for reuse)*/
    free((*pool)->threads);
    (*pool)->threads = NULL; //Dangling pointer

    /*Destroying the mutex*/
    if (pthread_mutex_destroy(&(*pool)->lock) != 0){
        return;
    }

    /*Destroying the condition variables*/
    if (
        pthread_cond_destroy(&(*pool)->done) != 0 ||
        pthread_cond_destroy(&(*pool)->working) != 0
    ){
        return;
    }

    /*Deallocating the main pointer*/
    free(*pool);
    *pool = NULL;
}

