#include <dissectors/ethernet/ethernet.h>

static const osi_dissector_t ethernet = {
  .layer         = OSI_LAYER_1,
  .name          = "Ethernet",
  .operations    = {
    .handles     = &ethernet_dissector_handles,
    .dump        = &ethernet_dissector_dump,
    .decapsulate = &ethernet_dissector_decapsulate
  }
};

/**
 * \brief Creates a new instance of an Ethernet
 * dissector.
 */
const osi_dissector_t* ethernet_dissector_new()
{
  return (&ethernet);
}

int ethernet_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->size < sizeof(struct ether_header)) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  
  struct ether_header *header = (struct ether_header *) packet->data;
  // Checking whether the type of the packet is
  // valid.
  u_int16_t type = ntohs(header->ether_type);
  if (type < 0x0200 || type > 0x9000) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }  
  return (OSI_PACKET_SUPPORTED);
}

void ethernet_dissector_dump(const packet_t* packet)
{
  if (!ethernet_dissector_handles(packet)) {
    return;
  }
  
  struct ether_header *eh = (struct ether_header *) packet->data;
  printf(ETHERNET_OUTPUT_HEADER);
  printf("Ethernet source address : %s\n", stringify_hw_addr(eh->ether_shost));
  printf("Ethernet destination address : %s\n", stringify_hw_addr(eh->ether_dhost));
  printf(ETHERNET_OUTPUT_FOOTER);
}

static uint32_t ethernet_dissector_get_hint(const packet_t* packet)
{
  return ntohs(((struct ether_header *) packet->data)->ether_type);
}

packet_t* ethernet_dissector_decapsulate(const packet_t* packet)
{
  if (!ethernet_dissector_handles(packet)) {
    return (NULL);
  }
  
  // Creating a new packet
  size_t size = packet->size - sizeof(struct ether_header);
  packet_t* decapsulated = malloc(sizeof(*decapsulated));
  decapsulated->size     = size;
  decapsulated->data     = malloc(size);
  decapsulated->protocol = ethernet_dissector_get_hint(packet);
  decapsulated->session  = NULL;
  
  const uint8_t* buffer_src = ((const uint8_t*) packet->data + sizeof(struct ether_header));
  memcpy(decapsulated->data, buffer_src, size);
  return (decapsulated);
}
