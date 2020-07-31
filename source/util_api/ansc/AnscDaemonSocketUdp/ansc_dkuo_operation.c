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

    module:	ansc_dkuo_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Daemon Socket Udp Object.

        *   AnscDkuoFinish
        *   AnscDkuoOpen
        *   AnscDkuoClose
        *   AnscDkuoEnableRecv
        *   AnscDkuoEnableSend

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


#include "ansc_dkuo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoFinish
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the external Worker Object to close
        the socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDkuoFinish
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_UDP_OBJECT)pMyObject->hDaemonEngine;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( pEngine )
    {
        returnStatus =
            pEngine->DelSocket
                (
                    (ANSC_HANDLE)pEngine,
                    (ANSC_HANDLE)pMyObject
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoOpen
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Engine Object to initialize
        socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDkuoOpen
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;

    if ( !pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bClosed = FALSE;
    }

    pMyObject->RecvBytesCount = 0;
    pMyObject->SendBytesCount = 0;
    pMyObject->LastRecvAt     = AnscGetTickInSeconds();
    pMyObject->LastSendAt     = AnscGetTickInSeconds();

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoClose
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called by the Engine Object to unload
        socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDkuoClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_UDP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSUO_WORKER_OBJECT        pWorker      = (PANSC_DSUO_WORKER_OBJECT      )pServer->hWorker;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bClosed = TRUE;
    }

    if ( pMyObject->hClientContext )
    {
        returnStatus =
            pWorker->Remove
                (
                    pWorker->hWorkerContext,
                    (ANSC_HANDLE)pMyObject
                );
    }
    else
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    if ( pServer->Mode & ANSC_DSUO_MODE_XSOCKET && pMyObject->Socket != XSKT_SOCKET_INVALID_SOCKET )
    {
        _xskt_closesocket(pMyObject->Socket);
    }
    else if ( !(pServer->Mode & ANSC_DSUO_MODE_XSOCKET) && pMyObject->Socket != ANSC_SOCKET_INVALID_SOCKET )
    {
        _ansc_closesocket(pMyObject->Socket);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoEnableRecv
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnable
            );

    description:

        This function is called by the Engine Object to configure
        the socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnable
                Specifies how to configure the socket object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDkuoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->OpLock);
    pMyObject->bRecvEnabled = bEnable;
    AnscReleaseLock(&pMyObject->OpLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoEnableSend
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnable
            );

    description:

        This function is called by the Engine Object to configure
        the socket object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnable
                Specifies how to configure the socket object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDkuoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;

    AnscAcquireLock(&pMyObject->OpLock);
    pMyObject->bSendEnabled = bEnable;
    AnscReleaseLock(&pMyObject->OpLock);

    return  returnStatus;
}
