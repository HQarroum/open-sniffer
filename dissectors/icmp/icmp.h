#ifndef _ICMP_H_
# define _ICMP_H_

#include "../../osi.h"
#include "../../termcaps.h"
#include "../../app.h"
#include <linux/icmp.h>
#include <stdio.h>

#define ICMP_TYPE_UNKNOWN "ICMP_TYPE_UNKNOWN"

#define ICMP_OUTPUT_HEADER \
  COLOR_BEGIN(MAGENTA,							\
    "-------------------------------------------------------\n"		\
    "--------------------  " BOLD("ICMP Header") "  ---------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define ICMP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

typedef struct icmp_dissector_packet_types_t
{
  // The ICMP packet type code.
  __u8        type_code;
  
  // The human-readable version of the
  // ICMP packet type code.
  const char* type_name;
} icmp_dissector_packet_types_t;

const osi_dissector_t*	icmp_dissector_new();
int			icmp_dissector_handles(const packet_t* packet);
void			icmp_dissector_dump(const packet_t* packet);
packet_t*		icmp_dissector_decapsulate(const packet_t* packet);

#endif
