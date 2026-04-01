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

#include "stp_threadpool.h"
#include "stp_queue.h"

typedef struct {
    pthread_mutex_t lock; /*Mutex variable*/
    pthread_cond_t cond_notify; /*Cond variable*/
    pthread_t *threads; /*Actual threads*/
    stp_queue_t *pq; /*Priority queue*/
    long total_threads; /*Total threads used*/
    uint16_t queue_size; /*Size of queue*/
}stp_tp_t;

//TODO: add macros flag for return value

stp_tp_t *stp_tp_create(uint16_t qsize_input){
    stp_tp_t *pool = malloc(sizeof(*pool));
    if (pool == NULL) return NULL;
    
    pool.pq = stp_pq_create(qsize_input);
    assert(pool.pq != NULL);

    pool.total_threads = GET_TOTSYS_THREAD;
    
    pool.threads = (pthread_t *) malloc(sizeof(pthread_t) * pool.total_threads);

    if (pthread_mutex_init(&pool.lock, NULL) != 0 ||
        pthread_cond_init(&pool.cond_notify, NULL) != 0){
        
        return NULL;
    }
    
    return pool;
}

int stp_tp_t stp_tp_add(void (*task)(void *), void *args){
    
    
}


