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

    module:	ansc_sco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Simplex Connector Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/30/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCO_INTERFACE_
#define  _ANSC_SCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_scopo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT SIMPLEX CONNECTOR OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SCO_MIN_PDO_POOL_SIZE                 32
#define  ANSC_SCO_MAX_PDO_POOL_SIZE                 128
#define  ANSC_SCO_TASK_BREAK_INTERVAL               200         /* in milliseconds */
#define  ANSC_SCO_TASK_CLEANUP_TIME                 3000        /* in milliseconds */

#define  ANSC_SCO_MODE_ASYNC                        0x00000001
#define  ANSC_SCO_MODE_PROXY                        0x00000002

#define  ANSC_SCO_RELAY_TASK_NAME                   "ansc_sco_relay_task"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_SCO_GET_MODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCO_SET_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

typedef  ANSC_STATUS
(*PFN_SCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SCO_ACQUIRE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCO_RELEASE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPdo
    );

typedef  ANSC_STATUS
(*PFN_SCO_OPEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCO_RELAY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRelayContext,
        PVOID                       buffer,
        ULONG                       ulSize
    );

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_SIMPLEX_CONNECTOR_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_SCO_PORT_OBJECT            LastPort;                                               \
    ANSC_SCO_PORT_OBJECT            NextPort;                                               \
    ULONG                           OperationMode;                                          \
    ULONG                           TotalPdoCount;                                          \
    BOOL                            bOpened;                                                \
                                                                                            \
    ANSC_EVENT                      AsyncRelayEvent;                                        \
    QUEUE_HEADER                    RelayPdoQueue;                                          \
    ANSC_LOCK                       RelayPdoQueueLock;                                      \
    SLIST_HEADER                    IdlePdoSList;                                           \
    ANSC_LOCK                       IdlePdoSListLock;                                       \
                                                                                            \
    PFN_SCO_GET_CONTEXT             GetLastPort;                                            \
    PFN_SCO_SET_CONTEXT             SetLastPort;                                            \
    PFN_SCO_GET_CONTEXT             GetNextPort;                                            \
    PFN_SCO_SET_CONTEXT             SetNextPort;                                            \
    PFN_SCO_GET_MODE                GetOperationMode;                                       \
    PFN_SCO_SET_MODE                SetOperationMode;                                       \
    PFN_SCO_RESET                   Reset;                                                  \
                                                                                            \
    PFN_SCO_ACQUIRE                 AcquirePdo;                                             \
    PFN_SCO_RELEASE                 ReleasePdo;                                             \
    PFN_SCO_OPEN                    Open;                                                   \
    PFN_SCO_CLOSE                   Close;                                                  \
                                                                                            \
    PFN_SCO_ASYNC_TASK              AsyncRelayTask;                                         \
    PFN_SCO_RELAY                   Relay;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SIMPLEX_CONNECTOR_OBJECT
{
    ANSC_SIMPLEX_CONNECTOR_CLASS_CONTENT
}
ANSC_SIMPLEX_CONNECTOR_OBJECT,  *PANSC_SIMPLEX_CONNECTOR_OBJECT;

#define  ACCESS_ANSC_SIMPLEX_CONNECTOR_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_SIMPLEX_CONNECTOR_OBJECT, Linkage)


/***********************************************************
  PACKET DESCRIPTOR OBJECT DEFINITION FOR SIMPLEX CONNECTOR
***********************************************************/

/*
 * This object (actually not an executable object, only a data structure) is used internall by
 * Pipe Connector to manage the work flow of the incoming and outgoing packets. Since we don't
 * really want to provide sophisticated buffer management in this object, this data structure
 * is sufficient for the "Connecting/Relaying" purpose.
 */
typedef  struct
_ANSC_SCO_PACKET_DESCRIPTOR
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hContext;
    PVOID                           PacketBuffer;
    ULONG                           PacketSize;
    BOOL                            bOwner;
}
ANSC_SCO_PACKET_DESCRIPTOR,  *PANSC_SCO_PACKET_DESCRIPTOR;

#define  ACCESS_ANSC_SCO_PACKET_DESCRIPTOR(p)       \
         ACCESS_CONTAINER(p, ANSC_SCO_PACKET_DESCRIPTOR, Linkage)


#endif
