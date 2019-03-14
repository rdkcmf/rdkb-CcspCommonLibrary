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

    module:	web_authco_interface.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Web Authentication Server
        Object.

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


#ifndef  _WEB_AUTHSO_INTERFACE_
#define  _WEB_AUTHSO_INTERFACE_


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
    PLATFORM INDEPENDENT WEB AUTH SERVER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_WEBAUTHSO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_WEBAUTHSO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_ENABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_AUTH_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_VERIFY_CRED)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hWaSessInfo
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_GEN_AUTH_REP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hWaSessInfo,
        BOOL                        bNonceExpired
    );

typedef  ANSC_STATUS
(*PFN_WEBAUTHSO_GEN_NONCE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWaSessInfo
    );

typedef  PUCHAR
(*PFN_WEBAUTHSO_GEN_DIGCHAL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWaSessInfo,
        BOOL                        bNonceExpired
    );

typedef  PUCHAR
(*PFN_WEBAUTHSO_GEN_BISCACHAL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWaSessInfo
    );

/*
 * WEB Auth Server object is defined to provide authentication services to
 * web server.
 */
#define  WEB_AUTH_SERVER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    HTTP_AUTH_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    WEB_AUTH_SERVER_PROPERTY        Property;                                               \
    WEB_AUTH_SESSION_INFO           DefaultSessInfo;                                        \
    ANSC_HANDLE                     hAcmIf;                                                 \
                                                                                            \
    PFN_WEBAUTHSO_SET_IF            SetAcmIf;                                               \
    PFN_WEBAUTHSO_ACTION            ResetProperty;                                          \
    PFN_WEBAUTHSO_GET_PROPERTY      GetProperty;                                            \
    PFN_WEBAUTHSO_SET_PROPERTY      SetProperty;                                            \
                                                                                            \
    PFN_WEBAUTHSO_AUTH_REQ          AuthenticateReq;                                        \
                                                                                            \
    PFN_WEBAUTHSO_VERIFY_CRED       VerifyCredential;                                       \
    /* end of object class content */                                                       \

typedef  struct
_WEB_AUTH_SERVER_OBJECT
{
    WEB_AUTH_SERVER_CLASS_CONTENT
}
WEB_AUTH_SERVER_OBJECT,  *PWEB_AUTH_SERVER_OBJECT;

#define  ACCESS_WEB_AUTH_SERVER_OBJECT(p)             \
         ACCESS_CONTAINER(p, WEB_AUTH_SERVER_OBJECT, Linkage)


#endif
