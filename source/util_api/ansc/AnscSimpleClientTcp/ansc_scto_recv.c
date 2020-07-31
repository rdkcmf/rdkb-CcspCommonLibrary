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

    module:	ansc_scto_recv.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Simple Client Tcp Object.

        *   AnscSctoRecvTask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/11/01    initial revision.

**********************************************************************/


#include "ansc_scto_global.h"

#define SCTO_MAX_RECV_RETRY_COUNT        100

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoRecvTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is a task thread created by the engage() function
        call to contain async packet-recv processing.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    char*                           recv_buffer  = NULL;
    ULONG                           ulBufferSize = 0;
    int                             recv_size    = 0;
    int                             s_result     = 0;
    int                             i            = 0;

    AnscTrace("AnscSctoRecvTask is activated ...!\n");

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening socket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new socket
     * object and associate it with the client. This is a repeated process until the socket owner
     * closes the socket.
     */
    while ( pMyObject->bActive )
    {
#ifdef _ANSC_USE_OPENSSL_
        if ( pMyObject->bTlsEnabled )
        {
        	/* wait for SSL to negotiate */
        	if ( !pMyObject->bTlsConnected || !pMyObject->hTlsConnection ) {
        		AnscSleep(100);
        		continue;
        	}
	    	s_result = openssl_poll (pMyObject->Socket, ANSC_SCTO_POLL_INTERVAL_MS*1000, 1, pMyObject->hTlsConnection);
        }
        else
        {
        	s_result = openssl_select_fd (pMyObject->Socket, ANSC_SCTO_POLL_INTERVAL_MS*1000, 1);
        }

    	if (s_result == 0)
    	{
        	if ( i++ > SCTO_MAX_RECV_RETRY_COUNT ) {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_SOCKET_TIMEOUT,
                            (ANSC_HANDLE)NULL
                        );
        		break;
        	}
        	else {
				/* yield CPU */
        		AnscSleep(500);
				continue;
        	}
    	}
    	else if (s_result < 0)
    	{
    		AnscTrace("AnscSctoRecvTask - select() returned error %d\n", s_result);
            if ( !pMyObject->bTlsEnabled )
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_SOCKET_ERROR,
                            (ANSC_HANDLE)NULL
                        );
            }
            else
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_TLS_ERROR,
                            (ANSC_HANDLE)NULL
                        );

            }
    		break;
    	}

#else
        PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;
        PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = (PANSC_BUFFER_DESCRIPTOR       )NULL;
        int                             s_error      = 0;
        ansc_fd_set                     ansc_read_fd_set;
        ansc_timeval                    ansc_timeval;
        xskt_fd_set                     xskt_read_fd_set;
        xskt_timeval                    xskt_timeval;

        /*
         * Since the original bsd compatible socket api doesn't support asynchronous operation, the
         * nonblocking status polling is the best we can get. As a matter of fact, the current unix
         * and linux actually still don't support asynchronous notification on any socket operation.
         */
        if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
        {
            XSKT_SOCKET_FD_ZERO(&xskt_read_fd_set);
            XSKT_SOCKET_FD_SET ((XSKT_SOCKET)pMyObject->Socket, &xskt_read_fd_set);

            xskt_timeval.tv_sec  = (ANSC_SCTO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            xskt_timeval.tv_usec = (ANSC_SCTO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }
        else
        {
            ANSC_SOCKET_FD_ZERO(&ansc_read_fd_set);
            ANSC_SOCKET_FD_SET (pMyObject->Socket, &ansc_read_fd_set);

            ansc_timeval.tv_sec  = (ANSC_SCTO_POLL_INTERVAL_MS / 1000);          /* number of seconds      */
            ansc_timeval.tv_usec = (ANSC_SCTO_POLL_INTERVAL_MS % 1000) * 1000;   /* number of microseconds */
        }

        /*
         * The _ansc_select() function returns the total number of socket handles that are ready
         * and contained in the fd_set structures, zero if the time limit expired, or SOCKET_ERROR
         * if an error occurred. Upon return, the structures are updated to reflect the subset of
         * these sockets that meet the specified condition.
         */
        if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
        {
            s_result = _xskt_select(pMyObject->Socket + 1, &xskt_read_fd_set, NULL, NULL, &xskt_timeval);
        }
        else
        {
            s_result = _ansc_select(pMyObject->Socket + 1, &ansc_read_fd_set, NULL, NULL, &ansc_timeval);
        }

        if ( s_result == 0 )
        {
        	if ( i++ > SCTO_MAX_RECV_RETRY_COUNT ) {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_SOCKET_TIMEOUT,
                            (ANSC_HANDLE)NULL
                        );
        		break;
        	}
        	else {
				/* yield CPU */
        		AnscSleep(500);
				continue;
        	}
        }
        else if ( ( (pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET) && (s_result == XSKT_SOCKET_ERROR)) ||
                  (!(pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET) && (s_result == ANSC_SOCKET_ERROR)) )
        {
            s_error = (pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

            if ( pMyObject->bActive )
            {
                if ( !pMyObject->bTlsEnabled || pMyObject->bTlsConnected )
                {
                        pWorker->Notify
                            (
                                pWorker->hWorkerContext,
                                ANSC_SCTOWO_EVENT_SOCKET_ERROR,
                                (ANSC_HANDLE)NULL
                            );
                }
            }

            break;
        }
        else if ( !pMyObject->bActive )
        {
            break;
        }
#endif
	
		/* 
 		 * 	Recv retry count needs to be reset to 0. Otherwise, for a long session,
 		 * 	the count will be accumulated and may cause the socket to be closed
 		 * 	prematurely and expectedly.
 		 */
		i = 0;

        /*
         * The underlying socket layer has indicated that there's data available to be processed,
         * we first retrieve the data into our own buffer and then query the external worker object
         * for further instructions.
         */
#ifdef _ANSC_USE_OPENSSL_
        if ( pMyObject->bActive )
        {
            recv_buffer =
                pMyObject->GetRecvBuffer
                    (
                        (ANSC_HANDLE)pMyObject,
                        &ulBufferSize
                    );

            if ( !recv_buffer )
            {
        		AnscTrace("AnscSctoRecvTask - recv_buffer running out, peer addr = %.2d.%.2d.%.2d.%.2d, peer port = %d.\n", pMyObject->PeerAddress.Dot[0], pMyObject->PeerAddress.Dot[1], pMyObject->PeerAddress.Dot[2], pMyObject->PeerAddress.Dot[3], pMyObject->PeerPort);

#ifdef   _DEBUG /* debug code to see what we are receiving actually */
if ( TRUE )
{
    char    test_buf[8192];
    int     recv_size = 8192;
    

        if ( pMyObject->bTlsEnabled )
        {
            /* wait for SSL to negotiate */
            if ( !pMyObject->bTlsConnected || !pMyObject->hTlsConnection ) {
                AnscSleep(100);
                continue;
            }
            s_result = openssl_read (pMyObject->Socket, test_buf, recv_size, pMyObject->hTlsConnection);
        }
        else
        {
            s_result = _ansc_recv(pMyObject->Socket, test_buf, recv_size, 0);
        }

        if ( s_result > 0 )
        {
            AnscTrace("AnscSctoRecvTask - received following data from peer:\n");
            AnscPrintBinaryMessage(test_buf, s_result);
        }
        else if ( s_result == 0 )
        {
            AnscTrace("AnscSctoRecvTask - received 0 bytes, peer closed socket.\n");    
        }
        else
        {
            AnscTrace("AnscSctoRecvTask - socket error.\n");
        }
}
#endif

                if ( !pMyObject->bTlsEnabled || pMyObject->bTlsConnected )
                {
                        pWorker->Notify
                            (
                                pWorker->hWorkerContext,
                                ANSC_SCTOWO_EVENT_RESOURCES,
                                (ANSC_HANDLE)NULL
                            );
                }

                break;
            }
            else
            {
                recv_size = (int)ulBufferSize;
            }
        }

        if ( pMyObject->bTlsEnabled )
        {
        	/* wait for SSL to negotiate */
        	if ( !pMyObject->bTlsConnected || !pMyObject->hTlsConnection ) {
        		AnscSleep(100);
        		continue;
        	}
        	s_result = openssl_read (pMyObject->Socket, recv_buffer, recv_size, pMyObject->hTlsConnection);
        }
        else
        {
            s_result = _ansc_recv(pMyObject->Socket, recv_buffer, recv_size, 0);
        }

        if ( s_result > 0 )
        {
            recv_size = s_result;
        }
        else if ( s_result == 0 )
        {
            if ( TRUE  )
            {
					pWorker->Notify
						(
							pWorker->hWorkerContext,
							ANSC_SCTOWO_EVENT_SOCKET_CLOSED,
							(ANSC_HANDLE)NULL
						);

            }
			pMyObject->bClosed = TRUE;
    		AnscTrace("AnscSctoRecvTask - recv() returned zero/%d, socket closed.\n", recv_size);
			break;
        }
        else
        {
    		AnscTrace("AnscSctoRecvTask - recv() returned error %d, %s\n", s_result, strerror (_ansc_get_last_error()));

            if ( !pMyObject->bTlsEnabled )
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_SOCKET_ERROR,
                            (ANSC_HANDLE)NULL
                        );
            }
            else
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_TLS_ERROR,
                            (ANSC_HANDLE)NULL
                        );

            }

            break;
        }

#else
       if ( !pMyObject->bTlsEnabled )
       {
           recv_buffer =
               pMyObject->GetRecvBuffer
                   (
                       (ANSC_HANDLE)pMyObject,
                       &ulBufferSize
                   );

           if ( !recv_buffer )
           {
        	   AnscTrace("AnscSctoRecvTask - recv_buffer running out.\n");
               if ( !pMyObject->bTlsEnabled || pMyObject->bTlsConnected )
               {
                       pWorker->Notify
                           (
                               pWorker->hWorkerContext,
                               ANSC_SCTOWO_EVENT_RESOURCES,
                               (ANSC_HANDLE)NULL
                           );
               }

               break;
           }
           else
           {
               recv_size = (int)ulBufferSize;
           }
       }
        else
        {
            /*
             * We need to give the TLS module sometime to initialize since there's a slight gap
             * between the creation of this task and the initialization of the TLS connection.
             */
            if ( pMyObject->bBroken || !pMyObject->hTlsConnection )
            {
                continue;
            }
            else if ( !pTlsScsIf->QueryConnection(pTlsScsIf->hOwnerContext, pMyObject->hTlsConnection) )
            {
                continue;
            }

            pBufferDesp = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo(TLS_MAX_RECORD_CHUNK_SIZE, 0, 0);

            if ( !pBufferDesp )
            {
                if ( !pMyObject->bTlsEnabled || pMyObject->bTlsConnected )
                {
                        pWorker->Notify
                            (
                                pWorker->hWorkerContext,
                                ANSC_SCTOWO_EVENT_RESOURCES,
                                (ANSC_HANDLE)NULL
                            );
                }

                break;
            }
            else
            {
                recv_buffer = AnscBdoGetBuffer    (pBufferDesp);
                recv_size   = AnscBdoGetBufferSize(pBufferDesp);
            }
        }

        if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
        {
            s_result = _xskt_recv((XSKT_SOCKET)pMyObject->Socket, recv_buffer, recv_size, 0);
        }
        else
        {
            s_result = _ansc_recv(pMyObject->Socket, recv_buffer, recv_size, 0);
        }

        if ( ( (pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET) && (s_result == XSKT_SOCKET_ERROR)) ||
             (!(pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET) && (s_result == ANSC_SOCKET_ERROR)) )
        {
            s_error = (pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET)? _xskt_get_last_error() : _ansc_get_last_error();

            if ( pMyObject->bTlsEnabled )
            {
                AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
            }

            if ( !pMyObject->bActive )
            {
                break;
            }

            if ( !pMyObject->bTlsEnabled || pMyObject->bTlsConnected )
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_SOCKET_ERROR,
                            (ANSC_HANDLE)NULL
                        );
            }

            break;
        }
        else if ( s_result == 0 )
        {
            if ( pMyObject->bTlsEnabled )
            {
                AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
            }

            if ( !pMyObject->bActive )
            {
                break;
            }

            if ( !pMyObject->bTlsEnabled || pMyObject->bTlsConnected )
            {
                    pWorker->Notify
                        (
                            pWorker->hWorkerContext,
                            ANSC_SCTOWO_EVENT_SOCKET_CLOSED,
                            (ANSC_HANDLE)NULL
                        );
            }

            pMyObject->bClosed = TRUE;

            break;
        }
        else if ( !pMyObject->bActive )
        {
            if ( pMyObject->bTlsEnabled )
            {
                AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
            }

            break;
        }
        else
        {
            recv_size = s_result;
        }
#endif
        /*
         * We have successfully transferred the received data into the buffer supplied by the data
         * owener though may not use up the whole buffer. Now is time to notify our loyal socket
         * owner about this exciting event.
         */
#ifdef _ANSC_USE_OPENSSL_
        if ( TRUE )
        {
            pMyObject->RecvBytesCount += (ULONG)recv_size;
            pMyObject->LastRecvAt      = AnscGetTickInSeconds();

                pMyObject->Recv
                    (
                        (ANSC_HANDLE)pMyObject,
                        recv_buffer,
                        (ULONG)recv_size
                    );
        }
#else
        if ( !pMyObject->bTlsEnabled )
        {
            pMyObject->RecvBytesCount += (ULONG)recv_size;
            pMyObject->LastRecvAt      = AnscGetTickInSeconds();

                pMyObject->Recv
                    (
                        (ANSC_HANDLE)pMyObject,
                        recv_buffer,
                        (ULONG)recv_size
                    );
        }
        else
        {
            pBufferDesp->BlockSize += recv_size;
                pMyObject->Recv2
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pBufferDesp
                    );
        }
#endif
    }

    return  ANSC_STATUS_SUCCESS;
}
