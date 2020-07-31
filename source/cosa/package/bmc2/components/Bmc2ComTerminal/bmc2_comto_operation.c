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

    module:	bmc2_comto_operation.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bmc2 Com Terminal Object.

        *   Bmc2ComtoAcqAccess
        *   Bmc2ComtoRelAccess
        *   Bmc2ComtoEngage
        *   Bmc2ComtoCancel
        *   Bmc2ComtoSetupEnv
        *   Bmc2ComtoCloseEnv

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


#include "bmc2_comto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoAcqAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoRelAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoRelAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    returnStatus = pMyObject->SetupEnv((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    pMyObject->bActive = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pMyObject->CloseEnv((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetupEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject          = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor     = (PSLAP_OBJECT_DESCRIPTOR    )NULL;

    if ( TRUE )
    {
        pMyObject->hSlapContainerTerminal =
            pSlapGoaIf->CreateContainer
                (
                    pSlapGoaIf->hOwnerContext,
                    BMC2_SLAP_CONTAINER_TERMINAL,
                    SLAP_CONTAINER_TYPE_STANDARD
                );

        if ( !pMyObject->hSlapContainerTerminal )
        {
            return  ANSC_STATUS_FAILURE;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor               = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2TermoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pMyObject->hSlapBmc2Terminal =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerTerminal,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );

        if ( pMyObject->hSlapBmc2Terminal )
        {
            PSLAP_OBJ_WRAPPER_OBJECT    pSlapWrapperObj      = (PSLAP_OBJ_WRAPPER_OBJECT  )pMyObject->hSlapBmc2Terminal;
            PSLAP_OBJ_RECORD_OBJECT     pSlapObjRecord       = (PSLAP_OBJ_RECORD_OBJECT   )pSlapWrapperObj->hWrappedObj;
            PSLAP_BMC2_TERMINAL_OBJECT  pSlapBmc2TerminalObj = (PSLAP_BMC2_TERMINAL_OBJECT)pSlapObjRecord->hSlapSrvComponent;
            SLAP_PARAMETER_LIST         SlapParamList;
            SLAP_VARIABLE*              pSlapVar;

            SlapInitParamList((&SlapParamList));

            SlapParamList.ParamCount = 2;
            pSlapVar = &SlapParamList.ParamArray[0];
            SlapInitVariable(pSlapVar);

            pSlapVar->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVar->Variant.varString = AnscCloneString("ptty");

            pSlapVar = &SlapParamList.ParamArray[1];
            SlapInitVariable(pSlapVar);

            pSlapVar->Syntax            = SLAP_VAR_SYNTAX_string;
            pSlapVar->Variant.varString = AnscCloneString(pMyObject->pPseudoTermDevName);

            pSlapBmc2TerminalObj->SetItem((ANSC_HANDLE)pSlapBmc2TerminalObj, &SlapParamList);

            SlapCleanParamList((&SlapParamList));
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoCloseEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject          = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);

    if ( pMyObject->hSlapBmc2Terminal )
    {
        pSlapGoaIf->DeleteObject
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapBmc2Terminal
            );
    }

    if ( TRUE )
    {
        pSlapGoaIf->DeleteContainer
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapContainerTerminal
            );
    }

    return  ANSC_STATUS_SUCCESS;
}
