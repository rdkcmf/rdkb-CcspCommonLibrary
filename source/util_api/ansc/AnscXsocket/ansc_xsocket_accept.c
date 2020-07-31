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

    module:	xskt_xsocket_accept.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xsocket Object.

        *   AnscXsocketAcceptTask

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

    caller:     created by the listen() member function

    prototype:

        ANSC_STATUS
        AnscXsocketAcceptTask
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This task is spawned by the previous call of XsocketListen()
        when the xsocket owner is ready to accept incoming connection.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsocketAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_XSOCKET_OBJECT            pMyObject    = (PANSC_XSOCKET_OBJECT)hThisObject;
    PANSC_XSINK_OBJECT              pXsinkHolder = (PANSC_XSINK_OBJECT  )pMyObject->hXsinkHolder;
    PANSC_XSOCKET_OBJECT            pNewXsocket  = NULL;
    int                             s_result     = 0;
    xskt_fd_set                     read_fd_set;
    xskt_timeval                    timeval;
    xskt_socket_addr_in             client_addr;
    int                             addrlen;

    AnscTrace("AnscXsocketAcceptTask is activated ...!\n");

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
        }

        /*
         * According to de facto standards of bsd compatible xsocket api, if the xsocket is currently
         * in the listen state, it will be marked as readable if an incoming connection request has
         * been received such that an accept is guaranteed to complete without blocking.
         */
        client_addr.sin_family      = XSKT_SOCKET_AF_INET;
        client_addr.sin_addr.s_addr = pMyObject->PeerAddress.Value;
        client_addr.sin_port        = _xskt_htons(pMyObject->PeerPort);
        addrlen                     = sizeof(client_addr);

        pNewXsocket =
            (PANSC_XSOCKET_OBJECT)AnscCreateXsocket
                (
                    pMyObject->hContainerContext,
                    pMyObject->hOwnerContext,
                    (ANSC_HANDLE)NULL
                );

        if ( !pNewXsocket )
        {
            continue;
        }
        else
        {
            pNewXsocket->Xsocket = _xskt_accept(pMyObject->Xsocket, (xskt_socket_addr*)&client_addr, (unsigned int*)&addrlen);
        }

        if ( pNewXsocket->Xsocket == XSKT_SOCKET_INVALID_SOCKET )
        {

            pNewXsocket->Remove((ANSC_HANDLE)pNewXsocket);

            continue;
        }
        else
        {
            pNewXsocket->bClosed = FALSE;
        }

        /*
         * We have created a new xsocket object for the incoming connection, before notifying the
         * xsocket owner via sink interface, we have to duplicate the content of the original xsocket
         * object. Although the duplication seems irrelevant in most cases, some component objects
         * may find it's very convenient to be able to retrieve peer's address information from the
         * newly created xsocket object.
         */
        pNewXsocket->Type              = pMyObject->Type;
        pNewXsocket->hXsinkObject      = (ANSC_HANDLE)NULL;
        pNewXsocket->HostAddress.Value = pMyObject->HostAddress.Value;
        pNewXsocket->HostPort          = pMyObject->HostPort;
        pNewXsocket->PeerAddress.Value = client_addr.sin_addr.s_addr;
        pNewXsocket->PeerPort          = _xskt_ntohs(client_addr.sin_port);

        returnStatus = pXsinkHolder->Accept((ANSC_HANDLE)pXsinkHolder, (ANSC_HANDLE)pNewXsocket);

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            pNewXsocket->Remove((ANSC_HANDLE)pNewXsocket);

            continue;
        }

        /*
         * The master "listening" xsocket doesn't need to receive any data since a new xsocket is
         * always created for the "real" connection. We need explicitly create a separate receive
         * task to receive data on the newly created xsocket by calling the open() member function.
         */
        pNewXsocket->Open((ANSC_HANDLE)pNewXsocket);
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
