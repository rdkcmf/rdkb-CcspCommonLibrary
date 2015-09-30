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

    module: kernel_socket.h

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

        Linux kernel

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Geng Yang

    ---------------------------------------------------------------

    revision:

        04/04/01    initial revision.
        10/06/01    separated from the ansc_socket.h, one for each
                    specific platform
        10/23/01    ported to linux by Geng Yang

**********************************************************************/


#ifndef  _KERNEL_SOCKET_
#define  _KERNEL_SOCKET_

/***********************************************************
    PLATFORM DEPENDENT DATA TYPE AND MACRO DEFINITIONS
***********************************************************/

/*
 * We provide a platform-independent wrapper for all the socket related operations. However, since
 * the original bsd socket api functions are pretty stupid and ugly, instead of wrapping around the
 * socket functions with better names, we simplify the interface a little bit.
 */
#ifdef   _ANSC_LINUX_2_6_

    #define  KernelSockGetState(pSocket)                ((struct socket*)(pSocket))->sk->sk_state

#else

    #define  KernelSockGetState(pSocket)                ((struct socket*)(pSocket))->sk->state

#endif


#ifndef _ANSC_SOCKET_LIBRARY_

    #define  MAX_SOCK_ADDR                          128
    #define  ANSC_SOCKET_ACCEPT_CHECKING_INTERVAL   50
    #define  ANSC_SOCKET_RECV_CHECKING_INTERVAL     10
    #define  TCP_TIMEOUT                            2000

    typedef  struct socket *            ANSC_SOCKET,            *PANSC_SOCKET;
    typedef  struct in_addr             ansc_in_addr,           *pansc_in_addr;
    typedef  struct sockaddr_in         ansc_socket_addr_in,    *pansc_socket_addr_in;
    typedef  struct sockaddr            ansc_socket_addr,       *pansc_socket_addr;
    typedef  fd_set                     ansc_fd_set,            *pansc_fd_set;
    typedef  struct timeval             ansc_timeval,           *pansc_timeval;
    typedef  int                        ansc_hostent,           *pansc_hostent;
    typedef  struct     ip_mreq         ansc_ip_mreq,           *pansc_ip_mreq;

    #define  ANSC_SOCKET_AF_INET                    AF_INET
    #define  ANSC_SOCKET_STREAM                     SOCK_STREAM
    #define  ANSC_SOCKET_DGRAM                      SOCK_DGRAM
    #define  ANSC_SOCKET_RAWIP                      SOCK_RAW
    #define  ANSC_SOCKET_INVALID_SOCKET             NULL
    #define  ANSC_SOCKET_ERROR                      -1
    #define  ANSC_SOCKET_ANY_ADDRESS                INADDR_ANY

    #define  ANSC_SOCKET_FD_SETSIZE                 32
    #define  ANSC_SOCKET_FD_CLR                     _fd_clr
    #define  ANSC_SOCKET_FD_ISSET                   _fd_isset
    #define  ANSC_SOCKET_FD_SET                     _fd_set
    #define  ANSC_SOCKET_FD_ZERO                    _fd_zero
    #define  ANSC_SOCKET_FD_ISNUL                   _fd_isnull
    #define  ANSC_SOCKET_FD_GET                     _fd_get

    #define  ANSC_SOCKET_SOL_SOCKET                 SOL_SOCKET
    #define  ANSC_SOCKET_SOL_IPPROTO_TCP            IPPROTO_TCP
    #define  ANSC_SOCKET_SOL_IPPROTO_IP             IPPROTO_IP
    #define  ANSC_SOCKET_SO_BROADCAST               SO_BROADCAST
    #define  ANSC_SOCKET_SO_REUSEADDR               SO_REUSEADDR
    #define  ANSC_SOCKET_SO_SNDTIMEO                SO_SNDTIMEO
    #define  ANSC_SOCKET_SO_RCVTIMEO                SO_RCVTIMEO
    #define  ANSC_SOCKET_SO_LASTRECVIF              0
    #define  ANSC_SOCKET_SO_TOS                     0
    #define  ANSC_SOCKET_SO_TTL                     0

    #define  ANSC_SOCKET_SD_RECV                    0
    #define  ANSC_SOCKET_SD_SEND                    1
    #define  ANSC_SOCKET_SD_BOTH                    2

    #define  ANSC_SOCKET_SO_TCP_USETLS              0
    #define  ANSC_SOCKET_SO_TCP_TLSEXPORTONLY       0
    #define  ANSC_SOCKET_SO_TCP_TLSSTRONGSEC        0

    #define  ANSC_SOCKET_IP_MULTICAST_TTL           IP_MULTICAST_TTL
    #define  ANSC_SOCKET_IP_MULTICAST_IF            IP_MULTICAST_IF
    #define  ANSC_SOCKET_IP_ADD_MEMBERSHIP          IP_ADD_MEMBERSHIP
    #define  ANSC_SOCKET_IP_DROP_MEMBERSHIP         IP_DROP_MEMBERSHIP
    #define  ANSC_SOCKET_IP_MULTICAST_LOOP          IP_MULTICAST_LOOP
    #define  ANSC_SOCKET_IP_RECV_UNICAST            0

    #define  _ansc_inet_ntoa                        al_addr2string

    __static_inline int
    _fd_setsize
    (
        int  socket,
        pansc_fd_set set
    )
    {
        return 0;
    }
    __static_inline int
    _fd_clr
    (
        int  socket,
        pansc_fd_set set
    )
    {
        return 0;
    }
    __static_inline int
    _fd_isset
    (
        int  socket,
        pansc_fd_set set
    )
    {
        return 0;
    }
    __static_inline int
    _fd_set
    (
        int  socket,
        pansc_fd_set set
    )
    {
        return 0;
    }
    __static_inline int
    _fd_zero
    (
        pansc_fd_set set
    )
    {
        return 0;
    }
    __static_inline int
    _fd_isnull
    (
        pansc_fd_set set
    )
    {
        return 0;
    }
    __static_inline int
    _fd_get
    (
        pansc_fd_set set,
        int  socket,
        int  index

    )
    {
        return 0;
    }

    /*
     * below are unsupported yet:
     *
     * typedef  struct fd_set               ansc_fd_set,            *pansc_fd_set;
     *
     * #define  _ansc_gethostbyaddr
     * #define  _ansc_select                               select
     * #define  ANSC_SOCKET_FD_CLR                         FD_CLR
     * #define  ANSC_SOCKET_FD_ISSET                       FD_ISSET
     * #define  ANSC_SOCKET_FD_SET                         FD_SET
     * #define  ANSC_SOCKET_FD_ZERO                        FD_ZERO
     */
    #define  _ansc_gethostname(name, len)               snprintf((name), (len), "AL_GATEWAY")
    #define  _ansc_gethostbyname(name)                  NULL

    __static_inline ULONG /* copied/modified from al_util.c */
    al_v4_inet_addr
        (
            char                        *pString
        )
    {
        int                             stringLength  = strlen( pString );
        int                             IndexInString = 0;
        int                             LengthCount   = 0;
        int                             i             = 0;
        int                             tempValue     = 0;
        unsigned long                   Address       = 0;
        unsigned char *                 pAddress      = (unsigned char *)&Address;

        for ( i = 0; i < 4; i ++ )
        {
            while (
                    (pString[IndexInString] >= '0') &&
                    (pString[IndexInString] <= '9') &&
                    (IndexInString < stringLength )
                  )
            {
                LengthCount ++;
                tempValue   = pAddress[i] * 10 + pString[IndexInString] - '0';
                pAddress[i] = pAddress[i] * 10 + pString[IndexInString] - '0';
                IndexInString ++;
                if ( LengthCount > 3 )
                {
                    return -1;
                }
            }

            if ( LengthCount == 0 || tempValue > 0xFF )
            {
                return -1;
            }

            tempValue   = 0;

            if ( i != 3 )
            {
                if ( pString[IndexInString] != '.' )
                {
                    return -1;
                }
                IndexInString ++;
            }

            LengthCount = 0;
        }

        if ( IndexInString != stringLength )
        {
            return -1;
        }

        return Address;
    }

    __static_inline void
    al_addr2string( unsigned char *  address, char*  string)
    {
        /*
         * temporary counters
         */
        int                             i       = 0;
        int                             counter = 0;

        /*
         *  each byte of address
         */
        unsigned char                   theByte;
        unsigned char                   HundredDigit;
        unsigned char                   TenDigit;
        unsigned char                   UnitDigit;

        /*
         * loop to do conversion
         */
        for ( i = 0; i < 4; i++ )
        {
            theByte      = address[i];

            HundredDigit = theByte / 100;
            TenDigit     = ( theByte - HundredDigit * 100 ) / 10;
            UnitDigit    = theByte % 10;

            if ( HundredDigit )
            {
                string[ counter++ ] = HundredDigit + '0';
            }
            if ( TenDigit || HundredDigit)
            {
                string[ counter++ ] = TenDigit + '0';
            }
            string[ counter++ ] = UnitDigit + '0';

            if ( i != 3 )
            {
                string[counter] = '.';
                counter++;
            }
        }

        /*
         * add NULL
         */
        string[counter] = 0;

        /*
         * return control to caller
         */
        return;
    }

    #define  _ansc_inet_addr                            al_v4_inet_addr
    #define  _ansc_htonl                                htonl
    #define  _ansc_htons                                htons
    #define  _ansc_ntohl                                ntohl
    #define  _ansc_ntohs                                ntohs

    __static_inline ANSC_SOCKET
    _ansc_accept
    (
        ANSC_SOCKET                 sock,
        ansc_socket_addr            *myaddr,
        int                         *addrlen
    )
    {
        ANSC_SOCKET                 newsock;
        int                         err = -1;

        err = -EMFILE;
        if (!(newsock = sock_alloc()))
        {
            goto out;
        }

        newsock->type   = sock->type;
        newsock->ops    = sock->ops;

        err = sock->ops->accept(sock, newsock, O_NONBLOCK);
        if (err < 0)
        {
            goto out_release;
        }

        if (myaddr)
        {
            if(newsock->ops->getname(newsock, (struct sockaddr *)myaddr, addrlen, 2)<0)
            {
                err = -ECONNABORTED;
                goto out_release;
            }
        }

    out:
        return newsock;

    out_release:
        sock_release(newsock);
        return NULL;
    }

    __static_inline int
    _ansc_bind
        (
            ANSC_SOCKET                 sock,
            ansc_socket_addr            *myaddr,
            int                         addrlen
        )
    {
        return sock->ops->bind (sock, myaddr, addrlen);
    }

    #define  _ansc_closesocket                       sock_release

    __static_inline int
    _ansc_connect
        (
            ANSC_SOCKET                 sock,
            ansc_socket_addr            *servaddr,
            int                         addrlen
        )
    {
        int err = 0;

        #ifdef _ANSC_LINUX_2_6_
        sock->sk->sk_sndtimeo = TCP_TIMEOUT;
        #else
        sock->sk->sndtimeo = TCP_TIMEOUT;
        #endif
        err = sock->ops->connect(sock, servaddr, addrlen, 0);
        return err;
    }

    __static_inline int
    _ansc_getsocketname
        (
            ANSC_SOCKET                 sock,
            ansc_socket_addr            *servaddr,
            int                         *addrlen
        )
    {
        return sock->ops->getname(sock, servaddr, addrlen, 0);
    }

    __static_inline int
    _ansc_getpeername
        (
            ANSC_SOCKET                 sock,
            ansc_socket_addr            *servaddr,
            int                         *addrlen
        )
    {
        return sock->ops->getname(sock, servaddr, addrlen, 1);
    }

    __static_inline int
    _ansc_getsocketopt
        (
            ANSC_SOCKET                 sock,
            int                         level,
            int                         optname,
            char                        *optval,
            int                         *optlen
        )
    {
        return sock->ops->getsockopt(sock, level, optname, optval, optlen);
    }


    __static_inline int
    _ansc_ioctlsocket
        (
            ANSC_SOCKET                 sock,
            int                         cmd,
            int                         arg
        )
    {
        return sock->ops->ioctl(sock, cmd, arg);
    }

    __static_inline int
    _ansc_listen
    (
        ANSC_SOCKET                 sock,
        int                         backlog
    )
    {
        return sock->ops->listen(sock, backlog);
    }

    __static_inline int
    _ansc_recvfrom
        (
            ANSC_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags,
            struct sockaddr             *addr,
            int                         *addr_len
        )
    {
        struct iovec                    iov;
        struct msghdr                   msg;
        int                             err = -1;
        mm_segment_t                    oldfs;

        #ifdef _ANSC_LINUX_2_6_
        if ( skb_queue_empty(&(sock->sk->sk_receive_queue)))
        #else
        if ( skb_queue_empty(&(sock->sk->receive_queue)))
        #endif
        {
            return 0;
        }

        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_iovlen      = 1;
        msg.msg_iov         = &iov;
        iov.iov_len         = len;
        iov.iov_base        = buff;
        msg.msg_name        = addr;
        msg.msg_namelen     = MAX_SOCK_ADDR;

        flags |= MSG_DONTWAIT;

        oldfs = get_fs();
        set_fs(KERNEL_DS);  // NECESSARY, WILL NOT WORK WITHOUT THIS
        err = sock_recvmsg(sock, &msg, len, flags);
        set_fs(oldfs);

        if(addr_len)
        {
            *addr_len = msg.msg_namelen;
        }

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _ansc_recv
        (
            ANSC_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags
        )
    {
        struct sockaddr                 *addr       = NULL;
        int                             *addr_len   = NULL;
        int                             err         = -1;
        struct iovec                    iov;
        struct msghdr                   msg;
        mm_segment_t                     oldfs;

        #ifdef _ANSC_LINUX_2_6_
        if ( skb_queue_empty(&(sock->sk->sk_receive_queue)))
        #else
        if ( skb_queue_empty(&(sock->sk->receive_queue)))
        #endif
        {
            return 0;
        }

        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_iovlen      = 1;
        msg.msg_iov         = &iov;
        iov.iov_len         = len;
        iov.iov_base        = buff;
        msg.msg_name        = addr;
        msg.msg_namelen     = MAX_SOCK_ADDR;

        flags |= MSG_DONTWAIT;

        oldfs = get_fs();
        set_fs(KERNEL_DS);  // NECESSARY, WILL NOT WORK WITHOUT THIS
        err = sock_recvmsg(sock, &msg, len, flags);
        set_fs(oldfs);

        if(addr_len)
        {
            *addr_len = msg.msg_namelen;
        }

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _ansc_sendto
        (
            ANSC_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags,
            struct sockaddr             *addr,
            int                         addr_len
        )
    {
        int                             err = -1;
        struct msghdr                   msg;
        struct iovec                    iov;
        mm_segment_t                    oldfs;

        iov.iov_base        = buff;
        iov.iov_len         = len;
        msg.msg_name        = addr;
        msg.msg_iov         = &iov;
        msg.msg_iovlen      = 1;
        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_namelen     = addr_len;

    /*
        if (sock->file->f_flags & O_NONBLOCK)
    */
        flags |= MSG_DONTWAIT;
        msg.msg_flags = flags;

        oldfs = get_fs();
        set_fs(KERNEL_DS);  // NECESSARY, WILL NOT WORK WITHOUT THIS
        err = sock_sendmsg(sock, &msg, len);
        set_fs(oldfs);

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _ansc_send
        (
            ANSC_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags
        )
    {
        struct sockaddr                 *addr       = NULL;
        int                             addr_len    = 0;
        int                             err         = -1;
        struct msghdr                   msg;
        struct iovec                    iov;
        mm_segment_t                     oldfs;

        iov.iov_base        = buff;
        iov.iov_len         = len;
        msg.msg_name        = addr;
        msg.msg_iov         = &iov;
        msg.msg_iovlen      = 1;
        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_namelen     = addr_len;
        msg.msg_flags       = flags;

        #ifdef _ANSC_LINUX_2_6_
        sock->sk->sk_sndtimeo = TCP_TIMEOUT;
        #else
        sock->sk->sndtimeo = TCP_TIMEOUT;
        #endif

        oldfs = get_fs();
        set_fs(KERNEL_DS);  // NECESSARY, WILL NOT WORK WITHOUT THIS
        err = sock_sendmsg(sock, &msg, len);
        set_fs(oldfs);

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _ansc_setsocketopt
        (
            ANSC_SOCKET                 sock,
            int                         level,
            int                         optname,
            char                        *optval,
            int                         optlen
        )
    {
        return sock->ops->setsockopt(sock, level, optname, optval, optlen);
    }

    __static_inline ANSC_SOCKET
    _ansc_socket
        (
            int                         family,
            int                         type,
            int                         protocol
        )
    {
        int                             err  = -1;
        ANSC_SOCKET                     sock = NULL;

        err = sock_create(family, type, protocol, &sock);
        if (err < 0)
        {
            return NULL;
        }
        #ifdef _ANSC_LINUX_2_6_
        sock->sk->sk_allocation = GFP_ATOMIC;
        #else
        sock->sk->allocation = GFP_ATOMIC;
        #endif
        return  sock;
    }

    #define _ansc_select(nfds, readfds, writefds, exceptfds, timeout)                           1

#ifdef _ANSC_LINUX_2_6_
    #define  _ansc_en_broadcast(s)                                                              \
        do {sock_set_flag(s->sk, SOCK_BROADCAST);} while(0)
#else
    #define  _ansc_en_broadcast(s)                                                              \
        do {s->sk->broadcast = 1;} while(0)
#endif

#ifdef _ANSC_LINUX_2_6_
    #define  _ansc_en_reuseaddr(s)                                                              \
        do {s->sk->sk_reuse = 1;} while(0)
#else
    #define  _ansc_en_reuseaddr(s)                                                              \
        do {s->sk->reuse = 1;} while(0)
#endif

    __static_inline int
    _ansc_get_last_error
        (
            void
        )
    {
        return -1;                                        /* nonsense in kernel */
    }

    __static_inline int
    _ansc_shutdown
        (
            ANSC_SOCKET                 sock,
            int                         how
        )
    {
        return sock->ops->shutdown(sock, how);
    }

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
#ifdef  _KERNEL_SOCKET_

    typedef  struct socket *            XSKT_SOCKET,            *PXSKT_SOCKET;
    typedef  struct sockaddr_in         xskt_socket_addr_in,    *pxskt_socket_addr_in;
    typedef  struct sockaddr            xskt_socket_addr,       *pxskt_socket_addr;
    typedef  struct timeval             xskt_timeval,           *pxskt_timeval;
    typedef  struct in_addr             xskt_in_addr,           *pxskt_in_addr;
    typedef  int                        xskt_hostent,           *pxskt_hostent;
    typedef  struct     ip_mreq         xskt_ip_mreq,           *pxskt_ip_mreq;

    typedef  fd_set                     xskt_fd_set,            *pxskt_fd_set;

    #define  XSKT_SOCKET_FD_SETSIZE                 32
    #define  XSKT_SOCKET_FD_CLR                     _xfd_clr
    #define  XSKT_SOCKET_FD_ISSET                   _xfd_isset
    #define  XSKT_SOCKET_FD_SET                     _xfd_set
    #define  XSKT_SOCKET_FD_ZERO                    _xfd_zero
    #define  XSKT_SOCKET_FD_ISNUL                   _xfd_isnull
    #define  XSKT_SOCKET_FD_GET                     _xfd_get

    #define  _xskt_inet_ntoa                        xal_addr2string

    __static_inline int 
    _xfd_clr
    (
        XSKT_SOCKET                 socket,
        pxskt_fd_set                set
    )
    {
        return  0;
    }
    __static_inline int
    _xfd_isset
    (
        XSKT_SOCKET                 socket,
        pxskt_fd_set                set
    )
    {
        return 1;
    }
    __static_inline int
    _xfd_set
    (
        XSKT_SOCKET                 socket,
        pxskt_fd_set                set
    )
    {
        return 0;
    }
    __static_inline int
    _xfd_zero
    (
        pxskt_fd_set                set
    )
    {
        return 0;
    }

    __static_inline int
    _xfd_isnull
    (
        pxskt_fd_set                set
    )
    {
        return 0;
    }
    __static_inline int
    _xfd_get
    (
        pxskt_fd_set                set,
        XSKT_SOCKET                 socket,
        int                         index
    )
    {
        return 0;
    }

    /*
     * below are unsupported yet:
     *
     * typedef  struct fd_set               xskt_fd_set,            *pansc_fd_set;
     *
     * #define  _xskt_gethostbyaddr
     * #define  _xskt_select                               select
     * #define  XSKT_SOCKET_FD_CLR                         FD_CLR
     * #define  XSKT_SOCKET_FD_ISSET                       FD_ISSET
     * #define  XSKT_SOCKET_FD_SET                         FD_SET
     * #define  XSKT_SOCKET_FD_ZERO                        FD_ZERO
     */
    #define  _xskt_gethostname(name, len)               snprintf((name), (len), "AL_GATEWAY")
    #define  _xskt_gethostbyname(name)                  NULL

    __static_inline ULONG /* copied/modified from al_util.c */
    xal_v4_inet_addr
        (
            char                        *pString
        )
    {
        int                             stringLength  = strlen( pString );
        int                             IndexInString = 0;
        int                             LengthCount   = 0;
        int                             i             = 0;
        int                             tempValue     = 0;
        unsigned long                   Address       = 0;
        unsigned char *                 pAddress      = (unsigned char *)&Address;

        for ( i = 0; i < 4; i ++ )
        {
            while (
                    (pString[IndexInString] >= '0') &&
                    (pString[IndexInString] <= '9') &&
                    (IndexInString < stringLength )
                  )
            {
                LengthCount ++;
                tempValue   = pAddress[i] * 10 + pString[IndexInString] - '0';
                pAddress[i] = pAddress[i] * 10 + pString[IndexInString] - '0';
                IndexInString ++;
                if ( LengthCount > 3 )
                {
                    return -1;
                }
            }

            if ( LengthCount == 0 || tempValue > 0xFF )
            {
                return -1;
            }

            tempValue   = 0;

            if ( i != 3 )
            {
                if ( pString[IndexInString] != '.' )
                {
                    return -1;
                }
                IndexInString ++;
            }

            LengthCount = 0;
        }

        if ( IndexInString != stringLength )
        {
            return -1;
        }

        return Address;
    }

    __static_inline void
    xal_addr2string( unsigned char *  address, char*  string)
    {
        /*
         * temporary counters
         */
        int                             i       = 0;
        int                             counter = 0;

        /*
         *  each byte of address
         */
        unsigned char                   theByte;
        unsigned char                   HundredDigit;
        unsigned char                   TenDigit;
        unsigned char                   UnitDigit;

        /*
         * loop to do conversion
         */
        for ( i = 0; i < 4; i++ )
        {
            theByte      = address[i];

            HundredDigit = theByte / 100;
            TenDigit     = ( theByte - HundredDigit * 100 ) / 10;
            UnitDigit    = theByte % 10;

            if ( HundredDigit )
            {
                string[ counter++ ] = HundredDigit + '0';
            }
            if ( TenDigit || HundredDigit)
            {
                string[ counter++ ] = TenDigit + '0';
            }
            string[ counter++ ] = UnitDigit + '0';

            if ( i != 3 )
            {
                string[counter] = '.';
                counter++;
            }
        }

        /*
         * add NULL
         */
        string[counter] = 0;

        /*
         * return control to caller
         */
        return;
    }

    #define  _xskt_inet_addr                        xal_v4_inet_addr
    #define  _xskt_htonl                            htonl
    #define  _xskt_htons                            htons
    #define  _xskt_ntohl                            ntohl
    #define  _xskt_ntohs                            ntohs

    #define  XSKT_SOCKET_AF_INET                    AF_INET
    #define  XSKT_SOCKET_STREAM                     SOCK_STREAM
    #define  XSKT_SOCKET_DGRAM                      SOCK_DGRAM
    #define  XSKT_SOCKET_RAWIP                      SOCK_RAW
    #define  XSKT_SOCKET_INVALID_SOCKET             NULL
    #define  XSKT_SOCKET_ERROR                      -1
    #define  XSKT_SOCKET_ANY_ADDRESS                INADDR_ANY

    #define  XSKT_SOCKET_SOL_SOCKET                 SOL_SOCKET
    #define  XSKT_SOCKET_SOL_IPPROTO_TCP            IPPROTO_TCP
    #define  XSKT_SOCKET_SOL_IPPROTO_IP             IPPROTO_IP
    #define  XSKT_SOCKET_SO_BROADCAST               SO_BROADCAST
    #define  XSKT_SOCKET_SO_REUSEADDR               SO_REUSEADDR
    #define  XSKT_SOCKET_SO_LASTRECVIF              0

    #define  XSKT_SOCKET_SD_RECV                    0
    #define  XSKT_SOCKET_SD_SEND                    1
    #define  XSKT_SOCKET_SD_BOTH                    2

    #define  XSKT_SOCKET_SO_TCP_USETLS              0
    #define  XSKT_SOCKET_SO_TCP_TLSEXPORTONLY       0
    #define  XSKT_SOCKET_SO_TCP_TLSSTRONGSEC        0

    #define  XSKT_SOCKET_IP_MULTICAST_TTL           IP_MULTICAST_TTL
    #define  XSKT_SOCKET_IP_MULTICAST_IF            IP_MULTICAST_IF
    #define  XSKT_SOCKET_IP_ADD_MEMBERSHIP          IP_ADD_MEMBERSHIP
    #define  XSKT_SOCKET_IP_DROP_MEMBERSHIP         IP_DROP_MEMBERSHIP
    #define  XSKT_SOCKET_IP_MULTICAST_LOOP          IP_MULTICAST_LOOP
    #define  XSKT_SOCKET_IP_RECV_UNICAST            0

    #define  MAX_SOCK_ADDR                          128
    #define  XSKT_SOCKET_ACCEPT_CHECKING_INTERVAL   50
    #define  XSKT_SOCKET_RECV_CHECKING_INTERVAL     10
    #define  TCP_TIMEOUT                            2000

    __static_inline XSKT_SOCKET
    _xskt_accept
    (
        XSKT_SOCKET                 sock,
        xskt_socket_addr            *myaddr,
        int                         *addrlen
    )
    {
        XSKT_SOCKET                 newsock;
        int                         err = -1;

        err = -EMFILE;
        if (!(newsock = sock_alloc()))
        {
            goto out;
        }

        newsock->type   = sock->type;
        newsock->ops    = sock->ops;

        err = sock->ops->accept(sock, newsock, O_NONBLOCK);
        if (err < 0)
        {
            goto out_release;
        }

        if (myaddr)
        {
            if(newsock->ops->getname(newsock, (struct sockaddr *)myaddr, addrlen, 2)<0)
            {
                err = -ECONNABORTED;
                goto out_release;
            }
        }

    out:
        return newsock;

    out_release:
        sock_release(newsock);
        return NULL;
    }

    __static_inline int
    _xskt_bind
        (
            XSKT_SOCKET                 sock,
            xskt_socket_addr            *myaddr,
            int                         addrlen
        )
    {
        return sock->ops->bind (sock, myaddr, addrlen);
    }

    #define  _xskt_closesocket                       sock_release

    __static_inline int
    _xskt_connect
        (
            XSKT_SOCKET                 sock,
            xskt_socket_addr            *servaddr,
            int                         addrlen
        )
    {
        int err = 0;

        #ifdef _ANSC_LINUX_2_6_
        sock->sk->sk_sndtimeo = TCP_TIMEOUT;
        #else
        sock->sk->sndtimeo = TCP_TIMEOUT;
        #endif
        err = sock->ops->connect(sock, servaddr, addrlen, 0);
        return err;
    }

    __static_inline int
    _xskt_getsocketname
        (
            XSKT_SOCKET                 sock,
            xskt_socket_addr            *servaddr,
            int                         *addrlen
        )
    {
        return sock->ops->getname(sock, servaddr, addrlen, 0);
    }

    __static_inline int
    _xskt_getpeername
        (
            XSKT_SOCKET                 sock,
            xskt_socket_addr            *servaddr,
            int                         *addrlen
        )
    {
        return sock->ops->getname(sock, servaddr, addrlen, 1);
    }

    __static_inline int
    _xskt_getsocketopt
        (
            XSKT_SOCKET                 sock,
            int                         level,
            int                         optname,
            char                        *optval,
            int                         *optlen
        )
    {
        return sock->ops->getsockopt(sock, level, optname, optval, optlen);
    }


    __static_inline int
    _xskt_ioctlsocket
        (
            XSKT_SOCKET                 sock,
            int                         cmd,
            int                         arg
        )
    {
        return sock->ops->ioctl(sock, cmd, arg);
    }

    __static_inline int
    _xskt_listen
    (
        XSKT_SOCKET                 sock,
        int                         backlog
    )
    {
        return sock->ops->listen(sock, backlog);
    }

    __static_inline int
    _xskt_recvfrom
        (
            XSKT_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags,
            struct sockaddr             *addr,
            int                         *addr_len
        )
    {
        struct iovec                    iov;
        struct msghdr                   msg;
        int                             err = -1;
        mm_segment_t                    oldfs;

        #ifdef _ANSC_LINUX_2_6_
        if ( skb_queue_empty(&(sock->sk->sk_receive_queue)))
        #else
        if ( skb_queue_empty(&(sock->sk->receive_queue)))
        #endif
        {
            return 0;
        }

        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_iovlen      = 1;
        msg.msg_iov         = &iov;
        iov.iov_len         = len;
        iov.iov_base        = buff;
        msg.msg_name        = addr;
        msg.msg_namelen     = MAX_SOCK_ADDR;

        flags |= MSG_DONTWAIT;

        oldfs = get_fs();
        set_fs(KERNEL_DS);  // NECESSARY, WILL NOT WORK WITHOUT THIS
        err = sock_recvmsg(sock, &msg, len, flags);
        set_fs(oldfs);

        if(addr_len)
        {
            *addr_len = msg.msg_namelen;
        }

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _xskt_recv
        (
            XSKT_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags
        )
    {
        struct sockaddr                 *addr       = NULL;
        int                             *addr_len   = NULL;
        int                             err         = -1;
        struct iovec                    iov;
        struct msghdr                   msg;
        mm_segment_t                     oldfs;

        #ifdef _ANSC_LINUX_2_6_
        if ( skb_queue_empty(&((sock)->sk->sk_receive_queue)))
        #else
        if ( skb_queue_empty(&((sock)->sk->receive_queue)))
        #endif
        {
            return 0;
        }

        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_iovlen      = 1;
        msg.msg_iov         = &iov;
        iov.iov_len         = len;
        iov.iov_base        = buff;
        msg.msg_name        = addr;
        msg.msg_namelen     = MAX_SOCK_ADDR;

        flags |= MSG_DONTWAIT;

        oldfs = get_fs();
        set_fs(KERNEL_DS);  /* NECESSARY, WILL NOT WORK WITHOUT THIS */
        err = sock_recvmsg(sock, &msg, len, flags);
        set_fs(oldfs);

        if ( addr_len )
        {
            *addr_len = msg.msg_namelen;
        }

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _xskt_sendto
        (
            XSKT_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags,
            struct sockaddr             *addr,
            int                         addr_len
        )
    {
        int                             err = -1;
        struct msghdr                   msg;
        struct iovec                    iov;
        mm_segment_t                    oldfs;

        iov.iov_base        = buff;
        iov.iov_len         = len;
        msg.msg_name        = addr;
        msg.msg_iov         = &iov;
        msg.msg_iovlen      = 1;
        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_namelen     = addr_len;

    /*
        if (sock->file->f_flags & O_NONBLOCK)
    */
        flags |= MSG_DONTWAIT;
        msg.msg_flags = flags;

        oldfs = get_fs();
        set_fs(KERNEL_DS);  // NECESSARY, WILL NOT WORK WITHOUT THIS
        err = sock_sendmsg(sock, &msg, len);
        set_fs(oldfs);

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _xskt_send
        (
            XSKT_SOCKET                 sock,
            void                        * buff,
            size_t                      len,
            unsigned                    flags
        )
    {
        struct sockaddr                 *addr       = NULL;
        int                             addr_len    = 0;
        int                             err         = -1;
        struct msghdr                   msg;
        struct iovec                    iov;
        mm_segment_t                     oldfs;

        iov.iov_base        = buff;
        iov.iov_len         = len;
        msg.msg_name        = addr;
        msg.msg_iov         = &iov;
        msg.msg_iovlen      = 1;
        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        msg.msg_namelen     = addr_len;
        msg.msg_flags       = flags;

        #ifdef _ANSC_LINUX_2_6_
        (sock)->sk->sk_sndtimeo = TCP_TIMEOUT;
        #else
        (sock)->sk->sndtimeo = TCP_TIMEOUT;
        #endif

        oldfs = get_fs();
        set_fs(KERNEL_DS);  // NECESSARY, WILL NOT WORK WITHOUT THIS
        err = sock_sendmsg(sock, &msg, len);
        set_fs(oldfs);

        iov.iov_base        = buff;
        iov.iov_len         = len;

        return err;
    }

    __static_inline int
    _xskt_setsocketopt
        (
            XSKT_SOCKET                 sock,
            int                         level,
            int                         optname,
            char                        *optval,
            int                         optlen
        )
    {
        return sock->ops->setsockopt(sock, level, optname, optval, optlen);
    }

    __static_inline XSKT_SOCKET
    _xskt_socket
        (
            int                         family,
            int                         type,
            int                         protocol
        )
    {
        int                             err  = -1;
        XSKT_SOCKET                     sock = NULL;

        err = sock_create(family, type, protocol, &sock);
        if (err < 0)
        {
            return NULL;
        }
        #ifdef _ANSC_LINUX_2_6_
        sock->sk->sk_allocation = GFP_ATOMIC;
        #else
        sock->sk->allocation = GFP_ATOMIC;
        #endif
        return  sock;
    }

    #define _xskt_select(nfds, readfds, writefds, exceptfds, timeout)                           1

    #ifdef _ANSC_LINUX_2_6_
        #define  _xskt_en_broadcast(s)              do {sock_set_flag(s->sk, SOCK_BROADCAST);} while(0)
    #else
        #define  _xskt_en_broadcast(s)              do {s->sk->broadcast = 1;} while(0)
    #endif

    /* We don't have to do anything in kernel socket */
    #define  _xskt_en_multicast(s, m_addr, if_addr)

    #ifdef _ANSC_LINUX_2_6_
        #define  _xskt_en_reuseaddr(s)              do {s->sk->sk_reuse = 1;} while(0)
    #else
        #define  _xskt_en_reuseaddr(s)              do {s->sk->reuse = 1;} while(0)
    #endif

    __static_inline int
    _xskt_get_last_error
        (
            void
        )
    {
        return -1;                                        /* nonsense in kernel */
    }

    __static_inline int
    _xskt_shutdown
        (
            XSKT_SOCKET                 sock,
            int                         how
        )
    {
        return sock->ops->shutdown(sock, how);
    }
#endif

#endif
