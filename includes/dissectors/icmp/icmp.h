#ifndef ICMP_H
#define ICMP_H

#include <linux/icmp.h>
#include <stdio.h>
#include <osi.h>
#include <termcaps.h>
#include <app.h>

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

/**
 * \brief Creates a new ICMP dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t* icmp_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int       icmp_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void      icmp_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to ICMP in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the ICMP segment removed
 */
packet_t* icmp_dissector_decapsulate(const packet_t* packet);

#endif
