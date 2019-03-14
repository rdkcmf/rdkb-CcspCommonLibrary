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

    module:	web_roo_exported_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Web Resource Owner
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/10/02    initial revision.

**********************************************************************/


#ifndef  _WEB_ROO_EXPORTED_API_
#define  _WEB_ROO_EXPORTED_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN WEB_ROO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
WebCreateResourceOwner
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_ROO_BASE.C
***********************************************************/

ANSC_HANDLE
WebRooCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
WebRooRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_ROO_STATES.C
***********************************************************/

ANSC_STATUS
WebRooResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_ROO_OPERATION.C
***********************************************************/

ANSC_STATUS
WebRooEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooCancel
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
