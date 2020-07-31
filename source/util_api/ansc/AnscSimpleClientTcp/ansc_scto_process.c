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

    module:	ansc_scto_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Simple Client Tcp Object.

        *   AnscSctoGetRecvBuffer
        *   AnscSctoRecv
        *   AnscSctoRecv2
        *   AnscSctoSend
        *   AnscSctoSend2

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        PVOID
        AnscSctoGetRecvBuffer
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
AnscSctoGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
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
        AnscSctoRecv
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
AnscSctoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    ULONG                           ulPmode      = ANSC_SCTOWO_PMODE_DISCARD;
    ULONG                           ulPacketSize = pMyObject->RecvPacketSize;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pMyObject->RecvPacketSize += ulSize;
    pMyObject->RecvOffset     += ulSize;

    ulPmode =
        pWorker->Query
            (
                pWorker->hWorkerContext,
                pMyObject->RecvBuffer,
                pMyObject->RecvPacketSize
            );

    switch ( ulPmode )
    {
        case    ANSC_SCTOWO_PMODE_DISCARD :

                returnStatus              = ANSC_STATUS_SUCCESS;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                break;

        case    ANSC_SCTOWO_PMODE_COLLECT :

                returnStatus = ANSC_STATUS_SUCCESS;

                break;

        case    ANSC_SCTOWO_PMODE_PROCESS :

                ulPacketSize              = pMyObject->RecvPacketSize;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                returnStatus =
                    pWorker->Process
                        (
                            pWorker->hWorkerContext,
                            pMyObject->RecvBuffer,
                            ulPacketSize
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

        default :

                returnStatus              = ANSC_STATUS_SUCCESS;
                pMyObject->RecvPacketSize = 0;
                pMyObject->RecvOffset     = 0;

                break;
    }

    return  returnStatus;
}

#ifndef _ANSC_USE_OPENSSL_

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoRecv2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hPayloadBdo
            );

    description:

        This function is called to process an incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hPayloadBdo
                Specifies the buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoRecv2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPayloadBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = (PANSC_BUFFER_DESCRIPTOR       )hPayloadBdo;

    if ( !pMyObject->bTlsEnabled )
    {
        return  pMyObject->Recv
                    (
                        (ANSC_HANDLE)pMyObject,
                        AnscBdoGetBlock    (pBufferDesp),
                        AnscBdoGetBlockSize(pBufferDesp)
                    );
    }
    else if ( !pTlsScsIf || !pBufferDesp || pMyObject->bBroken || !pMyObject->hTlsConnection )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pTlsScsIf->RecvTlsMessage
                (
                    pTlsScsIf->hOwnerContext,
                    pMyObject->hTlsConnection,
                    (ANSC_HANDLE)pBufferDesp
                );
    }

    return  returnStatus;
}
#endif

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSend
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
AnscSctoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

#ifdef _ANSC_USE_OPENSSL_
    return  pMyObject->Send2
                (
                    (ANSC_HANDLE)pMyObject,
                    buffer,
                    ulSize,
                    hReserved
                );
#else
    if ( !pMyObject->bTlsEnabled )
    {
        pMyObject->SendBytesCount += ulSize;
        pMyObject->LastSendAt      = AnscGetTickInSeconds();

        return  pMyObject->Send2
                    (
                        (ANSC_HANDLE)pMyObject,
                        buffer,
                        ulSize,
                        hReserved
                    );
    }
    else if ( !pMyObject->bTlsConnected || !pMyObject->hTlsScsIf )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        PTLS_SCS_INTERFACE              pTlsScsIf    = (PTLS_SCS_INTERFACE            )pMyObject->hTlsScsIf;
        PANSC_BUFFER_DESCRIPTOR         pBufferDesp  = NULL;
        pBufferDesp = (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo(0, 0, 0);

        if ( !pBufferDesp )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            AnscBdoSetOwner     (pBufferDesp, FALSE );
            AnscBdoSetBuffer    (pBufferDesp, buffer);
            AnscBdoSetBufferSize(pBufferDesp, ulSize);
            AnscBdoSetOffset    (pBufferDesp, 0     );
            AnscBdoSetBlockSize (pBufferDesp, ulSize);
        }

        pMyObject->SendBytesCount += ulSize;
        pMyObject->LastSendAt      = AnscGetTickInSeconds();

        returnStatus =
            pTlsScsIf->SendAppMessage
                (
                    pTlsScsIf->hOwnerContext,
                    pMyObject->hTlsConnection,
                    (ANSC_HANDLE)pBufferDesp
                );
    }
#endif

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSend2
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
AnscSctoSend2
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    int                             s_result     = 0;
    int                             s_error      = 0;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

#ifdef _ANSC_USE_OPENSSL_
    while ( ulSize > 0)
    {
        if ( pMyObject->bTlsEnabled )
        {
        	if ( !pMyObject->bTlsConnected || !pMyObject->hTlsConnection ) {
        		AnscTrace("AnscSctoSend2 - SSL connection is broken.\n");
                returnStatus = ANSC_STATUS_FAILURE;
        		break;
        	}
        	s_result = openssl_write (pMyObject->Socket, buffer, (int)ulSize, pMyObject->hTlsConnection);
        }
        else
        {
        	s_result = _ansc_send (pMyObject->Socket, buffer, (int)ulSize, 0);
        }

    	if (s_result <= 0)
    	{
    		AnscTrace("AnscSctoSend2 - send() returned error %d, %s\n", s_result, strerror (_ansc_get_last_error()));
    		break;
    	}

    	/*
    	else
    		AnscTrace("AnscSctoSend2 - send() %d/%d bytes.\n", s_result, (int)ulSize);
    	*/

    	buffer += s_result;
    	ulSize -= s_result;
    }
#else
    if ( pMyObject->Mode & ANSC_SCTO_MODE_XSOCKET )
    {
        s_result = _xskt_send(((XSKT_SOCKET)pMyObject->Socket), buffer, (int)ulSize, 0);

        if ( s_result == XSKT_SOCKET_ERROR )
        {
            s_error = _xskt_get_last_error();

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        s_result = _ansc_send(pMyObject->Socket, buffer, (int)ulSize, 0);

        if ( s_result == ANSC_SOCKET_ERROR )
        {
            s_error = _ansc_get_last_error();

            returnStatus = ANSC_STATUS_FAILURE;
        }
        else
        {
            returnStatus = ANSC_STATUS_SUCCESS;
        }
    }
#endif

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscTrace("AnscSctoSend2: CANNOT sent out data over TCP socket, error = %d!\n", s_error);

        if ( !pMyObject->bTlsEnabled )
        {
            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCTOWO_EVENT_SOCKET_ERROR,
                        (ANSC_HANDLE)NULL
                    );
        }
        else
        {
			returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        ANSC_SCTOWO_EVENT_TLS_ERROR,
                        (ANSC_HANDLE)NULL
                    );

        }
    }
    else
    {
        /*
        pMyObject->SendBytesCount += ulSize;
        pMyObject->LastSendAt      = AnscGetTickInSeconds();
        */
    }

    return  returnStatus;
}
