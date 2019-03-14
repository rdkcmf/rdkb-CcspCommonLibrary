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

    module:	ansc_dkto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Daemon Socket Tcp
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/08/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_DKTO_INTERNAL_API_
#define  _ANSC_DKTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DKTO_STATES.C
***********************************************************/

PUCHAR
AnscDktoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscDktoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_HANDLE
AnscDktoGetDaemonServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetDaemonServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServer
    );

ANSC_HANDLE
AnscDktoGetDaemonEngine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetDaemonEngine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEngine
    );

ANSC_HANDLE
AnscDktoGetClientContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetClientContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClient
    );

ULONG
AnscDktoGetBufferSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetBufferSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscDktoGetBufferContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetBufferContext
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    );

ANSC_HANDLE
AnscDktoGetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoSetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

BOOL
AnscDktoIsToBeCleaned
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoToClean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable,
        ULONG                       ulSeconds
    );

ANSC_STATUS
AnscDktoReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DKTO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscDktoFinish
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoInitTlsServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoOpenTlsServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoLock
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoUnlock
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDktoResetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DKTO_PROCESS.C
***********************************************************/

PVOID
AnscDktoGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscDktoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscDktoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_DKTO_TSAIF.C
***********************************************************/

ANSC_STATUS
AnscDktoTsaRecvAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    );

ANSC_STATUS
AnscDktoTsaSendTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    );

ANSC_STATUS
AnscDktoTsaNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    );


#endif
