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

    module:	ansc_dsuo_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Daemon Server Udp Object.

        *   AnscDsuoQuery
        *   AnscDsuoProcessSync
        *   AnscDsuoProcessAsync

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

        ULONG
        AnscDsuoQuery
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to query the desired process mode.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which data is received.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     process mode.

**********************************************************************/

ULONG
AnscDsuoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;
    ULONG                           ulPmode      = ANSC_DSUOWO_PMODE_DISCARD;

    ulPmode =
        pWorker->Query
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pSocket,
                buffer,
                ulSize
            );

    return  ulPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoProcessSync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process incoming packets.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which data is received.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operatoin.

**********************************************************************/

ANSC_STATUS
AnscDsuoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;

    returnStatus =
        pWorker->ProcessSync
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pSocket,
                buffer,
                ulSize
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoProcessAsync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process incoming packets.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object on which data is received.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operatoin.

**********************************************************************/

ANSC_STATUS
AnscDsuoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hSocket;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_UDP_OBJECT)pSocket->hDaemonEngine;

    returnStatus =
        pEngine->EnableRecv
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pSocket,
                FALSE
            );

    returnStatus =
        AnscSpawnTask
            (
                (void*)pMyObject->WorkerTask,
                (ANSC_HANDLE)pSocket,
                ANSC_DSUO_WORKER_TASK_NAME
            );

    return  returnStatus;
}
