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

#define IMM_MODE 0x0f
#define GRC_MODE 0xff


typedef struct stp_tp stp_tp_t;

stp_tp_t *stp_create_pool(int total_threads, uint16_t queue_size);
int stp_tp_add(stp_tp_t *pool, void (*task)(void *), void *args, uint16_t priority);
int stp_tp_wait(stp_tp_t *pool);
void stp_tp_free(stp_tp_t **pool, uint8_t mode);
void stp_tp_start(stp_tp_t *pool);


#endif /*__THREADPOOL__*/
