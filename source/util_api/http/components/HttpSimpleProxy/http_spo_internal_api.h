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

    module:	http_spo_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Simple Proxy
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/21/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_SPO_INTERNAL_API_
#define  _HTTP_SPO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_SPO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpSpoGetWamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoSetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpSpoGetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoSetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpSpoGetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoSetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpSpoGetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoSetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpSpoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ULONG
HttpSpoGetProxyMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoSetProxyMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
HttpSpoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpSpoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpSpoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_SPO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpSpoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoWorkerInit
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoWorkerUnload
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_SPO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpSpoAcquirePso
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoReleasePso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPso
    );

ANSC_STATUS
HttpSpoManufacturePsoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoDestroyPsoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpSpoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpSpoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpSpoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
HttpSpoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSpoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_SPO_WAM.C
***********************************************************/

BOOL
HttpSpoWamAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

ULONG
HttpSpoWamConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerInfo
    );

ANSC_STATUS
HttpSpoWamClose
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_SPO_SBC.C
***********************************************************/

ULONG
HttpSpoSbcRequestHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ULONG
HttpSpoSbcRequestBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_SPO_CBC.C
***********************************************************/

ULONG
HttpSpoCbcReplyHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ULONG
HttpSpoCbcReplyBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_SPO_PBC.C
***********************************************************/

ANSC_STATUS
HttpSpoPbcRecvReqHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpSpoPbcRecvReqBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpSpoPbcReqComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
HttpSpoPbcRecvRepHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpSpoPbcRecvRepBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpSpoPbcRepComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_SPO_PRVWO.C
***********************************************************/

BOOL
HttpSpoPrvwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

ANSC_STATUS
HttpSpoPrvwoSetOut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
HttpSpoPrvwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
HttpSpoPrvwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
HttpSpoPrvwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpSpoPrvwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpSpoPrvwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

ANSC_STATUS
HttpSpoPrvwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_SPO_PUBWO.C
***********************************************************/

BOOL
HttpSpoPubwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

ANSC_STATUS
HttpSpoPubwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
HttpSpoPubwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
HttpSpoPubwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpSpoPubwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpSpoPubwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

ANSC_STATUS
HttpSpoPubwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
