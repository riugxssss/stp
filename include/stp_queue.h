/* Priority queue using a -> Heap Binary
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


#ifndef QUEUE_H
#define QUEUE_H 

/*---- HEADERS SECTION ----*/

#include <stdint.h>


/*---- MACRO SECTION ----*/

/*--DEFAULT SIZE DEFINITIONS*/

/*I'll use bit shitfing operations to define macros
 * because it's commonly used in data structures size (e.g size-buffers and some impl in the Linux kernel)
 *
 * For those who don't know the use of defined: https://gcc.gnu.org/onlinedocs/cpp/Defined.html
 * We check if the default macro setted by the compiler (getted from the info about the compilation target)
 * exist, that means we are in modern systems (64 bit CPU)*/
 
#if defined(__x86_64__)
#   define DEFAULT_SIZE 65535 //uint16_t max number rappresentable
#elif defined(__arm__)
#   define DEFAULT_SIZE 256 * 16 //uint8_t * 16 = 4096
#else
#   define DEFAULT_SIZE 256 * 8 //uint8_t * 8 = 2048
#endif

/*--RETURN MACROS*/
#define STP_PEEK_FAIL 0x00

#define STP_I_FAIL 0x0f
#define STP_I_SUCC 0xf0

#define STP_R_FAIL 0x0e
#define STP_R_SUCC 0xe0

#define STP_MH_FAIL 0x0a
#define STP_MH_SUCC 0xa0

/*These macros defined below will help the insertion/deletion
 * from the actual queue, implementing a functional logic of a Heap Binary 
 * rappresented in (afaik) a (remember, only the logical/abstract view) 
 * Complete Binary Tree (https://runestone.academy/ns/books/published/pythonds3/Trees/BinaryHeapImplementation.html)
 * The practice implementation can be done in some ways, i prefer using an array and not pointers to rappresent a queue
 * For many reasons and the most important one is cache locality and cache hits, but in multithread programs the hardware
 * part is the most important, cache misses and cache coherence (MESI or MOESI for ARM) hardware protocol that affects
 * cores and the data fetching process.*/

#define GET_PARENT(idx)     (((idx) - 1) / 2)
#define GET_LEFT_C(idx)     ((2 * (idx)) + 1)
#define GET_RIGHT_C(idx)    ((2 * (idx)) + 2)

/*---- STRUCTURES ----*/
typedef struct stp_item {

    /*We use the void type meaning we can receive every type tasks 
     * and prepare it for a thread to wake and execute the scheduled (inserted in the pq)
     * job.*/
    void (*task)(void *);
    
    /*After the callback we define a pointer to void type, allowing 
     * the user to pass the parameter for the task*/
    void *args;

    /*A variable used to set the priority_task (it means when the task will be executed briefly)*/
    uint16_t priority_task; 

}stp_item_t;


/*A struct used to implement the priority queue for scheduled tasks*/

typedef struct stp_queue {
        
    /*A dynamic array that will hold all of our values to mantain a priority queue
     * with the propher Heap property in this project the property used is: min heap logic
     * The Min heap logic it basically want that the root hold the lowest priority value (in this case)
     * and the child of a parents need to be greater or equal (>=) the parent
     * altough, it breaks the min_heap logic
     * For more explanations i recommend: https://algs4.cs.princeton.edu/24pq/ 
     * That explains all of the concept even a mathematical view of it.*/
     struct stp_item *pq_arr;
    
     uint16_t queue_size; /*A 16 int variable to define the queue size*/

     uint16_t tot_task; /*An 16 int variable (0 - 65535) to count max 65536 items in the queue*/

}stp_queue_t;



/*---- QUEUE FUNCTIONS ----*/
int stp_check_property(stp_queue_t *pq);
stp_queue_t *stp_pq_create(uint16_t queue_size);
stp_item_t *stp_pq_peek(stp_queue_t *pq);
int stp_pq_insert(stp_queue_t *pq, void (*task)(void *), void *args, uint16_t priority);
int stp_pq_remove(stp_queue_t *pq);
void stp_pq_destroyq(stp_queue_t **pq);
int stp_pq_check_full(stp_queue_t *pq);

#ifdef __DEBUG_PQ
    void stp_pq_printq(stp_queue_t *pq);
#endif


#endif /*QUEUE_H*/
