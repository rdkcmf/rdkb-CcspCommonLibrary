/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**********************************************************************

    module:	kernel_socket.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/04/01    initial revision.
        10/06/01    separated from the ansc_socket.h, one for each
                    specific platform

**********************************************************************/


#ifndef  _KERNEL_SOCKET_
#define  _KERNEL_SOCKET_

#ifndef  _ANSC_SOCKET_LIBRARY_
/***********************************************************
    PLATFORM DEPENDENT DATA TYPE AND MACRO DEFINITIONS
***********************************************************/

/*
 * We provide a platform-independent wrapper for all the socket related operations. However, since
 * the original bsd socket api functions are pretty stupid and ugly, instead of wrapping around the
 * socket functions with better names, we simplify the interface a little bit.
 */
#include "h\socket.h"

typedef             int		        ANSC_SOCKET,            *PANSC_SOCKET;
typedef  struct     sockaddr        ansc_socket_addr,       *pansc_socket_addr;
typedef				ipa				ansc_in_addr,			*pansc_in_addr;
typedef  struct     fd_set          ansc_fd_set,            *pansc_fd_set;
typedef  struct     timeval         ansc_timeval,           *pansc_timeval;
typedef  struct     hostent         ansc_hostent,           *pansc_hostent;

typedef  struct sockaddr_in_1
{
	unsigned char	sin_len;
	unsigned char	sin_family;
	word			sin_port;
	union
	{
		ipa			s_addr;
	}sin_addr;
	char	sin_zero[8];
}

typedef  struct sockaddr_in_1		ansc_socket_add_in,		*pansc_socket_add_in;

#ifdef   _ANSC_LITTLE_ENDIAN_

ULONG
_ansc_ntohl
    (
        ULONG                       ulValue
    );

#define  _ansc_htonl(ulValue)                   _ansc_ntohl(ulValue)

#define  _ansc_ntohs(usValue)						( (USHORT)( (USHORT)((usValue & 0xFF00) >> 8) + (USHORT)((usValue & 0x00FF) << 8) ) )
#define  _ansc_htons(usValue)						_ansc_ntohs(usValue)

#else    /*_ANSC_BIG_ENDIAN_*/

#define  _ansc_ntohl(ulValue)                   ulValue
#define  _ansc_htonl(ulValue)                   ulValue
#define  _ansc_ntohs(usValue)					usValue
#define  _ansc_htons(usValue)					usValue

#endif   /*_ANSC_BIG_ENDIAN_*/

#define  _ansc_accept                               accept
#define  _ansc_bind                                 bind
#define  _ansc_closesocket                          s_close
#define  _ansc_connect                              connect
#define  _ansc_gethostbyaddr                        gethostbyaddr
#define  _ansc_gethostbyname                        gethostbyname
#define  _ansc_gethostname                          gethostname
#define  _ansc_getpeername                          getpeername
#define  _ansc_getsocketname                        getsockname
#define  _ansc_getsocketopt                         getsockopt
#define  _ansc_inet_addr                            inet_addr
#define  _ansc_inet_ntoa                            inet_ntoa
#define  _ansc_listen                               listen
#define  _ansc_recv                                 recv
#define  _ansc_recvfrom                             recvfrom
#define  _ansc_select                               select3
#define  _ansc_send                                 send
#define  _ansc_sendto                               sendto
#define  _ansc_setsocketopt                         setsockopt
#define  _ansc_shutdown                             shutdown
#define  _ansc_socket                               dr_socket

/* Not suppoorted in Nucleus
#define  _ansc_get_last_error                       WSAGetLastError
*/
#define  _ansc_get_last_error()						-1						

#define  _ansc_en_broadcast(s)                                                              \
         {                                                                                  \
            int                    en_broadcast = 1;                                    \
            int                     op_len       = sizeof(int);                            \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_SO_BROADCAST,                                               \
                    (void *)&en_broadcast,                                                   \
                    op_len                                                                  \
                );                                                                          \
         }


#define  ANSC_SOCKET_AF_INET                        AF_INET4
#define  ANSC_SOCKET_STREAM                         SOCK_STREAM
#define  ANSC_SOCKET_DGRAM                          SOCK_DGRAM
#define  ANSC_SOCKET_RAWIP                          SOCK_RAW
/* Not supported by Nucleus 
#define  ANSC_SOCKET_INVALID_SOCKET                 INVALID_SOCKET
#define  ANSC_SOCKET_ERROR                          SOCKET_ERROR
*/
#define  ANSC_SOCKET_ANY_ADDRESS                    INADDR_ANY

#define  ANSC_SOCKET_FD_SETSIZE                     FD_SETSIZE
#define  ANSC_SOCKET_FD_CLR                         FD_CLR
#define  ANSC_SOCKET_FD_ISSET                       FD_ISSET
#define  ANSC_SOCKET_FD_SET                         FD_SET
#define  ANSC_SOCKET_FD_ZERO                        FD_ZERO


#define  ANSC_SOCKET_FD_GET(set, s, i)				s = ((set)->fds_bits[(i)/NFDBITS] & (1 << ((i) % NFDBITS)))

#define  ANSC_SOCKET_INVALID_SOCKET					-1
#define  ANSC_SOCKET_ERROR							-1
#define  ANSC_SOCKET_SOL_SOCKET                     SOL_SOCKET
/* Not supported by Nucleus
#define  ANSC_SOCKET_SOL_IPPROTO_TCP                IPPROTO_TCP
*/
#define  ANSC_SOCKET_SO_BROADCAST                   SO_BROADCAST
#define  ANSC_SOCKET_SO_REUSEADDR					SO_REUSEPORT

#define  ANSC_SOCKET_SD_RECV                        SHUT_RD
#define  ANSC_SOCKET_SD_SEND                        SHUT_WR
#define  ANSC_SOCKET_SD_BOTH                        SHUT_RDWR

#endif
#endif







