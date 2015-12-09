#ifndef _APP_H_
# define _APP_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#ifdef __GNUC__
#define _UNUSED_ __attribute__ ((unused))
#else
#define _UNUSED_
#endif

/**
 * \brief Exits the program by returning a
 * failure. Also dumps the human-readable
 * output of errno.                                                                                                                                                                                                                         
 */
void fail(const char* context);

/**
 * \return a human-readable string associated
 * with the current time. The returned pointer
 * points to a static value, so you must not try
 * to free it.
 */
const char* get_current_time();

#endif
