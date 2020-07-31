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

    module:	ansc_dkuo_process.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the Ansc Daemon Socket Udp Object.

        *   AnscDkuoRecv
        *   AnscDkuoSend

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
        AnscDkuoRecv
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the pointer to packet data buffer.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDkuoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_SERVER_UDP_OBJECT  pServer      = (PANSC_DAEMON_SERVER_UDP_OBJECT)pMyObject->hDaemonServer;
    PANSC_DSUO_PACKET_OBJECT        pPacket      = (PANSC_DSUO_PACKET_OBJECT      )pMyObject->hPacket;
    ULONG                           ulPmode      = ANSC_DSUOWO_PMODE_DISCARD;
    BOOL                            bFreePacket  = TRUE;

    if ( pMyObject->bClosed )
    {
        pMyObject->SetPacket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)NULL);

        if ( pPacket )
        {
            returnStatus =
                pServer->ReleasePacket
                    (
                        (ANSC_HANDLE)pServer,
                        (ANSC_HANDLE)pPacket
                    );
        }

        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( !pPacket )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    ulPmode =
        pServer->Query
            (
                (ANSC_HANDLE)pServer,
                (ANSC_HANDLE)pMyObject,
                pPacket->RecvBuffer,
                pPacket->RecvPacketSize
            );

    switch ( ulPmode )
    {
        case    ANSC_DSUOWO_PMODE_DISCARD :

                pMyObject->SetPacket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)NULL);

                bFreePacket  = TRUE;
                returnStatus = ANSC_STATUS_SUCCESS;

                break;

        case    ANSC_DSUOWO_PMODE_PROCESS_SYNC :

                pMyObject->SetPacket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)NULL);

                bFreePacket  = TRUE;
                returnStatus =
                    pServer->ProcessSync
                        (
                            (ANSC_HANDLE)pServer,
                            (ANSC_HANDLE)pMyObject,
                            pPacket->RecvBuffer,
                            pPacket->RecvPacketSize
                        );

                break;

        case    ANSC_DSUOWO_PMODE_PROCESS_ASYNC :

                bFreePacket  = FALSE;
                returnStatus =
                    pServer->ProcessAsync
                        (
                            (ANSC_HANDLE)pServer,
                            (ANSC_HANDLE)pMyObject,
                            pPacket->RecvBuffer,
                            pPacket->RecvPacketSize
                        );

                break;

        case    ANSC_DSUOWO_PMODE_FINISH :

                pMyObject->SetPacket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)NULL);

                bFreePacket  = TRUE;
                returnStatus = pMyObject->Finish((ANSC_HANDLE)pMyObject);

                break;

        default :

                pMyObject->SetPacket((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)NULL);

                bFreePacket  = TRUE;
                returnStatus = ANSC_STATUS_SUCCESS;

                break;
    }

    if ( bFreePacket )
    {
        returnStatus =
            pServer->ReleasePacket
                (
                    (ANSC_HANDLE)pServer,
                    (ANSC_HANDLE)pPacket
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscDkuoSend
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
            );

    description:

        This function is called to process an outgoing packet.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the pointer to packet data buffer.

                ULONG                       ulSize
                Specifies the size of the packet data buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscDkuoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_DAEMON_SOCKET_UDP_OBJECT  pMyObject    = (PANSC_DAEMON_SOCKET_UDP_OBJECT)hThisObject;
    PANSC_DAEMON_ENGINE_UDP_OBJECT  pEngine      = (PANSC_DAEMON_ENGINE_UDP_OBJECT)pMyObject->hDaemonEngine;

    if ( pMyObject->bClosed )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    returnStatus =
        pEngine->Send
            (
                (ANSC_HANDLE)pEngine,
                (ANSC_HANDLE)pMyObject,
                buffer,
                ulSize,
                hReserved
            );

    return  returnStatus;
}
