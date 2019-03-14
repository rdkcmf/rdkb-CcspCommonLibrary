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

    module:	ansc_sdtopo_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for Server Doctor Tcp Patient Object.

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


#ifndef  _ANSC_SDTOPO_INTERFACE_
#define  _ANSC_SDTOPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_dsto_properties.h"


/***********************************************************
     PLATFORM INDEPENDENT DSTO WORKER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  ANSC_SDTOPO_EVENT_SOCKET_ERROR             1
#define  ANSC_SDTOPO_EVENT_SOCKET_CLOSED            2
#define  ANSC_SDTOPO_EVENT_RESOURCES                3

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SDTOPO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTOPO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_SDTOPO_OVERVIEW)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );

typedef  ANSC_STATUS
(*PFN_SDTOPO_SNAPSHOT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm,
        ULONG                       ulIndex
    );

typedef  ANSC_STATUS
(*PFN_SDTOPO_TRACE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SDTOPO_CLOSE_UP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm,
        ULONG                       ulIndex
    );

/*
 * In the process of developing and testing the Daemon Server Tcp/Udp Objects, we encountered
 * several problems that cannot be identified from the external behavior. The normal approach
 * of diagnosing is generating a log file or simply printing debug information on the screen.
 * A better solution is a request/reply based protocol for each specific application. The Server
 * Doctor Tcp/Udp Objects are used for this purpose.
 */
#define  ANSC_SDTO_PATIENT_CLASS_CONTENT                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hPatientContext;                                        \
                                                                                            \
    PFN_SDTOPO_OVERVIEW             Overview;                                               \
    PFN_SDTOPO_SNAPSHOT             Snapshot;                                               \
    PFN_SDTOPO_TRACE                TraceTransaction;                                       \
    PFN_SDTOPO_CLOSE_UP             EngineCloseUp;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_SDTO_PATIENT_OBJECT
{
    ANSC_SDTO_PATIENT_CLASS_CONTENT
}
ANSC_SDTO_PATIENT_OBJECT,  *PANSC_SDTO_PATIENT_OBJECT;

#define  ACCESS_ANSC_SDTO_PATIENT_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_SDTO_PATIENT_OBJECT, Linkage)


#endif
