#ifndef _TCP_H_
# define _TCP_H_

#include "../../osi.h"
#include "../../termcaps.h"
#include "../../app.h"
#include <linux/tcp.h>
#include <stdio.h>

#define TCP_OUTPUT_HEADER \
  COLOR_BEGIN(CYAN,						\
      "-------------------------------------------------------\n" \
      "---------------------  " BOLD("TCP Header") " ---------------------\n" \
      "-------------------------------------------------------\n" \
  )

#define TCP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

const osi_dissector_t*	tcp_dissector_new();
int			tcp_dissector_handles(const packet_t* packet);
void			tcp_dissector_dump(const packet_t* packet);
packet_t*		tcp_dissector_decapsulate(const packet_t* packet);

#endif
