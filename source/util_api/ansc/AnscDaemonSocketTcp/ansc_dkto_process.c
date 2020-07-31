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

    module:	ansc_dkto_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Daemon Socket Tcp Object.

        *   AnscDktoGetRecvBuffer
        *   AnscDktoRecv
        *   AnscDktoSend

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


#include "ansc_dkto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        PVOID
        AnscDktoGetRecvBuffer
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve a data buffer for holding
        incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PULONG                      pulSize
                Specifies the size of the available buffer.

    return:     buffer pointer.

**********************************************************************/

PVOID
AnscDktoGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    if ( pMyObject->bClosed )
    {
        return  NULL;
    }
    else if ( !pMyObject->RecvBuffer )
    {
        return  NULL;
    }

    *pulSize = pMyObject->RecvBufferSize - pMyObject->RecvOffset;

    return  (PVOID)((ULONG)pMyObject->RecvBuffer + pMyObject->RecvOffset);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoRecv
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the pointer to packet data buffer.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject     = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer       = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    ANSC_HANDLE                     hQueryContext = (ANSC_HANDLE)NULL;
    ULONG                           ulPmode       = ANSC_DSTOWO_PMODE_DISCARD;
    ULONG                           ulPacketSize  = pMyObject->RecvPacketSize;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pMyObject->RecvPacketSize += ulSize;
    pMyObject->RecvOffset     += ulSize;

    ulPmode =
        pServer->Query
            (
                (ANSC_HANDLE)pServer,
                (ANSC_HANDLE)pMyObject,
                pMyObject->RecvBuffer,
                pMyObject->RecvPacketSize,
                &hQueryContext
            );

    switch ( ulPmode )
    {
        case    ANSC_DSTOWO_PMODE_DISCARD :

                returnStatus              = ANSC_STATUS_SUCCESS;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                break;

        case    ANSC_DSTOWO_PMODE_COLLECT :

                returnStatus = ANSC_STATUS_SUCCESS;

                break;

        case    ANSC_DSTOWO_PMODE_PROCESS_SYNC :

                ulPacketSize              = pMyObject->RecvPacketSize;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                returnStatus =
                    pServer->ProcessSync
                        (
                            (ANSC_HANDLE)pServer,
                            (ANSC_HANDLE)pMyObject,
                            pMyObject->RecvBuffer,
                            ulPacketSize,
                            hQueryContext
                        );

                /*
                 * Here's an interesting trick can be played by client object: returning a special
                 * status "ansc_status_do_it_again" will enable the socket object to go through the
                 * worker_query() and worker_process() again. The need for such weird behavior is
                 * demanded by the fact that the interaction between the socket and client object
                 * is event-triggered.
                 *
                 * For instance, HTTP/1.1 supports message pipe-line, which means that client may
                 * send multiple requests at once without waiting for single response. The message
                 * processing object needs to triggered for every message. This feature is useful
                 * for pulling triggers if the message boundaries were not preserved during the
                 * receiving operation.
                 *
                 * However, this feature MUST be used with great caution since it may lead to state
                 * confusion and endless loop. In addition, client object MUST set the a non-zeroed
                 * recv_packet_size field before returning such status. Currently there's no API
                 * function provided for doing that, so the client is required to modify the data
                 * member directly.
                 */
                if ( returnStatus == ANSC_STATUS_DO_IT_AGAIN )
                {
                    if ( pMyObject->RecvPacketSize != 0 )
                    {
                        buffer                    = pMyObject->RecvBuffer;
                        ulSize                    = pMyObject->RecvPacketSize;
                        pMyObject->RecvPacketSize = 0;
                        pMyObject->RecvOffset     = 0;

                        returnStatus =
                            pMyObject->Recv
                                (
                                    (ANSC_HANDLE)pMyObject,
                                    buffer,
                                    ulSize
                                );
                    }
                }

                break;

        case    ANSC_DSTOWO_PMODE_PROCESS_ASYNC :

                returnStatus =
                    pServer->ProcessAsync
                        (
                            (ANSC_HANDLE)pServer,
                            (ANSC_HANDLE)pMyObject,
                            pMyObject->RecvBuffer,
                            pMyObject->RecvPacketSize,
                            hQueryContext
                        );

                break;

        case    ANSC_DSTOWO_PMODE_FINISH :

                returnStatus = pMyObject->Finish((ANSC_HANDLE)pMyObject);

                break;

        default :

                returnStatus              = ANSC_STATUS_SUCCESS;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                break;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSend
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to process an outgoing packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the pointer to packet data buffer.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->hDaemonEngine;

    if ( pMyObject->bClosed || pMyObject->bBroken )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    if ( pMyObject->bTlsConnected )
    {
        AnscAcquireLock(&pMyObject->TlsConnLock);

        if ( !pMyObject->hTlsConnection )
        {
            AnscReleaseLock(&pMyObject->TlsConnLock);

            return  ANSC_STATUS_FAILURE;
        }

        returnStatus =
            pEngine->Send
                (
                    (ANSC_HANDLE)pEngine,
                    (ANSC_HANDLE)pMyObject,
                    buffer,
                    ulSize,
                    hReserved
                );

        AnscReleaseLock(&pMyObject->TlsConnLock);
    }
    else
    {
        returnStatus =
            pEngine->Send
                (
                    (ANSC_HANDLE)pEngine,
                    (ANSC_HANDLE)pMyObject,
                    buffer,
                    ulSize,
                    hReserved
                );
    }

    return  returnStatus;
}
