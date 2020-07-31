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

    module:	ansc_bkto_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Broker Socket Tcp Object.

        *   AnscBktoGetHostAddress
        *   AnscBktoSetHostAddress
        *   AnscBktoGetHostPort
        *   AnscBktoSetHostPort
        *   AnscBktoGetPeerAddress
        *   AnscBktoSetPeerAddress
        *   AnscBktoGetPeerPort
        *   AnscBktoSetPeerPort
        *   AnscBktoGetBrokerServer
        *   AnscBktoSetBrokerServer
        *   AnscBktoGetBrokerEngine
        *   AnscBktoSetBrokerEngine
        *   AnscBktoGetClientContext
        *   AnscBktoSetClientContext
        *   AnscBktoGetBufferSize
        *   AnscBktoSetBufferSize
        *   AnscBktoGetBufferContext
        *   AnscBktoSetBufferContext
        *   AnscBktoIsToBeCleaned
        *   AnscBktoToClean
        *   AnscBktoReturn
        *   AnscBktoReset

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


#include "ansc_bkto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscBktoGetHostAddress
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

PUCHAR
AnscBktoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->HostAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetHostAddress
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->HostAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscBktoGetHostPort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

USHORT
AnscBktoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->HostPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetHostPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      usPort
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      usPort
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->HostPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscBktoGetPeerAddress
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

PUCHAR
AnscBktoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->PeerAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetPeerAddress
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      address
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      address
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->PeerAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscBktoGetPeerPort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

USHORT
AnscBktoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->PeerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetPeerPort
            (
                ANSC_HANDLE                 hThisObject,
                USHORT                      usPort
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                USHORT                      usPort
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->PeerPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBktoGetBrokerServer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
AnscBktoGetBrokerServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hBrokerServer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetBrokerServer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBrokerServer
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBrokerServer
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetBrokerServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBrokerServer
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->hBrokerServer = hBrokerServer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBktoGetBrokerEngine
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
AnscBktoGetBrokerEngine
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hBrokerEngine;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetBrokerEngine
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBrokerEngine
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBrokerEngine
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetBrokerEngine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBrokerEngine
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->hBrokerEngine = hBrokerEngine;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBktoGetClientContext
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
AnscBktoGetClientContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hClientContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetClientContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hClientContext
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hClientContext
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetClientContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientContext
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->hClientContext = hClientContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscBktoGetBufferSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
AnscBktoGetBufferSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->RecvBufferSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetBufferSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSize
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSize
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetBufferSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    if ( pMyObject->RecvBuffer )
    {
        AnscFreeMemory(pMyObject->RecvBuffer);

        pMyObject->RecvBuffer = NULL;
    }

    pMyObject->RecvBufferSize = ulSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscBktoGetBufferContext
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
AnscBktoGetBufferContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hRecvBufferTag;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoSetBufferContext
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hBufferTag
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the object state to be configured.

                ULONG                       ulSize
                Specifies the object state to be configured.

                ANSC_HANDLE                 hBufferTag
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoSetBufferContext
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;

    if ( !(pServer->Mode & ANSC_BSTO_MODE_FOREIGN_BUFFER) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        pMyObject->hRecvBufferTag = hBufferTag;
        pMyObject->RecvBuffer     = buffer;
        pMyObject->RecvBufferSize = ulSize;
        pMyObject->RecvPacketSize = 0;
        pMyObject->RecvOffset     = 0;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscBktoIsToBeCleaned
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

BOOL
AnscBktoIsToBeCleaned
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->bToBeCleaned;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoToClean
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnable,
                ULONG                       ulSeconds
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnable
                Specifies the object state to be configured.

                ULONG                       ulSeconds
                Specifies the number of seconds to be skipped.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoToClean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable,
        ULONG                       ulSeconds
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;
    PANSC_BROKER_ENGINE_TCP_OBJECT  pEngine      = (PANSC_BROKER_ENGINE_TCP_OBJECT)pMyObject->hBrokerEngine;
    ULONG                           ulCurTime    = AnscGetTickInSeconds();

    if ( !pMyObject->bClosed )
    {
        pMyObject->bToBeCleaned      = bEnable;
        pEngine  ->bCleaningDemanded = bEnable? TRUE : pEngine->bCleaningDemanded;

        pMyObject->LastRecvAt        = ulCurTime + ulSeconds - pServer->SocketTimeOut;
        pMyObject->LastSendAt        = pMyObject->LastRecvAt;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoReturn
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to return this socket object back to
        the global socket pool.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;

    returnStatus =
        pServer->ReleaseSocket
            (
                (ANSC_HANDLE)pServer,
                (ANSC_HANDLE)pMyObject
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscBktoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscBktoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_BROKER_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_BROKER_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_BROKER_SERVER_TCP_OBJECT  pServer      = (PANSC_BROKER_SERVER_TCP_OBJECT)pMyObject->hBrokerServer;

    pMyObject->Socket         = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->HashIndex      = 0;

    pMyObject->hBrokerEngine  = (ANSC_HANDLE)NULL;
    pMyObject->hClientContext = (ANSC_HANDLE)NULL;
    pMyObject->RecvBytesCount = 0;
    pMyObject->SendBytesCount = 0;
    pMyObject->LastRecvAt     = 0;
    pMyObject->LastSendAt     = 0;
    pMyObject->StartTime      = 0;
    pMyObject->bClosed        = TRUE;
    pMyObject->bBroken        = FALSE;
    pMyObject->bToBeCleaned   = FALSE;

    pMyObject->hRecvBufferTag = (ANSC_HANDLE)NULL;
    pMyObject->RecvPacketSize = 0;
    pMyObject->RecvOffset     = 0;

    if ( pServer->Mode & ANSC_BSTO_MODE_FOREIGN_BUFFER )
    {
        pMyObject->RecvBuffer     = NULL;
        pMyObject->RecvBufferSize = 0;
    }
    else if ( pServer->Mode & ANSC_BSTO_MODE_DYNAMIC_BUFFER )
    {
        if ( pMyObject->RecvBuffer )
        {
            AnscFreeMemory(pMyObject->RecvBuffer);

            pMyObject->RecvBuffer = NULL;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
