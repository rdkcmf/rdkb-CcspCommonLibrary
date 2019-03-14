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

    module:	slap_lco_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Loam Client Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/30/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_LCO_INTERFACE_
#define  _SLAP_LCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "ansc_ifo_cpc.h"

#include "slap_properties.h"
#include "slap_ifo_loa.h"


/***********************************************************
         SLAP LOAM CLIENT COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SLAP_LCO_OBJECT_STUB_TIMEOUT               3600        /* 1 hour, in seconds          */
#define  SLAP_LCO_PATROL_TIMER_INTERVAL             300 * 1000  /* 5 minutes, in milli-seconds */

#define  SLAP_LCO_OSO_TABLE_SIZE                    64

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPLCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  BOOL
(*PFN_SLAPLCO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bValue
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_SETUP)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_CLOSE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_INVOKE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLCO_GET_OSO1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLCO_GET_OSO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_ADD_OSO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle,
        char*                       obj_name,
        char*                       lpc_party_name,
        ANSC_HANDLE                 lpc_party_addr
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_DEL_OSO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 obj_handle
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_DEL_ALL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_LPC_TO_LOA1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        void*                       pImcpAnswer
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLCO_LPC_TO_LOA2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 obj_handle,
        ULONG                       obj_type,
        char*                       obj_name,
        SLAP_PARAMETER_LIST*        import_param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_LOA_TO_LPC1)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        void*                       pImcpCall
    );

typedef  ANSC_HANDLE
(*PFN_SLAPLCO_LOA_TO_LPC2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 lpc_party_addr,
        ANSC_HANDLE                 obj_handle
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_CONVERT1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_CONVERT2)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_CONVERT3)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

typedef  ANSC_STATUS
(*PFN_SLAPLCO_CONVERT4)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_VARIABLE*              slap_var
    );

/*
 * The Slap Loam Client Object is responsible for accessing SLAP objects created in other address
 * spaces and processes. It communicates with the Slap Loam Server Object to resolve the SLAP
 * object name to the corresponding SLAP module and establishes the LPC communication channcel
 * directly between this SLAP and peer SLAP environments.
 */
#define  SLAP_LOAM_CLIENT_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SLAP_LOAM_CLIENT_PROPERTY       Property;                                               \
    ANSC_HANDLE                     hPatrolTimerObj;                                        \
    ANSC_HANDLE                     hPatrolTimerIf;                                         \
    ANSC_HANDLE                     hSlapLoaIf;                                             \
    ANSC_HANDLE                     hSlapEnvController;                                     \
    ANSC_HANDLE                     hAnscLpcConnector;                                      \
    ANSC_HANDLE                     hMobileContainer;                                       \
    BOOL                            bActive;                                                \
                                                                                            \
    SLIST_HEADER                    OsoTable[SLAP_LCO_OSO_TABLE_SIZE];                      \
    ANSC_LOCK                       OsoTableLock;                                           \
                                                                                            \
    PFN_SLAPLCO_GET_IF              GetSlapLoaIf;                                           \
    PFN_SLAPLCO_GET_CONTEXT         GetSlapEnvController;                                   \
    PFN_SLAPLCO_GET_CONTEXT         GetAnscLpcConnector;                                    \
    PFN_SLAPLCO_SET_CONTEXT         SetAnscLpcConnector;                                    \
    PFN_SLAPLCO_GET_BOOL            GetMobilityOn;                                          \
    PFN_SLAPLCO_SET_BOOL            SetMobilityOn;                                          \
    PFN_SLAPLCO_GET_PROPERTY        GetProperty;                                            \
    PFN_SLAPLCO_SET_PROPERTY        SetProperty;                                            \
    PFN_SLAPLCO_RESET               ResetProperty;                                          \
    PFN_SLAPLCO_RESET               Reset;                                                  \
                                                                                            \
    PFN_SLAPLCO_ENGAGE              Engage;                                                 \
    PFN_SLAPLCO_CANCEL              Cancel;                                                 \
    PFN_SLAPLCO_SETUP               SetupEnv;                                               \
    PFN_SLAPLCO_CLOSE               CloseEnv;                                               \
    PFN_SLAPLCO_INVOKE              PatrolTimerInvoke;                                      \
                                                                                            \
    PFN_SLAPLCO_GET_OSO1            GetObjectStubByName;                                    \
    PFN_SLAPLCO_GET_OSO2            GetObjectStubByHandle;                                  \
    PFN_SLAPLCO_ADD_OSO             AddObjectStub;                                          \
    PFN_SLAPLCO_DEL_OSO             DelObjectStub;                                          \
    PFN_SLAPLCO_DEL_ALL             DelAllObjectStubs;                                      \
                                                                                            \
    PFN_SLAPLCO_LPC_TO_LOA1         LpcToLoaImcpInvoAnswer;                                 \
    PFN_SLAPLCO_LPC_TO_LOA2         LpcToLoaSlapObject;                                     \
    PFN_SLAPLCO_LOA_TO_LPC1         LoaToLpcImcpInvoCall;                                   \
    PFN_SLAPLCO_LOA_TO_LPC2         LoaToLpcSlapObject;                                     \
                                                                                            \
    PFN_SLAPLCO_CONVERT1            LoaToUoaParamList;                                      \
    PFN_SLAPLCO_CONVERT2            UoaToLoaParamList;                                      \
    PFN_SLAPLCO_CONVERT3            LoaToUoaVariable;                                       \
    PFN_SLAPLCO_CONVERT4            UoaToLoaVariable;                                       \
                                                                                            \
    PFN_SLAPLOAIF_ACQ_CON           LoaAcqContainerAccess;                                  \
    PFN_SLAPLOAIF_REL_CON           LoaRelContainerAccess;                                  \
                                                                                            \
    PFN_SLAPLOAIF_LOCATE_OBJ        LoaLocateObject;                                        \
    PFN_SLAPLOAIF_DELETE_OBJ        LoaDeleteObject;                                        \
    PFN_SLAPLOAIF_INVOKE_OBJ        LoaInvokeObject;                                        \
    PFN_SLAPLOAIF_ACQ_OBJ           LoaAcqObjectAccess;                                     \
    PFN_SLAPLOAIF_REL_OBJ           LoaRelObjectAccess;                                     \
    PFN_SLAPLOAIF_REL_OBJ2          LoaRelObjectAccess2;                                    \
    PFN_SLAPLOAIF_FLUSH_ALL         LoaFlushAllObjects;                                     \
                                                                                            \
    PFN_SLAPLOAIF_ENROLL_OBJ        LoaEnrollMobileObject;                                  \
    PFN_SLAPLOAIF_UNBIND_OBJ        LoaUnbindMobileObject;                                  \
    PFN_SLAPLOAIF_VERIFY_OBJ        LoaVerifyMobileObject;                                  \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_LOAM_CLIENT_OBJECT
{
    SLAP_LOAM_CLIENT_CLASS_CONTENT
}
SLAP_LOAM_CLIENT_OBJECT,  *PSLAP_LOAM_CLIENT_OBJECT;

#define  ACCESS_SLAP_LOAM_CLIENT_OBJECT(p)          \
         ACCESS_CONTAINER(p, SLAP_LOAM_CLIENT_OBJECT, Linkage)


/***********************************************************
          SLAP LOAM CLIENT OBJECT STUB DEFINITION
***********************************************************/

/*
 * The LPC-based SLAP access can be very slow due to the significant round-trip time delay for
 * every IMCP message transmitted. To minimize the performance penalty, it's necessary to reduce
 * the number of LPC calls for a typical LOA object access scenario. We use the following data
 * structure to cache the acquired LOA object handle.
 */
typedef  struct
_SLAP_LOA_OBJECT_STUB
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           HashIndex;
    ANSC_HANDLE                     hLoaSlapObject;
    char*                           ObjName;
    char*                           LpcPartyName;
    ANSC_HANDLE                     hLpcPartyAddr;
    ANSC_LOCK                       AccessLock;

    ULONG                           LastAccessAt;
    ULONG                           LastImcpError;
    ULONG                           RefCount;
}
SLAP_LOA_OBJECT_STUB,  *PSLAP_LOA_OBJECT_STUB;

#define  ACCESS_SLAP_LOA_OBJECT_STUB(p)             \
         ACCESS_CONTAINER(p, SLAP_LOA_OBJECT_STUB, Linkage)

#define  SlapLockLoaObjectStub(obj_stub)            AnscAcquireLock(&obj_stub->AccessLock)
#define  SlapUnlockLoaObjectStub(obj_stub)          AnscReleaseLock(&obj_stub->AccessLock)

#define  SlapCleanLoaObjectStub(obj_stub)                                                   \
         {                                                                                  \
            if ( obj_stub->ObjName )                                                        \
            {                                                                               \
                AnscFreeMemory(obj_stub->ObjName);                                          \
                                                                                            \
                obj_stub->ObjName = NULL;                                                   \
            }                                                                               \
                                                                                            \
            if ( obj_stub->LpcPartyName )                                                   \
            {                                                                               \
                AnscFreeMemory(obj_stub->LpcPartyName);                                     \
                                                                                            \
                obj_stub->LpcPartyName = NULL;                                              \
            }                                                                               \
         }

#define  SlapFreeLoaObjectStub(obj_stub)                                                    \
         {                                                                                  \
            SlapCleanLoaObjectStub(obj_stub);                                               \
                                                                                            \
            AnscFreeLock  (&obj_stub->AccessLock);                                          \
            AnscFreeMemory(obj_stub);                                                       \
         }


#endif
