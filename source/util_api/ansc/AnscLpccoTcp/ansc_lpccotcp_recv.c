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

    module:	ansc_lpccotcp_recv.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Lpcco Tcp Object.

        *   AnscLpccoTcpRecv
        *   AnscLpccoTcpRecvHello
        *   AnscLpccoTcpRecvAck
        *   AnscLpccoTcpRecvRequest
        *   AnscLpccoTcpRecvReply
        *   AnscLpccoTcpRecvBye

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/26/03    initial revision.

**********************************************************************/


#include "ansc_lpccotcp_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpRecv
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hBufferDesp
            );

    description:

        This function is called to process an incoming IMCP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which the message is
                received.

                ANSC_HANDLE                 hBufferDesp
                Specifies the incoming message buffer to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject     = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )hBufferDesp;
    PIMCP_HEADER                    pImcpHeader   = (PIMCP_HEADER                  )AnscBdoGetBlock(pBufferDesp);

    switch ( ImcpGetMsgType(pImcpHeader) )
    {
        case    IMCP_MESSAGE_TYPE_HELLO :

                returnStatus =
                    pMyObject->RecvHello
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSocket,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    IMCP_MESSAGE_TYPE_ACK :

                returnStatus =
                    pMyObject->RecvAck
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSocket,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    IMCP_MESSAGE_TYPE_REQUEST :

                returnStatus =
                    pMyObject->RecvRequest
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSocket,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    IMCP_MESSAGE_TYPE_REPLY :

                returnStatus =
                    pMyObject->RecvReply
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSocket,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        case    IMCP_MESSAGE_TYPE_BYE :

                returnStatus =
                    pMyObject->RecvBye
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSocket,
                            (ANSC_HANDLE)pBufferDesp
                        );

                break;

        default :

                AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

                returnStatus = ANSC_STATUS_DISCARD;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpRecvHello
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hBufferDesp
            );

    description:

        This function is called to process an incoming IMCP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which the message is
                received.

                ANSC_HANDLE                 hBufferDesp
                Specifies the incoming message buffer to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpRecvHello
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject         = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket     = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_LPC_PARTY_ADDR            pPartyAddr        = (PANSC_LPC_PARTY_ADDR          )NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp       = (PANSC_BUFFER_DESCRIPTOR       )hBufferDesp;
    PIMCP_HEADER                    pImcpHeader       = (PIMCP_HEADER                  )AnscBdoGetBlock(pBufferDesp);
    PIMCP_HELLO_MESSAGE             pImcpHelloMessage = (PIMCP_HELLO_MESSAGE           )ImcpGetMsgData  (pImcpHeader);
    ULONG                           ulPartyNameSize   = (ULONG                         )ImcpGetMsgLength(pImcpHeader) - sizeof(IMCP_HELLO_MESSAGE);
    char*                           party_name        = (char*                         )AnscAllocateMemory(ANSC_OBJECT_NAME_SIZE + 1);
    errno_t                         rc                = -1;

    if ( !party_name )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    AnscCopyMemory(party_name, ImcpHelloGetPartyName(pImcpHelloMessage), AnscGetMin2(ulPartyNameSize, ANSC_OBJECT_NAME_SIZE));
/*
#ifdef   _DEBUG
    if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_SERVER )
    {
        AnscTrace("\n -- !!!AnscLpccoTcp: HELLO is received from %s!!! --\n",  party_name);
    }
#endif
*/
    pPartyAddr =
        (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr2
            (
                (ANSC_HANDLE)pMyObject,
                party_name
            );

    if ( pPartyAddr )
    {
        if ( !(pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedIn) )
        {
            pPartyAddr->Timestamp   = 0;
            pPartyAddr->PartyState |= ANSC_LPC_PARTY_STATE_connectedIn;
        }
    }
    else
    {
        pPartyAddr = (PANSC_LPC_PARTY_ADDR)AnscAllocateMemory(sizeof(ANSC_LPC_PARTY_ADDR));

        if ( pPartyAddr )
        {
            rc = strcpy_s(pPartyAddr->PartyName, sizeof(pPartyAddr->PartyName), party_name);
            ERR_CHK(rc);

            pPartyAddr->StructSize      = sizeof(ANSC_LPC_PARTY_ADDR);
            pPartyAddr->RefCount        = 1;
            pPartyAddr->Timestamp       = 0;
            pPartyAddr->PartyState      = ANSC_LPC_PARTY_STATE_connectedIn;
            pPartyAddr->PartyType       = ANSC_LPC_PARTY_TYPE_client;
            pPartyAddr->PartyAddr.Value = ImcpHelloGetPartyAddrValue(pImcpHelloMessage);
            pPartyAddr->PartyPort       = ImcpHelloGetPartyPort     (pImcpHelloMessage);
            pPartyAddr->PartyProtocol   = TCP_TRANSPORT;
            pPartyAddr->PartySocket     = (ANSC_HANDLE)NULL;
            pPartyAddr->PartyReserved   = (ANSC_HANDLE)NULL;

            pMyObject->AddPartyAddr((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pPartyAddr);
        }
    }

    pDaemonSocket->SetClientContext((ANSC_HANDLE)pDaemonSocket, (ANSC_HANDLE)pPartyAddr);

    if ( pPartyAddr )
    {
        pPartyAddr->RefCount--;
    }

    AnscFreeBdo(pBufferDesp);

        pMyObject->SendAck
            (
                (ANSC_HANDLE)pMyObject,
                hSocket
            );

    AnscFreeMemory(party_name);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpRecvAck
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hBufferDesp
            );

    description:

        This function is called to process an incoming IMCP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which the message is
                received.

                ANSC_HANDLE                 hBufferDesp
                Specifies the incoming message buffer to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpRecvAck
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    )
{
    UNREFERENCED_PARAMETER(hSocket);
    PANSC_LPCCO_TCP_OBJECT          pMyObject       = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_LPC_PARTY_ADDR            pPartyAddr      = (PANSC_LPC_PARTY_ADDR          )NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp     = (PANSC_BUFFER_DESCRIPTOR       )hBufferDesp;
    PIMCP_HEADER                    pImcpHeader     = (PIMCP_HEADER                  )AnscBdoGetBlock(pBufferDesp);
    PIMCP_ACK_MESSAGE               pImcpAckMessage = (PIMCP_ACK_MESSAGE             )ImcpGetMsgData(pImcpHeader);
    ULONG                           ulPartyNameSize = (ULONG                         )ImcpGetMsgLength(pImcpHeader) - sizeof(IMCP_ACK_MESSAGE);
    char*                           party_name      = (char*                         )AnscAllocateMemory(ANSC_OBJECT_NAME_SIZE + 1);

    if ( !party_name )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    AnscCopyMemory(party_name, ImcpAckGetPartyName(pImcpAckMessage), AnscGetMin2(ulPartyNameSize, ANSC_OBJECT_NAME_SIZE));
/*
#ifdef   _DEBUG
    if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_SERVER )
    {
        AnscTrace("\n -- !!!AnscLpccoTcp: ACK is received from %s!!! --\n",  party_name);
    }
#endif
*/
    pPartyAddr =
        (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr2
            (
                (ANSC_HANDLE)pMyObject,
                party_name
            );

    if ( pPartyAddr )
    {
        pPartyAddr->PartyState |= ANSC_LPC_PARTY_STATE_connectedOut;
        pPartyAddr->RefCount--;
    }

    if ( TRUE )
    {
        AnscBdoShrinkRight(pBufferDesp, sizeof(IMCP_HEADER));

            (PANSC_LPCCO_PENDING_CALL)pMyObject->PopPendingCall
                (
                    (ANSC_HANDLE)pMyObject,
                    party_name,
                    IMCP_MESSAGE_TYPE_ACK,
                    0,
                    ANSC_LPC_ERROR_noError,
                    (ANSC_HANDLE)pBufferDesp
                );
    }

    AnscFreeMemory(party_name);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpRecvRequest
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hBufferDesp
            );

    description:

        This function is called to process an incoming IMCP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which the message is
                received.

                ANSC_HANDLE                 hBufferDesp
                Specifies the incoming message buffer to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpRecvRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject           = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_CPC_INTERFACE             pAnscCpcIf          = (PANSC_CPC_INTERFACE           )pMyObject->hAnscCpcIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket       = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_LPC_PARTY_ADDR            pPartyAddr          = (PANSC_LPC_PARTY_ADDR          )pDaemonSocket->GetClientContext((ANSC_HANDLE)pDaemonSocket);
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp         = (PANSC_BUFFER_DESCRIPTOR       )hBufferDesp;
    PIMCP_HEADER                    pImcpHeader         = (PIMCP_HEADER                  )AnscBdoGetBlock(pBufferDesp);
    PIMCP_REQUEST_MESSAGE           pImcpRequestMessage = (PIMCP_REQUEST_MESSAGE         )ImcpGetMsgData(pImcpHeader);
    PIMCP_PARTY_INFO                pImcpPartyInfo      = (PIMCP_PARTY_INFO              )NULL;
    ULONG                           ulSeqNumber         = (ULONG                         )ImcpRequestGetSeqNumber(pImcpRequestMessage);
    ULONG                           ulReqType           = (ULONG                         )ImcpRequestGetReqType  (pImcpRequestMessage);
    void*                           pInputBuffer        = (void*                         )ImcpRequestGetReqData  (pImcpRequestMessage);
    ULONG                           ulInputSize         = (ULONG                         )ImcpRequestGetReqSize  (pImcpRequestMessage);
    void*                           pOutputBuffer       = (void*                         )NULL;
    ULONG                           ulOutputSize        = (ULONG                         )0;
    ULONG                           ulErrorCode         = (ULONG                         )ANSC_LPC_ERROR_noError;
    char*                           party_name          = (char*                         )AnscAllocateMemory(ANSC_OBJECT_NAME_SIZE + 1);

    if ( !party_name )
    {
        return  ANSC_STATUS_RESOURCES;
    }
/*
#ifdef   _DEBUG
    if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_SERVER )
    {
        AnscTrace("\n -- !!!AnscLpccoTcp: REQ is received!!! --\n");
    }
#endif
*/
    switch ( ulReqType )
    {
        case    IMCP_REQUEST_TYPE_QUERY_PARTY :

                AnscCopyMemory(party_name, ImcpRequestGetReqData(pImcpRequestMessage), AnscGetMin2(ulInputSize, ANSC_OBJECT_NAME_SIZE));

                pPartyAddr =
                    (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr2
                        (
                            (ANSC_HANDLE)pMyObject,
                            party_name
                        );

                if ( !pPartyAddr )
                {
                    returnStatus =
                        pMyObject->SendReply
                            (
                                (ANSC_HANDLE)pMyObject,
                                hSocket,
                                NULL,
                                0,
                                ulSeqNumber,
                                ulReqType,
                                ANSC_LPC_ERROR_badPartyName
                            );
                }
                else
                {
                    ulOutputSize  = sizeof(IMCP_HEADER) + sizeof(IMCP_REPLY_MESSAGE) + sizeof(IMCP_PARTY_INFO);
                    pOutputBuffer = AnscAllocateMemory(ulOutputSize);

                    if ( !pOutputBuffer )
                    {
                        returnStatus =
                            pMyObject->SendReply
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    hSocket,
                                    NULL,
                                    0,
                                    ulSeqNumber,
                                    ulReqType,
                                    ANSC_LPC_ERROR_resources
                                );
                    }
                    else
                    {
                        pImcpPartyInfo = (PIMCP_PARTY_INFO)((ULONG)pOutputBuffer + sizeof(IMCP_HEADER) + sizeof(IMCP_REPLY_MESSAGE));

                        ImcpPartyInfoSetPartyAddrValue(pImcpPartyInfo, pPartyAddr->PartyAddr.Value);
                        ImcpPartyInfoSetPartyProtocol (pImcpPartyInfo, pPartyAddr->PartyProtocol  );
                        ImcpPartyInfoSetPartyPort     (pImcpPartyInfo, pPartyAddr->PartyPort      );
                        ImcpPartyInfoSetPartyName     (pImcpPartyInfo, pPartyAddr->PartyName      );
                    }

                    pPartyAddr->RefCount--;

                    returnStatus =
                        pMyObject->SendReply
                            (
                                (ANSC_HANDLE)pMyObject,
                                hSocket,
                                pImcpPartyInfo,
                                sizeof(IMCP_PARTY_INFO),
                                ulSeqNumber,
                                ulReqType,
                                ANSC_LPC_ERROR_noError
                            );

                    if ( pOutputBuffer )
                    {
                        AnscFreeMemory(pOutputBuffer);
                    }
                }

                break;

        case    IMCP_REQUEST_TYPE_LPC_CALL :

                returnStatus =
                    pAnscCpcIf->CallDispatch
                        (
                            pAnscCpcIf->hOwnerContext,
                            (ANSC_HANDLE)pPartyAddr,
                            pInputBuffer,
                            ulInputSize,
                            &pOutputBuffer,
                            &ulOutputSize,
                            &ulErrorCode
                        );

                if ( (returnStatus != ANSC_STATUS_SUCCESS   ) ||
                     (ulErrorCode  != ANSC_LPC_ERROR_noError) )
                {
                    returnStatus =
                        pMyObject->SendReply
                            (
                                (ANSC_HANDLE)pMyObject,
                                hSocket,
                                NULL,
                                0,
                                ulSeqNumber,
                                ulReqType,
                                ulErrorCode
                            );
                }
                else
                {
                    returnStatus =
                        pMyObject->SendReply
                            (
                                (ANSC_HANDLE)pMyObject,
                                hSocket,
                                pOutputBuffer,
                                ulOutputSize,
                                ulSeqNumber,
                                ulReqType,
                                ulErrorCode
                            );

                    if ( pOutputBuffer )
                    {
                        pAnscCpcIf->FreeOutputBuffer
                            (
                                pAnscCpcIf->hOwnerContext,
                                pOutputBuffer
                            );
                    }
                }

                break;

        default :

                returnStatus =
                    pMyObject->SendReply
                        (
                            (ANSC_HANDLE)pMyObject,
                            hSocket,
                            NULL,
                            0,
                            ulSeqNumber,
                            ulReqType,
                            ANSC_LPC_ERROR_invalidReqType
                        );

                break;
    }

    AnscFreeBdo   (pBufferDesp);
    AnscFreeMemory(party_name );
/*
#ifdef   _DEBUG
    if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_SERVER )
    {
        AnscTrace("\n -- !!!AnscLpccoTcp: REQ has been processed with status = %d!!! --\n", returnStatus);
    }
#endif
*/
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpRecvReply
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hBufferDesp
            );

    description:

        This function is called to process an incoming IMCP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which the message is
                received.

                ANSC_HANDLE                 hBufferDesp
                Specifies the incoming message buffer to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpRecvReply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    )
{
    UNREFERENCED_PARAMETER(hSocket);
    PANSC_LPCCO_TCP_OBJECT          pMyObject         = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp       = (PANSC_BUFFER_DESCRIPTOR       )hBufferDesp;
    PIMCP_HEADER                    pImcpHeader       = (PIMCP_HEADER                  )AnscBdoGetBlock(pBufferDesp);
    PIMCP_REPLY_MESSAGE             pImcpReplyMessage = (PIMCP_REPLY_MESSAGE           )ImcpGetMsgData(pImcpHeader);

    AnscBdoShrinkRight(pBufferDesp, sizeof(IMCP_HEADER) + sizeof(IMCP_REPLY_MESSAGE));

        (PANSC_LPCCO_PENDING_CALL)pMyObject->PopPendingCall
            (
                (ANSC_HANDLE)pMyObject,
                NULL,
                IMCP_MESSAGE_TYPE_REPLY,
                ImcpReplyGetSeqNumber(pImcpReplyMessage),
                ImcpReplyGetRepCode  (pImcpReplyMessage),
                (ANSC_HANDLE)pBufferDesp
            );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpRecvBye
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hBufferDesp
            );

    description:

        This function is called to process an incoming IMCP message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which the message is
                received.

                ANSC_HANDLE                 hBufferDesp
                Specifies the incoming message buffer to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpRecvBye
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hBufferDesp
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject       = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_CPC_INTERFACE             pAnscCpcIf      = (PANSC_CPC_INTERFACE           )pMyObject->hAnscCpcIf;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket   = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pConnTimerObj   = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hConnTimerObj;
    PANSC_LPC_PARTY_ADDR            pPartyAddr      = (PANSC_LPC_PARTY_ADDR          )NULL;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp     = (PANSC_BUFFER_DESCRIPTOR       )hBufferDesp;
    PIMCP_HEADER                    pImcpHeader     = (PIMCP_HEADER                  )AnscBdoGetBlock(pBufferDesp);
    PIMCP_BYE_MESSAGE               pImcpByeMessage = (PIMCP_BYE_MESSAGE             )ImcpGetMsgData  (pImcpHeader);
    ULONG                           ulPartyNameSize = (ULONG                         )ImcpGetMsgLength(pImcpHeader) - sizeof(IMCP_BYE_MESSAGE);
    char*                           party_name      = (char*                         )AnscAllocateMemory(ANSC_OBJECT_NAME_SIZE + 1);

    if ( !party_name )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    AnscCopyMemory(party_name, ImcpByeGetPartyName(pImcpByeMessage), AnscGetMin2(ulPartyNameSize, ANSC_OBJECT_NAME_SIZE));
/*
#ifdef   _DEBUG
    if ( pMyObject->LpcOpmode == ANSC_LPC_OPMODE_SERVER )
    {
        AnscTrace("\n -- !!!AnscLpccoTcp: BYE is received from %s!!! --\n",  party_name);
    }
#endif
*/
    pPartyAddr =
        (PANSC_LPC_PARTY_ADDR)pMyObject->GetPartyAddr2
            (
                (ANSC_HANDLE)pMyObject,
                party_name
            );

    if ( pPartyAddr )
    {
        if ( TRUE )
        {
                pAnscCpcIf->NotifyEvent
                    (
                        pAnscCpcIf->hOwnerContext,
                        ANSC_CPC_EVENT_DISCONNECTED,
                        (ANSC_HANDLE)pPartyAddr
                    );
        }

        pPartyAddr->Timestamp   = AnscGetTickInSeconds();
        pPartyAddr->PartyState &= ~ANSC_LPC_PARTY_STATE_connectedIn;

        if ( !(pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedIn ) &&
             !(pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedOut) )
        {
            pConnTimerObj->Stop       ((ANSC_HANDLE)pConnTimerObj);
            pConnTimerObj->SetInterval((ANSC_HANDLE)pConnTimerObj, ANSC_LPCCO_DEF_CONN_TIMEOUT);
            pConnTimerObj->Start      ((ANSC_HANDLE)pConnTimerObj);
        }

        pPartyAddr->RefCount--;
    }

    pDaemonSocket->SetClientContext((ANSC_HANDLE)pDaemonSocket, (ANSC_HANDLE)NULL);

    AnscFreeBdo   (pBufferDesp);
    AnscFreeMemory(party_name );

    return  ANSC_STATUS_SUCCESS;
}
