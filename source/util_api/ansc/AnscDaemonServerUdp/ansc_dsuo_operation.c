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

    module:	ansc_dsuo_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Daemon Server Udp Object.

        *   AnscDsuoEngage
        *   AnscDsuoCancel
        *   AnscDsuoStartEngines
        *   AnscDsuoStopEngines

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

        ANSC_STATUS
        AnscDsuoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the server daemon operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pMyObject->hWorker;
    /*RDKB-6181, CID-24758,24775; initilizing before use*/
    ansc_socket_addr_in             local_addr = {0};
    xskt_socket_addr_in             local_addr2 = {0};
    int                             s_result     = 0;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pWorker )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pWorker->Init(pWorker->hWorkerContext);

        pMyObject->bActive = TRUE;
    }

    /*
     * The underlying socket wrapper may require an explicit startup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
    {
        AnscStartupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscStartupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    /*
     * To engage the Udp Daemon, we need to perform following acts in the respective order:
     *
     *      (1) create the os-dependent socket
     *      (2) manufacture and start all the engines objects
     *      (3) manufacture the global socket object pool
     *      (4) bind to the socket and listen on it
     */

    if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
    {
        pMyObject->Socket = _xskt_socket(XSKT_SOCKET_AF_INET, XSKT_SOCKET_DGRAM, 0);
    }
    else
    {
        pMyObject->Socket = _ansc_socket(ANSC_SOCKET_AF_INET, ANSC_SOCKET_DGRAM, 0);
    }

    if ( (pMyObject->Socket == ANSC_SOCKET_INVALID_SOCKET && !(pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET)) ||
         (pMyObject->Socket == XSKT_SOCKET_INVALID_SOCKET &&  (pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET)) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }
    
	   _ansc_en_reuseaddr(pMyObject->Socket);
    
    if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
    {
        local_addr2.sin_family = XSKT_SOCKET_AF_INET;
        local_addr2.sin_port   = _xskt_htons(pMyObject->HostPort);

        if ( pMyObject->HostAddress.Value == 0 )
        {
            ((pansc_socket_addr_in)&local_addr2)->sin_addr.s_addr = XSKT_SOCKET_ANY_ADDRESS;
        }
        else
        {
            ((pansc_socket_addr_in)&local_addr2)->sin_addr.s_addr = pMyObject->HostAddress.Value;
        }
    }
    else
    {
        local_addr.sin_family = ANSC_SOCKET_AF_INET;
        local_addr.sin_port   = _ansc_htons(pMyObject->HostPort);

    if ( pMyObject->HostAddress.Value == 0 )
    {
        local_addr.sin_addr.s_addr = ANSC_SOCKET_ANY_ADDRESS;
    }
    else
    {
        local_addr.sin_addr.s_addr = pMyObject->HostAddress.Value;
    }
    }

    if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
    {
        AnscTrace("AnscDsuoEngage -- the address is 0x%X:%d, family %d.\n", ((pansc_socket_addr_in)&local_addr2)->sin_addr.s_addr, local_addr2.sin_port, local_addr2.sin_family);
        s_result = _xskt_bind(pMyObject->Socket, (xskt_socket_addr*)&local_addr2, sizeof(local_addr2));
    }
    else
    {
        s_result = _ansc_bind(pMyObject->Socket, (ansc_socket_addr*)&local_addr, sizeof(local_addr));
    }

    if ( s_result != 0 )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }

    pMyObject->ManufactureEnginePool((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureSocketPool((ANSC_HANDLE)pMyObject);
    pMyObject->ManufacturePacketPool((ANSC_HANDLE)pMyObject);
    pMyObject->StartEngines         ((ANSC_HANDLE)pMyObject);

    AnscResetEvent(&pMyObject->AcceptEvent);
    returnStatus =
        AnscSpawnTask
            (
                (void*)pMyObject->AcceptTask,
                (ANSC_HANDLE)pMyObject,
                ANSC_DSUO_ACCEPT_TASK_NAME
            );

    return  ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
    {
        _xskt_closesocket(pMyObject->Socket);
    }
    else
    {
        _ansc_closesocket(pMyObject->Socket);
    }

EXIT1:

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        pMyObject->bActive = FALSE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the server daemon operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pMyObject->hWorker;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pWorker )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pWorker->Unload(pWorker->hWorkerContext);

        pMyObject->bActive = FALSE;
    }

    if ( (pMyObject->Socket != ANSC_SOCKET_INVALID_SOCKET && !(pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET)) ||
         (pMyObject->Socket != XSKT_SOCKET_INVALID_SOCKET &&  (pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET)) )
    {
        AnscWaitEvent(&pMyObject->AcceptEvent, ANSC_DSUO_TASK_CLEANUP_TIME);

        if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
        {
            _xskt_shutdown   (pMyObject->Socket, XSKT_SOCKET_SD_BOTH);
            _xskt_closesocket(pMyObject->Socket);
        }
        else
        {
        _ansc_shutdown   (pMyObject->Socket, ANSC_SOCKET_SD_BOTH);
        _ansc_closesocket(pMyObject->Socket);
        }
    }

    pMyObject->StopEngines      ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyEnginePool((ANSC_HANDLE)pMyObject);
    pMyObject->DestroySocketPool((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyPacketPool((ANSC_HANDLE)pMyObject);

    /*
     * The underlying socket wrapper may require an explicit cleanup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */

    if ( pMyObject->Mode & ANSC_DSUO_MODE_XSOCKET )
    {
        AnscCleanupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscCleanupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoStartEngines
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start all the engines.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoStartEngines
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = NULL;
    ULONG                           ulFlags      = 0;
    ULONG                           i            = 0;

    AnscAcquireLock(&pMyObject->EngineArrayLock);

    for ( i = 0; i < pMyObject->EngineCount; i++ )
    {
        pEngine = (PANSC_DAEMON_ENGINE_UDP_OBJECT)pMyObject->EngineArray[i];
        ulFlags = pEngine->GetControlFlags((ANSC_HANDLE)pEngine);

        if ( pMyObject->Mode & ANSC_DSUO_MODE_ASYNC_SEND )
        {
            ulFlags |= ANSC_DEUO_FLAG_ASYNC_SEND;
        }

        if ( pMyObject->Mode & ANSC_DSUO_MODE_NO_TIMEOUT )
        {
            ulFlags |= ANSC_DEUO_FLAG_NO_TIMEOUT;
        }

        pEngine->SetControlFlags((ANSC_HANDLE)pEngine, ulFlags);
        pEngine->Start          ((ANSC_HANDLE)pEngine);
    }

    AnscReleaseLock(&pMyObject->EngineArrayLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoStopEngines
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to stop all the engines.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoStopEngines
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

        pEngine->Stop((ANSC_HANDLE)pEngine);
    }

    AnscReleaseLock(&pMyObject->EngineArrayLock);

    return  ANSC_STATUS_SUCCESS;
}
