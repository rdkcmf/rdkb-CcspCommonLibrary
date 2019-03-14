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

    module:	ansc_scto_tsaif.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        Ansc Simple Client Tcp Object.

        *   AnscSctoTsaRecvAppMessage
        *   AnscSctoTsaSendAppMessage
        *   AnscSctoTsaNotifyEvent

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/12/03    initial revision.

**********************************************************************/


#include "ansc_scto_global.h"

#ifndef _ANSC_USE_OPENSSL_

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoTsaRecvAppMessage
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
AnscSctoTsaRecvAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_BUFFER_DESCRIPTOR         pPayloadBdo  = (PANSC_BUFFER_DESCRIPTOR       )hMessageBdo;
    void*                           pRecvBuffer  = (void*                         )NULL;
    ANSC_HANDLE                     hRecvHandle  = (ANSC_HANDLE                   )NULL;
    ULONG                           ulRecvSize   = (ULONG                         )0;
    ULONG                           ulLeftSize   = (ULONG                         )AnscBdoGetBlockSize(pPayloadBdo);
    ULONG                           ulCopySize   = (ULONG                         )0;

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
                        ANSC_SCTOWO_EVENT_RESOURCES,
                        (ANSC_HANDLE)NULL
                    );

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
        pMyObject->LastRecvAt      = AnscGetTickInSeconds();

        returnStatus =
            pMyObject->Recv
                (
                    (ANSC_HANDLE)pMyObject,
                    pRecvBuffer,
                    ulCopySize
                );
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
        AnscSctoTsaSendTlsMessage
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
AnscSctoTsaSendTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hMessageBdo
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;
    PANSC_BUFFER_DESCRIPTOR         pPayloadBdo  = (PANSC_BUFFER_DESCRIPTOR)hMessageBdo;

    returnStatus =
        pMyObject->Send2
            (
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
        AnscSctoTsaNotifyEvent
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
AnscSctoTsaNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PANSC_SCTO_WORKER_OBJECT        pWorker      = (PANSC_SCTO_WORKER_OBJECT      )pMyObject->hWorker;

    switch ( ulEvent )
    {
        case    TLS_TSA_EVENT_CONNECTION_ESTABLISHED :

                pMyObject->bTlsConnected = TRUE;

                AnscSetEvent(&pMyObject->TlsConnEvent);

                break;

        case    TLS_TSA_EVENT_CONNECTION_CLOSED :

                pMyObject->bTlsConnected = FALSE;

                AnscSetEvent(&pMyObject->TlsConnEvent);

                break;

        case    TLS_TSA_EVENT_CONNECTION_TERMINATED :

                pMyObject->bBroken = TRUE;

                if ( pMyObject->bTlsConnected )
                {
                    pMyObject->bTlsConnected = FALSE;

                    returnStatus =
                        pWorker->Notify
                            (
                                pWorker->hWorkerContext,
                                ANSC_SCTOWO_EVENT_TLS_ERROR,
                                (ANSC_HANDLE)ulError
                            );
                }
                else
                {
                    pMyObject->LastEvent = ANSC_SCTOWO_EVENT_TLS_ERROR;
                    pMyObject->LastError = ulError;

                    AnscSetEvent(&pMyObject->TlsConnEvent);
                }

                break;

        case    TLS_TSA_EVENT_CONNECTION_TIMEOUT :

                pMyObject->bBroken = TRUE;

                if ( pMyObject->bTlsConnected )
                {
                    pMyObject->bTlsConnected = FALSE;

                    returnStatus =
                        pWorker->Notify
                            (
                                pWorker->hWorkerContext,
                                ANSC_SCTOWO_EVENT_TLS_TIMEOUT,
                                (ANSC_HANDLE)ulError
                            );
                }
                else
                {
                    pMyObject->LastEvent = ANSC_SCTOWO_EVENT_TLS_TIMEOUT;
                    pMyObject->LastError = ulError;

                    AnscSetEvent(&pMyObject->TlsConnEvent);
                }

                break;

        default :

                break;
    }

    return  returnStatus;
}
#endif
