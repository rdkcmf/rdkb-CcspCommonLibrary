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

    module:	bss_ifo_bsd.h

        For BroadWay Socket Service Implementation (BSS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Berkeley Software Distribution (BSD)
        Socket Interface Objects.

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


#ifndef  _BSS_IFO_BSD_
#define  _BSS_IFO_BSD_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"
#include "bss_definitions.h"


/***********************************************************
   BSS BERKELEY SOFTWARE DISTRIBUTION INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BSS_BSD_INTERFACE_NAME                     "bssBsdIf"
#define  BSS_BSD_INTERFACE_ID                       0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BSS_BSD_SOCKET
(*PFN_BSSBSDIF_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        ANSC_HANDLE                 hAddress,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_BIND)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        ANSC_HANDLE                 hAddress,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_CLOSESOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_CONNECT)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        ANSC_HANDLE                 hAddress,
        PULONG                      pulErrorCode
    );

typedef  ANSC_HANDLE
(*PFN_BSSBSDIF_GETHOSTBYADDR)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       address,
        int                         len,
        int                         type,
        PULONG                      pulErrorCode
    );

typedef  ANSC_HANDLE
(*PFN_BSSBSDIF_GETHOSTBYNAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulErrorCode
    );

typedef  char*
(*PFN_BSSBSDIF_GETHOSTNAME)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_GETPEERNAME)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        ANSC_HANDLE                 hAddress,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_GETSOCKNAME)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        ANSC_HANDLE                 hAddress,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_GETSOCKOPT)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        int                         level,
        int                         optname,
        char*                       optval,
        int*                        optlen,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_IOCTLSOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        long                        command,
        PULONG                      argp,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_LISTEN)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        int                         backlog,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        PVOID                       buffer,
        PULONG                      pulSize,
        ULONG                       ulFlags,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_RECVFROM)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        PVOID                       buffer,
        PULONG                      pulSize,
        ULONG                       ulFlags,
        ANSC_HANDLE                 hAddress,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_SELECT)
    (
        ANSC_HANDLE                 hThisObject,
        int                         nfds,
        ANSC_HANDLE                 hRdFds,
        ANSC_HANDLE                 hWrFds,
        ANSC_HANDLE                 hExFds,
        ANSC_HANDLE                 hTimeout,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_SEND)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        PVOID                       buffer,
        PULONG                      pulSize,
        ULONG                       ulFlags,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_SENDTO)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        PVOID                       buffer,
        PULONG                      pulSize,
        ULONG                       ulFlags,
        ANSC_HANDLE                 hAddress,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_SETSOCKOPT)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        int                         level,
        int                         optname,
        char*                       optval,
        int                         optlen,
        PULONG                      pulErrorCode
    );

typedef  ANSC_STATUS
(*PFN_BSSBSDIF_SHUTDOWN)
    (
        ANSC_HANDLE                 hThisObject,
        BSS_BSD_SOCKET              socket,
        int                         how,
        PULONG                      pulErrorCode
    );

typedef  BSS_BSD_SOCKET
(*PFN_BSSBSDIF_SOCKET)
    (
        ANSC_HANDLE                 hThisObject,
        int                         family,
        int                         type,
        int                         protocol,
        PULONG                      pulErrorCode
    );

/*
 * BroadWay Socket Service is an interface, not a protocol. As an interface, it is used to discover
 * and utilize the communications capabilities of any number of underlying transport protocols.
 * Because it is not a protocol, it does not in any way affect the bits on the wire, and does not
 * need to be utilized on both ends of a communications link. If you're familiar with Berkeley
 * Software Distribution (BSD) UNIX, you will find that the BroadWay Socket interface is very easy
 * to work with. Its programming is also centered around TCP/IP.
 */
#define  BSS_BSD_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BSSBSDIF_ACCEPT             Accept;                                                 \
    PFN_BSSBSDIF_BIND               Bind;                                                   \
    PFN_BSSBSDIF_CLOSESOCKET        CloseSocket;                                            \
    PFN_BSSBSDIF_CONNECT            Connect;                                                \
    PFN_BSSBSDIF_GETHOSTBYADDR      GetHostByAddr;                                          \
    PFN_BSSBSDIF_GETHOSTBYNAME      GetHostByName;                                          \
    PFN_BSSBSDIF_GETHOSTNAME        GetHostName;                                            \
    PFN_BSSBSDIF_GETPEERNAME        GetPeerName;                                            \
    PFN_BSSBSDIF_GETSOCKNAME        GetSockName;                                            \
    PFN_BSSBSDIF_GETSOCKOPT         GetSockOpt;                                             \
    PFN_BSSBSDIF_IOCTLSOCKET        IoctlSocket;                                            \
    PFN_BSSBSDIF_LISTEN             Listen;                                                 \
    PFN_BSSBSDIF_RECV               Recv;                                                   \
    PFN_BSSBSDIF_RECVFROM           RecvFrom;                                               \
    PFN_BSSBSDIF_SELECT             Select;                                                 \
    PFN_BSSBSDIF_SEND               Send;                                                   \
    PFN_BSSBSDIF_SENDTO             SendTo;                                                 \
    PFN_BSSBSDIF_SETSOCKOPT         SetSockOpt;                                             \
    PFN_BSSBSDIF_SHUTDOWN           Shutdown;                                               \
    PFN_BSSBSDIF_SOCKET             Socket;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_BSS_BSD_INTERFACE
{
    BSS_BSD_INTERFACE_CLASS_CONTENT
}
BSS_BSD_INTERFACE,  *PBSS_BSD_INTERFACE;

#define  ACCESS_BSS_BSD_INTERFACE(p)                \
         ACCESS_CONTAINER(p, BSS_BSD_INTERFACE, Linkage)


#endif
