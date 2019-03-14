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

    module:	ansc_scgowo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simple Client Gtp Worker Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/17/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCGOWO_INTERFACE_
#define  _ANSC_SCGOWO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_scgowo_interface.h"


/***********************************************************
     PLATFORM INDEPENDENT SCGO WORKER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SCGOWO_PMODE_DISCARD                  1
#define  ANSC_SCGOWO_PMODE_PROCESS                  3

#define  ANSC_SCGOWO_EVENT_SOCKET_ERROR             1
#define  ANSC_SCGOWO_EVENT_SOCKET_CLOSED            2
#define  ANSC_SCGOWO_EVENT_RESOURCES                3

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SCGOWO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCGOWO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_SCGOWO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SCGOWO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SCGOWO_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

/*
 * Gtp-based Internet Servers have extremely high requirements on performance, processing delay,
 * reliability, and scalability. While the base Ansc Socket Object is OK for most Gtp-based client
 * applications and even some low-end server applications, it's not suitable for high-end Internet
 * server applications. The Daemon Gtp Object MUST operate in a multi-tasking capable environment.
 * It opens a Gtp socket and accepts incoming connection requests. Although some functionalities
 * it provides are already available in the base socket object, this object is NOT derived from
 * the base Ansc Socket Object.
 */
#define  ANSC_SCGO_WORKER_CLASS_CONTENT                                                     \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hWorkerContext;                                         \
                                                                                            \
    PFN_SCGOWO_QUERY                Query;                                                  \
    PFN_SCGOWO_PROCESS              Process;                                                \
    PFN_SCGOWO_NOTIFY               Notify;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SCGO_WORKER_OBJECT
{
    ANSC_SCGO_WORKER_CLASS_CONTENT
}
ANSC_SCGO_WORKER_OBJECT,  *PANSC_SCGO_WORKER_OBJECT;

#define  ACCESS_ANSC_SCGO_WORKER_OBJECT(p)          \
         ACCESS_CONTAINER(p, ANSC_SCGO_WORKER_OBJECT, Linkage)


#endif
