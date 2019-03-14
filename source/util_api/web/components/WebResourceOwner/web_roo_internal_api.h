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

    module:	web_roo_internal_api.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Web Resource Owner
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#ifndef  _WEB_ROO_INTERNAL_API_
#define  _WEB_ROO_INTERNAL_API_


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_ROO_STATES.C
***********************************************************/

ANSC_HANDLE
WebRooGetAuthServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
WebRooGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
WebRooGetRcpIf
    (
        ANSC_HANDLE                 hThisObject
    );

char*
WebRooGetPathName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooSetPathName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    );

BOOL
WebRooIsDefaultOwner
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
WebRooSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN WEB_ROO_OPERATION.C
***********************************************************/

ANSC_STATUS
WebRooAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
WebRooReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN WEB_ROO_PROCESS.C
***********************************************************/

ULONG
WebRooQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooProcess
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN WEB_ROO_DO_STUFF.C
***********************************************************/

ANSC_STATUS
WebRooDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN WEB_ROO_DO_STUFF_UPNP.C
***********************************************************/

ANSC_STATUS
WebRooDoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoSearch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoMSearch
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoMPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoSubscribe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
WebRooDoUnsubscribe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN WEB_ROO_REPLY.C
***********************************************************/

ANSC_STATUS
WebRooReplyReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ULONG                       ulStatusCode,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply300
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply301
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply302
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply303
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply304
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply305
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply307
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply400
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply403
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply404
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply405
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply409
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply410
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply415
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply417
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply500
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply501
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply503
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
WebRooReply505
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN WEB_ROO_RCP.C
***********************************************************/

char*
WebRooRcpGetDocumentRoot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
WebRooRcpGetPathInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
WebRooRcpGetPathTranslated
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
WebRooRcpGetRemoteUser
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
WebRooRcpGetScriptName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
WebRooRcpGetServerName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

USHORT
WebRooRcpGetServerPort
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
WebRooRcpGetServerProtocol
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

char*
WebRooRcpGetServerSoftware
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );


#endif
