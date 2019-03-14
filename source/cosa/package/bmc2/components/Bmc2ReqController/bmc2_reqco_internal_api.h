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

    module:	bmc2_reqco_internal_api.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Bmc2 Req Controller
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_REQCO_INTERNAL_API_
#define  _BMC2_REQCO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN BMC2_REQCO_STATES.C
***********************************************************/

ANSC_HANDLE
Bmc2ReqcoGetBmc2PecIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoGetBmc2ComDomain
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoSetBmc2ComDomain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    );

ANSC_HANDLE
Bmc2ReqcoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnvController
    );

ANSC_HANDLE
Bmc2ReqcoGetCommandRequest
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoSetCommandRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandReq
    );

ANSC_HANDLE
Bmc2ReqcoGetCommandReply
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoSetCommandReply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandRep
    );

ANSC_HANDLE
Bmc2ReqcoGetCommandProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoSetCommandProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommandProperty
    );

ULONG
Bmc2ReqcoGetCliArgumentCount
    (
        ANSC_HANDLE                 hThisObject
    );

char*
Bmc2ReqcoGetCliArgument
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

ANSC_STATUS
Bmc2ReqcoAddCliArgument
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pArgument
    );

ANSC_STATUS
Bmc2ReqcoRemoveCliArguments
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoAbort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BMC2_REQCO_OPERATION.C
***********************************************************/

ANSC_STATUS
Bmc2ReqcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoAsyncJobTask
    (
        ANSC_HANDLE                 hAsyncJob
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN BMC2_REQCO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
Bmc2ReqcoAddControlCommand
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommand
    );

ANSC_STATUS
Bmc2ReqcoAddControlCommand2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       command_id,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_HANDLE
Bmc2ReqcoPopControlCommand
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
Bmc2ReqcoClearCcoQueue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoGetObjReference
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
Bmc2ReqcoAddObjReference
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjReference
    );

ANSC_STATUS
Bmc2ReqcoAddObjReference2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ANSC_HANDLE                 obj_container,
        ANSC_HANDLE                 obj_handle,
        ULONG                       obj_refcount
    );

ANSC_STATUS
Bmc2ReqcoDelObjReference
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
Bmc2ReqcoClearOroTable
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN BMC2_REQCO_CONTROL.C
***********************************************************/

ANSC_STATUS
Bmc2ReqcoDispatchCommands
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulPecStatus
    );

ANSC_STATUS
Bmc2ReqcoDoReturn
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
Bmc2ReqcoDoTransfer
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

ANSC_STATUS
Bmc2ReqcoDoExecute
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN BMC2_REQCO_PECIF.C
***********************************************************/

ANSC_HANDLE
Bmc2ReqcoPecGetCommand
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoPecGetInput
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoPecGetOutput
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoPecGetDomain
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoPecGetTerminal
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoPecGetApplication
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoPecGetServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
Bmc2ReqcoPecGetBmc2Object
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_HANDLE
Bmc2ReqcoPecGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

ANSC_STATUS
Bmc2ReqcoPecRetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    );

ANSC_STATUS
Bmc2ReqcoPecInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmc2Object,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        PULONG                      pulPecStatus
    );


#endif
