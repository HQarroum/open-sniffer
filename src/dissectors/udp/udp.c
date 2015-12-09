#include <dissectors/udp/udp.h>

static const osi_dissector_t udp = {
  .layer         = OSI_LAYER_3,
  .name          = "udp",
  .operations    = {
    .handles     = &udp_dissector_handles,
    .dump        = &udp_dissector_dump,
    .decapsulate = &udp_dissector_decapsulate
  }
};

/**
 * \brief Creates a new instance of an UDP
 * dissector.
 */
const osi_dissector_t* udp_dissector_new()
{
  return (&udp);
}

int udp_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->protocol != IPPROTO_UDP
      || packet->size < sizeof(struct udphdr)) {
    // Invalid packet.
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}
 
static uint32_t udp_dissector_get_hint(_UNUSED_ const packet_t* packet)
{
  return (0);
}

void udp_dissector_dump(const packet_t* packet)
{
  if (!udp_dissector_handles(packet)) {
    return;
  }
  
  struct udphdr* header = (struct udphdr*) packet->data;
  printf(UDP_OUTPUT_HEADER);
  printf("UDP source port : %d\n", ntohs(header->source));
  printf("UDP destination port : %d\n", ntohs(header->dest));
  printf(UDP_OUTPUT_FOOTER);
}

packet_t* udp_dissector_decapsulate(const packet_t* packet)
{
  if (!udp_dissector_handles(packet)) {
    return (NULL);
  }
  
  // Creating a new packet
  struct udphdr* header  = (struct udphdr*) packet->data;
  size_t size		 = packet->size - sizeof(struct udphdr);
  packet_t* decapsulated = malloc(sizeof(*decapsulated));
  decapsulated->size     = size;
  decapsulated->data     = malloc(size);
  decapsulated->protocol = udp_dissector_get_hint(packet);
  decapsulated->session  = osi_create_session(0, header->source, header->dest);

  const uint8_t* buffer_src = ((const uint8_t*) packet->data + sizeof(struct udphdr));
  memcpy(decapsulated->data, buffer_src, size);
  return (decapsulated);
}
