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

    module:	http_apo_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Advanced Proxy
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/04/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_APO_INTERNAL_API_
#define  _HTTP_APO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_APO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpApoGetWamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoSetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpApoGetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoSetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpApoGetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoSetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpApoGetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoSetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpApoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ULONG
HttpApoGetProxyMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoSetProxyMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
HttpApoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpApoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpApoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_APO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpApoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoWorkerInit
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoWorkerUnload
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_APO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpApoAcquirePso
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoReleasePso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPso
    );

ANSC_STATUS
HttpApoManufacturePsoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoDestroyPsoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpApoAcquireTro
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoReleaseTro
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTro
    );

ANSC_STATUS
HttpApoManufactureTroPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoDestroyTroPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpApoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpApoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpApoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
HttpApoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpApoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_APO_WAM.C
***********************************************************/

BOOL
HttpApoWamAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

ULONG
HttpApoWamConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServerInfo
    );

ANSC_STATUS
HttpApoWamClose
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_APO_SBC.C
***********************************************************/

ULONG
HttpApoSbcRequestHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ULONG
HttpApoSbcRequestBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_APO_CBC.C
***********************************************************/

ULONG
HttpApoCbcReplyHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ULONG
HttpApoCbcReplyBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_APO_PBC.C
***********************************************************/

ANSC_STATUS
HttpApoPbcRecvReqHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpApoPbcRecvReqBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpApoPbcReqComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
HttpApoPbcRecvRepHeaders
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpApoPbcRecvRepBody
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession,
        ANSC_HANDLE                 hMessage
    );

ANSC_STATUS
HttpApoPbcRepComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_APO_PRVWO.C
***********************************************************/

BOOL
HttpApoPrvwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

ANSC_STATUS
HttpApoPrvwoSetOut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
HttpApoPrvwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
HttpApoPrvwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
HttpApoPrvwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpApoPrvwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpApoPrvwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

ANSC_STATUS
HttpApoPrvwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_APO_PUBWO.C
***********************************************************/

BOOL
HttpApoPubwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

ANSC_STATUS
HttpApoPubwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
HttpApoPubwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
HttpApoPubwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpApoPubwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpApoPubwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

ANSC_STATUS
HttpApoPubwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
