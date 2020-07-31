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

    module:	ansc_dsto_worker.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoWorkerTask
        *   AnscDstoAsyncJobTask

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
        AnscDstoWorkerTask
            (
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is a task thread created by the engage() function
        call to contain async worker processing.

    argument:   ANSC_HANDLE                 hSocket
                Specifies the socket object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoWorkerTask
    (
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)pSocket->hDaemonServer;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pSocket->hDaemonEngine;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;
    ULONG                           ulPacketSize = pSocket->RecvPacketSize;

    pSocket->RecvPacketSize = 0;
    pSocket->RecvOffset     = 0;

    returnStatus =
        pWorker->ProcessAsync
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pSocket,
                pSocket->RecvBuffer,
                ulPacketSize,
                (ANSC_HANDLE)NULL
            );

    returnStatus =
        pEngine->EnableRecv
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pSocket,
                TRUE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoAsyncJobTask
            (
                ANSC_HANDLE                 hAsyncJob
            );

    description:

        This function is a task thread created by the accept() or other
        functions to process an async job.

    argument:   ANSC_HANDLE                 hAsyncJob
                Specifies the async job object to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDstoAsyncJobTask
    (
        ANSC_HANDLE                 hAsyncJob
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DSTO_ASYNC_JOB            pAsyncJob    = (PANSC_DSTO_ASYNC_JOB          )hAsyncJob;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)pAsyncJob->hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pCurEngine   = (PANSC_DAEMON_ENGINE_TCP_OBJECT)NULL;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)pAsyncJob->hSocket;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;
    ULONG                           ulRetryCount = 0;

    switch ( pAsyncJob->JobType )
    {
        case    ANSC_DSTO_JOB_TYPE_initTlsConnection :

                /*
                 * We have to assign an engine object to this new socket object. The assignment is done by
                 * round-robin. However, we need to make sure that the assigned engine object is not over-
                 * loaded. If it is, we repeat the search until finding a happen one.
                 */
                ulRetryCount = 0;

                if ( pSocket->bTlsEnabled )
                {
                    returnStatus = pSocket->InitTlsServer((ANSC_HANDLE)pSocket);

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        /*
                         * I don't understand why this line was commented out before: maybe the
                         * TLS_TSA implementation in AnscDaemonSocketTcp used to clean up socket
                         * object if anything goes wrong? Anyway, it seems a good idea to enable
                         * this line. 07/20/04 - I think the reason is that it's easier to close
                         * socket from the recv() context.
                         */
                        pSocket->Finish((ANSC_HANDLE)pSocket);

                        pMyObject->TccCount++;

                        break;
                    }
                }

                while (( pCurEngine =
                            (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->AssignEngine
                                (
                                    (ANSC_HANDLE)pMyObject
                                )) )
                {
                    returnStatus =
                        pCurEngine->AddSocket
                            (
                                (ANSC_HANDLE)pCurEngine,
                                (ANSC_HANDLE)pSocket
                            );

                    if ( returnStatus == ANSC_STATUS_SUCCESS )
                    {
                        pMyObject->TscCount++;

                        break;
                    }
                    else
                    {
                        ulRetryCount++;

                        if ( ulRetryCount >= pMyObject->EngineCount )
                        {
                            pSocket->Close ((ANSC_HANDLE)pSocket);
                            pSocket->Return((ANSC_HANDLE)pSocket);

                            pMyObject->TrcCount++;

                            returnStatus = ANSC_STATUS_FAILURE;

                            break;
                        }
                    }
                }

                /*
                 * If AddSocket() failed, socket has already been destroyed. In this case, we need
                 * to terminate TLS negotiation task silently. Note that the only scenario where
                 * AddSocket() fails is that the engine is serving a large number of sockets, the
                 * maximum socket number limitation has already been reached.
                 */
                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    break;
                }
                else if ( pSocket->bTlsEnabled )
                {
                    returnStatus = pSocket->OpenTlsServer((ANSC_HANDLE)pSocket);

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        /*
                         * I don't understand why this line was commented out before: maybe the
                         * TLS_TSA implementation in AnscDaemonSocketTcp used to clean up socket
                         * object if anything goes wrong? Anyway, it seems a good idea to enable
                         * this line. 07/20/04 - I think the reason is that it's easier to close
                         * socket from the recv() context.
                         */
                        pSocket->ToClean((ANSC_HANDLE)pSocket, TRUE, 2);

                        pMyObject->TccCount++;

                        break;
                    }
                }

                if ( !pWorker->Accept
                        (
                            pWorker->hWorkerContext,
                            (ANSC_HANDLE)pSocket,
                            &pSocket->hClientContext
                        ) )
                {
                    pSocket->ToClean((ANSC_HANDLE)pSocket, TRUE, 2);

                    pMyObject->TccCount++;

                    break;
                }
                else
                {
                    returnStatus =
                        pWorker->SetOut
                            (
                                pWorker->hWorkerContext,
                                (ANSC_HANDLE)pSocket
                            );
                }

                /*
                 * Mark the flag that we've completed initializing the TLS connection...
                 */
                pSocket->bTlsInitializing = FALSE;

                break;

        default :

                break;
    }

    AnscFreeMemory(pAsyncJob);

    return  returnStatus;
}
