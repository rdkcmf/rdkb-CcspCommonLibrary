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

    module:	ansc_bsto_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Broker Server Tcp Object.

        *   AnscBstoGetSocket
        *   AnscBstoAddSocket
        *   AnscBstoDelSocketByAddr
        *   AnscBstoDelSocket
        *   AnscBstoDelAllSockets
        *   AnscBstoAcquireSocket
        *   AnscBstoReleaseSocket
        *   AnscBstoManufactureSocketPool
        *   AnscBstoDestroySocketPool
        *   AnscBstoAssignEngine
        *   AnscBstoManufactureEnginePool
        *   AnscBstoDestroyEnginePool

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


#include "ansc_bsto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBstoGetSocket
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      dst_addr,
                USHORT                      dst_port,
                USHORT                      src_port
            );

    description:

        This function is called to retrieve a socket object.

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
AnscBstoGetSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           ulHashIndex  = AnscHashAddress(dst_addr, pMyObject->EngineCount);

    pEngine = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[ulHashIndex];
    pSocket =
        (PANSC_BROKER_SOCKET_TCP_OBJECT)pEngine->GetSocketByAddr
            (
                (ANSC_HANDLE)pEngine,
                dst_addr,
                dst_port,
                src_port
            );

    return  (ANSC_HANDLE)pSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBstoAddSocket
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      dst_addr,
                USHORT                      dst_port,
                USHORT                      src_port,
                ANSC_HANDLE                 hClientContext
            );

    description:

        This function is called to add a socket object.

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

    return:     handle of socket object.

**********************************************************************/

ANSC_HANDLE
AnscBstoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port,
        ANSC_HANDLE                 hClientContext
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           ulHashIndex  = AnscHashAddress(dst_addr, pMyObject->EngineCount);

    pEngine = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[ulHashIndex];
    pSocket =
        (PANSC_BROKER_SOCKET_TCP_OBJECT)pEngine->AddSocket
            (
                (ANSC_HANDLE)pEngine,
                dst_addr,
                dst_port,
                src_port,
                hClientContext
            );

    return  (ANSC_HANDLE)pSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoDelSocketByAddr
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      dst_addr,
                USHORT                      dst_port,
                USHORT                      src_port
            );

    description:

        This function is called to delete a socket object.

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
AnscBstoDelSocketByAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           ulHashIndex  = AnscHashAddress(dst_addr, pMyObject->EngineCount);

    pEngine      = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[ulHashIndex];
    returnStatus =
        pEngine->DelSocketByAddr
            (
                (ANSC_HANDLE)pEngine,
                dst_addr,
                dst_port,
                src_port
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoDelSocket
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called to delete a socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = (PANSC_BROKER_ENGINE_TCP_OBJECT)pSocket->hBrokerEngine;

    returnStatus =
        pEngine->DelSocket
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pSocket
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoDelAllSockets
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all socket objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBstoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->EngineArrayLock);

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[i];

        pEngine->DelAllSockets((ANSC_HANDLE)pEngine);
    }

    AnscReleaseLock(&pMyObject->EngineArrayLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBstoAcquireSocket
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
AnscBstoAcquireSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->SocketSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketSList);
    AnscReleaseLock(&pMyObject->SocketSListLock);

    if ( pSLinkEntry )
    {
        pSocket = ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pSocket;
    }
    else if ( !(pMyObject->Mode & ANSC_BSTO_MODE_COMPACT) )
    {
        if ( pMyObject->CurSocketCount >= pMyObject->MaxSocketCount )
        {
            return  (ANSC_HANDLE)NULL;
        }
    }

    pSocket =
        (PANSC_BROKER_SOCKET_TCP_OBJECT)AnscCreateBrokerSocketTcp
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSocket )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !(pMyObject->Mode & ANSC_BSTO_MODE_COMPACT) )
    {
        pMyObject->CurSocketCount++;
    }

    pSocket->SetBrokerServer ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)pMyObject   );
    pSocket->SetBrokerEngine ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL        );
    pSocket->SetClientContext((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL        );
    pSocket->SetBufferSize   ((ANSC_HANDLE)pSocket, pMyObject->MaxMessageSize);

    return  (ANSC_HANDLE)pSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoReleaseSocket
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
AnscBstoReleaseSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;

    pSocket->Reset((ANSC_HANDLE)pSocket);

    if ( pMyObject->Mode & ANSC_BSTO_MODE_COMPACT )
    {
        pSocket->Remove((ANSC_HANDLE)pSocket);
    }
    else
    {
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
        AnscBstoManufactureSocketPool
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
AnscBstoManufactureSocketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->Mode & ANSC_BSTO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < pMyObject->MinSocketCount; i++ )
    {
        pSocket =
            (PANSC_BROKER_SOCKET_TCP_OBJECT)AnscCreateBrokerSocketTcp
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

        pSocket->SetBrokerServer ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)pMyObject   );
        pSocket->SetBrokerEngine ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL        );
        pSocket->SetClientContext((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL        );
        pSocket->SetBufferSize   ((ANSC_HANDLE)pSocket, pMyObject->MaxMessageSize);

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
        AnscBstoDestroySocketPool
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
AnscBstoDestroySocketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->SocketSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketSList);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_BROKER_SOCKET_TCP_OBJECT(pSLinkEntry);
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
        AnscBstoAssignEngine
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      dst_addr
            );

    description:

        This function is called to assign a engine object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      dst_addr
                Specifies the destination address for which the engine
                is to be assigned.

    return:     handle of engine object.

**********************************************************************/

ANSC_HANDLE
AnscBstoAssignEngine
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           ulHashIndex  = AnscHashAddress(dst_addr, pMyObject->EngineCount);

    pEngine = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[ulHashIndex];

    return  (ANSC_HANDLE)pEngine;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoManufactureEnginePool
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
AnscBstoManufactureEnginePool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           i            = 0;

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine =
            (PANSC_BROKER_ENGINE_TCP_OBJECT)AnscCreateBrokerEngineTcp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pEngine )
        {
            continue;
        }

        pEngine->SetBrokerServer  ((ANSC_HANDLE)pEngine, (ANSC_HANDLE)pMyObject                            );
        pEngine->SetMaxSocketCount((ANSC_HANDLE)pEngine, pMyObject->MaxSocketCount / pMyObject->EngineCount);

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
        AnscBstoDestroyEnginePool
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
AnscBstoDestroyEnginePool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)hThisObject;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->EngineArrayLock);

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->EngineArray[i];

        pEngine->Remove((ANSC_HANDLE)pEngine);

        pMyObject->EngineArray[i] = (ANSC_HANDLE)NULL;
    }

    AnscReleaseLock(&pMyObject->EngineArrayLock);

    pMyObject->hNextEngineToUse = (ANSC_HANDLE)NULL;

    return  ANSC_STATUS_SUCCESS;
}
