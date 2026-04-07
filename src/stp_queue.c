/* Priority queue using a -> Heap Binary
 *
 * Copyright (C) 2026 R riugxss riugxs@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../include/stp_queue.h"
#include "../include/stp_helper.h"

//Function declaration
int stp_pq_check_property(stp_queue_t *pq);


/*A function that initialize the struct and allocate
 * memory for the struct and the pq_arr*/

stp_queue_t *stp_pq_create(uint16_t queue_size){

    stp_queue_t *pq = malloc(sizeof(*pq));
    CHECK_ALLOC(pq);

    pq->tot_task = 0; //Init to 0 the variable
    
    if (queue_size == 0){
        pq->pq_arr = malloc(sizeof(*pq->pq_arr) * DEFAULT_SIZE);
        pq->queue_size = DEFAULT_SIZE;
    }else {
        pq->pq_arr = malloc(sizeof(*pq->pq_arr) * queue_size);
        pq->queue_size = queue_size;
    }

    CHECK_ALLOC(pq->pq_arr);

    return pq;
}

/*This function simply get the struct with the highest priority
 * in the queue, it means we return the root of the struct*/

stp_item_t *stp_pq_peek(stp_queue_t *pq){
    if (!pq || !pq->pq_arr || pq->tot_task == 0)
        return STP_PEEK_FAIL;
    return &pq->pq_arr[0];
}

/*A simple swap function used for swapping
 * priority task value in the queue*/

static inline void stp_pq_swap(stp_item_t *ptr1, stp_item_t *ptr2){
    stp_item_t tmp_pr = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp_pr;
}

/*The STP insert function it is a from theory implementation i made, for the implementation
 * in general of the priority queue it comes from only theory, i never went through some 
 * implementation, that's why it is not perfect
 *
 * Explanation of the function:
 *
 * The insert function first check if hte priority doesn't violate the LIMITED_SIZE
 * that is >>> pow(2, 15) = 32768
 * 
 * The first case is if we don't have tasks, in that case we simply put the task as root
 *
 * pq->pq_arr[0]. ... =
 *
 * and return STP_I_SUCC
 *
 * STP_I_SUCC and STP_I_FAIL are macros used to custom return value from the functions used 
 * for function debug mostly.
 *
 * If the tot_task > 0 the function put another task in the queue and only after
 * doing the insertion, it checks if the queue respect the Heap Property of Minimum Heap
 * (The macro GET_PARENT only get the parent of a 'node' you can find the explanation in the header corrispective file)
 *
 * E.g Queue
 *
 *   root(priority = 2)
 *l(pr = 4)      r(pr = 5)
 *
 *the property tell that the parent is always <= childs
 *2 < 4? y 
 *2 < 5? y
 *
 *This case respect the property.
 *
 *
 *E.g (n2) Queue
 *
 *   root(priority = 2)
 *l(pr = 4)      r(pr = 1)
 *
 *2 < 4? y
 *2 < 1? n
 *
 * This did not respect so needs relocation, we take the min value in this case 1 and do a node swap
 * swapping the entire struct, the priority and the task you can find the way below
 * */

int stp_pq_insert(stp_queue_t *pq, void (*task)(void *), void *args, uint16_t priority){

    /*We check if the task really exist*/
    if (task == NULL){
        return STP_I_FAIL;
    }


    //First case where the total task = to 0
    if (pq->tot_task == 0){
        pq->tot_task++;
        pq->pq_arr[0].task = task;
        pq->pq_arr[0].priority_task = priority;
        pq->pq_arr[0].args = args;
        return STP_I_SUCC;
    }

    //If the first case condition fail, puts the paramers in the struct (Possible property violation)
    pq->pq_arr[pq->tot_task].task = task;
    pq->pq_arr[pq->tot_task].priority_task = priority;
    pq->pq_arr[pq->tot_task].args = args;
    pq->tot_task++;


    int tmptot = pq->tot_task - 1;
    int parent_idx;

    //We fast check if the property is valid, if it is no need to continue  
    if (stp_pq_check_property(pq) == STP_MH_SUCC){
        return STP_I_SUCC;
    }

    /*The fondamental section of this function. The possible property violation will be 
     * resolved in thi section, getting the parent index and checking if the property its valid
     * if not, totally swap.*/

    do {
        parent_idx = GET_PARENT(tmptot);

        if (pq->pq_arr[parent_idx].priority_task <= pq->pq_arr[tmptot].priority_task){
            break;
        }else {
            /*We change the array layout swapping 2 memory cells
             * in the swap function we manage it using pointers but in reality
             * we simply copy the entire struct
             *
             * E.g
             * struct 1 idx 0
             * struct 2 idx 1
             * swap(), we put struct 1 in a tmp struct:
             *
             * struct ... tmp = *ptrtocell C allows this and the name of this
             * operation is: shallow copy
             *
             * and we do the operation swap like in the function.*/
            stp_pq_swap(
                    &pq->pq_arr[parent_idx],
                    &pq->pq_arr[tmptot]
                    );
        }

        tmptot = parent_idx;

    }while (tmptot != 0);
    
    //All the insertion goes well and the queue respect the min heap property.
    return STP_I_SUCC;
}

/*The removing function will do the classical removal in a priority queue
 * so it will always remove from the root, it is called heapify down, it will take the last element
 * in the queue and put it root
 *
 * E.g
 *    root(pr 1)
 *   l(2)     r(3)
 * After
 *    root(pr 3)
 *   l(2)    NULL
 *
 * Of course that will lead to a violation of the min heap property, so we do the Heapify down.
 *
 * The Heapify down it consist in take the children with the lowest priority and do the swap (in this case will be only 2)
 *  
 * E.g (n2)
 *
 *  root(pr 2)
 * l(pr 3) NULL
 *
 * The heapify down is concluded when the smallest == start_idx
 *
 * the start idx is a integer variable (32 bit in modern systems) that is used to take the start father
 * of every cycle, it means we logic divide the array, ((O(log(n)) operational time)) because we treat the array
 * like a tree allowing to reduce significantly the time.
 *
 * */

int stp_pq_remove(stp_queue_t *pq){
    if (pq == NULL || pq->tot_task == 0)
        return STP_R_FAIL;
    
    pq->pq_arr[0] = pq->pq_arr[pq->tot_task - 1];
    pq->tot_task--;

    int 
    start_idx = 0,
    left,
    right,
    smallest,
    tmptot = pq->tot_task;

    while (1){
        left  = GET_LEFT_C(start_idx);
        right = GET_RIGHT_C(start_idx);
        smallest = start_idx;
    
        if (left < tmptot && pq->pq_arr[left].priority_task < pq->pq_arr[smallest].priority_task)
            smallest = left;
        if(right < tmptot && pq->pq_arr[right].priority_task < pq->pq_arr[smallest].priority_task)
            smallest = right;
        
        if (smallest == start_idx) break;

        stp_pq_swap(&pq->pq_arr[start_idx], &pq->pq_arr[smallest]);
        start_idx = smallest;
    }
    
    return STP_R_SUCC;
}

/*The destroy function of the queue it will be called when the 
 * thread pool ends*/

void stp_pq_destroyq(stp_queue_t **pq){
    /*if we pass in the free function a NULL value
     * it will work fine -> https://it.cppreference.com/w/c/memory/c/free*/
    
    if (pq == NULL || *pq == NULL || (*pq)->tot_task == 0)
        return; 

    free((*pq)->pq_arr);
    (*pq)->pq_arr = NULL;
    free(*pq);
    *pq = NULL;
}

/*This function is used to check if a queue is already valid
 * if it is -> return MH_SUCC
 * if its not -> return MH_FAIL
 *
 * */
int stp_pq_check_property(stp_queue_t *pq){
    if (pq == NULL || pq->tot_task == 0)
        return STP_MH_FAIL;
    for (int k = 0;k < pq->tot_task; k++){
        int left  =  GET_LEFT_C(k);
        int right =  GET_RIGHT_C(k);

        if (left < pq->tot_task && pq->pq_arr[k].priority_task > pq->pq_arr[left].priority_task)
            return STP_MH_FAIL;
        if (right < pq->tot_task && pq->pq_arr[k].priority_task > pq->pq_arr[right].priority_task)
            return STP_MH_FAIL;
    }
    
    return STP_MH_SUCC;
}


/*Only used if the __QUEUEDEB__ macro is defined
 * it print the entire array and allows to see the queue*/


#ifdef __QUEUEDEB__

void stp_pq_printq(stp_queue_t *pq){
    if (pq == NULL || pq->pq_arr == NULL)
        return;

    printf("START OF THE PRIORITY QUEUE\n");
    
    for (int i = 0; i < pq->tot_task;i++){
        printf("[%d] %d\n", i , pq->pq_arr[i]);
    }
}

int stp_pq_check_full(stp_queue_t *pq){
    return pq->queue_size == pq->tot_task;
}

#endif
