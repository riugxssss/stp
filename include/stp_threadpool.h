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

/*This macro uses sysconf from the unistd.h library
 * the general purpose of sysconf is to get information at the runtime of a program
 * in this i'm using sysconf to get the number of processors configured in a
 * 'x' operating system. For more information view the man page: https://man7.org/linux/man-pages/man3/sysconf.3.html*/

#define GET_TOTSYS_THREAD (sysconf(_SC_NPROCESSORS_CONF))

#define IMMEDIATE_SHUTDOWN 10
#define GRACEFUL_SHUTDOWN  11

typedef struct threadpool_t;




#endif /*__THREADPOOL__*/
