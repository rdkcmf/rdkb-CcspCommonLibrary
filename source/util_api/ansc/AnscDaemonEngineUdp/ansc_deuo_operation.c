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

    module:	ansc_deuo_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Daemon Engine Udp Object.

        *   AnscDeuoStart
        *   AnscDeuoStop
        *   AnscDeuoClean

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


#include "ansc_deuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoStart
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start the engine running.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    if ( pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bStarted = TRUE;
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    if ( TRUE )
    {
        AnscResetEvent(&pMyObject->RecvEvent);
            AnscSpawnTask
                (
                    (void*)pMyObject->RecvTask,
                    (ANSC_HANDLE)pMyObject,
                    ANSC_DEUO_RECV_TASK_NAME
                );
    }

    if ( pMyObject->ControlFlags & ANSC_DEUO_FLAG_ASYNC_SEND )
    {
        AnscResetEvent(&pMyObject->SendEvent);
            AnscSpawnTask
                (
                    (void*)pMyObject->SendTask,
                    (ANSC_HANDLE)pMyObject,
                    ANSC_DEUO_SEND_TASK_NAME
                );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoStop
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to stop the engine running.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoStop
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    if ( !pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bStarted = FALSE;
    }

    AnscWaitEvent(&pMyObject->RecvEvent, ANSC_DEUO_TASK_CLEANUP_TIME);
    AnscWaitEvent(&pMyObject->SendEvent, ANSC_DEUO_TASK_CLEANUP_TIME);

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoClean
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to clean out the socket table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoClean
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_UDP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pSocket      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = NULL;
    ULONG                           ulHashIndex  = 0;
    BOOL                            bSocketFound = FALSE;
    ULONG                           ulCurTime    = AnscGetTickInSeconds();

    if ( !pMyObject->bStarted )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pMyObject->ControlFlags & ANSC_DEUO_FLAG_NO_TIMEOUT )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    ulHashIndex  = 0;
    bSocketFound = FALSE;

    while ( ulHashIndex < ANSC_DEUO_SOCKET_TABLE_SIZE )
    {
        AnscAcquireLock(&pMyObject->SocketTableLock);

        bSocketFound = FALSE;
        pSLinkEntry  = AnscSListGetFirstEntry(&pMyObject->SocketTable[ulHashIndex]);

        while ( pSLinkEntry )
        {
            pSocket     = ACCESS_ANSC_DAEMON_SOCKET_UDP_OBJECT(pSLinkEntry);
            pSLinkEntry = AnscSListGetNextEntry(pSLinkEntry);

            if ( ((ulCurTime - pSocket->LastRecvAt) >= pMyObject->SocketTimeOut) &&
                 ((ulCurTime - pSocket->LastSendAt) >= pMyObject->SocketTimeOut) )
            {
                bSocketFound = TRUE;

                break;
            }
        }

        AnscReleaseLock(&pMyObject->SocketTableLock);

        if ( bSocketFound )
        {
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pSocket,
                        ANSC_DSUOWO_EVENT_TIME_OUT,
                        (ANSC_HANDLE)NULL
                    );

                pMyObject->DelSocket
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pSocket
                    );
        }
        else
        {
            ulHashIndex++;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
