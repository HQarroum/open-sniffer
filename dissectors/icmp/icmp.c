#include "icmp.h"

static const osi_dissector_t icmp = {
  .layer         = OSI_LAYER_3,
  .name          = "icmp",
  .operations    = {
    .handles     = &icmp_dissector_handles,
    .dump        = &icmp_dissector_dump,
    .decapsulate = &icmp_dissector_decapsulate
  }
};

/**
 * \brief Creates a new instance of an ICMP
 * dissector.
 */
const osi_dissector_t* icmp_dissector_new()
{
  return (&icmp);
}

static const icmp_dissector_packet_types_t types[] = {
  { 0,  "ICMP_ECHOREPLY"      },
  { 3,  "ICMP_DEST_UNREACH"   }, 
  { 4,  "ICMP_SOURCE_QUENCH"  },
  { 5,  "ICMP_REDIRECT"       },
  { 8,  "ICMP_ECHO"           },
  { 11, "ICMP_TIME_EXCEEDED"  },
  { 12, "ICMP_PARAMETERPROB"  },
  { 13, "ICMP_TIMESTAMP"      },
  { 14, "ICMP_TIMESTAMPREPLY" },
  { 15, "ICMP_INFO_REQUEST"   },
  { 16, "ICMP_INFO_REPLY"     },
  { 17, "ICMP_ADDRESS"        },
  { 18, "ICMP_ADDRESSREPLY"   }
};

static const char* icmp_dissector_packet_type_stringified(__u8 code)
{
  for (size_t i = 0; i < sizeof(types); ++i) {
    if (types[i].type_code == code) {
      return (types[i].type_name);
    }
  }
  return (ICMP_TYPE_UNKNOWN);
}

int icmp_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->protocol != IPPROTO_ICMP
      || packet->size < sizeof(struct icmphdr)) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}
 
void icmp_dissector_dump(const packet_t* packet)
{
  if (!icmp_dissector_handles(packet)) {
    return;
  }
  
  struct icmphdr* header = (struct icmphdr*) packet->data;
  printf(ICMP_OUTPUT_HEADER);
  printf("Packet type : %s\n", icmp_dissector_packet_type_stringified(header->type));
  printf("Packet code : %d\n", header->code);
  printf("Checksum : 0x%x\n", ntohs(header->checksum));
  printf(ICMP_OUTPUT_FOOTER);
}

packet_t* icmp_dissector_decapsulate(const packet_t* packet)
{
  if (!icmp_dissector_handles(packet)) {
    return (NULL);
  }
  
  // Creating a new packet
  size_t size = packet->size - sizeof(struct icmphdr);
  packet_t* decapsulated = malloc(sizeof(*decapsulated));
  decapsulated->size     = size;
  decapsulated->data     = malloc(size);
  decapsulated->protocol = 0;
  decapsulated->session  = NULL;

  const uint8_t* buffer_src = ((const uint8_t*) packet->data + sizeof(struct icmphdr));
  memcpy(decapsulated->data, buffer_src, size);
  return (decapsulated);
}
