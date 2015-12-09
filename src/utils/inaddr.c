/*
 * jabberd - Jabber Open Source Server
 * Copyright (c) 2002 Jeremie Miller, Thomas Muldowney,
 *                    Ryan Eatmon, Robert Norris
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA02111-1307USA
 */

/**
 * @file util/inaddr.c
 * @brief object like wrapper around struct sockaddr_storage
 *
 * The functions in this file are used as a wrapper around struct
 * sockaddr_storage to access this structure like an object. The structure
 * is seen as an object that contains an IPv4 or an IPv6 address and
 * these functions are the access methods to this object.
 * 
 * @warning this is the same as mio/mio_inaddr.c - changes made here need to be
 *          made there too. is there anyway we can merge these without
 *          requiring mio to depend on util?
 */

#include <utils/inaddr.h>

/**
 * set the address of a struct sockaddr_storage
 * (modeled after the stdlib function inet_pton)
 *
 * @param src the address that should be assigned to the struct sockaddr_storage
 *            (either a dotted quad for IPv4 or a compressed IPv6 address)
 * @param dst the struct sockaddr_storage that should get the new address
 * @return 1 on success, 0 if address is not valid
 */
int j_inet_pton(char *src, struct sockaddr_storage *dst)
{
  struct sockaddr_in *sin;
  struct sockaddr_in6 *sin6;

  memset(dst, 0, sizeof(struct sockaddr_storage));
  sin = (struct sockaddr_in *)dst;
  sin6 = (struct sockaddr_in6 *)dst;
    
  if(inet_pton(AF_INET, src, &sin->sin_addr) > 0)
    {
      dst->ss_family = AF_INET;
      return 1;
    }

  if(inet_pton(AF_INET6, src, &sin6->sin6_addr) > 0)
    {
      dst->ss_family = AF_INET6;
#ifdef SIN6_LEN
      sin6->sin6_len = sizeof(struct sockaddr_in6);
#endif
      return 1;
    }

  return 0;
}

/**
 * get the string representation of an address in struct sockaddr_storage
 * (modeled after the stdlib function inet_ntop)
 *
 * @param src the struct sockaddr_storage where the address should be read
 * @param dst where to write the result
 * @param size the size of the result buffer
 * @return NULL if failed, pointer to the result otherwise
 */
const char *j_inet_ntop(struct sockaddr_storage *src, char *dst, size_t size)
{
  struct sockaddr_in *sin;
  struct sockaddr_in6 *sin6;

  sin = (struct sockaddr_in *)src;
  sin6 = (struct sockaddr_in6 *)src;

  switch(src->ss_family)
    {
    case AF_UNSPEC:
    case AF_INET:
      return inet_ntop(AF_INET, &sin->sin_addr, dst, size);
    case AF_INET6:
      return inet_ntop(AF_INET6, &sin6->sin6_addr, dst, size);
    default:
      return NULL;
    }
}

/**
 * get the port number out of a struct sockaddr_storage
 *
 * @param sa the struct sockaddr_storage where we want to read the port
 * @return the port number (already converted to host byte order!)
 */
int j_inet_getport(struct sockaddr_storage *sa)
{
  struct sockaddr_in *sin;
  struct sockaddr_in6 *sin6;
    
  switch(sa->ss_family)
    {
    case AF_INET:
      sin = (struct sockaddr_in *)sa;
      return ntohs(sin->sin_port);
    case AF_INET6:
      sin6 = (struct sockaddr_in6 *)sa;
      return ntohs(sin6->sin6_port);
    default:
      return 0;
    }
}

/**
 * set the port number in a struct sockaddr_storage
 *
 * @param sa the struct sockaddr_storage where the port should be set
 * @param port the port number that should be set (in host byte order)
 * @return 1 on success, 0 if address family is not supported
 */
int j_inet_setport(struct sockaddr_storage *sa, in_port_t port)
{
  struct sockaddr_in *sin;
  struct sockaddr_in6 *sin6;

  sin = (struct sockaddr_in *)sa;
  sin6 = (struct sockaddr_in6 *)sa;

  switch(sa->ss_family)
    {
    case AF_INET:
      sin->sin_port = htons(port);
      return 1;
    case AF_INET6:
      sin6->sin6_port = htons(port);
      return 1;
    default:
      return 0;
    }
}

/**
 * calculate the size of an address structure
 * (on some unices the stdlibc functions for socket handling want to get the
 * size of the address structure that is contained in the
 * struct sockaddr_storage, not the size of struct sockaddr_storage itself)
 *
 * @param sa the struct sockaddr_storage for which we want to get the size of the contained address structure
 * @return the size of the contained address structure
 */
socklen_t j_inet_addrlen(struct sockaddr_storage *sa)
{
#ifdef SIN6_LEN
  if(sa->ss_len != 0)
    return sa->ss_len;
#endif
  switch(sa->ss_family)
    {
    case AF_INET:
      return sizeof(struct sockaddr_in);
    case AF_INET6:
      return sizeof(struct sockaddr_in6);
    default:
      return sizeof(struct sockaddr_storage);
    }
}

/**
 * Transforms a 6-byte hardware address to its human-readable
 * representation.
 * 
 * @return a static string containing the human readable of the
 * given hardware address.
 */
const char* stringify_hw_addr(const uint8_t* hw_addr)
{
  static char addr[ETH_STRING_LEN + 1];
  
  sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x",
         hw_addr[0] & 0xff,
         hw_addr[1] & 0xff,
         hw_addr[2] & 0xff,
         hw_addr[3] & 0xff,
         hw_addr[4] & 0xff,
         hw_addr[5] & 0xff);
  return (addr);
}
