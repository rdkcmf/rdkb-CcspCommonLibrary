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

    module:	slap_lco_operation.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Slap Loam Client Object.

        *   SlapLcoEngage
        *   SlapLcoCancel
        *   SlapLcoSetupEnv
        *   SlapLcoCloseEnv
        *   SlapLcoPatrolTimerInvoke

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


#include "slap_lco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoEngage
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
SlapLcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT     )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY   )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PANSC_LPC_CONNECTOR_OBJECT      pAnscLpcConnector  = (PANSC_LPC_CONNECTOR_OBJECT   )pMyObject->hAnscLpcConnector;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimerObj;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pAnscLpcConnector )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    if ( pProperty->bMobilityOn )
    {
        if ( !pSlapEnvController )
        {
            pSlapEnvController =
                (PSLAP_ENV_CONTROLLER_OBJECT)SlapCreateEnvController
                    (
                        pMyObject->hContainerContext,
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)NULL
                    );

            if ( !pSlapEnvController )
            {
                return  ANSC_STATUS_RESOURCES;
            }
            else
            {
                pMyObject->hSlapEnvController = (ANSC_HANDLE)pSlapEnvController;
            }
        }

        pSlapEnvController->Engage((ANSC_HANDLE)pSlapEnvController);
    }

    returnStatus = pMyObject->SetupEnv((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pPatrolTimerObj->Start((ANSC_HANDLE)pPatrolTimerObj);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoCancel
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
SlapLcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT     )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY   )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT  )pMyObject->hSlapEnvController;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimerObj;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    if ( TRUE )
    {
        pPatrolTimerObj->Stop((ANSC_HANDLE)pPatrolTimerObj);
    }

    if ( TRUE )
    {
        pMyObject->CloseEnv((ANSC_HANDLE)pMyObject);
    }

    if ( pProperty->bMobilityOn )
    {
        if ( pSlapEnvController )
        {
            pSlapEnvController->Cancel((ANSC_HANDLE)pSlapEnvController);
            pSlapEnvController->Remove((ANSC_HANDLE)pSlapEnvController);

            pMyObject->hSlapEnvController = (ANSC_HANDLE)NULL;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoSetupEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the runtime environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )NULL;

    if ( pProperty->bMobilityOn && pSlapEnvController )
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

        pMyObject->hMobileContainer =
            pSlapUoaIf->CreateContainer
                (
                    pSlapUoaIf->hOwnerContext,
                    "mobileContainer",
                    SLAP_CONTAINER_TYPE_STANDARD | SLAP_CONTAINER_TYPE_DEFAULT
                );

        if ( !pMyObject->hMobileContainer )
        {
            return  ANSC_STATUS_FAILURE;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoCloseEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the runtime environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject          = (PSLAP_LOAM_CLIENT_OBJECT   )hThisObject;
    PSLAP_LOAM_CLIENT_PROPERTY      pProperty          = (PSLAP_LOAM_CLIENT_PROPERTY )&pMyObject->Property;
    PSLAP_ENV_CONTROLLER_OBJECT     pSlapEnvController = (PSLAP_ENV_CONTROLLER_OBJECT)pMyObject->hSlapEnvController;
    PSLAP_UOA_INTERFACE             pSlapUoaIf         = (PSLAP_UOA_INTERFACE        )NULL;

    if ( pProperty->bMobilityOn && pSlapEnvController )
    {
        pSlapUoaIf = (PSLAP_UOA_INTERFACE)pSlapEnvController->GetSlapUoaIf((ANSC_HANDLE)pSlapEnvController);

        pSlapUoaIf->DeleteContainer
                (
                    pSlapUoaIf->hOwnerContext,
                    pMyObject->hMobileContainer
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapLcoPatrolTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called whenever the patrol timer fires.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapLcoPatrolTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_LOAM_CLIENT_OBJECT        pMyObject         = (PSLAP_LOAM_CLIENT_OBJECT  )hThisObject;
    PSLAP_LOA_OBJECT_STUB           pObjectStub       = (PSLAP_LOA_OBJECT_STUB     )NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr        = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           i                 = 0;
    ULONG                           ulCurTime         = AnscGetTickInSeconds();

    AnscAcquireLock(&pMyObject->OsoTableLock);

    for ( i = 0; i < SLAP_LCO_OSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->OsoTable[i]);

        while ( pSLinkEntry )
        {
            pObjectStub = ACCESS_SLAP_LOA_OBJECT_STUB(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            pPartyAddr  = (PANSC_LPC_PARTY_ADDR)pObjectStub->hLpcPartyAddr;

            if ( pObjectStub->RefCount > 0 )
            {
                continue;
            }
            else if ( (pObjectStub->LastImcpError              == 0                           ) &&
                      ((ulCurTime - pObjectStub->LastAccessAt) <  SLAP_LCO_OBJECT_STUB_TIMEOUT) )
            {
                continue;
            }
            else
            {
                AnscSListPopEntryByLink(&pMyObject->OsoTable[i], &pObjectStub->Linkage);

                if ( pPartyAddr )
                {
                    pMyObject->LoaRelObjectAccess2
                            (
                                (ANSC_HANDLE)pMyObject,
                                pObjectStub->hLpcPartyAddr,
                                pObjectStub->hLoaSlapObject
                            );

                    pPartyAddr->RefCount--;
                }

                SlapLockLoaObjectStub  (pObjectStub);
                SlapUnlockLoaObjectStub(pObjectStub);
                SlapFreeLoaObjectStub  (pObjectStub);
            }
        }
    }

    AnscReleaseLock(&pMyObject->OsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
