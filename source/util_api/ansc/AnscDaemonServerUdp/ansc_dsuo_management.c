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

    module:	ansc_dsuo_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Daemon Server Udp Object.

        *   AnscDsuoAcquireSocket
        *   AnscDsuoReleaseSocket
        *   AnscDsuoManufactureSocketPool
        *   AnscDsuoDestroySocketPool
        *   AnscDsuoAcquirePacket
        *   AnscDsuoReleasePacket
        *   AnscDsuoManufacturePacketPool
        *   AnscDsuoDestroyPacketPool
        *   AnscDsuoAssignEngine
        *   AnscDsuoManufactureEnginePool
        *   AnscDsuoDestroyEnginePool

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


#include "ansc_dsuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDsuoAcquireSocket
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire a socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of socket object.

**********************************************************************/

ANSC_HANDLE
AnscDsuoAcquireSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->SocketSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketSList);
    AnscReleaseLock(&pMyObject->SocketSListLock);

    if ( pSLinkEntry )
    {
        pSocket = ACCESS_ANSC_DAEMON_SOCKET_UDP_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pSocket;
    }
    else if ( pMyObject->CurSocketCount >= pMyObject->MaxSocketCount )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSocket =
        (PANSC_DAEMON_SOCKET_UDP_OBJECT)AnscCreateDaemonSocketUdp
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSocket )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pMyObject->CurSocketCount++;
    }

    pSocket->SetDaemonServer ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)pMyObject);
    pSocket->SetDaemonEngine ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL     );
    pSocket->SetClientContext((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL     );

    return  (ANSC_HANDLE)pSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoReleaseSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to release a socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoReleaseSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;

    pSocket->Reset((ANSC_HANDLE)pSocket);

    AnscAcquireLock   (&pMyObject->SocketSListLock);
    AnscSListPushEntry(&pMyObject->SocketSList, &pSocket->Linkage);
    AnscReleaseLock   (&pMyObject->SocketSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoManufactureSocketPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture the global socket pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoManufactureSocketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = NULL;
    ULONG                           i            = 0;

    for ( i = 0; i < pMyObject->MinSocketCount; i++ )
    {
        pSocket =
            (PANSC_DAEMON_SOCKET_UDP_OBJECT)AnscCreateDaemonSocketUdp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pSocket )
        {
            continue;
        }
        else
        {
            pMyObject->CurSocketCount++;
        }

        pSocket->SetDaemonServer ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)pMyObject);
        pSocket->SetDaemonEngine ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL     );
        pSocket->SetClientContext((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL     );

        AnscAcquireLock   (&pMyObject->SocketSListLock);
        AnscSListPushEntry(&pMyObject->SocketSList, &pSocket->Linkage);
        AnscReleaseLock   (&pMyObject->SocketSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoDestroySocketPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy the global socket pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoDestroySocketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->SocketSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketSList);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_DAEMON_SOCKET_UDP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketSList);

        pSocket->Remove((ANSC_HANDLE)pSocket);
        pMyObject->CurSocketCount--;
    }

    AnscReleaseLock(&pMyObject->SocketSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDsuoAcquirePacket
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to acquire a packet object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of packet object.

**********************************************************************/

ANSC_HANDLE
AnscDsuoAcquirePacket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PacketSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->PacketSList);
    AnscReleaseLock(&pMyObject->PacketSListLock);

    if ( pSLinkEntry )
    {
        pPacket = ACCESS_ANSC_DSUO_PACKET_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pPacket;
    }
    else if ( pMyObject->CurPacketCount >= pMyObject->MaxPacketCount )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pPacket = (PANSC_DSUO_PACKET_OBJECT)AnscAllocateMemory(sizeof(ANSC_DSUO_PACKET_OBJECT));

    if ( !pPacket )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pPacket->hServer        = (ANSC_HANDLE)pMyObject;
    pPacket->hEngine        = (ANSC_HANDLE)NULL;
    pPacket->hSocket        = (ANSC_HANDLE)NULL;

    pPacket->RecvBuffer     = AnscAllocateMemory(pMyObject->MaxMessageSize);
    pPacket->RecvBufferSize = pMyObject->MaxMessageSize;
    pPacket->RecvPacketSize = 0;

    if ( !pPacket->RecvBuffer )
    {
        AnscFreeMemory(pPacket);

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pMyObject->CurPacketCount++;
    }

    return  (ANSC_HANDLE)pPacket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoReleasePacket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPacket
            );

    description:

        This function is called to release a packet object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPacket
                Specifies the packet object to be released.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoReleasePacket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = (PANSC_DSUO_PACKET_OBJECT      )hPacket;

    pPacket->hServer        = (ANSC_HANDLE)pMyObject;
    pPacket->hEngine        = (ANSC_HANDLE)NULL;
    pPacket->hSocket        = (ANSC_HANDLE)NULL;
    pPacket->RecvPacketSize = 0;

    AnscAcquireLock   (&pMyObject->PacketSListLock);
    AnscSListPushEntry(&pMyObject->PacketSList, &pPacket->Linkage);
    AnscReleaseLock   (&pMyObject->PacketSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoManufacturePacketPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture the global packet pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoManufacturePacketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = NULL;
    ULONG                           i            = 0;

    for ( i = 0; i < pMyObject->MinPacketCount; i++ )
    {
        pPacket = (PANSC_DSUO_PACKET_OBJECT)AnscAllocateMemory(sizeof(ANSC_DSUO_PACKET_OBJECT));

        if ( !pPacket )
        {
            continue;
        }

        pPacket->hServer        = (ANSC_HANDLE)pMyObject;
        pPacket->hEngine        = (ANSC_HANDLE)NULL;
        pPacket->hSocket        = (ANSC_HANDLE)NULL;

        pPacket->RecvBuffer     = AnscAllocateMemory(pMyObject->MaxMessageSize);
        pPacket->RecvBufferSize = pMyObject->MaxMessageSize;
        pPacket->RecvPacketSize = 0;

        if ( !pPacket->RecvBuffer )
        {
            AnscFreeMemory(pPacket);

            continue;
        }
        else
        {
            pMyObject->CurPacketCount++;
        }

        AnscAcquireLock   (&pMyObject->PacketSListLock);
        AnscSListPushEntry(&pMyObject->PacketSList, &pPacket->Linkage);
        AnscReleaseLock   (&pMyObject->PacketSListLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoDestroyPacketPool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy the global packet pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoDestroyPacketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->PacketSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->PacketSList);

    while ( pSLinkEntry )
    {
        pPacket     = ACCESS_ANSC_DSUO_PACKET_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListPopEntry(&pMyObject->PacketSList);

        if ( pPacket->RecvBuffer )
        {
            AnscFreeMemory(pPacket->RecvBuffer);
        }

        AnscFreeMemory(pPacket);
        pMyObject->CurPacketCount--;
    }

    AnscReleaseLock(&pMyObject->PacketSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDsuoAssignEngine
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPacket
            );

    description:

        This function is called to assign a engine object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
                ANSC_HANDLE                 hPacket
                Specifies the packet object to which an engine object
                is to be assigned.

    return:     handle of engine object.

**********************************************************************/

ANSC_HANDLE
AnscDsuoAssignEngine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = (PANSC_DSUO_PACKET_OBJECT      )hPacket;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong(pPacket->PeerAddress.Value, pMyObject->EngineCount);

    pEngine = (PANSC_DAEMON_ENGINE_UDP_OBJECT)pMyObject->EngineArray[ulHashIndex];

    return  (ANSC_HANDLE)pEngine;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoManufactureEnginePool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture the global engine pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoManufactureEnginePool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = NULL;
    ULONG                           i            = 0;

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine =
            (PANSC_DAEMON_ENGINE_UDP_OBJECT)AnscCreateDaemonEngineUdp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pEngine )
        {
            continue;
        }

        pEngine->SetDaemonServer  ((ANSC_HANDLE)pEngine, (ANSC_HANDLE)pMyObject                            );
        pEngine->SetMaxSocketCount((ANSC_HANDLE)pEngine, pMyObject->MaxSocketCount / pMyObject->EngineCount);
        pEngine->SetSocketTimeOut ((ANSC_HANDLE)pEngine, pMyObject->SocketTimeOut                          );
        pEngine->SetPacketTimeOut ((ANSC_HANDLE)pEngine, pMyObject->PacketTimeOut                          );

        AnscAcquireLock(&pMyObject->EngineArrayLock);
        pMyObject->EngineArray[i] = (ANSC_HANDLE)pEngine;
        AnscReleaseLock(&pMyObject->EngineArrayLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoDestroyEnginePool
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy the global engine pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoDestroyEnginePool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->EngineArrayLock);

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine = (PANSC_DAEMON_ENGINE_UDP_OBJECT)pMyObject->EngineArray[i];

        pEngine->Remove((ANSC_HANDLE)pEngine);

        pMyObject->EngineArray[i] = (ANSC_HANDLE)NULL;
    }

    AnscReleaseLock(&pMyObject->EngineArrayLock);

    pMyObject->hNextEngineToUse = (ANSC_HANDLE)NULL;

    return  ANSC_STATUS_SUCCESS;
}
