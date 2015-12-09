#include <dissectors/arp/arp.h>

static const osi_dissector_t arp = {
  .layer         = OSI_LAYER_2,
  .name          = "arp",
  .operations    = {
    .handles     = &arp_dissector_handles,
    .dump        = &arp_dissector_dump,
    .decapsulate = &arp_dissector_decapsulate
  }
};

/**
 * \brief Creates a new instance of an ARP
 * dissector.
 */
const osi_dissector_t* arp_dissector_new()
{
  return (&arp);
}

int arp_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->protocol != ETHERTYPE_ARP
      || packet->size < sizeof(struct ether_arp)) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}

void arp_dissector_dump(const packet_t* packet)
{
  if (!arp_dissector_handles(packet)) {
    return;
  }
  
  struct ether_arp* arp_hdr = (struct ether_arp*) packet->data;
  printf(ARP_OUTPUT_HEADER);
  printf("ARP Sender Hardware address : %s\n", stringify_hw_addr(arp_hdr->arp_sha));
  printf("ARP Target Hardware address : %s\n", stringify_hw_addr(arp_hdr->arp_tha));
  printf(ARP_OUTPUT_FOOTER);
}

packet_t* arp_dissector_decapsulate(const packet_t* packet)
{
  if (!arp_dissector_handles(packet)) {
    return (NULL);
  }
  
  // Creating a new packet
  size_t size = packet->size - sizeof(struct ether_arp);
  packet_t* decapsulated = malloc(sizeof(*decapsulated));
  decapsulated->size     = size;
  decapsulated->data     = malloc(size);
  decapsulated->protocol = 0;
  decapsulated->session  = NULL;

  const uint8_t* buffer_src = ((const uint8_t*) packet->data + sizeof(struct ether_arp));
  memcpy(decapsulated->data, buffer_src, size);
  return (decapsulated);
}
