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

    module:	http_autho_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Authentication
        Object.

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


#ifndef  _HTTP_AUTHO_INTERNAL_API_
#define  _HTTP_AUTHO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_AUTHO_PROCESS.C
***********************************************************/

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
    );

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
    );

ANSC_STATUS
HttpAuthoCalcDigestHA2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      pQop,
        PUCHAR                      pEntityDigest,
        PUCHAR                      pDigestUri,
        PUCHAR                      pHA2
    );

ANSC_STATUS
HttpAuthoGetRequestHostUri
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      *ppHostName,
        PUSHORT                     pHostPort,
        PUCHAR                      *ppUriPath
    );

ANSC_HANDLE
HttpAuthoGetAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

ANSC_STATUS
HttpAuthoGetDigestAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAuthInfo,
        PUCHAR                      pDigData
    );

PUCHAR
HttpAuthoGenBasicChallenge
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRealm
    );

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
    );

ANSC_STATUS
HttpAuthoGenBasicResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        PUCHAR                      pRealm
    );

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
    );

ANSC_STATUS
HttpAuthoGenResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hAuthHeader
    );

ANSC_STATUS
HttpAuthoGenNonce
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pNonce,
        ULONG                       ulNonceLen
    );

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
    );

/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_AUTHO_STATE.C
***********************************************************/

ANSC_STATUS
HttpAuthoEnable
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

BOOL
HttpAuthoIsEnabled
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpAuthoGetCryptoObject
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
