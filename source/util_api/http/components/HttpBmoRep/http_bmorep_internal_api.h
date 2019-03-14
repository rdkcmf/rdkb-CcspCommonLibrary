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

    module:	http_bmorep_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Bmo Rep
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/27/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_BMOREP_INTERNAL_API_
#define  _HTTP_BMOREP_INTERNAL_API_


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_BMOREP_ACCESS.C
***********************************************************/

ULONG
HttpBmoRepGetStartLineSize
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoRepPredictBodySize
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpBmoRepIsHttpVersion11
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpBmoRepGetReqInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpBmoRepGetRepInfo
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoRepGetCode
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoRepGetMajorVersion
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoRepGetMinorVersion
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpBmoRepGetText
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpBmoRepGetProgressBits
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoRepSetReqInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    );

ANSC_STATUS
HttpBmoRepSetRepInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInfo
    );

ANSC_STATUS
HttpBmoRepSetCode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    );

ANSC_STATUS
HttpBmoRepSetMajorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    );

ANSC_STATUS
HttpBmoRepSetMinorVersion
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       version
    );

ANSC_STATUS
HttpBmoRepSetText
    (
        ANSC_HANDLE                 hThisObject,
        char*                       text
    );

ANSC_STATUS
HttpBmoRepSetProgressBits
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       progress_bits
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_BMOREP_STATES.C
***********************************************************/

PUCHAR
HttpBmoRepGetServerAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoRepSetServerAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
HttpBmoRepGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoRepSetServerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

ANSC_STATUS
HttpBmoRepReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_BMOREP_HEADER.C
***********************************************************/

ANSC_STATUS
HttpBmoRepDelStartLine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoRepParseStartLine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpBmoRepCopyStartLineFrom
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );


#endif
