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

    module:	http_authco_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Authentication Client Object.

        *   HttpAuthcoProcessRequest
        *   HttpAuthcoProcessResponse
        *   HttpAuthcoCalcDigestChallenge
        *   HttpAuthcoCalcDigestHA1
        *   HttpAuthcoCalcDigestHA2
        *   HttpAuthcoGetRequestHostUri
        *   HttpAuthcoGenerateDigestResponse
        *   HttpAuthcoGenerateCNonce

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
        HttpAuthcoProcessRequest
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest
            )

    description:

        This function is called to process the given request
        object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthcoProcessRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_BMO_REQ_OBJECT            pRequest     = (PHTTP_BMO_REQ_OBJECT    )hRequest;
    PHTTP_AUTHO_INFO                pAuthInfo    = NULL;
    PUCHAR                          pChal        = NULL;

    /* first remove Authorization header if any */
    pRequest->DelHeaderField((ANSC_HANDLE)pRequest, HTTP_HEADER_ID_AUTHORIZATION);

    AnscAcquireLock(&pMyObject->AuthInfoListLock);

    pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->FindAuthInfo((ANSC_HANDLE)pMyObject, hRequest);

    if ( pAuthInfo )
    {
        PHTTP_HFO_AUTHORIZATION     pHfoAuthorization   = NULL;
        PHTTP_AUTH_CREDENTIAL       pCredential         = NULL;
        ULONG                       ulNC                = 0;

        if ( pAuthInfo->AuthType == HTTP_AUTH_TYPE_DIGEST )
        {
            UCHAR                   NC[9];
            PUCHAR                  pEntityDigest   = NULL;

            /* get entity digest if 'auth-int' is specified in QOP, not supported yet! */

            ulNC    = pMyObject->GetNC              ((ANSC_HANDLE)pMyObject, hRequest, (ANSC_HANDLE)pAuthInfo);
            pMyObject->GenerateCNonce((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pAuthInfo);

            _ansc_sprintf(NC, "%.8X", ulNC);

            pChal   =
                pMyObject->CalcDigRep
                    (
                        (ANSC_HANDLE)pMyObject,
                        hRequest,
                        pAuthInfo->pUserName,
                        pAuthInfo->pPassword,
                        pAuthInfo->pRealm,
                        pAuthInfo->pDigest->pAlgorithm,
                        pAuthInfo->pDigest->pNonce,
                        pAuthInfo->pDigest->pCNonce,
                        pAuthInfo->pDigest->pQop,
                        (PUCHAR)NC,
                        pEntityDigest,
                        pAuthInfo->pUri
                    );
        }

        if ( TRUE )
        {
            BOOL                    bHfoOK  = TRUE;

            pHfoAuthorization   =
                (PHTTP_HFO_AUTHORIZATION)AnscAllocateMemory(sizeof(HTTP_HFO_AUTHORIZATION));

            if ( !pHfoAuthorization )
            {
                status  = ANSC_STATUS_RESOURCES;
            }
            else
            {
                pHfoAuthorization->HeaderId = HTTP_HEADER_ID_AUTHORIZATION;

                pCredential = &pHfoAuthorization->Credential;

                pCredential->AuthType   = pAuthInfo->AuthType;

                if ( pAuthInfo->AuthType == HTTP_AUTH_TYPE_BASIC )
                {
                    PHTTP_CREDENTIAL_BASIC  pBasicCred  = &pCredential->Credential.Basic;

                    if ( pAuthInfo->pUserName )
                    {
                        if ( AnscSizeOfString(pAuthInfo->pUserName) >= HTTP_MAX_USERNAME_SIZE )
                        {
                            bHfoOK  = FALSE;
                        }
                        else
                        {
                            AnscCopyString(pBasicCred->UserName, pAuthInfo->pUserName);
                        }
                    }

                    if ( pAuthInfo->pPassword )
                    {
                        if ( AnscSizeOfString(pAuthInfo->pPassword) >= HTTP_MAX_PASSWORD_SIZE )
                        {
                            bHfoOK  = FALSE;
                        }
                        else
                        {
                            AnscCopyString(pBasicCred->Password, pAuthInfo->pPassword);
                        }
                    }
                }
                else if ( pAuthInfo->AuthType == HTTP_AUTH_TYPE_DIGEST )
                {
                    PHTTP_CREDENTIAL_DIGEST pDigestCred = &pCredential->Credential.Digest;
                    PUCHAR                  pDigRep     = NULL;

                    pDigRep =
                        pMyObject->GenerateDigestRep
                            (
                                (ANSC_HANDLE)pMyObject,
                                hRequest,
                                (ANSC_HANDLE)pAuthInfo,
                                pChal,
                                ulNC
                            );

                    if ( !pDigRep || AnscSizeOfString(pDigRep) >= HTTP_MAX_USERNAME_SIZE )
                    {
                        bHfoOK  = FALSE;
                    }
                    else
                    {
                        AnscCopyString(pDigestCred->UserName, pDigRep);
                    }

                    if ( pDigRep )
                    {
                        AnscFreeMemory(pDigRep);
                    }
                }

                if ( bHfoOK )
                {
                    status =
                        pRequest->AddHeaderField
                            (
                                (ANSC_HANDLE)pRequest,
                                (ANSC_HANDLE)pHfoAuthorization
                            );
                }
                else
                {
                    AnscFreeMemory(pHfoAuthorization);
                    status = ANSC_STATUS_INTERNAL_ERROR;
                }
            }
        }
    }

    AnscReleaseLock(&pMyObject->AuthInfoListLock);

    if ( pChal )
    {
        AnscFreeMemory(pChal);
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthcoProcessResponse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                ANSC_HANDLE                 hResponse
            )

    description:

        This function is called to process the given response
        object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                ANSC_HANDLE                 hResponse
                Specifies the HTTP response object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthcoProcessResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_BMO_REQ_OBJECT            pRequest     = (PHTTP_BMO_REQ_OBJECT    )hRequest;
    PHTTP_BMO_REP_OBJECT            pResponse    = (PHTTP_BMO_REP_OBJECT    )hResponse;
    ULONG                           ulRepCode;
    PHTTP_HFO_WWW_AUTHENTICATE      pWwwAuth;

    ulRepCode   = pResponse->GetCode((ANSC_HANDLE)pResponse);

    if ( ulRepCode != HTTP_STATUS_UNAUTHORIZED )
    {
        return ANSC_STATUS_SUCCESS;
    }

    /* process WWW-Authenticate header if it exists */
    pWwwAuth    =
        (PHTTP_HFO_WWW_AUTHENTICATE)pResponse->GetHeaderField
            (
                (ANSC_HANDLE)pResponse,
                HTTP_HEADER_ID_WWW_AUTHENTICATE
            );

    if ( !pWwwAuth )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    if ( pWwwAuth->ChallengeCount == 0 )
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }
    else
    {
        PHTTP_AUTH_CHALLENGE        pChallenge  = NULL;
        ULONG                       i;
        BOOL                        bDigestAuth = FALSE;

        for ( i = 0; i < pWwwAuth->ChallengeCount; i ++ )
        {
            if ( pWwwAuth->ChallengeArray[i].AuthType == HTTP_AUTH_TYPE_BASIC )
            {
                pChallenge  = &pWwwAuth->ChallengeArray[i];
            }
            else if ( pWwwAuth->ChallengeArray[i].AuthType == HTTP_AUTH_TYPE_DIGEST )
            {
                pChallenge  = &pWwwAuth->ChallengeArray[i];
                bDigestAuth = TRUE;
                break;
            }
        }

        if ( !pChallenge )
        {
            return ANSC_STATUS_NOT_SUPPORTED;
        }

        status =
            pMyObject->AddAuthInfo
                (
                    (ANSC_HANDLE)pMyObject,
                    hRequest,
                    (ANSC_HANDLE)pChallenge
                );

        if ( status == ANSC_STATUS_SUCCESS )
        {
            /* client should send the request again with appropriate authorization info */
            status  = ANSC_STATUS_DO_IT_AGAIN;
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpAuthcoGenerateDigestResponse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                ANSC_HANDLE                 hAuthInfo,
                PUCHAR                      pRequestDigest,
                ULONG                       ulNC
            )

    description:

        This function is called to generate digest response.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                ANSC_HANDLE                 hAuthInfo
                Specifies the authentication info.

                PUCHAR                      pRequestDigest
                Specifies the HTTP request digest.

                ULONG                       ulNC
                Nonce count to be used.

    return:     digest response.

**********************************************************************/

PUCHAR
HttpAuthcoGenerateDigestResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hAuthInfo,
        PUCHAR                      pRequestDigest,
        ULONG                       ulNC
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO               pAuthInfo    = (PHTTP_AUTHO_INFO       )hAuthInfo;
    PUCHAR                          pDigRep      = NULL;
    ULONG                           ulDigRepSize = 0;
    PUCHAR                          pBuf         = NULL;
    ULONG                           ulBufLen     = 0;
    PUCHAR                          pHostName    = NULL;
    PUCHAR                          pUriPath     = NULL;
    USHORT                          HostPort     = 0;

    if ( !pAuthInfo->pDigest || !pRequestDigest )
    {
        return NULL;
    }

    status = pMyObject->GetRequestHostUri((ANSC_HANDLE)pMyObject, hRequest, &pHostName, &HostPort, &pUriPath);
    if ( status != ANSC_STATUS_SUCCESS )
    {
        return NULL;
    }

    ulDigRepSize    = AnscSizeOfString(HTTP_AUTH_NAME_realm) + 1 + AnscSizeOfString(pAuthInfo->pRealm) + 2;
    ulDigRepSize   += 2;

    ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_username) + 1 + AnscSizeOfString(pAuthInfo->pUserName) + 2;
    ulDigRepSize   += 2;

    if ( pAuthInfo->pDigest->pQop )
    {
        ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_qop) + 1 + AnscSizeOfString(pAuthInfo->pDigest->pQop) + 2;
        ulDigRepSize   += 2;
    }

    if ( pAuthInfo->pDigest->pAlgorithm )
    {
        ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_algorithm) + 1 + AnscSizeOfString(pAuthInfo->pDigest->pAlgorithm) + 2;
        ulDigRepSize   += 2;
    }
    else
    {
        ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_algorithm) + 1 + AnscSizeOfString(HTTP_AUTH_NAME_md5) + 2;
        ulDigRepSize   += 2;
    }

    ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_digest_uri) + 1 + AnscSizeOfString(pUriPath) + 2;
    ulDigRepSize   += 2;

    if ( pAuthInfo->pDigest->pNonce )
    {
        ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_nonce) + 1 + AnscSizeOfString(pAuthInfo->pDigest->pNonce) + 2;
        ulDigRepSize   += 2;
    }

    ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_cnonce) + 1 + AnscSizeOfString(pAuthInfo->pDigest->pCNonce) + 2;
    ulDigRepSize   += 2;

    ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_nc) + 1 + 8;
    ulDigRepSize   += 2;

    if ( pAuthInfo->pDigest->pOpaque )
    {
        ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_opaque) + 1 + AnscSizeOfString(pAuthInfo->pDigest->pOpaque) + 2;
        ulDigRepSize   += 2;
    }

    ulDigRepSize   += AnscSizeOfString(HTTP_AUTH_NAME_response) + 1 + AnscSizeOfString(pRequestDigest) + 2;
    ulDigRepSize   += 2;

    ulDigRepSize   += 16;

    pDigRep     = (PUCHAR)AnscAllocateMemory(ulDigRepSize);

    if ( !pDigRep )
    {
        goto EXIT;
    }

    _ansc_sprintf
        (
            pDigRep,
            "%s=\"%s\", %s=\"%s\"",
            HTTP_AUTH_NAME_realm,
            pAuthInfo->pRealm,
            HTTP_AUTH_NAME_username,
            pAuthInfo->pUserName
        );

    if ( pAuthInfo->pDigest->pQop )
    {
        _ansc_sprintf
            (
                pDigRep + AnscSizeOfString(pDigRep),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_qop,
                pAuthInfo->pDigest->pQop
            );
    }

    if ( pAuthInfo->pDigest->pAlgorithm )
    {
        _ansc_sprintf
            (
                pDigRep + AnscSizeOfString(pDigRep),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_algorithm,
                pAuthInfo->pDigest->pAlgorithm
            );
    }
    else
    {
        _ansc_sprintf
            (
                pDigRep + AnscSizeOfString(pDigRep),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_algorithm,
                HTTP_AUTH_NAME_md5
            );
    }

    _ansc_sprintf
        (
            pDigRep + AnscSizeOfString(pDigRep),
            ", %s=\"%s\"",
            HTTP_AUTH_NAME_digest_uri,
            pUriPath
        );

    if ( pAuthInfo->pDigest->pNonce )
    {
        _ansc_sprintf
            (
                pDigRep + AnscSizeOfString(pDigRep),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_nonce,
                pAuthInfo->pDigest->pNonce
            );
    }

    _ansc_sprintf
        (
            pDigRep + AnscSizeOfString(pDigRep),
            ", %s=\"%s\"",
            HTTP_AUTH_NAME_cnonce,
            pAuthInfo->pDigest->pCNonce
        );

    _ansc_sprintf
        (
            pDigRep + AnscSizeOfString(pDigRep),
            ", %s=%.8X",
            HTTP_AUTH_NAME_nc,
            ulNC
        );

    if ( pAuthInfo->pDigest->pOpaque )
    {
        _ansc_sprintf
            (
                pDigRep + AnscSizeOfString(pDigRep),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_opaque,
                pAuthInfo->pDigest->pOpaque
            );
    }

    _ansc_sprintf
        (
            pDigRep + AnscSizeOfString(pDigRep),
            ", %s=\"%s\"",
            HTTP_AUTH_NAME_response,
            pRequestDigest
        );

EXIT:

    if ( pHostName )
    {
        AnscFreeMemory(pHostName);
    }

    if ( pUriPath )
    {
        AnscFreeMemory(pUriPath);
    }

    return pDigRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthcoGenerateCNonce
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hAuthInfo
            )

    description:

        This function is called to generate CNonce.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hAuthInfo
                Specifies the authentication info.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthcoGenerateCNonce
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAuthInfo
    )
{
    PHTTP_AUTH_CLIENT_OBJECT        pMyObject    = (PHTTP_AUTH_CLIENT_OBJECT)hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo    = (PHTTP_AUTHO_INFO       )hAuthInfo;
    PUCHAR                          pBuf         = NULL;
    ULONG                           ulBufLen     = 0;
    ANSC_CRYPTO_HASH                MD5Hash;
    PUCHAR                          pCNonce      = pAuthInfo->pDigest->pCNonce;

    if ( !pCNonce )
    {
        pCNonce = (PUCHAR)AnscAllocateMemory(ANSC_MD5_OUTPUT_SIZE * 2 + 1);
        pAuthInfo->pDigest->pCNonce = pCNonce;
    }

    if ( !pCNonce )
    {
        return ANSC_STATUS_RESOURCES;
    }

    ulBufLen    =
        (pAuthInfo->pDigest->pNonce  ? AnscSizeOfString(pAuthInfo->pDigest->pNonce ) : 0) +
        (pAuthInfo->pDigest->pOpaque ? AnscSizeOfString(pAuthInfo->pDigest->pOpaque) : 0) +
        8 + 16;

    pBuf    = (PUCHAR)AnscAllocateMemory(ulBufLen);

    if ( !pBuf )
    {
        return ANSC_STATUS_RESOURCES;
    }

    if ( pAuthInfo->pDigest->pOpaque )
    {
        _ansc_sprintf
            (
                pBuf,
                "%s%s%.8X",
                pAuthInfo->pDigest->pNonce,
                pAuthInfo->pDigest->pOpaque,
                AnscGetTickInMilliSeconds()
            );
    }
    else
    {
        _ansc_sprintf
            (
                pBuf,
                "%s%.8X",
                pAuthInfo->pDigest->pNonce,
                AnscGetTickInMilliSeconds()
            );
    }

    AnscCryptoMd5Digest
        (
            pBuf,
            AnscSizeOfString(pBuf),
            &MD5Hash
        );

    HttpAuthoBinToHex(MD5Hash.Value, ANSC_MD5_OUTPUT_SIZE, pCNonce);

    AnscFreeMemory(pBuf);

    return ANSC_STATUS_SUCCESS;
}


