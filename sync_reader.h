#ifndef SYNC_READER_H
#define SYNC_READER_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "circular-linked-list/linked-list.h"
#include "io.h"
#include "osi.h"
#include "app.h"

struct sync_reader_t;
struct osi_session_t;

/**
 * \brief Definition of a packet_interceptor_t type.
 */
typedef packet_t* (*packet_interceptor_t)(const struct sync_reader_t* reader, const packet_t* packet);

/**
 * \brief Definition of a packet_callback_t type.
 */
typedef void (*packet_callback_t)(const struct sync_reader_t* reader, const packet_t* packet);

/**
 * \brief Definition of the sync_reader_t
 * structure.
 * `fd`		: The file descriptor associated with the reader.
 * `callbacks`	: An array of packet listeners.
 */
typedef struct sync_reader_t {
  int     fd;
  list_t  callback_list;
  list_t  interceptor_list;
} sync_reader_t;

/**
 * \brief A structure packing a pointer to a
 * reader, and a pointer to a packet.
 */
typedef struct sync_reader_pack_t {
  const sync_reader_t* reader;
  const packet_t* packet;
} sync_reader_pack_t;

/**
 * \brief Creates a new sync reader.
 * \param fd the file descriptor on which the
 * reader should actually read
 */
sync_reader_t sync_reader_create(int fd);

/**
 * \brief Adds a new packet listener to the reader.
 * \param reader the reader to attach the listener to
 * \param callback the callback to attach to the reader
 */
void          sync_reader_add_packet_callback(sync_reader_t* reader, packet_callback_t callback);

/**
 * \brief Removes a packet listener from the reader.
 * \param reader the reader to remove the listener from
 * \param callback the callback to detach from the reader
 */
void          sync_reader_remove_packet_callback(sync_reader_t* reader, packet_callback_t callback);

/**
 * \brief Adds a new packet interceptor to the reader.
 * \param reader the reader to attach the interceptor to
 * \param interceptor the interceptor to attach to the reader
 */
void          sync_reader_add_packet_interceptor(sync_reader_t* reader, packet_interceptor_t interceptor);

/**
 * \brief Removes a packet interceptor from the reader.
 * \param reader the reader to remove the interceptor from
 * \param interceptor the interceptor to detach from the reader
 */
void          sync_reader_remove_packet_interceptor(sync_reader_t* reader, packet_interceptor_t interceptor);

/**
 * \brief Blocks until the reader read a packet from the
 * internal file descriptor.
 * \param reader the reader to read from
 * \param len the size in bytes to read at most
 */
ssize_t       sync_reader_read(sync_reader_t* reader, size_t len);

#endif
