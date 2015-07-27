#ifndef _IP_H_
# define _IP_H_

#include "../../osi.h"
#include "../../termcaps.h"
#include <stdio.h>
#include <netinet/ip.h>

#define IP_OUTPUT_HEADER \
  COLOR_BEGIN(GREEN,					    \
    "-------------------------------------------------------\n"		\
    "--------------------  " BOLD("IP Header") "  ----------------------\n" \
    "-------------------------------------------------------\n"		\
  )

#define IP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

const osi_dissector_t*	ip_dissector_new();
int			ip_dissector_handles(const packet_t* packet);
void			ip_dissector_dump(const packet_t* packet);
packet_t*		ip_dissector_decapsulate(const packet_t* packet);

#endif
