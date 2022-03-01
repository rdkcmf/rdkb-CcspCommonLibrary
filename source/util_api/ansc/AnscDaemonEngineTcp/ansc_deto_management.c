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

    module:	ansc_deto_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Daemon Engine Tcp Object.

        *   AnscDetoGetSocketByAddress
        *   AnscDetoGetSocketByAddress2
        *   AnscDetoGetSocketByOsocket
        *   AnscDetoGetSocketByOsocket2
        *   AnscDetoAddSocket
        *   AnscDetoDelSocket
        *   AnscDetoDelAllSockets
        *   AnscDetoExpAllSockets
        *   AnscDetoEnableRecv
        *   AnscDetoEnableSend

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/09/01    initial revision.

**********************************************************************/


#include "ansc_deto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDetoGetSocketByAddress
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
AnscDetoGetSocketByAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->SocketTableLock);

    for ( i = 0; i < ANSC_DETO_SOCKET_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[i]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( (pSocket->PeerAddress.Value == *(PULONG)address) &&
                 (pSocket->PeerPort          == port            ) )
            {
                AnscReleaseLock(&pMyObject->SocketTableLock);

                return  (ANSC_HANDLE)pSocket;
            }
        }
    }

    AnscReleaseLock(&pMyObject->SocketTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDetoGetSocketByAddress2
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
AnscDetoGetSocketByAddress2
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->SocketTableLock);

    for ( i = 0; i < ANSC_DETO_SOCKET_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[i]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( (pSocket->PeerAddress.Value == *(PULONG)address) &&
                 (pSocket->PeerPort          == port            ) )
            {
                pSocket->Lock((ANSC_HANDLE)pSocket);

                AnscReleaseLock(&pMyObject->SocketTableLock);

                return  (ANSC_HANDLE)pSocket;
            }
        }
    }

    AnscReleaseLock(&pMyObject->SocketTableLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDetoGetSocketByOsocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_SOCKET                 socket
            );

    description:

        This function is called to retrieve a socket object by matching
        the os-dependent socket handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_SOCKET                 socket
                Specifies os-dependent socket handle to be matched.

    return:     handle of socket object.

**********************************************************************/

ANSC_HANDLE
AnscDetoGetSocketByOsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong((ULONG)socket, ANSC_DETO_SOCKET_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->SocketTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pSocket->Socket == socket )
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

        ANSC_HANDLE
        AnscDetoGetSocketByOsocket2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_SOCKET                 socket
            );

    description:

        This function is called to retrieve a socket object by matching
        the os-dependent socket handle.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_SOCKET                 socket
                Specifies os-dependent socket handle to be matched.

    return:     handle of socket object.

**********************************************************************/

ANSC_HANDLE
AnscDetoGetSocketByOsocket2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong((ULONG)socket, ANSC_DETO_SOCKET_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->SocketTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( pSocket->Socket == socket )
        {
            pSocket->Lock((ANSC_HANDLE)pSocket);

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
        AnscDetoAddSocket
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
AnscDetoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    ULONG                           ulHashIndex  = AnscHashUlong((ULONG)pSocket->Socket, ANSC_DETO_SOCKET_TABLE_SIZE);

	/*AnscTrace("AnscDetoAddSocket -- CurSocketCont %d Max Count %d\n",pMyObject->CurSocketCount,pMyObject->MaxSocketCount);*/

    if ( pMyObject->CurSocketCount >= pMyObject->MaxSocketCount )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    pSocket->HashIndex     = ulHashIndex;
    pSocket->hDaemonEngine = (ANSC_HANDLE)pMyObject;
    pSocket->LastRecvAt    = AnscGetTickInSecondsAbs();
    pSocket->LastSendAt    = pSocket->LastRecvAt;

    pSocket->Open((ANSC_HANDLE)pSocket);

    AnscAcquireLock   (&pMyObject->SocketTableLock);
    AnscSListPushEntry(&pMyObject->SocketTable[ulHashIndex], &pSocket->Linkage);
    AnscReleaseLock   (&pMyObject->SocketTableLock);

    pMyObject->CurSocketCount++;

    pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, TRUE);

    if ( !pSocket->bTlsEnabled )
    {
            pWorker->SetOut
                (
                    pWorker->hWorkerContext,
                    (ANSC_HANDLE)pSocket
                );
    }

    pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, TRUE);

    AnscSetEvent(&pMyObject->NewSocketEvent);

	/*AnscTrace("AnscDetoAddSocket -- returnStatus %d sockcount %d \n",returnStatus,pMyObject->CurSocketCount);*/
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoDelSocket
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
AnscDetoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    ULONG                           ulHashIndex  = pSocket->HashIndex;
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
    pMyObject->Cancel    ((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);

    pMyObject->CurSocketCount--;

    pSocket->Close ((ANSC_HANDLE)pSocket);
    pSocket->Return((ANSC_HANDLE)pSocket);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoDelAllSockets
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
AnscDetoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->SocketTableLock);

    for ( i = 0; i < ANSC_DETO_SOCKET_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketTable[i]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
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
        AnscDetoExpAllSockets
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to expire all socket objects from the
        internal hash table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoExpAllSockets
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;
    ULONG                           ulCurTime    = AnscGetTickInSecondsAbs();

    AnscAcquireLock(&pMyObject->SocketTableLock);

    for ( i = 0; i < ANSC_DETO_SOCKET_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[i]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            pSocket->bBroken    = TRUE;
            pSocket->LastRecvAt = ulCurTime + 30 - ANSC_DETO_SOCKET_TIMEOUT;
            pSocket->LastSendAt = ulCurTime + 30 - ANSC_DETO_SOCKET_TIMEOUT;

            pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
            pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
        }
    }

    AnscReleaseLock(&pMyObject->SocketTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoEnableRecv
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
AnscDetoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    ansc_fd_set*                    pRecvSet1    = (ansc_fd_set*                  )pMyObject->RecvSocketSet;
    xskt_fd_set*                    pRecvSet2    = (xskt_fd_set*                  )pMyObject->RecvSocketSet;

    if ( pSocket->hDaemonEngine != (ANSC_HANDLE)pMyObject )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    AnscAcquireLock(&pMyObject->RecvSocketSetLock);

    if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        if ( bEnable )
        {
            if ( !XSKT_SOCKET_FD_ISSET(pSocket->Socket, pRecvSet2) )
            {
                XSKT_SOCKET_FD_SET((XSKT_SOCKET)pSocket->Socket, pRecvSet2);
            }
        }
        else
        {
            if ( XSKT_SOCKET_FD_ISSET(pSocket->Socket, pRecvSet2) )
            {
                XSKT_SOCKET_FD_CLR((XSKT_SOCKET)pSocket->Socket, pRecvSet2);
            }
        }
    }
    else
    {
        if ( bEnable )
        {
            if ( !ANSC_SOCKET_FD_ISSET(pSocket->Socket, pRecvSet1) )
            {
                ANSC_SOCKET_FD_SET(pSocket->Socket, pRecvSet1);
            }
        }
        else
        {
            if ( ANSC_SOCKET_FD_ISSET(pSocket->Socket, pRecvSet1) )
            {
                ANSC_SOCKET_FD_CLR(pSocket->Socket, pRecvSet1);
            }
        }
    }

    AnscReleaseLock(&pMyObject->RecvSocketSetLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoEnableSend
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
AnscDetoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    ansc_fd_set*                    pSendSet1    = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    xskt_fd_set*                    pSendSet2    = (xskt_fd_set*                  )pMyObject->SendSocketSet;

    if ( pSocket->hDaemonEngine != (ANSC_HANDLE)pMyObject )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    AnscAcquireLock(&pMyObject->SendSocketSetLock);

    if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        if ( bEnable )
        {
            if ( !XSKT_SOCKET_FD_ISSET(pSocket->Socket, pSendSet2) )
            {
                XSKT_SOCKET_FD_SET((XSKT_SOCKET)pSocket->Socket, pSendSet2);
            }
        }
        else
        {
            if ( XSKT_SOCKET_FD_ISSET(pSocket->Socket, pSendSet2) )
            {
                XSKT_SOCKET_FD_CLR((XSKT_SOCKET)pSocket->Socket, pSendSet2);
            }
        }
    }
    else
    {
        if ( bEnable )
        {
            if ( !ANSC_SOCKET_FD_ISSET(pSocket->Socket, pSendSet1) )
            {
                ANSC_SOCKET_FD_SET(pSocket->Socket, pSendSet1);
            }
        }
        else
        {
            if ( ANSC_SOCKET_FD_ISSET(pSocket->Socket, pSendSet1) )
            {
                ANSC_SOCKET_FD_CLR(pSocket->Socket, pSendSet1);
            }
        }
    }

    AnscReleaseLock(&pMyObject->SendSocketSetLock);

    return  ANSC_STATUS_SUCCESS;
}
