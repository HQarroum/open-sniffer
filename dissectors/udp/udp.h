#ifndef _UDP_H_
# define _UDP_H_

#include "../../osi.h"
#include "../../termcaps.h"
#include "../../app.h"
#include <linux/udp.h>
#include <stdio.h>

#define UDP_OUTPUT_HEADER \
  COLOR_BEGIN(RED,    \
    "-------------------------------------------------------\n"		\
    "---------------------  " BOLD("UDP Header") "  --------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define UDP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

const osi_dissector_t*	udp_dissector_new();
int			udp_dissector_handles(const packet_t* packet);
void			udp_dissector_dump(const packet_t* packet);
packet_t*		udp_dissector_decapsulate(const packet_t* packet);

#endif
