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

    module:	http_tro_internal_api.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Http Trans Record
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/05/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_TRO_INTERNAL_API_
#define  _HTTP_TRO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_TRO_STATES.C
***********************************************************/

ANSC_HANDLE
HttpTroGetWamIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetWamIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpTroGetSbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetSbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpTroGetCbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpTroGetPbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetPbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpTroGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
HttpTroGetBmoReq
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetBmoReq
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    );

ANSC_HANDLE
HttpTroGetBmoRep
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetBmoRep
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage
    );

ANSC_HANDLE
HttpTroGetClientSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetClientSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_HANDLE
HttpTroGetServerSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetServerSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_HANDLE
HttpTroGetSbcContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetSbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

ANSC_HANDLE
HttpTroGetCbcContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetCbcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

char*
HttpTroGetServerName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetServerName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

PUCHAR
HttpTroGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    );

PUCHAR
HttpTroGetServerAddr
    (
        ANSC_HANDLE                 hThisObject
    );

USHORT
HttpTroGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    );

USHORT
HttpTroGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
HttpTroGetSbcPmode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetSbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    );

ULONG
HttpTroGetCbcPmode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetCbcPmode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPmode
    );

ULONG
HttpTroGetTransState
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroSetTransState
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulState
    );

ANSC_STATUS
HttpTroReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN HTTP_TRO_OPERATION.C
***********************************************************/

ANSC_STATUS
HttpTroTerminate
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroAcquireAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
HttpTroReleaseAccess
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_TRO_CLIENT.C
***********************************************************/

ULONG
HttpTroQueryForClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpTroRecvFromClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpTroFinishedByClient
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpTroSendToClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

ANSC_STATUS
HttpTroResumeTransaction
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved,
        ULONG                       ulSbcPmode
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN HTTP_TRO_SERVER.C
***********************************************************/

ULONG
HttpTroQueryForServer
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpTroRecvFromServer
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpTroFinishedByServer
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferContext
    );

ANSC_STATUS
HttpTroSendToServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulSendFlags
    );

ANSC_STATUS
HttpTroConnectToServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpTroSetUpConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
            FUNCTIONS IMPLEMENTED IN HTTP_TRO_TMH.C
***********************************************************/

ANSC_STATUS
HttpTroTmhClientNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpTroTmhClientSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );

ANSC_STATUS
HttpTroTmhServerNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
HttpTroTmhServerSerialize
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessage,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hSerializeContext
    );


#endif
