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

    module:	ansc_socket.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to socket operation. Instead
        of defining everything in a single file, we simply route
        every function and macro to the corresponding platform-
        dependent prototype definition.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/04/01    initial revision.
        10/06/01    isolate all platform-dependent definitions into
                    separate header files

**********************************************************************/


#ifndef  _ANSC_SOCKET_
#define  _ANSC_SOCKET_


/***********************************************************
    PLATFORM DEPENDENT DATA TYPE AND MACRO DEFINITIONS
***********************************************************/

/*
 * We define a list of socket error codes that are used frequently across TCP/IP protocol stack and
 * the socket library functions. Most error codes apply to both connection-oriented (TCP) and
 * connection-less (UDP, raw IP) sockets. Note that these error codes are not visible to any object
 * that is below the IP layer.
 */
#define  ANSC_SOCKET_ERROR_BASE                     0
#define  ANSC_SOCKET_ERROR_UNKNOWN                  1
#define  ANSC_SOCKET_ERROR_DST_UNREACHABLE          2
#define  ANSC_SOCKET_ERROR_RESOURCES                3
#define  ANSC_SOCKET_ERROR_BAD_SIZE                 4
#define  ANSC_SOCKET_ERROR_HARDWARE                 5
#define  ANSC_SOCKET_ERROR_BAD_FORMAT               6
#define  ANSC_SOCKET_ERROR_BAD_CHECKSUM             7
#define  ANSC_SOCKET_ERROR_NOT_SUPPORTED            8
#define  ANSC_SOCKET_ERROR_STACK_DOWN               9
#define  ANSC_SOCKET_ERROR_LIB_UNAVAILABLE          10

#define  ANSC_SOCKET_ERROR_BSD_BASE                 1000
#define  ANSC_SOCKET_ERROR_INVALID_SOCKET           ANSC_SOCKET_ERROR_BSD_BASE  + 1
#define  ANSC_SOCKET_ERROR_INVALID_AF               ANSC_SOCKET_ERROR_BSD_BASE  + 2
#define  ANSC_SOCKET_ERROR_INVALID_TYPE             ANSC_SOCKET_ERROR_BSD_BASE  + 3
#define  ANSC_SOCKET_ERROR_INVALID_PROTOCOL         ANSC_SOCKET_ERROR_BSD_BASE  + 4
#define  ANSC_SOCKET_ERROR_INVALID_CALL             ANSC_SOCKET_ERROR_BSD_BASE  + 5
#define  ANSC_SOCKET_ERROR_INVALID_ADDR             ANSC_SOCKET_ERROR_BSD_BASE  + 6
#define  ANSC_SOCKET_ERROR_INVALID_OPT              ANSC_SOCKET_ERROR_BSD_BASE  + 7
#define  ANSC_SOCKET_ERROR_INVALID_LEN              ANSC_SOCKET_ERROR_BSD_BASE  + 8
#define  ANSC_SOCKET_ERROR_INVALID_CMD              ANSC_SOCKET_ERROR_BSD_BASE  + 9
#define  ANSC_SOCKET_ERROR_INVALID_PARAM            ANSC_SOCKET_ERROR_BSD_BASE  + 10
#define  ANSC_SOCKET_ERROR_INVALID_SRC              ANSC_SOCKET_ERROR_BSD_BASE  + 11
#define  ANSC_SOCKET_ERROR_INVALID_NAME             ANSC_SOCKET_ERROR_BSD_BASE  + 12
#define  ANSC_SOCKET_ERROR_CLOSED                   ANSC_SOCKET_ERROR_BSD_BASE  + 13
#define  ANSC_SOCKET_ERROR_CLOSED_BYPEER            ANSC_SOCKET_ERROR_BSD_BASE  + 14
#define  ANSC_SOCKET_ERROR_WOULD_BLOCK              ANSC_SOCKET_ERROR_BSD_BASE  + 15
#define  ANSC_SOCKET_ERROR_EMPTY_FDSET              ANSC_SOCKET_ERROR_BSD_BASE  + 16
#define  ANSC_SOCKET_ERROR_TRUNCATED                ANSC_SOCKET_ERROR_BSD_BASE  + 17
#define  ANSC_SOCKET_ERROR_ADDR_INUSE               ANSC_SOCKET_ERROR_BSD_BASE  + 18
#define  ANSC_SOCKET_ERROR_CONN_RESET               ANSC_SOCKET_ERROR_BSD_BASE  + 19
#define  ANSC_SOCKET_ERROR_CONN_EXPIRED             ANSC_SOCKET_ERROR_BSD_BASE  + 20
#define  ANSC_SOCKET_ERROR_NO_INTERFACE             ANSC_SOCKET_ERROR_BSD_BASE  + 21
#define  ANSC_SOCKET_ERROR_CANNOT_REACH             ANSC_SOCKET_ERROR_BSD_BASE  + 22
#define  ANSC_SOCKET_ERROR_IF_REMOVED               ANSC_SOCKET_ERROR_BSD_BASE  + 23
#define  ANSC_SOCKET_ERROR_SHUTDOWN                 ANSC_SOCKET_ERROR_BSD_BASE  + 24
#define  ANSC_SOCKET_ERROR_TRY_AGAIN                ANSC_SOCKET_ERROR_BSD_BASE  + 25
#define  ANSC_SOCKET_ERROR_SERVER_ERROR             ANSC_SOCKET_ERROR_BSD_BASE  + 26
#define  ANSC_SOCKET_ERROR_TLS_FAILURE              ANSC_SOCKET_ERROR_BSD_BASE  + 27

/*
 * For objects associated with the Tcp sockets, the send operation can be completed without passing
 * any address information explicitly since the socket remembers peer's information. However, this
 * may not be the case for Udp and Raw sockets. Following data structures must be explicitly passed
 * into the socket_send() member function when it's called.
 */
typedef  struct
_ANSC_SOCKET_ADDRESS
{
    ANSC_IPV4_ADDRESS               Address;
    USHORT                          Port;
}
ANSC_SOCKET_ADDRESS,  *PANSC_SOCKET_ADDRESS;

/*
 * Resolving a domain name to an IP address is always pain-in-the-ass on BSD-compatible socket API
 * since no asychronous operation is available. We use following macro to simply the synchronous
 * name-resolution call.
 */
#define  AnscResolveHostName(name, address)                                                 \
         {                                                                                  \
            ansc_hostent*           resolved_hostent = NULL;                                \
                                                                                            \
            if ( AnscIsValidIpString(name) )                                                \
            {                                                                               \
                *(PULONG)address = _ansc_inet_addr(name);                                   \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                resolved_hostent = _ansc_gethostbyname(name);                               \
                                                                                            \
                if ( !resolved_hostent )                                                    \
                {                                                                           \
                    *(PULONG)address = 0;                                                   \
                }                                                                           \
                else if ( resolved_hostent->h_length < IPV4_ADDRESS_SIZE )                  \
                {                                                                           \
                    *(PULONG)address = 0;                                                   \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    *(PULONG)address = *(PULONG)resolved_hostent->h_addr;                   \
                }                                                                           \
            }                                                                               \
         }

#define  AnscResolveHostName2(name, addr_value_array, addr_value_count)                     \
         {                                                                                  \
            ansc_hostent*           resolved_hostent = NULL;                                \
            ULONG                   resolved_count   = 0;                                   \
                                                                                            \
            if ( AnscIsValidIpString(name) )                                                \
            {                                                                               \
                addr_value_array[0] = _ansc_inet_addr(name);                                \
                addr_value_count    = 1;                                                    \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                resolved_hostent = _ansc_gethostbyname(name);                               \
                                                                                            \
                if ( !resolved_hostent )                                                    \
                {                                                                           \
                    addr_value_count = 0;                                                   \
                }                                                                           \
                else if ( resolved_hostent->h_length < IPV4_ADDRESS_SIZE )                  \
                {                                                                           \
                    addr_value_count = 0;                                                   \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    while ( resolved_hostent->h_addr_list[resolved_count] &&                \
                            (resolved_count < addr_value_count) )                           \
                    {                                                                       \
                        addr_value_array[resolved_count] =                                  \
                            *(PULONG)resolved_hostent->h_addr_list[resolved_count];         \
                        resolved_count++;                                                   \
                    }                                                                       \
                                                                                            \
                    addr_value_count = resolved_count;                                      \
                }                                                                           \
            }                                                                               \
         }

/*
#define  AnscGetIpAddrString(address, addr_string)                                          \
         {                                                                                  \
            char*                   temp_string;                                            \
            ansc_in_addr            temp_addr;                                              \
            ULONG                   null_index;                                             \
                                                                                            \
            temp_addr.s_addr = *(PULONG)address;                                            \
            temp_string      = _ansc_inet_ntoa(temp_addr);                                  \
                                                                                            \
            AnscCopyString(addr_string, temp_string);                                       \
                                                                                            \
            null_index              = AnscSizeOfString(temp_string);                        \
            addr_string[null_index] = 0;                                                    \
         }
*/
#define  AnscGetIpAddrString(address, addr_string)                                          \
         {                                                                                  \
            ULONG                   kk         = 0;                                         \
            char*                   tmp_string = addr_string;                               \
            ANSC_IPV4_ADDRESS       tmp_address;                                            \
                                                                                            \
            tmp_address.Value = *(PULONG)address;                                           \
                                                                                            \
            for ( kk = 0; kk < IPV4_ADDRESS_SIZE; kk++ )                                    \
            {                                                                               \
                AnscGetUlongString((ULONG)tmp_address.Dot[kk], tmp_string);                 \
                                                                                            \
                tmp_string += AnscSizeOfString(tmp_string);                                 \
                                                                                            \
                if ( kk < (IPV4_ADDRESS_SIZE - 1) )                                         \
                {                                                                           \
                    *tmp_string = '.';                                                      \
                                                                                            \
                    tmp_string++;                                                           \
                }                                                                           \
            }                                                                               \
                                                                                            \
            *tmp_string = 0;                                                                \
         }

#define  AnscGetLocalHostAddress(address)                                                   \
         {                                                                                  \
            ansc_hostent*           resolved_hostent    = NULL;                             \
            ULONG                   local_host_name_len = 128;                              \
            char                    local_host_name[128];                                   \
            int                     error_code          = 0;                                \
                                                                                            \
            error_code = _ansc_gethostname(local_host_name, local_host_name_len);           \
                                                                                            \
            if ( error_code != 0 )                                                          \
            {                                                                               \
                *(PULONG)address = 0;                                                       \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                resolved_hostent = _ansc_gethostbyname(local_host_name);                    \
                                                                                            \
                if ( !resolved_hostent )                                                    \
                {                                                                           \
                    *(PULONG)address = 0;                                                   \
                }                                                                           \
                else if ( resolved_hostent->h_length < IPV4_ADDRESS_SIZE )                  \
                {                                                                           \
                    *(PULONG)address = 0;                                                   \
                }                                                                           \
                else                                                                        \
                {                                                                           \
                    *(PULONG)address = *(PULONG)resolved_hostent->h_addr;                   \
                }                                                                           \
            }                                                                               \
         }

/*
 * Following socket option-manipulation macros were originally defined in user_socket.h provided by
 * different operating system wrapper. We move them here since they're merely using well-known bsd
 * socket interface to control the socket, no OS-specific operation is involved.
 */
#define  _ansc_en_broadcast(s)                                                              \
         {                                                                                  \
            int                     en_broadcast = (int)-1;                                 \
            int                     op_len       = sizeof(en_broadcast);                    \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_SO_BROADCAST,                                               \
                    (char*)&en_broadcast,                                                   \
                    op_len                                                                  \
                );                                                                          \
         }

#define  _ansc_en_multicast(s, m_addr, if_addr)                                             \
         {                                                                                  \
            ansc_ip_mreq            temp_ip_mreq;                                           \
                                                                                            \
            temp_ip_mreq.imr_multiaddr.s_addr = m_addr;                                     \
            temp_ip_mreq.imr_interface.s_addr = if_addr;                                    \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_IPPROTO_IP,                                             \
                    ANSC_SOCKET_IP_ADD_MEMBERSHIP,                                          \
                    (char*)&temp_ip_mreq,                                                   \
                    sizeof(temp_ip_mreq)                                                    \
                );                                                                          \
         }

#define  _ansc_en_multicast_loop(s)                                                         \
         {                                                                                  \
            int                     en_multicast_loop = (int)-1;                            \
            int                     op_len            = sizeof(en_multicast_loop);          \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_IP_MULTICAST_LOOP,                                          \
                    (char*)&en_multicast_loop,                                              \
                    op_len                                                                  \
                );                                                                          \
         }

#define  _ansc_en_reuseaddr(s)                                                              \
         {                                                                                  \
            int                     en_reuseaddr = 1;                                       \
            int                     op_len       = sizeof(en_reuseaddr);                    \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_SO_REUSEADDR,                                               \
                    (char*)&en_reuseaddr,                                                   \
                    op_len                                                                  \
                );                                                                          \
         }

#define  _ansc_get_lastrecvifaddr(s, if_addr)                                               \
         {                                                                                  \
            ULONG                   recv_if_addr   = 0;                                     \
            int                     op_len         = sizeof(ULONG);                         \
            int                     i_return_value = 0;                                     \
                                                                                            \
            i_return_value =                                                                \
                _ansc_getsocketopt                                                          \
                    (                                                                       \
                        s,                                                                  \
                        ANSC_SOCKET_SOL_SOCKET,                                             \
                        ANSC_SOCKET_SO_LASTRECVIF,                                          \
                        (char*)&recv_if_addr,                                               \
                        &op_len                                                             \
                    );                                                                      \
                                                                                            \
            if_addr = (i_return_value == 0)? recv_if_addr : 0;                              \
         }

#define  _ansc_set_ip4tos(s, ip4_tos)                                                       \
         {                                                                                  \
            int                     op_len = sizeof(UCHAR);                                 \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_SO_TOS,                                                     \
                    (char*)&ip4_tos,                                                        \
                    op_len                                                                  \
                );                                                                          \
         }

#define  _ansc_set_ip4ttl(s, ip4_ttl)                                                       \
         {                                                                                  \
            int                     op_len = sizeof(UCHAR);                                 \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_SO_TTL,                                                     \
                    (char*)&ip4_ttl,                                                        \
                    op_len                                                                  \
                );                                                                          \
         }

#define  _ansc_en_usetls(s)                                                                 \
         {                                                                                  \
            BOOL                    en_usetls = TRUE;                                       \
            int                     op_len    = sizeof(BOOL);                               \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_IPPROTO_TCP,                                            \
                    ANSC_SOCKET_SO_TCP_USETLS,                                              \
                    (char*)&en_usetls,                                                      \
                    op_len                                                                  \
                );                                                                          \
         }

#define  _ansc_en_tlsexportonly(s)                                                          \
         {                                                                                  \
            BOOL                    en_tlsexportonly = TRUE;                                \
            int                     op_len           = sizeof(BOOL);                        \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_IPPROTO_TCP,                                            \
                    ANSC_SOCKET_SO_TCP_TLSEXPORTONLY,                                       \
                    (char*)&en_tlsexportonly,                                               \
                    op_len                                                                  \
                );                                                                          \
         }

#define  _ansc_en_tlsstrongsec(s)                                                           \
         {                                                                                  \
            BOOL                    en_tlsstrongsec = TRUE;                                 \
            int                     op_len          = sizeof(BOOL);                         \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_IPPROTO_TCP,                                            \
                    ANSC_SOCKET_SO_TCP_TLSSTRONGSEC,                                        \
                    (char*)&en_tlsstrongsec,                                                \
                    op_len                                                                  \
                );                                                                          \
         }


#define  _ansc_set_sndtimeo(s, sec, usec)                                                   \
         {                                                                                  \
            ansc_timeval            timeval;                                                \
                                                                                            \
            timeval.tv_sec  = sec;                                                          \
            timeval.tv_usec = usec;                                                         \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_SO_SNDTIMEO,                                                \
                    (char*)&timeval,                                                        \
                    sizeof(timeval)                                                         \
                );                                                                          \
         }

#define  _ansc_set_rcvtimeo(s, sec, usec)                                                   \
         {                                                                                  \
            ansc_timeval            timeval;                                                \
                                                                                            \
            timeval.tv_sec  = sec;                                                          \
            timeval.tv_usec = usec;                                                         \
                                                                                            \
            _ansc_setsocketopt                                                              \
                (                                                                           \
                    s,                                                                      \
                    ANSC_SOCKET_SOL_SOCKET,                                                 \
                    ANSC_SOCKET_SO_RCVTIMEO,                                                \
                    (char*)&timeval,                                                        \
                    sizeof(timeval)                                                         \
                );                                                                          \
         }

#define AnscStartupSocketWrapper(hOwnerContext)

#define AnscCleanupSocketWrapper(hOwnerContext)


/***********************************************************
        ANSC SOCKET LIBRARY FUNCTION API DEFINITIONS
***********************************************************/

/*
 * BroadWay Socket Service is an interface, not a protocol. As an interface, it is used to discover
 * and utilize the communications capabilities of any number of underlying transport protocols.
 * Because it is not a protocol, it does not in any way affect the bits on the wire, and does not
 * need to be utilized on both ends of a communications link. If you're familiar with Berkeley
 * Software Distribution (BSD) UNIX, you will find that the BroadWay Socket interface is very easy
 * to work with. Its programming is also centered around TCP/IP.
 */
#ifdef  _ANSC_SOCKET_LIBRARY_

    #include "bss_definitions.h"

    typedef             BSS_BSD_SOCKET          ANSC_SOCKET,            *PANSC_SOCKET;
    typedef  struct     _BSS_BSD_INADDR         ansc_in_addr,           *pansc_in_addr;
    typedef  struct     _BSS_BSD_SOCKADDR_IN    ansc_socket_addr_in,    *pansc_socket_addr_in;
    typedef  struct     _BSS_BSD_SOCKADDR       ansc_socket_addr,       *pansc_socket_addr;
    typedef  struct     _BSS_BSD_FDSET          ansc_fd_set,            *pansc_fd_set;
    typedef  struct     _BSS_BSD_TIMEVAL        ansc_timeval,           *pansc_timeval;
    typedef  struct     _BSS_BSD_HOSTENT        ansc_hostent,           *pansc_hostent;
    typedef  struct     _BSS_BSD_IP_MREQ        ansc_ip_mreq,           *pansc_ip_mreq;

    #define  _ansc_accept                           AnscSocketLibAccept
    #define  _ansc_bind                             AnscSocketLibBind
    #define  _ansc_closesocket                      AnscSocketLibCloseSocket
    #define  _ansc_connect                          AnscSocketLibConnect
    #define  _ansc_gethostbyaddr                    AnscSocketLibGetHostByAddr
    #define  _ansc_gethostbyname                    AnscSocketLibGetHostByName
    #define  _ansc_gethostname                      AnscSocketLibGetHostName
    #define  _ansc_getpeername                      AnscSocketLibGetPeerName
    #define  _ansc_getsocketname                    AnscSocketLibGetSockName
    #define  _ansc_getsocketopt                     AnscSocketLibGetSockOpt
    #define  _ansc_htonl                            AnscSocketLibHtonl
    #define  _ansc_htons                            AnscSocketLibHtons
    #define  _ansc_inet_addr                        AnscSocketLibInetAddr
    #define  _ansc_inet_ntoa                        AnscSocketLibInetNtoa
    #define  _ansc_ioctlsocket                      AnscSocketLibIoctlSocket
    #define  _ansc_listen                           AnscSocketLibListen
    #define  _ansc_ntohl                            AnscSocketLibNtohl
    #define  _ansc_ntohs                            AnscSocketLibNtohs
    #define  _ansc_recv                             AnscSocketLibRecv
    #define  _ansc_recvfrom                         AnscSocketLibRecvFrom
    #define  _ansc_select                           AnscSocketLibSelect
    #define  _ansc_send                             AnscSocketLibSend
    #define  _ansc_sendto                           AnscSocketLibSendTo
    #define  _ansc_setsocketopt                     AnscSocketLibSetSockOpt
    #define  _ansc_shutdown                         AnscSocketLibShutdown
    #define  _ansc_socket                           AnscSocketLibSocket
    #define  _ansc_get_last_error                   AnscSocketLibGetLastError

    #define  ANSC_SOCKET_FLAG_MSG_PEEK              BSS_BSD_FLAG_MSG_PEEK
    #define  ANSC_SOCKET_FLAG_MSG_OOB               BSS_BSD_FLAG_MSG_OOB

    #define  ANSC_SOCKET_AF_INET                    BSS_BSD_AF_INET
    #define  ANSC_SOCKET_STREAM                     BSS_BSD_SOCK_STREAM
    #define  ANSC_SOCKET_DGRAM                      BSS_BSD_SOCK_DGRAM
    #define  ANSC_SOCKET_RAWIP                      BSS_BSD_SOCK_RAW
    #define  ANSC_SOCKET_INVALID_SOCKET             BSS_BSD_SOCKET_INVALID_SOCKET
    #define  ANSC_SOCKET_ERROR                      BSS_BSD_SOCKET_ERROR
    #define  ANSC_SOCKET_ANY_ADDRESS                BSS_BSD_INADDR_ANY

    #define  ANSC_SOCKET_FD_SETSIZE                 BSS_BSD_FD_SETSIZE
    #define  ANSC_SOCKET_FD_CLR                     BSS_BSD_FD_CLR
    #define  ANSC_SOCKET_FD_ISSET                   BSS_BSD_FD_ISSET
    #define  ANSC_SOCKET_FD_SET                     BSS_BSD_FD_SET
    #define  ANSC_SOCKET_FD_ZERO                    BSS_BSD_FD_ZERO
    #define  ANSC_SOCKET_FD_ISNUL                   BSS_BSD_FD_ISNUL
    #define  ANSC_SOCKET_FD_GET                     BSS_BSD_FD_GET

    #define  ANSC_SOCKET_SOL_SOCKET                 BSS_BSD_SOL_SOCKET
    #define  ANSC_SOCKET_SOL_IPPROTO_TCP            BSS_BSD_IPPROTO_TCP
    #define  ANSC_SOCKET_SOL_IPPROTO_IP             BSS_BSD_IPPROTO_IP
    #define  ANSC_SOCKET_SO_BROADCAST               BSS_BSD_SO_ALL_BROADCAST
    #define  ANSC_SOCKET_SO_REUSEADDR               BSS_BSD_SO_ALL_REUSEADDR
    #define  ANSC_SOCKET_SO_LASTRECVIF              BSS_BSD_SO_ALL_LASTRECVIF
    #define  ANSC_SOCKET_SO_TOS                     BSS_BSD_SO_ALL_TOS
    #define  ANSC_SOCKET_SO_TTL                     BSS_BSD_SO_ALL_TTL

    #define  ANSC_SOCKET_SD_RECV                    BSS_BSD_SD_RECV
    #define  ANSC_SOCKET_SD_SEND                    BSS_BSD_SD_SEND
    #define  ANSC_SOCKET_SD_BOTH                    BSS_BSD_SD_BOTH

    #define  ANSC_SOCKET_SO_TCP_USETLS              BSS_BSD_SO_TCP_USETLS
    #define  ANSC_SOCKET_SO_TCP_TLSEXPORTONLY       BSS_BSD_SO_TCP_TLSEXPORTONLY
    #define  ANSC_SOCKET_SO_TCP_TLSSTRONGSEC        BSS_BSD_SO_TCP_TLSSTRONGSEC

    #define  ANSC_SOCKET_SO_ACCEPTCONN              BSS_BSD_SO_ACCEPTCONN
    #define  ANSC_SOCKET_SO_RCVLOWAT                BSS_BSD_SO_RCVLOWAT
    #define  ANSC_SOCKET_SO_RCVTIMEO                BSS_BSD_SO_RCVTIMEO
    #define  ANSC_SOCKET_SO_SNDLOWAT                BSS_BSD_SO_SNDLOWAT
    #define  ANSC_SOCKET_SO_SNDTIMEO                BSS_BSD_SO_SNDTIMEO
    #define  ANSC_SOCKET_SO_TYPE                    BSS_BSD_SO_TYPE

    #define  ANSC_SOCKET_IP_MULTICAST_TTL           BSS_BSD_SO_IP4_MULTICAST_TTL
    #define  ANSC_SOCKET_IP_MULTICAST_IF            BSS_BSD_SO_IP4_MULTICAST_IF
    #define  ANSC_SOCKET_IP_ADD_MEMBERSHIP          BSS_BSD_SO_IP4_ADD_MEMBERSHIP
    #define  ANSC_SOCKET_IP_DROP_MEMBERSHIP         BSS_BSD_SO_IP4_DROP_MEMBERSHIP
    #define  ANSC_SOCKET_IP_MULTICAST_LOOP          BSS_BSD_SO_IP4_MULTICAST_LOOP
    #define  ANSC_SOCKET_IP_RECV_UNICAST            BSS_BSD_SO_IP4_RECV_UNICAST

#include "ansc_socket_interface.h"
#endif

#define  AnscSocketLibHtonl                         AnscUlongFromHToN
#define  AnscSocketLibHtons                         AnscUshortFromHToN
#define  AnscSocketLibNtohl                         AnscUlongFromNToH
#define  AnscSocketLibNtohs                         AnscUshortFromNToH

/*
 * The base ansc_socket object was originally defined in this header file. We have separated the
 * object definition from the base data structures and compiling options, and put them into a
 * separate header file. But there're many existing ansc objects have already been compiled with
 * the old file structure, so we include the interface header file here.
 */
#include "ansc_xsocket_interface.h"


#endif
