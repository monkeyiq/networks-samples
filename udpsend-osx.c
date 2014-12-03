/**
 *
 * There are chunks of this file which are from czmq 2.2.0. This is
 * intentional as the desire is to test UDP packet sending on osx in
 * as close to what czmq would do for beacons as possible.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <errno.h>

typedef struct sockaddr_in inaddr_t;    
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#include "inetcommon.c"



int main( int argc, char** argv  )
{
    inaddr_t *address = 0;

    int udpsock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);


    if (udpsock == INVALID_SOCKET) {
          zsys_socket_error("socket\n");
    }

    //  Ask operating system for broadcast permissions on socket
    int on = 1;
    if (setsockopt (udpsock, SOL_SOCKET, SO_BROADCAST,
                    (char *) &on, sizeof (on)) == SOCKET_ERROR)
        zsys_socket_error ("setsockopt (SO_BROADCAST)\n");

    //  Allow multiple owners to bind to socket; incoming
    //  messages will replicate to each owner
    if (setsockopt (udpsock, SOL_SOCKET, SO_REUSEADDR,
                    (char *) &on, sizeof (on)) == SOCKET_ERROR)
        zsys_socket_error ("setsockopt (SO_REUSEADDR)");

    //  On some platforms we have to ask to reuse the port
    if (setsockopt (udpsock, SOL_SOCKET, SO_REUSEPORT,
                    (char *) &on, sizeof (on)) == SOCKET_ERROR)
        zsys_socket_error ("setsockopt (SO_REUSEPORT)");

    
    inaddr_t broadcast;
    inaddr_t selfaddress;

    //
    // This loop is very much the same as the one in czmq 2.2.0
    //
    struct ifaddrs *interfaces;
    if (getifaddrs (&interfaces) == 0) {
        int num_interfaces = 0;
        struct ifaddrs *interface = interfaces;
        while (interface) {
            //  On Solaris, loopback interfaces have a NULL in ifa_broadaddr
            if  (interface->ifa_broadaddr
                 && !(interface->ifa_flags & IFF_LOOPBACK)       //  Ignore loopback interface
                 &&  (interface->ifa_flags & IFF_BROADCAST)      //  Only use interfaces that have BROADCAST
                 && !(interface->ifa_flags & IFF_POINTOPOINT)    //  Ignore point to point interfaces.
                 &&   interface->ifa_addr
                 &&  (interface->ifa_addr->sa_family == AF_INET)) {
                selfaddress = *(inaddr_t *) interface->ifa_addr;
                broadcast = *(inaddr_t *) interface->ifa_broadaddr;
                broadcast.sin_port = htons (5670);
            }
            interface = interface->ifa_next;

        }
    }
    
    printf("broadcast.sin_port: %d\n", broadcast.sin_port );
    showAddress("broadcast.sin_addr", broadcast );
    
// this will fail at bind()
//broadcast.sin_addr.s_addr = 0xFFFFFFFF;
//broadcast.sin_addr.s_addr = 0;
//broadcast.sin_addr.s_addr = broadcast.sin_addr.s_addr & 0x00FFFFFF | 195<<24;
    showAddress("broadcast.sin_addr", broadcast );

    // just using broadcast is not ok. bind() fails.
    inaddr_t sockaddr = broadcast;
    // this is ok
    // sockaddr.sin_addr.s_addr = INADDR_ANY;

    // this is ok
    sockaddr = selfaddress;
    
    showAddress("binding to address", sockaddr );
    if (bind (udpsock, (struct sockaddr *) &sockaddr, sizeof (sockaddr)) == SOCKET_ERROR)
        zsys_socket_error ("bind");

    inaddr_t dstaddr = broadcast;
    //dstaddr.sin_addr.s_addr = (dstaddr.sin_addr.s_addr & 0x00FFFFFF) | 255<<24;
    //dstaddr.sin_addr.s_addr = (dstaddr.sin_addr.s_addr & 0x00FFFFFF) | 195<<24;
    showAddress("dstaddr", dstaddr );
    
    
    errno = 0;
    int v = sendto (udpsock,
                    "123", 4,
                    0,      //  Flags
                    (struct sockaddr *) &dstaddr,
                    sizeof (inaddr_t));
    int e = errno;
    printf("an errno (e) of zero seems ok, the packet gets to other machines for me...\n" );
    printf("udp send e:%d\n", e );
    printf("udp send v:%d\n", v );
    // 97 == EAFNOSUPPORT;
    if( e < 0 )
        zsys_socket_error("at end...");
    else
        printf("success...\n");
    
    return 0;
}
