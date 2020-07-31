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

    module:	ansc_dsuo_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Daemon Server Udp Object.

        *   AnscDsuoGetHostAddress
        *   AnscDsuoSetHostAddress
        *   AnscDsuoGetHostPort
        *   AnscDsuoSetHostPort
        *   AnscDsuoGetWorker
        *   AnscDsuoSetWorker
        *   AnscDsuoGetMaxMessageSize
        *   AnscDsuoSetMaxMessageSize
        *   AnscDsuoGetEngineCount
        *   AnscDsuoSetEngineCount
        *   AnscDsuoGetMinSocketCount
        *   AnscDsuoSetMinSocketCount
        *   AnscDsuoGetMaxSocketCount
        *   AnscDsuoSetMaxSocketCount
        *   AnscDsuoGetMinPacketCount
        *   AnscDsuoSetMinPacketCount
        *   AnscDsuoGetMaxPacketCount
        *   AnscDsuoSetMaxPacketCount
        *   AnscDsuoGetSocketTimeOut
        *   AnscDsuoSetSocketTimeOut
        *   AnscDsuoGetPacketTimeOut
        *   AnscDsuoSetPacketTimeOut
        *   AnscDsuoGetMode
        *   AnscDsuoSetMode
        *   AnscDsuoReset

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


#include "ansc_dsuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscDsuoGetHostAddress
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
AnscDsuoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->HostAddress.Dot;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetHostAddress
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
AnscDsuoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->HostAddress.Value = *(PULONG)address;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        USHORT
        AnscDsuoGetHostPort
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
AnscDsuoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->HostPort;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetHostPort
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
AnscDsuoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->HostPort = usPort;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDsuoGetWorker
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
AnscDsuoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->hWorker;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetWorker
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
AnscDsuoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

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
        AnscDsuoGetMaxMessageSize
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
AnscDsuoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->MaxMessageSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetMaxMessageSize
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
AnscDsuoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->MaxMessageSize = ulSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetEngineCount
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
AnscDsuoGetEngineCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->EngineCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetEngineCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoSetEngineCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->EngineCount = (ulCount < ANSC_DSUO_MAX_ENGINE_COUNT)? ulCount : ANSC_DSUO_MAX_ENGINE_COUNT;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetMinSocketCount
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
AnscDsuoGetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->MinSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetMinSocketCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoSetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->MinSocketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetMaxSocketCount
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
AnscDsuoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->MaxSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetMaxSocketCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->MaxSocketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetMinPacketCount
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
AnscDsuoGetMinPacketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->MinPacketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetMinPacketCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoSetMinPacketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->MinPacketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetMaxPacketCount
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
AnscDsuoGetMaxPacketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->MaxPacketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetMaxPacketCount
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCount
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCount
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoSetMaxPacketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->MaxPacketCount = ulCount;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetSocketTimeOut
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
AnscDsuoGetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->SocketTimeOut;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetSocketTimeOut
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSeconds
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoSetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->SocketTimeOut = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetPacketTimeOut
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
AnscDsuoGetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->PacketTimeOut;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetPacketTimeOut
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSeconds
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSeconds
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDsuoSetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->PacketTimeOut = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDsuoGetMode
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
AnscDsuoGetMode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    return  pMyObject->Mode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoSetMode
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
AnscDsuoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    )
{
    PANSC_DAEMON_SERVER_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SERVER_UDP_OBJECT)hThisObject;

    pMyObject->Mode = ulMode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDsuoReset
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
AnscDsuoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
