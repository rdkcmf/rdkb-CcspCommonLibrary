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

    module:	ansc_sptowo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Proxy Tcp Worker Object.

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


#ifndef  _ANSC_SPTOWO_INTERFACE_
#define  _ANSC_SPTOWO_INTERFACE_


/***********************************************************
     PLATFORM INDEPENDENT SPTO WORKER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SPTOWO_PMODE_DISCARD                  1
#define  ANSC_SPTOWO_PMODE_COLLECT                  2
#define  ANSC_SPTOWO_PMODE_PROCESS_SYNC             3
#define  ANSC_SPTOWO_PMODE_PROCESS_ASYNC            4
#define  ANSC_SPTOWO_PMODE_FINISH                   5

#define  ANSC_SPTOWO_EVENT_SOCKET_ERROR             1
#define  ANSC_SPTOWO_EVENT_SOCKET_CLOSED            2
#define  ANSC_SPTOWO_EVENT_RESOURCES                3
#define  ANSC_SPTOWO_EVENT_TIME_OUT                 4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SPTOWO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_INIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_UNLOAD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_SPTOWO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_SETOUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ULONG
(*PFN_SPTOWO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

typedef  ANSC_STATUS
(*PFN_SPTOWO_NOTIFY)
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
 * server applications. The Daemon Tcp Object MUST operate in a multi-tasking capable environment.
 * It opens a Tcp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_SPTO_WORKER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWorkerContext;                                         \
                                                                                            \
    PFN_SPTOWO_INIT                 Init;                                                   \
    PFN_SPTOWO_UNLOAD               Unload;                                                 \
                                                                                            \
    PFN_SPTOWO_ACCEPT               PrvAccept;                                              \
    PFN_SPTOWO_SETOUT               PrvSetOut;                                              \
    PFN_SPTOWO_REMOVE               PrvRemove;                                              \
    PFN_SPTOWO_QUERY                PrvQuery;                                               \
    PFN_SPTOWO_PROCESS              PrvProcessSync;                                         \
    PFN_SPTOWO_PROCESS              PrvProcessAsync;                                        \
    PFN_SPTOWO_COMPLETE             PrvSendComplete;                                        \
    PFN_SPTOWO_NOTIFY               PrvNotify;                                              \
                                                                                            \
    PFN_SPTOWO_ACCEPT               PubAccept;                                              \
    PFN_SPTOWO_REMOVE               PubRemove;                                              \
    PFN_SPTOWO_QUERY                PubQuery;                                               \
    PFN_SPTOWO_PROCESS              PubProcessSync;                                         \
    PFN_SPTOWO_PROCESS              PubProcessAsync;                                        \
    PFN_SPTOWO_COMPLETE             PubSendComplete;                                        \
    PFN_SPTOWO_NOTIFY               PubNotify;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SPTO_WORKER_OBJECT
{
    ANSC_SPTO_WORKER_CLASS_CONTENT
}
ANSC_SPTO_WORKER_OBJECT,  *PANSC_SPTO_WORKER_OBJECT;

#define  ACCESS_ANSC_SPTO_WORKER_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_SPTO_WORKER_OBJECT, Linkage)


#endif
