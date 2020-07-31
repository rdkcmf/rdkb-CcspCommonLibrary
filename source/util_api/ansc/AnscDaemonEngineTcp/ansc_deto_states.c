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

    module:	ansc_deto_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Daemon Engine Tcp Object.

        *   AnscDetoGetDaemonServer
        *   AnscDetoSetDaemonServer
        *   AnscDetoGetMaxSocketCount
        *   AnscDetoSetMaxSocketCount
        *   AnscDetoGetControlFlags
        *   AnscDetoSetControlFlags
        *   AnscDetoReset

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


#include "ansc_deto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscDetoGetDaemonServer
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
AnscDetoGetDaemonServer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    return  pMyObject->hDaemonServer;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoSetDaemonServer
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
AnscDetoSetDaemonServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDaemonServer
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    pMyObject->hDaemonServer = hDaemonServer;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDetoGetMaxSocketCount
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
AnscDetoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    return  pMyObject->MaxSocketCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoSetMaxSocketCount
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
AnscDetoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    pMyObject->MaxSocketCount = (ulCount < ANSC_DETO_MAX_SOCKET_COUNT)? ulCount : ANSC_DETO_MAX_SOCKET_COUNT;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscDetoGetControlFlags
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
AnscDetoGetControlFlags
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    return  pMyObject->ControlFlags;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoSetControlFlags
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
AnscDetoSetControlFlags
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;

    pMyObject->ControlFlags = ulFlags;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDetoReset
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
AnscDetoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_ENGINE_TCP_OBJECT  pMyObject    = (PANSC_DAEMON_ENGINE_TCP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_TCP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_TCP_OBJECT)pMyObject->hDaemonServer;
    ansc_fd_set*                    pRecvSet1    = (ansc_fd_set*                  )pMyObject->RecvSocketSet;
    xskt_fd_set*                    pRecvSet2    = (xskt_fd_set*                  )pMyObject->RecvSocketSet;
    ansc_fd_set*                    pSendSet1    = (ansc_fd_set*                  )pMyObject->SendSocketSet;
    xskt_fd_set*                    pSendSet2    = (xskt_fd_set*                  )pMyObject->SendSocketSet;

    if ( TRUE )
    {
        AnscAcquireLock(&pMyObject->RecvSocketSetLock);

        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            XSKT_SOCKET_FD_ZERO(pRecvSet2);
        }
        else
        {
            ANSC_SOCKET_FD_ZERO(pRecvSet1);
        }

        AnscReleaseLock(&pMyObject->RecvSocketSetLock);
    }

    if ( TRUE )
    {
        AnscAcquireLock(&pMyObject->SendSocketSetLock);

        if ( pServer->Mode & ANSC_DSTO_MODE_XSOCKET )
        {
            XSKT_SOCKET_FD_ZERO(pSendSet2);
        }
        else
        {
            ANSC_SOCKET_FD_ZERO(pSendSet1);
        }

        AnscReleaseLock(&pMyObject->SendSocketSetLock);
    }

    pMyObject->DelAllSockets((ANSC_HANDLE)pMyObject);

    pMyObject->bCleaningDemanded = FALSE;
    pMyObject->StartTime         = 0;
    pMyObject->AvgServingTime    = 0;
    pMyObject->TtcCount          = 0;
    pMyObject->MtcCount          = 0;
    pMyObject->TrcCount          = 0;
    pMyObject->MrcCount          = 0;
    pMyObject->TscCount          = 0;
    pMyObject->MscCount          = 0;

    return  ANSC_STATUS_SUCCESS;
}
