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

#define  HTTP_AUTH_INFO_TIMEOUT_VAL                 600         /* in seconds */

typedef  struct
_HTTP_AUTH_DIGEST_INFO
{
    PUCHAR                          pDomain;
    PUCHAR                          pNonce;
    PUCHAR                          pOpaque;
    PUCHAR                          pAlgorithm;
    PUCHAR                          pQop;

    ULONG                           NC;
}
HTTP_AUTH_DIGEST_INFO, *PHTTP_AUTH_DIGEST_INFO;


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
        AnscFreeMemory(pDinfo);                                     \
    }                                                               \
    while (0)


/*
 * HTTP Auth Info is defined for HTTP client to remeber authentication
 * information used after user's credential has been accepted. In this
 * way, client will always append Authorization header in the following
 * requests to the same server (and domain for digest authentication).
 */
typedef  struct
_HTTP_AUTH_INFO
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

    PHTTP_AUTH_DIGEST_INFO          pDigest;
}
HTTP_AUTH_INFO,  *PHTTP_AUTH_INFO;

#define  ACCESS_HTTP_AUTH_INFO_OBJECT(p)                            \
         ACCESS_CONTAINER(p, HTTP_AUTH_INFO_OBJECT, Linkage)

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

