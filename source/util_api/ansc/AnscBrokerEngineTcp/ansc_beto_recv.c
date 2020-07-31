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

    module:	ansc_beto_recv.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Broker Engine Tcp Object.

        *   AnscBetoRecvTask

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBetoRecvTask
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

typedef union 
{
    ansc_fd_set                     aset;
    xskt_fd_set                     xset;
}
uni_fd_set;

typedef union 
{
    ansc_timeval                    atv;
    xskt_timeval                    xtv;
}
uni_timeval;


ANSC_STATUS
AnscBetoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_ENGINE_TCP_OBJECT  pMyObject     = (PANSC_BROKER_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer       = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    ansc_fd_set*                    pRecvSet1     = (ansc_fd_set*                  )pMyObject->RecvSocketSet;
    xskt_fd_set*                    pRecvSet2     = (xskt_fd_set*                  )pMyObject->RecvSocketSet;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket       = NULL;
    ULONG                           ulLastCleanAt = AnscGetTickInSeconds();
    ANSC_SOCKET                     s_socket      = ANSC_SOCKET_INVALID_SOCKET;
    int                             s_result      = 0;
    int                             i             = 0;
    uni_fd_set                      read_fd_set;
    /*uni_fd_set                      excp_fd_set;*/
    uni_timeval                     timeval;

    AnscTrace("AnscBetoRecvTask is activated ...!\n");

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
        if ( pMyObject->bCleaningDemanded  || 
             (AnscGetTickInSeconds() - ulLastCleanAt) >= ANSC_BETO_CLEAN_TASK_INTERVAL )
        {
            pMyObject->Clean((ANSC_HANDLE)pMyObject);

            ulLastCleanAt                = AnscGetTickInSeconds();
            pMyObject->bCleaningDemanded = FALSE;
        }

        /*
         * The _ansc_select() function returns the total number of socket handles that are ready
         * and contained in the fd_set structures, zero if the time limit expired, or SOCKET_ERROR
         * if an error occurred. Upon return, the structures are updated to reflect the subset of
         * these sockets that meet the specified condition.
         */
    
        if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
        {
            /*
             * no need to use lock for read_fd_set here, dirty read does not matter.
             */
            read_fd_set.xset = *pRecvSet2;

            if( XSKT_SOCKET_FD_ISNUL(&read_fd_set.xset) )
            {
                if ( pServer->Mode & ANSC_BSTO_MODE_EVENT_SYNC )
                {
                    AnscWaitEvent (&pMyObject->NewSocketEvent, ANSC_BETO_WAIT_EVENT_INTERVAL);
                    AnscResetEvent(&pMyObject->NewSocketEvent);
                }
                else
                {
                    AnscSleep(ANSC_BETO_TASK_BREAK_INTERVAL);
                }

                continue;
            }

            timeval.xtv.tv_sec  = (ANSC_BETO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval.xtv.tv_usec = (ANSC_BETO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */

            s_result = _xskt_select(XSKT_SOCKET_FD_SETSIZE, &read_fd_set.xset, NULL, NULL, &timeval.xtv);

            if ( s_result == 0 )
            {
                continue;
            }

            if ( s_result == XSKT_SOCKET_ERROR )
            {
                pMyObject->Reset((ANSC_HANDLE)pMyObject);
                continue;
            }
            
        }
        else
        {
            /*
             * no need to use lock for read_fd_set here, dirty read does not matter.
             */
            read_fd_set.aset = *pRecvSet1;

            if( ANSC_SOCKET_FD_ISNUL(&read_fd_set.aset) )
            {

                if ( pServer->Mode & ANSC_BSTO_MODE_EVENT_SYNC )
                {
                    AnscWaitEvent (&pMyObject->NewSocketEvent, ANSC_BETO_WAIT_EVENT_INTERVAL);
                    AnscResetEvent(&pMyObject->NewSocketEvent);
                }
                else
                {
                    AnscSleep(ANSC_BETO_TASK_BREAK_INTERVAL);
                }

                continue;
            }

            timeval.atv.tv_sec  = (ANSC_BETO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            timeval.atv.tv_usec = (ANSC_BETO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */

            s_result = _ansc_select(ANSC_SOCKET_FD_SETSIZE, &read_fd_set.aset, NULL, NULL, &timeval.atv);

            if ( s_result == 0 )
            {
                continue;
            }

            if ( s_result == ANSC_SOCKET_ERROR )
            {
                pMyObject->Reset((ANSC_HANDLE)pMyObject);
                continue;
            }
            
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
            if ( !pMyObject->bStarted )
            {
                break;
            }

            if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
            {
                XSKT_SOCKET_FD_GET(&read_fd_set.xset, s_socket, (ULONG)i);
                if( s_socket == XSKT_SOCKET_INVALID_SOCKET )
                {
                    break;
                }

                if( ! XSKT_SOCKET_FD_ISSET(s_socket, pRecvSet2) )
                {
                    AnscTraceError(("AnscBetoRecvTask: XSKT_SOCKET_FD_ISSET returned FALSE.\n"));
                    continue;
                }
            }
            else
            {
                ANSC_SOCKET_FD_GET(&read_fd_set.aset, s_socket, (ULONG)i);
                if( s_socket == ANSC_SOCKET_INVALID_SOCKET )
                {
                    break;
                }

                if( ! ANSC_SOCKET_FD_ISSET(s_socket, pRecvSet1) )
                {
                    AnscTraceError(("AnscBetoRecvTask: XSKT_SOCKET_FD_ISSET returned FALSE.\n"));
                    continue;
                }
            }

            pSocket =
                (PANSC_BROKER_SOCKET_TCP_OBJECT)pMyObject->GetSocketByOsocket
                    (
                        (ANSC_HANDLE)pMyObject,
                        s_socket
                    );
 
            if ( !pSocket )
            {
                continue;
            }

            /*
             * We should make sure this socket is still valid before proceeding with the socket
             * receive operations. For example, the peer may have already closed or reset the
             * TCP connection while we're serving the previous socket request.
             *
             * 10/06/04 - It's believed this modification is slowing down the GUI and we're not
             * seeing tangible evidence that GUI responsivenss has been improved. So we disable
             * it for now.
             *
             * 11/20/09 - Re-activate the following code segment to validate the socket before
             * proceeding.
             */
            /*
            if ( pServer->Mode & ANSC_BSTO_MODE_XSOCKET )
            {
                XSKT_SOCKET_FD_ZERO((&excp_fd_set.xset));
                XSKT_SOCKET_FD_SET ((XSKT_SOCKET)s_socket, (&excp_fd_set.xset));

                timeval.xtv.tv_sec  = 0;
                timeval.xtv.tv_usec = 0;

                s_result_excp = _xskt_select(XSKT_SOCKET_FD_SETSIZE, NULL, NULL, &excp_fd_set.xset, &timeval.xtv);
            }
            else
            {
                ANSC_SOCKET_FD_ZERO((&excp_fd_set.aset));
                ANSC_SOCKET_FD_SET (s_socket, (&excp_fd_set.aset));

                timeval.atv.tv_sec  = 0;
                timeval.atv.tv_usec = 0;

                s_result_excp = _ansc_select(ANSC_SOCKET_FD_SETSIZE, NULL, NULL, &excp_fd_set.aset, &timeval.atv);
            }

            if ( ((s_result_excp == 1                )                                             ) ||
                 ((s_result_excp == XSKT_SOCKET_ERROR) &&  (pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) ||
                 ((s_result_excp == ANSC_SOCKET_ERROR) && !(pServer->Mode & ANSC_BSTO_MODE_XSOCKET)) )
            {
                if ( TRUE )
                {
                    pSocket->bBroken      = TRUE;
                    pSocket->bToBeCleaned = TRUE;

                    pMyObject->EnableRecv((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
                    pMyObject->EnableSend((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket, FALSE);
                }

                if ( pServer->Mode & ANSC_BSTO_MODE_AUTO_CLOSE )
                {
                    pMyObject->DelSocket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pSocket);
                }

                continue;
            }
            */

                pMyObject->Recv
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pSocket
                    );
 
        }
    }

    AnscSetEvent(&pMyObject->RecvEvent);

    return  ANSC_STATUS_SUCCESS;
}
