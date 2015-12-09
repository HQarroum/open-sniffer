#include "app.h"

/**
 * @brief Exits the program by returning a
 * failure. Also dumps the human-readable
 * output of errno.
 */
void fail(const char* context)
{
  perror(context);
  exit(EXIT_FAILURE);
}

/**
 * @return a human-readable string associated
 * with the current time. The returned pointer
 * points to a static value, so you must not try
 * to free it.
 */
const char* get_current_time()
{
  time_t      rawtime;
  struct tm*  timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  return asctime(timeinfo);
}
