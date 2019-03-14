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

    module:	http_authco_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Authentication
        Client Object.

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


#ifndef  _HTTP_AUTHCO_INTERNAL_API_
#define  _HTTP_AUTHCO_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_AUTHCO_PROCESS.C
***********************************************************/

ANSC_STATUS
HttpAuthcoProcessRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

ANSC_STATUS
HttpAuthcoProcessResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse
    );

PUCHAR
HttpAuthcoGenerateDigestResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hAuthInfo,
        PUCHAR                      pRequestDigest,
        ULONG                       ulNC
    );

ANSC_STATUS
HttpAuthcoGenerateCNonce
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hAuthInfo
    );

ULONG
HttpAuthcoGetNC
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hAuthInfo
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_AUTHCO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
HttpAuthcoAddAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hChallenge
    );

ANSC_HANDLE
HttpAuthcoFindAuthInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

ANSC_HANDLE
HttpAuthcoFindAuthInfo2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        PBOOL                       pbParentUri
    );

ANSC_STATUS
HttpAuthcoClearAllAuthInfo
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_AUTHCO_STATE.C
***********************************************************/

ANSC_STATUS
HttpAuthcoSetAcmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
HttpAuthcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
