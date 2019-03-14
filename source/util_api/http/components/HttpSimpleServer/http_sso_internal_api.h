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

    module:	http_sso_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Simple Server
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/07/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_SSO_INTERNAL_API_
#define  _HTTP_SSO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_SSO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpSsoGetWspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoSetWspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpSsoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpSsoGetFumIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoSetFumIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ULONG
HttpSsoGetServerMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoSetServerMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
HttpSsoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpSsoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpSsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_SSO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpSsoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoWorkerInit
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoWorkerUnload
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_SSO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpSsoAcquireWsso
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoReleaseWsso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWsso
    );

ANSC_STATUS
HttpSsoManufactureWssoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoDestroyWssoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpSsoAcquireWsto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoReleaseWsto
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWsso
    );

ANSC_STATUS
HttpSsoManufactureWstoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoDestroyWstoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpSsoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpSsoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpSsoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
HttpSsoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpSsoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_SSO_WSP.C
***********************************************************/

BOOL
HttpSsoWspAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientInfo
    );

ANSC_STATUS
HttpSsoWspServe
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWebsSession,
        ANSC_HANDLE                 hWebTrans,
        ANSC_HANDLE                 hMessage
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_SSO_DSTOWO.C
***********************************************************/

BOOL
HttpSsoDstowoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    );

ANSC_STATUS
HttpSsoDstowoSetOut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
HttpSsoDstowoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ULONG
HttpSsoDstowoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
HttpSsoDstowoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpSsoDstowoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
HttpSsoDstowoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    );

ANSC_STATUS
HttpSsoDstowoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
