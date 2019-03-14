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

    module:	bss_definitions.h

        For BroadWay Socket Service Implementation (BSS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the de facto BSD-compatible socket data
        data structures and some of our own extensions.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/01/02    initial revision.

**********************************************************************/


#ifndef  _BSS_DEFINITIONS_
#define  _BSS_DEFINITIONS_


/***********************************************************
           BSS BSD SOCKET PROTOCOL TYPE DEFINITION
***********************************************************/

/*
 * Actually we have defined all the transport types in separate header files. However, to show the
 * heart-warming spirit of being BSD-socket compatible, we redefine them here.
 */
#define  BSS_BSD_IPPROTO_IP                         0               /* dummy for IP                       */
#define  BSS_BSD_IPPROTO_ICMP                       1               /* control message protocol           */
#define  BSS_BSD_IPPROTO_IGMP                       2               /* internet group management protocol */
#define  BSS_BSD_IPPROTO_GGP                        3               /* gateway^2 (deprecated)             */
#define  BSS_BSD_IPPROTO_TCP                        6               /* tcp                                */
#define  BSS_BSD_IPPROTO_PUP                        12              /* pup                                */
#define  BSS_BSD_IPPROTO_UDP                        17              /* user datagram protocol             */
#define  BSS_BSD_IPPROTO_IDP                        22              /* xns idp                            */
#define  BSS_BSD_IPPROTO_ND                         77              /* UNOFFICIAL net disk proto          */
#define  BSS_BSD_IPPROTO_RAW                        255             /* raw IP packet                      */
#define  BSS_BSD_IPPROTO_MAX                        256

/*
 * Actually we have defined all the transport ports in separate header files. However, to show the
 * heart-warming spirit of being BSD-socket compatible, we redefine them here.
 */
#define  BSS_BSD_IPPORT_ECHO                        7
#define  BSS_BSD_IPPORT_DISCARD                     9
#define  BSS_BSD_IPPORT_SYSTAT                      11
#define  BSS_BSD_IPPORT_DAYTIME                     13
#define  BSS_BSD_IPPORT_NETSTAT                     15
#define  BSS_BSD_IPPORT_FTP                         21
#define  BSS_BSD_IPPORT_TELNET                      23
#define  BSS_BSD_IPPORT_SMTP                        25
#define  BSS_BSD_IPPORT_TIMESERVER                  37
#define  BSS_BSD_IPPORT_NAMESERVER                  42
#define  BSS_BSD_IPPORT_WHOIS                       43
#define  BSS_BSD_IPPORT_MTP                         57

#define  BSS_BSD_IPPORT_TFTP                        69
#define  BSS_BSD_IPPORT_RJE                         77
#define  BSS_BSD_IPPORT_FINGER                      79
#define  BSS_BSD_IPPORT_TTYLINK                     87
#define  BSS_BSD_IPPORT_SUPDUP                      95

#define  BSS_BSD_IPPORT_EXECSERVER                  512
#define  BSS_BSD_IPPORT_LOGINSERVER                 513
#define  BSS_BSD_IPPORT_CMDSERVER                   514
#define  BSS_BSD_IPPORT_EFSSERVER                   520

#define  BSS_BSD_IPPORT_BIFFUDP                     512
#define  BSS_BSD_IPPORT_WHOSERVER                   513
#define  BSS_BSD_IPPORT_ROUTESERVER                 520

#define  BSS_BSD_IPPORT_RESERVED                    1024


/***********************************************************
           BSS BSD SOCKET ADDRESS FAMILY DEFINITION
***********************************************************/

/*
 * "Address Family" is used to select the underlying network-layer protocol when opening a socket,
 * which in my opinion should be renamed to something has better and clearer meaning. Although
 * AF_UNSPEC is defined for backwards compatibility, using AF_UNSPEC for the "af" parameter when
 * creating a socket is STRONGLY DISCOURAGED. The interpretation of the  "protocol"  parameter
 * depends on the actual address family chosen. As environments grow to include more and more
 * address families that use overlapping protocol values there is more and more chance of choosing
 * an undesired address family when AF_UNSPEC is used.
 */
#define  BSS_BSD_AF_UNSPEC                          0               /* unspecified                     */
#define  BSS_BSD_AF_UNIX                            1               /* local to host (pipes, portals)  */
#define  BSS_BSD_AF_INET                            2               /* internetwork: UDP, TCP, etc.    */
#define  BSS_BSD_AF_IMPLINK                         3               /* arpanet imp addresses           */
#define  BSS_BSD_AF_PUP                             4               /* pup protocols: e.g. BSP         */
#define  BSS_BSD_AF_CHAOS                           5               /* mit CHAOS protocols             */
#define  BSS_BSD_AF_NS                              6               /* XEROX NS protocols              */
#define  BSS_BSD_AF_IPX                             BSS_BSD_AF_NS   /* IPX protocols: IPX, SPX, etc.   */
#define  BSS_BSD_AF_ISO                             7               /* ISO protocols                   */
#define  BSS_BSD_AF_OSI                             BSS_BSD_AF_ISO  /* OSI is ISO                      */
#define  BSS_BSD_AF_ECMA                            8               /* european computer manufacturers */
#define  BSS_BSD_AF_DATAKIT                         9               /* datakit protocols               */
#define  BSS_BSD_AF_CCITT                           10              /* CCITT protocols, X.25 etc       */
#define  BSS_BSD_AF_SNA                             11              /* IBM SNA                         */
#define  BSS_BSD_AF_DECnet                          12              /* DECnet                          */
#define  BSS_BSD_AF_DLI                             13              /* Direct data link interface      */
#define  BSS_BSD_AF_LAT                             14              /* LAT                             */
#define  BSS_BSD_AF_HYLINK                          15              /* NSC Hyperchannel                */
#define  BSS_BSD_AF_APPLETALK                       16              /* AppleTalk                       */
#define  BSS_BSD_AF_NETBIOS                         17              /* NetBios-style addresses         */
#define  BSS_BSD_AF_VOICEVIEW                       18              /* VoiceView                       */
#define  BSS_BSD_AF_FIREFOX                         19              /* Protocols from Firefox          */
#define  BSS_BSD_AF_UNKNOWN1                        20              /* Somebody is using this!         */
#define  BSS_BSD_AF_BAN                             21              /* Banyan                          */
#define  BSS_BSD_AF_ATM                             22              /* Native ATM Services             */
#define  BSS_BSD_AF_INET6                           23              /* Internetwork Version 6          */
#define  BSS_BSD_AF_CLUSTER                         24              /* Microsoft Wolfpack              */
#define  BSS_BSD_AF_12844                           25              /* IEEE 1284.4 WG AF               */

#define  BSS_BSD_AF_MAX                             26


/***********************************************************
              BSS BSD SOCKET OPTION DEFINITION
***********************************************************/

/*
 * Socket "option level" is a pretty stupid concept: options SHOULD have been defined for each
 * individual transport protocol (i.e., TCP, UDP ... and so forth) instead of sharing some of the
 * definitions while using the "level" parameter to differentiate the targets.
 */
#define  BSS_BSD_SOL_SOCKET                         BSS_BSD_SOL_ALL
#define  BSS_BSD_SOL_ALL                            0xFFFF          /* options for general socket level */
#define  BSS_BSD_SOL_TCP                            TCP_TRANSPORT   /* options for tcp socket level     */
#define  BSS_BSD_SOL_UDP                            UDP_TRANSPORT   /* options for udp socket level     */
#define  BSS_BSD_SOL_IP4                            IP4_TRANSPORT   /* options for raw socket level     */

/*
 * Socket options are used by Ansc Socket applications to customize the behavior of the protocol-
 * specific socket when the default settings don't satisfy the requirement.
 */
#define  BSS_BSD_SO_ALL_DEBUG                       0x0001          /* turn on debugging info recording */
#define  BSS_BSD_SO_ALL_ACCEPTCONN                  0x0002          /* socket has had listen()          */
#define  BSS_BSD_SO_ALL_REUSEADDR                   0x0003          /* allow local address reuse        */
#define  BSS_BSD_SO_ALL_KEEPALIVE                   0x0004          /* keep connections alive           */
#define  BSS_BSD_SO_ALL_BROADCAST                   0x0005          /* permit sending of broadcast msgs */
#define  BSS_BSD_SO_ALL_LINGER                      0x0006          /* linger on close if data present  */
#define  BSS_BSD_SO_ALL_DONTLINGER                  0x0007          /* don't linger on close regardless */
#define  BSS_BSD_SO_ALL_OOBINLINE                   0x0008          /* leave received OOB data in line  */
#define  BSS_BSD_SO_ALL_MAXMSGSIZE                  0x0009          /* maximum outbound message size    */
#define  BSS_BSD_SO_ALL_ADDRTYPE                    0x000A          /* address type: local, proxy ...   */
#define  BSS_BSD_SO_ALL_PROXYIF                     0x000B          /* local if_addr for proxy binding  */
#define  BSS_BSD_SO_ALL_LASTRECVIF                  0x000C          /* local if received last packet    */
#define  BSS_BSD_SO_ALL_TOS                         0x000D          /* 'tos' value in local-out packet  */
#define  BSS_BSD_SO_ALL_TTL                         0x000E          /* 'ttl' value in local-out packet  */

#define  BSS_BSD_SO_ALL_SNDBUF                      0x1001          /* send buffer size                 */
#define  BSS_BSD_SO_ALL_RCVBUF                      0x1002          /* receive buffer size              */
#define  BSS_BSD_SO_ALL_SNDLOWAT                    0x1003          /* send low-water mark              */
#define  BSS_BSD_SO_ALL_RCVLOWAT                    0x1004          /* receive low-water mark           */
#define  BSS_BSD_SO_ALL_SNDTIMEO                    0x1005          /* send timeout                     */
#define  BSS_BSD_SO_ALL_RCVTIMEO                    0x1006          /* receive timeout                  */
#define  BSS_BSD_SO_ALL_ERROR                       0x1007          /* get error status and clear       */
#define  BSS_BSD_SO_ALL_TYPE                        0x1008          /* get socket type                  */

#define  BSS_BSD_SO_ACCEPTCONN                      0x1101          /* socket is listening              */
#define  BSS_BSD_SO_RCVLOWAT                        0x1102          /* receives low watermark           */
#define  BSS_BSD_SO_RCVTIMEO                        0x1103          /* receives time-out                */
#define  BSS_BSD_SO_SNDLOWAT                        0x1104          /* sends low watermark              */
#define  BSS_BSD_SO_SNDTIMEO                        0x1105          /* sends time-out                   */
#define  BSS_BSD_SO_TYPE                            0x1106          /* type of the socket               */

#define  BSS_BSD_SO_TCP_NODELAY                     0x2001          /* delivering messages immediately  */

#define  BSS_BSD_SO_IP4_ADD_MEMBERSHIP              0x3001          /* join a multicast group           */
#define  BSS_BSD_SO_IP4_DROP_MEMBERSHIP             0x3002          /* leave a multicast group          */
#define  BSS_BSD_SO_IP4_MULTICAST_IF                0x3003          /* set the default mcast interface  */
#define  BSS_BSD_SO_IP4_MULTICAST_TTL               0x3004          /* set the default mcast ttl        */
#define  BSS_BSD_SO_IP4_MULTICAST_LOOP              0x3005          /* turn on/off for mcast loopback   */
#define  BSS_BSD_SO_IP4_RECV_UNICAST                0x3101          /* receive unicast on mcast socket  */

#define  BSS_BSD_SO_TCP_USETLS                      0x4001          /* enable TLS/SSL over TCP          */
#define  BSS_BSD_SO_TCP_TLSEXPORTONLY               0x4002          /* only use exportable TLS ciphers  */
#define  BSS_BSD_SO_TCP_TLSSTRONGSEC                0x4003          /* only use strongest TLS ciphers   */


/***********************************************************
          BSS BSD SOCKET I/O CONTROL CODE DEFINITION
***********************************************************/

/*
 * The Ansc socket applications can utilize bsd_ioctlsocket function on any socket in any state. It
 * is used to set or retrieve operating parameters associated with the socket, independent of the
 * protocol and communications subsystem. Here are the supported commands to use for I/O control.
 */
#define  BSS_BSD_FIONBIO                            1
#define  BSS_BSD_FIONREAD                           2
#define  BSS_BSD_SIOCATMARK                         3


/***********************************************************
       BSS BSD SOCKET BEHAVIOR CONTROL FLAG DEFINITION
***********************************************************/

/*
 * Certain socket function calls offer another way for socket applications to control the socket's
 * behavior in addition to the "socket options" and "ioctl codes". These functions usually take a
 * "flags" parameter which can be used to influence the behavior of the function invocation beyond
 * the options specified for the associated socket. The semantics of this function are determined
 * by the socket options and the flags parameter. The latter is constructed by using the bitwise OR
 * operator with any of the following values.
 */
#define  BSS_BSD_FLAG_MSG_PEEK                      0x00000001      /* peeks at the incoming data  */
#define  BSS_BSD_FLAG_MSG_OOB                       0x00000002      /* processes the oob data      */
#define  BSS_BSD_FLAG_MSG_DONTROUTE                 0x00000004      /* not subject to routing      */

#define  BSS_BSD_FLAG_SDH_SEND                      0x00000008      /* disable send after shutdown */
#define  BSS_BSD_FLAG_SDH_RECV                      0x00000010      /* disable recv after shutdown */
#define  BSS_BSD_FLAG_SDH_BOTH                      0x00000018      /* disable both after shutdown */

#define  BSS_BSD_SD_SEND                            BSS_BSD_FLAG_SDH_SEND
#define  BSS_BSD_SD_RECV                            BSS_BSD_FLAG_SDH_RECV
#define  BSS_BSD_SD_BOTH                            BSS_BSD_FLAG_SDH_BOTH


/***********************************************************
          BSS BSD POPULAR INTERNET ADDRESS DEFINITION
***********************************************************/

/*
 * Definitions of bits in internet address integers. On subnets, the decomposition of addresses to
 * host and net parts is done according to subnet mask, not the masks here.
 */
#define  BSS_BSD_IN_CLASSA(i)                       ( ((long)(i) & 0x80000000) == 0 )
#define  BSS_BSD_IN_CLASSA_NET                      0xFF000000
#define  BSS_BSD_IN_CLASSA_NSHIFT                   24
#define  BSS_BSD_IN_CLASSA_HOST                     0x00FFFFFF
#define  BSS_BSD_IN_CLASSA_MAX                      128

#define  BSS_BSD_IN_CLASSB(i)                       ( ((long)(i) & 0xC0000000) == 0x80000000 )
#define  BSS_BSD_IN_CLASSB_NET                      0xFFFF0000
#define  BSS_BSD_IN_CLASSB_NSHIFT                   16
#define  BSS_BSD_IN_CLASSB_HOST                     0x0000FFFF
#define  BSS_BSD_IN_CLASSB_MAX                      65536

#define  BSS_BSD_IN_CLASSC(i)                       ( ((long)(i) & 0xE0000000) == 0xc0000000 )
#define  BSS_BSD_IN_CLASSC_NET                      0xFFFFFF00
#define  BSS_BSD_IN_CLASSC_NSHIFT                   8
#define  BSS_BSD_IN_CLASSC_HOST                     0x000000FF

#define  BSS_BSD_IN_CLASSD(i)                       ( ((long)(i) & 0xF0000000) == 0xE0000000 )
#define  BSS_BSD_IN_CLASSD_NET                      0xF0000000       /* These ones aren't really */
#define  BSS_BSD_IN_CLASSD_NSHIFT                   28               /* net and host fields, but */
#define  BSS_BSD_IN_CLASSD_HOST                     0x0FFFFFFF       /* routing needn't know.    */
#define  BSS_BSD_IN_MULTICAST(i)                    IN_CLASSD(i)

#define  BSS_BSD_INADDR_ANY                         (ULONG)0x00000000
#define  BSS_BSD_INADDR_LOOPBACK                    0x7F000001
#define  BSS_BSD_INADDR_BROADCAST                   (ULONG)0xFFFFFFFF
#define  BSS_BSD_INADDR_NONE                        0xFFFFFFFF

#define  BSS_BSD_ADDR_ANY                           BSS_BSD_INADDR_ANY


/***********************************************************
                 BSS BSD SOCKET TYPE DEFINITION
***********************************************************/

/*
 * When the students implemented the original BSD socket library, they started big and failed to
 * deliver. The "socket type" is an example: it was conceived to be used as a service-type
 * identifier for the underlying transportation mechanism, and shield the socket applicatios from
 * dealing with the transport protocol directly. Oh, really?
 */
#define  BSS_BSD_SOCK_STREAM                        1               /* stream socket              */
#define  BSS_BSD_SOCK_DGRAM                         2               /* datagram socket            */
#define  BSS_BSD_SOCK_RAW                           3               /* raw-protocol interface     */
#define  BSS_BSD_SOCK_RDM                           4               /* reliably-delivered message */
#define  BSS_BSD_SOCK_SEQPACKET                     5               /* sequenced packet stream    */


/***********************************************************
           BSS BSD SOCKET DATA STRUCTURE DEFINITION
***********************************************************/

/*
 * The "socket descriptor" has long been defined as a signed/unsigned interger on most commerical
 * operating systems. We follow the same convention in the BSD-compatible interface, though we use
 * a complete different presentation.
 */
typedef  int  BSS_BSD_SOCKET,  *PBSS_BSD_SOCKET;

#define  BSS_BSD_SOCKET_INVALID_SOCKET              0
#define  BSS_BSD_SOCKET_ERROR                       -1

/*
 * The "fd_set" data structure is used by various Ansc Sockets functions and service providers,
 * such as the select function, to place sockets into a "set" for various purposes, such as testing
 * a given socket for readability using the readfds parameter of the select function.
 */
#ifndef  BSS_BSD_FDSETSIZE
#define  BSS_BSD_FDSETSIZE                          32
#endif

#define  BSS_BSD_FD_SETSIZE                         BSS_BSD_FDSETSIZE

typedef  struct
_BSS_BSD_FDSET
{
    ULONG                           fd_count;
    BSS_BSD_SOCKET                  fd_array[BSS_BSD_FDSETSIZE];
}
BSS_BSD_FDSET,  *PBSS_BSD_FDSET;

#define  BSS_BSD_FD_CLR                             BssBsdFdsClear
#define  BSS_BSD_FD_ISSET                           AnscSocketLibFdsIsSet
#define  BSS_BSD_FD_SET                             BssBsdFdsSet
#define  BSS_BSD_FD_ZERO                            BssBsdFdsZero
#define  BSS_BSD_FD_ISNUL                           BssBsdFdsIsNull
#define  BSS_BSD_FD_GET                             BssBsdFdsGet

#define  BssBsdFdsClear(fd, set)                                                            \
         {                                                                                  \
            PBSS_BSD_FDSET          fd_set1 = (PBSS_BSD_FDSET)set;                          \
            ULONG                   ii      = 0;                                            \
            ULONG                   jj      = 0;                                            \
                                                                                            \
            for ( ii = 0; ii < fd_set1->fd_count; ii++ )                                    \
            {                                                                               \
                if ( fd_set1->fd_array[ii] == fd )                                          \
                {                                                                           \
                    for ( jj = ii; jj < (fd_set1->fd_count - 1); jj++ )                     \
                    {                                                                       \
                        fd_set1->fd_array[jj] = fd_set1->fd_array[jj + 1];                  \
                    }                                                                       \
                                                                                            \
                    fd_set1->fd_count--;                                                    \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
         }

#define  BssBsdFdsSet(fd, set)                                                              \
         {                                                                                  \
            PBSS_BSD_FDSET          fd_set1 = (PBSS_BSD_FDSET)set;                          \
            ULONG                   ii      = 0;                                            \
                                                                                            \
            for ( ii = 0; ii < fd_set1->fd_count; ii++ )                                    \
            {                                                                               \
                if ( fd_set1->fd_array[ii] == fd )                                          \
                {                                                                           \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if ( ii >= fd_set1->fd_count )                                                  \
            {                                                                               \
                if ( fd_set1->fd_count < BSS_BSD_FDSETSIZE )                                \
                {                                                                           \
                    fd_set1->fd_array[fd_set1->fd_count++] = fd;                            \
                }                                                                           \
            }                                                                               \
         }

#define  BssBsdFdsZero(set)                         ( (set)->fd_count  = 0 )
#define  BssBsdFdsIsNull(set)                       ( (set)->fd_count == 0 )

#define  BssBsdFdsGet(set, fd, index)                                                       \
         {                                                                                  \
            PBSS_BSD_FDSET          fd_set1    = (PBSS_BSD_FDSET)set;                       \
            BSS_BSD_SOCKET          socket_nul = BSS_BSD_SOCKET_INVALID_SOCKET;             \
                                                                                            \
            fd = (index < fd_set1->fd_count)? fd_set1->fd_array[index] : socket_nul;        \
         }

/*
 * The Bss Socket hostent structure is used by functions to store information about a given host,
 * such as host name, IP address, and so forth. An application should never attempt to modify this
 * structure or to free any of its components.
 */
typedef  struct
_BSS_BSD_HOSTENT
{
    char*                           h_name;                     /* official name of host        */
    char**                          h_aliases;                  /* alias list                   */
    short                           h_addrtype;                 /* host address type            */
    short                           h_length;                   /* length of address            */
    char**                          h_addr_list;                /* list of addresses            */

    #define  h_addr                 h_addr_list[0]              /* address, backward compatible */
}
BSS_BSD_HOSTENT,  *PBSS_BSD_HOSTENT;

/*
 * The Ansc Sockets "in_addr" structure represents a host by its Internet address.
 */
typedef  struct
_BSS_BSD_INADDR
{
    union
    {
        struct{UCHAR  s_b1,s_b2,s_b3,s_b4;}S_un_b;
        struct{USHORT s_w1,s_w2;          }S_un_w;
        ULONG                              S_addr;
    }S_un;

    #ifndef  s_addr
    #define  s_addr                 S_un.S_addr                 /* can be used for most tcp & ip code */
    #endif

    #define  s_host                 S_un.S_un_b.s_b2            /* host on imp                        */
    #define  s_net                  S_un.S_un_b.s_b1            /* network                            */
    #define  s_imp                  S_un.S_un_w.s_w2            /* imp                                */
    #define  s_impno                S_un.S_un_b.s_b4            /* imp #                              */
    #define  s_lh                   S_un.S_un_b.s_b3            /* logical host                       */
}
BSS_BSD_INADDR,  *PBSS_BSD_INADDR;

/*
 * The Ansc Sockets "linger" structure maintains information about a specific socket that specifies
 * how that socket should behave when data is queued to be sent and the closesocket function is
 * called on the socket.
 */
typedef  struct
_BSS_BSD_LINGER
{
    USHORT                          l_onoff;
    USHORT                          l_linger;       /* timeout in seconds */
}
BSS_BSD_LINGER,  *PBSS_BSD_LINGER;

/*
 * The Ansc Sockets "sockaddr" structure varies depending on the protocol selected. Except for the
 * "sa_family" parameter, sockaddr contents are expressed in network byte order. The name parameter
 * is not strictly interpreted as a pointer to a sockaddr structure. It is presented in this manner
 * for Ansc Sockets compatibility. The actual structure is interpreted differently in the context
 * of different address families. The only requirements are that the first USHORT is the address
 * family and the total size of the memory buffer in bytes is namelen.
 */
typedef  struct
_BSS_BSD_SOCKADDR
{
    USHORT                          sa_family;
    char                            sa_data[14];
}
BSS_BSD_SOCKADDR,  *PBSS_BSD_SOCKADDR;

typedef  struct
_BSS_BSD_SOCKADDR_IN
{
    short                           sin_family;
    USHORT                          sin_port;
    BSS_BSD_INADDR                  sin_addr;
    char                            sin_zero[8];
}
BSS_BSD_SOCKADDR_IN,  *PBSS_BSD_SOCKADDR_IN;

typedef  struct
_BSS_BSD_SOCKPROTO
{
    USHORT                          sp_family;                  /* address family */
    USHORT                          sp_protocol;                /* protocol       */
}
BSS_BSD_SOCKPROTO,  *PBSS_BSD_SOCKPROTO;

/*
 * The Ansc Sockets timeval structure is used to specify time values.
 */
typedef  struct
_BSS_BSD_TIMEVAL
{
    long                            tv_sec;                     /* in seconds      */
    long                            tv_usec;                    /* in microseconds */
}
BSS_BSD_TIMEVAL,  *PBSS_BSD_TIMEVAL;

/*
 * Before a host can receive IP multicast datagrams, it must become a member of one or more IP
 * multicast groups. Following structure is defined to accompany the _setsockopt() call to join a
 * multicast group with IP_ADD_MEMBERSHIP option.
 */
typedef  struct
_BSS_BSD_IP_MREQ
{
    BSS_BSD_INADDR                  imr_multiaddr;
    BSS_BSD_INADDR                  imr_interface;
}
BSS_BSD_IP_MREQ,  *PBSS_BSD_IP_MREQ;


#endif
