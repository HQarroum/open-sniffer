#ifndef _IN_ADDR_H_
# define _IN_ADDR_H_

#include "../osi.h"

#define ETH_STRING_LEN	17

/**
 * set the address of a struct sockaddr_storage
 * (modeled after the stdlib function inet_pton)
 *
 * @param src the address that should be assigned to the struct sockaddr_storage
 *            (either a dotted quad for IPv4 or a compressed IPv6 address)
 * @param dst the struct sockaddr_storage that should get the new address
 * @return 1 on success, 0 if address is not valid
 */
int		j_inet_pton(char *src, struct sockaddr_storage *dst);

/**
 * get the string representation of an address in struct sockaddr_storage
 * (modeled after the stdlib function inet_ntop)
 *
 * @param src the struct sockaddr_storage where the address should be read
 * @param dst where to write the result
 * @param size the size of the result buffer
 * @return NULL if failed, pointer to the result otherwise
 */
const char*	j_inet_ntop(struct sockaddr_storage *src, char *dst, size_t size);

/**
 * get the port number out of a struct sockaddr_storage
 *
 * @param sa the struct sockaddr_storage where we want to read the port
 * @return the port number (already converted to host byte order!)
 */
int		j_inet_getport(struct sockaddr_storage *sa);

/**
 * set the port number in a struct sockaddr_storage
 *
 * @param sa the struct sockaddr_storage where the port should be set
 * @param port the port number that should be set (in host byte order)
 * @return 1 on success, 0 if address family is not supported
 */
int		j_inet_setport(struct sockaddr_storage *sa, in_port_t port);

/**
 * calculate the size of an address structure
 * (on some unices the stdlibc functions for socket handling want to get the
 * size of the address structure that is contained in the
 * struct sockaddr_storage, not the size of struct sockaddr_storage itself)
 *
 * @param sa the struct sockaddr_storage for which we want to get the size of the contained address structure
 * @return the size of the contained address structure
 */
socklen_t	j_inet_addrlen(struct sockaddr_storage *sa);

/**
 * Transforms a 6-byte hardware address to its human-readable
 * representation.
 *
 * @return a static string containing the human readable of the
 * given hardware address.
 */
const char*	stringify_hw_addr(const uint8_t* hw_addr);

#endif
