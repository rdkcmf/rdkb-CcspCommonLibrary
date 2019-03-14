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

    module:	ansc_sdto_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Server Doctor Tcp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/18/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_SDTO_INTERFACE_
#define  _ANSC_SDTO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_sdtopo_interface.h"


/***********************************************************
  PLATFORM INDEPENDENT SERVER_DOCTOR TCP OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SDTO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SDTO_GET_PATIENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTO_SET_PATIENT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPatient,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SDTO_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTO_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

typedef  ULONG
(*PFN_SDTO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTO_SET_COUNT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       count
    );

typedef  ANSC_STATUS
(*PFN_SDTO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  USHORT
(*PFN_SDTO_GET_PORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTO_SET_PORT)
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

typedef  ANSC_STATUS
(*PFN_SDTO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTOWO_INIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTOWO_UNLOAD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_SDTOWO_ACCEPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

typedef  ANSC_STATUS
(*PFN_SDTOWO_REMOVE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

typedef  ULONG
(*PFN_SDTOWO_QUERY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SDTOWO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_SDTOWO_COMPLETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

typedef  ANSC_STATUS
(*PFN_SDTOWO_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

/*
 * In the process of developing and testing the Daemon Server Tcp/Udp Objects, we encountered
 * several problems that cannot be identified from the external behavior. The normal approach
 * of diagnosing is generating a log file or simply printing debug information on the screen.
 * A better solution is a request/reply based protocol for each specific application. The Server
 * Doctor Tcp/Udp Objects are used for this purpose.
 */
#define  ANSC_SERVER_DOCTOR_TCP_CLASS_CONTENT                                               \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hServerPatient;                                         \
    ANSC_HANDLE                     hDsuoDaemon;                                            \
    ANSC_HANDLE                     hDsuoWorker;                                            \
    USHORT                          DaemonUdpPort;                                          \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SDTO_GET_PATIENT            GetPatient;                                             \
    PFN_SDTO_SET_PATIENT            SetPatient;                                             \
    PFN_SDTO_GET_PORT               GetDaemonPort;                                          \
    PFN_SDTO_SET_PORT               SetDaemonPort;                                          \
    PFN_SDTO_RESET                  Reset;                                                  \
                                                                                            \
    PFN_SDTO_ENGAGE                 Engage;                                                 \
    PFN_SDTO_CANCEL                 Cancel;                                                 \
                                                                                            \
    PFN_SDTOWO_INIT                 WoInit;                                                 \
    PFN_SDTOWO_UNLOAD               WoUnload;                                               \
                                                                                            \
    PFN_SDTOWO_ACCEPT               WoAccept;                                               \
    PFN_SDTOWO_REMOVE               WoRemove;                                               \
    PFN_SDTOWO_QUERY                WoQuery;                                                \
    PFN_SDTOWO_PROCESS              WoProcessSync;                                          \
    PFN_SDTOWO_PROCESS              WoProcessAsync;                                         \
    PFN_SDTOWO_COMPLETE             WoSendComplete;                                         \
    PFN_SDTOWO_NOTIFY               WoNotify;                                               \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SERVER_DOCTOR_TCP_OBJECT
{
    ANSC_SERVER_DOCTOR_TCP_CLASS_CONTENT
}
ANSC_SERVER_DOCTOR_TCP_OBJECT,  *PANSC_SERVER_DOCTOR_TCP_OBJECT;

#define  ACCESS_ANSC_SERVER_DOCTOR_TCP_OBJECT(p)    \
         ACCESS_CONTAINER(p, ANSC_SERVER_DOCTOR_TCP_OBJECT, Linkage)


#endif
