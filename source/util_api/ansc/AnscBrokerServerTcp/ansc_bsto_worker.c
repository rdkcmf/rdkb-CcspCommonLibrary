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

    module:	ansc_bsto_worker.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced async-task functions
        of the Ansc Broker Server Tcp Object.

        *   AnscBstoWorkerTask

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


#include "ansc_bsto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBstoWorkerTask
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
AnscBstoWorkerTask
    (
        ANSC_HANDLE                 hSocket
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pSocket      = (PANSC_BROKER_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BROKER_SERVER_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SERVER_TCP_OBJECT)pSocket->hBrokerServer;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = (PANSC_BROKER_ENGINE_TCP_OBJECT)pSocket->hBrokerEngine;
    PANSC_BSTO_WORKER_OBJECT        pWorker      = (PANSC_BSTO_WORKER_OBJECT      )pMyObject->hWorker;
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
