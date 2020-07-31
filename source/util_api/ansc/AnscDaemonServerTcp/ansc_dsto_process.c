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

    module:	ansc_dsto_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Daemon Server Tcp Object.

        *   AnscDstoQuery
        *   AnscDstoProcessSync
        *   AnscDstoProcessAsync

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

        ULONG
        AnscDstoQuery
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                PANSC_HANDLE                phQueryContext
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

                PANSC_HANDLE                phQueryContext
                Specifies a context handle related to query operation.

    return:     process mode.

**********************************************************************/

ULONG
AnscDstoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    )
{
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    ULONG                           ulPmode      = ANSC_DSTOWO_PMODE_DISCARD;

    ulPmode =
        pWorker->Query
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pSocket,
                buffer,
                ulSize,
                phQueryContext
            );

    return  ulPmode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoProcessSync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hQueryContext
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

                ANSC_HANDLE                 hQueryContext
                Specifies a context handle related to query operation.

    return:     status of operatoin.

**********************************************************************/

ANSC_STATUS
AnscDstoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;

    returnStatus =
        pWorker->ProcessSync
            (
                pWorker->hWorkerContext,
                (ANSC_HANDLE)pSocket,
                buffer,
                ulSize,
                hQueryContext
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDstoProcessAsync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hQueryContext
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

                ANSC_HANDLE                 hQueryContext
                Specifies a context handle related to query operation.

    return:     status of operatoin.

**********************************************************************/

ANSC_STATUS
AnscDstoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    UNREFERENCED_PARAMETER(hQueryContext);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pSocket      = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pSocket->hDaemonEngine;

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
                ANSC_DSTO_WORKER_TASK_NAME
            );

    return  returnStatus;
}
