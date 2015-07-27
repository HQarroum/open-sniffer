#ifndef _ARP_H_
# define _ARP_H_

#include "../../osi.h"
#include "../../termcaps.h"
#include "../../app.h"
#include "../../utils/inaddr.h"
#include <netinet/if_ether.h>
#include <stdio.h>

#define ARP_OUTPUT_HEADER \
  COLOR_BEGIN(YELLOW,					    \
    "-------------------------------------------------------\n" \
    "-------------------  " BOLD("ARP Header") "  ----------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define ARP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

const osi_dissector_t*	arp_dissector_new();
int			arp_dissector_handles(const packet_t* packet);
void			arp_dissector_dump(const packet_t* packet);
packet_t*		arp_dissector_decapsulate(const packet_t* packet);

#endif
