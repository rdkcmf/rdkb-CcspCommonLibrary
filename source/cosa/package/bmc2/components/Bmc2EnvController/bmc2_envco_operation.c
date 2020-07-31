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

    module:	bmc2_envco_operation.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bmc2 Env Controller Object.

        *   Bmc2EnvcoEngage
        *   Bmc2EnvcoCancel
        *   Bmc2EnvcoSetupEnv
        *   Bmc2EnvcoCloseEnv

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/24/05    initial revision.

**********************************************************************/


#include "bmc2_envco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoEngage
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
Bmc2EnvcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject          = (PBMC2_ENV_CONTROLLER_OBJECT)hThisObject;
    PBMC2_COM_EXECUTOR_OBJECT       pBmc2ComExecutor   = (PBMC2_COM_EXECUTOR_OBJECT  )pMyObject->hBmc2ComExecutor;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT)pMyObject->hBwrmEnvController;
    PSLAP_GOA_INTERFACE             pSlapGoaIf         = (PSLAP_GOA_INTERFACE        )pMyObject->hSlapGoaIf;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )pMyObject->hSlapUoaIf;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pSlapGoaIf || !pSlapUoaIf )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    pBmc2ComExecutor  ->SetBmc2EnvController((ANSC_HANDLE)pBmc2ComExecutor, (ANSC_HANDLE)pMyObject);
    pBmc2ComExecutor  ->Engage              ((ANSC_HANDLE)pBmc2ComExecutor  );
    pBwrmEnvController->Engage              ((ANSC_HANDLE)pBwrmEnvController);

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
        Bmc2EnvcoCancel
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
Bmc2EnvcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject          = (PBMC2_ENV_CONTROLLER_OBJECT)hThisObject;
    PBMC2_COM_EXECUTOR_OBJECT       pBmc2ComExecutor   = (PBMC2_COM_EXECUTOR_OBJECT  )pMyObject->hBmc2ComExecutor;
    PBWRM_ENV_CONTROLLER_OBJECT     pBwrmEnvController = (PBWRM_ENV_CONTROLLER_OBJECT)pMyObject->hBwrmEnvController;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pBmc2ComExecutor  ->Cancel((ANSC_HANDLE)pBmc2ComExecutor  );
    pBwrmEnvController->Cancel((ANSC_HANDLE)pBwrmEnvController);

    pMyObject->CloseEnv((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoSetupEnv
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
Bmc2EnvcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject      = (PBMC2_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf     = (PSLAP_GOA_INTERFACE        )pMyObject->hSlapGoaIf;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR    )NULL;

    if ( TRUE )
    {
        pMyObject->hSlapContainerApp =
            pSlapGoaIf->CreateContainer
                (
                    pSlapGoaIf->hOwnerContext,
                    BMC2_SLAP_CONTAINER_APP,
                    SLAP_CONTAINER_TYPE_STANDARD
                );

        if ( !pMyObject->hSlapContainerApp )
        {
            return  ANSC_STATUS_FAILURE;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2AppoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }

        pMyObject->hSlapBmc2App =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerApp,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2SrvoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }

        pMyObject->hSlapBmc2Server =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerApp,
                    pObjDescriptor->ObjName,
                    (ANSC_HANDLE)pMyObject
                );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2TermoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2DomoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2ComoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2OutoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2InoGetSlapObjDescriptor(pMyObject->hContainerContext);
        returnStatus   =
            pSlapGoaIf->EnrollObjDescriptor
                (
                    pSlapGoaIf->hOwnerContext,
                    (ANSC_HANDLE)pObjDescriptor
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
    }

    if ( pMyObject->hSlapContainerServices )
    {
        pMyObject->hSlapVarEntity =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerServices,
                    SLAP_VEO_UOA_NAME,
                    (ANSC_HANDLE)NULL
                );

        pMyObject->hSlapVarConverter =
            pSlapGoaIf->CreateObject
                (
                    pSlapGoaIf->hOwnerContext,
                    pMyObject->hSlapContainerServices,
                    SLAP_VCO_UOA_NAME,
                    (ANSC_HANDLE)NULL
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2EnvcoCloseEnv
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
Bmc2EnvcoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_ENV_CONTROLLER_OBJECT     pMyObject      = (PBMC2_ENV_CONTROLLER_OBJECT)hThisObject;
    PSLAP_GOA_INTERFACE             pSlapGoaIf     = (PSLAP_GOA_INTERFACE        )pMyObject->hSlapGoaIf;
    PSLAP_OBJECT_DESCRIPTOR         pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR    )NULL;

    if ( TRUE )
    {
        pSlapGoaIf->DeleteContainer
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapContainerApp
            );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2AppoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pSlapGoaIf->UnbindObjDescriptor
            (
                pSlapGoaIf->hOwnerContext,
                pObjDescriptor->ObjName
            );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2SrvoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pSlapGoaIf->UnbindObjDescriptor
            (
                pSlapGoaIf->hOwnerContext,
                pObjDescriptor->ObjName
            );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2TermoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pSlapGoaIf->UnbindObjDescriptor
            (
                pSlapGoaIf->hOwnerContext,
                pObjDescriptor->ObjName
            );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2DomoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pSlapGoaIf->UnbindObjDescriptor
            (
                pSlapGoaIf->hOwnerContext,
                pObjDescriptor->ObjName
            );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2ComoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pSlapGoaIf->UnbindObjDescriptor
            (
                pSlapGoaIf->hOwnerContext,
                pObjDescriptor->ObjName
            );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2OutoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pSlapGoaIf->UnbindObjDescriptor
            (
                pSlapGoaIf->hOwnerContext,
                pObjDescriptor->ObjName
            );
    }

    if ( TRUE )
    {
        pObjDescriptor = (PSLAP_OBJECT_DESCRIPTOR)SlapBmc2InoGetSlapObjDescriptor(pMyObject->hContainerContext);
        pSlapGoaIf->UnbindObjDescriptor
            (
                pSlapGoaIf->hOwnerContext,
                pObjDescriptor->ObjName
            );
    }

    if ( pMyObject->hSlapVarEntity )
    {
        pSlapGoaIf->DeleteObject
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapVarEntity
            );
    }

    if ( pMyObject->hSlapVarConverter )
    {
        pSlapGoaIf->DeleteObject
            (
                pSlapGoaIf->hOwnerContext,
                pMyObject->hSlapVarConverter
            );
    }

    return  ANSC_STATUS_SUCCESS;
}
