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

    module:	web_sso_internal_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Web Simple Server
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


#ifndef  _WEB_SSO_INTERNAL_API_
#define  _WEB_SSO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_SSO_STATES.C
***********************************************************/

ANSC_HANDLE
WebSsoGetSiteManager
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
WebSsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebSsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
WebSsoGetFumIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
WebSsoGetCspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
WebSsoGetServerMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebSsoSetServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
WebSsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
WebSsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
WebSsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebSsoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_SSO_OPERATION.C
***********************************************************/

ANSC_STATUS
WebSsoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebSsoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_SSO_CSPIF.C
***********************************************************/

ULONG
WebSsoCspGetCfgMode
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
WebSsoCspIsReadOnly
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
WebSsoCspIsSwitchOn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebSsoCspEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebSsoCspCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebSsoCspAddHttpDaemon
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDaemonInfo
    );

ANSC_STATUS
WebSsoCspDelHttpDaemon
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usDaemonPort
    );

ANSC_STATUS
WebSsoCspSetHttpServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
WebSsoCspSetPersistentConn
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bPersistentConn
    );

ANSC_STATUS
WebSsoCspSetProductName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pProductName
    );

/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_SSO_FUMIF.C
***********************************************************/

ANSC_STATUS
WebSsoRegisterMdhIf
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRequestUri,
        ANSC_HANDLE                 hMdhIf
    );

ANSC_HANDLE
WebSsoGetMdhIf
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pRequestUri
    );

#endif
