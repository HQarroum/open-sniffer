#ifndef ARP_H
#define ARP_H

#include <stdio.h>
#include <osi.h>
#include <termcaps.h>
#include <app.h>
#include <utils/inaddr.h>
#include <netinet/if_ether.h>

#define ARP_OUTPUT_HEADER \
  COLOR_BEGIN(YELLOW,					    \
    "-------------------------------------------------------\n" \
    "-------------------  " BOLD("ARP Header") "  ----------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define ARP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

/**
 * \brief Creates a new ARP dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t* arp_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int  arp_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void arp_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to ARP in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the ARP segment removed
 */
packet_t* arp_dissector_decapsulate(const packet_t* packet);

#endif
