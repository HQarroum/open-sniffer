#include "io.h"

/**
 * \brief Returns a file descriptor associated
 * with a RAW socket.
 */
int make_raw_socket()
{
  int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  
  if (fd == -1) {
    fail("socket");
  }
  return (fd);
}

/**
 * \brief Allows to receive messages from a socket,
 * and may be used to receive data on a socket whether
 * or not it is connection-oriented.
 */
ssize_t xrecvfrom(int sockfd, void *buf, size_t len, int flags,
		 struct sockaddr *src_addr, socklen_t *addrlen)
{
  ssize_t size = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
  
  if (size == -1) {
    fail("recvfrom");
  }
  return (size);
}

/**
 * \brief Allows to transmit a message to another socket.
 */
ssize_t xsendto(int sockfd, const void *buf, size_t len, int flags,
		const struct sockaddr *dest_addr, socklen_t addrlen)
{
  ssize_t size = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
  
  if (size == -1) {
    fail("sendto");
  }
  return (size);
}

/**
 * \brief Closes a file descriptor, so that it no longer
 * refers to any file and may be reused.
 */
int xclose(int fd)
{
  int value = close(fd);
  
  if (value == -1) {
    fail("close");
  }
  return (value);
}
