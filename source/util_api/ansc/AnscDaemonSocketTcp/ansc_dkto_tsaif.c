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

    module:	ansc_dkto_tsaif.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        Ansc Daemon Socket Tcp Object.

        *   AnscDktoTsaRecvAppMessage
        *   AnscDktoTsaSendAppMessage
        *   AnscDktoTsaNotifyEvent

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/13/03    initial revision.

**********************************************************************/


#include "ansc_dkto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoTsaRecvAppMessage
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessageBdo
            );

    description:

        This function is called by the TLS connection controller to
        indicate an application message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessageBdo
                Specifies the buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoTsaRecvAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->hDaemonEngine;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;
    PANSC_BUFFER_DESCRIPTOR         pPayloadBdo  = (PANSC_BUFFER_DESCRIPTOR       )hMessageBdo;
    void*                           pRecvBuffer  = (void*                         )NULL;
    ULONG                           ulRecvSize   = (ULONG                         )0;
    ULONG                           ulLeftSize   = (ULONG                         )0;
    ULONG                           ulCopySize   = (ULONG                         )0;
    ULONG                           ulServingT1  = 0;
    ULONG                           ulServingT2  = 0;
    ULONG                           ulWaitCount  = 0;

    /*
     * Since the TLS handshake is done in the asynchronous task instead of the socket_recv task,
     * it's possible that we start receiving TLS client data traffic (i.e. HTTP) before handshake
     * task considers the handshaking is done. To accommodate such race condition, we need to sleep
     * for a while if there's no buffer available.
     */
    while ( !pMyObject->GetRecvBuffer
                (
                    (ANSC_HANDLE)pMyObject,
                    &ulRecvSize
                ) )
    {
        AnscSleep(10);

        ulWaitCount++;

        if ( ulWaitCount > 30 ) /* sleep for 300ms */
        {
            returnStatus = ANSC_STATUS_INTERNAL_ERROR;

            goto  EXIT1;
        }
    }

    /*RDKB-6182, CID-24214, null check before use*/
    if(pPayloadBdo)
    {
        ulLeftSize   = (ULONG                         )AnscBdoGetBlockSize(pPayloadBdo);
    }
    else
    {
        returnStatus = ANSC_STATUS_BAD_PAYLOAD; /*advice appropriate error*/
        goto  EXIT1;
    }


    while ( ulLeftSize > 0 )
    {
        pRecvBuffer =
            pMyObject->GetRecvBuffer
                (
                    (ANSC_HANDLE)pMyObject,
                    &ulRecvSize
                );

        if ( !pRecvBuffer )
        {
            returnStatus =
                pWorker->Notify
                    (
                        pWorker->hWorkerContext,
                        (ANSC_HANDLE)pMyObject,
                        ANSC_DSTOWO_EVENT_RESOURCES,
                        (ANSC_HANDLE)NULL
                    );

            break;
        }
        else if ( ulRecvSize == 0 )
        {
            pMyObject->ToClean((ANSC_HANDLE)pMyObject, TRUE, 2);

            break;
        }

        if ( ulRecvSize >= ulLeftSize )
        {
            ulCopySize = ulLeftSize;
        }
        else
        {
            ulCopySize = ulRecvSize;
        }

        if ( TRUE )
        {
            AnscCopyMemory
                (
                    pRecvBuffer,
                    AnscBdoGetBlock(pPayloadBdo),
                    ulCopySize
                );

            AnscBdoShrinkRight(pPayloadBdo, ulCopySize);

            ulLeftSize -= ulCopySize;
        }

        pMyObject->RecvBytesCount += ulCopySize;
        pMyObject->LastRecvAt      = AnscGetTickInSecondsAbs();

        /*
         * We have successfully transferred the received data into the buffer supplied by the
         * socket owener though may not use up the while buffer. Now is time to notify our loyal
         * socket owner about this exciting event.
         */
        ulServingT1  = AnscGetTickInMilliSeconds();
        returnStatus =
            pMyObject->Recv
                (
                    (ANSC_HANDLE)pMyObject,
                    pRecvBuffer,
                    ulCopySize
                );
        ulServingT2  = AnscGetTickInMilliSeconds();

        pEngine->AvgServingTime =
            (pEngine->AvgServingTime == 0)? (ulServingT2 - ulServingT1) : ((ulServingT2 - ulServingT1) + pEngine->AvgServingTime * 7) / 8;
        pEngine->TscCount++;
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pPayloadBdo )
    {
        AnscFreeBdo((ANSC_HANDLE)pPayloadBdo);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoTsaSendTlsMessage
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hMessageBdo
            );

    description:

        This function is called by the TLS connection controller to
        send out a TLS encapsulated message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hMessageBdo
                Specifies the buffer descriptor to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoTsaSendTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->hDaemonEngine;
    PANSC_BUFFER_DESCRIPTOR         pPayloadBdo  = (PANSC_BUFFER_DESCRIPTOR       )hMessageBdo;

    if ( pMyObject->bClosed || pMyObject->bBroken )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pEngine->Send2
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pMyObject,
                AnscBdoGetBlock    (pPayloadBdo),
                AnscBdoGetBlockSize(pPayloadBdo),
                (ANSC_HANDLE)NULL
            );

    AnscFreeBdo((ANSC_HANDLE)pPayloadBdo);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoTsaNotifyEvent
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulEvent,
                ULONG                       ulError,
                ANSC_HANDLE                 hEventContext
            );

    description:

        This function is called by the TLS connection controller to
        indicate an event.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulEvent
                Specifies the event to be processed.

                ULONG                       ulError
                Specifies the error code associated with the event.

                ANSC_HANDLE                 hEventContext
                Specifies the event-specific processing context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoTsaNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    
#ifndef _ANSC_USE_OPENSSL_
    
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->hDaemonEngine;
    PANSC_DSTO_WORKER_OBJECT        pWorker      = (PANSC_DSTO_WORKER_OBJECT      )pServer->hWorker;

    switch ( ulEvent )
    {
        case    TLS_TSA_EVENT_CONNECTION_ESTABLISHED :

                pMyObject->bTlsConnected = TRUE;

                AnscSetEvent(&pMyObject->TlsConnEvent);

                break;

        case    TLS_TSA_EVENT_CONNECTION_CLOSED     :
        case    TLS_TSA_EVENT_CONNECTION_TERMINATED :
        case    TLS_TSA_EVENT_CONNECTION_TIMEOUT    :

                pMyObject->bTlsConnected = FALSE;

                AnscSetEvent(&pMyObject->TlsConnEvent);

                break;

        default :

                break;
    }
#endif
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulEvent);
    UNREFERENCED_PARAMETER(ulError);
    UNREFERENCED_PARAMETER(hEventContext); 
    return  returnStatus;
}
