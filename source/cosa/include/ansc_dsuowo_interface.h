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

    module:	ansc_dsuowo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Daemon Server Udp Worker Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/10/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_DSUOWO_INTERFACE_
#define  _ANSC_DSUOWO_INTERFACE_


/***********************************************************
     PLATFORM INDEPENDENT DSUO WORKER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_DSUOWO_PMODE_DISCARD                  1
#define  ANSC_DSUOWO_PMODE_PROCESS_SYNC             3
#define  ANSC_DSUOWO_PMODE_PROCESS_ASYNC            4
#define  ANSC_DSUOWO_PMODE_FINISH                   5

#define  ANSC_DSUOWO_EVENT_SOCKET_ERROR             1
#define  ANSC_DSUOWO_EVENT_SOCKET_CLOSED            2
#define  ANSC_DSUOWO_EVENT_RESOURCES                3
#define  ANSC_DSUOWO_EVENT_TIME_OUT                 4

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSUOWO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSUOWO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_DSUOWO_INIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSUOWO_UNLOAD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_DSUOWO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

typedef  ANSC_STATUS
(*PFN_DSUOWO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ULONG
(*PFN_DSUOWO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_DSUOWO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_DSUOWO_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

typedef  ANSC_STATUS
(*PFN_DSUOWO_NOTIFY)
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
#define  ANSC_DSUO_WORKER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWorkerContext;                                         \
                                                                                            \
    PFN_DSUOWO_INIT                 Init;                                                   \
    PFN_DSUOWO_UNLOAD               Unload;                                                 \
                                                                                            \
    PFN_DSUOWO_ACCEPT               Accept;                                                 \
    PFN_DSUOWO_REMOVE               Remove;                                                 \
    PFN_DSUOWO_QUERY                Query;                                                  \
    PFN_DSUOWO_PROCESS              ProcessSync;                                            \
    PFN_DSUOWO_PROCESS              ProcessAsync;                                           \
    PFN_DSUOWO_COMPLETE             SendComplete;                                           \
    PFN_DSUOWO_NOTIFY               Notify;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_DSUO_WORKER_OBJECT
{
    ANSC_DSUO_WORKER_CLASS_CONTENT
}
ANSC_DSUO_WORKER_OBJECT,  *PANSC_DSUO_WORKER_OBJECT;

#define  ACCESS_ANSC_DSUO_WORKER_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_DSUO_WORKER_OBJECT, Linkage)


#endif
