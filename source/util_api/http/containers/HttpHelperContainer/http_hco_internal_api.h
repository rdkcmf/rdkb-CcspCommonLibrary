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

    module:	http_hco_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Helper
        Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_HCO_INTERNAL_API_
#define  _HTTP_HCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_HCO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpHcoGetAtoStatus
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpHcoGetAtoHeader
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpHcoGetAtoMethod
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpHcoGetAtoCgiEnv
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpHcoGetAtoFxCat1
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpHcoGetAtoFxCat2
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpHcoGetSmartClock
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_HCO_HELP.C
***********************************************************/

ULONG
HttpHcoGetHeaderId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

char*
HttpHcoGetHeaderName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulHeaderId
    );

ULONG
HttpHcoGetMethodId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

char*
HttpHcoGetMethodName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethodId
    );

ULONG
HttpHcoGetCgiEnvId
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

char*
HttpHcoGetReasonPhrase
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    );

ULONG
HttpHcoGetExtMediaType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_ext
    );

ULONG
HttpHcoGetExtSubType
    (
        ANSC_HANDLE                 hThisObject,
        char*                       file_ext
    );

ANSC_STATUS
HttpHcoGetClockTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    );

ANSC_STATUS
HttpHcoGetUtcTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_HCO_EXTENSION.C
***********************************************************/

ANSC_STATUS
HttpHcoEnrollExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpHcoManufactureExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpHcoDestroyExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_HCO_FEATURE.C
***********************************************************/

ANSC_STATUS
HttpHcoEnrollFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpHcoManufactureFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpHcoDestroyFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
