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

    module:	ansc_dkto_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Daemon Socket Tcp Object.

        *   AnscDktoGetPeerAddress
        *   AnscDktoSetPeerAddress
        *   AnscDktoGetPeerPort
        *   AnscDktoSetPeerPort
        *   AnscDktoGetDaemonServer
        *   AnscDktoSetDaemonServer
        *   AnscDktoGetDaemonEngine
        *   AnscDktoSetDaemonEngine
        *   AnscDktoGetClientContext
        *   AnscDktoSetClientContext
        *   AnscDktoGetBufferSize
        *   AnscDktoSetBufferSize
        *   AnscDktoGetBufferContext
        *   AnscDktoSetBufferContext
        *   AnscDktoGetTlsScsIf
        *   AnscDktoSetTlsScsIf
        *   AnscDktoIsToBeCleaned
        *   AnscDktoToClean
        *   AnscDktoReturn
        *   AnscDktoReset

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

        PUCHAR
        AnscDktoGetPeerAddress
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
AnscDktoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->PeerAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetPeerAddress
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
AnscDktoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->PeerAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscDktoGetPeerPort
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
AnscDktoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->PeerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetPeerPort
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
AnscDktoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->PeerPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDktoGetDaemonServer
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
AnscDktoGetDaemonServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hDaemonServer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetDaemonServer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDaemonServer
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDaemonServer
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoSetDaemonServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDaemonServer
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->hDaemonServer = hDaemonServer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDktoGetDaemonEngine
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
AnscDktoGetDaemonEngine
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hDaemonEngine;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetDaemonEngine
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDaemonEngine
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDaemonEngine
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoSetDaemonEngine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDaemonEngine
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->hDaemonEngine = hDaemonEngine;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDktoGetClientContext
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
AnscDktoGetClientContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hClientContext;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetClientContext
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
AnscDktoSetClientContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClientContext
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->hClientContext = hClientContext;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDktoGetBufferSize
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
AnscDktoGetBufferSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->RecvBufferSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetBufferSize
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
AnscDktoSetBufferSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

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
        AnscDktoGetBufferContext
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
AnscDktoGetBufferContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hRecvBufferTag;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetBufferContext
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
AnscDktoSetBufferContext
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;

    if ( !(pServer->Mode & ANSC_DSTO_MODE_FOREIGN_BUFFER) )
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

        ANSC_HANDLE
        AnscDktoGetTlsScsIf
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
AnscDktoGetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->hTlsScsIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoSetTlsScsIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDktoSetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    pMyObject->hTlsScsIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscDktoIsToBeCleaned
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
AnscDktoIsToBeCleaned
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;

    return  pMyObject->bToBeCleaned;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoToClean
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
AnscDktoToClean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable,
        ULONG                       ulSeconds
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_TCP_OBJECT)pMyObject->hDaemonEngine;
    ULONG                           ulCurTime    = AnscGetTickInSecondsAbs();

    if ( !pMyObject->bClosed )
    {
        pMyObject->bToBeCleaned      = bEnable;
        pEngine  ->bCleaningDemanded = bEnable? TRUE : pEngine->bCleaningDemanded;

        pMyObject->LastRecvAt        = ulCurTime + ulSeconds - pEngine->SocketTimeOut;
        pMyObject->LastSendAt        = pMyObject->LastRecvAt;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDktoReturn
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
AnscDktoReturn
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;

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
        AnscDktoReset
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
AnscDktoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;

    pMyObject->Socket         = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->HashIndex      = 0;

    pMyObject->hDaemonEngine  = (ANSC_HANDLE)NULL;
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

    pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;
    pMyObject->bTlsEnabled    = FALSE;
    pMyObject->bTlsConnected  = FALSE;

    if ( pServer->Mode & ANSC_DSTO_MODE_FOREIGN_BUFFER )
    {
        pMyObject->RecvBuffer     = NULL;
        pMyObject->RecvBufferSize = 0;
    }
    else if ( pServer->Mode & ANSC_DSTO_MODE_DYNAMIC_BUFFER )
    {
        if ( pMyObject->RecvBuffer )
        {
            AnscFreeMemory(pMyObject->RecvBuffer);

            pMyObject->RecvBuffer = NULL;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
