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

    module:	http_ifo_cas.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the HTTP Client Authentication Service
        Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/11/05    initial revision.

**********************************************************************/


#ifndef  _HTTP_IFO_CAS_
#define  _HTTP_IFO_CAS_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
 HTTP AUTH CREDENTIAL MANAGEMENT INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  ANSC_STATUS
(*PFN_HTTPCASIF_ENABLE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    );

typedef  BOOL
(*PFN_HTTPCASIF_IS_ENABLED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_HTTPCASIF_PROCESS_REQ)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

typedef  ANSC_STATUS
(*PFN_HTTPCASIF_PROCESS_REP)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hWebcTransObj
    );

typedef  ANSC_STATUS
(*PFN_HTTPCASIF_CLR_AUTHINFO)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * HTTP Authentication Credential Management interface is defined to
 * retrieve user credential on the request URI.
 */
#define  HTTP_CAS_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_HTTPCASIF_ENABLE            EnableAuth;                                             \
    PFN_HTTPCASIF_IS_ENABLED        IsAuthEnabled;                                          \
    PFN_HTTPCASIF_PROCESS_REQ       ProcessRequest;                                         \
    PFN_HTTPCASIF_PROCESS_REP       ProcessResponse;                                        \
    PFN_HTTPCASIF_CLR_AUTHINFO      ClearAuthInfo;                                          \
    /* end of object class content */                                                       \

typedef  struct
_HTTP_CAS_INTERFACE
{
    HTTP_CAS_INTERFACE_CLASS_CONTENT
}
HTTP_CAS_INTERFACE,  *PHTTP_CAS_INTERFACE;

#define  ACCESS_HTTP_CAS_INTERFACE(p)               \
         ACCESS_CONTAINER(p, HTTP_CAS_INTERFACE, Linkage)


#endif

