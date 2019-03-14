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

    module:	ansc_ipc_transmitter_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Ansc IPC Transmitter Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/23/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_IPC_TRANSMITTER_INTERFACE_
#define  _ANSC_IPC_TRANSMITTER_INTERFACE_


/***********************************************************
    PLATFORM DEPENDENT PORT TRANSMITTER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the container object definition.
 */
typedef  ANSC_STATUS
(*PFN_ANSC_ASYNC_NOTIFY)
    (
        ANSC_HANDLE                 hAsyncContext,
        ANSC_STATUS                 asyncStatus
    );

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_IPCT_GET_PORT_BYNAME)
    (
        ANSC_HANDLE                 hOwnerContext,
        char*                       name
    );

typedef  ANSC_HANDLE
(*PFN_IPCT_GET_PORT_BYPID)
    (
        ANSC_HANDLE                 hOwnerContext,
        ULONG                       pid
    );

typedef  ANSC_STATUS
(*PFN_IPCT_SETUP_CONNECTION)
    (
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hSrcPort,
        ANSC_HANDLE                 hDstPort
    );

typedef  ANSC_STATUS
(*PFN_IPCT_RESET_CONNECTION)
    (
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hSrcPort,
        ANSC_HANDLE                 hDstPort
    );

typedef  ULONG
(*PFN_IPCT_QUERY_CONNECTION)
    (
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hSrcPort,
        ANSC_HANDLE                 hDstPort
    );

typedef  ANSC_STATUS
(*PFN_IPCT_EXCHANGE)
    (
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hSrcPort,
        ANSC_HANDLE                 hDstPort,
        PVOID                       pSrcbuffer,
        ULONG                       ulSrcSize,
        PVOID                       pDstbuffer,
        ULONG                       ulDstSize
    );

typedef  ANSC_STATUS
(*PFN_IPCT_EXCHANGE_ASYNC)
    (
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hSrcPort,
        ANSC_HANDLE                 hDstPort,
        PVOID                       pSrcbuffer,
        ULONG                       ulSrcSize,
        PVOID                       pDstbuffer,
        ULONG                       ulDstSize,
        PFN_ANSC_ASYNC_NOTIFY       pfnAsyncNotify,
        ANSC_HANDLE                 hAsyncContext
    );

/*
 * The transportantation mechanism used for IPC is different on different operating systems. On
 * Microsoft windows platforms, message queue, mailbox are primary IPC mechansims for user-mode
 * applications while device I/O control is the primary IPC for mixed communication for two
 * processes in different modes.
 */
#define  ANSC_IPC_TRANSMITTER_CLASS_CONTENT                                                 \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hOwnerContext;                                          \
                                                                                            \
    PFN_IPCT_GET_PORT_BYNAME        GetPortByName;                                          \
    PFN_IPCT_GET_PORT_BYPID         GetPortByPid;                                           \
    PFN_IPCT_SETUP_CONNECTION       SetupConnection;                                        \
    PFN_IPCT_RESET_CONNECTION       ResetConnection;                                        \
    PFN_IPCT_QUERY_CONNECTION       QueryConnection;                                        \
                                                                                            \
    PFN_IPCT_EXCHANGE               Exchange;                                               \
    PFN_IPCT_EXCHANGE_ASYNC         ExchangeAsync;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_IPC_TRANSMITTER
{
    ANSC_IPC_TRANSMITTER_CLASS_CONTENT
}
ANSC_IPC_TRANSMITTER,  *PANSC_IPC_TRANSMITTER;


#endif
