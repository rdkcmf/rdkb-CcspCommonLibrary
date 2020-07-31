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

    module:	stun_sco_operation.c

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Stun Simple Client Object.

        *   StunScoEngage
        *   StunScoCancel
        *   StunScoStageTimerInvoke
        *   StunScoRetryTimerInvoke
        *   StunScoProbeTimerInvoke

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/31/08    initial revision.

**********************************************************************/


#include "stun_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdp1 = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdp1;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pSimpleClientUdp1->SetHostAddress   ((ANSC_HANDLE)pSimpleClientUdp1, pProperty->ClientAddr.Dot);
    pSimpleClientUdp1->SetHostPort      ((ANSC_HANDLE)pSimpleClientUdp1, pProperty->ClientPort    );
    pSimpleClientUdp1->SetPeerAddress   ((ANSC_HANDLE)pSimpleClientUdp1, pProperty->ServerAddr.Dot);
    pSimpleClientUdp1->SetPeerPort      ((ANSC_HANDLE)pSimpleClientUdp1, pProperty->ServerPort    );
    pSimpleClientUdp1->SetMaxMessageSize((ANSC_HANDLE)pSimpleClientUdp1, STUN_MAX_MSG_SIZE        );
    pSimpleClientUdp1->SetMode          ((ANSC_HANDLE)pSimpleClientUdp1, pProperty->ClientMode    );
    pSimpleClientUdp1->Engage           ((ANSC_HANDLE)pSimpleClientUdp1);

    pMyObject->bActive = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStageTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hStageTimerObj;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pProbeTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hProbeTimerObj;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdp1 = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdp1;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    while ( pMyObject->AsyncTaskCount > 0 )
    {
        AnscSetEvent(&pMyObject->SocketEventA);
        AnscSleep(1000);
    }

    pStageTimerObj->Stop((ANSC_HANDLE)pStageTimerObj);
    pRetryTimerObj->Stop((ANSC_HANDLE)pRetryTimerObj);
    pProbeTimerObj->Stop((ANSC_HANDLE)pProbeTimerObj);

    pSimpleClientUdp1->Cancel((ANSC_HANDLE)pSimpleClientUdp1);

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoStageTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when timer is fired.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoStageTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pProbeTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hProbeTimerObj;

    switch ( pMyObject->ClientStage )
    {
        case    STUN_SCO_CLIENT_STAGE_Discovering :

                /*
                 * So the stage timer finally fired. We have either succeeded or failed in
                 * acquiring a NAT binding. The simplest way of telling is to check whether the
                 * detected NAT-ed IP address is zero.
                 */
                if ( pMyObject->BindingInfo.Ip4Addr.Value == 0 )
                {
                    break;
                }
                else
                {
                    pMyObject->ClientStage = STUN_SCO_CLIENT_STAGE_Maintaining;

                    /*
                     * STUN can be used to discover the lifetimes of the bindings created by the
                     * NAT. In many cases, the client will need to refresh the binding, either
                     * through a new STUN request, or an application packet, in order for the
                     * application to continue to use the binding. By discovering the binding life-
                     * time, the client can determine how frequently it needs to refresh.
                     */
                    returnStatus =
                        AnscSpawnTask
                            (
                                pMyObject->AsyncDiscoverTask,
                                (ANSC_HANDLE)pMyObject,
                                "stun_discover_task"
                            );

                    /*
                     * Before the best keepalive interval is determined, we shall use the minimum
                     * interval to avoid being disconnected.
                     */
                    pProperty->CurKeepAliveInterval = pProperty->MinKeepAliveInterval;

                    pProbeTimerObj->SetInterval((ANSC_HANDLE)pProbeTimerObj, pProperty->CurKeepAliveInterval * 1000);
                    pProbeTimerObj->Start      ((ANSC_HANDLE)pProbeTimerObj);
                }

                break;

        default :

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoRetryTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when timer is fired.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoRetryTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;
    PSTUN_SIMPLE_CLIENT_PROPERTY    pProperty         = (PSTUN_SIMPLE_CLIENT_PROPERTY  )&pMyObject->Property;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pRetryTimerObj    = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hRetryTimerObj;
    PANSC_SIMPLE_CLIENT_UDP_OBJECT  pSimpleClientUdp1 = (PANSC_SIMPLE_CLIENT_UDP_OBJECT)pMyObject->hSimpleClientUdp1;
    PSTUN_BSM_INTERFACE             pStunBsmIf        = (PSTUN_BSM_INTERFACE           )pMyObject->hStunBsmIf;

    /*
     * Reliability is accomplished through client retransmissions. Clients SHOULD retransmit the
     * request starting with an interval of 100ms, doubling every retransmit until the interval
     * reaches 1.6s. Retransmissions continue with intervals of 1.6s until a response is received,
     * or a total of 9 requests have been sent. If no response is received by 1.6 seconds after the
     * last request has been sent, the client SHOULD consider the transaction to have failed. In
     * other words, requests would be sent at times 0ms, 100ms, 300ms, 700ms, 1500ms, 3100ms,
     * 4700ms, 6300ms, and 7900ms. At 9500ms, the client considers the transaction to have failed
     * if no response has been received.
     */
    if ( pMyObject->SocketState1 == STUN_SCO_SOCKET_STATE_Requesting )
    {
        if ( pMyObject->RetransTime1 >= STUN_MIN_RETRANSMISSION_TIMES )
        {
            pMyObject->ClientState      = STUN_SCO_CLIENT_STATE_Idle;
            pMyObject->SocketState1     = STUN_SCO_SOCKET_STATE_Idle;
            pMyObject->RetransInterval1 = STUN_MIN_RETRANSMISSION_INTERVAL;
            pMyObject->RetransTime1     = 0;
            pMyObject->bMsgIntegrity1   = FALSE;

            if ( pMyObject->LastSendMsg1 )
            {
                AnscFreeMemory(pMyObject->LastSendMsg1);

                pMyObject->LastSendMsg1 = NULL;
                pMyObject->LastMsgSize1 = 0;
            }

            if ( pStunBsmIf )
            {
                returnStatus =
                    pStunBsmIf->Notify
                        (
                            pStunBsmIf->hOwnerContext,
                            STUN_BSM_EVENT_serverUnreachable,
                            (ANSC_HANDLE)NULL
                        );
            }

            return  ANSC_STATUS_SUCCESS;
        }

        if ( pMyObject->LastSendMsg1 )
        {
            pMyObject->RetransInterval1 = AnscGetMin2(pMyObject->RetransInterval1 * 2, STUN_MAX_RETRANSMISSION_INTERVAL);
            pMyObject->RetransTime1    += 1;
            returnStatus                =
                pSimpleClientUdp1->Send2
                    (
                        (ANSC_HANDLE)pSimpleClientUdp1,
                        pProperty->ServerAddr.Value,
                        pProperty->ServerPort,
                        pMyObject->LastSendMsg1,
                        pMyObject->LastMsgSize1,
                        (ANSC_HANDLE)NULL
                    );

            pRetryTimerObj->SetInterval((ANSC_HANDLE)pRetryTimerObj, pMyObject->RetransInterval1);
            pRetryTimerObj->Start      ((ANSC_HANDLE)pRetryTimerObj);
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        StunScoProbeTimerInvoke
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when timer is fired.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
StunScoProbeTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSTUN_SIMPLE_CLIENT_OBJECT      pMyObject         = (PSTUN_SIMPLE_CLIENT_OBJECT    )hThisObject;

    pMyObject->MaintainBinding((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
