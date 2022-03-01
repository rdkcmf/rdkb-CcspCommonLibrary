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

    module:	ansc_beto_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Broker Engine Tcp Object.

        *   AnscBetoGetSocketByAddr
        *   AnscBetoGetSocketByOsocket
        *   AnscBetoAddSocket
        *   AnscBetoDelSocketByAddr
        *   AnscBetoDelSocket
        *   AnscBetoDelAllSockets
        *   AnscBetoEnableRecv
        *   AnscBetoEnableSend

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/19/02    initial revision.

**********************************************************************/


#include "ansc_beto_global.h"

static
_ANSC_INLINE_
ULONG
GetAddrHashValue
    (
        PUCHAR                  dst_addr,
        USHORT                  dst_port,
        USHORT                  src_port
    )
{
    return (*(PULONG)dst_addr) + dst_port + src_port;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBetoGetSocketByAddr
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      dst_addr,
                USHORT                      dst_port,
                USHORT                      src_port
            );

    description:

        This function is called to retrieve a socket object by matching
        the peer address and port number.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      dst_addr
                Specifies the destination address to be matched.

                USHORT                      dst_port
                Specifies the socket port to be matched.

                USHORT                      src_port
                Specifies the socket port to be matched.

    return:     handle of socket object.

**********************************************************************/

ANSC_HANDLE
AnscBetoGetSocketByAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    ULONG                           ulHashIndex;

    ulHashIndex = AnscHashUlong(GetAddrHashValue(dst_addr, dst_port, src_port), ANSC_BETO_SOCKET_TABLE_SIZE);
        
    AnscAcquireLock(&pMyObject->SocketTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->AddrHashTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT_ADDRLINK(pSLinkEntry);
        pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

        if ( (pSocket->PeerAddress.Value == *(PULONG)dst_addr) &&
             (pSocket->PeerPort          == dst_port         ) &&
             (pSocket->HostPort          == src_port         ) )
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
        AnscBetoGetSocketByOsocket
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
AnscBetoGetSocketByOsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = AnscHashUlong((ULONG)socket, ANSC_BETO_SOCKET_TABLE_SIZE);

    AnscAcquireLock(&pMyObject->SocketTableLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->SocketTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT(pSLinkEntry);
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
        AnscBetoAddSocket
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      dst_addr,
                USHORT                      dst_port,
                USHORT                      src_port,
                ANSC_HANDLE                 hClientContext
            );

    description:

        This function is called to add a socket object into the
        internal hash table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      dst_addr
                Specifies the destination address to be matched.

                USHORT                      dst_port
                Specifies the socket port to be matched.

                USHORT                      src_port
                Specifies the socket port to be matched.

                ANSC_HANDLE                 hClientContext
                Specifies the client context associated with the socket.

    return:     newly added socket object.

**********************************************************************/

ANSC_HANDLE
AnscBetoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port,
        ANSC_HANDLE                 hClientContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    ULONG                           ulHashIndex  = 0;
    ULONG                           ulHashIndexAddr;

    /*AnscTrace("AnscBetoAddSocket!\n dstinationaddress %X dst_port %X src_port %X",*(PULONG)dst_addr,dst_port,src_port);*/

    if ( pMyObject->CurSocketCount >= pMyObject->MaxSocketCount )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSocket =
        (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->GetSocketByAddr
            (
                (ANSC_HANDLE)pMyObject,
                dst_addr,
                dst_port,
                src_port
            );

    if ( pSocket )
    {
        return  (ANSC_HANDLE)pSocket;
    }

    pSocket =
        (PANSC_BROKER_SOCKET_TCP_OBJECT)pServer->AcquireSocket
            (
                (ANSC_HANDLE)pServer
            );

    if ( !pSocket )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pSocket->HostAddress.Value = pServer->HostAddress.Value;
    pSocket->HostPort          = src_port;
    pSocket->PeerAddress.Value = *(PULONG)dst_addr;
    pSocket->PeerPort          = dst_port;

    pSocket->HashIndex         = ulHashIndex;
    pSocket->hBrokerEngine     = (ANSC_HANDLE)pMyObject;
    pSocket->hClientContext    = hClientContext;
    pSocket->LastRecvAt        = AnscGetTickInSeconds();
    pSocket->LastSendAt        = AnscGetTickInSeconds();

    returnStatus = pSocket->Open((ANSC_HANDLE)pSocket);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        if ( ((pSocket->Socket != (ANSC_SOCKET)XSKT_SOCKET_INVALID_SOCKET) &&  (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) ||
             ((pSocket->Socket != (ANSC_SOCKET)ANSC_SOCKET_INVALID_SOCKET) && !(pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) )
        {
            if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
            {
                _xskt_shutdown   (pSocket->Socket, XSKT_SOCKET_SD_SEND);
                _xskt_closesocket(pSocket->Socket);
            }
            else
            {
                _ansc_shutdown   (pSocket->Socket, ANSC_SOCKET_SD_SEND);
                _ansc_closesocket(pSocket->Socket);
            }
        }

        pSocket->Return((ANSC_HANDLE)pSocket);

        /*AnscTrace("AnscBetoAddSocket -- fail to open socket !\n");*/
        return  (ANSC_HANDLE)NULL;
    }

    ulHashIndex        = AnscHashUlong((ULONG)pSocket->Socket, ANSC_BETO_SOCKET_TABLE_SIZE);
    pSocket->HashIndex = ulHashIndex;


    if ( !pWorker->Accept
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pSocket,
                &pSocket->hClientContext
            ) )
    {
        pSocket->Close ((ANSC_HANDLE)pSocket);
        pSocket->Return((ANSC_HANDLE)pSocket);

        /*AnscTrace("AnscBetoAddSocket -- fail to worker accept \n");*/
        return  (ANSC_HANDLE)NULL;
    }

    ulHashIndexAddr = AnscHashUlong
        (
            GetAddrHashValue((UCHAR*)&(pSocket->PeerAddress.Value), pSocket->PeerPort, pSocket->HostPort), 
            ANSC_BETO_SOCKET_TABLE_SIZE
        );

    AnscAcquireLock   (&pMyObject->SocketTableLock);
    AnscSListPushEntry(&pMyObject->SocketTable[ulHashIndex], &pSocket->Linkage);
    AnscSListPushEntry(&pMyObject->AddrHashTable[ulHashIndexAddr], &pSocket->AddrLink);
    pMyObject->CurSocketCount++;
    AnscReleaseLock   (&pMyObject->SocketTableLock);

    pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, TRUE);
    pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, TRUE);

    AnscSetEvent(&pMyObject->NewSocketEvent);

    AnscTrace("AnscBetoAddSocket -- pSocket %p\n", pSocket);

    return  (ANSC_HANDLE)pSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoDelSocketByAddr
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      dst_addr,
                USHORT                      dst_port,
                USHORT                      src_port
            );

    description:

        This function is called to delete a socket object from the
        internal hash table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      dst_addr
                Specifies the destination address to be matched.

                USHORT                      dst_port
                Specifies the socket port to be matched.

                USHORT                      src_port
                Specifies the socket port to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBetoDelSocketByAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;

    pSocket =
        (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->GetSocketByAddr
            (
                (ANSC_HANDLE)pMyObject,
                dst_addr,
                dst_port,
                src_port
            );

    if ( pSocket )
    {
        returnStatus =
            pMyObject->DelSocket
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pSocket
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoDelSocket
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
AnscBetoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    ULONG                           ulHashIndex  = pSocket->HashIndex;
    ULONG                           ulHashIndexAddr;
    BOOL                            bLinkFound   = FALSE;

    ulHashIndexAddr = AnscHashUlong
        (
            GetAddrHashValue((UCHAR*)&(pSocket->PeerAddress.Value), pSocket->PeerPort, pSocket->HostPort), 
            ANSC_BETO_SOCKET_TABLE_SIZE
        );

    AnscAcquireLock(&pMyObject->SocketTableLock);

    bLinkFound = AnscSListPopEntryByLink(&pMyObject->SocketTable[ulHashIndex], &pSocket->Linkage);

    /*
     * remove from the addr hash table too:
     */
    AnscSListPopEntryByLink(&pMyObject->AddrHashTable[ulHashIndexAddr], &pSocket->AddrLink);

    if( ! bLinkFound )
    {
        AnscReleaseLock(&pMyObject->SocketTableLock);
        return  ANSC_STATUS_SUCCESS;
    }
    
    pMyObject->CurSocketCount--;

    AnscReleaseLock(&pMyObject->SocketTableLock);

    pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
    pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
    pMyObject->Cancel    ((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);

    pSocket->Close ((ANSC_HANDLE)pSocket);
    pSocket->Return((ANSC_HANDLE)pSocket);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoDelAllSockets
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
AnscBetoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->SocketTableLock);

    for ( i = 0; i < ANSC_BETO_SOCKET_TABLE_SIZE; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketTable[i]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketTable[i]);

            pSocket->Close ((ANSC_HANDLE)pSocket);
            pSocket->Return((ANSC_HANDLE)pSocket);
        }

        /*
         * Reset the addr hash table as well:
         */
        AnscSListInitializeHeader(&pMyObject->AddrHashTable[i]);
    }

    pMyObject->CurSocketCount = 0;

    AnscReleaseLock(&pMyObject->SocketTableLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoEnableRecv
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
AnscBetoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    ansc_fd_set*                    pRecvSet1    = (ansc_fd_set*                  )pMyObject->RecvSocketSet;
    xskt_fd_set*                    pRecvSet2    = (xskt_fd_set*                  )pMyObject->RecvSocketSet;

    if ( pSocket->hBrokerEngine != (ANSC_HANDLE)pMyObject )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    AnscAcquireLock(&pMyObject->RecvSocketSetLock);

    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
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
        AnscBetoEnableSend
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
AnscBetoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    ansc_fd_set*                    pSendSet1    = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    xskt_fd_set*                    pSendSet2    = (xskt_fd_set*                  )pMyObject->SendSocketSet;

    if ( pSocket->hBrokerEngine != (ANSC_HANDLE)pMyObject )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    AnscAcquireLock(&pMyObject->SendSocketSetLock);

    if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
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
