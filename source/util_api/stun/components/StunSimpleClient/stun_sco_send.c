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

    module:	stun_sco_send.c

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced message-sending
        functions of the Stun Simple Client Object.

        *   StunScoSendBindingRequest1
        *   StunScoSendBindingRequestA
        *   StunScoSendBindingRequestB

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/01/08    initial revision.

**********************************************************************/


#include "stun_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSendBindingRequest1
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the outgoing packet to be processed.

                ULONG                       ulSize
                Specifies the size of the outgoing packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSendBindingRequest1
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdp1 = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdp1;

    if ( pMyObject->LastSendMsg1 )
    {
        AnscFreeMemory(pMyObject->LastSendMsg1);

        pMyObject->LastSendMsg1 = NULL;
        pMyObject->LastMsgSize1 = 0;
    }

    pMyObject->SocketState1     = STUN_SCO_SOCKET_STATE_Requesting;
    pMyObject->RetransInterval1 = STUN_MIN_RETRANSMISSION_INTERVAL;
    pMyObject->RetransTime1     = 1;
    pMyObject->LastMsgSize1     = ulSize;
    pMyObject->LastSendMsg1     = buffer;
    returnStatus                =
        pSimpleClientUdp1->Send2
            (
                (ANSC_HANDLE)pSimpleClientUdp1,
                pProperty->ServerAddr.Value,
                pProperty->ServerPort,
                buffer,
                ulSize,
                (ANSC_HANDLE)NULL
            );

    pRetryTimerObj->SetInterval((ANSC_HANDLE)pRetryTimerObj, pMyObject->RetransInterval1);
    pRetryTimerObj->Start      ((ANSC_HANDLE)pRetryTimerObj);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSendBindingRequestA
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the outgoing packet to be processed.

                ULONG                       ulSize
                Specifies the size of the outgoing packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSendBindingRequestA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpA = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpA;

    pMyObject->SocketStateA = STUN_SCO_SOCKET_STATE_Requesting;
    returnStatus            =
        pSimpleClientUdpA->Send2
            (
                (ANSC_HANDLE)pSimpleClientUdpA,
                pProperty->ServerAddr.Value,
                pProperty->ServerPort,
                buffer,
                ulSize,
                (ANSC_HANDLE)NULL
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoSendBindingRequestB
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the outgoing packet to be processed.

                ULONG                       ulSize
                Specifies the size of the outgoing packet.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoSendBindingRequestB
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdpB = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdpB;

    pMyObject->SocketStateB = STUN_SCO_SOCKET_STATE_Requesting;
    returnStatus            =
        pSimpleClientUdpB->Send2
            (
                (ANSC_HANDLE)pSimpleClientUdpB,
                pProperty->ServerAddr.Value,
                pProperty->ServerPort,
                buffer,
                ulSize,
                (ANSC_HANDLE)NULL
            );

    return  returnStatus;
}
