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

    module:	ansc_lpccotcp_send.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Lpcco Tcp Object.

        *   AnscLpccoTcpSendHello
        *   AnscLpccoTcpSendAck
        *   AnscLpccoTcpSendRequest
        *   AnscLpccoTcpSendReply
        *   AnscLpccoTcpSendBye

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
        AnscLpccoTcpSendHello
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to send a HELLO message over the
        specified socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object over which the message is
                to be sent.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpSendHello
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject         = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pBrokerSocket     = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PIMCP_HEADER                    pImcpHeader       = (PIMCP_HEADER                  )NULL;
    PIMCP_HELLO_MESSAGE             pImcpHelloMessage = (PIMCP_HELLO_MESSAGE           )NULL;
    ULONG                           ulImcpMsgSize     = (ULONG                         )0;

    if ( TRUE )
    {
        ulImcpMsgSize = sizeof(IMCP_HEADER) + sizeof(IMCP_HELLO_MESSAGE) + AnscSizeOfString(pMyObject->PartyName);
        pImcpHeader   = (PIMCP_HEADER)AnscAllocateMemory(ulImcpMsgSize);

        if ( !pImcpHeader )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            ImcpSetMsgType  (pImcpHeader, IMCP_MESSAGE_TYPE_HELLO            );
            ImcpSetMsgLength(pImcpHeader, ulImcpMsgSize - sizeof(IMCP_HEADER));

            pImcpHelloMessage = (PIMCP_HELLO_MESSAGE)ImcpGetMsgData(pImcpHeader);
        }

        ImcpHelloSetPartyAddrValue(pImcpHelloMessage, pMyObject->MyAddress.Value);
        ImcpHelloSetPartyPort     (pImcpHelloMessage, pMyObject->MyPort         );
        ImcpHelloSetPartyName     (pImcpHelloMessage, pMyObject->PartyName      );
    }

    returnStatus =
        pBrokerSocket->Send
            (
                (ANSC_HANDLE)pBrokerSocket,
                pImcpHeader,
                ulImcpMsgSize,
                (ANSC_HANDLE)NULL
            );

    AnscFreeMemory(pImcpHeader);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSendAck
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to send an ACK message over the
        specified socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object over which the message is
                to be sent.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpSendAck
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject       = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket   = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PIMCP_HEADER                    pImcpHeader     = (PIMCP_HEADER                  )NULL;
    PIMCP_ACK_MESSAGE               pImcpAckMessage = (PIMCP_ACK_MESSAGE             )NULL;
    ULONG                           ulImcpMsgSize   = (ULONG                         )0;

    if ( TRUE )
    {
        ulImcpMsgSize = sizeof(IMCP_HEADER) + sizeof(IMCP_ACK_MESSAGE) + AnscSizeOfString(pMyObject->PartyName);
        pImcpHeader   = (PIMCP_HEADER)AnscAllocateMemory(ulImcpMsgSize);

        if ( !pImcpHeader )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            ImcpSetMsgType  (pImcpHeader, IMCP_MESSAGE_TYPE_ACK              );
            ImcpSetMsgLength(pImcpHeader, ulImcpMsgSize - sizeof(IMCP_HEADER));

            pImcpAckMessage = (PIMCP_ACK_MESSAGE)ImcpGetMsgData(pImcpHeader);
        }

        ImcpAckSetReserved (pImcpAckMessage, 0                   );
        ImcpAckSetPartyName(pImcpAckMessage, pMyObject->PartyName);
    }

    returnStatus =
        pDaemonSocket->Send
            (
                (ANSC_HANDLE)pDaemonSocket,
                pImcpHeader,
                ulImcpMsgSize,
                (ANSC_HANDLE)NULL
            );

    AnscFreeMemory(pImcpHeader);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSendRequest
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                void*                       buffer,
                ULONG                       ulSize,
                ULONG                       ulSeqNumber,
                ULONG                       ulReqType
            );

    description:

        This function is called to send a REQUEST message over the
        specified socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object over which the message is
                to be sent.

                void*                       buffer
                Specifies the raw data buffer to be sent.

                ULONG                       ulSize
                Specifies the size of the raw data buffer.

                ULONG                       ulSeqNumber
                Specifies the sequence number associated with this
                message.

                ULONG                       ulReqType
                Specifies the request type of the message.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpSendRequest
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        void*                       buffer,
        ULONG                       ulSize,
        ULONG                       ulSeqNumber,
        ULONG                       ulReqType
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pBrokerSocket       = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    void*                           pMessageBuffer      = (void*                         )NULL;
    ULONG                           ulBufferSize        = (ULONG                         )0;
    PIMCP_HEADER                    pImcpHeader         = (PIMCP_HEADER                  )NULL;
    PIMCP_REQUEST_MESSAGE           pImcpRequestMessage = (PIMCP_REQUEST_MESSAGE         )NULL;
    ULONG                           ulImcpMsgSize       = (ULONG                         )sizeof(IMCP_HEADER) + sizeof(IMCP_REQUEST_MESSAGE) + ulSize;

    if ( buffer && ulSize )
    {
        pImcpHeader         = (PIMCP_HEADER         )((ULONG)buffer - sizeof(IMCP_REQUEST_MESSAGE) - sizeof(IMCP_HEADER));
        pImcpRequestMessage = (PIMCP_REQUEST_MESSAGE)((ULONG)buffer - sizeof(IMCP_REQUEST_MESSAGE));
    }
    else
    {
        ulBufferSize   = sizeof(IMCP_HEADER) + sizeof(IMCP_REQUEST_MESSAGE);
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpHeader         = (PIMCP_HEADER         )pMessageBuffer;
            pImcpRequestMessage = (PIMCP_REQUEST_MESSAGE)ImcpGetMsgData(pImcpHeader);
        }
    }

    if ( TRUE )
    {
        ImcpSetMsgType         (pImcpHeader,         IMCP_MESSAGE_TYPE_REQUEST          );
        ImcpSetMsgLength       (pImcpHeader,         ulImcpMsgSize - sizeof(IMCP_HEADER));
        ImcpRequestSetSeqNumber(pImcpRequestMessage, ulSeqNumber                        );
        ImcpRequestSetReqType  (pImcpRequestMessage, ulReqType                          );
        ImcpRequestSetReqSize  (pImcpRequestMessage, ulSize                             );
    }

    returnStatus =
        pBrokerSocket->Send
            (
                (ANSC_HANDLE)pBrokerSocket,
                pImcpHeader,
                ulImcpMsgSize,
                (ANSC_HANDLE)NULL
            );

    if ( pMessageBuffer )
    {
        AnscFreeMemory(pMessageBuffer);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSendReply
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                void*                       buffer,
                ULONG                       ulSize,
                ULONG                       ulSeqNumber,
                ULONG                       ulReqType,
                ULONG                       ulRepCode
            );

    description:

        This function is called to send a REPLY message over the
        specified socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object over which the message is
                to be sent.

                void*                       buffer
                Specifies the raw data buffer to be sent.

                ULONG                       ulSize
                Specifies the size of the raw data buffer.

                ULONG                       ulSeqNumber
                Specifies the sequence number associated with this
                message.

                ULONG                       ulReqType
                Specifies the request type of the message.

                ULONG                       ulRepCode
                Specifies the reply code of the message.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpSendReply
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        void*                       buffer,
        ULONG                       ulSize,
        ULONG                       ulSeqNumber,
        ULONG                       ulReqType,
        ULONG                       ulRepCode
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket     = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    void*                           pMessageBuffer    = (void*                         )NULL;
    ULONG                           ulBufferSize      = (ULONG                         )0;
    PIMCP_HEADER                    pImcpHeader       = (PIMCP_HEADER                  )NULL;
    PIMCP_REPLY_MESSAGE             pImcpReplyMessage = (PIMCP_REPLY_MESSAGE           )NULL;
    ULONG                           ulImcpMsgSize     = (ULONG                         )sizeof(IMCP_HEADER) + sizeof(IMCP_REPLY_MESSAGE) + ulSize;

    if ( buffer && ulSize )
    {
        pImcpHeader       = (PIMCP_HEADER       )((ULONG)buffer - sizeof(IMCP_REPLY_MESSAGE) - sizeof(IMCP_HEADER));
        pImcpReplyMessage = (PIMCP_REPLY_MESSAGE)((ULONG)buffer - sizeof(IMCP_REPLY_MESSAGE));
    }
    else
    {
        ulBufferSize   = sizeof(IMCP_HEADER) + sizeof(IMCP_REPLY_MESSAGE);
        pMessageBuffer = AnscAllocateMemory(ulBufferSize);

        if ( !pMessageBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pImcpHeader       = (PIMCP_HEADER       )pMessageBuffer;
            pImcpReplyMessage = (PIMCP_REPLY_MESSAGE)ImcpGetMsgData(pImcpHeader);
        }
    }

    if ( TRUE )
    {
        ImcpSetMsgType       (pImcpHeader,       IMCP_MESSAGE_TYPE_REPLY            );
        ImcpSetMsgLength     (pImcpHeader,       ulImcpMsgSize - sizeof(IMCP_HEADER));
        ImcpReplySetSeqNumber(pImcpReplyMessage, ulSeqNumber                        );
        ImcpReplySetReqType  (pImcpReplyMessage, ulReqType                          );
        ImcpReplySetRepCode  (pImcpReplyMessage, ulRepCode                          );
        ImcpReplySetRepSize  (pImcpReplyMessage, ulSize                             );
    }

    returnStatus =
        pDaemonSocket->Send
            (
                (ANSC_HANDLE)pDaemonSocket,
                pImcpHeader,
                ulImcpMsgSize,
                (ANSC_HANDLE)NULL
            );

    if ( pMessageBuffer )
    {
        AnscFreeMemory(pMessageBuffer);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpSendBye
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to send a BYE message over the
        specified socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object over which the message is
                to be sent.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpSendBye
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_LPCCO_TCP_OBJECT          pMyObject       = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pBrokerSocket   = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PIMCP_HEADER                    pImcpHeader     = (PIMCP_HEADER                  )NULL;
    PIMCP_BYE_MESSAGE               pImcpByeMessage = (PIMCP_BYE_MESSAGE             )NULL;
    ULONG                           ulImcpMsgSize   = (ULONG                         )0;

    if ( TRUE )
    {
        ulImcpMsgSize = sizeof(IMCP_HEADER) + sizeof(IMCP_BYE_MESSAGE) + AnscSizeOfString(pMyObject->PartyName);
        pImcpHeader   = (PIMCP_HEADER)AnscAllocateMemory(ulImcpMsgSize);

        if ( !pImcpHeader )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            ImcpSetMsgType  (pImcpHeader, IMCP_MESSAGE_TYPE_BYE            );
            ImcpSetMsgLength(pImcpHeader, ulImcpMsgSize - sizeof(IMCP_HEADER));

            pImcpByeMessage = (PIMCP_BYE_MESSAGE)ImcpGetMsgData(pImcpHeader);
        }

        ImcpByeSetReserved (pImcpByeMessage, 0                   );
        ImcpByeSetPartyName(pImcpByeMessage, pMyObject->PartyName);
    }

    returnStatus =
        pBrokerSocket->Send
            (
                (ANSC_HANDLE)pBrokerSocket,
                pImcpHeader,
                ulImcpMsgSize,
                (ANSC_HANDLE)NULL
            );

    AnscFreeMemory(pImcpHeader);

    return  returnStatus;
}
