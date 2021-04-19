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

    module:	bmc2_reqco_management.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Bmc2 Req Controller Object.

        *   Bmc2ReqcoAddControlCommand
        *   Bmc2ReqcoAddControlCommand2
        *   Bmc2ReqcoPopControlCommand
        *   Bmc2ReqcoClearCcoQueue
        *   Bmc2ReqcoGetObjReference
        *   Bmc2ReqcoAddObjReference
        *   Bmc2ReqcoAddObjReference2
        *   Bmc2ReqcoDelObjReference
        *   Bmc2ReqcoClearOroTable

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
        Bmc2ReqcoAddControlCommand
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCommand
            );

    description:

        This function is called to add a new control command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCommand
                Specifies the control command to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoAddControlCommand
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCommand
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject       = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_CONTROL_COMMAND     pControlCommand = (PBMC2_REQCO_CONTROL_COMMAND)hCommand;

    AnscAcquireLock   (&pMyObject->CcoQueueLock);
    AnscQueuePushEntry(&pMyObject->CcoQueue, &pControlCommand->Linkage);
    AnscReleaseLock   (&pMyObject->CcoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoAddControlCommand2
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       command_id,
                SLAP_PARAMETER_LIST*        param_list
            );

    description:

        This function is called to add a new control command.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       command_id
                Specifies the control command id to be added.

                SLAP_PARAMETER_LIST*        param_list
                Specifies the parameter list associated with command.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoAddControlCommand2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       command_id,
        SLAP_PARAMETER_LIST*        param_list
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject       = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_CONTROL_COMMAND     pControlCommand = (PBMC2_REQCO_CONTROL_COMMAND)NULL;

    pControlCommand = (PBMC2_REQCO_CONTROL_COMMAND)AnscAllocateMemory(sizeof(BMC2_REQCO_CONTROL_COMMAND));

    if ( !pControlCommand )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pControlCommand->CommandId = command_id;
        pControlCommand->ParamList = param_list;
    }

    returnStatus =
        pMyObject->AddControlCommand
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pControlCommand
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoPopControlCommand
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to pop a control command from the front
        of the command queue.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     control command.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoPopControlCommand
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject       = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_CONTROL_COMMAND     pControlCommand = (PBMC2_REQCO_CONTROL_COMMAND)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY         )NULL;

    AnscAcquireLock(&pMyObject->CcoQueueLock);
    pSLinkEntry = AnscQueuePopEntry(&pMyObject->CcoQueue);
    AnscReleaseLock(&pMyObject->CcoQueueLock);

    if ( pSLinkEntry )
    {
        pControlCommand = ACCESS_BMC2_REQCO_CONTROL_COMMAND(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pControlCommand;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoClearCcoQueue
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clear the command queue.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoClearCcoQueue
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject       = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_CONTROL_COMMAND     pControlCommand = (PBMC2_REQCO_CONTROL_COMMAND)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY         )NULL;

    AnscAcquireLock(&pMyObject->CcoQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->CcoQueue);

    while ( pSLinkEntry )
    {
        pControlCommand = ACCESS_BMC2_REQCO_CONTROL_COMMAND(pSLinkEntry);
        pSLinkEntry     = AnscQueuePopEntry(&pMyObject->CcoQueue);

        Bmc2ReqcoFreeCco(pControlCommand);
    }

    AnscReleaseLock(&pMyObject->CcoQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ReqcoGetObjReference
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to retrieve a obj reference by matching
        the obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be matched.

    return:     obj reference.

**********************************************************************/

ANSC_HANDLE
Bmc2ReqcoGetObjReference
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject     = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_OBJ_REFERENCE       pObjReference = (PBMC2_REQCO_OBJ_REFERENCE  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           ulHashIndex   = AnscHashString(obj_name, AnscSizeOfString(obj_name), BMC2_REQCO_ORO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OroTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OroTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pObjReference = ACCESS_BMC2_REQCO_OBJ_REFERENCE(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pObjReference->ObjName,obj_name) == 0 )
        {
            AnscReleaseLock(&pMyObject->OroTableLock);

            return  pObjReference;
        }
    }

    AnscReleaseLock(&pMyObject->OroTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoAddObjReference
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjReference
            );

    description:

        This function is called to add a obj reference.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjReference
                Specifies the obj_reference to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoAddObjReference
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjReference
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject     = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_OBJ_REFERENCE       pObjReference = (PBMC2_REQCO_OBJ_REFERENCE  )hObjReference;
    ULONG                           ulHashIndex   = AnscHashString(pObjReference->ObjName, AnscSizeOfString(pObjReference->ObjName), BMC2_REQCO_ORO_TABLE_SIZE);

    AnscAcquireLock   (&pMyObject->OroTableLock);
    AnscQueuePushEntry(&pMyObject->OroTable[ulHashIndex], &pObjReference->Linkage);
    AnscReleaseLock   (&pMyObject->OroTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoAddObjReference2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name,
                ANSC_HANDLE                 obj_container,
                ANSC_HANDLE                 obj_handle,
                ULONG                       obj_refcount
            );

    description:

        This function is called to add a obj reference.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name of the obj_reference.

                ANSC_HANDLE                 obj_container
                Specifies the obj_container of the obj_reference.

                ANSC_HANDLE                 obj_handle
                Specifies the obj_handle of the obj_reference.

                ULONG                       obj_refcount
                Specifies the obj_refcount of the obj_reference.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoAddObjReference2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name,
        ANSC_HANDLE                 obj_container,
        ANSC_HANDLE                 obj_handle,
        ULONG                       obj_refcount
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject     = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_REQCO_OBJ_REFERENCE       pObjReference = (PBMC2_REQCO_OBJ_REFERENCE  )NULL;
    AnscHashString(obj_name, AnscSizeOfString(obj_name), BMC2_REQCO_ORO_TABLE_SIZE);

    pObjReference = (PBMC2_REQCO_OBJ_REFERENCE)AnscAllocateMemory(sizeof(BMC2_REQCO_OBJ_REFERENCE));

    if ( !pObjReference )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pObjReference->ObjName      = AnscCloneString(obj_name);
        pObjReference->ObjContainer = obj_container;
        pObjReference->ObjHandle    = obj_handle;
        pObjReference->ObjRefCount  = obj_refcount;
    }

    returnStatus =
        pMyObject->AddObjReference
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pObjReference
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoDelObjReference
            (
                ANSC_HANDLE                 hThisObject,
                char*                       obj_name
            );

    description:

        This function is called to delete a obj reference by matching
        the obj_name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       obj_name
                Specifies the obj_name to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoDelObjReference
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);
    PBMC2_REQCO_OBJ_REFERENCE       pObjReference      = (PBMC2_REQCO_OBJ_REFERENCE  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry        = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           ulHashIndex        = AnscHashString(obj_name, AnscSizeOfString(obj_name), BMC2_REQCO_ORO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->OroTableLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->OroTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pObjReference = ACCESS_BMC2_REQCO_OBJ_REFERENCE(pSLinkEntry);
        pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pObjReference->ObjName,obj_name) == 0 )
        {
            AnscQueuePopEntryByLink(&pMyObject->OroTable[ulHashIndex], &pObjReference->Linkage);

            while ( pObjReference->ObjRefCount > 0 )
            {
                pSlapGoaIf->RelObjectAccess(pSlapGoaIf->hOwnerContext, pObjReference->ObjHandle);

                pObjReference->ObjRefCount--;
            }

            Bmc2ReqcoFreeOro(pObjReference);

            break;
        }
    }

    AnscReleaseLock(&pMyObject->OroTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ReqcoClearOroTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clear the reference table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ReqcoClearOroTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_REQ_CONTROLLER_OBJECT     pMyObject          = (PBMC2_REQ_CONTROLLER_OBJECT)hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);
    PBMC2_REQCO_OBJ_REFERENCE       pObjReference      = (PBMC2_REQCO_OBJ_REFERENCE  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry        = (PSINGLE_LINK_ENTRY         )NULL;
    ULONG                           i                  = 0;

    AnscAcquireLock(&pMyObject->OroTableLock);

    for ( i = 0; i < BMC2_REQCO_ORO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->OroTable[i]);

        while ( pSLinkEntry )
        {
            pObjReference = ACCESS_BMC2_REQCO_OBJ_REFERENCE(pSLinkEntry);
            pSLinkEntry   = AnscQueuePopEntry(&pMyObject->OroTable[i]);

            while ( pObjReference->ObjRefCount > 0 )
            {
                pSlapGoaIf->RelObjectAccess(pSlapGoaIf->hOwnerContext, pObjReference->ObjHandle);

                pObjReference->ObjRefCount--;
            }

            Bmc2ReqcoFreeOro(pObjReference);
        }
    }

    AnscReleaseLock(&pMyObject->OroTableLock);

    return  ANSC_STATUS_SUCCESS;
}
