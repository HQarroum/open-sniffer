#include <dissectors/ip/ip.h>

static const osi_dissector_t ip = {
  .layer         = OSI_LAYER_2,
  .name          = "IP",
  .operations    = {
    .handles     = &ip_dissector_handles,
    .dump        = &ip_dissector_dump,
    .decapsulate = &ip_dissector_decapsulate
  }
};

/**
 * \brief Creates a new instance of an IP
 * dissector.
 */
const osi_dissector_t* ip_dissector_new()
{
  return (&ip);
}

static const char* ip_to_string(uint32_t ip_address)
{
  return (inet_ntoa(*((struct in_addr*) &ip_address)));
}

/**
 * \brief Returns whether the dissector can handle
 * the given packet.
 * \return 0 if the dissector cannot handle the
 * given packet, 1 otherwise.
 */
int ip_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->size < sizeof(struct iphdr*)
      || packet->protocol != ETHERTYPE_IP) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  
  // Verifying the correctness of the packet.
  struct iphdr *header = (struct iphdr*) packet->data;
  size_t hdr_size = header->ihl * 4;
  if ((packet->size < hdr_size)
      || (header->version != 4 && header->version != 6)) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}

static uint32_t ip_dissector_get_hint(const packet_t* packet)
{
  return ((struct iphdr*) packet->data)->protocol;
}

void ip_dissector_dump(const packet_t* packet)
{
  if (!ip_dissector_handles(packet)) {
    return;
  }
  
  struct iphdr* header = (struct iphdr*) packet->data;
  printf(IP_OUTPUT_HEADER);
  printf("Source address : %s\n", ip_to_string(header->saddr));
  printf("Destination address : %s\n", ip_to_string(header->daddr));
  printf("Checksum : 0x%x\n", ntohs(header->check));
  printf(IP_OUTPUT_FOOTER);
}

packet_t* ip_dissector_decapsulate(const packet_t* packet)
{
  if (!ip_dissector_handles(packet)) {
    return (NULL);
  }
  
  // Creating a new packet
  struct iphdr *header = (struct iphdr*) packet->data;
  size_t size = packet->size - (header->ihl * 4);
  
  packet_t* decapsulated = malloc(sizeof(*decapsulated));
  decapsulated->size     = size;
  decapsulated->data     = malloc(size);
  decapsulated->protocol = ip_dissector_get_hint(packet);
  decapsulated->session  = NULL;
  
  const uint8_t* buffer_src = ((const uint8_t*) packet->data + (header->ihl * 4));
  memcpy(decapsulated->data, buffer_src, size);
  return (decapsulated);
}
