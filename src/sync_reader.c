#include <sync_reader.h>
#include <osi.h>

/**
 * \brief A predicate function used to compare whether the
 * given data pointer and the element associated with the
 * given node are equal.
 */
static int pointer_comparator(_UNUSED_ size_t index, node_t* node, void* data)
{
  return (data == node->element);
}

/**
 * \brief A predicate function that calls back packet handler
 * sequentially.
 */
static int notify_iterator(_UNUSED_ size_t index, node_t* node, void* data)
{
  packet_callback_t callback = (packet_callback_t) node->element;
  sync_reader_pack_t* pack   = (sync_reader_pack_t*) data;
  
  callback(pack->reader, pack->packet);
  return (0);
}

/**
 * \brief Iterates over packet callbacks to notify them of
 * a newly received packet.
 */
static void notify_packet(sync_reader_t* reader, const packet_t* packet)
{
  sync_reader_pack_t pack = {
    .reader = reader,
    .packet = packet
  };
  list_iterate_over_nodes(&(reader->callback_list), notify_iterator, &pack);
}

/**
 * \brief Creates a new sync reader.
 */
sync_reader_t sync_reader_create(int fd)
{
  sync_reader_t reader;
  
  reader.fd               = fd;
  reader.callback_list    = list_create_static();
  reader.interceptor_list = list_create_static();
  return (reader);
}

/**
 * Registers a new packet callback to the callback list.
 */
void sync_reader_add_packet_callback(sync_reader_t* reader, packet_callback_t callback)
{
  list_push_back(&(reader->callback_list), callback);
}

/**
 * Unregisters a packet callback from the callback list.
 */
void sync_reader_remove_packet_callback(sync_reader_t* reader, packet_callback_t callback)
{
  node_t* node = list_find_node_if(&(reader->callback_list), pointer_comparator, callback);
  list_remove_node(&(reader->callback_list), node);
}

/**
 * Registers a new packet interceptor to the interceptor list.
 */
void sync_reader_add_packet_interceptor(sync_reader_t* reader, packet_interceptor_t interceptor)
{
  list_push_back(&(reader->interceptor_list), interceptor);
}

/**
 * Unregisters a packet interceptor from the interceptor list.
 */
void sync_reader_remove_packet_interceptor(sync_reader_t* reader, packet_interceptor_t interceptor)
{
  node_t* node = list_find_node_if(&(reader->interceptor_list), pointer_comparator, interceptor);
  list_remove_node(&(reader->interceptor_list), node);
}

/**
 * \brief Reads on the socket and waits for data to
 * be received. This function will notify packet handlers
 * each time a new packet has been caught.
 */
ssize_t sync_reader_read(sync_reader_t* reader, size_t len)
{
  struct sockaddr	saddr;
  socklen_t		saddr_size;
  uint8_t*		buffer;
  packet_t		packet;
  
  saddr_size = sizeof(saddr);
  buffer     = malloc(len * sizeof(*buffer));
  memset(buffer, 0, len * sizeof(*buffer));
  
  // Reading from the file descriptor.
  ssize_t size_read = xrecvfrom(reader->fd, buffer, len, 0, &saddr, &saddr_size);
  
  // Building the packet.
  packet.data     = buffer;
  packet.size     = size_read;
  packet.protocol = 0;
  packet.session  = NULL;
  
  notify_packet(reader, &packet);
  free(buffer);
  return (size_read);
}
