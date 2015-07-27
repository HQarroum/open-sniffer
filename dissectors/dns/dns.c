#include "dns.h"

static const osi_dissector_t dns = {
  .layer         = OSI_LAYER_7,
  .name          = "dns",
  .operations    = {
    .handles     = &dns_dissector_handles,
    .dump        = &dns_dissector_dump,
    .decapsulate = &dns_dissector_decapsulate
  }
};

/**
 * \brief A mapping between a DNS packet type
 * and its human-readable representation.
 */
static const dns_type_map ns_types[] = {
  { ns_t_invalid,	"Invalid" },
  { ns_t_a,		"A" },
  { ns_t_ns,		"NS" },
  { ns_t_md,		"MD" },
  { ns_t_mf,		"MF" },
  { ns_t_cname,		"CNAME" },
  { ns_t_soa,		"SOA" },
  { ns_t_mb,		"MB" }, 
  { ns_t_mg,		"MG" },
  { ns_t_mr,		"MR" },
  { ns_t_null,		"NULL" },
  { ns_t_wks,		"WKS" },
  { ns_t_ptr,		"PTR" },
  { ns_t_hinfo,		"HINFO" },
  { ns_t_minfo,		"MINFO" },
  { ns_t_mx,		"MX" },
  { ns_t_txt,		"TXT" },
  { ns_t_rp,		"RP" }, 
  { ns_t_afsdb,		"AFSDB" },
  { ns_t_x25,		"X25" },
  { ns_t_isdn,		"ISDN" },
  { ns_t_rt,		"RT" },
  { ns_t_nsap,		"NSAP" },
  { ns_t_nsap_ptr,	"NSAP_PTR" },
  { ns_t_sig,		"SIG" },
  { ns_t_key,		"KEY" },
  { ns_t_px,		"PX" },
  { ns_t_gpos,		"GPOS" },
  { ns_t_aaaa,		"AAAA" },
  { ns_t_loc,		"LOC" },
  { ns_t_nxt,		"NXT" },
  { ns_t_eid,		"EID" },
  { ns_t_nimloc,	"NIMLOC" },
  { ns_t_srv,		"SRV" },
  { ns_t_atma,		"ATMA" },
  { ns_t_naptr,		"NAPTR" },
  { ns_t_kx,		"KX" },
  { ns_t_cert,		"CERT" },
  { ns_t_a6,		"A6" },
  { ns_t_dname,		"DNAME" },
  { ns_t_sink,		"SINK" },
  { ns_t_opt,		"OPT" },
  { ns_t_apl,		"APL" },
  { ns_t_tkey,		"TKEY" },
  { ns_t_tsig,		"TSIG" },
  { ns_t_ixfr,		"IXFR" },
  { ns_t_axfr,		"AXFR" },
  { ns_t_mailb,		"MAILB" },
  { ns_t_maila,		"MAILA" },
  { ns_t_any,		"ANY" },
  { ns_t_zxfr,		"ZXFR" },
  { ns_t_max,		"MAX" }
};

/**
 * \brief Creates a new instance of an DNS
 * dissector.
 */
const osi_dissector_t* dns_dissector_new()
{
  return (&dns);
}

static const char* dns_dissector_type_to_string(ns_type type)
{
  for (size_t i = 0; i < sizeof(ns_types) / sizeof(*ns_types); ++i) {
    if (ns_types[i].type == type) {
      return (ns_types[i].value);
    }
  }
  return (ns_types[0].value);
}

static int dn_expand(const unsigned char *base, const unsigned char *end, const unsigned char *src, char *dest, int space)
{
  const unsigned char *p = src;
  char *dend, *dbegin = dest;
  int len = -1, i, j;
  
  if (p==end || space <= 0) return -1;
  dend = dest + (space > 254 ? 254 : space);
  /* detect reference loop using an iteration counter */
  for (i=0; i < end-base; i+=2) {
    /* loop invariants: p<end, dest<dend */
    if (*p & 0xc0) {
      if (p+1==end) return -1;
      j = ((p[0] & 0x3f) << 8) | p[1];
      if (len < 0) len = p+2-src;
      if (j >= end-base) return -1;
      p = base+j;
    } else if (*p) {
      if (dest != dbegin) *dest++ = '.';
      j = *p++;
      if (j >= end-p || j >= dend-dest) return -1;
      while (j--) *dest++ = *p++;
    } else {
      *dest = 0;
      if (len < 0) len = p+1-src;
      return len;
    }
  }
  return -1;
}

/**
 * @brief Returns the human-readable representation of
 * the given IPv4 address and moves the pointer 4 bytes
 * further.
 */
static const char* _a_rr(uint8_t** ptr)
{
  struct in_addr in;

  NS_GET32(in.s_addr, *ptr);
  in.s_addr = ntohl(in.s_addr);
  return inet_ntoa(in);
}

/**
 * @brief Returns the human-readable representation of
 * the given IPv6 address and moves the pointer 16 bytes
 * further.
 */
static void* _aaaa_rr(uint8_t** data)
{
  static char		addr[INET6_ADDRSTRLEN];
  struct sockaddr_in6	sa6;
  
  memset(&sa6, 0, sizeof(sa6));
  sa6.sin6_family = AF_INET6;
#ifdef SIN6_LEN
  sa6.sin6_len = sizeof(sa6);
#endif
  
  for (size_t i = 0; i < 16; i++) {
    sa6.sin6_addr.s6_addr[i] = (*data)[i];
  }
  
  j_inet_ntop((struct sockaddr_storage *)&sa6, addr, sizeof(addr));
  *data = *data + 16;
  return (addr);
}

/**
 * @brief Returns the string associated with the given
 * data buffer and moves the pointer x bytes further,
 * x being the length of the string.
 */
static const char*  _cname_rr(const uint8_t* base, const uint8_t* end, uint8_t** data)
{
  static char	ns_name[MAX_NAME_LENGTH];
  int		size_of_name;
  
  memset(ns_name, 0, sizeof(ns_name));
  if ((size_of_name = dn_expand(base, end, *data, ns_name, sizeof(ns_name))) != -1) {
    *data += size_of_name;
    return (ns_name);
  }
  return (NULL);
}

/**
 * Returns whether the given DNS header is a request or a response.
 */
static const char* dns_dissector_type_of_packet(const dnshdr* packet)
{
  return (packet->qr ? DNS_PACKET_TYPE_RESPONSE : DNS_PACKET_TYPE_QUERY);
}

static uint32_t get_minimum_packet_size(const dnshdr* header)
{
  uint16_t questions	 = ntohs(header->qcount);
  uint16_t answers	 = ntohs(header->ancount);
  uint32_t required_size = 0;
  
  // Computing the minimum required size of the packet.
  required_size = sizeof(dnshdr) + (questions * sizeof(dns_query));
  if (header->qr) {
    // In the case of a response, we add the dns resource records
    // size for each received answer.
    required_size += (answers * sizeof(dns_resource_record));
  }
  return (required_size);
}

static void dns_dissector_dump_query_packet(const packet_t* packet)
{
  dnshdr* header	= (struct dnshdr*) packet->data;
  uint8_t* ptr_to_query	= (uint8_t*) (packet->data + sizeof(struct dnshdr));
  uint8_t* data		= ptr_to_query;
  uint8_t* end		= packet->data + packet->size;
  uint16_t questions	= ntohs(header->qcount);
  uint16_t type;
  
  printf("# of questions : %d\n", questions);
  for (size_t i = 0; i < questions && data < end; ++i) {
    char buffer[MAX_NAME_LENGTH];
    int  size;
    
    memset(buffer, 0, sizeof(buffer));
    if ((size = dn_expand(ptr_to_query, end, data, buffer, sizeof(buffer))) == -1) {
      // Invalid packet.
      return;
    }
    printf("Query : %s\n", buffer);
    data = data + size;
  }
  // Retrieving the type of the query.
  NS_GET16(type, data);
  printf("Query type : %s (%" PRIu16 ")\n", dns_dissector_type_to_string(type), type);
}

static void dns_dissector_dump_response_packet(const packet_t* packet)
{
  struct dnshdr* header = (struct dnshdr*) packet->data;
  uint8_t* ptr_to_query	= (uint8_t*) (packet->data + sizeof(struct dnshdr));
  uint8_t* data		= ptr_to_query;
  uint8_t* end		= packet->data + packet->size;
  uint16_t questions	= ntohs(header->qcount);
  uint16_t answers	= ntohs(header->ancount);
  uint16_t type, _UNUSED_ class, _UNUSED_ ttl, _UNUSED_ len;
  
  printf("Response code : %d\n", header->rcode);
  printf("# of questions : %d\n", questions);
  printf("# of answers : %d\n", answers);
  printf("# of authoritative servers : %d\n", ntohs(header->nscount));
  printf("# of additional records : %d\n", ntohs(header->arcount));
  
  // Printing the queries.
  for (size_t i = 0; i < questions && data < end; ++i) {
    char buffer[MAX_NAME_LENGTH];
    int size;
    
    memset(buffer, 0, sizeof(buffer));
    if ((size = dn_expand(ptr_to_query, end, data, buffer, sizeof(buffer))) == -1) {
      // Invalid packet.
      return;
    }
    printf("Query address : %s\n", buffer);
    data = data + size;
  }
  
  NS_GET16(type, data);
  NS_GET16(class, data);
  
  printf("Query type : %s (%" PRIu16 ")\n", dns_dissector_type_to_string(type), type);
  
  // Printing the responses.
  for (size_t i = 0; i < answers && data < end; ++i) {
    size_t size_of_name;
    
    // Skipping the domain name.
    size_of_name = strlen((const char*) data);
    data = data + size_of_name;
    
    NS_GET16(type, data);
    NS_GET16(class, data);
    NS_GET32(ttl, data);
    NS_GET16(len, data);
    
    printf(UNDERLINED("Response #%zu (%s)\n"),
	   (i + 1),
	   dns_dissector_type_to_string(type));
    switch (type) {
    case ns_t_a:
      printf("IPv4 address : %s\n", _a_rr(&data));
      break;
      
    case ns_t_aaaa:
      printf("IPv6 address : %s\n", (char*) _aaaa_rr(&data));
      break;
    
    case ns_t_ns:
    case ns_t_cname:
      printf("Alias : %s\n", _cname_rr(ptr_to_query, end, &data));
      break;

    default:
      return;
    }
  }
}

/**
 * @return whether the DNS dissector handles the given
 * packet.
 */
int dns_dissector_handles(const packet_t* packet)
{
  uint32_t	required_size;
  uint16_t	questions;
  uint16_t	answers;
  const dnshdr*	header;
  
  if (!packet || !packet->data
      || packet->size < sizeof(dnshdr)
      || packet->session == NULL) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  
  // Verifying that the packet does not contain
  // crazy data.
  header	= (const dnshdr*) packet->data;
  questions	= ntohs(header->qcount);
  answers	= ntohs(header->ancount);
  if (header->unused != 0 || (header->rcode > 5)
      || questions > 5 || answers > 10) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  
  // Computing the minimum required size of
  // the received packet.
  required_size = get_minimum_packet_size((const dnshdr*) packet->data);
  if (packet->size < required_size) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}
 
void dns_dissector_dump(const packet_t* packet)
{
  const dnshdr*	header;
  
  if (!dns_dissector_handles(packet)) {
    return;
  }
  
  header    = (const dnshdr*) packet->data;
  printf(DNS_OUTPUT_HEADER);
  printf("Identifier : %d\n", ntohs(header->id));
  printf("Type of packet : %s\n", dns_dissector_type_of_packet(header));
  if (header->rcode != 0) {
    printf("An error response was received from the remote server\n");
    return;
  }
  header->qr ?
    dns_dissector_dump_response_packet(packet) :
    dns_dissector_dump_query_packet(packet);
  printf(DNS_OUTPUT_FOOTER);
}

packet_t* dns_dissector_decapsulate(_UNUSED_ const packet_t* packet)
{
  return (NULL);
}
