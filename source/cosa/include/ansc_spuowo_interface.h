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

    module:	ansc_spuowo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Proxy Udp Worker Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/13/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_SPUOWO_INTERFACE_
#define  _ANSC_SPUOWO_INTERFACE_


/***********************************************************
     PLATFORM INDEPENDENT SPUO WORKER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SPUOWO_PMODE_DISCARD                  1
#define  ANSC_SPUOWO_PMODE_PROCESS_SYNC             3
#define  ANSC_SPUOWO_PMODE_PROCESS_ASYNC            4
#define  ANSC_SPUOWO_PMODE_FINISH                   5

#define  ANSC_SPUOWO_EVENT_SOCKET_ERROR             1
#define  ANSC_SPUOWO_EVENT_SOCKET_CLOSED            2
#define  ANSC_SPUOWO_EVENT_RESOURCES                3
#define  ANSC_SPUOWO_EVENT_TIME_OUT                 4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SPUOWO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUOWO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_SPUOWO_INIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPUOWO_UNLOAD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_SPUOWO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

typedef  ANSC_STATUS
(*PFN_SPUOWO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ULONG
(*PFN_SPUOWO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SPUOWO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SPUOWO_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

typedef  ANSC_STATUS
(*PFN_SPUOWO_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

/*
 * Udp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Udp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Daemon Udp Object MUST operate in a multi-tasking capable environment.
 * It opens a Udp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_SPUO_WORKER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWorkerContext;                                         \
                                                                                            \
    PFN_SPUOWO_INIT                 Init;                                                   \
    PFN_SPUOWO_UNLOAD               Unload;                                                 \
                                                                                            \
    PFN_SPUOWO_ACCEPT               PrvAccept;                                              \
    PFN_SPUOWO_REMOVE               PrvRemove;                                              \
    PFN_SPUOWO_QUERY                PrvQuery;                                               \
    PFN_SPUOWO_PROCESS              PrvProcessSync;                                         \
    PFN_SPUOWO_PROCESS              PrvProcessAsync;                                        \
    PFN_SPUOWO_COMPLETE             PrvSendComplete;                                        \
    PFN_SPUOWO_NOTIFY               PrvNotify;                                              \
                                                                                            \
    PFN_SPUOWO_REMOVE               PubRemove;                                              \
    PFN_SPUOWO_QUERY                PubQuery;                                               \
    PFN_SPUOWO_PROCESS              PubProcessSync;                                         \
    PFN_SPUOWO_PROCESS              PubProcessAsync;                                        \
    PFN_SPUOWO_COMPLETE             PubSendComplete;                                        \
    PFN_SPUOWO_NOTIFY               PubNotify;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SPUO_WORKER_OBJECT
{
    ANSC_SPUO_WORKER_CLASS_CONTENT
}
ANSC_SPUO_WORKER_OBJECT,  *PANSC_SPUO_WORKER_OBJECT;

#define  ACCESS_ANSC_SPUO_WORKER_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_SPUO_WORKER_OBJECT, Linkage)


#endif
