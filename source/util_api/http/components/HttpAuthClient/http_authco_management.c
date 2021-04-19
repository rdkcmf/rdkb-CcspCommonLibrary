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

    module:	http_authco_management.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Authentication Client Object.

        *   HttpAuthcoAddAuthInfo
        *   HttpAuthcoFindAuthInfo
        *   HttpAuthcoFindAuthInfo2

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/07/05    initial revision.

**********************************************************************/


#include "http_authco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthcoAddAuthInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                ANSC_HANDLE                 hChallenge
            )

    description:

        This function is called to save the given authentication
        info.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                ANSC_HANDLE                 hChallenge
                Specifies the authentication challenge.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthcoAddAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hChallenge
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTH_CHALLENGE            pChallenge   = (PHTTP_AUTH_CHALLENGE    )hChallenge;
    PHTTP_AUTHO_INFO                pAuthInfo    = NULL;
    PHTTP_ACM_INTERFACE             pHttpAcmIf   = (PHTTP_ACM_INTERFACE     )pMyObject->hAcmIf;
    BOOL                            bDigestAuth  = (pChallenge->AuthType == HTTP_AUTH_TYPE_DIGEST);
    PUCHAR                          pHostName    = NULL;
    PUCHAR                          pUriPath     = NULL;
    USHORT                          HostPort     = 0;
    BOOL                            bParentUri   = FALSE;

    if ( !pHttpAcmIf )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    status = pMyObject->GetRequestHostUri((ANSC_HANDLE)pMyObject, hRequest, &pHostName, &HostPort, &pUriPath);

    if ( status != ANSC_STATUS_SUCCESS )
    {
        status = ANSC_STATUS_INTERNAL_ERROR;
        goto ABNORM;
    }

    AnscAcquireLock(&pMyObject->AuthInfoListLock);

    pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->FindAuthInfo2((ANSC_HANDLE)pMyObject, hRequest, &bParentUri);

    if ( pAuthInfo )
    {
        AnscSListPopEntryByLink(&pMyObject->AuthInfoList, &pAuthInfo->Linkage);
        HttpAuthInfoRemove(pAuthInfo);
        pAuthInfo   = NULL;
    }

    HttpAuthInfoCreate(pAuthInfo);

    if ( !pAuthInfo )
    {
        status  = ANSC_STATUS_RESOURCES;
        goto EXIT;
    }

    if ( bDigestAuth )
    {
        HttpAuthDigestInfoCreate(pAuthInfo->pDigest);

        if ( !pAuthInfo->pDigest )
        {
            status  = ANSC_STATUS_RESOURCES;
            goto EXIT;
        }
    }

    /* set up authentication info */
    pAuthInfo->AuthType     = pChallenge->AuthType;
    status =
        pHttpAcmIf->GetCredential
            (
                pHttpAcmIf->hOwnerContext,
                pHostName,
                HostPort,
                pUriPath,
                &pAuthInfo->pUserName,
                &pAuthInfo->pPassword
            );

    if ( status != ANSC_STATUS_SUCCESS )
    {
        goto EXIT;
    }

    pAuthInfo->pHostName    = pHostName;
    pAuthInfo->HostPort     = HostPort;
    pAuthInfo->pUri         = pUriPath;

    if ( !bDigestAuth )
    {
        /* basic authentication */
        PHTTP_CHALLENGE_BASIC       pBasicChal  = &pChallenge->Challenge.Basic;

        pAuthInfo->pRealm   = (PUCHAR)AnscCloneString(pBasicChal->Realm);
    }
    else
    {
        /* digest authentication */
        PHTTP_CHALLENGE_DIGEST      pDigestChal = &pChallenge->Challenge.Digest;
        PUCHAR                      pChal       = (PUCHAR)pDigestChal->Realm;

        pMyObject->GetDigestAuthInfo
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pAuthInfo,
                pChal
            );
    }

    /* sweep out old entries */
    if ( TRUE )
    {
        ULONG                       ulCount = (ULONG)AnscSListQueryDepth(&pMyObject->AuthInfoList);

        while ( ulCount >= pMyObject->AuthInfoMaxCount )
        {
            PSINGLE_LINK_ENTRY          pLastEntry = NULL;

            pLastEntry = AnscSListGetLastEntry(&pMyObject->AuthInfoList);
            AnscSListPopEntryByLink(&pMyObject->AuthInfoList, pLastEntry);

            ulCount --;
        }
    }

    AnscSListPushEntry(&pMyObject->AuthInfoList, &pAuthInfo->Linkage);

    status = ANSC_STATUS_SUCCESS;

EXIT:
    AnscReleaseLock(&pMyObject->AuthInfoListLock);

ABNORM:
    if ( status != ANSC_STATUS_SUCCESS )
    {
        if ( pHostName )
        {
            AnscFreeMemory(pHostName);
        }

        if ( pUriPath )
        {
            AnscFreeMemory(pUriPath);
        }

        if ( pAuthInfo )
        {
            HttpAuthInfoRemove(pAuthInfo);
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpAuthcoFindAuthInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest
            )

    description:

        This function is called to get authentication info which
        should apply to the given request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

    return:     authentication info.

**********************************************************************/

ANSC_HANDLE
HttpAuthcoFindAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ANSC_HANDLE                     hAuthInfo    = (ANSC_HANDLE)NULL;
    PUCHAR                          pHostName    = NULL;
    PUCHAR                          pUriPath     = NULL;
    USHORT                          HostPort     = 0;
    PUCHAR                          pDomain      = NULL;
    PUCHAR                          pMatchStr    = NULL;

    status = pMyObject->GetRequestHostUri((ANSC_HANDLE)pMyObject, hRequest, &pHostName, &HostPort, &pUriPath);

    if ( status != ANSC_STATUS_SUCCESS || !pUriPath || !pHostName )
    {
        goto EXIT;
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->AuthInfoList);
    while (pSLinkEntry)
    {
        pAuthInfo   = ACCESS_HTTP_AUTHO_INFO_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pAuthInfo->HostPort != HostPort )
        {
            continue;
        }

        if ( !strcasecmp((char*)pAuthInfo->pHostName, (char*)pHostName) == 0 )
        {
            continue;
        }

        if ( strcmp((char*)pAuthInfo->pUri, (char*)pUriPath) == 0 )
        {
            hAuthInfo   = (ANSC_HANDLE)pAuthInfo;
            break;
        }

        pMatchStr   = (PUCHAR)_ansc_strstr((const char*)pUriPath, (const char*)pAuthInfo->pUri);
        if ( pMatchStr == pUriPath )
        {
            hAuthInfo   = (ANSC_HANDLE)pAuthInfo;
            break;
        }

        if ( pAuthInfo->AuthType == HTTP_AUTH_TYPE_DIGEST )
        {
            PUCHAR                          *pURIs;
            ULONG                           i, ulCount;
            BOOL                            bMatch  = FALSE;

            if ( pAuthInfo->pDigest->pDomainURIs )
            {
                ulCount = pAuthInfo->pDigest->pDomainURIs->NumURIs;
                pURIs   = pAuthInfo->pDigest->pDomainURIs->pURIs;

                for ( i = 0; i < ulCount; i ++)
                {
                    pMatchStr   = (PUCHAR)_ansc_strstr((const char*)pUriPath, (const char*)pURIs[i]);
                    if ( pMatchStr == pUriPath )
                    {
                        bMatch  = TRUE;
                        break;
                    }
                }

                if ( bMatch )
                {
                    break;
                }
            }
            else
            {
                pDomain = pAuthInfo->pDigest->pDomain;

                if ( pDomain )
                {
                	pMatchStr   = (PUCHAR)_ansc_strstr((const char*)pUriPath, (const char*)pDomain);
					if ( pMatchStr == pUriPath )
					{
						break;
					}
                }
            }
        }
    }

EXIT:

    if ( pHostName )
    {
        AnscFreeMemory(pHostName);
    }

    if ( pUriPath )
    {
        AnscFreeMemory(pUriPath);
    }

    /* move this info to front-most */
    if ( hAuthInfo )
    {
        pAuthInfo   = (PHTTP_AUTHO_INFO)hAuthInfo;

        AnscSListPopEntryByLink(&pMyObject->AuthInfoList, &pAuthInfo->Linkage);
        AnscSListPushEntry(&pMyObject->AuthInfoList, &pAuthInfo->Linkage);
    }

    return hAuthInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpAuthcoFindAuthInfo2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                PBOOL                       pbParentUri
            )

    description:

        This function is called to get authentication info that
        should apply to the given request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                PBOOL                       pbParentUri
                On return, it indicates if the request URI is the
                parent of the one that is saved in the returned
                authentication info. In this case, we will update
                the saved URI with the request URI.

    return:     authentication info.

**********************************************************************/

ANSC_HANDLE
HttpAuthcoFindAuthInfo2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PBOOL                       pbParentUri
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ANSC_HANDLE                     hAuthInfo    = (ANSC_HANDLE)NULL;
    PUCHAR                          pHostName    = NULL;
    PUCHAR                          pUriPath     = NULL;
    USHORT                          HostPort     = 0;
    PUCHAR                          pDomain      = NULL;
    PUCHAR                          pMatchStr;

    *pbParentUri    = FALSE;

    status = pMyObject->GetRequestHostUri((ANSC_HANDLE)pMyObject, hRequest, &pHostName, &HostPort, &pUriPath);

    if ( status != ANSC_STATUS_SUCCESS || !pUriPath || !pHostName )
    {
        goto EXIT;
    }

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->AuthInfoList);
    while (pSLinkEntry)
    {
        pAuthInfo   = ACCESS_HTTP_AUTHO_INFO_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pAuthInfo->HostPort != HostPort )
        {
            continue;
        }

        if ( !strcasecmp((char*)pAuthInfo->pHostName, (char*)pHostName) == 0 )
        {
            continue;
        }

        if ( strcmp((char*)pAuthInfo->pUri, (char*)pUriPath) == 0 )
        {
            hAuthInfo       = (ANSC_HANDLE)pAuthInfo;
            break;
        }

        pMatchStr   = (PUCHAR)_ansc_strstr((const char*)pUriPath, (const char*)pAuthInfo->pUri);
        if ( pMatchStr == pUriPath )
        {
            hAuthInfo       = (ANSC_HANDLE)pAuthInfo;
            break;
        }

        pMatchStr   = (PUCHAR)_ansc_strstr((const char*)pAuthInfo->pUri, (const char*)pUriPath);
        if ( pMatchStr == pAuthInfo->pUri )
        {
            hAuthInfo       = (ANSC_HANDLE)pAuthInfo;
            *pbParentUri    = TRUE;
            break;
        }

        if ( pAuthInfo->AuthType == HTTP_AUTH_TYPE_DIGEST )
        {
            PUCHAR                          *pURIs;
            ULONG                           i, ulCount;
            BOOL                            bMatch  = FALSE;

            if ( pAuthInfo->pDigest->pDomainURIs )
            {
                ulCount = pAuthInfo->pDigest->pDomainURIs->NumURIs;
                pURIs   = pAuthInfo->pDigest->pDomainURIs->pURIs;

                for ( i = 0; i < ulCount; i ++)
                {
                    pMatchStr   = (PUCHAR)_ansc_strstr((const char*)pUriPath, (const char*)pURIs[i]);
                    if ( pMatchStr == pUriPath )
                    {
                        bMatch  = TRUE;
                        break;
                    }
                }

                if ( bMatch )
                {
                    hAuthInfo   = (ANSC_HANDLE)pAuthInfo;
                    break;
                }
            }
            else
            {
                pDomain = pAuthInfo->pDigest->pDomain;

                pMatchStr   = (PUCHAR)_ansc_strstr((const char*)pUriPath, (const char*)pDomain);
                if ( pMatchStr == pUriPath )
                {
                    hAuthInfo   = (ANSC_HANDLE)pAuthInfo;
                    break;
                }
            }
        }
    }

EXIT:

    if ( pHostName )
    {
        AnscFreeMemory(pHostName);
    }

    if ( pUriPath )
    {
        AnscFreeMemory(pUriPath);
    }

    /* move this info to front-most */
    if ( hAuthInfo )
    {
        pAuthInfo   = (PHTTP_AUTHO_INFO)hAuthInfo;

        AnscSListPopEntryByLink(&pMyObject->AuthInfoList, &pAuthInfo->Linkage);
        AnscSListPushEntry(&pMyObject->AuthInfoList, &pAuthInfo->Linkage);
    }

    return hAuthInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpAuthcoGetNC
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                ANSC_HANDLE                 hAuthInfo
            )

    description:

        This function is called to get NC value to be used in the
        current outgoing request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                ANSC_HANDLE                 hAuthInfo
                Specifies the authentication info.

    return:     status of operation.

**********************************************************************/

ULONG
HttpAuthcoGetNC
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hAuthInfo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hRequest);
    PHTTP_AUTHO_INFO                pAuthInfo    = (PHTTP_AUTHO_INFO       )hAuthInfo;
    ULONG                           ulNC         = 0;
    PULONG                          pulNC        = NULL;

    if ( pAuthInfo->pDigest )
    {
        pulNC   = &pAuthInfo->pDigest->NC;
    }

    if ( pulNC  )
    {
        ulNC    = ++(*pulNC);
    }

    return ulNC;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthcoClearAllAuthInfo
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to clear all cached authentication
        info.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthcoClearAllAuthInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->AuthInfoListLock);

    /* remove auth info */
    pSLinkEntry = AnscSListPopEntry(&pMyObject->AuthInfoList);
    while (pSLinkEntry)
    {
        pAuthInfo   = ACCESS_HTTP_AUTHO_INFO_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->AuthInfoList);

        HttpAuthInfoRemove(pAuthInfo);
    }

    AnscReleaseLock(&pMyObject->AuthInfoListLock);

    return ANSC_STATUS_SUCCESS;
}


