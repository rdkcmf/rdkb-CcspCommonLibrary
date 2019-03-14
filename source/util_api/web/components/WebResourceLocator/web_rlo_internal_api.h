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

    module:	web_rlo_internal_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Web Resource Locator
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/


#ifndef  _WEB_RLO_INTERNAL_API_
#define  _WEB_RLO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_RLO_STATES.C
***********************************************************/

ANSC_HANDLE
WebRloGetRootUriPath
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRloGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
WebRloSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
WebRloResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRloReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_RLO_OPERATION.C
***********************************************************/

ANSC_STATUS
WebRloEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRloCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_RLO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
WebRloMakUriPath
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_HANDLE
WebRloGetUriPath
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_HANDLE
WebRloGetResourceOwner1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_HANDLE
WebRloGetResourceOwner2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    );

ANSC_STATUS
WebRloAddResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hRoo
    );

ANSC_STATUS
WebRloPopResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_STATUS
WebRloDelResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeRoo
    );


#endif
