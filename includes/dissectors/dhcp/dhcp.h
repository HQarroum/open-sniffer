#ifndef DHCP_H
#define DHCP_H

#include <stdio.h>
#include <stdint.h>
#include <osi.h>
#include <termcaps.h>
#include <utils/inaddr.h>

#define DHCP_OUTPUT_HEADER \
  COLOR_BEGIN(GREEN,					    \
    "-------------------------------------------------------\n"		\
    "-------------------  " BOLD("DHCP Header") " ----------------------\n" \
    "-------------------------------------------------------\n"		\
  )

#define DHCP_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END

#define DHCP_UNKNOWN_HW_TYPE "Unknown"

/**
 * \brief The DHCP header definition;
 */
typedef struct dhcphdr
{
  uint8_t  op;
  uint8_t  htype;
  uint8_t  hlen;
  uint8_t  hops;
  uint32_t xid;
  uint16_t secs;
  uint16_t flags;
  uint32_t client_addr;
  uint32_t your_addr;
  uint32_t server_addr;
  uint32_t gateway_addr;
  uint8_t  client_hw_addr[16];
  uint8_t  server_name[64];
} __attribute__ ((__packed__)) dhcphdr;

/**
 * \brief A mapping between a DHCP harware type
 * and its human-readable representation.
 */
typedef struct dhcp_hw_type_map
{
  uint8_t type;
  char*   value;
} dhcp_hw_type_map;

/**
 * \brief Creates a new DHCP dissector.
 * \return an instance of a dissector
 */
const osi_dissector_t*	dhcp_dissector_new();

/**
 * \return whether the dissector can handle the
 * given packet.
 * \see osi_dissector_t
 */
int       dhcp_dissector_handles(const packet_t* packet);

/**
 * \brief Dumps the given packet on the standard output.
 * \see osi_dissector_t
 */
void      dhcp_dissector_dump(const packet_t* packet);

/**
 * \brief Removes the bytes related to DHCP in the given packet.
 * \return a pointer to a packet with every bytes related to
 * the IP segment removed
 * \note In this implementation, the DHCP dissector will return
 * NULL.
 */
packet_t* dhcp_dissector_decapsulate(const packet_t* packet);

#endif
