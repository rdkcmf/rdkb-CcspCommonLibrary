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

    module:	ansc_dsuo_worker.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Daemon Server Udp Object.

        *   AnscDsuoWorkerTask

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
        AnscDsuoWorkerTask
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
AnscDsuoWorkerTask
    (
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)pSocket->hDaemonServer;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_UDP_OBJECT)pSocket->hDaemonEngine;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = (PANSC_DSUO_PACKET_OBJECT      )pSocket->hPacket;

    if ( !pPacket )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pWorker->ProcessAsync
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pSocket,
                pPacket->RecvBuffer,
                pPacket->RecvPacketSize
            );

    if ( pPacket )
    {
        pSocket->SetPacket((ANSC_HANDLE)pSocket, (ANSC_HANDLE)NULL);

        returnStatus =
            pMyObject->ReleasePacket
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPacket
                );
    }

    returnStatus =
        pEngine->EnableRecv
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pSocket,
                TRUE
            );

    return  returnStatus;
}
