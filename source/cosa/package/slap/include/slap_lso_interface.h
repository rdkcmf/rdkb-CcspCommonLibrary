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

    module:	slap_lso_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Loam Server Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/02/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_LSO_INTERFACE_
#define  _SLAP_LSO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_ifo_cpc.h"

#include "slap_properties.h"


/***********************************************************
         SLAP LOAM SERVER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPLSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  BOOL
(*PFN_SLAPLSO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bMobilityOn
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_SETUP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLSO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLpcPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        void**                      ppOutputBuffer,
        PULONG                      pulOutputSize,
        PULONG                      pulErrorCode
    );

/*
 * The Slap Loam Server Object is responsible for processing IMCP-based SLAP object access and
 * management requests. It serves as a proxy and shields the underlying SLAP_UOA implementation
 * from having to deal with the various IPC/LPC/RPC situations. In addition to communicateing with
 * the Slap Loam Client Object directly, the Loam Server also interacts with the Loam Manager to
 * resolve the local SLAP object name.
 */
#define  SLAP_LOAM_SERVER_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLAP_LOAM_SERVER_PROPERTY       Property;                                               \
    ANSC_HANDLE                     hAnscCpcIf;                                             \
    ANSC_HANDLE                     hAnscLpcConnector;                                      \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SLAPLSO_GET_IF              GetAnscCpcIf;                                           \
    PFN_SLAPLSO_GET_CONTEXT         GetAnscLpcConnector;                                    \
    PFN_SLAPLSO_SET_CONTEXT         SetAnscLpcConnector;                                    \
    PFN_SLAPLSO_GET_IF              GetSlapUoaIf;                                           \
    PFN_SLAPLSO_SET_IF              SetSlapUoaIf;                                           \
    PFN_SLAPLSO_GET_BOOL            GetMobilityOn;                                          \
    PFN_SLAPLSO_SET_BOOL            SetMobilityOn;                                          \
                                                                                            \
    PFN_SLAPLSO_GET_PROPERTY        GetProperty;                                            \
    PFN_SLAPLSO_SET_PROPERTY        SetProperty;                                            \
    PFN_SLAPLSO_RESET               ResetProperty;                                          \
    PFN_SLAPLSO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPLSO_ENGAGE              Engage;                                                 \
    PFN_SLAPLSO_CANCEL              Cancel;                                                 \
    PFN_SLAPLSO_SETUP               SetupEnv;                                               \
                                                                                            \
    PFN_SLAPLSO_PROCESS             ProcessCallAcqc;                                        \
    PFN_SLAPLSO_PROCESS             ProcessCallRelc;                                        \
    PFN_SLAPLSO_PROCESS             ProcessCallLoco;                                        \
    PFN_SLAPLSO_PROCESS             ProcessCallDelo;                                        \
    PFN_SLAPLSO_PROCESS             ProcessCallInvo;                                        \
    PFN_SLAPLSO_PROCESS             ProcessCallAcqo;                                        \
    PFN_SLAPLSO_PROCESS             ProcessCallRelo;                                        \
                                                                                            \
    PFN_ANSCCPCIF_DISPATCH          CpcCallDispatch;                                        \
    PFN_ANSCCPCIF_FREE              CpcFreeOutputBuffer;                                    \
    PFN_ANSCCPCIF_NOTIFY            CpcNotifyEvent;                                         \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_LOAM_SERVER_OBJECT
{
    SLAP_LOAM_SERVER_CLASS_CONTENT
}
SLAP_LOAM_SERVER_OBJECT,  *PSLAP_LOAM_SERVER_OBJECT;

#define  ACCESS_SLAP_LOAM_SERVER_OBJECT(p)          \
         ACCESS_CONTAINER(p, SLAP_LOAM_SERVER_OBJECT, Linkage)


#endif
