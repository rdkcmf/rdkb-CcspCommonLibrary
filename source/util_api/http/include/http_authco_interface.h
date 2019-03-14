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

    module:	http_authco_interface.h

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


#ifndef  _HTTP_AUTHCO_INTERFACE_
#define  _HTTP_AUTHCO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "http_autho_interface.h"
#include "http_autho_exported_api.h"


/***********************************************************
   PLATFORM INDEPENDENT HTTP AUTH CLIENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  HTTP_AUTHCO_AINFO_MAX_COUNT                32

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_HTTPAUTHCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_HTTPAUTHCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_PROCESS_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_PROCESS_REP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_ADD_AUTHINFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hChallenge
    );

typedef  ANSC_HANDLE
(*PFN_HTTPAUTHCO_GET_AUTHINFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

typedef  ANSC_HANDLE
(*PFN_HTTPAUTHCO_GET_AUTHINFO2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PBOOL                       pbParentUri
    );

typedef  ULONG
(*PFN_HTTPAUTHCO_GET_NC)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hAuthInfo
    );

typedef  PUCHAR
(*PFN_HTTPAUTHCO_GEN_DIGREP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hAuthInfo,
        PUCHAR                      pRequestDigest,
        ULONG                       ulNC
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_GEN_CNONCE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAuthInfo
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_SET_AINFO_MAX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMaxCount
    );

typedef  ANSC_STATUS
(*PFN_HTTPAUTHCO_CLR_AUTHINFO)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * HTTP Authentication object is defined to provide authentication services to
 * HTTP client.
 */
#define  HTTP_AUTHCO_CLASS_CONTENT                                                          \
    /* duplication of the base object class content */                                      \
    HTTP_AUTH_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hAcmIf;                                                 \
    SLIST_HEADER                    AuthInfoList;                                           \
    ANSC_LOCK                       AuthInfoListLock;                                       \
    ULONG                           AuthInfoMaxCount;                                       \
                                                                                            \
    PFN_HTTPAUTHCO_SET_IF           SetAcmIf;                                               \
    PFN_HTTPAUTHCO_SET_AINFO_MAX    SetMaxAinfoCount;                                       \
                                                                                            \
    PFN_HTTPAUTHCO_PROCESS_REQ      ProcessReq;                                             \
    PFN_HTTPAUTHCO_PROCESS_REP      ProcessRep;                                             \
                                                                                            \
    PFN_HTTPAUTHCO_ADD_AUTHINFO     AddAuthInfo;                                            \
    PFN_HTTPAUTHCO_GET_AUTHINFO     FindAuthInfo;                                           \
    PFN_HTTPAUTHCO_GET_AUTHINFO2    FindAuthInfo2;                                          \
    PFN_HTTPAUTHCO_CLR_AUTHINFO     ClearAllAuthInfo;                                       \
                                                                                            \
    PFN_HTTPAUTHCO_GEN_DIGREP       GenerateDigestRep;                                      \
    PFN_HTTPAUTHCO_GEN_CNONCE       GenerateCNonce;                                         \
                                                                                            \
    PFN_HTTPAUTHCO_GET_NC           GetNC;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_AUTH_CLIENT_OBJECT
{
    HTTP_AUTHCO_CLASS_CONTENT
}
HTTP_AUTH_CLIENT_OBJECT,  *PHTTP_AUTH_CLIENT_OBJECT;

#define  ACCESS_HTTP_AUTHCO_OBJECT(p)                 \
         ACCESS_CONTAINER(p, HTTP_AUTHCO_CLIENT_OBJECT, Linkage)


#endif
