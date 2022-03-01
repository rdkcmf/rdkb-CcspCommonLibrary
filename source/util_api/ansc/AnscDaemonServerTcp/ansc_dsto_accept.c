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

    module:	ansc_dsto_accept.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoAcceptTask

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

        ANSC_STATUS
        AnscDstoAcceptTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the engage() function
        call to handle incoming socket requests..

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject     = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_WORKER_OBJECT        pWorker       = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pCurEngine    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)NULL;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pNewSocket    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;
    ULONG                           ulSocketCount = 0;
    ULONG                           ulRetryCount  = 0;
    ULONG                           ulMccCount    = 0;
    ULONG                           ulMrcCount    = 0;
    ULONG                           ulMscCount    = 0;
    ULONG                           ulMwtPrevious = AnscGetTickInSecondsAbs();
    ANSC_SOCKET                     tmpSocket1    = -1;
    XSKT_SOCKET                     tmpSocket2    = -1;
    int                             s_result      = 0;
    int                             s_error       = 0;
    ansc_fd_set                     read_fd_set1;
    ansc_timeval                    timeval1;
    xskt_fd_set                     read_fd_set2;
    xskt_timeval                    timeval2;
#ifdef _ANSC_IPV6_COMPATIBLE_
    struct sockaddr_storage         client_sockaddr1;
    struct sockaddr_storage         client_sockaddr2;
    socklen_t                       sa_len = sizeof(struct sockaddr_storage);
#else
    ansc_socket_addr_in             client_addr1;
    xskt_socket_addr_in             client_addr2;
    int                             addrlen;
#endif
    AnscTrace("AnscDstoAcceptTask is activated ...!\n");

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening socket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new socket
     * object and associate it with the client. This is a repeated process until the socket owner
     * closes the socket.
     */
    while ( pMyObject->bActive )
    {
        ANSC_COMMIT_TASK();

        /*
         * To defend against DoS attack, we need to check whether all the engines are already fully
         * loaded. If so, we don't accept any more incoming requests until one of the engines frees
         * up slot for new requests.
         */
        if ( TRUE )
        {
            ulSocketCount = 0;

            AnscAcquireLock(&pMyObject->EngineQueueLock);

            pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->EngineQueue);

            while ( pSLinkEntry )
            {
                pCurEngine  = ACCESS_ANSC_DAEMON_ENGINE_TCP_OBJECT(pSLinkEntry);
                pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

                ulSocketCount += pCurEngine->CurSocketCount;
            }

            AnscReleaseLock(&pMyObject->EngineQueueLock);

            if ( ulSocketCount >= pMyObject->MaxSocketCount )
            {
                AnscSleep(1000);

                continue;
            }
        }

        /*
         * To accurately estimate the performance of the Daemon Sever, we not only have to record
         * the total number of rejection/serving, but also the rejection/serving rate in the last
         * minute. So we will be able to determine when the problem really takes place.
         */
        if ( (AnscGetTickInSecondsAbs() - ulMwtPrevious) > 60 )
        {
            pMyObject->MccCount = ulMccCount;
            pMyObject->MrcCount = ulMrcCount;
            pMyObject->MscCount = ulMscCount;
            ulMccCount          = 0;
            ulMrcCount          = 0;
            ulMscCount          = 0;

            ulMwtPrevious = AnscGetTickInSecondsAbs();
        }

        /*
         * Since the original bsd compatible socket api doesn't support asynchronous operation, the
         * nonblocking status polling is the best we can get. As a matter of fact, the current unix
         * and linux actually still don't support asynchronous notification on any socket operation.
         */
        if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            XSKT_SOCKET_FD_ZERO(&read_fd_set2);
            XSKT_SOCKET_FD_SET ((XSKT_SOCKET)pMyObject->Socket, &read_fd_set2);

            timeval2.tv_sec  = (ANSC_DSTO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval2.tv_usec = (ANSC_DSTO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }
        else
        {
            ANSC_SOCKET_FD_ZERO(&read_fd_set1);
            ANSC_SOCKET_FD_SET (pMyObject->Socket, &read_fd_set1);

            timeval1.tv_sec  = (ANSC_DSTO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval1.tv_usec = (ANSC_DSTO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }

        /*
         * Since only one socket is included in the fd_set, we only distinguish the result between
         * one and non-one values. If error is detected, we shall close the socket and notify the
         * socket owner immediately.
         */
        if ( pMyObject->Mode & ANSC_DSTO_MODE_POLLING_ACCEPT )
        {
            if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
            {
                s_result = _xskt_select(pMyObject->Socket + 1, &read_fd_set2, NULL, NULL, &timeval2);
            }
            else
            {
                s_result = _ansc_select(pMyObject->Socket + 1, &read_fd_set1, NULL, NULL, &timeval1);
            }

            if ( s_result == 0 )
            {
                continue;
            }
            else if ( ((s_result == XSKT_SOCKET_ERROR) &&  (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
                      ((s_result == ANSC_SOCKET_ERROR) && !(pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) )
            {
                s_error = (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

                continue;
            }
        }

        /*
         * According to de facto standards of bsd compatible socket api, if the socket is currently
         * in the listen state, it will be marked as readable if an incoming connection request has
         * been received such that an accept is guaranteed to complete without blocking.
         */
#ifndef _ANSC_IPV6_COMPATIBLE_
        if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            client_addr2.sin_family                                 = XSKT_SOCKET_AF_INET;
            ((pansc_socket_addr_in)&client_addr2)->sin_addr.s_addr  = 0;
            client_addr2.sin_port                                   = _xskt_htons(pMyObject->HostPort);
            addrlen                                                 = sizeof(client_addr2);
        }
        else
        {
            client_addr1.sin_family      = ANSC_SOCKET_AF_INET;
            client_addr1.sin_addr.s_addr = 0;
            client_addr1.sin_port        = _ansc_htons(pMyObject->HostPort);
            addrlen                      = sizeof(client_addr1);
        }
#endif
        /*AnscTrace("AnscDstoAcceptTask -- a new connection comes in \n");*/

		/* Linux Kernel Socket is not so reliable that we have to try it first */
		if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
		{
#ifdef _ANSC_IPV6_COMPATIBLE_
            tmpSocket2  = _xskt_accept(pMyObject->Socket, (xskt_socket_addr*)&client_sockaddr2, &sa_len);
#else
	        tmpSocket2  = _xskt_accept(pMyObject->Socket, (xskt_socket_addr*)&client_addr2, (unsigned int *)&addrlen);
#endif
		}
		else
		{
#ifdef _ANSC_IPV6_COMPATIBLE_
            tmpSocket1  = _ansc_accept(pMyObject->Socket, (ansc_socket_addr*)&client_sockaddr1, &sa_len);
#else
			tmpSocket1  = _ansc_accept(pMyObject->Socket, (ansc_socket_addr*)&client_addr1, (unsigned int *)&addrlen);
#endif
		}

	/*CID: 60378 Argument cannot be negative*/
        if ( tmpSocket1 < 0 && tmpSocket2 < 0 )
        {
            AnscSleep(10);

            pMyObject->TrcCount++;
            ulMrcCount         ++;

            continue;
        }
        pNewSocket =
            (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->AcquireSocket
                (
                    (ANSC_HANDLE)pMyObject
                );

        if ( !pNewSocket )
        {
			if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
			{
	            _xskt_closesocket(tmpSocket2);
			}
            else
            {
	            _ansc_closesocket(tmpSocket1);
            }

            pMyObject->TrcCount++;
            ulMrcCount         ++;

            continue;
        }
        else
        {
            if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
            {
                pNewSocket->Socket = tmpSocket2;
            }
            else
            {
                pNewSocket->Socket = tmpSocket1;
            }
        }

        if ( ((pNewSocket->Socket == XSKT_SOCKET_INVALID_SOCKET) &&  (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
             ((pNewSocket->Socket == ANSC_SOCKET_INVALID_SOCKET) && !(pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)) )
        {
            s_error = (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

            pNewSocket->Return((ANSC_HANDLE)pNewSocket);

            pMyObject->TccCount++;
            ulMccCount         ++;

            continue;
        }
        else if ( !pMyObject->bActive )
        {
            pNewSocket->Close ((ANSC_HANDLE)pNewSocket);
            pNewSocket->Return((ANSC_HANDLE)pNewSocket);

            break;
        }

        /*
         * We have created a new socket object for the incoming connection, before notifying the
         * socket owner via sink interface, we have to duplicate the content of the original socket
         * object. Although the duplication seems irrelevant in most cases, some component objects
         * may find it's very convenient to be able to retrieve peer's address information from the
         * newly created socket object.
         */
#ifdef _ANSC_IPV6_COMPATIBLE_
        
        char                            port[8]  = {0};
        size_t                          portlen  = sizeof(port);
        int                             error    = 0;
       
        if ( pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            error = 
                getnameinfo
                    (
                        (struct sockaddr*)pMyObject->pHostAddr2->ai_addr,
                        pMyObject->pHostAddr2->ai_addrlen,
                        pNewSocket->HostAddr,
                        sizeof(pNewSocket->HostAddr),
                        port,
                        portlen,
                        NI_NUMERICSERV
                    );
                    
            if ( error != 0 )
            {
                CcspTraceError(("!!! getnameinfo host error: %d !!!\n", error));
                continue;
            }
                    
            pNewSocket->HostPort = _ansc_atoi(port);
            AnscZeroMemory(port, sizeof(port));
                    
            error = 
                getnameinfo
                    (
                        (struct sockaddr*)&client_sockaddr2,
                        sa_len,
                        pNewSocket->PeerAddr,
                        sizeof(pNewSocket->PeerAddr),
                        port,
                        portlen,
                        NI_NUMERICSERV
                    );
                    
            if ( error != 0 )
            {
                CcspTraceError(("!!! getnameinfo peer error: %d !!!\n", error));
                continue;
            }
            
            pNewSocket->PeerPort = _ansc_atoi(port);
        }
        else
        {
            error = 
                getnameinfo
                    (
                        (struct sockaddr*)pMyObject->pHostAddr1->ai_addr,
                        pMyObject->pHostAddr1->ai_addrlen,
                        pNewSocket->HostAddr,
                        sizeof(pNewSocket->HostAddr),
                        port,
                        portlen,
                        NI_NUMERICSERV
                    );
                    
            if ( error != 0 )
            {
                CcspTraceError(("!!! getnameinfo host error: %d !!!\n", error));
                continue;
            }
                    
            pNewSocket->HostPort = _ansc_atoi(port);
            AnscZeroMemory(port, sizeof(port));
                    
            error = 
                getnameinfo
                    (
                        (struct sockaddr*)&client_sockaddr1,
                        sa_len,
                        pNewSocket->PeerAddr,
                        sizeof(pNewSocket->PeerAddr),
                        port,
                        portlen,
                        NI_NUMERICSERV
                    );
                    
            if ( error != 0 )
            {
                CcspTraceError(("!!! getnameinfo peer error: %d !!!\n", error));
                continue;
            }
            
            pNewSocket->PeerPort = _ansc_atoi(port);
        }
        
#else
        pNewSocket->HostAddress.Value = pMyObject->HostAddress.Value;
        pNewSocket->HostPort          = pMyObject->HostPort;
        pNewSocket->PeerAddress.Value = (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)
                                            ? ((pansc_socket_addr_in)&client_addr2)->sin_addr.s_addr
                                            : client_addr1.sin_addr.s_addr;
        pNewSocket->PeerPort          = (pMyObject->Mode & ANSC_DSTO_MODE_XSOCKET)? _xskt_ntohs(client_addr2.sin_port) : _ansc_ntohs(client_addr1.sin_port);
#endif
        pNewSocket->LastRecvAt        = AnscGetTickInSecondsAbs();

        pNewSocket->hTlsScsIf         = pMyObject->hTlsScsIf;
        pNewSocket->hTlsConnection    = (ANSC_HANDLE)NULL;
        pNewSocket->bTlsEnabled       = pMyObject->bTlsEnabled;
        pNewSocket->bTlsConnected     = FALSE;

        /*
         * If the SSL/TLS is enabled, we need to complete SSL/TLS handshake negotiation before
         * notifying the worker that a new socket connection has been established. Since the TLS
         * handshake may take quite sometime to complete, we should spawn a separate task to do
         * just that instead of blocking the 'accept' task.
         */
        if ( pMyObject->bTlsEnabled )
        {
#ifndef _ANSC_USE_OPENSSL_
            pAsyncJob = (PANSC_DSTO_ASYNC_JOB)AnscAllocateMemory(sizeof(ANSC_DSTO_ASYNC_JOB));

            if ( !pAsyncJob )
            {
                pNewSocket->Close ((ANSC_HANDLE)pNewSocket);
                pNewSocket->Return((ANSC_HANDLE)pNewSocket);

                pMyObject->TccCount++;
                ulMccCount         ++;

                continue;
            }
            else
            {
                pAsyncJob->JobType     = ANSC_DSTO_JOB_TYPE_initTlsConnection;
                pAsyncJob->hThisObject = (ANSC_HANDLE)pMyObject;
                pAsyncJob->hSocket     = (ANSC_HANDLE)pNewSocket;
            }

            returnStatus =
                pMyObject->SpawnTask3
                    (
                        (ANSC_HANDLE)pMyObject,
                        (void*      )pMyObject->AsyncJobTask,
                        (ANSC_HANDLE)pAsyncJob,
                        "AnscDstoAsyncJobTask",
                        USER_DEFAULT_TASK_PRIORITY,
                        4*USER_DEFAULT_TASK_STACK_SIZE

                    );

            continue;
#else
            SSL * ssl = NULL;
            
            ssl = openssl_accept(pNewSocket->Socket);
            
            if ( !ssl )
            {
                pNewSocket->bTlsConnected = FALSE;
                continue;
            }
            else
            {
                s_error = openssl_validate_certificate (pMyObject->Socket, NULL , ssl, SSL_SERVER_CALLS, false);
                if ( s_error == 0 )
                {
                    AnscTrace("%s - openssl_validate_certificate() failed %p.\n", __FUNCTION__, ssl);
                    continue;
                }

                pNewSocket->hTlsConnection = ssl;
                pNewSocket->bTlsConnected = TRUE;
            }

#endif
        }

        /*
         * Before we hand this socket object to one of the daemon engines, we SHALL give the worker
         * object a chance to examine the incoming socket request. A bunch of features can further
         * be implemented within this call:
         *
         *      $ authentication based on the IP address and port number
         *      $ establish session relationships
         *
         * However, the original intention was to give the worker an opportunity to associate some
         * session-specific information with this socket.
         */
        if ( !pWorker->Accept
                (
                    pWorker->hWorkerContext,
                    (ANSC_HANDLE)pNewSocket,
                    &pNewSocket->hClientContext
                ) )
        {
            pNewSocket->Close ((ANSC_HANDLE)pNewSocket);
            pNewSocket->Return((ANSC_HANDLE)pNewSocket);

            pMyObject->TccCount++;
            ulMccCount         ++;

            continue;
        }

        /*
         * We have to assign an engine object to this new socket object. The assignment is done by
         * round-robin. However, we need to make sure that the assigned engine object is not over-
         * loaded. If it is, we repeat the search until finding a happen one.
         */
        ulRetryCount = 0;

        while ( (pCurEngine =
                    (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->AssignEngine
                        (
                            (ANSC_HANDLE)pMyObject
                        )) )
        {
            returnStatus =
                pCurEngine->AddSocket
                    (
                        (ANSC_HANDLE)pCurEngine,
                        (ANSC_HANDLE)pNewSocket
                    );

            if ( returnStatus == ANSC_STATUS_SUCCESS )
            {
				AnscTrace("AnscDstoAcceptTask -- successfully add a socket to engine \n");
                pMyObject->TscCount++;
                ulMscCount         ++;

                break;
            }
            else
            {
				AnscTrace("AnscDstoAcceptTask -- failed to add a socket to engine \n");

                ulRetryCount++;

                if ( ulRetryCount >= pMyObject->EngineCount )
                {
                    pNewSocket->Close ((ANSC_HANDLE)pNewSocket);
                    pNewSocket->Return((ANSC_HANDLE)pNewSocket);

                    pMyObject->TrcCount++;
                    ulMrcCount         ++;

                    break;
                }
            }
        }
    }

    AnscSetEvent(&pMyObject->AcceptEvent);

    return  ANSC_STATUS_SUCCESS;
}
