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

    module:	bmc2_reqco_control.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the Bmc2 Req Controller Object.

        *   Bmc2ReqcoDispatchCommands
        *   Bmc2ReqcoDoReturn
        *   Bmc2ReqcoDoTransfer
        *   Bmc2ReqcoDoExecute

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


#include "bmc2_reqco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoDispatchCommands
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pulPecStatus
            );

    description:

        This function is called to dispatch the control commands.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pulPecStatus
                Specifies the PEC status to be returned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoDispatchCommands
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulPecStatus
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject       = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_CONTROL_COMMAND     pControlCommand = (PBMC2_REQCO_CONTROL_COMMAND)NULL;

    *pulPecStatus = BMC2_PEC_STATUS_continue;

    while (( pControlCommand = (PBMC2_REQCO_CONTROL_COMMAND)pMyObject->PopControlCommand((ANSC_HANDLE)pMyObject) ))
    {
        switch ( pControlCommand->CommandId )
        {
            case    BMC2_REQCO_COMMAND_return :

                    pMyObject->DoReturn
                        (
                            (ANSC_HANDLE)pMyObject,
                            pControlCommand->ParamList
                        );

                    *pulPecStatus = BMC2_PEC_STATUS_terminate;

                    break;

            case    BMC2_REQCO_COMMAND_transfer :

                    pMyObject->DoTransfer
                        (
                            (ANSC_HANDLE)pMyObject,
                            pControlCommand->ParamList
                        );

                    *pulPecStatus = BMC2_PEC_STATUS_terminate;

                    break;

            case    BMC2_REQCO_COMMAND_execute :

                    pMyObject->DoExecute
                        (
                            (ANSC_HANDLE)pMyObject,
                            pControlCommand->ParamList
                        );

                    break;

            default :

                    break;
        }

        Bmc2ReqcoFreeCco(pControlCommand);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoDoReturn
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to execute a control command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list associated with command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoDoReturn
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(param_list);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoDoTransfer
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to execute a control command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list associated with command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoDoTransfer
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_ASYNC_JOB           pAsyncJob    = (PBMC2_REQCO_ASYNC_JOB      )NULL;
    ANSC_EVENT                      asyncEvent;

    AnscInitializeEvent(&asyncEvent);
    AnscResetEvent     (&asyncEvent);

    pAsyncJob = (PBMC2_REQCO_ASYNC_JOB)AnscAllocateMemory(sizeof(BMC2_REQCO_ASYNC_JOB));

    if ( pAsyncJob )
    {
        pAsyncJob->hThisObject = (ANSC_HANDLE)pMyObject;
        pAsyncJob->CommandId   = BMC2_REQCO_COMMAND_transfer;
        pAsyncJob->ParamList   = NULL;
        pAsyncJob->hAsyncEvent = (ANSC_HANDLE)&asyncEvent;
        pAsyncJob->hReserved   = (ANSC_HANDLE)NULL;

        if ( param_list )
        {
	    /*CID: 164053 Resource leak- Free the memory in SlapVhoCloneParamList */
	    /* SlapVhoCloneParamList allocates memory that is stored into dst_param_list*/
            SlapCloneParamList(param_list, pAsyncJob->ParamList);
        }

        AnscSpawnTask
            (
                (void*      )pMyObject->AsyncJobTask,
                (ANSC_HANDLE)pAsyncJob,
                "Bmc2ReqcoAsyncJobTask"
            );

        AnscWaitEvent(&asyncEvent, 0xFFFFFFFF);
    }

    AnscFreeEvent(&asyncEvent);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoDoExecute
            (
                ANSC_HANDLE                 hThisObject,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to execute a control command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list associated with command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoDoExecute
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject    = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_ASYNC_JOB           pAsyncJob    = (PBMC2_REQCO_ASYNC_JOB      )NULL;
    ANSC_EVENT                      asyncEvent;

    AnscInitializeEvent(&asyncEvent);
    AnscResetEvent     (&asyncEvent);

    pAsyncJob = (PBMC2_REQCO_ASYNC_JOB)AnscAllocateMemory(sizeof(BMC2_REQCO_ASYNC_JOB));

    if ( pAsyncJob )
    {
        pAsyncJob->hThisObject = (ANSC_HANDLE)pMyObject;
        pAsyncJob->CommandId   = BMC2_REQCO_COMMAND_execute;
        pAsyncJob->ParamList   = NULL;
        pAsyncJob->hAsyncEvent = (ANSC_HANDLE)&asyncEvent;
        pAsyncJob->hReserved   = (ANSC_HANDLE)NULL;

        if ( param_list )
        {
	    /*CID: 164052 Resource leak - Free the ptr in SlapVhoCloneParamList */
	    /* SlapVhoCloneParamList allocates memory that is stored into dst_param_list*/
            SlapCloneParamList(param_list, pAsyncJob->ParamList);
        }

        AnscSpawnTask
            (
                (void*      )pMyObject->AsyncJobTask,
                (ANSC_HANDLE)pAsyncJob,
                "Bmc2ReqcoAsyncJobTask"
            );

        AnscWaitEvent(&asyncEvent, 0xFFFFFFFF);
    }

    AnscFreeEvent(&asyncEvent);

    return  ANSC_STATUS_SUCCESS;
}
