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

    module:	web_vho_operation.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Web Virtual Host Object.

        *   WebVhoAcquireAccess
        *   WebVhoReleaseAccess
        *   WebVhoEngage
        *   WebVhoCancel
        *   WebVhoMatchName
        *   WebVhoSessionTdoInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#include "web_vho_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoAcquireAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire the access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;

    AnscAcquireLock(&pMyObject->SyncLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoReleaseAccess
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to release the access to the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;

    AnscReleaseLock(&pMyObject->SyncLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject        = (PWEB_VIRTUAL_HOST_OBJECT     )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator = (PWEB_RESOURCE_LOCATOR_OBJECT )pMyObject->hResourceLocator;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo      = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    pResourceLocator->Engage((ANSC_HANDLE)pResourceLocator);

    pSessionTdo->SetTimerType((ANSC_HANDLE)pSessionTdo, ANSC_TIMER_TYPE_PERIODIC     );
    pSessionTdo->SetInterval ((ANSC_HANDLE)pSessionTdo, WEB_VHO_SESSION_TIME_INTERVAL);
    pSessionTdo->Start       ((ANSC_HANDLE)pSessionTdo);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoCancel
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
WebVhoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject        = (PWEB_VIRTUAL_HOST_OBJECT     )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator = (PWEB_RESOURCE_LOCATOR_OBJECT )pMyObject->hResourceLocator;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pSessionTdo      = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hSessionTdo;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    pResourceLocator->Cancel((ANSC_HANDLE)pResourceLocator);

    pSessionTdo->Stop((ANSC_HANDLE)pSessionTdo);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WebVhoMatchName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to match the host name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the host name to be matched.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
WebVhoMatchName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;

    if ( pProperty->HostName[0] == '*' )
    {
        return  TRUE;
    }
    else if ( strcasecmp(name, pProperty->HostName) == 0 )
    {
        return  TRUE;
    }

    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoSessionTdoInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Session Timer Descriptor Object
        to clean up all the expired session objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoSessionTdoInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_LSM_INTERFACE              pLsmIf       = (PWEB_LSM_INTERFACE        )pMyObject->hLsmIf;
    PWEB_GENERAL_SESSION_OBJECT     pSession     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulCurTime    = AnscGetTickInSeconds();
    ULONG                           i            = 0;
    BOOL                            bExpired     = FALSE;

    AnscAcquireLock(&pMyObject->GsoTableLock);

    for ( i = 0; i < WEB_VHO_GSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->GsoTable[i]);

        while ( pSLinkEntry )
        {
            pSession    = ACCESS_WEB_GENERAL_SESSION_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);
            ulCurTime   = AnscGetTickInSeconds();

            if ( pSession->bLocked )
            {
                continue;
            }
            else
            {
                ulCurTime = AnscGetTickInSecondsAbs();
            }

            if ( pSession->MaxLifespan != 0 )
            {
                bExpired = WEB_SEESION_EXPIRED(pSession->TimeStampInSec, ulCurTime, pSession->MaxLifespan);
            }

            if ( !bExpired )
            {
                bExpired = 
                    WEB_SEESION_EXPIRED(pSession->LastReqAtInSec, ulCurTime, pSession->LsmExpire) &&
                    WEB_SEESION_EXPIRED(pSession->LastRepAtInSec, ulCurTime, pSession->LsmExpire);

                if ( !bExpired )
                {
                    /* session was marked to be closed */
                    bExpired = (pSession->GetLsmMaxAge((ANSC_HANDLE)pSession) == 0);
                }
            }

            if ( bExpired )
            {
                AnscSListPopEntryByLink(&pMyObject->GsoTable[i], &pSession->Linkage);

                pSession->AcquireAccess((ANSC_HANDLE)pSession);

                pLsmIf->ExpireSession
                        (
                            pLsmIf->hOwnerContext,
                            (ANSC_HANDLE)pSession
                        );

                pSession->ReleaseAccess((ANSC_HANDLE)pSession);
                pSession->Cancel       ((ANSC_HANDLE)pSession);
                pSession->Remove       ((ANSC_HANDLE)pSession);
            }
        }
    }

    AnscReleaseLock(&pMyObject->GsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
