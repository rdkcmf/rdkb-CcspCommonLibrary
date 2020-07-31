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

    module:	ansc_deuo_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Daemon Engine Udp Object.

        *   AnscDeuoGetDaemonServer
        *   AnscDeuoSetDaemonServer
        *   AnscDeuoGetMaxSocketCount
        *   AnscDeuoSetMaxSocketCount
        *   AnscDeuoGetSocketTimeOut
        *   AnscDeuoSetSocketTimeOut
        *   AnscDeuoGetPacketTimeOut
        *   AnscDeuoSetPacketTimeOut
        *   AnscDeuoGetControlFlags
        *   AnscDeuoSetControlFlags
        *   AnscDeuoReset

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


#include "ansc_deuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDeuoGetDaemonServer
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
AnscDeuoGetDaemonServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    return  pMyObject->hDaemonServer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoSetDaemonServer
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
AnscDeuoSetDaemonServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDaemonServer
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    pMyObject->hDaemonServer = hDaemonServer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDeuoGetMaxSocketCount
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
AnscDeuoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    return  pMyObject->MaxSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoSetMaxSocketCount
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
AnscDeuoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    pMyObject->MaxSocketCount = (ulCount < ANSC_DEUO_MAX_SOCKET_COUNT)? ulCount : ANSC_DEUO_MAX_SOCKET_COUNT;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDeuoGetSocketTimeOut
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
AnscDeuoGetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    return  pMyObject->SocketTimeOut;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoSetSocketTimeOut
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
AnscDeuoSetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    pMyObject->SocketTimeOut = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDeuoGetPacketTimeOut
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
AnscDeuoGetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    return  pMyObject->PacketTimeOut;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoSetPacketTimeOut
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
AnscDeuoSetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    pMyObject->PacketTimeOut = ulSeconds;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDeuoGetControlFlags
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
AnscDeuoGetControlFlags
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    return  pMyObject->ControlFlags;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoSetControlFlags
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulFlags
            );

    description:

        This function is called to configure the object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulFlags
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDeuoSetControlFlags
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    pMyObject->ControlFlags = ulFlags;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDeuoReset
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
AnscDeuoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_UDP_OBJECT)hThisObject;

    pMyObject->DelAllPackets((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllSockets((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
