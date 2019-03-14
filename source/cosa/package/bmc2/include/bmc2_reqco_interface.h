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

    module:	bmc2_reqco_interface.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Req Controller Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_REQCO_INTERFACE_
#define  _BMC2_REQCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "bmc2_properties.h"

#include "bmc2_ifo_pec.h"


/***********************************************************
       BMC2 REQ CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BMC2_REQCO_MAX_ARGUMENT_NUMBER             32
#define  BMC2_REQCO_ORO_TABLE_SIZE                  32

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2REQCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2REQCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BMC2REQCO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_BMC2REQCO_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_BMC2REQCO_GET_ARG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ADD_ARG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pArgument
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_REMOVE_ARGS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ABORT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_SETUP_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_CLOSE_ENV)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ASYNC_TASK)
    (
        ANSC_HANDLE                 hAsyncJob
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ADD_CCO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommand
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ADD_CCO2)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       command_id,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  ANSC_HANDLE
(*PFN_BMC2REQCO_POP_CCO)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_BMC2REQCO_GET_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ADD_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjReference
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_ADD_ORO2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ANSC_HANDLE                 obj_container,
        ANSC_HANDLE                 obj_handle,
        ULONG                       obj_refcount
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_DEL_ORO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_DISPATCH)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulPecStatus
    );

typedef  ANSC_STATUS
(*PFN_BMC2REQCO_DO_STUFF)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

/*
 * The Bmc2 Req Controller Object encapsulates the implementation of a CLI Domain. Some advanced CLI
 * interfaces adopt a concept known as "Hierarchical Menu System". By dividing the CLI command set
 * into different domains, it becomes possible to reuse same command syntax for different purposes
 * in different domains. Furthermore, it's easier to apply access control based on the domain.
 */
#define  BMC2_REQ_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hBmc2PecIf;                                             \
    ANSC_HANDLE                     hBmc2ComDomain;                                         \
    ANSC_HANDLE                     hBmc2EnvController;                                     \
    ANSC_HANDLE                     hCommandRequest;                                        \
    ANSC_HANDLE                     hCommandReply;                                          \
    ANSC_HANDLE                     hCommandProperty;                                       \
    ULONG                           CliArgumentCount;                                       \
    char*                           CliArgumentArray[BMC2_REQCO_MAX_ARGUMENT_NUMBER];       \
    BOOL                            bAborted;                                               \
    BOOL                            bActive;                                                \
                                                                                            \
    ANSC_HANDLE                     hSlapContainerReq;                                      \
    ANSC_HANDLE                     hSlapBmc2Command;                                       \
    ANSC_HANDLE                     hSlapBmc2Input;                                         \
    ANSC_HANDLE                     hSlapBmc2Output;                                        \
                                                                                            \
    QUEUE_HEADER                    CcoQueue;                                               \
    ANSC_LOCK                       CcoQueueLock;                                           \
    QUEUE_HEADER                    OroTable[BMC2_REQCO_ORO_TABLE_SIZE];                    \
    ANSC_LOCK                       OroTableLock;                                           \
                                                                                            \
    PFN_BMC2REQCO_GET_IF            GetBmc2PecIf;                                           \
    PFN_BMC2REQCO_GET_CONTEXT       GetBmc2ComDomain;                                       \
    PFN_BMC2REQCO_SET_CONTEXT       SetBmc2ComDomain;                                       \
    PFN_BMC2REQCO_GET_CONTEXT       GetBmc2EnvController;                                   \
    PFN_BMC2REQCO_SET_CONTEXT       SetBmc2EnvController;                                   \
    PFN_BMC2REQCO_GET_CONTEXT       GetCommandRequest;                                      \
    PFN_BMC2REQCO_SET_CONTEXT       SetCommandRequest;                                      \
    PFN_BMC2REQCO_GET_CONTEXT       GetCommandReply;                                        \
    PFN_BMC2REQCO_SET_CONTEXT       SetCommandReply;                                        \
    PFN_BMC2REQCO_GET_CONTEXT       GetCommandProperty;                                     \
    PFN_BMC2REQCO_SET_CONTEXT       SetCommandProperty;                                     \
    PFN_BMC2REQCO_GET_COUNT         GetCliArgumentCount;                                    \
    PFN_BMC2REQCO_GET_ARG           GetCliArgument;                                         \
    PFN_BMC2REQCO_ADD_ARG           AddCliArgument;                                         \
    PFN_BMC2REQCO_REMOVE_ARGS       RemoveCliArguments;                                     \
    PFN_BMC2REQCO_ABORT             Abort;                                                  \
    PFN_BMC2REQCO_RESET             Reset;                                                  \
                                                                                            \
    PFN_BMC2REQCO_ENGAGE            Engage;                                                 \
    PFN_BMC2REQCO_CANCEL            Cancel;                                                 \
    PFN_BMC2REQCO_SETUP_ENV         SetupEnv;                                               \
    PFN_BMC2REQCO_CLOSE_ENV         CloseEnv;                                               \
    PFN_BMC2REQCO_ASYNC_TASK        AsyncJobTask;                                           \
                                                                                            \
    PFN_BMC2REQCO_ADD_CCO           AddControlCommand;                                      \
    PFN_BMC2REQCO_ADD_CCO2          AddControlCommand2;                                     \
    PFN_BMC2REQCO_POP_CCO           PopControlCommand;                                      \
    PFN_BMC2REQCO_CLEAR             ClearCcoQueue;                                          \
                                                                                            \
    PFN_BMC2REQCO_GET_ORO           GetObjReference;                                        \
    PFN_BMC2REQCO_ADD_ORO           AddObjReference;                                        \
    PFN_BMC2REQCO_ADD_ORO2          AddObjReference2;                                       \
    PFN_BMC2REQCO_DEL_ORO           DelObjReference;                                        \
    PFN_BMC2REQCO_CLEAR             ClearOroTable;                                          \
                                                                                            \
    PFN_BMC2REQCO_DISPATCH          DispatchCommands;                                       \
    PFN_BMC2REQCO_DO_STUFF          DoReturn;                                               \
    PFN_BMC2REQCO_DO_STUFF          DoTransfer;                                             \
    PFN_BMC2REQCO_DO_STUFF          DoExecute;                                              \
                                                                                            \
    PFN_BMC2PECIF_GET_OBJ1          PecGetCommand;                                          \
    PFN_BMC2PECIF_GET_OBJ1          PecGetInput;                                            \
    PFN_BMC2PECIF_GET_OBJ1          PecGetOutput;                                           \
    PFN_BMC2PECIF_GET_OBJ1          PecGetDomain;                                           \
    PFN_BMC2PECIF_GET_OBJ1          PecGetTerminal;                                         \
    PFN_BMC2PECIF_GET_OBJ1          PecGetApplication;                                      \
    PFN_BMC2PECIF_GET_OBJ1          PecGetServer;                                           \
                                                                                            \
    PFN_BMC2PECIF_GET_OBJ2          PecGetBmc2Object;                                       \
    PFN_BMC2PECIF_GET_PAGE          PecGetCookedPage;                                       \
    PFN_BMC2PECIF_RET_PAGE          PecRetCookedPage;                                       \
    PFN_BMC2PECIF_INVOKE            PecInvokeObject;                                        \
    /* end of object class content */                                                       \

typedef  struct
_BMC2_REQ_CONTROLLER_OBJECT
{
    BMC2_REQ_CONTROLLER_CLASS_CONTENT
}
BMC2_REQ_CONTROLLER_OBJECT,  *PBMC2_REQ_CONTROLLER_OBJECT;

#define  ACCESS_BMC2_REQ_CONTROLLER_OBJECT(p)           \
         ACCESS_CONTAINER(p, BMC2_REQ_CONTROLLER_OBJECT, Linkage)


/***********************************************************
    BMC2 REQ CONTROLLER UTILITY DATA STRUCTURE DEFINITION
***********************************************************/

/*
 * The Bmc2.Server object allows a SCP page to either transfer the page execution to another SCP
 * page or execute another page within the current context. If we perform such page execution in
 * the current context, we probably will run into stack-overflow problem. So we always create a
 * separate task and a separate Req Controller for new page execution.
 */
typedef  struct
_BMC2_REQCO_ASYNC_JOB
{
    ANSC_HANDLE                     hThisObject;
    ULONG                           CommandId;
    SLAP_PARAMETER_LIST*            ParamList;
    ANSC_HANDLE                     hAsyncEvent;
    ANSC_HANDLE                     hReserved;
}
BMC2_REQCO_ASYNC_JOB,  *PBMC2_REQCO_ASYNC_JOB;

#define  Bmc2ReqcoCleanAsyncJob(tbc_job)                                                    \
         {                                                                                  \
            if ( tbc_job->ParamList )                                                       \
            {                                                                               \
                SlapFreeParamList(tbc_job->ParamList);                                      \
                                                                                            \
                tbc_job->ParamList = NULL;                                                  \
            }                                                                               \
         }

#define  Bmc2ReqcoFreeAsyncJob(tbf_job)                                                     \
         {                                                                                  \
            Bmc2ReqcoCleanAsyncJob(tbf_job);                                                \
            AnscFreeMemory        (tbf_job);                                                \
         }

/*
 * Some services provided by BMC2 objects cannot be completed right away during the invocation via
 * the SLAP interface. For example, the Bmc2.Server Object allows a SCP page to transfer the flow
 * of execution to another SCP page by calling Bmc2.Server.Transfer(). Such context switch should
 * be performed in Req Controller, instead of each service object.
 */
#define  BMC2_REQCO_COMMAND_return                  1
#define  BMC2_REQCO_COMMAND_flush                   2
#define  BMC2_REQCO_COMMAND_transfer                3
#define  BMC2_REQCO_COMMAND_execute                 4

typedef  struct
_BMC2_REQCO_CONTROL_COMMAND
{
    SINGLE_LINK_ENTRY               Linkage;
    ULONG                           CommandId;
    SLAP_PARAMETER_LIST*            ParamList;
}
BMC2_REQCO_CONTROL_COMMAND,  *PBMC2_REQCO_CONTROL_COMMAND;

#define  ACCESS_BMC2_REQCO_CONTROL_COMMAND(p)       \
         ACCESS_CONTAINER(p, BMC2_REQCO_CONTROL_COMMAND, Linkage)

#define  Bmc2ReqcoCleanCco(tbc_cco)                                                         \
         {                                                                                  \
            if ( tbc_cco->ParamList )                                                       \
            {                                                                               \
                SlapFreeParamList(tbc_cco->ParamList);                                      \
                                                                                            \
                tbc_cco->ParamList = NULL;                                                  \
            }                                                                               \
         }

#define  Bmc2ReqcoFreeCco(tbf_cco)                                                          \
         {                                                                                  \
            Bmc2ReqcoCleanCco(tbf_cco);                                                     \
            AnscFreeMemory   (tbf_cco);                                                     \
         }

/*
 * SLAP employs a reference count-based mechanism to synchronize the read/write access on any SLAP
 * service objects. SLAP client modules need to explicitly acquire/release the access to the SLAP
 * objects by using the APIs provided by the SLAP_UOA interface. Because BMC2 tries to build a more
 * loose environment which gives the script writers more flexibility, BMC2 has to handle the object
 * access carefully. Internally, the Req Controller has implemented a object reference table, which
 * maintains the reference information for each SLAP object this page has ever accessed.
 */
typedef  struct
_BMC2_REQCO_OBJ_REFERENCE
{
    SINGLE_LINK_ENTRY               Linkage;
    char*                           ObjName;
    ANSC_HANDLE                     ObjContainer;
    ANSC_HANDLE                     ObjHandle;
    ULONG                           ObjRefCount;
}
BMC2_REQCO_OBJ_REFERENCE,  *PBMC2_REQCO_OBJ_REFERENCE;

#define  ACCESS_BMC2_REQCO_OBJ_REFERENCE(p)         \
         ACCESS_CONTAINER(p, BMC2_REQCO_OBJ_REFERENCE, Linkage)

#define  Bmc2ReqcoCleanOro(tbc_oro)                                                         \
         {                                                                                  \
            if ( tbc_oro->ObjName )                                                         \
            {                                                                               \
                AnscFreeMemory(tbc_oro->ObjName);                                           \
                                                                                            \
                tbc_oro->ObjName = NULL;                                                    \
            }                                                                               \
         }

#define  Bmc2ReqcoFreeOro(tbf_oro)                                                          \
         {                                                                                  \
            Bmc2ReqcoCleanOro(tbf_oro);                                                     \
            AnscFreeMemory   (tbf_oro);                                                     \
         }


#endif
