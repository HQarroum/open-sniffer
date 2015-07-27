#ifndef _HTTP_H_
# define _HTTP_H_

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

const osi_dissector_t*	http_dissector_new();
int			http_dissector_handles(const packet_t* packet);
void			http_dissector_dump(const packet_t* packet);
packet_t*		http_dissector_decapsulate(const packet_t* packet);

#endif
