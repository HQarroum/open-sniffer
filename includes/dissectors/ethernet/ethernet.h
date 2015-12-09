#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdio.h>
#include <osi.h>
#include <termcaps.h>
#include <utils/inaddr.h>

#define ETHERNET_OUTPUT_HEADER \
  COLOR_BEGIN(BLUE,					    \
    "-------------------------------------------------------\n"		\
    "-----------------  " BOLD("Ethernet Header") "  -------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define ETHERNET_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

/**
 * \brief Creates a new Ethernet dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t* ethernet_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int       ethernet_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void      ethernet_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to Ethernet in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the Ethernet segment removed
 */
packet_t* ethernet_dissector_decapsulate(const packet_t* packet);

#endif
