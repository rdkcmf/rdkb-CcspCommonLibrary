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

    module:	ansc_dkto_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Daemon Socket Tcp Object.

        *   AnscDktoFinish
        *   AnscDktoOpen
        *   AnscDktoClose
        *   AnscDktoInitTlsServer
        *   AnscDktoOpenTlsServer
        *   AnscDktoLock
        *   AnscDktoUnlock
        *   AnscDktoResetTimestamp

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/09/01    initial revision.
        06/03/04    break the original InitTlsServer() into two
                    functions: InitTlsServer() and OpenTlsServer().

**********************************************************************/


#include "ansc_dkto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoFinish
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the external Worker Object to close
        the socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoFinish
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->hDaemonEngine;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pEngine )
    {
        returnStatus =
            pEngine->DelSocket
                (
                    (ANSC_HANDLE)pEngine,
                    (ANSC_HANDLE)pMyObject
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoOpen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Engine Object to initialize
        socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;

    if ( !pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->StartTime = AnscGetTickInSecondsAbs();
        pMyObject->bClosed   = FALSE;
        pMyObject->bBroken   = FALSE;
    }

    if ( !pMyObject->RecvBuffer && !(pServer->Mode & ANSC_DSTO_MODE_FOREIGN_BUFFER) )
    {
        pMyObject->RecvBuffer     = AnscAllocateMemory(pMyObject->RecvBufferSize);
        pMyObject->RecvPacketSize = 0;
        pMyObject->RecvOffset     = 0;

        if ( !pMyObject->RecvBuffer )
        {
            pMyObject->bClosed = TRUE;

            return  ANSC_STATUS_RESOURCES;
        }
    }

    pMyObject->RecvBytesCount = 0;
    pMyObject->SendBytesCount = 0;
    pMyObject->LastRecvAt     = AnscGetTickInSecondsAbs();
    pMyObject->LastSendAt     = AnscGetTickInSecondsAbs();

    /*
     * To work around certain problem in Linux TCP/IP stack, we need to explicitly set the timeout
     * for sending operation...
     */
    _ansc_set_sndtimeo(pMyObject->Socket, 3, 0);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Engine Object to unload
        socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }

#ifndef _ANSC_USE_OPENSSL_
    if ( pMyObject->bTlsEnabled && pMyObject->hTlsScsIf )
    {
        PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;
        AnscSetEvent(&pMyObject->TlsConnEvent);

        /*
         * If the TLS failed to complete handshake, the InitTlsServer() call would remove the TLS
         * conection. Since InitTlsServer() is invoked in a different task context (created by
         * AnscDstoAsyncJobTask() of AnscDaemonServerTcp), we should wait for it to clean up the
         * mess before continue.
         */
        if ( !pMyObject->bTlsConnected )
        {
            if ( pMyObject->hTlsConnection )
            {
                AnscAcquireLock(&pMyObject->TlsConnLock);

                returnStatus =
                    pTlsScsIf->RemoveConnection
                        (
                            pTlsScsIf->hOwnerContext,
                            pMyObject->hTlsConnection
                        );

                pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;

                AnscReleaseLock(&pMyObject->TlsConnLock);
            }
        }
        else if ( pMyObject->hTlsConnection )
        {
            if ( pMyObject->bTlsConnected )
            {
                pMyObject->bTlsConnected = FALSE;

                returnStatus =
                    pTlsScsIf->CloseConnection
                        (
                            pTlsScsIf->hOwnerContext,
                            pMyObject->hTlsConnection
                        );
            }

            AnscAcquireLock(&pMyObject->TlsConnLock);

            returnStatus =
                pTlsScsIf->RemoveConnection
                    (
                        pTlsScsIf->hOwnerContext,
                        pMyObject->hTlsConnection
                    );

            pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;

            AnscReleaseLock(&pMyObject->TlsConnLock);
        }

        pMyObject->bTlsConnected  = FALSE;
        pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;
    }
#else
    if ( pMyObject->hTlsConnection )
    {
        SSL *ssl = (SSL *) pMyObject->hTlsConnection;

        AnscTrace ("SSL connection %p is closed.\n", ssl);
        SSL_shutdown (ssl);
        SSL_free (ssl);

        pMyObject->bTlsConnected  = FALSE;
        pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;
    }

#endif
    
    returnStatus =
        pWorker->Remove
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pMyObject
            );

    if ( pMyObject->Socket != ANSC_SOCKET_INVALID_SOCKET )
    {
        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            _xskt_shutdown   (pMyObject->Socket, XSKT_SOCKET_SD_SEND);
            _xskt_closesocket(pMyObject->Socket);
        }
        else
        {
            _ansc_shutdown   (pMyObject->Socket, ANSC_SOCKET_SD_SEND);
            _ansc_closesocket(pMyObject->Socket);
        }
    }

    pMyObject->bClosed = TRUE;
    pMyObject->bBroken = FALSE;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoInitTlsServer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to initialize TLS server.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoInitTlsServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
#ifndef _ANSC_USE_OPENSSL_
    
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;
    PTLS_TSA_INTERFACE              pTlsTsaIf    = (PTLS_TSA_INTERFACE            )pMyObject->hTlsTsaIf;
    TLS_CONNECTION_PARAMS           tlsConnParams;

    if ( !pMyObject->bTlsEnabled || !pTlsScsIf )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( TRUE )
    {
        TlsInitConnParams((&tlsConnParams));

        tlsConnParams.bSessionSharing  = TRUE;
        tlsConnParams.bQuickHandshake  = TRUE;
        tlsConnParams.bReqCertificate  = pServer->bTlsReqCert;
        tlsConnParams.bTlsClient       = FALSE;

        tlsConnParams.HostID           = pMyObject->HostAddress.Value;
        tlsConnParams.PeerID           = pMyObject->PeerAddress.Value;
        tlsConnParams.SessionIDSize    = 0;

        tlsConnParams.CipherSuiteCount = 0;
        tlsConnParams.CompressionCount = 0;
    }

    pMyObject->hTlsConnection =
        pTlsScsIf->CreateConnection
            (
                pTlsScsIf->hOwnerContext,
                (ANSC_HANDLE)pTlsTsaIf
            );

    if ( !pMyObject->hTlsConnection )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pTlsScsIf->ConfigConnection
                (
                    pTlsScsIf->hOwnerContext,
                    pMyObject->hTlsConnection,
                    (ANSC_HANDLE)&tlsConnParams
                );
    }
#endif
    
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoOpenTlsServer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start TLS server operation.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoOpenTlsServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
#ifndef _ANSC_USE_OPENSSL_
    
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;
    PTLS_TSA_INTERFACE              pTlsTsaIf    = (PTLS_TSA_INTERFACE            )pMyObject->hTlsTsaIf;

    /*AnscResetEvent(&pMyObject->TlsConnEvent);*/

    /*
     * Mark the flag that we're initializing the TLS connection...
     */
    pMyObject->bTlsInitializing = TRUE;

    returnStatus =
        pTlsScsIf->StartConnection
            (
                pTlsScsIf->hOwnerContext,
                pMyObject->hTlsConnection
            );

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        AnscWaitEvent(&pMyObject->TlsConnEvent, 0xFFFFFFFF);
    }

    if ( !pMyObject->bTlsConnected )
    {
        /*
        returnStatus =
            pTlsScsIf->RemoveConnection
                (
                    pTlsScsIf->hOwnerContext,
                    pMyObject->hTlsConnection
                );

        pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;
        */

        returnStatus = ANSC_STATUS_FAILURE;
    }
    else
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    /*
     * DO NOT mark the 'bTlsInitializing' flag here, we let the caller do it...
     */

#endif    
    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoLock
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to lock the socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoLock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->bLocked = TRUE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoUnlock
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unlock the socket.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoUnlock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->bLocked = FALSE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoResetTimestamp
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset the timestamps.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoResetTimestamp
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->LastRecvAt = AnscGetTickInSecondsAbs();
    pMyObject->LastSendAt = AnscGetTickInSecondsAbs();

    return  ANSC_STATUS_SUCCESS;
}
