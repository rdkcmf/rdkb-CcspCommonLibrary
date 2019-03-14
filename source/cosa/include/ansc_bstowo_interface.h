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

    module:	ansc_bstowo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Broker Server Tcp Worker Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_BSTOWO_INTERFACE_
#define  _ANSC_BSTOWO_INTERFACE_


/***********************************************************
     PLATFORM INDEPENDENT BSTO WORKER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_BSTOWO_PMODE_DISCARD                  1
#define  ANSC_BSTOWO_PMODE_COLLECT                  2
#define  ANSC_BSTOWO_PMODE_PROCESS_SYNC             3
#define  ANSC_BSTOWO_PMODE_PROCESS_ASYNC            4
#define  ANSC_BSTOWO_PMODE_FINISH                   5

#define  ANSC_BSTOWO_EVENT_SOCKET_ERROR             1
#define  ANSC_BSTOWO_EVENT_SOCKET_CLOSED            2
#define  ANSC_BSTOWO_EVENT_RESOURCES                3
#define  ANSC_BSTOWO_EVENT_TIME_OUT                 4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BSTOWO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTOWO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_BSTOWO_INIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BSTOWO_UNLOAD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_BSTOWO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

typedef  ANSC_STATUS
(*PFN_BSTOWO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ULONG
(*PFN_BSTOWO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

typedef  ANSC_STATUS
(*PFN_BSTOWO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

typedef  ANSC_STATUS
(*PFN_BSTOWO_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

typedef  ANSC_STATUS
(*PFN_BSTOWO_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

/*
 * Tcp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Tcp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Broker Tcp Object MUST operate in a multi-tasking capable environment.
 * It opens a Tcp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_BSTO_WORKER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWorkerContext;                                         \
                                                                                            \
    PFN_BSTOWO_INIT                 Init;                                                   \
    PFN_BSTOWO_UNLOAD               Unload;                                                 \
                                                                                            \
    PFN_BSTOWO_ACCEPT               Accept;                                                 \
    PFN_BSTOWO_REMOVE               Remove;                                                 \
    PFN_BSTOWO_QUERY                Query;                                                  \
    PFN_BSTOWO_PROCESS              ProcessSync;                                            \
    PFN_BSTOWO_PROCESS              ProcessAsync;                                           \
    PFN_BSTOWO_COMPLETE             SendComplete;                                           \
    PFN_BSTOWO_NOTIFY               Notify;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_BSTO_WORKER_OBJECT
{
    ANSC_BSTO_WORKER_CLASS_CONTENT
}
ANSC_BSTO_WORKER_OBJECT,  *PANSC_BSTO_WORKER_OBJECT;

#define  ACCESS_ANSC_BSTO_WORKER_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_BSTO_WORKER_OBJECT, Linkage)


#endif
