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

    module:	http_wcso_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Webc Session
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/21/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_WCSO_INTERNAL_API_
#define  _HTTP_WCSO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WCSO_STATES.C
***********************************************************/

char*
HttpWcsoGetPeerName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetPeerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

USHORT
HttpWcsoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

PUCHAR
HttpWcsoGetHostAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetHostAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
HttpWcsoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      port
    );

ULONG
HttpWcsoGetSessionFlags
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetSessionFlags
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );

ANSC_HANDLE
HttpWcsoGetBspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetBspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpWcsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ULONG
HttpWcsoGetSessionState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ANSC_STATUS
HttpWcsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpWcsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpWcsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoReset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoReturn
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_WCSO_OPERATION.C
***********************************************************/

BOOL
HttpWcsoMatchServer
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        USHORT                      port,
        ULONG                       flags
    );

BOOL
HttpWcsoIsReadyToRequest
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_WCSO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpWcsoGetEndWcto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpWcsoGetCurWcto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpWcsoAddNewWcto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoDelAllWctos
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WCSO_REQUEST.C
***********************************************************/

ANSC_STATUS
HttpWcsoConnect
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWcsoRequest
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWcsoConnectPeer
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                    ipAddrToConnect
    );

ANSC_STATUS
HttpWcsoConnectionFailed
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_WCSO_SCTOWO.C
***********************************************************/

ULONG
HttpWcsoSctowoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpWcsoSctowoProcess
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
HttpWcsoSctowoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpWcsoReqRetryTask
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
