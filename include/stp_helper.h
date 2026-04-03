#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*This macro uses sysconf from the unistd.h library
 * the general purpose of sysconf is to get information at the runtime of a program
 * in this i'm using sysconf to get the number of processors configured in a 
 * 'x' operating system. For more information view the man page: https://man7.org/linux/man-pages/man3/sysconf.3.html*/

#define GET_TOTSYS_THREAD   (sysconf(_SC_NPROCESSORS_CONF))


#define GET_TOTCURR_THREAD  (sysconf(_SC_NPROCESSORS_ONLN))

/*A macro helper to check if the ptr is NULL
if it is we use the abort() function in the stdlib.h that will send
a SIGVABORT() with signal code 134*/

#define CHECK_ALLOC(ptr) do { \
    if ((ptr) == NULL){ \
        fprintf(stderr, \
            "[FATAL] %s:%d => ALLOCATION FAILED\n" \
            "[!]ERRNO CODE: %d\n" \
            "[!]ERROR: %s\n", \
            __FILE__ , __LINE__, \
            errno, strerror(errno)); \
        abort(); \
    }   \
}   while(0)
    




#endif /*HELPER_h*/ 
