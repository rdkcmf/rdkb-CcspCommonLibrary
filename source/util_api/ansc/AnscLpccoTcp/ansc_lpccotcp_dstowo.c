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

    module:	ansc_lpccotcp_dstowo.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced working functions
        of the Ansc Lpcco Tcp Object.

        *   AnscLpccoTcpDwoInit
        *   AnscLpccoTcpDwoUnload
        *   AnscLpccoTcpDwoAccept
        *   AnscLpccoTcpDwoSetOut
        *   AnscLpccoTcpDwoRemove
        *   AnscLpccoTcpDwoQuery
        *   AnscLpccoTcpDwoProcessSync
        *   AnscLpccoTcpDwoProcessAsync
        *   AnscLpccoTcpDwoSendComplete
        *   AnscLpccoTcpDwoNotify

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/25/03    initial revision.

**********************************************************************/


#include "ansc_lpccotcp_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoInit
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoInit
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoUnload
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoUnload
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscLpccoTcpDwoAccept
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PANSC_HANDLE                phClientContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PANSC_HANDLE                phClientContext
                Specifies the client context to be returned.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
AnscLpccoTcpDwoAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PANSC_HANDLE                phClientContext
    )
{
    UNREFERENCED_PARAMETER(phClientContext);
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = NULL;

    pBufferDesp =
        (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
            (
                ANSC_LPC_DEF_MESSAGE_SIZE,
                0,
                0
            );

    if ( pBufferDesp )
    {
        pDaemonSocket->SetBufferContext
            (
                (ANSC_HANDLE)pDaemonSocket,
                AnscBdoGetEndOfBlock(pBufferDesp),
                AnscBdoGetLeftSize  (pBufferDesp),
                (ANSC_HANDLE)pBufferDesp
            );
    }

    return  TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoSetOut
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoSetOut
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoRemove
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoRemove
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    )
{
    PANSC_LPCCO_TCP_OBJECT          pMyObject     = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pConnTimerObj = (PANSC_TIMER_DESCRIPTOR_OBJECT )pMyObject->hConnTimerObj;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pDaemonSocket->GetBufferContext((ANSC_HANDLE)pDaemonSocket);
    PANSC_LPC_PARTY_ADDR            pPartyAddr    = (PANSC_LPC_PARTY_ADDR          )pDaemonSocket->GetClientContext((ANSC_HANDLE)pDaemonSocket);

    pDaemonSocket->SetClientContext((ANSC_HANDLE)pDaemonSocket, (ANSC_HANDLE)NULL         );
    pDaemonSocket->SetBufferContext((ANSC_HANDLE)pDaemonSocket, NULL, 0, (ANSC_HANDLE)NULL);

    if ( pBufferDesp )
    {
        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);
    }

    if ( pPartyAddr )
    {
        pPartyAddr->Timestamp   = AnscGetTickInSeconds();
        pPartyAddr->PartyState &= ~ANSC_LPC_PARTY_STATE_connectedIn;

        /*
         * We shouldn't return until the reference count of 'pPartyAddr' reaches zero. Because if
         * we do, the socket may be deleted while there's still active task is trying to send
         * message to this party. For example: if LPC manager is issuing a LOCO call to all LPC
         * parties while one party is trying disconnect, a race condition is formed.
         */
        /*
        while ( pPartyAddr->ActiveCalls > 0 )
        {
            AnscSleep(20);
        }
        */

        if ( !(pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedIn ) &&
             !(pPartyAddr->PartyState & ANSC_LPC_PARTY_STATE_connectedOut) )
        {
            if ( pMyObject->bActive )
            {
                pConnTimerObj->Stop       ((ANSC_HANDLE)pConnTimerObj);
                pConnTimerObj->SetInterval((ANSC_HANDLE)pConnTimerObj, ANSC_LPCCO_DEF_CONN_TIMEOUT);
                pConnTimerObj->Start      ((ANSC_HANDLE)pConnTimerObj);
            }
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscLpccoTcpDwoQuery
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                PANSC_HANDLE                phQueryContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                PANSC_HANDLE                phQueryContext
                Specifies the recv context associated with buffer.

    return:     process mode.

**********************************************************************/

ULONG
AnscLpccoTcpDwoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    )
{
    UNREFERENCED_PARAMETER(buffer);
    PANSC_LPCCO_TCP_OBJECT          pMyObject     = (PANSC_LPCCO_TCP_OBJECT        )hThisObject;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;
    PANSC_BUFFER_DESCRIPTOR         pBufferDesp   = (PANSC_BUFFER_DESCRIPTOR       )pDaemonSocket->GetBufferContext((ANSC_HANDLE)pDaemonSocket);
    PANSC_BUFFER_DESCRIPTOR         pNewBdo       = (PANSC_BUFFER_DESCRIPTOR       )NULL;
    PIMCP_HEADER                    pImcpHeader   = (PIMCP_HEADER                  )NULL;
    ULONG                           ulMsgSize     = (ULONG                         )0;
    ULONG                           ulCopySize    = (ULONG                         )0;

    if ( !pBufferDesp )
    {
        return  ANSC_DSTOWO_PMODE_FINISH;
    }
    else
    {
        pBufferDesp->BlockSize += ulSize;
    }

    if ( AnscBdoGetUsableSize(pBufferDesp) <= sizeof(IMCP_HEADER) )
    {
        pNewBdo =
            (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                (
                    ANSC_LPC_DEF_MESSAGE_SIZE,
                    0,
                    AnscBdoGetBlockSize(pBufferDesp)
                );

        if ( !pNewBdo )
        {
            return  ANSC_DSTOWO_PMODE_FINISH;
        }
        else
        {
            ulCopySize   = AnscBdoGetBlockSize(pBufferDesp);
                AnscBdoCopyFrom
                    (
                        (ANSC_HANDLE)pBufferDesp,
                        AnscBdoGetBlock(pNewBdo),
                        &ulCopySize,
                        0
                    );

            pDaemonSocket->SetBufferContext
                (
                    (ANSC_HANDLE)pDaemonSocket,
                    AnscBdoGetEndOfBlock(pNewBdo),
                    AnscBdoGetLeftSize  (pNewBdo),
                    (ANSC_HANDLE)pNewBdo
                );
        }

        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

        return  ANSC_DSTOWO_PMODE_COLLECT;
    }
    else if ( AnscBdoGetBlockSize(pBufferDesp) < sizeof(IMCP_HEADER) )
    {
        /*
         * We need to rollback the block size of the buffer descriptor before we return 'Collect'.
         * Next time when this Query() function is called, the 'ulSize' parameter will be set to
         * the total size of un-processed data, not the size of the newly received data.
         */
        pBufferDesp->BlockSize -= ulSize;

        return  ANSC_DSTOWO_PMODE_COLLECT;
    }
    else
    {
        pImcpHeader = (PIMCP_HEADER)AnscBdoGetBlock(pBufferDesp);
        ulMsgSize   = (ULONG       )ImcpGetMsgSize (pImcpHeader);
    }

    if ( ulMsgSize == AnscBdoGetBlockSize(pBufferDesp) )
    {
        pNewBdo =
            (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                (
                    ANSC_LPC_DEF_MESSAGE_SIZE,
                    0,
                    0
                );

        if ( !pNewBdo )
        {
            return  ANSC_DSTOWO_PMODE_FINISH;
        }
        else
        {
            pDaemonSocket->SetBufferContext
                (
                    (ANSC_HANDLE)pDaemonSocket,
                    AnscBdoGetEndOfBlock(pNewBdo),
                    AnscBdoGetLeftSize  (pNewBdo),
                    (ANSC_HANDLE)pNewBdo
                );
        }

            pMyObject->Recv
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pDaemonSocket,
                    (ANSC_HANDLE)pBufferDesp
                );

        return  ANSC_DSTOWO_PMODE_DISCARD;
    }
    else if ( ulMsgSize < AnscBdoGetBlockSize(pBufferDesp) )
    {
        pNewBdo =
            (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                (
                    ulMsgSize,
                    0,
                    ulMsgSize
                );

        if ( !pNewBdo )
        {
            return  ANSC_DSTOWO_PMODE_FINISH;
        }
        else
        {
            ulCopySize   = ulMsgSize;
                AnscBdoCopyFrom
                    (
                        (ANSC_HANDLE)pBufferDesp,
                        AnscBdoGetBlock(pNewBdo),
                        &ulCopySize,
                        0
                    );
        }

            pMyObject->Recv
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pDaemonSocket,
                    (ANSC_HANDLE)pNewBdo
                );

        /*
         * As Lina discovered, we cannot simply reset the 'BlockSize' to zero and make another
         * Query() call. If we do and the returned mode is COLLECT, the socket object would be
         * confused because the data processed in this call is still remembered and its size will
         * be covered in the next Query() call. We need to follow the steps as listed below to
         * ensure the packet data in the previous call is cleaned:
         *
         *      - Set the 'BlockSize' to whatever data left after processing.
         *      - Advance 'Offset' field by the message size.
         *      - Reset the buffer context of the socket object.
         *      - Calling Query() again with 'ulSize' set to zero.
         */
        pBufferDesp->Offset    += ulMsgSize;
        pBufferDesp->BlockSize -= ulMsgSize;

        pDaemonSocket->SetBufferContext
            (
                (ANSC_HANDLE)pDaemonSocket,
                AnscBdoGetEndOfBlock(pBufferDesp),
                AnscBdoGetLeftSize  (pBufferDesp),
                (ANSC_HANDLE)pBufferDesp
            );

        return  pMyObject->DwoQuery
                    (
                        (ANSC_HANDLE)pMyObject,
                        (ANSC_HANDLE)pDaemonSocket,
                        AnscBdoGetEndOfBlock(pBufferDesp),  /*AnscBdoGetBlock(pBufferDesp),*/
                        0,                                  /*ulLeftSize,                  */
                        phQueryContext
                    );
    }
    else if ( ulMsgSize <= AnscBdoGetUsableSize(pBufferDesp) )
    {
        /*
         * We need to rollback the block size of the buffer descriptor before we return 'Collect'.
         * Next time when this Query() function is called, the 'ulSize' parameter will be set to
         * the total size of un-processed data, not the size of the newly received data.
         */
        pBufferDesp->BlockSize -= ulSize;

        return  ANSC_DSTOWO_PMODE_COLLECT;
    }
    else
    {
        pNewBdo =
            (PANSC_BUFFER_DESCRIPTOR)AnscAllocateBdo
                (
                    ulMsgSize,
                    0,
                    AnscBdoGetBlockSize(pBufferDesp)
                );

        if ( !pNewBdo )
        {
            return  ANSC_DSTOWO_PMODE_FINISH;
        }
        else
        {
            ulCopySize   = AnscBdoGetBlockSize(pBufferDesp);
                AnscBdoCopyFrom
                    (
                        (ANSC_HANDLE)pBufferDesp,
                        AnscBdoGetBlock(pNewBdo),
                        &ulCopySize,
                        0
                    );

            pDaemonSocket->SetBufferContext
                (
                    (ANSC_HANDLE)pDaemonSocket,
                    AnscBdoGetEndOfBlock(pNewBdo),
                    AnscBdoGetLeftSize  (pNewBdo),
                    (ANSC_HANDLE)pNewBdo
                );
        }

        AnscFreeBdo((ANSC_HANDLE)pBufferDesp);

        return  ANSC_DSTOWO_PMODE_COLLECT;
    }

    return  ANSC_DSTOWO_PMODE_DISCARD;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoProcessSync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hQueryContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                ANSC_HANDLE                 hQueryContext
                Specifies the recv context associated with buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    UNREFERENCED_PARAMETER(hQueryContext);

    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoProcessAsync
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hQueryContext
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                PVOID                       buffer
                Specifies the data buffer to be processed.

                ULONG                       ulSize
                Specifies the size of data buffer to be processed.

                ANSC_HANDLE                 hQueryContext
                Specifies the recv context associated with buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    UNREFERENCED_PARAMETER(hQueryContext);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoSendComplete
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ANSC_HANDLE                 hReserved,
                ANSC_STATUS                 status
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                ANSC_HANDLE                 hReserved
                Specifies the transparent context passed in send().

                ANSC_STATUS                 status
                Specifies the status of the send operation.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoSendComplete
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ANSC_HANDLE                 hReserved,
        ANSC_STATUS                 status
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(hSocket);
    UNREFERENCED_PARAMETER(hReserved);
    UNREFERENCED_PARAMETER(status);
    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscLpccoTcpDwoNotify
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hSocket,
                ULONG                       ulEvent,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called by the internal Server Object as part
        of the worker function set.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hSocket
                Specifies the socket object associated with the call.

                ULONG                       ulEvent
                Specifies the event to be notified.

                ANSC_HANDLE                 hReserved
                Specifies the transparent context associated with event.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscLpccoTcpDwoNotify
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        ULONG                       ulEvent,
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pDaemonSocket = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hSocket;

    switch ( ulEvent )
    {
        case    ANSC_DSTOWO_EVENT_SOCKET_ERROR :

                pDaemonSocket->ToClean((ANSC_HANDLE)pDaemonSocket, TRUE, 2);

                break;

        case    ANSC_DSTOWO_EVENT_SOCKET_CLOSED :

                pDaemonSocket->Finish((ANSC_HANDLE)pDaemonSocket);

                break;

        case    ANSC_DSTOWO_EVENT_RESOURCES :

                break;

        case    ANSC_DSTOWO_EVENT_TIME_OUT :

                break;

        default :

                break;
    }

    return  ANSC_STATUS_SUCCESS;
}
