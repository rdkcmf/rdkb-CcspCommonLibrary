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

    module:	stun_ifo_bsm.h

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Binding State Monitor Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/29/08    initial revision.

**********************************************************************/


#ifndef  _STUN_IFO_BSM_
#define  _STUN_IFO_BSM_

#include "ansc_ifo_interface.h"

/***********************************************************
       STUN BINDING STATE MONITOR INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  STUN_BSM_INTERFACE_NAME                    "stunBindingStateMonitorIf"
#define  STUN_BSM_INTERFACE_ID                      0

#define  STUN_BSM_EVENT_serverUnreachable           1
#define  STUN_BSM_EVENT_errorReturned               2
#define  STUN_BSM_EVENT_unknownMsgReceived          3
#define  STUN_BSM_EVENT_bindingRemoved              4
#define  STUN_BSM_EVENT_bindingChanged              5

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_STUNBSMIF_RECV)
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

typedef  ANSC_STATUS
(*PFN_STUNBSMIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

/*
 * As a general requirement, each module SHOULD provide an interface for external components to
 * configure its parameters and policies. Although the benefit of unifying the definition and usage
 * of such an interface is obvious, we DON'T want to impose any set of rules on the implementation.
 * Instead, we expect every module will implement its configuration interfaces independently.
 */
#define  STUN_BSM_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_STUNBSMIF_RECV              RecvMsg1;                                               \
    PFN_STUNBSMIF_RECV              RecvMsg2;                                               \
    PFN_STUNBSMIF_NOTIFY            Notify;                                                 \
    /* end of object class content */                                                       \

typedef  struct
_STUN_BSM_INTERFACE
{
    STUN_BSM_INTERFACE_CLASS_CONTENT
}
STUN_BSM_INTERFACE,  *PSTUN_BSM_INTERFACE;

#define  ACCESS_STUN_BSM_INTERFACE(p)               \
         ACCESS_CONTAINER(p, STUN_BSM_INTERFACE, Linkage)


#endif
