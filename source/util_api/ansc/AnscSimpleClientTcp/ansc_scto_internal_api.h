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

    module:	ansc_scto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Simple Client Tcp
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/11/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCTO_INTERNAL_API_
#define  _ANSC_SCTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_SCTO_STATES.C
***********************************************************/

BOOL
AnscSctoGetSocketBindToDevice
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetSocketBindToDevice
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bBind
    );

PCHAR
AnscSctoGetSocketDeviceName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetSocketDeviceName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pDeviceName
    );

PUCHAR
AnscSctoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscSctoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

PUCHAR
AnscSctoGetPeerName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetPeerName
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      peername
    );

PUCHAR
AnscSctoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscSctoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_STATUS
AnscSctoGetHostNames
    (
        ANSC_HANDLE                 hThisObject,
	ANSC_HANDLE		    hHostNames
    );

ANSC_STATUS
AnscSctoSetHostNames
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHostNames
    );

ANSC_HANDLE
AnscSctoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

ULONG
AnscSctoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscSctoGetBufferContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetBufferContext
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    );

ULONG
AnscSctoGetMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_HANDLE
AnscSctoGetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoSetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
AnscSctoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_SCTO_RECV.C
***********************************************************/

ANSC_STATUS
AnscSctoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_SCTO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscSctoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscSctoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

#ifndef _ANSC_USE_OPENSSL_
ANSC_STATUS
AnscSctoInitTlsClient
    (
        ANSC_HANDLE                 hThisObject
    );

#endif
/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_SCTO_PROCESS.C
***********************************************************/

PVOID
AnscSctoGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscSctoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

#ifndef _ANSC_USE_OPENSSL_

ANSC_STATUS
AnscSctoRecv2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPayloadBdo
    );

#endif
ANSC_STATUS
AnscSctoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscSctoSend2
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_SCTO_TSAIF.C
***********************************************************/
#ifndef _ANSC_USE_OPENSSL_


ANSC_STATUS
AnscSctoTsaRecvAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    );

ANSC_STATUS
AnscSctoTsaSendTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    );

ANSC_STATUS
AnscSctoTsaNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    );

#endif

#endif
