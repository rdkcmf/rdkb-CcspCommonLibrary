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

    module:	http_wcto_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Webc Trans
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/22/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_WCTO_INTERNAL_API_
#define  _HTTP_WCTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WCTO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpWctoGetBspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoSetBspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpWctoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpWctoGetWebcSession
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoSetWebcSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebcSession
    );

ANSC_HANDLE
HttpWctoGetBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoSetBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    );

ANSC_HANDLE
HttpWctoGetBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoSetBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    );

ANSC_HANDLE
HttpWctoGetTcpSimpleClient
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoSetTcpSimpleClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
HttpWctoGetTransState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoSetTransState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ANSC_STATUS
HttpWctoReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_WCTO_OPERATION.C
***********************************************************/

BOOL
HttpWctoIsLastTrans
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
HttpWctoDoLastTrans
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpWctoReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WCTO_REQUEST.C
***********************************************************/

ANSC_STATUS
HttpWctoRequest
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpWctoDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpWctoDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpWctoDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpWctoDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpWctoDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpWctoDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpWctoDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpWctoDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_WCTO_PROCESS.C
***********************************************************/

ULONG
HttpWctoQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpWctoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpWctoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

ANSC_STATUS
HttpWctoFinish
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpWctoAbort
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_WCTO_TMH.C
***********************************************************/

ANSC_STATUS
HttpWctoTmhNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpWctoTmhSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );


#endif
