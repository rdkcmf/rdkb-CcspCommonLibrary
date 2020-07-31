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

    module:	web_autho_process.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Web Auth Server Object.

        *   WebAuthsoAuthenticateReq
        *   WebAuthsoVerifyCredential
        *   WebAuthsoGenerateAuthResponse
        *   WebAuthsoGenerateNonce

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/12/05    initial revision.

**********************************************************************/


#include "web_authso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebAuthsoAuthenticateReq
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                ANSC_HANDLE                 hResponse
            )

    description:

        This function is called to authenticate the request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                HTTP request object.

                ANSC_HANDLE                 hResponse
                HTTP response object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebAuthsoAuthenticateReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PWEB_AUTH_SERVER_OBJECT         pMyObject    = (PWEB_AUTH_SERVER_OBJECT)hThisObject;
    PWEB_AUTH_SESSION_INFO          pSessInfo    = (PWEB_AUTH_SESSION_INFO )NULL;
    PHTTP_BMO_REP_OBJECT            pResponse    = (PHTTP_BMO_REP_OBJECT   )hResponse;
    PWEB_AUTH_SERVER_PROPERTY       pProperty    = &pMyObject->Property;
    PWEB_GENERAL_SESSION_OBJECT     pWebGso      = NULL;
    BOOL                            bNonceExpired= FALSE;

    if ( !pMyObject->IsEnabled((ANSC_HANDLE)pMyObject) )
    {
        return ANSC_STATUS_SUCCESS;
    }

    pWebGso = (PWEB_GENERAL_SESSION_OBJECT)pResponse->GetRecvContext((ANSC_HANDLE)pResponse);

    if ( !pWebGso )
    {
        /* no session associated - in case session tracking is not enabled */
        return ANSC_STATUS_NOT_SUPPORTED;
    }
    else
    {
        pSessInfo   = (PWEB_AUTH_SESSION_INFO)pWebGso->GetAuthSessInfo((ANSC_HANDLE)pWebGso);
    }

    /* verify credential specified in request */
    status  = 
        pMyObject->VerifyCredential
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)hRequest,
                (ANSC_HANDLE)pSessInfo
            );

    if ( status != ANSC_STATUS_SUCCESS )
    {
        if ( status == ANSC_STATUS_DO_IT_AGAIN || pSessInfo->Nonce[0] == 0 )
        {
            if ( pSessInfo->Nonce[0] != 0 )
            {
                bNonceExpired             = TRUE;
            }

            /* generate a new Nonce */
            status = 
                pMyObject->GenNonce
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSessInfo->Nonce,
                        ANSC_MD5_OUTPUT_SIZE * 2 + 1
                    );

            pSessInfo->NonceTimestamp = AnscGetTickInSeconds();
            pSessInfo->NC             = 0;
        }

        pSessInfo->ServerAuthInfo = *pProperty;

        if ( pProperty->AuthType == HTTP_AUTH_TYPE_BASIC )
        {
            status = 
                pMyObject->GenBasicResponse
                    (
                        (ANSC_HANDLE)pMyObject,
                        hResponse,
                        pProperty->Realm
                    );
        }
        else
        {
            status = 
                pMyObject->GenDigestResponse
                    (
                        (ANSC_HANDLE)pMyObject,
                        hResponse,
                        pProperty->Realm,
                        ( pProperty->Domain[0] == 0 ) ? NULL : pProperty->Domain,
                        (PUCHAR)(( pProperty->Algorithm == WEB_AUTH_ALGORITHM_MD5_SESS ) ? HTTP_AUTH_NAME_md5_sess : HTTP_AUTH_NAME_md5),
                        (PUCHAR)(pProperty->bNoQop ? NULL : HTTP_AUTH_NAME_auth),
                        pSessInfo->Nonce,
                        bNonceExpired
                    );
        }

        if ( status == ANSC_STATUS_SUCCESS )
        {
            status = ANSC_STATUS_BAD_AUTH_DATA;
        }
        else
        {
            status = ANSC_STATUS_FAILURE;
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebAuthsoVerifyCredential
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                ANSC_HANDLE                 hWaSessInfo
            )

    description:

        This function is called to verify user's credential.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                HTTP request object.

                ANSC_HANDLE                 hWaSessInfo
                Web auth info used to verify user's credential.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebAuthsoVerifyCredential
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hWaSessInfo
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PWEB_AUTH_SERVER_OBJECT         pMyObject    = (PWEB_AUTH_SERVER_OBJECT)hThisObject;
    PWEB_AUTH_SESSION_INFO          pSessInfo    = (PWEB_AUTH_SESSION_INFO )hWaSessInfo;
    PWEB_AUTH_SERVER_PROPERTY       pProperty    = &pMyObject->Property;
    PHTTP_AUTHO_INFO                pAuthInfo    = (PHTTP_AUTHO_INFO       )NULL;
    PWEB_ACM_INTERFACE              pAcmIf       = (PWEB_ACM_INTERFACE     )pMyObject->hAcmIf;
    PUCHAR                          pHostName    = NULL;
    PUCHAR                          pUriPath     = NULL;
    USHORT                          HostPort     = 0;
    PUCHAR                          pPassword    = NULL;

    if ( !pAcmIf )
    {
        /*
         * !!! If this interface is not specified, we assume other means will be
         * used for HTTP authentication such as BSP-based authentication. We must 
         * pay attention to this issue in order not to bypass authentication by
         * programming mistakes.
         */
        status = ANSC_STATUS_SUCCESS;
        goto EXIT;
    }

    if ( !pSessInfo )
    {
        pSessInfo   = &pMyObject->DefaultSessInfo;
    }

    pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->GetAuthInfo((ANSC_HANDLE)pMyObject, hRequest);

    if ( !pAuthInfo )
    {
        status = ANSC_STATUS_BAD_AUTH_DATA;
        goto EXIT;
    }

    status = 
        pMyObject->GetRequestHostUri
            (
                (ANSC_HANDLE)pMyObject, 
                hRequest, 
                &pHostName, 
                &HostPort, 
                &pUriPath
            );

    if ( status != ANSC_STATUS_SUCCESS )
    {
        status = ANSC_STATUS_BAD_AUTH_DATA;
        goto EXIT;
    }

    if ( pAuthInfo->AuthType == HTTP_AUTH_TYPE_BASIC )
    {
        status = 
            pAcmIf->GetCredential
                (
                    pAcmIf->hOwnerContext,
                    pHostName,
                    HostPort,
                    pUriPath,
                    pAuthInfo->pUserName,
                    pAuthInfo->pRealm,
                    &pPassword
                );
    }
    else
    {
        status = 
            pAcmIf->GetCredential
                (
                    pAcmIf->hOwnerContext,
                    pHostName,
                    HostPort,
                    pAuthInfo->pDigest->pDigestUri,    /* use digest uri instead of requst uri since proxy may change it */
                    pAuthInfo->pUserName,
                    pAuthInfo->pRealm,
                    &pPassword
                );
    }

    if ( status == ANSC_STATUS_PASS_THROUGH )
    {
        /* in this case, we assume authentication will be handled by application, could it be BSP page e.g. */
        status = ANSC_STATUS_SUCCESS;
        goto EXIT;
    }

    if ( status != ANSC_STATUS_SUCCESS )
    {
        /* user does not exist */
        status = ANSC_STATUS_BAD_AUTH_DATA;
        goto EXIT;
    }

    status =
        pMyObject->Verify
            (
                (ANSC_HANDLE)pMyObject,
                hRequest,
                (ANSC_HANDLE)pAuthInfo,
                pPassword,
                pProperty->AuthType,
                pProperty->Realm,
                pSessInfo->Nonce,
                pProperty->NonceTimeout,
                pSessInfo->NonceTimestamp
            );
 
    if ( pPassword )
    {
        AnscFreeMemory(pPassword);
    }

EXIT:

    if ( pAuthInfo )
    {
        HttpAuthInfoRemove(pAuthInfo);
    }

    if ( pHostName )
    {
        AnscFreeMemory(pHostName);
    }

    if ( pUriPath )
    {
        AnscFreeMemory(pUriPath);
    }

    return status;
}


