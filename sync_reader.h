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
  int       fd;
  list_t    callback_list;
  list_t    interceptor_list;
} sync_reader_t;

/**
 * \brief A structure packing a pointer to a
 * reader, and a pointer to a packet.
 */
typedef struct sync_reader_pack_t {
  const sync_reader_t*reader;
  const packet_t* packet;
} sync_reader_pack_t;

/**
 * \brief Creates a new sync reader.
 */
sync_reader_t	sync_reader_create(int fd);
void     sync_reader_add_packet_callback(sync_reader_t* reader, packet_callback_t callback);
void     sync_reader_remove_packet_callback(sync_reader_t* reader, packet_callback_t callback);
void     sync_reader_add_packet_interceptor(sync_reader_t* reader, packet_interceptor_t interceptor);
void     sync_reader_remove_packet_interceptor(sync_reader_t* reader, packet_interceptor_t interceptor);
ssize_t  sync_reader_read(sync_reader_t* reader, size_t len);

#endif
