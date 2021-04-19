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

    module:	http_autho_process.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Http Authentication Object.

        *   HttpAuthoCalcDigResponse
        *   HttpAuthoCalcDigestHA1
        *   HttpAuthoCalcDigestHA2
        *   HttpAuthoGetRequestHostUri
        *   HttpAuthoGetDigestAuthInfo

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


#include "http_autho_global.h"
#include "ansc_crypto_internal_api.h"
#include "safec_lib_common.h"

static const char                   g_httpAuthMsgBody[] =
"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
"<html><head>"
"<title>401 Unauthorized</title>"
"</head><body>"
"<h1>Authorization Required</h1>"
"<p>"
"This server could not verify that you "
"are authorized to access the document requested.  "
"Either you supplied the wrong credentials (e.g., bad password), or your "
"browser doesn't understand how to supply the credentials required"
"</p>"
"<hr>"
"</body>"
"</html>"
;


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpAuthoCalcDigResponse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                PUCHAR                      pUserName,
                PUCHAR                      pPassword,
                PUCHAR                      pRealm,
                PUCHAR                      pAlgorithm,
                PUCHAR                      pNonce,
                PUCHAR                      pCNonce,
                PUCHAR                      pQop,
                PUCHAR                      pNC,
                PUCHAR                      pEntityDigest
            )

    description:

        This function is called to calculate digest response.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                PUCHAR                      pUserName
                User name.

                PUCHAR                      pPassword
                User's password.

                PUCHAR                      pRealm
                Realm name.

                PUCHAR                      pAlgorithm
                The name of algorithm.

                PUCHAR                      pNonce
                The Nonce value.

                PUCHAR                      pCNonce
                CNonce value.

                PUCHAR                      pQop
                QOP value.

                PUCHAR                      pNC
                Nonce-Count value.

                PUCHAR                      pEntityDigest
                Specifies the digest of entity body. It must be
                specified if algorithm is specified as 'md5-sess'.

    return:     digest response.

**********************************************************************/

PUCHAR
HttpAuthoCalcDigResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      pUserName,
        PUCHAR                      pPassword,
        PUCHAR                      pRealm,
        PUCHAR                      pAlgorithm,
        PUCHAR                      pNonce,
        PUCHAR                      pCNonce,
        PUCHAR                      pQop,
        PUCHAR                      pNC,
        PUCHAR                      pEntityDigest,
        PUCHAR                      pDigestUri
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT  )hThisObject;
    PCHAR                           pBuf         = NULL;
    ULONG                           ulSize       = 0;
    PUCHAR                          pDigRep      = NULL;
    ANSC_CRYPTO_HASH                MD5Hash;
    PUCHAR                          pHA1, pHA2;
    errno_t                         rc           = -1;

    pDigRep = (PUCHAR)AnscAllocateMemory(ANSC_MD5_OUTPUT_SIZE * 2 + 1);
    if ( !pDigRep )
    {
        return NULL;
    }

    pHA1    = (PUCHAR)AnscAllocateMemory(ANSC_MD5_OUTPUT_SIZE * 2 + 1);
    pHA2    = (PUCHAR)AnscAllocateMemory(ANSC_MD5_OUTPUT_SIZE * 2 + 1);

    if ( !pHA1 || !pHA2 )
    {
        goto EXIT2;
    }

    status =
        pMyObject->CalcDigestHA1
            (
                (ANSC_HANDLE)pMyObject,
                hRequest,
                pUserName,
                pPassword,
                pRealm,
                pAlgorithm,
                pNonce,
                pCNonce,
                pHA1
            );

    if ( status != ANSC_STATUS_SUCCESS )
    {
        goto EXIT2;
    }

#if 0
    AnscTrace("pHA1 = %s\n", pHA1);
#endif

    status =
        pMyObject->CalcDigestHA2
            (
                (ANSC_HANDLE)pMyObject,
                hRequest,
                pQop,
                pEntityDigest,
                pDigestUri,
                pHA2
            );

    if ( status != ANSC_STATUS_SUCCESS )
    {
        goto EXIT2;
    }

#if 0
    AnscTrace("pHA2 = %s\n", pHA2);
#endif

    ulSize  = ANSC_MD5_OUTPUT_SIZE * 2 + 1 + AnscSizeOfString((const char*)pNonce) + 1;
    if ( pQop )
    {
        ulSize  +=
            8 + 1 +                                             /* Nonce Count */
            AnscSizeOfString((const char*)pCNonce) + 1 +
            AnscSizeOfString((const char*)pQop)   + 1;
    }
    ulSize  += ANSC_MD5_OUTPUT_SIZE * 2;

    pBuf    = (char *)AnscAllocateMemory(ulSize + 16);

    if ( !pBuf )
    {
        goto EXIT2;
    }

    if ( pQop )
    {
        rc = sprintf_s(pBuf, (ulSize + 16), "%s:%s:%s:%s:%s:%s", pHA1, pNonce, pNC,  pCNonce, pQop, pHA2);
    }
    else
    {
        rc = sprintf_s(pBuf, (ulSize + 16), "%s:%s:%s", pHA1, pNonce, pHA2);
    }
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }
    AnscCryptoMd5Digest((PVOID)pBuf, AnscSizeOfString((const char*)pBuf), &MD5Hash);
    AnscFreeMemory(pBuf);

    HttpAuthoBinToHex(MD5Hash.Value, ANSC_MD5_OUTPUT_SIZE, pDigRep);

#if 0
    AnscTrace("pDigRep = %s\n", pDigRep);
#endif

    goto EXIT1;

EXIT2:

    if ( pDigRep )
    {
        AnscFreeMemory(pDigRep);
        pDigRep = NULL;
    }

EXIT1:
    if ( pHA1 )
    {
        AnscFreeMemory(pHA1);
    }

    if ( pHA2)
    {
        AnscFreeMemory(pHA2);
    }

    return pDigRep;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoCalcDigestHA1
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                PUCHAR                      pUserName,
                PUCHAR                      pPassword,
                PUCHAR                      pRealm,
                PUCHAR                      pAlgorithm,
                PUCHAR                      pNonce,
                PUCHAR                      pCNonce,
                PUCHAR                      pHA1
            )

    description:

        This function is called to calculate HA1.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                PUCHAR                      pUserName
                User name.

                PUCHAR                      pPassword
                User's password.

                PUCHAR                      pRealm
                Realm name.

                PUCHAR                      pAlgorithm
                The name of algorithm.

                PUCHAR                      pNonce
                The Nonce value.

                PUCHAR                      pCNonce
                CNonce value.

                PUCHAR                      pHA1
                On successful return, it contains HA1 value.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoCalcDigestHA1
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      pUserName,
        PUCHAR                      pPassword,
        PUCHAR                      pRealm,
        PUCHAR                      pAlgorithm,
        PUCHAR                      pNonce,
        PUCHAR                      pCNonce,
        PUCHAR                      pHA1
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hRequest);
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PCHAR                           pBuf         = NULL;
    ULONG                           ulSize       = 0;
    ANSC_CRYPTO_HASH                MD5Hash;
    errno_t                         rc           = -1;

    ulSize  =
        AnscSizeOfString((const char*)pUserName) + 1 +
        AnscSizeOfString((const char*)pRealm)    + 1 +
        (pPassword?AnscSizeOfString((const char*)pPassword):0);

    if ( pAlgorithm && strcasecmp((char*)pAlgorithm, HTTP_AUTH_NAME_md5_sess) == 0 )
    {
        if ( ulSize < ANSC_MD5_OUTPUT_SIZE * 2 )
        {
            ulSize  = ANSC_MD5_OUTPUT_SIZE * 2;
        }

        ulSize    += 1 + AnscSizeOfString((const char*)pNonce) + 1;
        if ( pCNonce )
        {
            ulSize  += AnscSizeOfString((const char*)pCNonce);
        }
    }

    pBuf  = (char *)AnscAllocateMemory(ulSize + 16);
    if ( !pBuf )
    {
        status = ANSC_STATUS_RESOURCES;
    }
    /*
     * The following code which complies with RFC 2617 doesn't work with IE and Mozilla for some reason
     * when algorithm "MD5-sess" is specified. The reason is these browsers give the ASCII format (32 bytes) of
     * hashed value over user:realm:password instead of hex value (16 bytes). My guess is all web server will
     * follow these rules for compatibility reasons. Hopefully all browsers and HTTP clients which support
     * MD5-sess will follow this rule.
     */
    /*
    else
    {
        _ansc_sprintf(pBuf, "%s:%s:%s", pUserName, pRealm, pPassword);

        AnscCryptoMd5Digest((PVOID)pBuf, AnscSizeOfString(pBuf), &MD5Hash);

        if ( pAlgorithm && strcasecmp(pAlgorithm, HTTP_AUTH_NAME_md5_sess) == 0 )
        {
            ULONG                   ulMsgSize = ANSC_MD5_OUTPUT_SIZE;

            AnscCopyMemory(pBuf, MD5Hash.Value, ANSC_MD5_OUTPUT_SIZE);

            _ansc_sprintf(pBuf + ANSC_MD5_OUTPUT_SIZE, ":%s:%s", pNonce, pCNonce);

            ulMsgSize += AnscSizeOfString(pBuf + ANSC_MD5_OUTPUT_SIZE);

            AnscCryptoMd5Digest((PVOID)pBuf, ulMsgSize, &MD5Hash);
        }

        AnscFreeMemory(pBuf);

        HttpAuthoBinToHex(MD5Hash.Value, ANSC_MD5_OUTPUT_SIZE, pHA1);
    }
    */
    else
    {
        rc = sprintf_s(pBuf, (ulSize + 16), "%s:%s:%s", pUserName, pRealm, pPassword?pPassword:(PUCHAR)"");
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        AnscCryptoMd5Digest((PVOID)pBuf, AnscSizeOfString((const char*)pBuf), &MD5Hash);
        HttpAuthoBinToHex(MD5Hash.Value, ANSC_MD5_OUTPUT_SIZE, pHA1);

        if ( pAlgorithm && strcasecmp((char*)pAlgorithm, HTTP_AUTH_NAME_md5_sess) == 0 )
        {
            rc = sprintf_s(pBuf, (ulSize + 16), "%s:%s:%s", pHA1, pNonce, pCNonce);
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }

            AnscCryptoMd5Digest((PVOID)pBuf, AnscSizeOfString((const char*)pBuf), &MD5Hash);
            HttpAuthoBinToHex(MD5Hash.Value, ANSC_MD5_OUTPUT_SIZE, pHA1);
        }

        AnscFreeMemory(pBuf);
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoCalcDigestHA2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                PUCHAR                      pQop,
                PUCHAR                      pEntityDigest,
                PUCHAR                      pDigestUri,
                PUCHAR                      pHA2
            )

    description:

        This function is called to calculate HA2.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                PUCHAR                      pQop
                QOP value.

                PUCHAR                      pEntityDigest
                Specifies the digest of entity body. It must be
                specified if algorithm is specified as 'md5-sess'.

                PUCHAR                      pDigestUri
                Specifies the digest URI.

                PUCHAR                      pHA2
                On successful return, it contains HA2 value.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoCalcDigestHA2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      pQop,
        PUCHAR                      pEntityDigest,
        PUCHAR                      pDigestUri,
        PUCHAR                      pHA2
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pRequest     = (PHTTP_BMO_REQ_OBJECT    )hRequest;
    PCHAR                           pMethodName  = (PCHAR                  )pRequest->GetMethodName((ANSC_HANDLE)pRequest);
    PCHAR                           pBuf         = NULL;
    ULONG                           ulSize       = 0;
    BOOL                            bAuthInt     = FALSE;
    PUCHAR                          pUriPath     = NULL;
    ANSC_CRYPTO_HASH                MD5Hash = {0}; /*RDKB-6235, CID-24777, initialize before use*/
    errno_t                         rc           = -1;

    if ( !pMethodName || *pMethodName == '\0' )
    {
        ULONG                       ulMethodCode  = pRequest->GetMethod((ANSC_HANDLE)pRequest);

        switch ( ulMethodCode )
        {

            case    HTTP_METHOD_CODE_OPTIONS:

                    pMethodName = HTTP_METHOD_NAME_OPTIONS;

                    break;

            case    HTTP_METHOD_CODE_GET:

                    pMethodName = HTTP_METHOD_NAME_GET;

                    break;

            case    HTTP_METHOD_CODE_HEAD:

                    pMethodName = HTTP_METHOD_NAME_HEAD;

                    break;

            case    HTTP_METHOD_CODE_POST:

                    pMethodName = HTTP_METHOD_NAME_POST;

                    break;

            case    HTTP_METHOD_CODE_PUT:

                    pMethodName = HTTP_METHOD_NAME_PUT;

                    break;

            case    HTTP_METHOD_CODE_DELETE:

                    pMethodName = HTTP_METHOD_NAME_DELETE;

                    break;

            case    HTTP_METHOD_CODE_TRACE:

                    pMethodName = HTTP_METHOD_NAME_TRACE;

                    break;

            case    HTTP_METHOD_CODE_CONNECT:

                    pMethodName = HTTP_METHOD_NAME_CONNECT;

                    break;

            case    HTTP_METHOD_CODE_NOTIFY:

                    pMethodName = HTTP_METHOD_NAME_NOTIFY;

                    break;

            case    HTTP_METHOD_CODE_SEARCH:

                    pMethodName = HTTP_METHOD_NAME_SEARCH;

                    break;

            case    HTTP_METHOD_CODE_M_SEARCH:

                    pMethodName = HTTP_METHOD_NAME_M_SEARCH;

                    break;

            case    HTTP_METHOD_CODE_M_POST:

                    pMethodName = HTTP_METHOD_NAME_M_POST;

                    break;

            case    HTTP_METHOD_CODE_SUBSCRIBE:

                    pMethodName = HTTP_METHOD_NAME_SUBSCRIBE;

                    break;

            case    HTTP_METHOD_CODE_UNSUBSCRIBE:

                    pMethodName = HTTP_METHOD_NAME_UNSUBSCRIBE;

                    break;
        }
    }

    if ( !pMethodName )
    {
        status = ANSC_STATUS_INTERNAL_ERROR;
        goto EXIT;
    }

    /*
    status = pMyObject->GetRequestHostUri((ANSC_HANDLE)pMyObject, hRequest, &pHostName, &HostPort, &pUriPath);
    */

    pUriPath = pDigestUri;

    ulSize  = AnscSizeOfString((const char*)pMethodName) + 1 + AnscSizeOfString((const char*)pUriPath);

    if ( pQop && strcasecmp((char*)pQop, HTTP_AUTH_NAME_auth_int) == 0 )
    {
        bAuthInt    = TRUE;
        ulSize     += 1 + AnscSizeOfString((const char*)pEntityDigest);
    }

    pBuf    = (char *)AnscAllocateMemory(ulSize + 16);
    if ( !pBuf )
    {
        status = ANSC_STATUS_RESOURCES;
    }
    else
    {
        if(bAuthInt)
        {
            rc = sprintf_s(pBuf, (ulSize + 16), "%s:%s:%s", pMethodName, pUriPath, pEntityDigest);
        }
        else
        {
            rc = sprintf_s(pBuf, (ulSize + 16), "%s:%s", pMethodName, pUriPath);
        }
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        if ( status == ANSC_STATUS_SUCCESS )
        {
        	AnscCryptoMd5Digest((PVOID)pBuf, AnscSizeOfString((const char*)pBuf), &MD5Hash);
        }

        AnscFreeMemory(pBuf);
    }

    HttpAuthoBinToHex(MD5Hash.Value, ANSC_MD5_OUTPUT_SIZE, pHA2);

EXIT:
    /*
    if ( pUriPath )
    {
        AnscFreeMemory(pUriPath);
    }


    if ( pHostName )
    {
        AnscFreeMemory(pHostName);
    }
    */

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoGetRequestHostUri
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                PUCHAR                      *ppHostName,
                PUSHORT                     pHostPort,
                PUCHAR                      *ppUriPath
            )

    description:

        This function is called to get request host, port and
        URI path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                Specifies the HTTP request object.

                PUCHAR                      *ppHostName
                On return, it contains host name.

                PUSHORT                     pHostPort
                On return, it contains host port.

                PUCHAR                      *ppUriPath
                On return, it contains URI path.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoGetRequestHostUri
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      *ppHostName,
        PUSHORT                     pHostPort,
        PUCHAR                      *ppUriPath
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REQ_OBJECT            pRequest     = (PHTTP_BMO_REQ_OBJECT)hRequest;
    PHTTP_HFO_HOST                  pHfoHost     = NULL;
    PHTTP_REQUEST_INFO              pReqInfo     = NULL;

    *ppHostName = NULL;
    *ppUriPath  = NULL;
    *pHostPort  = 0;

    pReqInfo    = (PHTTP_REQUEST_INFO)pRequest->GetReqInfo((ANSC_HANDLE)pRequest);
    pHfoHost    = (PHTTP_HFO_HOST    )pRequest->GetHeaderField((ANSC_HANDLE)pRequest, HTTP_HEADER_ID_HOST);

    if ( pHfoHost && pHfoHost->HostName[0] != '\0' )
    {
        *ppHostName      = (PUCHAR)AnscCloneString(pHfoHost->HostName);
        *pHostPort      = pHfoHost->HostPort;
    }

    if ( pReqInfo )
    {
        PHTTP_REQUEST_URI           pReqUri     = &pReqInfo->RequestUri;
        ULONG                       ulPathLen   = 0;
        ULONG                       i;
        PUCHAR                      pPath       = NULL;
        errno_t                     rc          = -1;

        if ( *ppHostName == NULL )
        {
            *ppHostName    = (PUCHAR)AnscCloneString(pReqUri->HostName);
        }

        if ( *pHostPort == 0 )
        {
            *pHostPort     = pReqUri->HostPort;
        }
        for ( i = 0; i < pReqUri->PathLevel; i ++ )
        {
            ulPathLen   += AnscSizeOfString(pReqUri->PathArray[i]) + 1;
        }

        ulPathLen   += 16;

        pPath   = (PUCHAR)AnscAllocateMemory(ulPathLen);

        if ( !pPath )
        {
            status = ANSC_STATUS_RESOURCES;
        }
        else
        {
            if (
                   (pReqUri->PathLevel == 1 && strcmp(pReqUri->PathArray[0], "/") == 0 ) ||
                   pReqUri->PathLevel == 0
               )
            {
                pPath[0]    = '/';
                pPath[1]    = 0;
            }
            else
            {
                for ( i = 1; i < pReqUri->PathLevel; i ++ )
                {
                    rc = strcpy_s((char*)pPath + AnscSizeOfString((const char*)pPath), (ulPathLen - AnscSizeOfString((const char*)pPath)), "/");
                    ERR_CHK(rc);

                    if ( strcmp(pReqUri->PathArray[i], "/") == 0 )
                    {
                        continue;
                    }

                    rc = strcpy_s((char*)(pPath + AnscSizeOfString((const char*)pPath)), (ulPathLen - AnscSizeOfString((const char*)pPath)), (char*)pReqUri->PathArray[i]);
                    ERR_CHK(rc);
                }
            }

            *ppUriPath  = pPath;
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpAuthoGetAuthInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest
            );

    description:

        This function is called to get authentication info from
        the given request object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                HTTP request object.

                return:     status of operation.

**********************************************************************/

ANSC_HANDLE
HttpAuthoGetAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    )
{
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT   )hThisObject;
    PHTTP_BMO_REQ_OBJECT            pRequest     = (PHTTP_BMO_REQ_OBJECT)hRequest;
    PHTTP_HFO_AUTHORIZATION         pHfoAuth     = (PHTTP_HFO_AUTHORIZATION)NULL;
    PHTTP_AUTHO_INFO                pAuthInfo    = (PHTTP_AUTHO_INFO       )NULL;
    PHTTP_AUTH_CREDENTIAL           pCredential  = NULL;
    BOOL                            bDigestAuth  = FALSE;

    pHfoAuth    =
        (PHTTP_HFO_AUTHORIZATION)pRequest->GetHeaderField
            (
                (ANSC_HANDLE)pRequest,
                HTTP_HEADER_ID_AUTHORIZATION
            );

    if ( !pHfoAuth )
    {
        return NULL;
    }

    pCredential = &pHfoAuth->Credential;

    switch ( pCredential->AuthType )
    {
        case    HTTP_AUTH_TYPE_BASIC:

                bDigestAuth = FALSE;

                break;

        case    HTTP_AUTH_TYPE_DIGEST:

                bDigestAuth = TRUE;

                break;

        default:

                return NULL;    /* scheme not supported */

                break;
    }

    HttpAuthInfoCreate(pAuthInfo);

    if ( !pAuthInfo )
    {
        return NULL;
    }

    if ( bDigestAuth )
    {
        HttpAuthDigestInfoCreate(pAuthInfo->pDigest);

        if ( !pAuthInfo->pDigest )
        {
            HttpAuthInfoRemove(pAuthInfo);
            return NULL;
        }
    }

    pAuthInfo->AuthType     = pCredential->AuthType;

    if ( !bDigestAuth )
    {
        PHTTP_CREDENTIAL_BASIC      pBasicCred  = &pCredential->Credential.Basic;

        pAuthInfo->pUserName    = (PUCHAR)AnscCloneString(pBasicCred->UserName);
        pAuthInfo->pPassword    = (PUCHAR)AnscCloneString(pBasicCred->Password);
    }
    else
    {
        PHTTP_CREDENTIAL_DIGEST     pDigestCred = &pCredential->Credential.Digest;
        PUCHAR                      pChal       = (PUCHAR)pDigestCred->UserName;

        pMyObject->GetDigestAuthInfo
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pAuthInfo,
                pChal
            );
    }

    return pAuthInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoGetDigestAuthInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hAuthInfo,
                PUCHAR                      pDigData
            )

    description:

        This function is called to extract digest authentication
        info from the given digest data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hAuthInfo
                On successful return, it contains the digest
                authentication info.

                PUCHAR                      pDigData
                Specifies the digest data to be parsed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoGetDigestAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAuthInfo,
        PUCHAR                      pDigData
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTHO_INFO                pAuthInfo    = (PHTTP_AUTHO_INFO    )hAuthInfo;
    PUCHAR                          pChal        = pDigData;
    PUCHAR                          pNext, pValue, pValueLast;
    ULONG                           ulLen, ulNameLen;

    while ( pChal )
    {
        while ( pChal && (*pChal == ' ') ) /*RDKB-6235, CID-24093, nul check before use*/
        {
            pChal ++;
        }

        if ( !pChal )
        {
            break;
        }

        pNext       = (PUCHAR)_ansc_strchr((const char*)pChal, ',');
        ulLen       = pNext ? (ULONG)(pNext - pChal) : AnscSizeOfString((const char*)pChal);

        pValue      = (PUCHAR)_ansc_memchr(pChal, '=', ulLen);
        ulNameLen   = pValue ? (ULONG)(pValue - pChal) : AnscSizeOfString((const char*)pChal);
        pValue ++;
        pValueLast  = pNext ? pNext - 1 : pChal + AnscSizeOfString((const char*)pChal) - 1;

        if ( pValueLast >= pValue && *pValue == '"' )
        {
            pValue ++;

            if ( *pValueLast == '"' )
            {
                pValueLast --;
            }
        }

        if ( pValueLast < pValue )
        {
            pChal   = pNext ? pNext + 1 : NULL;
            continue;
        }

        if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_realm) &&
             AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_realm, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pRealm, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_domain) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_domain, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pDomain, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_nonce) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_nonce, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pNonce, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_opaque) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_opaque, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pOpaque, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_algorithm) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_algorithm, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pAlgorithm, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_qop) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_qop, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pQop, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_response) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_response, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pResponse, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_digest_uri) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_digest_uri, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pDigestUri, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_nc) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_nc, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pNonceCount, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_cnonce) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_cnonce, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pDigest->pCNonce, pValue, pValueLast - pValue + 1);
        }
        else if ( ulNameLen == AnscSizeOfString(HTTP_AUTH_NAME_username) &&
                  AnscEqualString2((char*)pChal, HTTP_AUTH_NAME_username, ulNameLen, FALSE) )
        {
            HttpAuthCloneMemory(pAuthInfo->pUserName, pValue, pValueLast - pValue + 1);
        }

        pChal   = pNext ? pNext + 1 : NULL;
    }

    if ( pAuthInfo->pDigest->pDomain )
    {
        PUCHAR                  pDomain = pAuthInfo->pDigest->pDomain;
        PUCHAR                  pNext;
        ULONG                   ulCount = 0;

        pNext   = (PUCHAR)_ansc_strchr((const char*)pDomain, ' ');

        if ( pNext )
        {
            while ( pDomain )
            {
                while (*pDomain == ' ')
                {
                    pDomain ++;
                }

                if ( *pDomain == 0 )
                {
                    break;
                }

                pNext   = (PUCHAR)_ansc_strchr((const char*)pDomain, ' ');

                ulCount ++;

                pDomain = pNext ? pNext + 1 : NULL;
            }

            if ( ulCount > 1 )
            {
                pAuthInfo->pDigest->pDomainURIs =
                    (PHTTP_AUTHO_DIGEST_DOMAIN_URIS)AnscAllocateMemory(sizeof(HTTP_AUTHO_DIGEST_DOMAIN_URIS));

                if ( pAuthInfo->pDigest->pDomainURIs )
                {
                    PUCHAR      *pURIs  = (PUCHAR *)AnscAllocateMemory(sizeof(PUCHAR) * ulCount);
                    ULONG       ulLen;

                    if ( !pURIs )
                    {
                        AnscFreeMemory(pAuthInfo->pDigest->pDomainURIs);
                        pAuthInfo->pDigest->pDomainURIs = NULL;
                    }
                    else
                    {
                        pAuthInfo->pDigest->pDomainURIs->NumURIs    = ulCount;
                        pAuthInfo->pDigest->pDomainURIs->pURIs      = pURIs;
                    }

                    pDomain = pAuthInfo->pDigest->pDomain;
                    ulCount = 0;

                    while ( pDomain )
                    {
                        while (*pDomain == ' ')
                        {
                            pDomain ++;
                        }

                        if ( *pDomain == 0 )
                        {
                            break;
                        }

                        pNext   = (PUCHAR)_ansc_strchr((const char*)pDomain, ' ');
                        ulLen   = pNext ? (ULONG)(pNext - pDomain) : AnscSizeOfString((const char*)pDomain);
                        /*CID: 63078 Dereference after null check*/
                        if (pURIs)
                        {
                          HttpAuthCloneMemory(pURIs[ulCount], pDomain, ulLen);
                          ulCount ++;
                        }

                        pDomain = pNext ? pNext + 1 : NULL;
                    }
                }
            }
        }
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpAuthoGenBasicChallenge
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pRealm
            )

    description:

        This function is called to generate basic challenge value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pRealm
                Authentication realm.

    return:     status of operation.

**********************************************************************/

PUCHAR
HttpAuthoGenBasicChallenge
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRealm
    )
{
    UNREFERENCED_PARAMETER(hThisObject);

    return (PUCHAR)AnscCloneString((char*)pRealm);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpAuthoGenDigestChallenge
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pRealm,
                PUCHAR                      pDomain,
                PUCHAR                      pAlgorithm,
                PUCHAR                      pQop,
                PUCHAR                      pNonce,
                BOOL                        bNonceExpired
            )

    description:

        This function is called to generate digest challenge value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pRealm
                Authentication realm.

                PUCHAR                      pDomain
                Authentication domain.

                PUCHAR                      pAlgorithm
                Algorithm name, be it either "MD5" or "MD5-sess" as
                RFC 2167 specifies.

                PUCHAR                      pQop
                Qop value, be it either NULL or "auth". "auth-int"
                is not supported in this version.

                PUCHAR                      pNonce
                Nonce value to be used.

                BOOL                        bNonceExpired
                If true, "stale=true" will be present in the
                challenge.

    return:     status of operation.

**********************************************************************/

PUCHAR
HttpAuthoGenDigestChallenge
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRealm,
        PUCHAR                      pDomain,
        PUCHAR                      pAlgorithm,
        PUCHAR                      pQop,
        PUCHAR                      pNonce,
        BOOL                        bNonceExpired
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PUCHAR                          pDigChal     = NULL;
    ULONG                           ulSize       = 0;
    errno_t                         rc           = -1;

    ulSize  = AnscSizeOfString(HTTP_AUTH_NAME_realm) + 1 + AnscSizeOfString((const char*)pRealm) + 2;
    ulSize += 2;

    ulSize += AnscSizeOfString(HTTP_AUTH_NAME_nonce) + 1 + AnscSizeOfString((const char*)pNonce) + 2;
    ulSize += 2;

    ulSize += AnscSizeOfString(HTTP_AUTH_NAME_algorithm) + 1 + AnscSizeOfString((const char*)pAlgorithm) + 2;
    ulSize += 2;

    if ( pDomain )
    {
        ulSize += AnscSizeOfString(HTTP_AUTH_NAME_domain) + 1 + AnscSizeOfString((const char*)pDomain) + 2;
        ulSize += 2;
    }

    if ( pQop )
    {
        ulSize += AnscSizeOfString(HTTP_AUTH_NAME_qop) + 1 + AnscSizeOfString((const char*)pQop) + 2;
        ulSize += 2;
    }

    if ( bNonceExpired )
    {
        ulSize += AnscSizeOfString(HTTP_AUTH_NAME_stale) + 1 + AnscSizeOfString(HTTP_AUTH_NAME_true) + 2;
        ulSize += 2;
    }

    ulSize += 16;

    pDigChal    = (PUCHAR)AnscAllocateMemory(ulSize);

    if ( !pDigChal )
    {
        return NULL;
    }

    /* realm */
    rc = sprintf_s
        (
            (char*)pDigChal,
            ulSize,
            "%s=\"%s\"",
            HTTP_AUTH_NAME_realm,
            pRealm
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    /* nonce */
    rc = sprintf_s
        (
            (char*)pDigChal + AnscSizeOfString((const char*)pDigChal),
            (ulSize - AnscSizeOfString((const char*)pDigChal)),
            ", %s=\"%s\"",
            HTTP_AUTH_NAME_nonce,
            pNonce
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    /* algorithm */
    rc = sprintf_s
        (
            (char*)pDigChal + AnscSizeOfString((const char*)pDigChal),
            (ulSize - AnscSizeOfString((const char*)pDigChal)),
            ", %s=\"%s\"",
            HTTP_AUTH_NAME_algorithm,
            pAlgorithm
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
    }

    /* domain */
    if ( pDomain )
    {
        rc = sprintf_s
            (
                (char*)pDigChal + AnscSizeOfString((const char*)pDigChal),
                (ulSize - AnscSizeOfString((const char*)pDigChal)),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_domain,
                pDomain
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }

    /* qop */
    if ( pQop )
    {
        rc = sprintf_s
            (
                (char*)pDigChal + AnscSizeOfString((const char*)pDigChal),
                (ulSize - AnscSizeOfString((const char*)pDigChal)),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_qop,
                pQop
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }

    /* stale if neccessary */
    if ( bNonceExpired )
    {
        rc = sprintf_s
            (
                (char*)pDigChal + AnscSizeOfString((const char*)pDigChal),
                (ulSize - AnscSizeOfString((const char*)pDigChal)),
                ", %s=\"%s\"",
                HTTP_AUTH_NAME_stale,
                HTTP_AUTH_NAME_true
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }
    }

    return pDigChal;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoGenBasicResponse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hResponse
                PUCHAR                      pRealm
            )

    description:

        This function is called to generate basic challenge response.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pRealm
                Authentication realm.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoGenBasicResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        PUCHAR                      pRealm
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_RESOURCES;
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT  )hThisObject;
    PHTTP_HFO_WWW_AUTHENTICATE      pHfoWwwAuth  = NULL;
    PHTTP_AUTH_CHALLENGE            pAuthChal    = NULL;
    PUCHAR                          pChalReq     = NULL;
    PHTTP_CHALLENGE_BASIC           pBasicAuth   = NULL;
    errno_t                         rc           = -1;

    pChalReq    = pMyObject->GenBasicChallenge((ANSC_HANDLE)pMyObject, pRealm);
    if ( !pChalReq )
    {
        return status;
    }

    /* construct WWW-Authenticate header */
    pHfoWwwAuth     = (PHTTP_HFO_WWW_AUTHENTICATE)AnscAllocateMemory(sizeof(HTTP_HFO_WWW_AUTHENTICATE));

    if ( pHfoWwwAuth )
    {
        pHfoWwwAuth->Flags          = 0;
        pHfoWwwAuth->HeaderId       = HTTP_HEADER_ID_WWW_AUTHENTICATE;

        pHfoWwwAuth->ChallengeCount = 1;
        pAuthChal                   = &pHfoWwwAuth->ChallengeArray[0];

        pAuthChal->AuthType = HTTP_AUTH_TYPE_BASIC;
        pBasicAuth  = &pAuthChal->Challenge.Basic;
        rc = strcpy_s(pBasicAuth->Realm, sizeof(pBasicAuth->Realm), (char*)pChalReq);
        ERR_CHK(rc);

        status = pMyObject->GenResponse((ANSC_HANDLE)pMyObject, hResponse, (ANSC_HANDLE)pHfoWwwAuth);
    }

    if ( pChalReq )
    {
        AnscFreeMemory(pChalReq);
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        HttpAuthoGenDigestChallenge
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hResponse,
                PUCHAR                      pRealm,
                PUCHAR                      pDomain,
                PUCHAR                      pAlgorithm,
                PUCHAR                      pQop,
                PUCHAR                      pNonce,
                BOOL                        bNonceExpired
            )

    description:

        This function is called to generate digest challenge response.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hResponse
                The HTTP response object.

                PUCHAR                      pRealm
                Authentication realm.

                PUCHAR                      pDomain
                Authentication domain.

                PUCHAR                      pAlgorithm
                Algorithm name, be it either "MD5" or "MD5-sess" as
                RFC 2167 specifies.

                PUCHAR                      pQop
                Qop value, be it either NULL or "auth". "auth-int"
                is not supported in this version.

                PUCHAR                      pNonce
                Nonce value to be used.

                BOOL                        bNonceExpired
                If true, "stale=true" will be present in the
                challenge.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoGenDigestResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        PUCHAR                      pRealm,
        PUCHAR                      pDomain,
        PUCHAR                      pAlgorithm,
        PUCHAR                      pQop,
        PUCHAR                      pNonce,
        BOOL                        bNonceExpired
    )
{
    ANSC_STATUS                     status       = ANSC_STATUS_RESOURCES;
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT  )hThisObject;
    PHTTP_HFO_WWW_AUTHENTICATE      pHfoWwwAuth  = NULL;
    PHTTP_AUTH_CHALLENGE            pAuthChal    = NULL;
    PUCHAR                          pChalReq     = NULL;
    PHTTP_CHALLENGE_DIGEST          pDigestAuth  = NULL;
    errno_t                         rc           = -1;

    pChalReq    =
        pMyObject->GenDigestChallenge
            (
                (ANSC_HANDLE)pMyObject,
                pRealm,
                pDomain,
                pAlgorithm,
                pQop,
                pNonce,
                bNonceExpired
            );

    if ( !pChalReq )
    {
        return status;
    }

    /* construct WWW-Authenticate header */
    pHfoWwwAuth     = (PHTTP_HFO_WWW_AUTHENTICATE)AnscAllocateMemory(sizeof(HTTP_HFO_WWW_AUTHENTICATE));

    if ( pHfoWwwAuth )
    {
        pHfoWwwAuth->Flags          = 0;
        pHfoWwwAuth->HeaderId       = HTTP_HEADER_ID_WWW_AUTHENTICATE;

        pHfoWwwAuth->ChallengeCount = 1;
        pAuthChal                   = &pHfoWwwAuth->ChallengeArray[0];

        pAuthChal->AuthType = HTTP_AUTH_TYPE_DIGEST;
        pDigestAuth = &pAuthChal->Challenge.Digest;
        rc = strcpy_s(pDigestAuth->Realm, sizeof(pDigestAuth->Realm), (char*)pChalReq);
        ERR_CHK(rc);

        status = pMyObject->GenResponse((ANSC_HANDLE)pMyObject, hResponse, (ANSC_HANDLE)pHfoWwwAuth);
    }

    if ( pChalReq )
    {
        AnscFreeMemory(pChalReq);
    }

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoGenResponse
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hResponse,
                ANSC_HANDLE                 hAuthHeader
            )

    description:

        This function is called to generate response.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hResponse
                Http response object.

                ANSC_HANDLE                 hAuthHeader
                Authentication header.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoGenResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hAuthHeader
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_BMO_REP_OBJECT            pResponse    = (PHTTP_BMO_REP_OBJECT)hResponse;
    PHTTP_HFO_WWW_AUTHENTICATE      pHfoWwwAuth  = (PHTTP_HFO_WWW_AUTHENTICATE)hAuthHeader;
    PHTTP_HFO_CONTENT_TYPE          pHfoConType  = NULL;
    PHTTP_HFO_CONNECTION            pHfoConn     = NULL;
    PHTTP_HFO_CONTENT_LENGTH        pHfoConLen   = NULL;
    PHTTP_RESPONSE_INFO             pRepInfo     = NULL;
    errno_t                         rc           = -1;

    /* add WWW-Authenticate header */
    if ( pHfoWwwAuth )
    {
        pResponse->AddHeaderField
            (
                (ANSC_HANDLE)pResponse,
                (ANSC_HANDLE)pHfoWwwAuth
            );
    }

    /* construct Content-Type header */
    pHfoConType = (PHTTP_HFO_CONTENT_TYPE)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_TYPE));

    if ( pHfoConType )
    {
        pHfoConType->Flags      = 0;
        pHfoConType->HeaderId   = HTTP_HEADER_ID_CONTENT_TYPE;

        pHfoConType->MediaType  = IANA_MEDIA_TYPE_CODE_TEXT;
        pHfoConType->SubType    = IANA_MT_TX_STYPE_CODE_HTML;
        rc = strcpy_s(pHfoConType->Parameters, sizeof(pHfoConType->Parameters), "charset=iso-8859-1");
        ERR_CHK(rc);

        pResponse->AddHeaderField
            (
                (ANSC_HANDLE)pResponse,
                (ANSC_HANDLE)pHfoConType
            );
    }

    /* construct Connection header */
    pHfoConn    = (PHTTP_HFO_CONNECTION)AnscAllocateMemory(sizeof(HTTP_HFO_CONNECTION));

    if ( pHfoConn )
    {
        PHTTP_CONNECTION_TOKEN      pConnToken;

        pHfoConn->Flags         = 0;
        pHfoConn->HeaderId      = HTTP_HEADER_ID_CONNECTION;

        pHfoConn->TokenCount    = 1;
        pConnToken              = &pHfoConn->TokenArray[0];

        pConnToken->OptionType  = IANA_CONNECTION_CODE_KEEP_ALIVE;
        rc = strcpy_s(pConnToken->Token, sizeof(pConnToken->Token), IANA_CONNECTION_TEXT_KEEP_ALIVE);
        ERR_CHK(rc);

        pResponse->AddHeaderField
            (
                (ANSC_HANDLE)pResponse,
                (ANSC_HANDLE)pHfoConn
            );
    }

    /* construct Content-Length header */
    pHfoConLen  = (PHTTP_HFO_CONTENT_LENGTH)AnscAllocateMemory(sizeof(HTTP_HFO_CONTENT_LENGTH));

    if ( pHfoConLen )
    {
        pHfoConLen->Flags       = 0;
        pHfoConLen->HeaderId    = HTTP_HEADER_ID_CONTENT_LENGTH;

        pHfoConLen->BodySize    = AnscSizeOfString(g_httpAuthMsgBody);

        pResponse->AddHeaderField
            (
                (ANSC_HANDLE)pResponse,
                (ANSC_HANDLE)pHfoConLen
            );
    }

    pRepInfo = (PHTTP_RESPONSE_INFO)AnscAllocateMemory(sizeof(HTTP_RESPONSE_INFO));

    if ( !pRepInfo )
    {
        status = ANSC_STATUS_RESOURCES;
        goto EXIT;
    }
    else
    {
        pRepInfo->MajorVersion = HTTP_CURRENT_MAJOR_VERSION;
        pRepInfo->MinorVersion = HTTP_CURRENT_MINOR_VERSION;
        pRepInfo->StatusCode   = pHfoWwwAuth ? HTTP_STATUS_UNAUTHORIZED : HTTP_STATUS_INTERNAL_SERVER_ERROR;

        rc = strcpy_s(pRepInfo->ReasonPhrase, sizeof(pRepInfo->ReasonPhrase), "Unauthorized");
        ERR_CHK(rc);
    }

    pResponse->SetRepInfo
        (
            (ANSC_HANDLE)pResponse,
            (ANSC_HANDLE)pRepInfo
        );

    pResponse->AppendBody
        (
            (ANSC_HANDLE)pResponse,
            (PVOID      )g_httpAuthMsgBody,
            AnscSizeOfString(g_httpAuthMsgBody)
        );

EXIT:

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoGenNonce
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pNonce,
                ULONG                       ulNonceLen
            )

    description:

        This function is called to generate nonce.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pNonce
                Buffer to hold nonce.

                ULONG                       ulNonceLen
                The length in bytes the nonce would be, caller needs
                to make sure the buffer is large enough.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoGenNonce
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pNonce,
        ULONG                       ulNonceLen
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    ULONG                           ulTimeNow    = AnscGetTickInMilliSeconds();
    ULONG                           ulRandomBytes= 0;
    ANSC_CRYPTO_HASH                MD5Hash;
    errno_t                         rc           = -1;

    if ( ulNonceLen <= 2 )
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    ulRandomBytes = ( ulNonceLen - 1 ) / 2;
    if ( ulRandomBytes >= ANSC_MD5_OUTPUT_SIZE )
    {
        ulRandomBytes = ANSC_MD5_OUTPUT_SIZE;
    }

    status =
    	AnscCryptoGetRandomOctets
            (
                NULL,
                ulTimeNow,
                MD5Hash.Value,
                ANSC_MD5_OUTPUT_SIZE
            );

    if ( status != ANSC_STATUS_SUCCESS )
    {
        rc = sprintf_s((char*)MD5Hash.Value, sizeof(MD5Hash.Value), "%.8X", (unsigned int)ulTimeNow); /*Need to confirm*/
        if(rc < EOK)
        {
            ERR_CHK(rc);
        }

        AnscCryptoMd5Digest
            (
                (PVOID)MD5Hash.Value,
                AnscSizeOfString((const char*)MD5Hash.Value),
                &MD5Hash
            );

        status = ANSC_STATUS_SUCCESS;
    }

    HttpAuthoBinToHex(MD5Hash.Value, ulRandomBytes, pNonce);
    pNonce[ulRandomBytes * 2] = 0;

    return status;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoVerify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRequest,
                PUCHAR                      pPassword
                ULONG                       ulServerAuthType,
                PUCHAR                      pServerAuthRealm,
                PUCHAR                      pServerNonce,
                ULONG                       ulNonceTimeout
            )

    description:

        This function is called to verify user's credentials.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRequest
                HTTP request object.

                ULONG                       ulServerAuthType
                Authentication type server specified.

                PUCHAR                      pServerAuthRealm
                Authentication realm server specified.

                PUCHAR                      pServerNonce
                Nonce server specified.

                PUCHAR                      pPassword
                User's password.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoVerify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hAuthInfo,
        PUCHAR                      pPassword,
        ULONG                       ulServerAuthType,
        PUCHAR                      pServerAuthRealm,
        PUCHAR                      pServerNonce,
        ULONG                       ulNonceTimeout,
        ULONG                       ulNonceTimestamp
    )
{
    UNREFERENCED_PARAMETER(hRequest);
    ANSC_STATUS                     status       = ANSC_STATUS_SUCCESS;
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT   )hThisObject;
    PHTTP_AUTHO_INFO                pAuthInfo    = (PHTTP_AUTHO_INFO    )hAuthInfo;
    
    if ( !pAuthInfo )
    {
        pAuthInfo   = (PHTTP_AUTHO_INFO)pMyObject->GetAuthInfo((ANSC_HANDLE)pMyObject, hRequest);
    }

    if ( !pAuthInfo )
    {
        status = ANSC_STATUS_DO_IT_AGAIN;
        goto EXIT;
    }

    if ( !pAuthInfo->pUserName )
    {
        status = ANSC_STATUS_BAD_AUTH_DATA;
        goto EXIT;
    }

    if ( pAuthInfo->AuthType != ulServerAuthType )
    {
        status = ANSC_STATUS_BAD_AUTH_DATA;
        goto EXIT;
    }

    if ( pAuthInfo->AuthType == HTTP_AUTH_TYPE_BASIC )
    {
        if ( !pPassword && !pAuthInfo->pPassword )
        {
            status = ANSC_STATUS_SUCCESS;
        }
        else if ( pPassword && pAuthInfo->pPassword )
        {
            if ( strcmp((char*)pPassword, (char*)pAuthInfo->pPassword) == 0 )
            {
                status = ANSC_STATUS_SUCCESS;
            }
            else
            {
                status = ANSC_STATUS_BAD_AUTH_DATA;
            }
        }
        else
        {
            status  = ANSC_STATUS_BAD_AUTH_DATA;
        }
    }
    else
    {
        PHTTP_AUTHO_DIGEST_INFO         pDigestInfo  = pAuthInfo->pDigest;
        PUCHAR                          pDigRep      = NULL;
        PUCHAR                          pEntityDigest= NULL;

        if ( !pDigestInfo->pDigestUri                           ||
             !pAuthInfo->pRealm                                 ||
             !pDigestInfo->pResponse                            ||
             (pDigestInfo->pQop && !pDigestInfo->pNonceCount)   ||
             !pDigestInfo->pNonce                               ||
             (pDigestInfo->pQop && !pDigestInfo->pCNonce) )
        {
            status = ANSC_STATUS_BAD_AUTH_DATA;
            goto EXIT;
        }

        if ( !strcmp((char*)pAuthInfo->pRealm, (char*)pServerAuthRealm) == 0 )
        {
            status = ANSC_STATUS_BAD_AUTH_DATA;
            goto EXIT;
        }

        if ( !strcmp((char*)pDigestInfo->pNonce, (char*)pServerNonce) == 0 )
        {
            status = ANSC_STATUS_BAD_AUTH_DATA;
            goto EXIT;
        }

        pDigRep =
            pMyObject->CalcDigRep
                (
                    (ANSC_HANDLE)pMyObject,
                    hRequest,
                    pAuthInfo->pUserName,
                    pPassword,
                    pAuthInfo->pRealm,
                    pDigestInfo->pAlgorithm,
                    pDigestInfo->pNonce,
                    pDigestInfo->pCNonce,
                    pDigestInfo->pQop,
                    pDigestInfo->pNonceCount,
                    pEntityDigest,
                    pDigestInfo->pDigestUri
                );

        if ( !pDigRep )
        {
            status = ANSC_STATUS_RESOURCES;
        }
        else
        {
            if ( strcmp((char*)pDigRep, (char*)pDigestInfo->pResponse) == 0 )
            {
                status = ANSC_STATUS_SUCCESS;
            }
            else
            {
                status = ANSC_STATUS_BAD_AUTH_DATA;
            }

            AnscFreeMemory(pDigRep);
        }

        if ( status == ANSC_STATUS_SUCCESS && ulNonceTimeout != 0 )
        {
            /* check if the nonce has expired */
            ULONG                   ulTimeNow    = AnscGetTickInSeconds();
            BOOL                    bNonceExpired;

            bNonceExpired   =
                (ulTimeNow >= ulNonceTimestamp) ?
                    ( ulTimeNow - ulNonceTimestamp > ulNonceTimeout ) :
                    ( 0xFFFFFFFF - ulNonceTimestamp + ulTimeNow > ulNonceTimeout );

            if ( bNonceExpired )
            {
                status  = ANSC_STATUS_DO_IT_AGAIN;
            }
        }
    }

EXIT:

    if ( (ANSC_HANDLE)NULL == hAuthInfo && pAuthInfo )
    {
        HttpAuthInfoRemove(pAuthInfo);
    }

    return status;
}

