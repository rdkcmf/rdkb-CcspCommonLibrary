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

    module:	web_vho_internal_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Web Virtual Host
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


#ifndef  _WEB_VHO_INTERNAL_API_
#define  _WEB_VHO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_VHO_STATES.C
***********************************************************/

char*
WebVhoGetHostName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebVhoSetHostName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

USHORT
WebVhoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebVhoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

ANSC_HANDLE
WebVhoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebVhoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
WebVhoGetLsmIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebVhoSetLsmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

BOOL
WebVhoIsDefaultHost
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebVhoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
WebVhoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
WebVhoSetHttpAuth
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ULONG                       http_auth_method,
        ANSC_HANDLE                 hWebAcmIf
    );

ANSC_STATUS
WebVhoSetHttpAuth2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hWebAcmIf,
		ULONG						http_auth_method, 
        ULONG                       alg,              
        BOOL                        bNoQop,
        ULONG                       NonceTimeout,     
        char*                       realm,            
        char*                       domain            
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_VHO_OPERATION.C
***********************************************************/

ANSC_STATUS
WebVhoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebVhoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
WebVhoMatchName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
WebVhoSessionTdoInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_VHO_MANAGEMENT.C
***********************************************************/

ANSC_STATUS
WebVhoAttachRoo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRoo
    );

ANSC_STATUS
WebVhoDetachRoo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_STATUS
WebVhoDeleteRoo
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

ANSC_HANDLE
WebVhoIdentifyGso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_HANDLE
WebVhoGetGso
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId,
        char*                       identifier,
        PUCHAR                      address,
        USHORT                      port
    );

ANSC_HANDLE
WebVhoAddGso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       identifier,
        PUCHAR                      address,
        USHORT                      port
    );

ANSC_STATUS
WebVhoDelGso
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionId,
        PUCHAR                      address,
        USHORT                      port
    );

ANSC_STATUS
WebVhoDelAllGsos
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN WEB_VHO_PROCESS.C
***********************************************************/

ULONG
WebVhoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
WebVhoProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
WebVhoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ULONG                       ulEvent
    );

ANSC_STATUS
WebVhoConstructResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ULONG                       ulStatusCode
    );

ANSC_STATUS
WebVhoAddHttpRepHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebVhoGetResourcePath
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hTokenChain,
        BOOL                        bReferred
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN WEB_VHO_LSM.C
***********************************************************/

ANSC_STATUS
WebVhoLsmNewContact
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
WebVhoLsmClassifyClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
WebVhoLsmExpireSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
WebVhoLsmEndSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );


#endif
