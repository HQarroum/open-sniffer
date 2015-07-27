#include "tcp.h"

static const osi_dissector_t tcp = {
  .layer         = OSI_LAYER_3,
  .name          = "tcp",
  .operations    = {
    .handles     = &tcp_dissector_handles,
    .dump        = &tcp_dissector_dump,
    .decapsulate = &tcp_dissector_decapsulate
  }
};

/**
 * \brief Creates a new instance of an TCP
 * dissector.
 */
const osi_dissector_t* tcp_dissector_new()
{
  return (&tcp);
}

static const char* tcp_dissector_get_packet_type(const struct tcphdr* header)
{
  if (header->fin) {
    return ("FIN");
  } else if (header->rst) {
    return ("RST");
  } else if (header->psh) {
    return ("PSH");
  } else if (header->syn) {
    return ("SYN");
  } else {
    return (NULL);
  }
}

int tcp_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->size < sizeof(struct tcphdr*)
      || packet->protocol != IPPROTO_TCP) {
    return (0);
  }
  
  struct tcphdr* header = (struct tcphdr*) packet->data;
  size_t size           = header->doff * 4;
  if (packet->size <= size) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}
 
void tcp_dissector_dump(const packet_t* packet)
{
  if (!tcp_dissector_handles(packet)) {
    return;
  }
  
  struct tcphdr* header = (struct tcphdr*) packet->data;
  printf(TCP_OUTPUT_HEADER);
  printf("Packet type : %s\n", tcp_dissector_get_packet_type(header));
  printf("Source port : %u\n", ntohs(header->source));
  printf("Destination port : %u\n", ntohs(header->dest));
  printf("Sequence number : %u\n", ntohs(header->seq));
  printf("Acknowledgement number : %u\n", ntohs(header->ack_seq));
  printf("Header length : %u bytes\n", ntohs(header->doff * 4));
  printf(TCP_OUTPUT_FOOTER);
}

packet_t* tcp_dissector_decapsulate(const packet_t* packet)
{
  if (!tcp_dissector_handles(packet)) {
    return (NULL);
  }
  
  // Creating a new packet
  struct tcphdr* header = (struct tcphdr*) packet->data;
  size_t size = packet->size - (header->doff * 4);
  packet_t* decapsulated = malloc(sizeof(*decapsulated));
  decapsulated->size     = size;
  decapsulated->data     = malloc(size);
  decapsulated->protocol = 0;
  decapsulated->session  = osi_create_session(header->seq, header->source, header->dest);
  
  const uint8_t* buffer_src = ((const uint8_t*) packet->data + (header->doff * 4));
  memcpy(decapsulated->data, buffer_src, size);
  return (decapsulated);
}
