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

    module:	web_vho_management.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Web Virtual Host Object.

        *   WebVhoAttachRoo
        *   WebVhoDetachRoo
        *   WebVhoDeleteRoo
        *   WebVhoIdentifyGso
        *   WebVhoGetGso
        *   WebVhoAddGso
        *   WebVhoDelGso
        *   WebVhoDelAllGsos

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#include "web_vho_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoAttachRoo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRoo
            );

    description:

        This function is called to attach a Resource Owner Object to
        the virtual host.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRoo
                Specifies the Resource Owner Object to be attached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoAttachRoo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRoo
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject        = (PWEB_VIRTUAL_HOST_OBJECT    )hThisObject;
    PHTTP_HFP_INTERFACE             pHfpIf           = (PHTTP_HFP_INTERFACE         )pMyObject->hHfpIf;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator = (PWEB_RESOURCE_LOCATOR_OBJECT)pMyObject->hResourceLocator;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner   = (PWEB_RESOURCE_OWNER_OBJECT  )hRoo;

    pResourceOwner->hOwnerContext = (ANSC_HANDLE)pMyObject;

    pResourceOwner->SetHfpIf((ANSC_HANDLE)pResourceOwner, (ANSC_HANDLE)pHfpIf);

    returnStatus =
        pResourceLocator->AddResourceOwner
            (
                (ANSC_HANDLE)pResourceLocator,
                pResourceOwner->GetPathName((ANSC_HANDLE)pResourceOwner),
                (ANSC_HANDLE)pResourceOwner
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoDetachRoo
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to detach a Resource Owner Object by
        matching the path name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path of the Resource Owner Object to be
                detached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoDetachRoo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject        = (PWEB_VIRTUAL_HOST_OBJECT    )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator = (PWEB_RESOURCE_LOCATOR_OBJECT)pMyObject->hResourceLocator;

    returnStatus =
        pResourceLocator->PopResourceOwner
            (
                (ANSC_HANDLE)pResourceLocator,
                path
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoDeleteRoo
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to delete a Resource Owner Object by
        matching the path name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path of the Resource Owner Object to be
                deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoDeleteRoo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject        = (PWEB_VIRTUAL_HOST_OBJECT    )hThisObject;
    PWEB_RESOURCE_LOCATOR_OBJECT    pResourceLocator = (PWEB_RESOURCE_LOCATOR_OBJECT)pMyObject->hResourceLocator;

    returnStatus =
        pResourceLocator->DelResourceOwner
            (
                (ANSC_HANDLE)pResourceLocator,
                path,
                TRUE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebVhoIdentifyGso
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBmoReq
            );

    description:

        This function is called to extract the "Cookie" header field
        from the request and identify the corresonding session object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBmoReq
                Specifies the HTTP request message object.

    return:     general session object.

**********************************************************************/

ANSC_HANDLE
WebVhoIdentifyGso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject      = (PWEB_VIRTUAL_HOST_OBJECT   )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty      = (PWEB_VIRTUAL_HOST_PROPERTY )&pMyObject->Property;
    PHTTP_BMO_REQ_OBJECT            pBmoReq        = (PHTTP_BMO_REQ_OBJECT       )hBmoReq;
    PHTTP_HFO_COOKIE                pHttpHfoCookie = (PHTTP_HFO_COOKIE           )NULL;
    PHTTP_COOKIE_CONTENT            pCookieContent = (PHTTP_COOKIE_CONTENT       )NULL;
    PWEB_GENERAL_SESSION_OBJECT     pSession       = (PWEB_GENERAL_SESSION_OBJECT)NULL;
    ULONG                           ulSessionId    = (ULONG                      )0xFFFFFFFF;
    char*                           pLsmIdentifier = (char*                      )NULL;
    char                            pEmptyLsmId[]  = {0};
    BOOL                            bLsmIdAlloc    = (BOOL                       )FALSE;
    ULONG                           ulClientAddr   = (ULONG                      )AnscReadUlong(pBmoReq->GetClientAddr((ANSC_HANDLE)pBmoReq));
    USHORT                          usClientPort   = (USHORT                     )pBmoReq->GetClientPort((ANSC_HANDLE)pBmoReq);
    BOOL                            bGotSessionId  = (BOOL                       )FALSE;
    BOOL                            bGotLsmId      = (BOOL                       )FALSE;
    ULONG                           i              = 0;
    ULONG                           ulCookieIndex  = 0;

    if ( !pProperty->bEnableSessionTracking )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pHttpHfoCookie =
        (PHTTP_HFO_COOKIE)pBmoReq->GetHeaderField2
            (
                (ANSC_HANDLE)pBmoReq,
                HTTP_HEADER_ID_COOKIE,
                ulCookieIndex ++
            );

    while ( pHttpHfoCookie )
    {
        for ( i = 0; i < pHttpHfoCookie->CookieCount; i++ )
        {
            pCookieContent = &pHttpHfoCookie->CookieArray[i];

            if ( !bGotSessionId     &&
                 strcmp(pCookieContent->Name,pProperty->VhoCookieName) == 0 )
            {
                PUCHAR              pCookieValue = (PUCHAR)pCookieContent->Value;

                /* Some clients will use double quote in cookie value
                 * although we don't specify in Set-Cookie.
                 */
                if ( pCookieValue[0] == '"' )
                {
                    pCookieValue ++;
                }
                
                ulSessionId = (ULONG)_ansc_atol((const char *)pCookieValue);
                bGotSessionId = TRUE;
            }
            else if ( !bGotLsmId      &&
                      strcmp(pCookieContent->Name,pProperty->LsmCookieName) == 0 )
            {
                bGotLsmId      = TRUE;
                pLsmIdentifier = pCookieContent->Value;

                /* Some clients will use double quote in cookie value
                 * although we don't specify in Set-Cookie. This may 
                 * cause session match failure, and therefore authentication
                 * may fail.
                 */
                
                if ( pLsmIdentifier )
                {
                    ULONG           ulLen = (ULONG)AnscSizeOfString(pLsmIdentifier);

                    if ( ulLen == 2 && *pLsmIdentifier == '"' && *(pLsmIdentifier + 1) == '"' )
                    {
                        pLsmIdentifier = pEmptyLsmId;
                    }
                    else if ( ulLen > 2 && *pLsmIdentifier == '"' && *(pLsmIdentifier + ulLen - 1) == '"' )
                    {
                        pLsmIdentifier = AnscAllocateMemory(ulLen);
                        AnscCopyMemory(pLsmIdentifier, pCookieContent->Value + 1, ulLen - 2);
                        pLsmIdentifier[ulLen - 2] = 0;
                        bLsmIdAlloc = TRUE;
                    }
                }
            }
        }

        if ( bGotSessionId && bGotLsmId )
        {
            break;
        }

        pHttpHfoCookie =
            (PHTTP_HFO_COOKIE)pBmoReq->GetHeaderField2
                (
                    (ANSC_HANDLE)pBmoReq,
                    HTTP_HEADER_ID_COOKIE, 
                    ulCookieIndex ++
                );
    } 

    pSession =
        (PWEB_GENERAL_SESSION_OBJECT)pMyObject->GetGso
            (
                (ANSC_HANDLE)pMyObject,
                ulSessionId,
                pLsmIdentifier,
                (PUCHAR)&ulClientAddr,
                usClientPort
            );

    if ( bLsmIdAlloc && pLsmIdentifier )
    {
        AnscFreeMemory(pLsmIdentifier);
    }

    return  (ANSC_HANDLE)pSession;
}


static
ANSC_HANDLE
WebVhoGetOldestGso
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId,
        char*                       identifier,
        PUCHAR                      address,
        PULONG                      pulSessionCount
    )
{
    UNREFERENCED_PARAMETER(identifier);
    UNREFERENCED_PARAMETER(ulSessionId);
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_GENERAL_SESSION_OBJECT     pSession     = NULL;
    ULONG                           ulClientAddr = 0;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i;
    ULONG                           ulCSessCount = 0;
    PWEB_GENERAL_SESSION_OBJECT     pOldSession  = NULL;

    for ( i = 0; i < WEB_VHO_GSO_TABLE_SIZE; i ++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->GsoTable[i]);

        while ( pSLinkEntry )
        {
            pSession     = ACCESS_WEB_GENERAL_SESSION_OBJECT(pSLinkEntry);
            ulClientAddr = *(PULONG)pSession->GetClientAddr((ANSC_HANDLE)pSession);
            pSession->GetClientPort((ANSC_HANDLE)pSession);
            pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

            if ( pSession->LsmMaxAge != 0 && ulClientAddr == AnscReadUlong(address) )
            {
                ulCSessCount ++;
                if ( !pOldSession )
                {
                    pOldSession = pSession;
                }
                else 
                if ( 
                       pSession->LastReqAtInSec <= pOldSession->LastReqAtInSec &&
                       pSession->LastRepAtInSec <= pOldSession->LastRepAtInSec
                   )
                {
                    pOldSession = pSession;
                }
            }
        }
    }

    *pulSessionCount = ulCSessCount;

    return  (ANSC_HANDLE)pSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebVhoGetGso
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSessionId,
                char*                       identifier,
                PUCHAR                      address,
                USHORT                      port
            );

    description:

        This function is called to retrieve a General Session Object by
        matching the session identifier.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSessionId
                Specifies the session id assigned by this Virtual Host
                Object.

                char*                       identifier
                Specifies the identifier of the General Session Object
                to be matched.

                PUCHAR                      address
                Specifies the client IP address to be matched.

                USHORT                      port
                Specifies the client port number to be matched.

    return:     general session object.

**********************************************************************/

ANSC_HANDLE
WebVhoGetGso
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId,
        char*                       identifier,
        PUCHAR                      address,
        USHORT                      port
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_SITE_MANAGER_OBJECT        pSiteManager = (PWEB_SITE_MANAGER_OBJECT  )pMyObject->hOwnerContext;
    WEB_SITE_MANAGER_PROPERTY       SMProperty;
    PWEB_GENERAL_SESSION_OBJECT     pSession     = NULL;
    ULONG                           ulClientAddr = 0;
    USHORT                          usClientPort = 0;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(ulSessionId, WEB_VHO_GSO_TABLE_SIZE);
    ULONG                           i;
    ULONG                           ulCSessCount = 0;
    PWEB_GENERAL_SESSION_OBJECT     pOldSession  = NULL;

    AnscAcquireLock(&pMyObject->GsoTableLock);

    if ( ulSessionId != 0xFFFFFFFF )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->GsoTable[ulHashIndex]);

        while ( pSLinkEntry )
        {
            pSession     = ACCESS_WEB_GENERAL_SESSION_OBJECT(pSLinkEntry);
            ulClientAddr = *(PULONG)pSession->GetClientAddr((ANSC_HANDLE)pSession);
            usClientPort = pSession->GetClientPort((ANSC_HANDLE)pSession);
            pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

            if ( pSession->GetLsmMaxAge((ANSC_HANDLE)pSession) == 0 )
            {   
                /* this session is marked to be closed */
                continue;
            }

            if ( (pSession->GetSessionId((ANSC_HANDLE)pSession) == ulSessionId) && 
                 (ulClientAddr == AnscReadUlong(address)) )
            {
                char*               pLsmId       = pSession->GetLsmIdentifier((ANSC_HANDLE)pSession);

                if ( !identifier )
                {
                    if ( !pLsmId || AnscSizeOfString(pLsmId) == 0 )
                    {
                        AnscReleaseLock(&pMyObject->GsoTableLock);

                        pSession->AcquireAccess((ANSC_HANDLE)pSession);

                        return  (ANSC_HANDLE)pSession;
                    }
                    else
                    {
                        break;
                    }
                }
                else if ( strcmp(pLsmId,identifier) == 0 )
                {
                    AnscReleaseLock(&pMyObject->GsoTableLock);

                    pSession->AcquireAccess((ANSC_HANDLE)pSession);

                    return  (ANSC_HANDLE)pSession;
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    {
        for ( i = 0; i < WEB_VHO_GSO_TABLE_SIZE; i ++ )
        {
            pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->GsoTable[i]);

            while ( pSLinkEntry )
            {
                pSession     = ACCESS_WEB_GENERAL_SESSION_OBJECT(pSLinkEntry);
                ulClientAddr = *(PULONG)pSession->GetClientAddr((ANSC_HANDLE)pSession);
                usClientPort = pSession->GetClientPort((ANSC_HANDLE)pSession);
                pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

                if ( (ulClientAddr == AnscReadUlong(address)) && usClientPort == port )
                {
                    AnscReleaseLock(&pMyObject->GsoTableLock);

                    /* same connection */
                    pSession->AcquireAccess((ANSC_HANDLE)pSession);

                    return  (ANSC_HANDLE)pSession;
                }
            }
        }
    }

    pSiteManager->GetProperty((ANSC_HANDLE)pSiteManager, &SMProperty);

    pOldSession = 
        (PWEB_GENERAL_SESSION_OBJECT)WebVhoGetOldestGso
            (
                (ANSC_HANDLE)pMyObject,
                ulSessionId,
                identifier,
                address,
                &ulCSessCount
            );

    if ( pOldSession && SMProperty.MaxSessionCountPerPeer != 0 && ulCSessCount >= SMProperty.MaxSessionCountPerPeer )
    {
        pOldSession->Terminate((ANSC_HANDLE)pOldSession);
    }

    AnscReleaseLock(&pMyObject->GsoTableLock);

    /*
     * We couldn't find a general session object for this client, we shall create a new one and
     * let the Logical Session Manager classify the client based on his/her identifier.
     */
    pSession =
        (PWEB_GENERAL_SESSION_OBJECT)pMyObject->AddGso
            (
                (ANSC_HANDLE)pMyObject,
                identifier,
                address,
                port
            );

    return  (ANSC_HANDLE)pSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebVhoAddGso
            (
                ANSC_HANDLE                 hThisObject,
                char*                       identifier,
                PUCHAR                      address,
                USHORT                      port
            );

    description:

        This function is called to add a General Session Object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       identifier
                Specifies the identifier of the General Session Object
                to be added, which could be NULL.

                PUCHAR                      address
                Specifies the client IP address to be added.

                USHORT                      port
                Specifies the client port number to be added.

    return:     general session object.

**********************************************************************/

ANSC_HANDLE
WebVhoAddGso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       identifier,
        PUCHAR                      address,
        USHORT                      port
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_VIRTUAL_HOST_PROPERTY      pProperty    = (PWEB_VIRTUAL_HOST_PROPERTY)&pMyObject->Property;
    PHTTP_HFP_INTERFACE             pHfpIf       = (PHTTP_HFP_INTERFACE       )pMyObject->hHfpIf;
    PWEB_LSM_INTERFACE              pLsmIf       = (PWEB_LSM_INTERFACE        )pMyObject->hLsmIf;
    PWEB_GENERAL_SESSION_OBJECT     pSession     = NULL;
    ULONG                           ulHashIndex  = 0;

    pMyObject->GlobalSidCount++;

    if ( pMyObject->GlobalSidCount >= 0x7FFFFFFF )
    {
        pMyObject->GlobalSidCount = 0;
    }

    pSession =
        (PWEB_GENERAL_SESSION_OBJECT)WebCreateGeneralSession
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSession )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pSession->SetHfpIf      ((ANSC_HANDLE)pSession, (ANSC_HANDLE)pHfpIf          );
        pSession->SetSessionId  ((ANSC_HANDLE)pSession, pMyObject->GlobalSidCount    );
        pSession->SetClientAddr ((ANSC_HANDLE)pSession, address                      );
        pSession->SetClientPort ((ANSC_HANDLE)pSession, port                         );
        pSession->SetLsmContext ((ANSC_HANDLE)pSession, (ANSC_HANDLE)NULL            );
        pSession->SetLsmMaxAge  ((ANSC_HANDLE)pSession, pProperty->LsmCookieMaxAge   );
        pSession->SetLsmExpire  ((ANSC_HANDLE)pSession, pProperty->SessionTimeOut    );
        pSession->SetMaxLifespan((ANSC_HANDLE)pSession, pProperty->SessionMaxLifespan);
    }

    if ( identifier )
    {
        pSession->SetLsmIdentifier((ANSC_HANDLE)pSession, identifier);

        pLsmIf->ClassifyClient
                (
                    pLsmIf->hOwnerContext,
                    (ANSC_HANDLE)pSession
                );
    }
    else
    {
        pLsmIf->NewContact
                (
                    pLsmIf->hOwnerContext,
                    (ANSC_HANDLE)pSession
                );
    }

    ulHashIndex         = AnscHashUlong(pSession->SessionId, WEB_VHO_GSO_TABLE_SIZE);
    pSession->HashIndex = ulHashIndex;

    AnscAcquireLock(&pMyObject->GsoTableLock);

    pSession->AcquireAccess((ANSC_HANDLE)pSession);
    pSession->Engage       ((ANSC_HANDLE)pSession);

    AnscSListPushEntry(&pMyObject->GsoTable[ulHashIndex], &pSession->Linkage);

    AnscReleaseLock(&pMyObject->GsoTableLock);

    return  (ANSC_HANDLE)pSession;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoDelGso
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSessionId,
                PUCHAR                      address,
                USHORT                      port
            );

    description:

        This function is called to delete a General Session Object by
        matching the session identifier.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSessionId
                Specifies the session id assigned by this Virtual Host
                Object.

                PUCHAR                      address
                Specifies the client IP address to be matched.

                USHORT                      port
                Specifies the client port number to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoDelGso
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId,
        PUCHAR                      address,
        USHORT                      port
    )
{
    UNREFERENCED_PARAMETER(port);
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_LSM_INTERFACE              pLsmIf       = (PWEB_LSM_INTERFACE        )pMyObject->hLsmIf;
    PWEB_GENERAL_SESSION_OBJECT     pSession     = NULL;
    ULONG                           ulClientAddr = 0;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(ulSessionId, WEB_VHO_GSO_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->GsoTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->GsoTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSession     = ACCESS_WEB_GENERAL_SESSION_OBJECT(pSLinkEntry);
        ulClientAddr = *(PULONG)pSession->GetClientAddr((ANSC_HANDLE)pSession);
        pSLinkEntry  = AnscSListGetNextEntry(pSLinkEntry);

        if ( (pSession->GetSessionId((ANSC_HANDLE)pSession) == ulSessionId           ) &&
             (ulClientAddr                                  == AnscReadUlong(address)) )
        {
            AnscSListPopEntryByLink(&pMyObject->GsoTable[ulHashIndex], &pSession->Linkage);

            pSession->AcquireAccess((ANSC_HANDLE)pSession);

            pLsmIf->EndSession
                    (
                        pLsmIf->hOwnerContext,
                        (ANSC_HANDLE)pSession
                    );

            pSession->ReleaseAccess((ANSC_HANDLE)pSession);
            pSession->Cancel       ((ANSC_HANDLE)pSession);
            pSession->Remove       ((ANSC_HANDLE)pSession);
        }
    }

    AnscReleaseLock(&pMyObject->GsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebVhoDelAllGsos
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all General Session Objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebVhoDelAllGsos
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PWEB_VIRTUAL_HOST_OBJECT        pMyObject    = (PWEB_VIRTUAL_HOST_OBJECT  )hThisObject;
    PWEB_LSM_INTERFACE              pLsmIf       = (PWEB_LSM_INTERFACE        )pMyObject->hLsmIf;
    PWEB_GENERAL_SESSION_OBJECT     pSession     = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->GsoTableLock);

    for ( i = 0; i < WEB_VHO_GSO_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->GsoTable[i]);

        while ( pSLinkEntry )
        {
            pSession    = ACCESS_WEB_GENERAL_SESSION_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListPopEntry(&pMyObject->GsoTable[i]);

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

    AnscReleaseLock(&pMyObject->GsoTableLock);

    return  ANSC_STATUS_SUCCESS;
}
