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

    module:	http_autho_interface.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Authentication Client
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


#ifndef  _HTTP_AUTHO_INTERFACE_
#define  _HTTP_AUTHO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


/***********************************************************
   PLATFORM INDEPENDENT HTTP AUTH CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPAUTHO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPAUTHO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_ENABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  BOOL
(*PFN_HTTPAUTHO_IS_ENABLED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PUCHAR
(*PFN_HTTPAUTHO_CALC_DIGREP)
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

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_CALC_HA1)
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

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_CALC_HA2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      pQop,
        PUCHAR                      pEntityDigest,
        PUCHAR                      pDigestUri,
        PUCHAR                      pHA2
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_GET_HOSTURI)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PUCHAR                      *ppHostName,
        PUSHORT                     pHostPort,
        PUCHAR                      *ppUriPath
    );

typedef  ANSC_HANDLE
(*PFN_HTTPAUTHO_GET_AINFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_GET_DAINFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAuthInfo,
        PUCHAR                      pDigData
    );

typedef  PUCHAR
(*PFN_HTTPAUTHO_GEN_BCHAL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRealm
    );

typedef  PUCHAR
(*PFN_HTTPAUTHO_GEN_DCHAL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRealm,
        PUCHAR                      pDomain,
        PUCHAR                      pAlgorithm,
        PUCHAR                      pQop,
        PUCHAR                      pNonce,
        BOOL                        bNonceExpired
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_GEN_BRESP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        PUCHAR                      pRealm
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_GEN_DRESP)
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

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_GEN_RESP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hAuthHeader
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_GEN_NONCE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pNonce,
        ULONG                       ulNonceLen
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHO_VERIFY)
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

/*
 * HTTP Authentication object is defined to provide authentication services to
 * HTTP client and server.
 */
#define  HTTP_AUTH_CLASS_CONTENT                                                            \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BOOL                            bAuthOn;                                                \
                                                                                            \
    PFN_HTTPAUTHO_ENABLE            Enable;                                                 \
    PFN_HTTPAUTHO_IS_ENABLED        IsEnabled;                                              \
    PFN_HTTPAUTHO_RESET             Reset;                                                  \
                                                                                            \
    PFN_HTTPAUTHO_CALC_DIGREP       CalcDigRep;                                             \
                                                                                            \
    PFN_HTTPAUTHO_CALC_HA1          CalcDigestHA1;                                          \
    PFN_HTTPAUTHO_CALC_HA2          CalcDigestHA2;                                          \
    PFN_HTTPAUTHO_GET_HOSTURI       GetRequestHostUri;                                      \
                                                                                            \
    PFN_HTTPAUTHO_GET_AINFO         GetAuthInfo;                                            \
    PFN_HTTPAUTHO_GET_DAINFO        GetDigestAuthInfo;                                      \
                                                                                            \
    PFN_HTTPAUTHO_GEN_BRESP         GenBasicResponse;                                       \
    PFN_HTTPAUTHO_GEN_DRESP         GenDigestResponse;                                      \
    PFN_HTTPAUTHO_GEN_RESP          GenResponse;                                            \
                                                                                            \
    PFN_HTTPAUTHO_GEN_BCHAL         GenBasicChallenge;                                      \
    PFN_HTTPAUTHO_GEN_DCHAL         GenDigestChallenge;                                     \
                                                                                            \
    PFN_HTTPAUTHO_GEN_NONCE         GenNonce;                                               \
                                                                                            \
    PFN_HTTPAUTHO_VERIFY            Verify;                                                 \
    /* end of object class content */                                                       \


typedef  struct
_HTTP_AUTH_OBJECT
{
    HTTP_AUTH_CLASS_CONTENT
}
HTTP_AUTH_OBJECT,  *PHTTP_AUTH_OBJECT;

#define  ACCESS_HTTP_AUTH_OBJECT(p)             \
         ACCESS_CONTAINER(p, HTTP_AUTH_OBJECT, Linkage)


__inline  static  void
HttpAuthoBinToHex
    (
        PUCHAR                  Bin,
        ULONG                   ulBinLen,
        PUCHAR                  Hex
    )
{
    ULONG                       i;
    USHORT                      j;

    for ( i = 0; i < ulBinLen; i++ )
    {
        j = (Bin[i] >> 4) & 0x0F;
        if (j <= 9)
            Hex[i*2] = (j + '0');
         else
            Hex[i*2] = (j + 'a' - 10);
        j = Bin[i] & 0x0F;
        if (j <= 9)
            Hex[i*2+1] = (j + '0');
         else
            Hex[i*2+1] = (j + 'a' - 10);
    }

    Hex[ulBinLen * 2] = '\0';
}


#endif
