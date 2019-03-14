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

    module:	ansc_deto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Daemon Engine Tcp
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/08/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_DETO_INTERNAL_API_
#define  _ANSC_DETO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DETO_STATES.C
***********************************************************/

ANSC_HANDLE
AnscDetoGetDaemonServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoSetDaemonServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServer
    );

ULONG
AnscDetoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDetoGetControlFlags
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoSetControlFlags
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );

ANSC_STATUS
AnscDetoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_DETO_RECV.C
***********************************************************/

ANSC_STATUS
AnscDetoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_DETO_SEND.C
***********************************************************/

ANSC_STATUS
AnscDetoSendTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DETO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscDetoStart
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoStop
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoCancel
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDetoClean
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoDemandToBeCleaned
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoSnapshot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );

ANSC_STATUS
AnscDetoCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_DETO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscDetoGetSocketByAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

ANSC_HANDLE
AnscDetoGetSocketByAddress2
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

ANSC_HANDLE
AnscDetoGetSocketByOsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    );

ANSC_HANDLE
AnscDetoGetSocketByOsocket2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    );

ANSC_STATUS
AnscDetoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDetoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDetoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoExpAllSockets
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDetoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );

ANSC_STATUS
AnscDetoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DETO_PROCESS.C
***********************************************************/

ANSC_STATUS
AnscDetoRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDetoRecv2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDetoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscDetoSend2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


#endif
