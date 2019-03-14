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

    module:	web_mco_internal_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Web Module
        Container Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/02    initial revision.

**********************************************************************/


#ifndef  _WEB_MCO_INTERNAL_API_
#define  _WEB_MCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN WEB_MCO_STATES.C
***********************************************************/

ANSC_HANDLE
WebMcoGetHttpHco
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
WebMcoGetWebSso
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_MCO_EXTENSION.C
***********************************************************/

ANSC_STATUS
WebMcoEnrollExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebMcoManufactureExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebMcoDestroyExtensionObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN WEB_MCO_FEATURE.C
***********************************************************/

ANSC_STATUS
WebMcoEnrollFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebMcoManufactureFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebMcoDestroyFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN WEB_MCO_OPERATION.C
***********************************************************/

ANSC_STATUS
WebMcoIgnite
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebMcoPutOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebMcoSwitch
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSwitchOn
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN WEB_MCO_CONTROL.C
***********************************************************/

ANSC_STATUS
WebMcoRegisterIfs
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebMcoDispatchIfs
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIqcIf
    );


#endif
