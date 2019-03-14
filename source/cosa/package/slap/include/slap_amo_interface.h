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

    module:	slap_amo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Access Manager Objects.

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


#ifndef  _SLAP_AMO_INTERFACE_
#define  _SLAP_AMO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "slap_properties.h"
#include "slap_ifo_goa.h"
#include "slap_ifo_poa.h"


/***********************************************************
        SLAP ENV CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPAMO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPAMO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT2)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list,
        ANSC_HANDLE                 hSlapGoaRtc
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT3)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT4)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var,
        ANSC_HANDLE                 hSlapGoaRtc
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT5)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT6)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        SLAP_PARAMETER_LIST*        param_list,
        ANSC_HANDLE                 hSlapGoaRtc
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT7)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

typedef  ANSC_STATUS
(*PFN_SLAPAMO_CONVERT8)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        SLAP_VARIABLE*              slap_var,
        ANSC_HANDLE                 hSlapGoaRtc
    );

/*
 * The Slap Access Manager Object is responsible for setting up the object access environment and
 * providing a generic object access mechanism, which allows the upper-layer modules to access any
 * SLAP object within the system in the same way. Not only it encapsulates the local SLAP object
 * access mechanism implemented by SlapEnvController, but also it may create IMCP LPC-based remote
 * SLAP object access channel.
 */
#define  SLAP_ACCESS_MANAGER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLAP_ACCESS_MANAGER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hSlapGoaIf;                                             \
    ANSC_HANDLE                     hSlapPoaIf;                                             \
    ANSC_HANDLE                     hSlapEnvController;                                     \
    ANSC_HANDLE                     hSlapLoamClient;                                        \
    ANSC_HANDLE                     hSlapLoamServer;                                        \
    ANSC_HANDLE                     hAnscLpcConnector;                                      \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SLAPAMO_GET_IF              GetSlapGoaIf;                                           \
    PFN_SLAPAMO_GET_IF              GetSlapPoaIf;                                           \
    PFN_SLAPAMO_GET_CONTEXT         GetSlapEnvController;                                   \
    PFN_SLAPAMO_GET_CONTEXT         GetSlapLoamClient;                                      \
    PFN_SLAPAMO_GET_CONTEXT         GetSlapLoamServer;                                      \
    PFN_SLAPAMO_GET_CONTEXT         GetAnscLpcConnector;                                    \
                                                                                            \
    PFN_SLAPAMO_GET_PROPERTY        GetProperty;                                            \
    PFN_SLAPAMO_SET_PROPERTY        SetProperty;                                            \
    PFN_SLAPAMO_RESET               ResetProperty;                                          \
    PFN_SLAPAMO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPAMO_ENGAGE              Engage;                                                 \
    PFN_SLAPAMO_CANCEL              Cancel;                                                 \
                                                                                            \
    PFN_SLAPAMO_CONVERT1            GoaToUoaParamList;                                      \
    PFN_SLAPAMO_CONVERT2            UoaToGoaParamList;                                      \
    PFN_SLAPAMO_CONVERT3            GoaToUoaVariable;                                       \
    PFN_SLAPAMO_CONVERT4            UoaToGoaVariable;                                       \
    PFN_SLAPAMO_CONVERT5            GoaToLoaParamList;                                      \
    PFN_SLAPAMO_CONVERT6            LoaToGoaParamList;                                      \
    PFN_SLAPAMO_CONVERT7            GoaToLoaVariable;                                       \
    PFN_SLAPAMO_CONVERT8            LoaToGoaVariable;                                       \
                                                                                            \
    PFN_SLAPGOAIF_GET_MODE          GoaGetAggregationMode;                                  \
    PFN_SLAPGOAIF_SET_MODE          GoaSetAggregationMode;                                  \
    PFN_SLAPGOAIF_GET_PORT          GoaGetLpcPartyPort;                                     \
    PFN_SLAPGOAIF_SET_PORT          GoaSetLpcPartyPort;                                     \
                                                                                            \
    PFN_SLAPGOAIF_CREATE_CON        GoaCreateContainer;                                     \
    PFN_SLAPGOAIF_DELETE_CON        GoaDeleteContainer;                                     \
                                                                                            \
    PFN_SLAPGOAIF_CREATE_OBJ        GoaCreateObject;                                        \
    PFN_SLAPGOAIF_DELETE_OBJ        GoaDeleteObject;                                        \
    PFN_SLAPGOAIF_INVOKE_OBJ        GoaInvokeObject;                                        \
    PFN_SLAPGOAIF_INVOKE_OBJ2       GoaInvokeObject2;                                       \
    PFN_SLAPGOAIF_ATTACH_OBJ        GoaAttachObject;                                        \
    PFN_SLAPGOAIF_DETACH_OBJ        GoaDetachObject;                                        \
    PFN_SLAPGOAIF_UNLINK_OBJ        GoaUnlinkObject;                                        \
    PFN_SLAPGOAIF_ACQ_OBJ           GoaAcqObjectAccess;                                     \
    PFN_SLAPGOAIF_REL_OBJ           GoaRelObjectAccess;                                     \
    PFN_SLAPGOAIF_FLUSH_ALL         GoaFlushAllObjects;                                     \
                                                                                            \
    PFN_SLAPGOAIF_ENROLL_OBJ        GoaEnrollObjDescriptor;                                 \
    PFN_SLAPGOAIF_UNBIND_OBJ        GoaUnbindObjDescriptor;                                 \
    PFN_SLAPGOAIF_VERIFY_OBJ        GoaVerifyObjDescriptor;                                 \
                                                                                            \
    PFN_SLAPGOAIF_ENROLL_OBJ        GoaEnrollMobileObject;                                  \
    PFN_SLAPGOAIF_UNBIND_OBJ        GoaUnbindMobileObject;                                  \
    PFN_SLAPGOAIF_VERIFY_OBJ        GoaVerifyMobileObject;                                  \
                                                                                            \
    PFN_SLAPPOAIF_ACQ_OBJ           PoaAcqObjectAccess;                                     \
    PFN_SLAPPOAIF_REL_OBJ           PoaRelObjectAccess;                                     \
                                                                                            \
    PFN_SLAPPOAIF_CREATE_OBJ        PoaCreateProxyObject;                                   \
    PFN_SLAPPOAIF_ENROLL_OBJ        PoaEnrollProxyObject;                                   \
    PFN_SLAPPOAIF_UNBIND_OBJ        PoaUnbindProxyObject;                                   \
    PFN_SLAPPOAIF_VERIFY_OBJ        PoaVerifyProxyObject;                                   \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_ACCESS_MANAGER_OBJECT
{
    SLAP_ACCESS_MANAGER_CLASS_CONTENT
}
SLAP_ACCESS_MANAGER_OBJECT,  *PSLAP_ACCESS_MANAGER_OBJECT;

#define  ACCESS_SLAP_ACCESS_MANAGER_OBJECT(p)       \
         ACCESS_CONTAINER(p, SLAP_ACCESS_MANAGER_OBJECT, Linkage)


#endif
