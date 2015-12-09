#ifndef OSI_LAYER_H
#define OSI_LAYER_H

#include "io.h"
#include "app.h"
#include "termcaps.h"
#include "circular-linked-list/linked-list.h"

#define OSI_PACKET_NOT_SUPPORTED	0
#define OSI_PACKET_SUPPORTED		1

#define OSI_PACKET_HEADER \
  BOLD(									\
       "-------------------------------------------------------\n"	\
       "----------------  New packet received  ----------------\n"	\
       "-------------------------------------------------------\n"	\
       " Received : %s"							\
       " Packet size : %zu bytes\n"					\
       "-------------------------------------------------------\n"	\
       "-------------------------------------------------------\n"	\
  )

#define OSI_PACKET_FOOTER \
  BOLD(								\
       "-------------------------------------------------------\n\n\n"	\
  )

/**
 * \brief Definition of a network packet.
 * `data`       : The payload associated with the packet.
 * `size`       : The playload size.
 * `protocol`   : A hint about the protocol that is transported in the packet.
 * `session`    : The session associated with the packet, if any.
 */
typedef struct packet_t {
  void*                 data;
  size_t                size;
  uint32_t              protocol;
  struct osi_session_t* session;
} packet_t;

/**
 * \brief The OSI layer types definition.
 */
typedef enum osi_layer_t
{
  OSI_LAYER_1 = 1,
  OSI_LAYER_2 = 2,
  OSI_LAYER_3 = 3,
  OSI_LAYER_4 = 4,
  OSI_LAYER_5 = 5,
  OSI_LAYER_6 = 6,
  OSI_LAYER_7 = 7
} osi_layer_t;

/**
 * Declaration of the `osi_dissector_handles_t` type used to
 * implement a callback in dissectors stating whether they can handle
 * the received packet.
 */
typedef int		(*osi_dissector_handles_t)(const packet_t* packet);

/**
 * Declaration of the `osi_dissector_decapsulate_t` type used to
 * implement a callback in dissectors taking a packet as an input and returning
 * a decapsulated packet ready to be forwarded to the next `osi_layer_t`.
 */
typedef packet_t*	(*osi_dissector_decapsulate_t)(const packet_t* packet);

/**
 * Declaration of the `osi_dissector_dump_t` type used to
 * implement a callback in dissectors which outputs informations
 * about the given packet.
 */
typedef void		(*osi_dissector_dump_t)(const packet_t* packet);

typedef struct osi_dissector_operations_t
{
  // Returns whether the dissector handles the given packet.
  osi_dissector_handles_t handles;
  
  // Returns a new instance of a packet which contains decapsulated
  // data packet.
  osi_dissector_decapsulate_t decapsulate;
  
  // Dumps the content of the packet.
  osi_dissector_dump_t dump;
  
} osi_dissector_operations_t;

/**
 * \brief The dissector type definition.
 */
typedef struct osi_dissector_t
{
  osi_layer_t	layer;
  osi_dissector_operations_t operations;
  const char*	name;
} osi_dissector_t;

/**
 * Identifies a session. A session is a container
 * holding the required informations to track
 * a set of packets that belong to the same `stream`.
 *
 * This can typically be used in the context of protocols
 * such as TCP which associates packets to a stateful
 * connection. To re-trace and recompose the data part of
 * a TCP connection, a dissector can use the session associated
 * with each received packets.
 */
typedef struct osi_session_t
{
  // The unique identifier of the session.
  uint32_t id;
  
  // The source port, if any.
  uint16_t src_port;
  
  // The destination port, if any.
  uint16_t dest_port;
} osi_session_t;

/**
 * Definition of an `osi_stack_t`.
 */
typedef struct osi_stack_t
{
  list_t	dissectors;
  uint8_t	nb_dissectors;
} osi_stack_t;

typedef struct osi_packet_layer_pair_t
{
  const packet_t*	packet;
  osi_layer_t		layer;
} osi_packet_layer_pair_t;

/**
 * \brief Creates and returns a new instance
 * of an `osi_stack_t`.
 */
osi_stack_t		osi_stack_new();

/**
 * \brief Clears the dissector list maintained
 * by the given `stack`.
 */
void			osi_stack_clear(osi_stack_t* stack);

/**
 * \brief Iterates on every dissectors across the OSI layer which can handle
 * the packet and outputs informations about the packet for each of them.
 */
void			osi_dump_packet(const osi_stack_t* stack, const packet_t* packet);

/**
 * \brief Registers a new dissector with the given osi stack.
 */
void			osi_register_dissector(osi_stack_t* stack, const osi_dissector_t* dissector);

/**
 * \brief Finds a dissector for the given packet and the given layer.
 */
const osi_dissector_t*	osi_find_dissector_by_packet(const osi_stack_t* stack, const packet_t* packet, osi_layer_t layer);

/**
 * \brief Finds a dissector by name.
 */
const osi_dissector_t*	osi_find_dissector_by_name(const osi_stack_t* stack, const char* name);

/**
 * \brief Creates a new `osi_session_t` instance.
 * \Return a pointer to an `osi_session_t` instance.
 */
osi_session_t*		osi_create_session(uint32_t id, uint16_t src_port, uint16_t dest_port);

/**
 * \brief Destroys the given `session`.
 */
void			osi_destroy_session(osi_session_t* session);

/**
 * \brief Destroys the given `packet`.
 */
void			osi_destroy_packet(packet_t* packet);

#endif
