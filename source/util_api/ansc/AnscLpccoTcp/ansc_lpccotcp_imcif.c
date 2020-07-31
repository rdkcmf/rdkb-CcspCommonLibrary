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

    module:	ansc_lpccotcp_imcif.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the Ansc Lpcco Tcp Object.

        *   AnscLpccoTcpImcCallRequest
        *   AnscLpccoTcpImcCallRequest2

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/03    initial revision.

**********************************************************************/


#include "ansc_lpccotcp_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpImcCallRequest
            (
                ANSC_HANDLE                 hThisObject,
                char*                       party_name,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                ANSC_HANDLE*                phOutputBdo,
                PULONG                      pulErrorCode,
                ULONG                       ulTimeout
            );

    description:

        This function is called to issue a LPC call request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       party_name,
                Specifies the 'party_name' of the target party.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                ANSC_HANDLE*                phOutputBdo
                Specifies the output buffer descriptor of the call to
                be returned.

                PULONG                      pulErrorCode
                Specifies the erorr code of the operation.

                ULONG                       ulTimeout
                Specifies the timeout value for this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpImcCallRequest
    (
        ANSC_HANDLE                 hThisObject,
        char*                       party_name,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOutputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject         = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pBrokerServer     = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pBrokerSocket     = (PANSC_BROKER_SOCKET_TCP_OBJECT)NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr        = (PANSC_LPC_PARTY_ADDR          )NULL;
    PANSC_LPC_PARTY_ADDR            pManagerPartyAddr = (PANSC_LPC_PARTY_ADDR          )NULL;
    PIMCP_PARTY_INFO                pRepliedPartyInfo = (PIMCP_PARTY_INFO              )NULL;
    PANSC_LPCCO_PENDING_CALL        pPendingCall      = (PANSC_LPCCO_PENDING_CALL      )NULL;
    PANSC_BUFFER_DESCRIPTOR         pOutputBdo        = (PANSC_BUFFER_DESCRIPTOR       )NULL;
    ULONG                           ulCallSeqNum      = (ULONG                         )0;
    BOOL                            bSyncedCall       = TRUE;
    ANSC_EVENT                      callEvent;

    *pulErrorCode = ANSC_LPC_ERROR_noError;

    if ( ulTimeout == 0 )
    {
        ulTimeout = pMyObject->CallTimeout;
    }

    /*
     * We need to synchronize the access to the IMCP channel. Note that lock up the whole section
     * is a bit of extreme, which is probably something we can come back and optimize. For example:
     * we should be able to send another request while waiting for the reply for the first one.
     */
    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject);

    /*
     * If the specified 'party_name' is the name of the local LPC connector, we need to loopback
     * the request from a different task. Although this scenario is very unlikely in a real-world
     * deployment environment, it's still a useful feature to have.
     */

    /*
     * It seems that the considerations for a multi-stage LPC call are not necessary. The upper
     * layer can actually resolve 'party_name' to a 'party_addr' by employing some sort of protocol.
     * However, we MUST also consider the scenario where the upper layer simply uses a hardcoded
     * 'party_name' or other prior knowledge about the LPC parties.
     */
    pPendingCall = (PANSC_LPCCO_PENDING_CALL)AnscAllocateMemory(sizeof(ANSC_LPCCO_PENDING_CALL));

    if ( !pPendingCall )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT1;
    }
    else
    {
        /*pMyObject->AcqAccess((ANSC_HANDLE)pMyObject);*/
        ulCallSeqNum = ++pMyObject->SeqNumber;
        /*pMyObject->RelAccess((ANSC_HANDLE)pMyObject);*/

        AnscInitializeEvent(&callEvent);
        AnscResetEvent     (&callEvent);

        pPendingCall->PartyName     = party_name;
        pPendingCall->ImcpMsgType   = IMCP_MESSAGE_TYPE_REPLY;
        pPendingCall->CallSeqNumber = ulCallSeqNum;
        pPendingCall->CallState     = ANSC_LPC_CALL_STATE_initialized;
        pPendingCall->CallError     = ANSC_LPC_ERROR_noError;
        pPendingCall->CallEvent     = &callEvent;
        pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;
    }

    pPartyAddr =
        (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr2
            (
                (ANSC_HANDLE)pMyObject,
                party_name
            );

    /*
     * If the requested 'party_name' cannot be found in the internal party table, we then need to
     * perform name resolution by sending a QUERY_PARTY request to the central LPC manager. However
     * if the 'party_addr' of LPC manager cannot be found, we consider it's a fatal error.
     */
    if ( !pPartyAddr )
    {
        if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_SERVER )
        {
            returnStatus  = ANSC_STATUS_BAD_NAME;
            *pulErrorCode = ANSC_LPC_ERROR_badPartyName;

            goto  EXIT2;
        }
        else
        {
            pManagerPartyAddr =
                (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr
                    (
                        (ANSC_HANDLE)pMyObject,
                        ANSC_LPC_MANAGER_PARTY_NAME
                    );
        }

        if ( !pManagerPartyAddr || !pManagerPartyAddr->PartySocket )
        {
            returnStatus  = ANSC_STATUS_INTERNAL_ERROR;
            *pulErrorCode = ANSC_LPC_ERROR_managerUnavailable;

            goto  EXIT2;
        }

        /*
         * Now we enter the 'resolving' stage of the LPC call: send a QUERY_PARTY request to the
         * central LPC manager, and wait for reply. This call is added to the pending call queue
         * which will be signalled when the REPLY message is received.
         */
        pPendingCall->ImcpMsgType   = IMCP_MESSAGE_TYPE_REPLY;
        pPendingCall->CallState     = ANSC_LPC_CALL_STATE_resolving;
        pPendingCall->CallError     = ANSC_LPC_ERROR_requestTimeout;
        pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;

        returnStatus = pMyObject->AddPendingCall((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pPendingCall);
        returnStatus =
            pMyObject->SendRequest
                (
                    (ANSC_HANDLE)pMyObject,
                    pManagerPartyAddr->PartySocket,
                    party_name,
                    AnscSizeOfString(party_name),
                    ulCallSeqNum,
                    IMCP_REQUEST_TYPE_QUERY_PARTY
                );

        /*
         * Let's sleep for a while. We will be waken up when a valid REPLY message is received or
         * an error occurs. We can determine the current LPC status by examining the 'CallError'
         * field of the pending call.
         */
        AnscWaitEvent (pPendingCall->CallEvent, ulTimeout);
        AnscResetEvent(pPendingCall->CallEvent);

        if ( pPendingCall->CallError != ANSC_LPC_ERROR_noError )
        {
            if ( pPendingCall->CallError == ANSC_LPC_ERROR_requestTimeout )
            {
                AnscTrace("\n -- !!!IMCP QUERY_PARTY for %s has timed out!!! --\n",  party_name);

                pMyObject->PopPendingCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        pPendingCall->PartyName,
                        pPendingCall->ImcpMsgType,
                        pPendingCall->CallSeqNumber,
                        pPendingCall->CallError,
                        (ANSC_HANDLE)NULL
                    );
            }

            returnStatus  = ANSC_STATUS_UNAPPLICABLE;
            *pulErrorCode = pPendingCall->CallError;

            goto  EXIT2;
        }
        else if ( !pPendingCall->CallOutputBdo )
        {
            returnStatus  = ANSC_STATUS_FAILURE;
            *pulErrorCode = ANSC_LPC_ERROR_invalidReply;

            goto  EXIT2;
        }
        else
        {
            pOutputBdo        = (PANSC_BUFFER_DESCRIPTOR)pPendingCall->CallOutputBdo;
            pRepliedPartyInfo = (PIMCP_PARTY_INFO       )AnscBdoGetBlock(pOutputBdo);
            pPartyAddr        = (PANSC_LPC_PARTY_ADDR   )AnscAllocateMemory(sizeof(ANSC_LPC_PARTY_ADDR));
        }

        if ( !pPartyAddr )
        {
            returnStatus  = ANSC_STATUS_RESOURCES;
            *pulErrorCode = ANSC_LPC_ERROR_resources;

            goto  EXIT2;
        }
        else
        {
            AnscCopyMemory
                (
                    pPartyAddr->PartyName,
                    ImcpPartyInfoGetPartyName(pRepliedPartyInfo),
                    AnscBdoGetBlockSize(pOutputBdo) - sizeof(IMCP_PARTY_INFO)
                );

            pPartyAddr->StructSize      = sizeof(ANSC_LPC_PARTY_ADDR);
            pPartyAddr->RefCount        = 1;    /* not 0, to prevent existing PartyAddr from being released */
            pPartyAddr->ActiveCalls     = 0;
            pPartyAddr->Timestamp       = 0;
            pPartyAddr->PartyState      = 0;
            pPartyAddr->PartyType       = ANSC_LPC_PARTY_TYPE_client;
            pPartyAddr->PartyAddr.Value = ImcpPartyInfoGetPartyAddrValue(pRepliedPartyInfo);
            pPartyAddr->PartyProtocol   = ImcpPartyInfoGetPartyProtocol (pRepliedPartyInfo);
            pPartyAddr->PartyPort       = ImcpPartyInfoGetPartyPort     (pRepliedPartyInfo);
            pPartyAddr->PartySocket     = (ANSC_HANDLE)NULL;
            pPartyAddr->PartyReserved   = (ANSC_HANDLE)NULL;

            pMyObject->AddPartyAddr((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pPartyAddr);

            if ( pOutputBdo )
            {
                AnscFreeBdo((ANSC_HANDLE)pOutputBdo);

                pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;
            }
        }
    }

    pPartyAddr->ActiveCalls++;

    /*
     * If we come this far, we have either resolved the specified 'party_name' or the target party
     * already exists in the internal party table. The next thing we need to do is to check whether
     * we're currently connected to the target party and make a connection attempt if we are not.
     */
    if ( !pPartyAddr->PartySocket )
    {
        pBrokerSocket =
            (PANSC_BROKER_SOCKET_TCP_OBJECT)pBrokerServer->AddSocket
                (
                    (ANSC_HANDLE)pBrokerServer,
                    pPartyAddr->PartyAddr.Dot,
                    pPartyAddr->PartyPort,
                    0,
                    (ANSC_HANDLE)pPartyAddr
                );

        if ( !pBrokerSocket )
        {
            returnStatus  = ANSC_STATUS_INTERNAL_ERROR;
            *pulErrorCode = ANSC_LPC_ERROR_networkFailure;

            goto  EXIT2;
        }
        else
        {
            pPendingCall->ImcpMsgType   = IMCP_MESSAGE_TYPE_ACK;
            pPendingCall->CallSeqNumber = 0;
            pPendingCall->CallState     = ANSC_LPC_CALL_STATE_connecting;
            pPendingCall->CallError     = ANSC_LPC_ERROR_requestTimeout;
            pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;

            returnStatus = pMyObject->AddPendingCall((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pPendingCall);
            returnStatus =
                pMyObject->SendHello
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pBrokerSocket
                    );
        }

        /*
         * Let's sleep for a while. We will be waken up when a valid ACK message is received or
         * an error occurs. We can determine the current LPC status by examining the 'CallError'
         * field of the pending call.
         */
        AnscWaitEvent (pPendingCall->CallEvent, ulTimeout);
        AnscResetEvent(pPendingCall->CallEvent);

        if ( pPendingCall->CallError != ANSC_LPC_ERROR_noError )
        {
            if ( pPendingCall->CallError == ANSC_LPC_ERROR_requestTimeout )
            {
                AnscTrace("\n -- !!!IMCP HELLO to %s has timed out!!! --\n",  party_name);

                pMyObject->PopPendingCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        pPendingCall->PartyName,
                        pPendingCall->ImcpMsgType,
                        pPendingCall->CallSeqNumber,
                        pPendingCall->CallError,
                        (ANSC_HANDLE)NULL
                    );
            }

            returnStatus  = ANSC_STATUS_UNAPPLICABLE;
            *pulErrorCode = pPendingCall->CallError;

            goto  EXIT3;
        }
        else if ( !pPendingCall->CallOutputBdo )
        {
            returnStatus  = ANSC_STATUS_FAILURE;
            *pulErrorCode = ANSC_LPC_ERROR_invalidReply;

            goto  EXIT3;
        }
        else
        {
            /*
             * Is there a need to verify the party name in the ACKed message?
             */
            AnscFreeBdo(pPendingCall->CallOutputBdo);

            pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;
        }

        pPartyAddr->PartySocket = (ANSC_HANDLE)pBrokerSocket;
    }

    /*
     * We need to synchronize the access to the IMCP channel. Note that lock up the whole section
     * is a bit of extreme, which is probably something we can come back and optimize. For example:
     * we should be able to send another request while waiting for the reply for the first one.
     */
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject);

    bSyncedCall = FALSE;

    /*
     * We have either successfully established a LPC connection with the target party or the LPC
     * connection has existed for quite some time, now it's time to issue the LPC call.
     */
    if ( TRUE )
    {
        pPendingCall->ImcpMsgType   = IMCP_MESSAGE_TYPE_REPLY;
        pPendingCall->CallSeqNumber = ulCallSeqNum;
        pPendingCall->CallState     = ANSC_LPC_CALL_STATE_calling;
        pPendingCall->CallError     = ANSC_LPC_ERROR_requestTimeout;
        pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;

        returnStatus = pMyObject->AddPendingCall((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pPendingCall);
        returnStatus =
            pMyObject->SendRequest
                (
                    (ANSC_HANDLE)pMyObject,
                    pPartyAddr->PartySocket,
                    pInputBuffer,
                    ulInputSize,
                    ulCallSeqNum,
                    IMCP_REQUEST_TYPE_LPC_CALL
                );

        /*
         * Let's sleep for a while. We will be waken up when a valid REPLY message is received or
         * an error occurs. We can determine the current LPC status by examining the 'CallError'
         * field of the pending call.
         */
        AnscWaitEvent (pPendingCall->CallEvent, ulTimeout);
        AnscResetEvent(pPendingCall->CallEvent);

        if ( pPendingCall->CallError != ANSC_LPC_ERROR_noError )
        {
            if ( pPendingCall->CallError == ANSC_LPC_ERROR_requestTimeout )
            {
                AnscTrace("\n -- !!!IMCP LPC_CALL to %s has timed out!!! --\n",  party_name);

                pMyObject->PopPendingCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        pPendingCall->PartyName,
                        pPendingCall->ImcpMsgType,
                        pPendingCall->CallSeqNumber,
                        pPendingCall->CallError,
                        (ANSC_HANDLE)NULL
                    );
            }

            returnStatus  = ANSC_STATUS_UNAPPLICABLE;
            *pulErrorCode = pPendingCall->CallError;

            goto  EXIT3;
        }
        else if ( !pPendingCall->CallOutputBdo )
        {
            returnStatus  = ANSC_STATUS_FAILURE;
            *pulErrorCode = ANSC_LPC_ERROR_invalidReply;

            goto  EXIT3;
        }
        else
        {
            *phOutputBdo  = pPendingCall->CallOutputBdo;
            *pulErrorCode = ANSC_LPC_ERROR_noError;

            pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT3:

    if ( pBrokerSocket )
    {
        /*
        pMyObject->SendBye
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBrokerSocket
            );

        pBrokerServer->DelSocket((ANSC_HANDLE)pBrokerServer, (ANSC_HANDLE)pBrokerSocket);

        pPartyAddr->PartySocket = (ANSC_HANDLE)NULL;
        */
    }

EXIT2:

    if ( pPartyAddr )
    {
        pPartyAddr->RefCount--;
        pPartyAddr->ActiveCalls--;
    }

    if ( pPendingCall )
    {
        if ( pPendingCall->CallOutputBdo )
        {
            AnscFreeBdo(pPendingCall->CallOutputBdo);
        }

        AnscFreeEvent (pPendingCall->CallEvent);
        AnscFreeMemory(pPendingCall);
    }

EXIT1:

    if ( bSyncedCall )
    {
        pMyObject->RelAccess((ANSC_HANDLE)pMyObject);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpImcCallRequest2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPartyAddr,
                void*                       pInputBuffer,
                ULONG                       ulInputSize,
                ANSC_HANDLE*                phOutputBdo,
                PULONG                      pulErrorCode,
                ULONG                       ulTimeout
            );

    description:

        This function is called to issue a LPC call request.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPartyAddr
                Specifies the 'party_addr' of the target party.

                void*                       pInputBuffer
                Specifies the input buffer of the call.

                ULONG                       ulInputSize
                Specifies the size of the input buffer.

                ANSC_HANDLE*                phOutputBdo
                Specifies the output buffer descriptor of the call to
                be returned.

                PULONG                      pulErrorCode
                Specifies the erorr code of the operation.

                ULONG                       ulTimeout
                Specifies the timeout value for this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpImcCallRequest2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPartyAddr,
        void*                       pInputBuffer,
        ULONG                       ulInputSize,
        ANSC_HANDLE*                phOutputBdo,
        PULONG                      pulErrorCode,
        ULONG                       ulTimeout
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject         = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pBrokerServer     = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pBrokerSocket     = (PANSC_BROKER_SOCKET_TCP_OBJECT)NULL;
    PANSC_LPC_PARTY_ADDR            pPartyAddr        = (PANSC_LPC_PARTY_ADDR          )hPartyAddr;
    PANSC_LPCCO_PENDING_CALL        pPendingCall      = (PANSC_LPCCO_PENDING_CALL      )NULL;
    ULONG                           ulCallSeqNum      = (ULONG                         )0;
    BOOL                            bSyncedCall       = TRUE;
    ANSC_EVENT                      callEvent;

    *pulErrorCode = ANSC_LPC_ERROR_noError;

    if ( ulTimeout == 0 )
    {
        ulTimeout = pMyObject->CallTimeout;
    }

    if ( !pPartyAddr )
    {
        returnStatus  = ANSC_STATUS_INTERNAL_ERROR;
        *pulErrorCode = ANSC_LPC_ERROR_internalError;

        goto  EXIT1;
    }
    else if ( (pPartyAddr->Timestamp   != 0                                ) &&
              (pPartyAddr->PartySocket == (ANSC_HANDLE)NULL                ) &&
             !(pPartyAddr->PartyState  &  ANSC_LPC_PARTY_STATE_connectedIn ) &&
             !(pPartyAddr->PartyState  &  ANSC_LPC_PARTY_STATE_connectedOut) )
    {
        returnStatus  = ANSC_STATUS_FAILURE;
        *pulErrorCode = ANSC_LPC_ERROR_partyDisconnected;

        goto  EXIT1;
    }

    /*
     * We need to synchronize the access to the IMCP channel. Note that lock up the whole section
     * is a bit of extreme, which is probably something we can come back and optimize. For example:
     * we should be able to send another request while waiting for the reply for the first one.
     */
    pMyObject->AcqAccess((ANSC_HANDLE)pMyObject);

    /*
     * If the specified 'party_name' is the name of the local LPC connector, we need to loopback
     * the request from a different task. Although this scenario is very unlikely in a real-world
     * deployment environment, it's still a useful feature to have.
     */

    /*
     * It seems that the considerations for a multi-stage LPC call are not necessary. The upper
     * layer can actually resolve 'party_name' to a 'party_addr' by employing some sort of protocol.
     * However, we MUST also consider the scenario where the upper layer simply uses a hardcoded
     * 'party_name' or other prior knowledge about the LPC parties.
     */
    pPendingCall = (PANSC_LPCCO_PENDING_CALL)AnscAllocateMemory(sizeof(ANSC_LPCCO_PENDING_CALL));

    if ( !pPendingCall )
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto  EXIT1;
    }
    else
    {
        /*pMyObject->AcqAccess((ANSC_HANDLE)pMyObject);*/
        ulCallSeqNum = ++pMyObject->SeqNumber;
        /*pMyObject->RelAccess((ANSC_HANDLE)pMyObject);*/

        AnscInitializeEvent(&callEvent);
        AnscResetEvent     (&callEvent);

        pPendingCall->PartyName     = pPartyAddr->PartyName;
        pPendingCall->ImcpMsgType   = IMCP_MESSAGE_TYPE_REPLY;
        pPendingCall->CallSeqNumber = ulCallSeqNum;
        pPendingCall->CallState     = ANSC_LPC_CALL_STATE_initialized;
        pPendingCall->CallError     = ANSC_LPC_ERROR_noError;
        pPendingCall->CallEvent     = &callEvent;
        pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;
    }

    pPartyAddr->ActiveCalls++;

    /*
     * If we come this far, we have either resolved the specified 'party_name' or the target party
     * already exists in the internal party table. The next thing we need to do is to check whether
     * we're currently connected to the target party and make a connection attempt if we are not.
     */
    if ( !pPartyAddr->PartySocket )
    {
        pBrokerSocket =
            (PANSC_BROKER_SOCKET_TCP_OBJECT)pBrokerServer->AddSocket
                (
                    (ANSC_HANDLE)pBrokerServer,
                    pPartyAddr->PartyAddr.Dot,
                    pPartyAddr->PartyPort,
                    0,
                    (ANSC_HANDLE)pPartyAddr
                );

        if ( !pBrokerSocket )
        {
            returnStatus  = ANSC_STATUS_INTERNAL_ERROR;
            *pulErrorCode = ANSC_LPC_ERROR_networkFailure;

            goto  EXIT2;
        }
        else
        {
            pPendingCall->ImcpMsgType   = IMCP_MESSAGE_TYPE_ACK;
            pPendingCall->CallSeqNumber = 0;
            pPendingCall->CallState     = ANSC_LPC_CALL_STATE_connecting;
            pPendingCall->CallError     = ANSC_LPC_ERROR_requestTimeout;
            pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;

            returnStatus = pMyObject->AddPendingCall((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pPendingCall);
            returnStatus =
                pMyObject->SendHello
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pBrokerSocket
                    );
        }

        /*
         * Let's sleep for a while. We will be waken up when a valid ACK message is received or
         * an error occurs. We can determine the current LPC status by examining the 'CallError'
         * field of the pending call.
         */
        AnscWaitEvent (pPendingCall->CallEvent, ulTimeout);
        AnscResetEvent(pPendingCall->CallEvent);

        if ( pPendingCall->CallError != ANSC_LPC_ERROR_noError )
        {
            if ( pPendingCall->CallError == ANSC_LPC_ERROR_requestTimeout )
            {
                AnscTrace("\n -- !!!IMCP HELLO to %s has timed out!!! --\n",  pPartyAddr->PartyName);

                pMyObject->PopPendingCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        pPendingCall->PartyName,
                        pPendingCall->ImcpMsgType,
                        pPendingCall->CallSeqNumber,
                        pPendingCall->CallError,
                        (ANSC_HANDLE)NULL
                    );
            }

            returnStatus  = ANSC_STATUS_UNAPPLICABLE;
            *pulErrorCode = pPendingCall->CallError;

            goto  EXIT3;
        }
        else if ( !pPendingCall->CallOutputBdo )
        {
            returnStatus  = ANSC_STATUS_FAILURE;
            *pulErrorCode = ANSC_LPC_ERROR_invalidReply;

            goto  EXIT3;
        }
        else
        {
            /*
             * Is there a need to verify the party name in the ACKed message?
             */
            AnscFreeBdo(pPendingCall->CallOutputBdo);

            pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;
        }

        pPartyAddr->PartySocket = (ANSC_HANDLE)pBrokerSocket;
    }

    /*
     * We need to synchronize the access to the IMCP channel. Note that lock up the whole section
     * is a bit of extreme, which is probably something we can come back and optimize. For example:
     * we should be able to send another request while waiting for the reply for the first one.
     */
    pMyObject->RelAccess((ANSC_HANDLE)pMyObject);

    bSyncedCall = FALSE;

    /*
     * We have either successfully established a LPC connection with the target party or the LPC
     * connection has existed for quite some time, now it's time to issue the LPC call.
     */
    if ( TRUE )
    {
        pPendingCall->ImcpMsgType   = IMCP_MESSAGE_TYPE_REPLY;
        pPendingCall->CallSeqNumber = ulCallSeqNum;
        pPendingCall->CallState     = ANSC_LPC_CALL_STATE_calling;
        pPendingCall->CallError     = ANSC_LPC_ERROR_requestTimeout;
        pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;

        returnStatus = pMyObject->AddPendingCall((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pPendingCall);
        returnStatus =
            pMyObject->SendRequest
                (
                    (ANSC_HANDLE)pMyObject,
                    pPartyAddr->PartySocket,
                    pInputBuffer,
                    ulInputSize,
                    ulCallSeqNum,
                    IMCP_REQUEST_TYPE_LPC_CALL
                );

        /*
         * Let's sleep for a while. We will be waken up when a valid REPLY message is received or
         * an error occurs. We can determine the current LPC status by examining the 'CallError'
         * field of the pending call.
         */
        AnscWaitEvent (pPendingCall->CallEvent, ulTimeout);
        AnscResetEvent(pPendingCall->CallEvent);

        if ( pPendingCall->CallError != ANSC_LPC_ERROR_noError )
        {
            if ( pPendingCall->CallError == ANSC_LPC_ERROR_requestTimeout )
            {
                AnscTrace("\n -- !!!IMCP LPC_CALL to %s has timed out!!! --\n",  pPartyAddr->PartyName);

                pMyObject->PopPendingCall
                    (
                        (ANSC_HANDLE)pMyObject,
                        pPendingCall->PartyName,
                        pPendingCall->ImcpMsgType,
                        pPendingCall->CallSeqNumber,
                        pPendingCall->CallError,
                        (ANSC_HANDLE)NULL
                    );
            }

            returnStatus  = ANSC_STATUS_UNAPPLICABLE;
            *pulErrorCode = pPendingCall->CallError;

            goto  EXIT3;
        }
        else if ( !pPendingCall->CallOutputBdo )
        {
            returnStatus  = ANSC_STATUS_FAILURE;
            *pulErrorCode = ANSC_LPC_ERROR_invalidReply;

            goto  EXIT3;
        }
        else
        {
            *phOutputBdo  = pPendingCall->CallOutputBdo;
            *pulErrorCode = ANSC_LPC_ERROR_noError;

            pPendingCall->CallOutputBdo = (ANSC_HANDLE)NULL;
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT3:

    if ( pBrokerSocket )
    {
        /*
        pMyObject->SendBye
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pBrokerSocket
            );

        pBrokerServer->DelSocket((ANSC_HANDLE)pBrokerServer, (ANSC_HANDLE)pBrokerSocket);

        pPartyAddr->PartySocket = (ANSC_HANDLE)NULL;
        */
    }

EXIT2:

    if ( pPendingCall )
    {
        if ( pPendingCall->CallOutputBdo )
        {
            AnscFreeBdo(pPendingCall->CallOutputBdo);
        }

        AnscFreeEvent (pPendingCall->CallEvent);
        AnscFreeMemory(pPendingCall);
    }

    if ( pPartyAddr )
    {
        pPartyAddr->ActiveCalls--;
    }

EXIT1:

    if ( bSyncedCall )
    {
        pMyObject->RelAccess((ANSC_HANDLE)pMyObject);
    }

    return  returnStatus;
}
