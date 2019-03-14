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

    module:	slap_eco_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Env Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_ECO_INTERFACE_
#define  _SLAP_ECO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "slap_properties.h"
#include "slap_ifo_uoa.h"
#include "slap_ifo_bss.h"


/***********************************************************
        SLAP ENV CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_ECO_OCO_TABLE_SIZE                    64

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPECO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPECO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  BOOL
(*PFN_SLAPECO_GET_CALL_INFO)
    (
        ANSC_HANDLE                 hThisObject,
        char**                      call_name,
        ULONG*                      call_timestamp,     /* in number of seconds after system is up */
        ULONG*                      call_age            /* in number of seconds */
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_SETUP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPECO_GET_OCO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_ADD_OCO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_DEL_OCO1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       container_name
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_DEL_OCO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContainer
    );

typedef  ANSC_STATUS
(*PFN_SLAPECO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The Slap Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * slap object loading, the concept of "Environment Control" is really important.
 */
#define  SLAP_ENV_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLAP_ENV_CONTROLLER_PROPERTY    Property;                                               \
    ANSC_HANDLE                     hSlapUoaIf;                                             \
    ANSC_HANDLE                     hSlapBssIf;                                             \
    ANSC_HANDLE                     hSlapObjMapper;                                         \
    ANSC_HANDLE                     hSlapVarConverter;                                      \
    ANSC_HANDLE                     hSlapVarMapper;                                         \
    ANSC_HANDLE                     hSlapDefContainer;                                      \
    BOOL                            bActive;                                                \
                                                                                            \
    QUEUE_HEADER                    OcoTable[SLAP_ECO_OCO_TABLE_SIZE];                      \
    ANSC_LOCK                       OcoTableLock;                                           \
                                                                                            \
    BOOL                            bCallPending;                                           \
    char                            PendingCallName[256];                                   \
    ULONG                           PendingCallTimestamp;                                   \
                                                                                            \
    PFN_SLAPECO_GET_IF              GetSlapUoaIf;                                           \
    PFN_SLAPECO_GET_IF              GetSlapBssIf;                                           \
    PFN_SLAPECO_GET_CONTEXT         GetSlapObjMapper;                                       \
    PFN_SLAPECO_GET_CONTEXT         GetSlapVarConverter;                                    \
    PFN_SLAPECO_GET_CONTEXT         GetSlapVarMapper;                                       \
    PFN_SLAPECO_GET_CONTEXT         GetSlapDefContainer;                                    \
    PFN_SLAPECO_GET_CALL_INFO       GetPendingCallInfo;                                     \
                                                                                            \
    PFN_SLAPECO_GET_PROPERTY        GetProperty;                                            \
    PFN_SLAPECO_SET_PROPERTY        SetProperty;                                            \
    PFN_SLAPECO_RESET               ResetProperty;                                          \
    PFN_SLAPECO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPECO_ENGAGE              Engage;                                                 \
    PFN_SLAPECO_CANCEL              Cancel;                                                 \
    PFN_SLAPECO_SETUP               SetupEnv;                                               \
                                                                                            \
    PFN_SLAPECO_GET_OCO             GetSlapContainer;                                       \
    PFN_SLAPECO_ADD_OCO             AddSlapContainer;                                       \
    PFN_SLAPECO_DEL_OCO1            DelSlapContainer1;                                      \
    PFN_SLAPECO_DEL_OCO2            DelSlapContainer2;                                      \
    PFN_SLAPECO_DEL_ALL             DelAllSlapContainers;                                   \
                                                                                            \
    PFN_SLAPUOAIF_CREATE_CON        UoaCreateContainer;                                     \
    PFN_SLAPUOAIF_DELETE_CON        UoaDeleteContainer;                                     \
    PFN_SLAPUOAIF_ACQ_CON           UoaAcqContainerAccess;                                  \
    PFN_SLAPUOAIF_REL_CON           UoaRelContainerAccess;                                  \
                                                                                            \
    PFN_SLAPUOAIF_CREATE_OBJ        UoaCreateObject;                                        \
    PFN_SLAPUOAIF_DELETE_OBJ        UoaDeleteObject;                                        \
    PFN_SLAPUOAIF_DELETE_OBJ2       UoaDeleteObject2;                                       \
    PFN_SLAPUOAIF_INVOKE_OBJ        UoaInvokeObject;                                        \
    PFN_SLAPUOAIF_GET_CONTEXT       UoaGetObjInsContext;                                    \
    PFN_SLAPUOAIF_SET_CONTEXT       UoaSetObjInsContext;                                    \
    PFN_SLAPUOAIF_GET_OPTION        UoaGetObjCallOption;                                    \
    PFN_SLAPUOAIF_ACQ_OBJ           UoaAcqObjectAccess;                                     \
    PFN_SLAPUOAIF_REL_OBJ           UoaRelObjectAccess;                                     \
                                                                                            \
    PFN_SLAPUOAIF_ENROLL_OBJ        UoaEnrollObjDescriptor;                                 \
    PFN_SLAPUOAIF_UNBIND_OBJ        UoaUnbindObjDescriptor;                                 \
    PFN_SLAPUOAIF_VERIFY_OBJ        UoaVerifyObjDescriptor;                                 \
                                                                                            \
    PFN_SLAPUOAIF_CREATE_OBJ        UoaCreateProxyObject;                                   \
    PFN_SLAPUOAIF_ENROLL_OBJ        UoaEnrollProxyObject;                                   \
    PFN_SLAPUOAIF_UNBIND_OBJ        UoaUnbindProxyObject;                                   \
    PFN_SLAPUOAIF_VERIFY_OBJ        UoaVerifyProxyObject;                                   \
                                                                                            \
    PFN_SLAPBSSIF_GET_FLAG          BssGetPoolingFlag;                                      \
    PFN_SLAPBSSIF_SET_FLAG          BssSetPoolingFlag;                                      \
    PFN_SLAPBSSIF_GET_SIZE          BssGetDefPoolSize;                                      \
    PFN_SLAPBSSIF_SET_SIZE          BssSetDefPoolSize;                                      \
    PFN_SLAPBSSIF_GET_CONTEXT       BssGetVarConverter;                                     \
    PFN_SLAPBSSIF_GET_CONTEXT       BssGetVarMapper;                                        \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_ENV_CONTROLLER_OBJECT
{
    SLAP_ENV_CONTROLLER_CLASS_CONTENT
}
SLAP_ENV_CONTROLLER_OBJECT,  *PSLAP_ENV_CONTROLLER_OBJECT;

#define  ACCESS_SLAP_ENV_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, SLAP_ENV_CONTROLLER_OBJECT, Linkage)


#endif
