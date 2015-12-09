#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include "app.h"

/**
 * \brief Returns a file descriptor associated
 * with a RAW socket.
 */
int	make_raw_socket();

/**
 * \brief Allows to receive messages from a socket,
 * and may be used to receive data on a socket whether
 * or not it is connection-oriented.
 */
ssize_t xrecvfrom(int sockfd, void *buf, size_t len, int flags,
		  struct sockaddr *src_addr, socklen_t *addrlen);

/**
 * \brief Allows to transmit a message to another socket.
 */
ssize_t xsendto(int sockfd, const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen);

/**
 * \brief Closes a file descriptor, so that it no longer
 * refers to any file and may be reused.
 */
int	xclose(int fd);

#endif // SOCKET_H
