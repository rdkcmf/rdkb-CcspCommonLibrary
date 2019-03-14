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

    module:	ansc_todco_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the TOD Client Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/14/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_TODCO_INTERFACE_
#define  _ANSC_TODCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "ansc_ifo_tod.h"
#include "ansc_scuowo_interface.h"


/***********************************************************
      PLATFORM INDEPENDENT TOD CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_TODCO_MAX_MESSAGE_SIZE                64
#define  ANSC_TODCO_DEF_SYNC_TIMEOUT                120 * 1000  /* 2 minutes, in milli-seconds */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_TODCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TODCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_TODCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TODCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  PUCHAR
(*PFN_TODCO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TODCO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  USHORT
(*PFN_TODCO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TODCO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

typedef  ULONG
(*PFN_TODCO_GET_RESULT)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulTimestamp
    );

typedef  ANSC_STATUS
(*PFN_TODCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TODCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TODCO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_TODCO_SYNCHRONIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimeoutInSec
    );

/*
 * Hosts on the ARPA Internet that choose to implement a Time Protocol are expected to adopt and
 * implement this standard. This protocol provides a site-independent, machine readable date and
 * time. The Time service sends back to the originating source the time in seconds since midnight
 * on January first 1900. One motivation arises from the fact that not all systems have a date/time
 * clock, and all are subject to occasional human or machine error.  The use of time-servers makes
 * it possible to quickly confirm or correct a system's idea of the time, by making a brief poll of
 * several independent sites on the network.
 */
#define  ANSC_TOD_CLIENT_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSyncTimerObj;                                          \
    ANSC_HANDLE                     hSyncTimerIf;                                           \
    ANSC_HANDLE                     hSimpleClientUdp;                                       \
    ANSC_HANDLE                     hClientUdpWorker;                                       \
    ANSC_HANDLE                     hAnscTodIf;                                             \
    ULONG                           SyncResult;                                             \
    ULONG                           Timestamp;                                              \
                                                                                            \
    PFN_TODCO_GET_ADDR              GetClientAddress;                                       \
    PFN_TODCO_SET_ADDR              SetClientAddress;                                       \
    PFN_TODCO_GET_PORT              GetClientPort;                                          \
    PFN_TODCO_SET_PORT              SetClientPort;                                          \
    PFN_TODCO_GET_ADDR              GetServerAddress;                                       \
    PFN_TODCO_SET_ADDR              SetServerAddress;                                       \
    PFN_TODCO_GET_PORT              GetServerPort;                                          \
    PFN_TODCO_SET_PORT              SetServerPort;                                          \
                                                                                            \
    PFN_TODCO_GET_CONTEXT           GetSimpleClientUdp;                                     \
    PFN_TODCO_GET_CONTEXT           GetClientUdpWorker;                                     \
    PFN_TODCO_GET_IF                GetAnscTodIf;                                           \
    PFN_TODCO_SET_IF                SetAnscTodIf;                                           \
    PFN_TODCO_GET_RESULT            GetSyncResult;                                          \
                                                                                            \
    PFN_TODCO_ENGAGE                Engage;                                                 \
    PFN_TODCO_CANCEL                Cancel;                                                 \
    PFN_TODCO_INVOKE                SyncTimerInvoke;                                        \
    PFN_TODCO_SYNCHRONIZE           Synchronize;                                            \
                                                                                            \
    PFN_SCUOWO_QUERY                ScuowoQuery;                                            \
    PFN_SCUOWO_PROCESS              ScuowoProcess;                                          \
    PFN_SCUOWO_NOTIFY               ScuowoNotify;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_TOD_CLIENT_OBJECT
{
    ANSC_TOD_CLIENT_CLASS_CONTENT
}
ANSC_TOD_CLIENT_OBJECT,  *PANSC_TOD_CLIENT_OBJECT;

#define  ACCESS_ANSC_TOD_CLIENT_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_TOD_CLIENT_OBJECT, Linkage)


#endif
