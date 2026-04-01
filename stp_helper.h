#ifndef HELPER_H
#define HELPER_H

/*This macro uses sysconf from the unistd.h library
 * the general purpose of sysconf is to get information at the runtime of a program
 * in this i'm using sysconf to get the number of processors configured in a 
 * 'x' operating system. For more information view the man page: https://man7.org/linux/man-pages/man3/sysconf.3.html*/

#define GET_TOTSYS_THREAD (sysconf(_SC_NPROCESSORS_CONF))



#endif /*HELPER_h*/ 
