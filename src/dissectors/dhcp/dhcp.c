#include <dissectors/dhcp/dhcp.h>

static const osi_dissector_t dhcp = {
  .layer         = OSI_LAYER_7,
  .name          = "DHCP",
  .operations    = {
    .handles     = &dhcp_dissector_handles,
    .dump        = &dhcp_dissector_dump,
    .decapsulate = &dhcp_dissector_decapsulate
  }
};

/**
 * \brief A mapping between hardware types
 * and their human-readable representation.
 */
static const dhcp_hw_type_map hw_types[] = {
  { 1,       "Ethernet" },
  { 6,       "IEEE 802 Networks" },
  { 7,       "ARCNET" },
  { 11,      "LocalTalk" },
  { 12,      "LocalNet" },
  { 14,      "SMDS" },
  { 15,      "Frame Relay" },
  { 16,      "ATM" },
  { 17,      "HDLC" },
  { 18,      "Fibre Channel" },
  { 19,      "ATM" },
  { 20,      "Serial Line" },
};

/**
 * \brief Creates a new instance of a DHCP
 * dissector.
 */
const osi_dissector_t* dhcp_dissector_new()
{
  return (&dhcp);
}

static const char* ip_to_string(uint32_t ip_address)
{
  return (inet_ntoa(*((struct in_addr*) &ip_address)));
}

static const char* op_to_string(uint8_t op)
{
  return (op == 1 ? "Request" : "Reply");
}

/**
 * \return a human-readable representation of the
 * given hardware type.
 */
static const char* hw_type_to_string(uint8_t type)
{
  for (size_t i = 0; i < sizeof(hw_types) / sizeof(*hw_types); ++i) {
    if (hw_types[i].type == type) {
      return (hw_types[i].value);
    }
  }
  return (DHCP_UNKNOWN_HW_TYPE);
}

/**
 * \brief Returns whether the dissector can handle
 * the given packet.
 * \return 0 if the dissector cannot handle the
 * given packet, 1 otherwise.
 */
int dhcp_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->size < sizeof(struct dhcphdr*)) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  
  // Verifying the correctness of the packet.
  struct dhcphdr *header = (struct dhcphdr*) packet->data;
  if ((header->op != 1 && header->op != 2)
      || (header->htype > 20)) {
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}

void dhcp_dissector_dump(const packet_t* packet)
{
  if (!dhcp_dissector_handles(packet)) {
    return;
  }
  
  struct dhcphdr* header = (struct dhcphdr*) packet->data;
  printf(DHCP_OUTPUT_HEADER);
  printf("Type : %s\n", op_to_string(header->op));
  printf("Hardware type : %s\n", hw_type_to_string(header->htype));
  printf("XID : %x\n", ntohl(header->xid));
  printf("CIADDR : %s\n", ip_to_string(header->client_addr));
  printf("YIADDR : %s\n", ip_to_string(header->your_addr));
  printf("SIADDR : %s\n", ip_to_string(header->server_addr));
  printf("Client address : %s\n",
	 stringify_hw_addr(header->client_hw_addr));
	 /*	 header->client_hw_addr[0],
	 header->client_hw_addr[1],
	 header->client_hw_addr[2],
	 header->client_hw_addr[3],
	 header->client_hw_addr[4],
	 header->client_hw_addr[5],
	 header->client_hw_addr[6],
	 header->client_hw_addr[7]);*/
  printf(DHCP_OUTPUT_FOOTER);
}

packet_t* dhcp_dissector_decapsulate(_UNUSED_ const packet_t* packet)
{
  return (NULL);
}
