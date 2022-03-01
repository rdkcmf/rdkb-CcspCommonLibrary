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

    module:	ansc_dsto_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoEngage
        *   AnscDstoCancel
        *   AnscDstoStartEngines
        *   AnscDstoStopEngines

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
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoEngage
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
AnscDstoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;
    int                             s_result     = 0;
#ifdef _ANSC_IPV6_COMPATIBLE_
    errno_t                         rc                   = -1;
    ansc_addrinfo                   ansc_hints           = {0};
    ansc_addrinfo*                  pansc_local_addrinfo = NULL;
    xskt_addrinfo                   xskt_hints           = {0};
    xskt_addrinfo*                  pxskt_local_addrinfo = NULL;
    USHORT                          usPort               = 0;
    char                            port[6]              = {0};
#else
    /*RDKB-6151, CID-24487,24794; initializing variable before use*/
    ansc_socket_addr_in             local_addr1 = {0};
    xskt_socket_addr_in             local_addr2 = {0};
#endif

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

        pMyObject->StartTime = AnscGetTickInSecondsAbs();
        pMyObject->bActive   = TRUE;
    }

    /*
     * The underlying socket wrapper may require an explicit startup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        AnscStartupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscStartupSocketWrapper((ANSC_HANDLE)pMyObject);
    }
    
#ifdef _ANSC_IPV6_COMPATIBLE_

    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        xskt_hints.ai_family   = AF_UNSPEC;
        xskt_hints.ai_socktype = XSKT_SOCKET_STREAM;
        xskt_hints.ai_flags    = AI_PASSIVE | AI_ADDRCONFIG;

        usPort = pMyObject->GetHostPort((ANSC_HANDLE)pMyObject);
        rc = sprintf_s(port, sizeof(port), "%d", usPort);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            returnStatus = ANSC_STATUS_FAILURE;
            goto  EXIT1;
        }
        CcspTraceInfo(("!!! Host Name: %s, Host Port: %s !!!\n", pMyObject->HostName, port));

        if ( _xskt_getaddrinfo
                (
                    pMyObject->HostName[0] ? pMyObject->HostName : "::", 
                    port,
                    &xskt_hints,
                    &pxskt_local_addrinfo
                ) 
            )
        {
            CcspTraceError(("!!! error 1 !!!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
            
            goto  EXIT1;
        }
        
        pMyObject->pHostAddr2 = pxskt_local_addrinfo;
    }
    else
    {
        ansc_hints.ai_family   = AF_UNSPEC;
        ansc_hints.ai_socktype = ANSC_SOCKET_STREAM;
        ansc_hints.ai_flags    = AI_PASSIVE | AI_ADDRCONFIG;

        usPort = pMyObject->GetHostPort((ANSC_HANDLE)pMyObject);
        rc = sprintf_s(port, sizeof(port), "%d", usPort);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            returnStatus = ANSC_STATUS_FAILURE;
            goto  EXIT1;
        }

        CcspTraceInfo(("!!! Host Name: %s, Host Port: %s !!!\n", pMyObject->HostName, port));

        if ( _ansc_getaddrinfo
                (
                    pMyObject->HostName[0] ? pMyObject->HostName : "::",
                    port,
                    &ansc_hints,
                    &pansc_local_addrinfo
                ) 
            )
        {
            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
        
        pMyObject->pHostAddr1 = pansc_local_addrinfo;
    }

#endif

    /*
     * To engage the Tcp Daemon, we need to perform following acts in the respective order:
     *
     *      (1) create the os-dependent socket
     *      (2) manufacture and start all the engines objects
     *      (3) manufacture the global socket object pool
     *      (4) bind to the socket and listen on it
     */
    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
#ifdef _ANSC_IPV6_COMPATIBLE_
        pMyObject->Socket = _xskt_socket(pxskt_local_addrinfo->ai_family, pxskt_local_addrinfo->ai_socktype, 0);
#else
        pMyObject->Socket = _xskt_socket(XSKT_SOCKET_AF_INET, XSKT_SOCKET_STREAM, 0);
#endif
    }
    else
    {
#ifdef _ANSC_IPV6_COMPATIBLE_
        pMyObject->Socket = _ansc_socket(pansc_local_addrinfo->ai_family, pansc_local_addrinfo->ai_socktype, 0);
#else
        pMyObject->Socket = _ansc_socket(ANSC_SOCKET_AF_INET, ANSC_SOCKET_STREAM, 0);
#endif
    }

    if ( ((pMyObject->Socket == XSKT_SOCKET_INVALID_SOCKET) &&  (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
         ((pMyObject->Socket == ANSC_SOCKET_INVALID_SOCKET) && !(pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) )
    {
        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }

    _ansc_en_reuseaddr(pMyObject->Socket);

#ifndef _ANSC_IPV6_COMPATIBLE_
    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
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
        local_addr1.sin_family = ANSC_SOCKET_AF_INET;
        local_addr1.sin_port   = _ansc_htons(pMyObject->HostPort);

        if ( pMyObject->HostAddress.Value == 0 )
        {
            local_addr1.sin_addr.s_addr = ANSC_SOCKET_ANY_ADDRESS;
        }
        else
        {
            local_addr1.sin_addr.s_addr = pMyObject->HostAddress.Value;
        }
    }
#endif

    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
#ifdef _ANSC_IPV6_COMPATIBLE_
        s_result = _xskt_bind(pMyObject->Socket, pxskt_local_addrinfo->ai_addr, pxskt_local_addrinfo->ai_addrlen);
#else
        AnscTrace("AnscDstoEngage -- the address is 0x%X:%d, familty %d.\n", _ansc_ntohl(local_addr2.sin_addr.s_addr), _ansc_ntohs(local_addr2.sin_port), local_addr2.sin_family);
        s_result = _xskt_bind(pMyObject->Socket, (xskt_socket_addr*)&local_addr2, sizeof(local_addr2));
#endif
    }
    else
    {
#ifdef _ANSC_IPV6_COMPATIBLE_
        s_result = _ansc_bind(pMyObject->Socket, pansc_local_addrinfo->ai_addr, pansc_local_addrinfo->ai_addrlen);
#else
        s_result = _ansc_bind(pMyObject->Socket, (ansc_socket_addr*)&local_addr1, sizeof(local_addr1));
#endif
    }

    if ( s_result != 0 )
    {
		AnscTrace
            (
                "AnscDstoEngage -- failed to bind to the socket, error code is %d!!!\n",
                (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET) ? _xskt_get_last_error() : _ansc_get_last_error()
            );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }

    pMyObject->ManufactureEnginePool((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureSocketPool((ANSC_HANDLE)pMyObject);
    pMyObject->StartEngines         ((ANSC_HANDLE)pMyObject);

    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        s_result = _xskt_listen(pMyObject->Socket, ANSC_SOCKET_BACKLOG_VALUE);
    }
    else
    {
        s_result = _ansc_listen(pMyObject->Socket, ANSC_SOCKET_BACKLOG_VALUE);
    }

    if ( s_result != 0 )
    {
		AnscTrace("AnscDstoEngage -- failed to listen on the socket!\n");

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }

    /*
     * If the compilation option '_ANSC_SOCKET_TLS_LAYER_' is enabled, we can simply let the ANSC
     * socket layer to perform the SSL/TLS functionality; otherwise, we need to prepare for doing
     * SSL/TLS internally.
     */
    if ( pMyObject->Mode & ANSC_DSTO_MODE_TLS_ENABLED )
    {
#ifdef _ANSC_USE_OPENSSL_
        pMyObject->bTlsEnabled = TRUE;
        if ( !openssl_init(SSL_SERVER_CALLS) )
        {
        	AnscTrace("AnscSctoEngage - openssl_init() failed!\n");
            returnStatus = ANSC_STATUS_FAILURE;
            goto  EXIT2;
        }

#else
        #ifdef  _ANSC_SOCKET_TLS_LAYER_
        {
            _ansc_en_usetls(pMyObject->Socket);

            pMyObject->bTlsEnabled = FALSE;
        }
        #else
        {
            pMyObject->hTlsScsIf   = (pMyObject->hTlsScsIf != NULL)? pMyObject->hTlsScsIf : AnscSocketTlsGetScsIf();
            pMyObject->bTlsEnabled = TRUE;
            pMyObject->bTlsReqCert = (pMyObject->Mode & ANSC_DSTO_MODE_TLS_REQ_CERT);
        }
        #endif
#endif
    }

    AnscResetEvent(&pMyObject->AcceptEvent);
    returnStatus =
        pMyObject->SpawnTask3
            (
                (ANSC_HANDLE)pMyObject,
                (void*      )pMyObject->AcceptTask,
                (ANSC_HANDLE)pMyObject,
                ANSC_DSTO_ACCEPT_TASK_NAME,
                USER_DEFAULT_TASK_PRIORITY,
                2*USER_DEFAULT_TASK_STACK_SIZE

            );

    return  ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
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
        AnscDstoCancel
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
AnscDstoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;

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

    if ( ((pMyObject->Socket != XSKT_SOCKET_INVALID_SOCKET) &&  (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
         ((pMyObject->Socket != ANSC_SOCKET_INVALID_SOCKET) && !(pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) )
    {
        if ( pMyObject->Mode & ANSC_DSTO_MODE_POLLING_ACCEPT )
        {
            AnscWaitEvent(&pMyObject->AcceptEvent, ANSC_DSTO_TASK_CLEANUP_TIME);

            if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
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
        else
        {
            if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
            {
                _xskt_shutdown   (pMyObject->Socket, XSKT_SOCKET_SD_BOTH);
                _xskt_closesocket(pMyObject->Socket);
            }
            else
            {
                _ansc_shutdown   (pMyObject->Socket, ANSC_SOCKET_SD_BOTH);
                _ansc_closesocket(pMyObject->Socket);
            }

            AnscWaitEvent(&pMyObject->AcceptEvent, ANSC_DSTO_TASK_CLEANUP_TIME);
        }
    }

    pMyObject->StopEngines      ((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyEnginePool((ANSC_HANDLE)pMyObject);
    pMyObject->DestroySocketPool((ANSC_HANDLE)pMyObject);

    /*
     * The underlying socket wrapper may require an explicit cleanup() call, such is the case on
     * Microsoft windows platforms. The wrapper initialization has to done for each task. On most
     * real-time operating systems, this call is not required.
     */
    if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
    {
        AnscCleanupXsocketWrapper((ANSC_HANDLE)pMyObject);
    }
    else
    {
        AnscCleanupSocketWrapper((ANSC_HANDLE)pMyObject);
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoStartEngines
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
AnscDstoStartEngines
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    ULONG                           ulFlags      = 0;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->EngineQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->EngineQueue);

    while ( pSLinkEntry )
    {
        pEngine     = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
        ulFlags     = pEngine->GetControlFlags((ANSC_HANDLE)pEngine);

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

    AnscReleaseLock(&pMyObject->EngineQueueLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoStopEngines
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
AnscDstoStopEngines
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;

    AnscAcquireLock(&pMyObject->EngineQueueLock);

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->EngineQueue);

    while ( pSLinkEntry )
    {
        pEngine     = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        pEngine->Stop((ANSC_HANDLE)pEngine);
    }

    AnscReleaseLock(&pMyObject->EngineQueueLock);

    return  ANSC_STATUS_SUCCESS;
}
