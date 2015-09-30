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

    module:	user_socket.h

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


#ifndef  _USER_SOCKET_
#define  _USER_SOCKET_


/***********************************************************
    PLATFORM DEPENDENT DATA TYPE AND MACRO DEFINITIONS
***********************************************************/

/*
 * We provide a platform-independent wrapper for all the socket related operations. However, since
 * the original bsd socket api functions are pretty stupid and ugly, instead of wrapping around the
 * socket functions with better names, we simplify the interface a little bit.
 */
#ifndef  _ANSC_SOCKET_LIBRARY_

    #include <winsock2.h>
    #include <ws2tcpip.h>

    typedef             SOCKET          ANSC_SOCKET,            *PANSC_SOCKET;
    typedef  struct     in_addr         ansc_in_addr,           *pansc_in_addr;
    typedef  struct     sockaddr_in     ansc_socket_addr_in,    *pansc_socket_addr_in;
    typedef  struct     sockaddr        ansc_socket_addr,       *pansc_socket_addr;
    typedef  struct     fd_set          ansc_fd_set,            *pansc_fd_set;
    typedef  struct     timeval         ansc_timeval,           *pansc_timeval;
    typedef  struct     hostent         ansc_hostent,           *pansc_hostent;
    typedef  struct     ip_mreq         ansc_ip_mreq,           *pansc_ip_mreq;

    #define  _ansc_accept                           accept
    #define  _ansc_bind                             bind
    #define  _ansc_closesocket                      closesocket
    #define  _ansc_connect                          connect
    #define  _ansc_gethostbyaddr                    gethostbyaddr
    #define  _ansc_gethostbyname                    gethostbyname
    #define  _ansc_gethostname                      gethostname
    #define  _ansc_getpeername                      getpeername
    #define  _ansc_getsocketname                    getsockname
    #define  _ansc_getsocketopt                     getsockopt
    #define  _ansc_htonl                            htonl
    #define  _ansc_htons                            htons
    #define  _ansc_inet_addr                        inet_addr
    #define  _ansc_inet_ntoa                        inet_ntoa
    #define  _ansc_ioctlsocket                      ioctlsocket
    #define  _ansc_listen                           listen
    #define  _ansc_ntohl                            ntohl
    #define  _ansc_ntohs                            ntohs
    #define  _ansc_recv                             recv
    #define  _ansc_recvfrom                         recvfrom
    #define  _ansc_select                           select
    #define  _ansc_send                             send
    #define  _ansc_sendto                           sendto
    #define  _ansc_setsocketopt                     setsockopt
    #define  _ansc_shutdown                         shutdown
    #define  _ansc_socket                           socket
    #define  _ansc_get_last_error                   WSAGetLastError

    #define  ANSC_SOCKET_FLAG_MSG_PEEK              MSG_PEEK
    #define  ANSC_SOCKET_FLAG_MSG_OOB               MSG_OOB

    #define  ANSC_SOCKET_AF_INET                    AF_INET
    #define  ANSC_SOCKET_STREAM                     SOCK_STREAM
    #define  ANSC_SOCKET_DGRAM                      SOCK_DGRAM
    #define  ANSC_SOCKET_RAWIP                      SOCK_RAW
    #define  ANSC_SOCKET_INVALID_SOCKET             INVALID_SOCKET
    #define  ANSC_SOCKET_ERROR                      SOCKET_ERROR
    #define  ANSC_SOCKET_ANY_ADDRESS                INADDR_ANY

    #define  ANSC_SOCKET_FD_SETSIZE                 FD_SETSIZE
    #define  ANSC_SOCKET_FD_CLR                     FD_CLR
    #define  ANSC_SOCKET_FD_ISSET                   FD_ISSET
    #define  ANSC_SOCKET_FD_SET                     FD_SET
    #define  ANSC_SOCKET_FD_ZERO                    FD_ZERO
    #define  ANSC_SOCKET_FD_ISNUL(set)              ( (((ansc_fd_set*)(set))->fd_count == 0) )
    #define  ANSC_SOCKET_FD_GET(set, s, i)                                                  \
             {                                                                              \
                ansc_fd_set*        socket_set = (ansc_fd_set*)set;                         \
                ANSC_SOCKET         socket_nul = ANSC_SOCKET_INVALID_SOCKET;                \
                                                                                            \
                s = ((u_int)i < socket_set->fd_count)? socket_set->fd_array[i] : socket_nul;\
             }

    #define  ANSC_SOCKET_SOL_SOCKET                 SOL_SOCKET
    #define  ANSC_SOCKET_SOL_IPPROTO_TCP            IPPROTO_TCP
    #define  ANSC_SOCKET_SOL_IPPROTO_IP             IPPROTO_IP
    #define  ANSC_SOCKET_SO_BROADCAST               SO_BROADCAST
    #define  ANSC_SOCKET_SO_REUSEADDR               SO_REUSEADDR
    #define  ANSC_SOCKET_SO_LASTRECVIF              0
    #define  ANSC_SOCKET_SO_TOS                     0
    #define  ANSC_SOCKET_SO_TTL                     0

    #define  ANSC_SOCKET_SD_RECV                    SD_RECEIVE
    #define  ANSC_SOCKET_SD_SEND                    SD_SEND
    #define  ANSC_SOCKET_SD_BOTH                    SD_BOTH

    #define  ANSC_SOCKET_SO_TCP_USETLS              0
    #define  ANSC_SOCKET_SO_TCP_TLSEXPORTONLY       0
    #define  ANSC_SOCKET_SO_TCP_TLSSTRONGSEC        0

    #define  ANSC_SOCKET_SO_ACCEPTCONN              SO_ACCEPTCONN
    #define  ANSC_SOCKET_SO_RCVLOWAT                SO_RCVLOWAT
    #define  ANSC_SOCKET_SO_RCVTIMEO                SO_RCVTIMEO
    #define  ANSC_SOCKET_SO_SNDLOWAT                SO_SNDLOWAT
    #define  ANSC_SOCKET_SO_SNDTIMEO                SO_SNDTIMEO
    #define  ANSC_SOCKET_SO_TYPE                    SO_TYPE

    #define  ANSC_SOCKET_IP_MULTICAST_TTL           IP_MULTICAST_TTL
    #define  ANSC_SOCKET_IP_MULTICAST_IF            IP_MULTICAST_IF
    #define  ANSC_SOCKET_IP_ADD_MEMBERSHIP          IP_ADD_MEMBERSHIP
    #define  ANSC_SOCKET_IP_DROP_MEMBERSHIP         IP_DROP_MEMBERSHIP
    #define  ANSC_SOCKET_IP_MULTICAST_LOOP          IP_MULTICAST_LOOP
    #define  ANSC_SOCKET_IP_RECV_UNICAST            0

#endif


/***********************************************************
    PLATFORM DEPENDENT DATA TYPE AND MACRO DEFINITIONS
***********************************************************/

/*
 * Simulating the complete TCP/IP stack over the UDP socket introduces a problem: the underlying
 * Ethernet-over-Udp object MUST bind to the socket layer provided by the operating system while
 * the real socket applications MUST bind to the ANSC socket library. But both are using the same
 * wrapper function calls. These two scenarios cannot be differentiated merely by the #ifdef or
 * other similar approaches. To solve this problem, we define following macros, which always link
 * to the external socket library, hence "xskt".
 */
#ifdef  _USER_SOCKET_

    #include <winsock2.h>
    #include <ws2tcpip.h>

    typedef             SOCKET          XSKT_SOCKET,            *PXSKT_SOCKET;
    typedef  struct     in_addr         xskt_in_addr,           *pxskt_in_addr;
    typedef  struct     sockaddr_in     xskt_socket_addr_in,    *pxskt_socket_addr_in;
    typedef  struct     sockaddr        xskt_socket_addr,       *pxskt_socket_addr;
    typedef  struct     fd_set          xskt_fd_set,            *pxskt_fd_set;
    typedef  struct     timeval         xskt_timeval,           *pxskt_timeval;
    typedef  struct     hostent         xskt_hostent,           *pxskt_hostent;
    typedef  struct     ip_mreq         xskt_ip_mreq,           *pxskt_ip_mreq;

    #define  _xskt_accept                           accept
    #define  _xskt_bind                             bind
    #define  _xskt_closesocket                      closesocket
    #define  _xskt_connect                          connect
    #define  _xskt_gethostbyaddr                    gethostbyaddr
    #define  _xskt_gethostbyname                    gethostbyname
    #define  _xskt_gethostname                      gethostname
    #define  _xskt_getpeername                      getpeername
    #define  _xskt_getsocketname                    getsockname
    #define  _xskt_getsocketopt                     getsockopt
    #define  _xskt_htonl                            htonl
    #define  _xskt_htons                            htons
    #define  _xskt_inet_addr                        inet_addr
    #define  _xskt_inet_ntoa                        inet_ntoa
    #define  _xskt_ioctlsocket                      ioctlsocket
    #define  _xskt_listen                           listen
    #define  _xskt_ntohl                            ntohl
    #define  _xskt_ntohs                            ntohs
    #define  _xskt_recv                             recv
    #define  _xskt_recvfrom                         recvfrom
    #define  _xskt_select                           select
    #define  _xskt_send                             send
    #define  _xskt_sendto                           sendto
    #define  _xskt_setsocketopt                     setsockopt
    #define  _xskt_shutdown                         shutdown
    #define  _xskt_socket                           socket
    #define  _xskt_get_last_error                   WSAGetLastError

    #define  XsktResolveHostName(name, address)                                             \
             {                                                                              \
                xskt_hostent*           resolved_hostent = NULL;                            \
                                                                                            \
                if ( AnscIsValidIpString(name) )                                            \
                {                                                                           \
                    *(PULONG)address = _xskt_inet_addr(name);                               \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    resolved_hostent = _xskt_gethostbyname(name);                           \
                                                                                            \
                    if ( !resolved_hostent )                                                \
                    {                                                                       \
                        *(PULONG)address = 0;                                               \
                    }                                                                       \
                    else if ( resolved_hostent->h_length < IPV4_ADDRESS_SIZE )              \
                    {                                                                       \
                        *(PULONG)address = 0;                                               \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        *(PULONG)address = *(PULONG)resolved_hostent->h_addr;               \
                    }                                                                       \
                }                                                                           \
             }

    #define  XsktResolveHostName2(name, addr_value_array, addr_value_count)                 \
             {                                                                              \
                xskt_hostent*           resolved_hostent = NULL;                            \
                ULONG                   resolved_count   = 0;                               \
                                                                                            \
                if ( AnscIsValidIpString(name) )                                            \
                {                                                                           \
                    addr_value_array[0] = _xskt_inet_addr(name);                            \
                    addr_value_count    = 1;                                                \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    resolved_hostent = _xskt_gethostbyname(name);                           \
                                                                                            \
                    if ( !resolved_hostent )                                                \
                    {                                                                       \
                        addr_value_count = 0;                                               \
                    }                                                                       \
                    else if ( resolved_hostent->h_length < IPV4_ADDRESS_SIZE )              \
                    {                                                                       \
                        addr_value_count = 0;                                               \
                    }                                                                       \
                    else                                                                    \
                    {                                                                       \
                        while ( resolved_hostent->h_addr_list[resolved_count] &&            \
                                (resolved_count < addr_value_count) )                       \
                        {                                                                   \
                            addr_value_array[resolved_count] =                              \
                                *(PULONG)resolved_hostent->h_addr_list[resolved_count];     \
                            resolved_count++;                                               \
                        }                                                                   \
                                                                                            \
                        addr_value_count = resolved_count;                                  \
                    }                                                                       \
                }                                                                           \
             }

    #define  _xskt_en_broadcast(s)                                                          \
             {                                                                              \
                BOOL                    en_broadcast = TRUE;                                \
                int                     op_len       = sizeof(BOOL);                        \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_SOCKET,                                             \
                        XSKT_SOCKET_SO_BROADCAST,                                           \
                        (char*)&en_broadcast,                                               \
                        op_len                                                              \
                    );                                                                      \
             }

    #define  _xskt_en_multicast(s, m_addr, if_addr)                                         \
             {                                                                              \
                ansc_ip_mreq            temp_ip_mreq;                                       \
                                                                                            \
                temp_ip_mreq.imr_multiaddr.s_addr = m_addr;                                 \
                temp_ip_mreq.imr_interface.s_addr = XSKT_SOCKET_ANY_ADDRESS;                \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_IPPROTO_IP,                                         \
                        XSKT_SOCKET_IP_ADD_MEMBERSHIP,                                      \
                        (char*)&temp_ip_mreq,                                               \
                        sizeof(temp_ip_mreq)                                                \
                    );                                                                      \
             }

    #define  _xskt_en_multicast_loop(s)                                                     \
             {                                                                              \
                BOOL                    en_multicast_loop = TRUE;                           \
                int                     op_len            = sizeof(BOOL);                   \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_SOCKET,                                             \
                        XSKT_SOCKET_IP_MULTICAST_LOOP,                                      \
                        (char*)&en_multicast_loop,                                          \
                        op_len                                                              \
                    );                                                                      \
             }

    #define  _xskt_en_reuseaddr(s)                                                          \
             {                                                                              \
                BOOL                    en_reuseaddr = TRUE;                                \
                int                     op_len       = sizeof(BOOL);                        \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_SOCKET,                                             \
                        XSKT_SOCKET_SO_REUSEADDR,                                           \
                        (char*)&en_reuseaddr,                                               \
                        op_len                                                              \
                    );                                                                      \
             }

    #define  _xskt_en_usetls(s)                                                             \
             {                                                                              \
                BOOL                    en_usetls = TRUE;                                   \
                int                     op_len    = sizeof(BOOL);                           \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_IPPROTO_TCP,                                        \
                        XSKT_SOCKET_SO_TCP_USETLS,                                          \
                        (char*)&en_usetls,                                                  \
                        op_len                                                              \
                    );                                                                      \
             }

    #define  _xskt_en_tlsexportonly(s)                                                      \
             {                                                                              \
                BOOL                    en_tlsexportonly = TRUE;                            \
                int                     op_len           = sizeof(BOOL);                    \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_IPPROTO_TCP,                                        \
                        XSKT_SOCKET_SO_TCP_TLSEXPORTONLY,                                   \
                        (char*)&en_tlsexportonly,                                           \
                        op_len                                                              \
                    );                                                                      \
             }

    #define  _xskt_en_tlsstrongsec(s)                                                       \
             {                                                                              \
                BOOL                    en_tlsstrongsec = TRUE;                             \
                int                     op_len          = sizeof(BOOL);                     \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_IPPROTO_TCP,                                        \
                        XSKT_SOCKET_SO_TCP_TLSSTRONGSEC,                                    \
                        (char*)&en_tlsstrongsec,                                            \
                        op_len                                                              \
                    );                                                                      \
             }

    #define  _xskt_set_sndtimeo(s, sec, usec)                                               \
             {                                                                              \
                xskt_timeval            timeval;                                            \
                                                                                            \
                timeval.tv_sec  = sec;                                                      \
                timeval.tv_usec = usec;                                                     \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_SOCKET,                                             \
                        XSKT_SOCKET_SO_SNDTIMEO,                                            \
                        (char*)&timeval,                                                    \
                        sizeof(timeval)                                                     \
                    );                                                                      \
             }

    #define  _xskt_set_rcvtimeo(s, sec, usec)                                               \
             {                                                                              \
                xskt_timeval            timeval;                                            \
                                                                                            \
                timeval.tv_sec  = sec;                                                      \
                timeval.tv_usec = usec;                                                     \
                                                                                            \
                _xskt_setsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        XSKT_SOCKET_SOL_SOCKET,                                             \
                        XSKT_SOCKET_SO_RCVTIMEO,                                            \
                        (char*)&timeval,                                                    \
                        sizeof(timeval)                                                     \
                    );                                                                      \
             }

    #define  XSKT_SOCKET_FLAG_MSG_PEEK              MSG_PEEK
    #define  XSKT_SOCKET_FLAG_MSG_OOB               MSG_OOB

    #define  XSKT_SOCKET_AF_INET                    AF_INET
    #define  XSKT_SOCKET_STREAM                     SOCK_STREAM
    #define  XSKT_SOCKET_DGRAM                      SOCK_DGRAM
    #define  XSKT_SOCKET_RAWIP                      SOCK_RAW
    #define  XSKT_SOCKET_INVALID_SOCKET             INVALID_SOCKET
    #define  XSKT_SOCKET_ERROR                      SOCKET_ERROR
    #define  XSKT_SOCKET_ANY_ADDRESS                INADDR_ANY

    #define  XSKT_SOCKET_FD_SETSIZE                 FD_SETSIZE
    #define  XSKT_SOCKET_FD_CLR                     FD_CLR
    #define  XSKT_SOCKET_FD_ISSET                   FD_ISSET
    #define  XSKT_SOCKET_FD_SET                     FD_SET
    #define  XSKT_SOCKET_FD_ZERO                    FD_ZERO
    #define  XSKT_SOCKET_FD_ISNUL(set)              ( (((xskt_fd_set*)(set))->fd_count == 0) )
    #define  XSKT_SOCKET_FD_GET(set, s, i)                                                  \
             {                                                                              \
                xskt_fd_set*        socket_set = (xskt_fd_set*)set;                         \
                XSKT_SOCKET         socket_nul = XSKT_SOCKET_INVALID_SOCKET;                \
                                                                                            \
                s = ((u_int)i < socket_set->fd_count)? socket_set->fd_array[i] : socket_nul;\
             }

    #define  XSKT_SOCKET_SOL_SOCKET                 SOL_SOCKET
    #define  XSKT_SOCKET_SOL_IPPROTO_TCP            IPPROTO_TCP
    #define  XSKT_SOCKET_SOL_IPPROTO_IP             IPPROTO_IP
    #define  XSKT_SOCKET_SO_BROADCAST               SO_BROADCAST
    #define  XSKT_SOCKET_SO_REUSEADDR               SO_REUSEADDR
    #define  XSKT_SOCKET_SO_LASTRECVIF              0

    #define  XSKT_SOCKET_SD_RECV                    SD_RECEIVE
    #define  XSKT_SOCKET_SD_SEND                    SD_SEND
    #define  XSKT_SOCKET_SD_BOTH                    SD_BOTH

    #define  XSKT_SOCKET_SO_TCP_USETLS              0
    #define  XSKT_SOCKET_SO_TCP_TLSEXPORTONLY       0
    #define  XSKT_SOCKET_SO_TCP_TLSSTRONGSEC        0

    #define  XSKT_SOCKET_SO_ACCEPTCONN              SO_ACCEPTCONN
    #define  XSKT_SOCKET_SO_RCVLOWAT                SO_RCVLOWAT
    #define  XSKT_SOCKET_SO_RCVTIMEO                SO_RCVTIMEO
    #define  XSKT_SOCKET_SO_SNDLOWAT                SO_SNDLOWAT
    #define  XSKT_SOCKET_SO_SNDTIMEO                SO_SNDTIMEO
    #define  XSKT_SOCKET_SO_TYPE                    SO_TYPE

    #define  XSKT_SOCKET_IP_MULTICAST_TTL           IP_MULTICAST_TTL
    #define  XSKT_SOCKET_IP_MULTICAST_IF            IP_MULTICAST_IF
    #define  XSKT_SOCKET_IP_ADD_MEMBERSHIP          IP_ADD_MEMBERSHIP
    #define  XSKT_SOCKET_IP_DROP_MEMBERSHIP         IP_DROP_MEMBERSHIP
    #define  XSKT_SOCKET_IP_MULTICAST_LOOP          IP_MULTICAST_LOOP
    #define  XSKT_SOCKET_IP_RECV_UNICAST            0

#endif


#endif
