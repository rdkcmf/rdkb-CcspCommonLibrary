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

    module:	ansc_xsocket_recv.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xsocket Object.

        *   AnscXsocketRecvTask

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/02    initial revision.

**********************************************************************/


#include "ansc_xsocket_global.h"


/**********************************************************************

    caller:     created by the open() member function

    prototype:

        ANSC_STATUS
        AnscXsocketRecvTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This task is spawned by the previous call of XsocketOpen() when
        the xsocket owner is ready to receive data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketRecvTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject     = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder  = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;
    ANSC_HANDLE                     hRecvHandle   = (ANSC_HANDLE         )NULL;
    char*                           recv_buffer   = NULL;
    ULONG                           ulBufferSize  = 0;
    int                             recv_size     = 0;
    ULONG                           ulRetryCounts = 0;
    int                             s_result      = 0;
    xskt_fd_set                     read_fd_set;
    xskt_timeval                    timeval;
    /*xskt_socket_addr_in             peer_addr;*/
    xskt_addrinfo*                  pXsktAddrInfo = (xskt_addrinfo*)pMyObject->pPeerAddrInfo;

    AnscTrace("AnscXsocketRecvTask is activated ...!\n");

    /*
     * It's critical that the xsocket owner has already associated a sink object interface with the
     * current xsocket object. The interface exposed by the sink object is the only mechanism to
     * inform the xsocket owner that the xsocket state has changed or data has arrived.
     */
    if ( !pXsinkHolder )
    {
        return  ANSC_STATUS_FAILURE;
    }

    /*
     * As a scalable server implemention, we shall accept as many incoming client connections as
     * possible and can only be limited by the system resources. Once the listening xsocket becomes
     * readable, which means an incoming connection attempt has arrived. We create a new xsocket
     * object and associate it with the client. This is a repeated process until the xsocket owner
     * closes the xsocket.
     */
    while ( !pMyObject->bClosed )
    {
        ANSC_COMMIT_TASK();

        /*
         * Since the original bsd compatible xsocket api doesn't support asynchronous operation, the
         * nonblocking status polling is the best we can get. As a matter of fact, the current unix
         * and linux actually still don't support asynchronous notification on any xsocket operation.
         */
        XSKT_SOCKET_FD_ZERO(&read_fd_set);
        XSKT_SOCKET_FD_SET (pMyObject->Xsocket, &read_fd_set);

        timeval.tv_sec  = ANSC_XSOCKET_POLL_INTERVAL;
        timeval.tv_usec = 0;

        /*
         * Since only one xsocket is included in the fd_set, we only distinguish the result between
         * one and non-one values. If error is detected, we shall close the xsocket and notify the
         * xsocket owner immediately.
         */
        s_result = _xskt_select(pMyObject->Xsocket + 1, &read_fd_set, NULL, NULL, &timeval);

        if ( s_result == 0 )
        {
            continue;
        }
        else if ( s_result == XSKT_SOCKET_ERROR )
        {

            if ( pMyObject->bClosed )
            {
                break;
            }
            else if ( pMyObject->Mode & ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE )
            {
                pXsinkHolder->Abort ((ANSC_HANDLE)pXsinkHolder);
                pMyObject   ->Remove((ANSC_HANDLE)pMyObject   );

                return  ANSC_STATUS_SUCCESS;
            }
            else
            {
                AnscSleep(20);

                continue;
            }
        }
        else if ( pMyObject->bClosed )
        {
            break;
        }

        /*
         * Ask the associated sink object for more buffer to hold the incoming data. The sink
         * may return null if it couldn't allocate more resources temporarily. In this case, we
         * will wait for a while and ask again.
         */
        ulRetryCounts = 0;
        recv_buffer   = pXsinkHolder->GetRecvBuffer((ANSC_HANDLE)pXsinkHolder, &hRecvHandle, &ulBufferSize);

        while ( !recv_buffer && (ulRetryCounts < ANSC_XSOCKET_MAX_RECV_RETRY_COUNTS) )
        {
            AnscSleep(ANSC_XSOCKET_RECV_HOLD_BACK);

            recv_buffer = pXsinkHolder->GetRecvBuffer((ANSC_HANDLE)pXsinkHolder, &hRecvHandle, &ulBufferSize);
            ulRetryCounts++;
        }

        if ( !recv_buffer )
        {
            continue;
        }
        else
        {
            recv_size = (int)ulBufferSize;
        }

        /*
         * The receive procedure for Tcp and Udp are different, we will not repeat the difference
         * here since all kinds of xsocket api documentation has explained it very well. The only
         * thing worth noting is that we ignore the possibility that the recvfrom() request may be
         * rejected due to small buffer size. The xsocket owner should make sure that the buffer it
         * has allocated is big enough for all the Udp messages.
         */
        /*
        peer_addr.sin_family      = XSKT_SOCKET_AF_INET;
        peer_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
        peer_addr.sin_port        = _xskt_htons(pMyObject->PeerPort);
        addrlen                   = sizeof(peer_addr);
        */

        if ( pMyObject->Type == ANSC_XSOCKET_TYPE_TCP )
        {
            s_result = _xskt_recv(pMyObject->Xsocket, recv_buffer, recv_size, 0);
        }
        else if ( pMyObject->Type == ANSC_XSOCKET_TYPE_UDP )
        {
            s_result = _xskt_recvfrom(pMyObject->Xsocket, recv_buffer, recv_size, 0, pXsktAddrInfo->ai_addr, &pXsktAddrInfo->ai_addrlen);
        }
        else if ( pMyObject->Type == ANSC_XSOCKET_TYPE_RAW )
        {
            s_result = _xskt_recvfrom(pMyObject->Xsocket, recv_buffer, recv_size, 0, pXsktAddrInfo->ai_addr, &pXsktAddrInfo->ai_addrlen);
        }

        if ( s_result == XSKT_SOCKET_ERROR )
        {

            CcspTraceError(("recvfrom error %s\n", strerror(errno)));

            if ( pMyObject->bClosed )
            {
                break;
            }
            else if ( pMyObject->Mode & ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE )
            {
                pXsinkHolder->Abort ((ANSC_HANDLE)pXsinkHolder);
                pMyObject   ->Remove((ANSC_HANDLE)pMyObject   );

                return  ANSC_STATUS_SUCCESS;
            }
            else
            {
                continue;
            }
        }
        else if ( pMyObject->bClosed )
        {
            break;
        }
        else if ( s_result == 0 )
        {
            if ( pMyObject->Mode & ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE )
            {
                pXsinkHolder->Close ((ANSC_HANDLE)pXsinkHolder, TRUE);
                pMyObject   ->Remove((ANSC_HANDLE)pMyObject);

                return  ANSC_STATUS_SUCCESS;
            }
            else
            {
                pXsinkHolder->Close ((ANSC_HANDLE)pXsinkHolder, TRUE);
                pMyObject   ->Remove((ANSC_HANDLE)pMyObject);

                return  ANSC_STATUS_SUCCESS;
            }
        }
        else
        {
            recv_size = s_result;
        }

        /*
         * If the current xsocket is a Udp xsocket, each message may be sent from different remote
         * peers. Since we are trying to provide a unified xsocket interface for both Tcp and Udp,
         * there's currently no peer information associated with each Udp message (we may in the
         * future provide such interface however). So we reset the peer address information each
         * time a Udp message is received.
         */
         /*
        if ( pMyObject->Type == ANSC_XSOCKET_TYPE_UDP )
        {
            pMyObject->PeerAddress.Value = peer_addr.sin_addr.s_addr;
            pMyObject->PeerPort          = _xskt_ntohs(peer_addr.sin_port);
        }
        else if ( pMyObject->Type == ANSC_XSOCKET_TYPE_RAW )
        {
            pMyObject->PeerAddress.Value = peer_addr.sin_addr.s_addr;
            pMyObject->PeerPort          = _xskt_ntohs(peer_addr.sin_port);
        }
        */

        /*
         * We have successfully transferred the received data into the buffer supplied by the xsocket
         * owener though may not use up the while buffer. Now is time to notify our loyal xsocket
         * owner about this exciting event.
         */
        returnStatus =
            pXsinkHolder->Recv
                (
                    (ANSC_HANDLE)pXsinkHolder,
                    hRecvHandle,
                    recv_buffer,
                    (ULONG)recv_size
                );

        if ( (returnStatus != ANSC_STATUS_SUCCESS) && (returnStatus != ANSC_STATUS_PENDING) )
        {
            if ( pMyObject->bClosed )
            {
                break;
            }
            else if ( pMyObject->Mode & ANSC_XSOCKET_MODE_AUTOMATIC_CLOSE )
            {
                pMyObject->Close ((ANSC_HANDLE)pMyObject);
                pMyObject->Remove((ANSC_HANDLE)pMyObject);

                return  ANSC_STATUS_SUCCESS;
            }
            else
            {
                continue;
            }
        }
    }

    if ( pMyObject->bClosed )
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }
    else
    {
        returnStatus = ANSC_STATUS_DO_IT_AGAIN;
    }

    return  returnStatus;
}
