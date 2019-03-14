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

    module:	http_pso_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Proxy Session
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_PSO_INTERNAL_API_
#define  _HTTP_PSO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_PSO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpPsoGetWamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoGetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoGetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoGetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpPsoGetClientSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetClientSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_HANDLE
HttpPsoGetServerSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetServerSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_HANDLE
HttpPsoGetSbcContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetSbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_HANDLE
HttpPsoGetCbcContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetCbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

char*
HttpPsoGetServerName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetServerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

PUCHAR
HttpPsoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    );

PUCHAR
HttpPsoGetServerAddr
    (
        ANSC_HANDLE                 hThisObject
    );

USHORT
HttpPsoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    );

USHORT
HttpPsoGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpPsoGetSbcPmode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetSbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    );

ULONG
HttpPsoGetCbcPmode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetCbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    );

ULONG
HttpPsoGetSessionState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoSetSessionState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ANSC_STATUS
HttpPsoReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_PSO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpPsoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_PSO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpPsoGetLastBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpPsoGetLastBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpPsoGetCurBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpPsoGetCurBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpPsoGetBmoReqByTid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

ANSC_HANDLE
HttpPsoGetBmoRepByTid
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    );

ANSC_HANDLE
HttpPsoAddNewBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpPsoAddNewBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoDelAllBmoReqs
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpPsoDelAllBmoReps
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_PSO_CLIENT.C
***********************************************************/

ANSC_HANDLE
HttpPsoGetClientBuffer
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpPsoQueryForClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoRecvFromClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoFinishedByClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoSendToClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

BOOL
HttpPsoAcceptClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpPsoAuthenticate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_PSO_SERVER.C
***********************************************************/

ANSC_HANDLE
HttpPsoGetServerBuffer
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpPsoQueryForServer
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoRecvFromServer
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoFinishedByServer
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpPsoSendToServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

ANSC_STATUS
HttpPsoConnectToServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_PSO_SERVER.C
***********************************************************/

ANSC_STATUS
HttpPsoTmhClientNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpPsoTmhClientSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );

ANSC_STATUS
HttpPsoTmhServerNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpPsoTmhServerSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );


#endif
