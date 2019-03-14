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

    module:	stun_sco_internal_api.h

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the Stun Simple Client
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/30/08    initial revision.

**********************************************************************/


#ifndef  _STUN_SCO_INTERNAL_API_
#define  _STUN_SCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN STUN_SCO_STATES.C
***********************************************************/

ANSC_HANDLE
StunScoGetStunBsmIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetStunBsmIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ULONG
StunScoGetClientAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetClientAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIp4Addr
    );

USHORT
StunScoGetClientPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetClientPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usUdpPort
    );

ULONG
StunScoGetClientMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetClientMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ULONG
StunScoGetServerAddr
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetServerAddr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIp4Addr
    );

USHORT
StunScoGetServerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetServerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usUdpPort
    );

char*
StunScoGetUsername
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetUsername
    (
        ANSC_HANDLE                 hThisObject,
        char*                       username
    );

char*
StunScoGetPassword
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetPassword
    (
        ANSC_HANDLE                 hThisObject,
        char*                       password
    );

int
StunScoGetMinKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetMinKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject,
        int                         iSeconds
    );

int
StunScoGetMaxKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoSetMaxKeepAliveInterval
    (
        ANSC_HANDLE                 hThisObject,
        int                         iSeconds
    );

ANSC_STATUS
StunScoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
StunScoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
StunScoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN STUN_SCO_OPERATION.C
***********************************************************/

ANSC_STATUS
StunScoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoStageTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoRetryTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoProbeTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN STUN_SCO_ACTION.C
***********************************************************/

ANSC_STATUS
StunScoPlayRole1
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoDiscoverBinding
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoDiscoverBindingTimeoutA
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoDiscoverBindingTimeoutB
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoMaintainBinding
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoNotifyBindingChange
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
StunScoAsyncDiscoverTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN STUN_SCO_RECV.C
***********************************************************/

ANSC_STATUS
StunScoRecvBindingResponse1
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoRecvBindingResponseA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoRecvBindingResponseB
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoRecvBindingErrorResponse1
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoRecvBindingErrorResponseA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoRecvBindingErrorResponseB
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

BOOL
StunScoVerifyMsgIntegrity
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN STUN_SCO_SEND.C
***********************************************************/

ANSC_STATUS
StunScoSendBindingRequest1
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoSendBindingRequestA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoSendBindingRequestB
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN STUN_SCO_SUCOWO1.C
***********************************************************/

ULONG
StunScoScuowo1Query
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoScuowo1Process
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoScuowo1Notify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN STUN_SCO_SUCOWOA.C
***********************************************************/

ULONG
StunScoScuowoAQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoScuowoAProcess
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoScuowoANotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN STUN_SCO_SUCOWOB.C
***********************************************************/

ULONG
StunScoScuowoBQuery
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoScuowoBProcess
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
StunScoScuowoBNotify
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    );


#endif
