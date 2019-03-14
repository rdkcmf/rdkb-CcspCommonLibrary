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

    module:	http_fco_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Form Content
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/17/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_FCO_INTERNAL_API_
#define  _HTTP_FCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_FCO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpFcoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpFcoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
HttpFcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_FCO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpFcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpFcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_FCO_ACCESS.C
***********************************************************/

ANSC_HANDLE
HttpFcoGetUriParams
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpFcoGetFormInput
    (
        ANSC_HANDLE                 hThisObject
    );

char*
HttpFcoGetFormParam
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

void*
HttpFcoGetFormParam2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulSize
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN HTTP_FCO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpFcoGetFormElement
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
HttpFcoAddFormElement
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hFeo
    );

ANSC_STATUS
HttpFcoDelFormElement
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
HttpFcoDelAllFeos
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
