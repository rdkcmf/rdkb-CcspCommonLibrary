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

    module:	ansc_deuo_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Daemon Engine Udp Object.

        *   AnscDeuoGetPacket
        *   AnscDeuoAddPacket
        *   AnscDeuoDelPacket
        *   AnscDeuoDelAllPackets
        *   AnscDeuoGetSocket
        *   AnscDeuoAddSocket
        *   AnscDeuoDelSocket
        *   AnscDeuoDelAllSockets
        *   AnscDeuoEnableRecv
        *   AnscDeuoEnableSend

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/10/01    initial revision.

**********************************************************************/


#include "ansc_deuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDeuoGetPacket
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the oldest packet object in
        the waiting queue.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of packet object.

**********************************************************************/

ANSC_HANDLE
AnscDeuoGetPacket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PacketQueueLock);
    pSLinkEntry = AnscQueuePopEntry(&pMyObject->PacketQueue);
    AnscReleaseLock(&pMyObject->PacketQueueLock);

    if ( pSLinkEntry )
    {
        pPacket = ACCESS_ANSC_DSUO_PACKET_OBJECT(pSLinkEntry);
    }
    else
    {
        pPacket = NULL;
    }

    return  (ANSC_HANDLE)pPacket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoAddPacket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPacket
            );

    description:

        This function is called to add a packet object into the
        internal waiting queue.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPacket
                Specifies the packet object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoAddPacket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = (PANSC_DSUO_PACKET_OBJECT      )hPacket;

    pPacket->hEngine = (ANSC_HANDLE)pMyObject;
    pPacket->hSocket = (ANSC_HANDLE)NULL;

    AnscAcquireLock   (&pMyObject->PacketQueueLock);
    AnscQueuePushEntry(&pMyObject->PacketQueue, &pPacket->Linkage);
    AnscReleaseLock   (&pMyObject->PacketQueueLock);

    AnscSetEvent(&pMyObject->NewPacketEvent);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoDelPacket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPacket
            );

    description:

        This function is called to remove a packet object from the
        internal waiting queue.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPacket
                Specifies the packet object to be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoDelPacket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_UDP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = (PANSC_DSUO_PACKET_OBJECT      )hPacket;
    BOOL                            bLinkFound   = FALSE;

    AnscAcquireLock(&pMyObject->PacketQueueLock);
    bLinkFound = AnscQueuePopEntryByLink(&pMyObject->PacketQueue, &pPacket->Linkage);
    AnscReleaseLock(&pMyObject->PacketQueueLock);

    if ( !bLinkFound )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    returnStatus =
        pServer->ReleasePacket
            (
                (ANSC_HANDLE)pServer,
                (ANSC_HANDLE)pPacket
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoDelAllPackets
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to remove all packet objects from the
        internal hash table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoDelAllPackets
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_UDP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PacketQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->PacketQueue);

    while ( pSLinkEntry )
    {
        pPacket     = ACCESS_ANSC_DSUO_PACKET_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->PacketQueue);

            pServer->ReleasePacket
                (
                    (ANSC_HANDLE)pServer,
                    (ANSC_HANDLE)pPacket
                );
    }

    AnscReleaseLock(&pMyObject->PacketQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDeuoGetSocket
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address,
                USHORT                      port
            );

    description:

        This function is called to retrieve a socket object by matching
        the peer address and port number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies peer's IP address to be matched.

                USHORT                      port
                Specifies peer's port number to be matched.

    return:     handle of socket object.

**********************************************************************/

ANSC_HANDLE
AnscDeuoGetSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(*(PULONG)address, ANSC_DEUO_SOCKET_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->SocketTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_DAEMON_SOCKET_UDP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( (pSocket->PeerAddress.Value == *(PULONG)address) &&
             (pSocket->PeerPort          == port            ) )
        {
            AnscReleaseLock(&pMyObject->SocketTableLock);

            return  (ANSC_HANDLE)pSocket;
        }
    }

    AnscReleaseLock(&pMyObject->SocketTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoAddSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to add a socket object into the
        internal hash table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;
    ULONG                           ulHashIndex  = AnscHashUlong(pSocket->PeerAddress.Value, ANSC_DEUO_SOCKET_TABLE_SIZE);

    pSocket->HashIndex     = ulHashIndex;
    pSocket->hDaemonEngine = (ANSC_HANDLE)pMyObject;
    pSocket->LastRecvAt    = AnscGetTickInSeconds();
    pSocket->LastSendAt    = AnscGetTickInSeconds();

    pSocket->Open((ANSC_HANDLE)pSocket);

    AnscAcquireLock   (&pMyObject->SocketTableLock);
    AnscSListPushEntry(&pMyObject->SocketTable[ulHashIndex], &pSocket->Linkage);
    AnscReleaseLock   (&pMyObject->SocketTableLock);

    pMyObject->CurSocketCount++;

    pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, TRUE);
    pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, TRUE);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoDelSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to remove a socket object from the
        internal hash table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be removed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;
    ULONG                           ulHashIndex  = AnscHashUlong(pSocket->PeerAddress.Value, ANSC_DEUO_SOCKET_TABLE_SIZE);
    BOOL                            bLinkFound   = FALSE;

    AnscAcquireLock(&pMyObject->SocketTableLock);
    bLinkFound = AnscSListPopEntryByLink(&pMyObject->SocketTable[ulHashIndex], &pSocket->Linkage);
    AnscReleaseLock(&pMyObject->SocketTableLock);

    if ( !bLinkFound )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
    pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);

    pMyObject->CurSocketCount--;

    pSocket->Close ((ANSC_HANDLE)pSocket);
    pSocket->Return((ANSC_HANDLE)pSocket);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoDelAllSockets
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to remove all socket objects from the
        internal hash table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->SocketTableLock);

    for ( i = 0; i < ANSC_DEUO_SOCKET_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketTable[i]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_UDP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketTable[i]);

            pSocket->Close ((ANSC_HANDLE)pSocket);
            pSocket->Return((ANSC_HANDLE)pSocket);
        }
    }

    AnscReleaseLock(&pMyObject->SocketTableLock);

    pMyObject->CurSocketCount = 0;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoEnableRecv
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                BOOL                        bEnable
            );

    description:

        This function is called to add/remove a socket object into/from
        the socket-recv set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be add or removed.

                BOOL                        bEnable
                Specifies the operation of this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;

    if ( pSocket->hDaemonEngine != (ANSC_HANDLE)pMyObject )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pSocket->EnableRecv
            (
                (ANSC_HANDLE)pSocket,
                bEnable
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoEnableSend
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                BOOL                        bEnable
            );

    description:

        This function is called to add/remove a socket object into/from
        the socket-send set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be add or removed.

                BOOL                        bEnable
                Specifies the operation of this call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;

    if ( pSocket->hDaemonEngine != (ANSC_HANDLE)pMyObject )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pSocket->EnableSend
            (
                (ANSC_HANDLE)pSocket,
                bEnable
            );

    return  returnStatus;
}
