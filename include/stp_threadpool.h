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

#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <stdint.h>

#define STP_FAIL 0x0
#define STP_SUCC 0x1

/*If the DEBUG is defined it will print info msgs 
of the STDOUT stream (your terminal)*/

#ifdef _DEBUG
# define PRINT_MSG(x)           fprintf(stdout, "%s\n", x )
# define PRINT_INFO(f1, f2)     fprintf(stdout, "%s | %d\n", f1, f2)
#define __DEBUG_PQ
#else
#define PRINT_MSG(x)            ((void)0)
#define PRINT_INFO(f1, f2)      ((void)0)
#endif

typedef struct stp_tp stp_tp_t;

stp_tp_t *stp_create_pool(int total_threads, uint16_t queue_size);
int stp_tp_add(stp_tp_t *pool, void (*task)(void *), void *args, uint16_t priority);
int stp_tp_wait(stp_tp_t *pool);
void stp_tp_free(stp_tp_t **pool);

#endif /*__THREADPOOL__*/
