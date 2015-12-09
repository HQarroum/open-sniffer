#ifndef HTTP_H
#define HTTP_H

#include "../../osi.h"
#include "../../termcaps.h"
#include "../../app.h"
#include "http-parser.h"
#include <stdio.h>

#define HTTP_OUTPUT_HEADER \
  COLOR_BEGIN(LIGHT_MAGENTA,			    \
    "-------------------------------------------------------\n"		\
    "--------------------  " BOLD("HTTP Header") "  --------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define HTTP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

typedef struct http_method_map
{
  uint8_t	id;
  const char*	value;
} http_method_map;

/**
 * \brief Creates a new HTTP dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t*	http_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int       http_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void      http_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to HTTP in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the HTTP segment removed
 * \note In this implementation, an HTTP packet is not decapsulated.
 */
packet_t* http_dissector_decapsulate(const packet_t* packet);

#endif
