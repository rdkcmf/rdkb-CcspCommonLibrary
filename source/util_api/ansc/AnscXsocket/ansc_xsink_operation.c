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

    module:	ansc_xsink_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xsink Object.

        *   AnscXsinkGetRecvBuffer
        *   AnscXsinkAccept
        *   AnscXsinkRecv
        *   AnscXsinkClose
        *   AnscXsinkAbort

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/02    initial revision.

**********************************************************************/


#include "ansc_xsocket_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        PVOID
        AnscXsinkGetRecvBuffer
            (
                ANSC_HANDLE                 hThisObject,
                PANSC_HANDLE                phRecvHandle,
                PULONG                      pulSize
            );

    description:

        This function is called by the receive task to retrieve buffer
        from the socket owner to hold received data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PANSC_HANDLE                phRecvHandle
                Specifies a context which is associated with the
                returned buffer.

                PULONG                      pulSize
                This parameter returns the buffer size.

    return:     buffer pointer.

**********************************************************************/

PVOID
AnscXsinkGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_HANDLE                phRecvHandle,
        PULONG                      pulSize
    )
{
    PANSC_XSINK_OBJECT              pMyObject      = (PANSC_XSINK_OBJECT  )hThisObject;
    PANSC_XSOCKET_OBJECT            pXsocketObject = (PANSC_XSOCKET_OBJECT)pMyObject->hXsocketObject;
    PANSC_XSINK_OBJECT              pExternalXsink = (PANSC_XSINK_OBJECT  )pXsocketObject->hXsinkObject;

    if ( pExternalXsink )
    {
        return  pExternalXsink->GetRecvBuffer
                    (
                        (ANSC_HANDLE)pExternalXsink,
                        phRecvHandle,
                        pulSize
                    );
    }
    else
    {
        *phRecvHandle = (ANSC_HANDLE)NULL;
        *pulSize      = 0;
    }

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkAccept
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hNewXsocket
            );

    description:

        This function notifies the socket owner when network data
        arrives at the socket or socket status has changed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hNewXsocket
                Specifies the socket object we have just created.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkAccept
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hNewXsocket
    )
{
    PANSC_XSINK_OBJECT              pMyObject      = (PANSC_XSINK_OBJECT  )hThisObject;
    PANSC_XSOCKET_OBJECT            pXsocketObject = (PANSC_XSOCKET_OBJECT)pMyObject->hXsocketObject;
    PANSC_XSINK_OBJECT              pExternalXsink = (PANSC_XSINK_OBJECT  )pXsocketObject->hXsinkObject;

    if ( pExternalXsink )
    {
        return  pExternalXsink->Accept
                    (
                        (ANSC_HANDLE)pExternalXsink,
                        hNewXsocket
                    );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkRecv
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRecvHandle,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function notifies the socket owner when network data
        arrives at the socket or socket status has changed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRecvHandle
                Specifies the context returned by get_recv_buffer().

                PVOID                       buffer
                Specifies the buffer holding the received data.

                ULONG                       ulSize
                Specifies the size of the data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecvHandle,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    PANSC_XSINK_OBJECT              pMyObject      = (PANSC_XSINK_OBJECT  )hThisObject;
    PANSC_XSOCKET_OBJECT            pXsocketObject = (PANSC_XSOCKET_OBJECT)pMyObject->hXsocketObject;
    PANSC_XSINK_OBJECT              pExternalXsink = (PANSC_XSINK_OBJECT  )pXsocketObject->hXsinkObject;

    if ( pExternalXsink )
    {
        return  pExternalXsink->Recv
                    (
                        (ANSC_HANDLE)pExternalXsink,
                        hRecvHandle,
                        buffer,
                        ulSize
                    );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkClose
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bByPeer
            );

    description:

        This function notifies the socket owner when network data
        arrives at the socket or socket status has changed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bByPeer
                Specifies whether the host or the peer closed the
                connection.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkClose
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bByPeer
    )
{
    PANSC_XSINK_OBJECT              pMyObject      = (PANSC_XSINK_OBJECT  )hThisObject;
    PANSC_XSOCKET_OBJECT            pXsocketObject = (PANSC_XSOCKET_OBJECT)pMyObject->hXsocketObject;
    PANSC_XSINK_OBJECT              pExternalXsink = (PANSC_XSINK_OBJECT  )pXsocketObject->hXsinkObject;

    if ( pExternalXsink )
    {
        return  pExternalXsink->Close
                    (
                        (ANSC_HANDLE)pExternalXsink,
                        bByPeer
                    );
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXsinkAbort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function notifies the socket owner when critical network
        failure is detected.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXsinkAbort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XSINK_OBJECT              pMyObject      = (PANSC_XSINK_OBJECT  )hThisObject;
    PANSC_XSOCKET_OBJECT            pXsocketObject = (PANSC_XSOCKET_OBJECT)pMyObject->hXsocketObject;
    PANSC_XSINK_OBJECT              pExternalXsink = (PANSC_XSINK_OBJECT  )pXsocketObject->hXsinkObject;

    if ( pExternalXsink )
    {
        return  pExternalXsink->Abort((ANSC_HANDLE)pExternalXsink);
    }

    return  ANSC_STATUS_SUCCESS;
}
