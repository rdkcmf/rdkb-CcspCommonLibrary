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

    module:	ansc_dsto_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoGetSocketByAddress
        *   AnscDstoGetSocketByOsocket
        *   AnscDstoAcquireSocket
        *   AnscDstoReleaseSocket
        *   AnscDstoManufactureSocketPool
        *   AnscDstoDestroySocketPool
        *   AnscDstoAssignEngine
        *   AnscDstoManufactureEnginePool
        *   AnscDstoDestroyEnginePool

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/08/01    initial revision.

**********************************************************************/


#include "ansc_dsto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDstoGetSocketByAddress
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
AnscDstoGetSocketByAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->EngineQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->EngineQueue);

    while ( pSLinkEntry )
    {
        pEngine     = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
        pSocket     =
            pEngine->GetSocketByAddress2
                (
                    (ANSC_HANDLE)pEngine,
                    address,
                    port
                );

        if ( pSocket )
        {
            AnscReleaseLock(&pMyObject->EngineQueueLock);

            return  (ANSC_HANDLE)pSocket;
        }
    }

    AnscReleaseLock(&pMyObject->EngineQueueLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDstoGetSocketByOsocket
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
AnscDstoGetSocketByOsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->EngineQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->EngineQueue);

    while ( pSLinkEntry )
    {
        pEngine     = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
        pSocket     =
            pEngine->GetSocketByOsocket2
                (
                    (ANSC_HANDLE)pEngine,
                    socket
                );

        if ( pSocket )
        {
            AnscReleaseLock(&pMyObject->EngineQueueLock);

            return  (ANSC_HANDLE)pSocket;
        }
    }

    AnscReleaseLock(&pMyObject->EngineQueueLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDstoAcquireSocket
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
AnscDstoAcquireSocket
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->SocketSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketSList);
    AnscReleaseLock(&pMyObject->SocketSListLock);

    if ( pSLinkEntry )
    {
        pSocket = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);

        return  (ANSC_HANDLE)pSocket;
    }
    else if ( !(pMyObject->Mode & ANSC_DSTO_MODE_COMPACT) )
    {
        if ( pMyObject->CurSocketCount >= pMyObject->MaxSocketCount )
        {
            return  (ANSC_HANDLE)NULL;
        }
    }

    pSocket =
        (PANSC_DAEMON_SOCKET_TCP_OBJECT)AnscCreateDaemonSocketTcp
            (
                pMyObject->hContainerContext,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSocket )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else if ( !(pMyObject->Mode & ANSC_DSTO_MODE_COMPACT) )
    {
        pMyObject->CurSocketCount++;
    }

    pSocket->SetDaemonServer ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)pMyObject   );
    pSocket->SetDaemonEngine ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL        );
    pSocket->SetClientContext((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL        );
    pSocket->SetBufferSize   ((ANSC_HANDLE)pSocket, pMyObject->MaxMessageSize);

    return  (ANSC_HANDLE)pSocket;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoReleaseSocket
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
AnscDstoReleaseSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;

    pSocket->Reset((ANSC_HANDLE)pSocket);

    if ( pMyObject->Mode & ANSC_DSTO_MODE_COMPACT )
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
        AnscDstoManufactureSocketPool
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
AnscDstoManufactureSocketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    ULONG                           i            = 0;

    if ( pMyObject->Mode & ANSC_DSTO_MODE_COMPACT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    for ( i = 0; i < pMyObject->MinSocketCount; i++ )
    {
        pSocket =
            (PANSC_DAEMON_SOCKET_TCP_OBJECT)AnscCreateDaemonSocketTcp
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

        pSocket->SetDaemonServer ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)pMyObject   );
        pSocket->SetDaemonEngine ((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL        );
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
        AnscDstoDestroySocketPool
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
AnscDstoDestroySocketPool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->SocketSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->SocketSList);

    while ( pSLinkEntry )
    {
        pSocket     = ACCESS_ANSC_DAEMON_SOCKET_TCP_OBJECT(pSLinkEntry);
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
        AnscDstoAssignEngine
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to assign a engine object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of engine object.

**********************************************************************/

ANSC_HANDLE
AnscDstoAssignEngine
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pNewEngine   = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulFlags      = 0;
    ULONG                           ulTryTimes   = 0;

    if ( pMyObject->hNextEngineToUse )
    {
        pEngine = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->hNextEngineToUse;
    }
    else
    {
        AnscAcquireLock(&pMyObject->EngineQueueLock);
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->EngineQueue);
        AnscReleaseLock(&pMyObject->EngineQueueLock);

        if ( pSLinkEntry )
        {
            pEngine = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        }
        else
        {
            return  (ANSC_HANDLE)NULL;
        }
    }

    /*
     * We need to check whether the engine is currently busy...
     */
    while ( pEngine->bBusy && (ulTryTimes < pMyObject->EngineCount) )
    {
        ulTryTimes++;

        pSLinkEntry = AnscQueueGetNextEntry(&pEngine->Linkage);

        if ( pSLinkEntry )
        {
            pEngine = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        }
        else
        {
            pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->EngineQueue);
            pEngine     = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        }
    }

    while ( pEngine->bBusy )
    {
        /*
         * We need to create a new engine and assign the socket to it.
         */
        pNewEngine =
            (PANSC_DAEMON_ENGINE_TCP_OBJECT)AnscCreateDaemonEngineTcp
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pNewEngine )
        {
            break;
        }
        else
        {
            pMyObject->EngineCount++;
        }

        pNewEngine->SetDaemonServer  ((ANSC_HANDLE)pNewEngine, (ANSC_HANDLE)pMyObject );
        pNewEngine->SetMaxSocketCount((ANSC_HANDLE)pNewEngine, pEngine->MaxSocketCount);

        AnscAcquireLock   (&pMyObject->EngineQueueLock);
        AnscQueuePushEntry(&pMyObject->EngineQueue, &pNewEngine->Linkage);
        AnscReleaseLock   (&pMyObject->EngineQueueLock);

        pEngine = pNewEngine;

        /*
         * Don't forget to start the engine.
         */
        if ( TRUE )
        {
            ulFlags = pEngine->GetControlFlags((ANSC_HANDLE)pEngine);

            if ( pMyObject->Mode & ANSC_DSTO_MODE_ASYNC_SEND )
            {
                ulFlags |= ANSC_DETO_FLAG_ASYNC_SEND;
            }

            if ( pMyObject->Mode & ANSC_DSTO_MODE_NO_TIMEOUT )
            {
                ulFlags |= ANSC_DETO_FLAG_NO_TIMEOUT;
            }

            pEngine->SetControlFlags((ANSC_HANDLE)pEngine, ulFlags);
            pEngine->Start          ((ANSC_HANDLE)pEngine);
        }

        break;
    }

    /*
     * Let's prepare for the next assignable engine using round-robin.
     */
    AnscAcquireLock(&pMyObject->EngineQueueLock);
    pSLinkEntry = AnscQueueGetNextEntry(&pEngine->Linkage);
    AnscReleaseLock(&pMyObject->EngineQueueLock);

    if ( pSLinkEntry )
    {
        pMyObject->hNextEngineToUse = (ANSC_HANDLE)ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
    }
    else
    {
        pMyObject->hNextEngineToUse = (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pEngine;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoManufactureEnginePool
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
AnscDstoManufactureEnginePool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           i            = 0;

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine =
            (PANSC_DAEMON_ENGINE_TCP_OBJECT)AnscCreateDaemonEngineTcp
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

        AnscAcquireLock   (&pMyObject->EngineQueueLock);
        AnscQueuePushEntry(&pMyObject->EngineQueue, &pEngine->Linkage);
        AnscReleaseLock   (&pMyObject->EngineQueueLock);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoDestroyEnginePool
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
AnscDstoDestroyEnginePool
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->EngineQueueLock);

    pSLinkEntry = AnscQueuePopEntry(&pMyObject->EngineQueue);

    while ( pSLinkEntry )
    {
        pEngine     = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->EngineQueue);

        pEngine->Remove((ANSC_HANDLE)pEngine);
    }

    AnscReleaseLock(&pMyObject->EngineQueueLock);

    pMyObject->hNextEngineToUse = (ANSC_HANDLE)NULL;

    return  ANSC_STATUS_SUCCESS;
}
