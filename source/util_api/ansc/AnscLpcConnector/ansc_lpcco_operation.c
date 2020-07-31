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

    module:	ansc_lpcco_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Lpc Connector Object.

        *   AnscLpccoAcqAccess
        *   AnscLpccoRelAccess
        *   AnscLpccoEngage
        *   AnscLpccoCancel
        *   AnscLpccoEnroll
        *   AnscLpccoUnbind
        *   AnscLpccoConnectToManager
        *   AnscLpccoConnTimerInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/24/03    initial revision.

**********************************************************************/


#include "ansc_lpcco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoAcqAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire object access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoRelAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release object access.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoRelAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;

    AnscReleaseLock(&pMyObject->AccessLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject     = (PANSC_LPC_CONNECTOR_OBJECT   )hThisObject;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pMyObject->PartyName )
    {
        return  ANSC_STATUS_NOT_READY;
    }

    /*
     * We first need to add the local address information into the party table. The upper layer
     * MUST configure the 'party_name' before engaging the IMC. We then initiate contact to the
     * central LPC manager. No, wait! It seems that we cannot determine the local address infor-
     * mation until the LPC manager assigns one.
     */
    returnStatus = pMyObject->Enroll((ANSC_HANDLE)pMyObject);

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
        AnscLpccoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel object activities.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject     = (PANSC_LPC_CONNECTOR_OBJECT   )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pConnTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hConnTimerObj;

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
        pConnTimerObj->Stop((ANSC_HANDLE)pConnTimerObj);
    }

    returnStatus = pMyObject->Unbind((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoEnroll
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enroll this IPC connector to the
        central LPC manager. This function also adds the local address
        information into the party table

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoEnroll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoUnbind
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unbind this IPC connector from the
        central LPC manager.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoUnbind
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoConnectToManager
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to connect this IPC connector to the
        central LPC manager.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoConnectToManager
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoConnTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when the connection timer fires.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoConnTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_LPC_CONNECTOR_OBJECT      pMyObject    = (PANSC_LPC_CONNECTOR_OBJECT)hThisObject;
    PANSC_CPC_INTERFACE             pAnscCpcIf   = (PANSC_CPC_INTERFACE       )pMyObject->hAnscCpcIf;
    PANSC_LPC_PARTY_ADDR            pPartyAddr   = (PANSC_LPC_PARTY_ADDR      )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY        )NULL;
    ULONG                           i            = 0;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireLock(&pMyObject->PartyTableLock);

    for ( i = 0; i < ANSC_LPCCO_PARTY_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->PartyTable[i]);

        while ( pSLinkEntry )
        {
            pPartyAddr  = ACCESS_ANSC_LPC_PARTY_ADDR(pSLinkEntry);
            pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

            if ( pPartyAddr->Timestamp == 0 )
            {
                continue;
            }
            else if ( (pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedIn ) ||
                      (pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedOut) )
            {
                continue;
            }
            else if ( pPartyAddr->PartyType == ANSC_LPC_PARTY_TYPE_manager )
            {
                if ( (pMyObject->LpcOpmode  == ANSC_LPC_OPMODE_CLIENT) &&
                     (pMyObject->bConnected == TRUE                  ) )
                {
                    pMyObject->bConnected = FALSE;
                }

                continue;
            }
            else if ( pPartyAddr->PartyType != ANSC_LPC_PARTY_TYPE_client )
            {
                continue;
            }
            else if ( pPartyAddr->RefCount > 0 )
            {
                continue;
            }

            AnscQueuePopEntryByLink(&pMyObject->PartyTable[i], &pPartyAddr->Linkage);

            if ( TRUE )
            {
                    pAnscCpcIf->NotifyEvent
                        (
                            pAnscCpcIf->hOwnerContext,
                            ANSC_CPC_EVENT_DISCONNECTED,
                            (ANSC_HANDLE)pPartyAddr
                        );
            }

            AnscFreeMemory(pPartyAddr);
        }
    }

    AnscReleaseLock(&pMyObject->PartyTableLock);

    if ( (pMyObject->LpcOpmode  == ANSC_LPC_OPMODE_CLIENT) &&
         (pMyObject->bConnected == FALSE                 ) )
    {
        pMyObject->ConnectToManager((ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}
