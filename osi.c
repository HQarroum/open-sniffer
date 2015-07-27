#include "osi.h"

/**
 * @brief Predicate looking for a dissector handling the given packet.
 */
static int find_dissector_by_packet(_UNUSED_ size_t index, node_t* node, void* data)
{
  osi_dissector_t* dissector	 = (osi_dissector_t*) node->element;
  osi_packet_layer_pair_t* pair  = (osi_packet_layer_pair_t*) data;
  
  return (dissector && dissector->layer == pair->layer
	  && dissector->operations.handles(pair->packet));
}

/**
 * @brief Predicate looking for a dissector given a name.
 */
static int find_dissector_by_name(_UNUSED_ size_t index, node_t* node, void* data)
{
  osi_dissector_t* dissector = (osi_dissector_t*) node->element;
  const char* name           = (const char*) data;
  
  return (dissector && !strcmp(dissector->name, name));
}

/**
 * @brief Creates a new OSI stack.
 */
osi_stack_t osi_stack_new()
{
  return ((osi_stack_t) {
      .nb_dissectors = 0
  });
}

/**
 * @brief Clears the dissector list maintained
 * by the given `stack`.
 */
void osi_stack_clear(osi_stack_t* stack)
{
  list_clear(&(stack->dissectors));
}

packet_t* osi_dump_packet_by_layer(const osi_stack_t* stack, const packet_t* packet, osi_layer_t layer)
{  
  const osi_dissector_t* dissector = osi_find_dissector_by_packet(stack, packet, layer);
  packet_t* p = NULL;
  
  if (dissector) {
    p = dissector->operations.decapsulate(packet);
    dissector->operations.dump(packet);
  }
  return (p);
}

void osi_dump_packet_recursive(const osi_stack_t* stack, const packet_t* packet, osi_layer_t layer)
{
  packet_t* p;
  
  if (layer > OSI_LAYER_7) {
    return;
  }
  p = osi_dump_packet_by_layer(stack, packet, layer);
  if (p) {
    osi_dump_packet_recursive(stack, p, ++layer);
    osi_destroy_packet(p);
  } else {
    osi_dump_packet_recursive(stack, packet, ++layer);
  }
}

void osi_dump_packet(const osi_stack_t* stack, const packet_t* packet)
{
  packet_t* p;
  
  printf(OSI_PACKET_HEADER, get_current_time(), packet->size);
  p = osi_dump_packet_by_layer(stack, packet, OSI_LAYER_1);
  if (p) {
    osi_dump_packet_recursive(stack, p, OSI_LAYER_2);
    osi_destroy_packet(p);
  }
  printf(OSI_PACKET_FOOTER);
}

void osi_register_dissector(osi_stack_t* stack, const osi_dissector_t* dissector)
{
  list_push_back(&(stack->dissectors), dissector);
}

const osi_dissector_t* osi_find_dissector_by_packet(const osi_stack_t* stack, const packet_t* packet, osi_layer_t layer)
{
  node_t*		  node;
  osi_packet_layer_pair_t pair = {
    .packet = packet,
    .layer  = layer
  };
  
  node = list_find_node_if(&(stack->dissectors), find_dissector_by_packet, &pair);
  return (node ? node->element : NULL);
}

const osi_dissector_t* osi_find_dissector_by_name(const osi_stack_t* stack, const char* name)
{
  node_t* node = list_find_node_if(&(stack->dissectors), find_dissector_by_name, name);
  
  return (node ? node->element : NULL);
}

/**
 * \brief Creates a new `osi_session_t` instance.
 * \Return a pointer to an `osi_session_t` instance.
 */
osi_session_t* osi_create_session(uint32_t id, uint16_t src_port, uint16_t dest_port)
{
  osi_session_t* session = malloc(sizeof(*session));
  
  session->id            = id;
  session->src_port      = src_port;
  session->dest_port     = dest_port;
  return (session);
}

/**
 * \brief Frees the given `osi_session_t`.
 */
void osi_destroy_session(osi_session_t* session)
{
  free(session);
}

/**
 * @brief Destroys a packet which was dynamically
 * allocated.
 */
void osi_destroy_packet(packet_t* packet)
{
  if (packet) {
    osi_destroy_session(packet->session);
    free(packet->data);
    free(packet);
  }
}
