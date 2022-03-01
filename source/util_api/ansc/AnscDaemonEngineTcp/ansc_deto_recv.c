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

    module:	ansc_deto_recv.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Daemon Engine Tcp Object.

        *   AnscDetoRecvTask

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

        ANSC_STATUS
        AnscDetoRecvTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the start() function
        call to contain async packet-recv processing.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDetoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject     = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer       = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    ansc_fd_set*                    pRecvSet1     = (ansc_fd_set*                  )pMyObject->RecvSocketSet;
    xskt_fd_set*                    pRecvSet2     = (xskt_fd_set*                  )pMyObject->RecvSocketSet;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket       = NULL;
    ULONG                           ulLastCleanAt = AnscGetTickInSecondsAbs();
    ANSC_SOCKET                     s_socket      = ANSC_SOCKET_INVALID_SOCKET;
    int                             s_result      = 0;
    int                             i             = 0;
    ansc_fd_set*                    read_fd_set1  = NULL;
    xskt_fd_set*                    read_fd_set2  = NULL;
    ansc_fd_set*                    excp_fd_set1  = NULL;
    xskt_fd_set*                    excp_fd_set2  = NULL;
    ansc_timeval                    timeval1;
    xskt_timeval                    timeval2;

    AnscTrace("AnscDetoRecvTask is activated ...!\n");

    read_fd_set1 = (ansc_fd_set*)AnscAllocateMemory(sizeof(ansc_fd_set));
    read_fd_set2 = (xskt_fd_set*)AnscAllocateMemory(sizeof(xskt_fd_set));
    excp_fd_set1 = (ansc_fd_set*)AnscAllocateMemory(sizeof(ansc_fd_set));
    excp_fd_set2 = (xskt_fd_set*)AnscAllocateMemory(sizeof(xskt_fd_set));

    if ( !read_fd_set1 || !read_fd_set2 || !excp_fd_set1 || !excp_fd_set2 )
    {
        goto  EXIT1;
    }

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening socket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new socket
     * object and associate it with the client. This is a repeated process until the socket owner
     * closes the socket.
     */
    while ( pMyObject->bStarted )
    {
        ANSC_COMMIT_TASK();

        /*
         * To avoid letting the old half-dead sockets hogging up the system resource, we need to
         * periodically invoke the cleaning routine. The default interval is 10 seconds, and the
         * idle timeout value is 90 seconds.
         */
        if ( pMyObject->bCleaningDemanded )
        {
            pMyObject->Clean((ANSC_HANDLE)pMyObject);

            ulLastCleanAt                = AnscGetTickInSecondsAbs();
            pMyObject->bCleaningDemanded = FALSE;
        }
        else if ( (AnscGetTickInSecondsAbs() - ulLastCleanAt) >= ANSC_DETO_CLEAN_TASK_INTERVAL )
        {
            pMyObject->Clean((ANSC_HANDLE)pMyObject);

            ulLastCleanAt                = AnscGetTickInSecondsAbs();
            pMyObject->bCleaningDemanded = FALSE;
        }

        /*
         * Since the original bsd compatible socket api doesn't support asynchronous operation, the
         * nonblocking status polling is the best we can get. As a matter of fact, the current unix
         * and linux actually still don't support asynchronous notification on any socket operation.
         */
        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            AnscAcquireLock(&pMyObject->RecvSocketSetLock);
            *read_fd_set2 = *pRecvSet2;
            AnscReleaseLock(&pMyObject->RecvSocketSetLock);
        }
        else
        {
            AnscAcquireLock(&pMyObject->RecvSocketSetLock);
            *read_fd_set1 = *pRecvSet1;
            AnscReleaseLock(&pMyObject->RecvSocketSetLock);
        }

        if ( ( (pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && XSKT_SOCKET_FD_ISNUL(read_fd_set2)) ||
             (!(pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && ANSC_SOCKET_FD_ISNUL(read_fd_set1)) )
        {
            if ( pServer->Mode & ANSC_DSTO_MODE_EVENT_SYNC )
            {
                AnscWaitEvent (&pMyObject->NewSocketEvent, ANSC_DETO_WAIT_EVENT_INTERVAL);
                AnscResetEvent(&pMyObject->NewSocketEvent);

                if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
                {
                    AnscAcquireLock(&pMyObject->RecvSocketSetLock);
                    *read_fd_set2 = *pRecvSet2;
                    AnscReleaseLock(&pMyObject->RecvSocketSetLock);
                }
                else
                {
                    AnscAcquireLock(&pMyObject->RecvSocketSetLock);
                    *read_fd_set1 = *pRecvSet1;
                    AnscReleaseLock(&pMyObject->RecvSocketSetLock);
                }

                if ( ( (pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && XSKT_SOCKET_FD_ISNUL(read_fd_set2)) ||
                     (!(pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && ANSC_SOCKET_FD_ISNUL(read_fd_set1)) )
                {
                    AnscTaskRelinquish();

                    continue;
                }
            }
            else
            {
                AnscSleep(ANSC_DETO_TASK_BREAK_INTERVAL);

                continue;
            }
        }

        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            timeval2.tv_sec  = (ANSC_DETO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval2.tv_usec = (ANSC_DETO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }
        else
        {
            timeval1.tv_sec  = (ANSC_DETO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval1.tv_usec = (ANSC_DETO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }

        /*
         * The _ansc_select() function returns the total number of socket handles that are ready
         * and contained in the fd_set structures, zero if the time limit expired, or SOCKET_ERROR
         * if an error occurred. Upon return, the structures are updated to reflect the subset of
         * these sockets that meet the specified condition.
         */
        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            s_result = _xskt_select(XSKT_SOCKET_FD_SETSIZE, read_fd_set2, NULL, NULL, &timeval2);
        }
        else
        {
            s_result = _ansc_select(ANSC_SOCKET_FD_SETSIZE, read_fd_set1, NULL, NULL, &timeval1);
        }

        if ( s_result == 0 )
        {
            continue;
        }
        else if ( ( (pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && (s_result == XSKT_SOCKET_ERROR)) ||
                  (!(pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && (s_result == ANSC_SOCKET_ERROR)) )
        {

            /*
             * Previously we simply reset everything when _ansc_select() fails, which is not a good
             * solution: we shall notify the worker module and gracefully shutdown the socket(s)
             * that caused the error.
             */
            /*
            pMyObject->Reset((ANSC_HANDLE)pMyObject);
            */
            pMyObject->ExpAllSockets((ANSC_HANDLE)pMyObject);

            continue;
        }
        else if ( !pMyObject->bStarted )
        {
            break;
        }

        /*
         * If there're multiple sockets are receiving data, we loop through the returned fd_set
         * structure and process them one-by-one. However, we have a slight problem: the resulted
         * fd_set consists of only the native socket handles, not the associated Socket Objects.
         * We have to first retrieve the peer's IP address from the socket, and use it to find
         * the associated socket object.
         */
        for ( i = 0; i < s_result; i++ )
        {
            if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
            {
                XSKT_SOCKET_FD_GET(read_fd_set2, s_socket, (ULONG)i);
            }
            else
            {
                ANSC_SOCKET_FD_GET(read_fd_set1, s_socket, (ULONG)i);
            }

            if ( ( (pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && (s_socket == XSKT_SOCKET_INVALID_SOCKET)) ||
                 (!(pServer->Mode & ANSC_DSTO_MODE_XSOCKET) && (s_socket == ANSC_SOCKET_INVALID_SOCKET)) )
            {
                break;
            }
            else
            {
                pSocket =
                    (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->GetSocketByOsocket
                        (
                            (ANSC_HANDLE)pMyObject,
                            s_socket
                        );
                /*
                 * We should make sure this socket is still valid before proceeding with the socket
                 * receive operations. For example, the peer may have already closed or reset the
                 * TCP connection while we're serving the previous socket request.
                 *
                 * 10/06/04 - It's believed this modification is slowing down the GUI and we're not
                 * seeing tangible evidence that GUI responsivenss has been improved. So we disable
                 * it for now.
                 */
                /*
                if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
                {
                    XSKT_SOCKET_FD_ZERO(excp_fd_set2);
                    XSKT_SOCKET_FD_SET ((XSKT_SOCKET)s_socket, excp_fd_set2);

                    timeval2.tv_sec  = 0;
                    timeval2.tv_usec = 0;

                    s_result_excp = _xskt_select(XSKT_SOCKET_FD_SETSIZE, NULL, NULL, excp_fd_set2, &timeval2);
                }
                else
                {
                    ANSC_SOCKET_FD_ZERO(excp_fd_set1);
                    ANSC_SOCKET_FD_SET (s_socket, excp_fd_set1);

                    timeval1.tv_sec  = 0;
                    timeval1.tv_usec = 0;

                    s_result_excp = _ansc_select(ANSC_SOCKET_FD_SETSIZE, NULL, NULL, excp_fd_set1, &timeval1);
                }
                */
            }

            /*
            if ( ((s_result_excp == 1                )                                             ) ||
                 ((s_result_excp == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) ||
                 ((s_result_excp == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_DSTO_MODE_XSOCKET)) )
            {
                if ( TRUE )
                {
                    pSocket->bBroken = TRUE;

                    pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
                    pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
                }

                if ( pSocket->bTlsEnabled )
                {
                    if ( pSocket->bTlsEnabled && pSocket->bTlsConnected && !pSocket->bTlsInitializing )
                    {
                        returnStatus =
                            pWorker->Notify
                                (
                                    pWorker->hWorkerContext,
                                    (ANSC_HANDLE)pSocket,
                                    ANSC_DSTOWO_EVENT_SOCKET_ERROR,
                                    (ANSC_HANDLE)NULL
                                );
                    }
                    else
                    {
                        AnscSetEvent(&pSocket->TlsConnEvent);
                    }
                }
                else
                {
                    returnStatus =
                        pWorker->Notify
                            (
                                pWorker->hWorkerContext,
                                (ANSC_HANDLE)pSocket,
                                ANSC_DSTOWO_EVENT_SOCKET_ERROR,
                                (ANSC_HANDLE)NULL
                            );
                }

                if ( pServer->Mode & ANSC_DSTO_MODE_AUTO_CLOSE )
                {
                    pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
                }

                pMyObject->TrcCount++;

                continue;
            }
            else
            {
                pSocket =
                    (PANSC_DAEMON_SOCKET_TCP_OBJECT)pMyObject->GetSocketByOsocket
                        (
                            (ANSC_HANDLE)pMyObject,
                            s_socket
                        );
            }
            */

            if ( !pSocket )
            {
                continue;
            }
            else if ( pSocket->bTlsEnabled )
            {
#ifdef _ANSC_USE_OPENSSL_
                pMyObject->bBusy = TRUE;

                    pMyObject->Recv
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pSocket
                        );

                pMyObject->bBusy = FALSE;
#else
                pMyObject->bBusy = TRUE;

                    pMyObject->Recv2
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pSocket
                        );

                pMyObject->bBusy = FALSE;
#endif
            }
            else
            {
                pMyObject->bBusy = TRUE;

                    pMyObject->Recv
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pSocket
                        );

                pMyObject->bBusy = FALSE;
            }

            /*
             * Check whether 'bToBeCleaned' flag is set for this socket: if it is, we should close
             * this socket right away; otherwise, we continue the processing. WARNING!!! This new
             * change seems to incur instability in SLAP, we have to roll back to the initial
             * approach.
             */
            /*
            if ( pSocket->bToBeCleaned )
            {
                returnStatus =
                    pMyObject->DelSocket
                        (
                            (ANSC_HANDLE)pMyObject,
                            (ANSC_HANDLE)pSocket
                        );
            }
            */

            if ( !pMyObject->bStarted )
            {
                break;
            }
        }
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    AnscSetEvent(&pMyObject->RecvEvent);

    if ( read_fd_set1 )
    {
        AnscFreeMemory(read_fd_set1);
    }

    if ( read_fd_set2 )
    {
        AnscFreeMemory(read_fd_set2);
    }

    if ( excp_fd_set1 )
    {
        AnscFreeMemory(excp_fd_set1);
    }

    if ( excp_fd_set2 )
    {
        AnscFreeMemory(excp_fd_set2);
    }

    return  ANSC_STATUS_SUCCESS;
}
