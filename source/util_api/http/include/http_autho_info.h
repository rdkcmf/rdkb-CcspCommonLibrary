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

    module:	http_autho_info.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Http Authentication Object.

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


#ifndef  _HTTP_AUTHO_INFO_
#define  _HTTP_AUTHO_INFO_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */


/***********************************************************
    PLATFORM INDEPENDENT HTTP AUTH INFO DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  HTTP_AUTH_NAME_basic                       "basic"
#define  HTTP_AUTH_NAME_digest                      "digest"

#define  HTTP_AUTH_NAME_realm                       "realm"
#define  HTTP_AUTH_NAME_domain                      "domain"
#define  HTTP_AUTH_NAME_nonce                       "nonce"
#define  HTTP_AUTH_NAME_opaque                      "opaque"
#define  HTTP_AUTH_NAME_stale                       "stale"
#define  HTTP_AUTH_NAME_algorithm                   "algorithm"
#define  HTTP_AUTH_NAME_qop                         "qop"
#define  HTTP_AUTH_NAME_username                    "username"
#define  HTTP_AUTH_NAME_digest_uri                  "uri"
#define  HTTP_AUTH_NAME_response                    "response"
#define  HTTP_AUTH_NAME_cnonce                      "cnonce"
#define  HTTP_AUTH_NAME_nc                          "nc"

#define  HTTP_AUTH_NAME_md5                         "MD5"
#define  HTTP_AUTH_NAME_md5_sess                    "md5-sess"
#define  HTTP_AUTH_NAME_auth                        "auth"
#define  HTTP_AUTH_NAME_auth_int                    "auth-int"
#define  HTTP_AUTH_NAME_true                        "true"


#define  HttpAuthCloneMemory(pDst, pSrc, ulSize)            \
    do                                                      \
    {                                                       \
        pDst    = NULL;                                     \
        if ( ulSize != 0 )                                  \
        {                                                   \
            pDst    = (PUCHAR)AnscAllocateMemory(ulSize+1); \
            if ( pDst )                                     \
            {                                               \
                AnscCopyMemory								\
					((PVOID)pDst, (PVOID)pSrc, ulSize);     \
                pDst[ulSize]    = 0;                        \
            }                                               \
        }                                                   \
    }                                                       \
    while (0)

typedef  struct
_HTTP_AUTHO_DIGEST_DOMAIN_URIS
{
    ULONG                           NumURIs;
    PUCHAR                          *pURIs;
}
HTTP_AUTHO_DIGEST_DOMAIN_URIS, *PHTTP_AUTHO_DIGEST_DOMAIN_URIS;

typedef  struct
_HTTP_AUTHO_DIGEST_INFO
{
    PUCHAR                          pDomain;
    PUCHAR                          pNonce;
    PUCHAR                          pOpaque;
    PUCHAR                          pAlgorithm;
    PUCHAR                          pQop;

    ULONG                           NC;

    PUCHAR                          pResponse;
    PUCHAR                          pDigestUri;
    PUCHAR                          pNonceCount;
    PUCHAR                          pCNonce;

    PHTTP_AUTHO_DIGEST_DOMAIN_URIS  pDomainURIs;
}
HTTP_AUTHO_DIGEST_INFO, *PHTTP_AUTHO_DIGEST_INFO;

#define  HttpAuthDigestInfoCreate(pDigestInfo)                              \
    do                                                                      \
    {                                                                       \
        pDigestInfo   =                                                     \
            (PHTTP_AUTHO_DIGEST_INFO)AnscAllocateMemory                     \
                (                                                           \
                    sizeof(HTTP_AUTHO_DIGEST_INFO)                          \
                );                                                          \
        if ( pDigestInfo )                                                  \
        {                                                                   \
            AnscZeroMemory(pDigestInfo, sizeof(HTTP_AUTHO_DIGEST_INFO));    \
        }                                                                   \
    }                                                                       \
    while (0)

#define  HttpAuthDigestInfoRemove(pDinfo)                           \
    do                                                              \
    {                                                               \
        if ( !(pDinfo) )                                            \
        {                                                           \
            break;                                                  \
        }                                                           \
        if ( (pDinfo)->pDomain )                                    \
        {                                                           \
            AnscFreeMemory((pDinfo)->pDomain);                      \
        }                                                           \
        if ( (pDinfo)->pNonce )                                     \
        {                                                           \
            AnscFreeMemory((pDinfo)->pNonce);                       \
        }                                                           \
        if ( (pDinfo)->pOpaque )                                    \
        {                                                           \
            AnscFreeMemory((pDinfo)->pOpaque);                      \
        }                                                           \
        if ( (pDinfo)->pAlgorithm )                                 \
        {                                                           \
            AnscFreeMemory((pDinfo)->pAlgorithm);                   \
        }                                                           \
        if ( (pDinfo)->pQop )                                       \
        {                                                           \
            AnscFreeMemory((pDinfo)->pQop);                         \
        }                                                           \
        if ( (pDinfo)->pResponse )                                  \
        {                                                           \
            AnscFreeMemory((pDinfo)->pResponse);                    \
        }                                                           \
        if ( (pDinfo)->pDigestUri )                                 \
        {                                                           \
            AnscFreeMemory((pDinfo)->pDigestUri);                   \
        }                                                           \
        if ( (pDinfo)->pNonceCount )                                \
        {                                                           \
            AnscFreeMemory((pDinfo)->pNonceCount);                  \
        }                                                           \
        if ( (pDinfo)->pCNonce )                                    \
        {                                                           \
            AnscFreeMemory((pDinfo)->pCNonce);                      \
        }                                                           \
        if ( (pDinfo)->pDomainURIs )                                \
        {                                                           \
            if ( (pDinfo)->pDomainURIs->pURIs )                     \
            {                                                       \
                AnscFreeMemory((pDinfo)->pDomainURIs->pURIs);       \
            }                                                       \
            AnscFreeMemory((pDinfo)->pDomainURIs);                  \
        }                                                           \
        AnscFreeMemory(pDinfo);                                     \
    }                                                               \
    while (0)


/*
 * HTTP Autho Info is defined to hold information all about HTTP
 * authentication.
 */
typedef  struct
_HTTP_AUTHO_INFO
{
    SINGLE_LINK_ENTRY               Linkage;

    ULONG                           Timestamp;

    PUCHAR                          pHostName;
    USHORT                          HostPort;
    PUCHAR                          pUri;

    ULONG                           AuthType;
    PUCHAR                          pRealm;
    PUCHAR                          pUserName;
    PUCHAR                          pPassword;

    PHTTP_AUTHO_DIGEST_INFO         pDigest;
}
HTTP_AUTHO_INFO,  *PHTTP_AUTHO_INFO;

#define  ACCESS_HTTP_AUTHO_INFO_OBJECT(p)                           \
         ACCESS_CONTAINER(p, HTTP_AUTHO_INFO, Linkage)

#define  HttpAuthInfoCreate(pAuthInfo)                              \
    do                                                              \
    {                                                               \
        pAuthInfo   =                                               \
            (PHTTP_AUTHO_INFO)AnscAllocateMemory                    \
                (                                                   \
                    sizeof(HTTP_AUTHO_INFO)                         \
                );                                                  \
        if ( pAuthInfo )                                            \
        {                                                           \
            AnscZeroMemory(pAuthInfo, sizeof(HTTP_AUTHO_INFO));     \
        }                                                           \
    }                                                               \
    while (0)

#define  HttpAuthInfoRemove(pAuthInfo)                              \
    do                                                              \
    {                                                               \
        if ( (pAuthInfo)->pHostName )                               \
        {                                                           \
            AnscFreeMemory((pAuthInfo)->pHostName);                 \
        }                                                           \
        if ( (pAuthInfo)->pUri )                                    \
        {                                                           \
            AnscFreeMemory((pAuthInfo)->pUri);                      \
        }                                                           \
        if ( (pAuthInfo)->pRealm )                                  \
        {                                                           \
            AnscFreeMemory((pAuthInfo)->pRealm);                    \
        }                                                           \
        if ( (pAuthInfo)->pUserName )                               \
        {                                                           \
            AnscFreeMemory((pAuthInfo)->pUserName);                 \
        }                                                           \
        if ( (pAuthInfo)->pPassword )                               \
        {                                                           \
            AnscFreeMemory((pAuthInfo)->pPassword);                 \
        }                                                           \
        if ( (pAuthInfo)->AuthType == HTTP_AUTH_TYPE_DIGEST )       \
        {                                                           \
            HttpAuthDigestInfoRemove((pAuthInfo)->pDigest);         \
        }                                                           \
        AnscFreeMemory(pAuthInfo);                                  \
    }                                                               \
    while (0)


#endif

