#ifndef IP_H
#define IP_H

#include <stdio.h>
#include <netinet/ip.h>
#include <osi.h>
#include <termcaps.h>

#define IP_OUTPUT_HEADER \
  COLOR_BEGIN(GREEN,					    \
    "-------------------------------------------------------\n"		\
    "--------------------  " BOLD("IP Header") "  ----------------------\n" \
    "-------------------------------------------------------\n"		\
  )

#define IP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

/**
 * \brief Creates a new IP dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t*	ip_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int       ip_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void      ip_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to IP in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the IP segment removed
 */
packet_t* ip_dissector_decapsulate(const packet_t* packet);

#endif
