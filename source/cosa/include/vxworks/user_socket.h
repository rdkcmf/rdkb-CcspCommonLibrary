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

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

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

#define  INVALID_SOCKET                             -1

#ifndef  SOCKET_ERROR
#define  SOCKET_ERROR                               -1
#endif


/*
 * We provide a platform-independent wrapper for all the socket related operations. However, since
 * the original bsd socket api functions are pretty stupid and ugly, instead of wrapping around the
 * socket functions with better names, we simplify the interface a little bit.
 */

#ifndef  _ANSC_SOCKET_LIBRARY_

    typedef             int             ANSC_SOCKET,            *PANSC_SOCKET;
    typedef  struct     in_addr         ansc_in_addr,           *pansc_in_addr;
    typedef  struct     sockaddr_in     ansc_socket_addr_in,    *pansc_socket_addr_in;
    typedef  struct     sockaddr        ansc_socket_addr,       *pansc_socket_addr;

    /*
     * SHX: we have to go with the new fd_set definition, this will affect select,
     *      all FD_** function.
     *
    typedef  struct     fd_set          ansc_fd_set,            *pansc_fd_set;
     *
     */
    typedef  struct
    _ansc_fd_set
    {
        unsigned int                fd_count;
        fd_set                      set;
    }
    ansc_fd_set, *pansc_fd_set;

    typedef  struct     timeval         ansc_timeval,           *pansc_timeval;
    typedef  struct     hostent         ansc_hostent,           *pansc_hostent;

    #define  _ansc_accept                           accept
    #define  _ansc_bind                             bind
    #define  _ansc_closesocket                      close
    #define  _ansc_connect                          connect
    #define  _ansc_gethostbyaddr                    gethostbyaddr

    ansc_hostent*
    _ansc_gethostbyname
        (
            char*                       pDomainName
        );

    #define  _ansc_gethostname                      gethostname

    #define  _ansc_getpeername                      getpeername
    #define  _ansc_getsocketname                    getsockname
    #define  _ansc_getsocketopt                     getsockopt
    #define  _ansc_htonl                            htonl
    #define  _ansc_htons                            htons
    #define  _ansc_inet_addr                        inet_addr
    #define  _ansc_inet_ntoa                        inet_ntoa
    #define  _ansc_listen                           listen
    #define  _ansc_ntohl                            ntohl
    #define  _ansc_ntohs                            ntohs
    #define  _ansc_recv                             recv
    #define  _ansc_recvfrom                         recvfrom

    /*
    #define  _ansc_select                           select
     */
    int
    _ansc_select
        (
            int                     width,
            pansc_fd_set            pReadFds,
            pansc_fd_set            pWriteFds,
            pansc_fd_set            pExceptFds,
            struct timeval*         pTimeout
        );

    #define  _ansc_send                             send
    #define  _ansc_sendto                           sendto
    #define  _ansc_setsocketopt                     setsockopt
    #define  _ansc_shutdown                         shutdown
    #define  _ansc_socket                           socket
    #define  _ansc_get_last_error()                 (0)

    #define  ANSC_SOCKET_AF_INET                    AF_INET
    #define  ANSC_SOCKET_STREAM                     SOCK_STREAM
    #define  ANSC_SOCKET_DGRAM                      SOCK_DGRAM
    #define  ANSC_SOCKET_RAWIP                      SOCK_RAW
    #define  ANSC_SOCKET_INVALID_SOCKET             INVALID_SOCKET
    #define  ANSC_SOCKET_ERROR                      SOCKET_ERROR
    #define  ANSC_SOCKET_ANY_ADDRESS                INADDR_ANY

    #define  ANSC_SOCKET_FD_SETSIZE                 FD_SETSIZE

    /*
    #define  ANSC_SOCKET_FD_CLR                     FD_CLR
     */
    #define  ANSC_SOCKET_FD_CLR(fd, pSet)                                                   \
                {                                                                           \
                    (pSet)->fd_count --;                                                    \
                    FD_CLR(fd, &pSet->set);                                                 \
                }

    /*
    #define  ANSC_SOCKET_FD_ISSET                   FD_ISSET
    #define  ANSC_SOCKET_FD_SET                     FD_SET
    #define  ANSC_SOCKET_FD_ZERO                    FD_ZERO
    #define  ANSC_SOCKET_FD_ISNUL(set)              ( (((ansc_fd_set*)(set))->fd_count == 0) )
     */

    #define  ANSC_SOCKET_FD_ISSET(fd, pSet)         ( FD_ISSET((fd), (&(pSet)->set)) )

    #define  ANSC_SOCKET_FD_SET(fd, pSet)                                                   \
                 {                                                                          \
                    (pSet)->fd_count ++;                                                    \
                    FD_SET(fd, &(pSet)->set);                                               \
                 }

    #define  ANSC_SOCKET_FD_ZERO(pSet)                                                      \
                 {                                                                          \
                    (pSet)->fd_count = 0;                                                   \
                    FD_ZERO(&(pSet)->set);                                                  \
                 }

    #define  ANSC_SOCKET_FD_ISNUL(pSet)             ( ((pSet)->fd_count) == 0 )

    /*
    #define  ANSC_SOCKET_FD_GET(set, s, i)                                                  \
             {                                                                              \
                ansc_fd_set*        socket_set = (ansc_fd_set*)set;                         \
                ANSC_SOCKET         socket_nul = ANSC_SOCKET_INVALID_SOCKET;                \
                                                                                            \
                s = ((u_int)i < socket_set->fd_count)? socket_set->fd_array[i] : socket_nul;\
             }
     */
    void
    _ansc_socket_fd_get
        (
            pansc_fd_set            pSet,
            int*                    pSocket,
            int                     i
        );
    #define  ANSC_SOCKET_FD_GET(set, s, i)          _ansc_socket_fd_get((set), &(s), i)

    #define  ANSC_SOCKET_SOL_SOCKET                 SOL_SOCKET
    #define  ANSC_SOCKET_SOL_IPPROTO_TCP            IPPROTO_TCP
    #define  ANSC_SOCKET_SO_BROADCAST               SO_BROADCAST
    #define  ANSC_SOCKET_SO_REUSEADDR               SO_REUSEADDR

    #define  ANSC_SOCKET_SD_RECV                    0
    #define  ANSC_SOCKET_SD_SEND                    1
    #define  ANSC_SOCKET_SD_BOTH                    2
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

    typedef             int             XSKT_SOCKET,            *PXSKT_SOCKET;
    typedef  struct     in_addr         xskt_in_addr,           *pxskt_in_addr;
    typedef  struct     sockaddr        xskt_socket_addr,       *pxskt_socket_addr;
    typedef  struct     fd_set          xskt_fd_set,            *pxskt_fd_set;
    typedef  struct     timeval         xskt_timeval,           *pxskt_timeval;
    typedef  struct     hostent         xskt_hostent,           *pxskt_hostent;

    #define  xskt_socket_addr_in                    ansc_socket_addr_in
    #define  pxskt_socket_addr_in                   pansc_socket_addr_in
    #define  _xskt_accept                           accept
    #define  _xskt_bind                             bind
    #define  _xskt_closesocket                      close
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
    #define  _xskt_get_last_error()                 (0)

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
    #define  XSKT_SOCKET_FD_ISNUL(set)              ( 0 )
    #define  XSKT_SOCKET_FD_GET(set, s, i)

    #define  XSKT_SOCKET_SOL_SOCKET                 SOL_SOCKET
    #define  XSKT_SOCKET_SOL_IPPROTO_TCP            IPPROTO_TCP
    #define  XSKT_SOCKET_SO_BROADCAST               SO_BROADCAST
    #define  XSKT_SOCKET_SO_REUSEADDR               SO_REUSEADDR

    #define  XSKT_SOCKET_SD_RECV                    0
    #define  XSKT_SOCKET_SD_SEND                    1
    #define  XSKT_SOCKET_SD_BOTH                    2
	#define	 AnscStartupXsocketWrapper(p)           0;assert(FALSE)
	#define	 AnscCleanupXsocketWrapper(p)           0;assert(FALSE)

#endif


#endif
