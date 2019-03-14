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

    module:	tls_smo_operation.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the TLS Session Manager Object.

        *   TlsSmoEngage
        *   TlsSmoCancel
        *   TlsSmoPatrolTimerInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/16/03    initial revision.

**********************************************************************/


#include "tls_smo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject    = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimer = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimer;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pPatrolTimer->Start((ANSC_HANDLE)pPatrolTimer);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject    = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pPatrolTimer = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hPatrolTimer;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pPatrolTimer->Stop((ANSC_HANDLE)pPatrolTimer);

    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsSmoPatrolTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when the patrol timer is fired.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsSmoPatrolTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_SESSION_MANAGER_OBJECT     pMyObject     = (PTLS_SESSION_MANAGER_OBJECT  )hThisObject;
    PTLS_SESSION_ENTRY_OBJECT       pSessionEntry = (PTLS_SESSION_ENTRY_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = (PSINGLE_LINK_ENTRY           )NULL;
    ULONG                           ulTimestamp   = 0;
    ULONG                           ulCurTime     = AnscGetTickInSeconds();
    ULONG                           i             = 0;

    AnscAcquireLock(&pMyObject->SeoTableLock);

    for ( i = 0; i < TLS_SMO_SEO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->SeoTable[i]);

        while ( pSLinkEntry )
        {
            pSessionEntry = ACCESS_TLS_SESSION_ENTRY_OBJECT(pSLinkEntry);
            pSLinkEntry   = AnscQueueGetNextEntry(pSLinkEntry);
            ulTimestamp   = pSessionEntry->Timestamp;

            if ( (ulTimestamp - ulCurTime) > TLS_SMO_SESSION_TIMEOUT )
            {
                AnscQueuePopEntryByLink(&pMyObject->SeoTable[i], &pSessionEntry->Linkage);

                pSessionEntry->Remove((ANSC_HANDLE)pSessionEntry);
            }
        }
    }

    AnscReleaseLock(&pMyObject->SeoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
