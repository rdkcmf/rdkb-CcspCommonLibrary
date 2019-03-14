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

    module:	http_wsto_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Webs Trans
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_WSTO_INTERNAL_API_
#define  _HTTP_WSTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WSTO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpWstoGetWspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetWspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpWstoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpWstoGetWebsSession
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetWebsSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsSession
    );

ANSC_HANDLE
HttpWstoGetBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    );

ANSC_HANDLE
HttpWstoGetBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    );

ANSC_HANDLE
HttpWstoGetWebSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetWebSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

char*
HttpWstoGetServerName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetServerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

PUCHAR
HttpWstoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    );

USHORT
HttpWstoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpWstoGetTransState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoSetTransState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ANSC_STATUS
HttpWstoReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_WSTO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpWstoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWstoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_WSTO_CLIENT.C
***********************************************************/

ULONG
HttpWstoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpWstoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpWstoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

ANSC_STATUS
HttpWstoFinish
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_WSTO_TMH.C
***********************************************************/

ANSC_STATUS
HttpWstoTmhNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpWstoTmhSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );


#endif
