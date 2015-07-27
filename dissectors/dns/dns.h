#ifndef _DNS_H_
# define _DNS_H_

#include "../../osi.h"
#include "../../termcaps.h"
#include "../../utils/inaddr.h"
#include <stdio.h>
#include <inttypes.h>

// DNS structures and macros.
#include </usr/include/arpa/nameser.h>

// The maximum size allocated to a domain name.
#define MAX_NAME_LENGTH			256

// DNS packet types.
#define DNS_PACKET_TYPE_QUERY		"QUERY"
#define DNS_PACKET_TYPE_RESPONSE	"RESPONSE"

#define DNS_OUTPUT_HEADER \
  COLOR_BEGIN(LIGHT_BLUE, \
    "-------------------------------------------------------\n"		\
    "--------------------  " BOLD("DNS Header") "  ---------------------\n" \
    "-------------------------------------------------------\n" \
  )

#define DNS_OUTPUT_FOOTER \
  "-------------------------------------------------------\n"	\
  COLOR_END


typedef struct dnshdr {
  unsigned        id :16;         /*%< query identification number */
#if BYTE_ORDER == BIG_ENDIAN
  /* fields in third byte */
  unsigned        qr: 1;          /*%< response flag */
  unsigned        opcode: 4;      /*%< purpose of message */
  unsigned        aa: 1;          /*%< authoritive answer */
  unsigned        tc: 1;          /*%< truncated message */
  unsigned        rd: 1;          /*%< recursion desired */
  /* fields in fourth byte */
  unsigned        ra: 1;          /*%< recursion available */
  unsigned        unused :1;      /*%< unused bits (MBZ as of 4.9.3a3) */
  unsigned        ad: 1;          /*%< authentic data from named */
  unsigned        cd: 1;          /*%< checking disabled by resolver */
  unsigned        rcode :4;       /*%< response code */
#elif BYTE_ORDER == LITTLE_ENDIAN || BYTE_ORDER == PDP_ENDIAN
  /* fields in third byte */
  unsigned        rd :1;          /*%< recursion desired */
  unsigned        tc :1;          /*%< truncated message */
  unsigned        aa :1;          /*%< authoritive answer */
  unsigned        opcode :4;      /*%< purpose of message */
  unsigned        qr :1;          /*%< response flag */
  /* fields in fourth byte */
  unsigned        rcode :4;       /*%< response code */
  unsigned        cd: 1;          /*%< checking disabled by resolver */
  unsigned        ad: 1;          /*%< authentic data from named */
  unsigned        unused :1;      /*%< unused bits (MBZ as of 4.9.3a3) */
  unsigned        ra :1;          /*%< recursion available */
#endif
  /* remaining bytes */
  unsigned        qcount :16;    /*%< number of question entries */
  unsigned        ancount :16;    /*%< number of answer entries */
  unsigned        nscount :16;    /*%< number of authority entries */
  unsigned        arcount :16;    /*%< number of resource entries */
} dnshdr;

/**
 * \brief A mapping between a DNS packet type
 * and its human-readable representation.
 */
typedef struct dns_type_map
{
  ns_type type;
  char*   value;
} dns_type_map;

/**
 * Definition of a DNS question.
 */
typedef struct dns_query {
  uint16_t type;
  uint16_t class;
} __attribute__ ((__packed__)) dns_query;

/**
 * A DNS resource record as sent back
 * by a remote DNS server in a response.
 */
typedef struct	dns_resource_record {
  dns_query	query;
  uint32_t	ttl;
  uint16_t	len;
} __attribute__ ((__packed__)) dns_resource_record;

/**
 * \brief Creates a new DNS dissector.
 */
const osi_dissector_t*		dns_dissector_new();

/**
 * \brief States whether the DNS dissector can handle
 * the given packet.
 */
int				dns_dissector_handles(const packet_t* packet);
void				dns_dissector_dump(const packet_t* packet);
packet_t*			dns_dissector_decapsulate(const packet_t* packet);

#endif
