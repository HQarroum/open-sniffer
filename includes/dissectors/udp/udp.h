#ifndef UDP_H
#define UDP_H

#include <stdio.h>
#include <linux/udp.h>
#include <osi.h>
#include <termcaps.h>
#include <app.h>

#define UDP_OUTPUT_HEADER \
  COLOR_BEGIN(RED,    \
    "-------------------------------------------------------\n"		\
    "---------------------  " BOLD("UDP Header") "  --------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define UDP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

/**
 * \brief Creates a new UDP dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t*	udp_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int       udp_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void      udp_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to UDP in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the UDP segment removed
 */
packet_t* udp_dissector_decapsulate(const packet_t* packet);

#endif
