#ifndef _ETHERNET_H_
# define _ETHERNET_H_

#include "../../osi.h"
#include "../../termcaps.h"
#include "../../utils/inaddr.h"
#include <stdio.h>

#define ETHERNET_OUTPUT_HEADER \
  COLOR_BEGIN(BLUE,					    \
    "-------------------------------------------------------\n"		\
    "-----------------  " BOLD("Ethernet Header") "  -------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define ETHERNET_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

const osi_dissector_t*	ethernet_dissector_new();
int			ethernet_dissector_handles(const packet_t* packet);
void			ethernet_dissector_dump(const packet_t* packet);
packet_t*		ethernet_dissector_decapsulate(const packet_t* packet);

#endif
