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

    module:	bmc2_comdo_operation.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bmc2 Com Domain Object.

        *   Bmc2ComdoEngage
        *   Bmc2ComdoCancel
        *   Bmc2ComdoSetupEnv
        *   Bmc2ComdoCloseEnv

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


#include "bmc2_comdo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoEngage
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
Bmc2ComdoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

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
        Bmc2ComdoCancel
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
Bmc2ComdoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject    = (PBMC2_COM_DOMAIN_OBJECT)hThisObject;

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
        Bmc2ComdoSetupEnv
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
Bmc2ComdoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject          = (PBMC2_COM_DOMAIN_OBJECT    )hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor     = (PSLAP_OBJECT_DESCRIPTOR    )NULL;

    if ( TRUE )
    {
        pMyObject->hSlapContainerDomain =
            pSlapGoaIf->CreateContainer
                (
                    pSlapGoaIf->hOwnerContext,
                    BMC2_SLAP_CONTAINER_DOMAIN,
                    SLAP_CONTAINER_TYPE_STANDARD
                );

        if ( !pMyObject->hSlapContainerDomain )
        {
            return  ANSC_STATUS_FAILURE;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor             = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2DomoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pMyObject->hSlapBmc2Domain =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerDomain,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComdoCloseEnv
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
Bmc2ComdoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_DOMAIN_OBJECT         pMyObject          = (PBMC2_COM_DOMAIN_OBJECT    )hThisObject;
    PBMC2_ENV_CONTROLLER_OBJECT     pBmc2EnvController = (PBMC2_ENV_CONTROLLER_OBJECT)pMyObject->hBmc2EnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pBmc2EnvController->GetSlapGoaIf((ANSC_HANDLE)pBmc2EnvController);

/*
    if ( pMyObject->hSlapBmc2Domain )
    {
        returnStatus =
            pSlapGoaIf->DeleteObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapBmc2Domain
                );
    }
*/

    if ( TRUE )
    {
        pSlapGoaIf->DeleteContainer
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapContainerDomain
            );
    }

    return  ANSC_STATUS_SUCCESS;
}
