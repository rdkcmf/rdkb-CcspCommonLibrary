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

    module:	ansc_scto_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Simple Client Tcp Object.

        *   AnscSctoGetSocketBindToDevice
        *   AnscSctoSetSocketBindToDevice
        *   AnscSctoGetSocketDeviceName
        *   AnscSctoSetSocketDeviceName
        *   AnscSctoGetHostAddress
        *   AnscSctoSetHostAddress
        *   AnscSctoGetHostPort
        *   AnscSctoSetHostPort
        *   AnscSctoGetPeerName
        *   AnscSctoSetPeerName
        *   AnscSctoGetPeerAddress
        *   AnscSctoSetPeerAddress
        *   AnscSctoGetPeerPort
        *   AnscSctoSetPeerPort
        *   AnscSctoGetWorker
        *   AnscSctoSetWorker
        *   AnscSctoGetMaxMessageSize
        *   AnscSctoSetMaxMessageSize
        *   AnscSctoGetBufferContext
        *   AnscSctoSetBufferContext
        *   AnscSctoGetMode
        *   AnscSctoSetMode
        *   AnscSctoGetTlsScsIf
        *   AnscSctoSetTlsScsIf
        *   AnscSctoReset

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

BOOL
AnscSctoGetSocketBindToDevice
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->bSocketBindToDevice;
}

ANSC_STATUS
AnscSctoSetSocketBindToDevice
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bBind
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->bSocketBindToDevice = bBind;

    return  ANSC_STATUS_SUCCESS;
}

PCHAR
AnscSctoGetSocketDeviceName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->SocketDeviceName;
}

ANSC_STATUS
AnscSctoSetSocketDeviceName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                      pDeviceName
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    AnscCopyString(pMyObject->SocketDeviceName, pDeviceName);

    return  ANSC_STATUS_SUCCESS;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscSctoGetHostAddress
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
AnscSctoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->HostAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetHostAddress
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
AnscSctoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->HostAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscSctoGetHostPort
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
AnscSctoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->HostPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetHostPort
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
AnscSctoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->HostPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************
 *
 *     caller:     owner of this object
 *
 *     prototype:
 *
 *     PUCHAR
 *     AnscSctoGetPeerName
 *         (
 *             ANSC_HANDLE                 hThisObject
 *         );
 *
 *     description:
 *
 *     This function is called to retrieve the object state.
 *
 *     argument:   ANSC_HANDLE                 hThisObject
 *                 This handle is actually the pointer of this object
 *                 itself.
 *
 *     return:     object state.
 *
 ***********************************************************************/

PUCHAR
AnscSctoGetPeerName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  (PUCHAR)pMyObject->PeerName;
}


/**********************************************************************
 *
 *     caller:     owner of this object
 *
 *     prototype:
 *
 *     ANSC_STATUS
 *     AnscSctoSetPeerName
 *         (
 *             ANSC_HANDLE                 hThisObject,
 *             PUCHAR                      peername
 *         );
 *
 *     description:
 *
 *     This function is called to configure the object state.
 *
 *     argument:   ANSC_HANDLE                 hThisObject
 *                 This handle is actually the pointer of this object
 *                 itself.
 *
 *                 PUCHAR                      address
 *                 Specifies the object state to be configured.
 *
 *     return:     status of operation.
 *
 *                                                                                                                                         **********************************************************************/

ANSC_STATUS
AnscSctoSetPeerName
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      peername
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    AnscCopyString(pMyObject->PeerName,(char*) peername);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscSctoGetPeerAddress
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
AnscSctoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->PeerAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetPeerAddress
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
AnscSctoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->PeerAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscSctoGetPeerPort
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
AnscSctoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->PeerPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetPeerPort
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
AnscSctoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->PeerPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************
 *
 *     caller:     owner of this object
 *
 *     prototype:
 *
 *     ANSC_STATUS
 *     AnscSctoGetHostNames
 *         (
 *             ANSC_HANDLE                 hThisObject,
 *	       ANSC_HANDLE		   hHostNames
 *         );
 *
 *     description:
 *
 *     This function is called to retrieve the object state.
 *
 *     argument:   ANSC_HANDLE                 hThisObject
 *                 This handle is actually the pointer of this object
 *                 itself.
 *
 *     return:     object state.
 *
 ***********************************************************************/

ANSC_STATUS
AnscSctoGetHostNames
    (
        ANSC_HANDLE                 hThisObject,
	ANSC_HANDLE		    hHostNames
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PHTTP_SCTO_HOST_NAMES	    pHostNames	 = (PHTTP_SCTO_HOST_NAMES)&pMyObject->hostNames;

    *(PHTTP_SCTO_HOST_NAMES)hHostNames = *pHostNames;

    return  returnStatus;
}


/**********************************************************************
 *
 *     caller:     owner of this object
 *
 *     prototype:
 *
 *     ANSC_STATUS
 *     AnscSctoSetHostNames
 *         (
 *             ANSC_HANDLE                 hThisObject,
 *             ANSC_HANDLE                 hHostNames
 *         );
 *
 *     description:
 *
 *     This function is called to configure the object state.
 *
 *     argument:   ANSC_HANDLE                 hThisObject
 *                 This handle is actually the pointer of this object
 *                 itself.
 *
 *                 ANSC_HANDLE			hHostNames
 *                 Specifies the object state to be configured.
 *
 *     return:     status of operation.
 *
 *                                                                                                                                       **********************************************************************/

ANSC_STATUS
AnscSctoSetHostNames
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE		    hHostNames
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;
    PHTTP_SCTO_HOST_NAMES           pHostNames   = (PHTTP_SCTO_HOST_NAMES)&pMyObject->hostNames;

    *pHostNames = *(PHTTP_SCTO_HOST_NAMES)hHostNames;

    return  returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscSctoGetWorker
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
AnscSctoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->hWorker;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetWorker
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hWorker,
                ULONG                       ulSize
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hWorker
                Specifies the object state to be configured.

                ULONG                       ulSize
                Specifies the size of the worker object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    if ( pMyObject->hWorker )
    {
        AnscFreeMemory(pMyObject->hWorker);

        pMyObject->hWorker = (ANSC_HANDLE)NULL;
    }

    if ( ulSize > 0 )
    {
        pMyObject->hWorker = (ANSC_HANDLE)AnscAllocateMemory(ulSize);

        if ( pMyObject->hWorker )
        {
            AnscCopyMemory
                (
                    (PVOID)pMyObject->hWorker,
                    hWorker,
                    ulSize
                );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscSctoGetMaxMessageSize
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
AnscSctoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->MaxMessageSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetMaxMessageSize
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
AnscSctoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    pMyObject->MaxMessageSize = ulSize;
    pMyObject->RecvBufferSize = ulSize;

    if ( pMyObject->RecvBuffer && !(pMyObject->Mode & ANSC_SCTO_MODE_FOREIGN_BUFFER) )
    {
        AnscFreeMemory(pMyObject->RecvBuffer);

        pMyObject->RecvBuffer = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscSctoGetBufferContext
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
AnscSctoGetBufferContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->hRecvBufferTag;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetBufferContext
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
AnscSctoSetBufferContext
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    if ( !(pMyObject->Mode & ANSC_SCTO_MODE_FOREIGN_BUFFER) )
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

        ULONG
        AnscSctoGetMode
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
AnscSctoGetMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->Mode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetMode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulMode
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulMode
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->Mode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscSctoGetTlsScsIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function retrieves the state of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     state of object.

**********************************************************************/

ANSC_HANDLE
AnscSctoGetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    return  pMyObject->hTlsScsIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoSetTlsScsIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function configures the state of the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscSctoSetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->hTlsScsIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscSctoReset
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
AnscSctoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_SIMPLE_CLIENT_TCP_OBJECT  pMyObject    = (PANSC_SIMPLE_CLIENT_TCP_OBJECT)hThisObject;

    pMyObject->Socket         = ANSC_SOCKET_INVALID_SOCKET;
    pMyObject->bActive        = FALSE;
    pMyObject->bBroken        = FALSE;

    pMyObject->hClientContext = (ANSC_HANDLE)NULL;
    pMyObject->RecvBytesCount = 0;
    pMyObject->SendBytesCount = 0;
    pMyObject->LastRecvAt     = 0;
    pMyObject->LastSendAt     = 0;

    pMyObject->hRecvBufferTag = (ANSC_HANDLE)NULL;
    pMyObject->RecvPacketSize = 0;
    pMyObject->RecvOffset     = 0;

    pMyObject->hTlsConnection = (ANSC_HANDLE)NULL;
    pMyObject->bTlsEnabled    = FALSE;
    pMyObject->bTlsConnected  = FALSE;

    if ( pMyObject->Mode & ANSC_SCTO_MODE_FOREIGN_BUFFER )
    {
        pMyObject->RecvBuffer     = NULL;
        pMyObject->RecvBufferSize = 0;
    }
    else if ( pMyObject->RecvBuffer )
    {
        AnscFreeMemory(pMyObject->RecvBuffer);

        pMyObject->RecvBuffer = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
