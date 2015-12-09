#ifndef TCP_H
#define TCP_H

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

/**
 * Returns whether a TCP header have the given
 * flag set.
 */
#define TCP_HAS_FLAG(header, type) \
  tcp_flag_word(header) & TCP_FLAG_##type

/**
 * \brief Creates a new TCP dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t*	tcp_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int       tcp_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void      tcp_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to TCP in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the TCP segment removed
 */
packet_t* tcp_dissector_decapsulate(const packet_t* packet);

#endif
