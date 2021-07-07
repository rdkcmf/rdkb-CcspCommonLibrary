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

    module:	http_sco_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Simple Client
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


#ifndef  _HTTP_SCO_INTERNAL_API_
#define  _HTTP_SCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_SCO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpScoGetBspIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoSetBspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpScoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpScoGetCasIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpScoGetClientAuthObj
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpScoGetClientMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoSetClientMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

char*
HttpScoGetProductName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoSetProductName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
HttpScoGetHostNames
    (
        ANSC_HANDLE                 hThisObject,
	ANSC_HANDLE		    hHostNames
    );

ANSC_STATUS
HttpScoSetHostNames
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostNames
    );

ANSC_STATUS
HttpScoSetSessionIdleTimeout
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTimeout
    );

ANSC_STATUS
HttpScoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpScoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
HttpScoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoReset
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoGetCredential
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pHostName,
        USHORT                      HostPort,
        PUCHAR                      pUriPath,
        PUCHAR*                     ppUserName,
        PUCHAR*                     ppPassword
    );

/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_SCO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpScoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoSessionTdoInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_SCO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
HttpScoAcquireWcso
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoReleaseWcso
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWcso
    );

ANSC_STATUS
HttpScoManufactureWcsoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoDestroyWcsoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpScoAcquireWcto
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoReleaseWcto
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWcso
    );

ANSC_STATUS
HttpScoManufactureWctoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoDestroyWctoPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpScoAcquireBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoReleaseBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq
    );

ANSC_STATUS
HttpScoManufactureBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoDestroyBmoReqPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
HttpScoAcquireBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoReleaseBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoRep
    );

ANSC_STATUS
HttpScoManufactureBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoDestroyBmoRepPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoSetPeerAddresses
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPeerAddresses,
        PBOOL                       pbSameAsCurrent
    );

BOOL
HttpScoGetNextPeerAddr
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                   *pPeerAddr
    );

ULONG
HttpScoGetCurPeerAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoResetPeerAddrPick
    (
        ANSC_HANDLE                 hThisObject,
        uint32_t                    ipAddr
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_SCO_BSPIF.C
***********************************************************/

ANSC_STATUS
HttpScoBspPolish
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpScoBspBrowse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpScoBspNotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hReqContext,
        ANSC_HANDLE                 hBmoReq,
        ANSC_HANDLE                 hBmoRep
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN HTTP_SCO_REQUEST.C
***********************************************************/

ANSC_STATUS
HttpScoRequest
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoRequest2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       request_line,
        ANSC_HANDLE                 hReqContext
    );

ANSC_STATUS
HttpScoRequest3
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMethod,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls,
        char*                       pUserName,
        char*                       pPassword
    );

ANSC_STATUS
HttpScoDoOptions
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDoGet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDoHead
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDoPost
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDoPut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDoDelete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDoTrace
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDoConnect
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReqUri,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_SCO_ACCESS.C
***********************************************************/

ANSC_HANDLE
HttpScoMapWcso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_HANDLE
HttpScoGetWcso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_HANDLE
HttpScoAddWcso
    (
        ANSC_HANDLE                 hThisObject,
        char*                       host,
        USHORT                      port,
        ANSC_HANDLE                 hReqContext,
        BOOL                        bUseTls
    );

ANSC_STATUS
HttpScoDelAllWcsos
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN HTTP_SCO_CASIF.C
***********************************************************/

ANSC_STATUS
HttpScoCasEnableAuth
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    );

BOOL
HttpScoCasIsAuthEnabled
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpScoCasProcessRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest
    );

ANSC_STATUS
HttpScoCasProcessResponse
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRequest,
        ANSC_HANDLE                 hResponse,
        ANSC_HANDLE                 hWebcTransObj
    );

ANSC_STATUS
HttpScoCasClearAuthInfo
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
