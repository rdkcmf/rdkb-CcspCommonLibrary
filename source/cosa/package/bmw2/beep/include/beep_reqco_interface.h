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

    module:	beep_reqco_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Beep Req Controller Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/16/04    initial revision.

**********************************************************************/


#ifndef  _BEEP_REQCO_INTERFACE_
#define  _BEEP_REQCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "beep_ifo_pec.h"


/***********************************************************
       BEEP REQ CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  BEEP_REQCO_ORO_TABLE_SIZE                  32

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPREQCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPREQCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  BOOL
(*PFN_BEEPREQCO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hAsyncJob
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_ADD_CCO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommand
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_ADD_CCO2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       command_id,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_HANDLE
(*PFN_BEEPREQCO_POP_CCO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BEEPREQCO_GET_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_ADD_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjReference
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_ADD_ORO2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ANSC_HANDLE                 obj_container,
        ANSC_HANDLE                 obj_handle,
        ULONG                       obj_refcount
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_DEL_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_DISPATCH)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulPecStatus
    );

typedef  ANSC_STATUS
(*PFN_BEEPREQCO_DO_STUFF)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

/*
 * Beep Req Controller Objet must be instantiated for every page execution request. This object
 * provides a generic BEEP/SLAP object access interface to the dynamic page execution engines. Not
 * only it inherits the application and session level SLAP containers from other objects, it also
 * creates the request level container, which constructs the operational environment for the Env
 * Request and Response objects.
 */
#define  BEEP_REQ_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBeepPecIf;                                             \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ANSC_HANDLE                     hBeepPesIf;                                             \
    ANSC_HANDLE                     hBeepClientSession;                                     \
    ANSC_HANDLE                     hBeepPageContainer;                                     \
    ANSC_HANDLE                     hHttpBmoReq;                                            \
    ANSC_HANDLE                     hHttpBmoRep;                                            \
    BOOL                            bAggrWrites;                                            \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     hSlapContainerReq;                                      \
    ANSC_HANDLE                     hSlapBeepRequest;                                       \
    ANSC_HANDLE                     hSlapBeepResponse;                                      \
                                                                                            \
    QUEUE_HEADER                    CcoQueue;                                               \
    ANSC_LOCK                       CcoQueueLock;                                           \
    QUEUE_HEADER                    OroTable[BEEP_REQCO_ORO_TABLE_SIZE];                    \
    ANSC_LOCK                       OroTableLock;                                           \
                                                                                            \
    PFN_BEEPREQCO_GET_IF            GetBeepPecIf;                                           \
    PFN_BEEPREQCO_GET_IF            GetBeepPemIf;                                           \
    PFN_BEEPREQCO_SET_IF            SetBeepPemIf;                                           \
    PFN_BEEPREQCO_GET_IF            GetBeepPesIf;                                           \
    PFN_BEEPREQCO_SET_IF            SetBeepPesIf;                                           \
    PFN_BEEPREQCO_GET_CONTEXT       GetBeepClientSession;                                   \
    PFN_BEEPREQCO_SET_CONTEXT       SetBeepClientSession;                                   \
    PFN_BEEPREQCO_GET_CONTEXT       GetBeepPageContainer;                                   \
    PFN_BEEPREQCO_SET_CONTEXT       SetBeepPageContainer;                                   \
    PFN_BEEPREQCO_GET_CONTEXT       GetHttpBmoReq;                                          \
    PFN_BEEPREQCO_SET_CONTEXT       SetHttpBmoReq;                                          \
    PFN_BEEPREQCO_GET_CONTEXT       GetHttpBmoRep;                                          \
    PFN_BEEPREQCO_SET_CONTEXT       SetHttpBmoRep;                                          \
    PFN_BEEPREQCO_GET_BOOL          GetAggrWrites;                                          \
    PFN_BEEPREQCO_SET_BOOL          SetAggrWrites;                                          \
    PFN_BEEPREQCO_RESET             Reset;                                                  \
                                                                                            \
    PFN_BEEPREQCO_ENGAGE            Engage;                                                 \
    PFN_BEEPREQCO_CANCEL            Cancel;                                                 \
    PFN_BEEPREQCO_SETUP_ENV         SetupEnv;                                               \
    PFN_BEEPREQCO_CLOSE_ENV         CloseEnv;                                               \
    PFN_BEEPREQCO_ASYNC_TASK        AsyncJobTask;                                           \
                                                                                            \
    PFN_BEEPREQCO_ADD_CCO           AddControlCommand;                                      \
    PFN_BEEPREQCO_ADD_CCO2          AddControlCommand2;                                     \
    PFN_BEEPREQCO_POP_CCO           PopControlCommand;                                      \
    PFN_BEEPREQCO_CLEAR             ClearCcoQueue;                                          \
                                                                                            \
    PFN_BEEPREQCO_GET_ORO           GetObjReference;                                        \
    PFN_BEEPREQCO_ADD_ORO           AddObjReference;                                        \
    PFN_BEEPREQCO_ADD_ORO2          AddObjReference2;                                       \
    PFN_BEEPREQCO_DEL_ORO           DelObjReference;                                        \
    PFN_BEEPREQCO_CLEAR             ClearOroTable;                                          \
                                                                                            \
    PFN_BEEPREQCO_DISPATCH          DispatchCommands;                                       \
    PFN_BEEPREQCO_DO_STUFF          DoReturn;                                               \
    PFN_BEEPREQCO_DO_STUFF          DoFlush;                                                \
    PFN_BEEPREQCO_DO_STUFF          DoTransfer;                                             \
    PFN_BEEPREQCO_DO_STUFF          DoExecute;                                              \
                                                                                            \
    PFN_BEEPPECIF_GET_OBJ1          PecGetRequest;                                          \
    PFN_BEEPPECIF_GET_OBJ1          PecGetResponse;                                         \
    PFN_BEEPPECIF_GET_OBJ1          PecGetApplication;                                      \
    PFN_BEEPPECIF_GET_OBJ1          PecGetSession;                                          \
    PFN_BEEPPECIF_GET_OBJ1          PecGetServer;                                           \
                                                                                            \
    PFN_BEEPPECIF_GET_OBJ2          PecGetBeepObject;                                       \
    PFN_BEEPPECIF_GET_PAGE          PecGetCookedPage;                                       \
    PFN_BEEPPECIF_RET_PAGE          PecRetCookedPage;                                       \
    PFN_BEEPPECIF_INVOKE            PecInvokeObject;                                        \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_REQ_CONTROLLER_OBJECT
{
    BEEP_REQ_CONTROLLER_CLASS_CONTENT
}
BEEP_REQ_CONTROLLER_OBJECT,  *PBEEP_REQ_CONTROLLER_OBJECT;

#define  ACCESS_BEEP_REQ_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, BEEP_REQ_CONTROLLER_OBJECT, Linkage)


/***********************************************************
    BEEP REQ CONTROLLER UTILITY DATA STRUCTURE DEFINITION
***********************************************************/

/*
 * The Beep.Server object allows a BSP page to either transfer the page execution to another BSP
 * page or execute another page within the current context. If we perform such page execution in
 * the current context, we probably will run into stack-overflow problem. So we always create a
 * separate task and a separate Req Controller for new page execution.
 */
typedef  struct
_BEEP_REQCO_ASYNC_JOB
{
    ANSC_HANDLE                     hThisObject;
    ULONG                           CommandId;
    SLAP_PARAMETER_LIST*            ParamList;
    ANSC_HANDLE                     hAsyncEvent;
    ANSC_HANDLE                     hReserved;
}
BEEP_REQCO_ASYNC_JOB,  *PBEEP_REQCO_ASYNC_JOB;

#define  BeepReqcoCleanAsyncJob(tbc_job)                                                    \
         {                                                                                  \
            if ( tbc_job->ParamList )                                                       \
            {                                                                               \
                SlapFreeParamList(tbc_job->ParamList);                                      \
                                                                                            \
                tbc_job->ParamList = NULL;                                                  \
            }                                                                               \
         }

#define  BeepReqcoFreeAsyncJob(tbf_job)                                                     \
         {                                                                                  \
            BeepReqcoCleanAsyncJob(tbf_job);                                                \
            AnscFreeMemory        (tbf_job);                                                \
         }

/*
 * Some services provided by BEEP objects cannot be completed right away during the invocation via
 * the SLAP interface. For example, the Beep.Server Object allows a BSP page to transfer the flow
 * of execution to another BSP page by calling Beep.Server.Transfer(). Such context switch should
 * be performed in Req Controller, instead of each service object.
 */
#define  BEEP_REQCO_COMMAND_return                  1
#define  BEEP_REQCO_COMMAND_flush                   2
#define  BEEP_REQCO_COMMAND_transfer                3
#define  BEEP_REQCO_COMMAND_execute                 4

typedef  struct
_BEEP_REQCO_CONTROL_COMMAND
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           CommandId;
    SLAP_PARAMETER_LIST*            ParamList;
}
BEEP_REQCO_CONTROL_COMMAND,  *PBEEP_REQCO_CONTROL_COMMAND;

#define  ACCESS_BEEP_REQCO_CONTROL_COMMAND(p)       \
         ACCESS_CONTAINER(p, BEEP_REQCO_CONTROL_COMMAND, Linkage)

#define  BeepReqcoCleanCco(tbc_cco)                                                         \
         {                                                                                  \
            if ( tbc_cco->ParamList )                                                       \
            {                                                                               \
                SlapFreeParamList(tbc_cco->ParamList);                                      \
                                                                                            \
                tbc_cco->ParamList = NULL;                                                  \
            }                                                                               \
         }

#define  BeepReqcoFreeCco(tbf_cco)                                                          \
         {                                                                                  \
            BeepReqcoCleanCco(tbf_cco);                                                     \
            AnscFreeMemory   (tbf_cco);                                                     \
         }

/*
 * SLAP employs a reference count-based mechanism to synchronize the read/write access on any SLAP
 * service objects. SLAP client modules need to explicitly acquire/release the access to the SLAP
 * objects by using the APIs provided by the SLAP_UOA interface. Because BEEP tries to build a more
 * loose environment which gives the script writers more flexibility, BEEP has to handle the object
 * access carefully. Internally, the Req Controller has implemented a object reference table, which
 * maintains the reference information for each SLAP object this page has ever accessed.
 */
typedef  struct
_BEEP_REQCO_OBJ_REFERENCE
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           ObjName;
    ANSC_HANDLE                     ObjContainer;
    ANSC_HANDLE                     ObjHandle;
    ULONG                           ObjRefCount;
}
BEEP_REQCO_OBJ_REFERENCE,  *PBEEP_REQCO_OBJ_REFERENCE;

#define  ACCESS_BEEP_REQCO_OBJ_REFERENCE(p)         \
         ACCESS_CONTAINER(p, BEEP_REQCO_OBJ_REFERENCE, Linkage)

#define  BeepReqcoCleanOro(tbc_oro)                                                         \
         {                                                                                  \
            if ( tbc_oro->ObjName )                                                         \
            {                                                                               \
                AnscFreeMemory(tbc_oro->ObjName);                                           \
                                                                                            \
                tbc_oro->ObjName = NULL;                                                    \
            }                                                                               \
         }

#define  BeepReqcoFreeOro(tbf_oro)                                                          \
         {                                                                                  \
            BeepReqcoCleanOro(tbf_oro);                                                     \
            AnscFreeMemory   (tbf_oro);                                                     \
         }


#endif
