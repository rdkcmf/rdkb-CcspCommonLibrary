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

    module:	ansc_beto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Broker Engine Tcp
        Object.

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


#ifndef  _ANSC_BETO_INTERNAL_API_
#define  _ANSC_BETO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_BETO_STATES.C
***********************************************************/

ANSC_HANDLE
AnscBetoGetBrokerServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBetoSetBrokerServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServer
    );

ULONG
AnscBetoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBetoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscBetoGetControlFlags
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBetoSetControlFlags
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );

ANSC_STATUS
AnscBetoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_BETO_RECV.C
***********************************************************/

ANSC_STATUS
AnscBetoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_BETO_SEND.C
***********************************************************/

ANSC_STATUS
AnscBetoSendTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_BETO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscBetoStart
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBetoStop
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBetoCancel
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscBetoClean
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBetoSnapshot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );

ANSC_STATUS
AnscBetoCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_BETO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscBetoGetSocketByAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

ANSC_HANDLE
AnscBetoGetSocketByOsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    );

ANSC_HANDLE
AnscBetoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port,
        ANSC_HANDLE                 hClientContext
    );

ANSC_STATUS
AnscBetoDelSocketByAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

ANSC_STATUS
AnscBetoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscBetoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBetoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );

ANSC_STATUS
AnscBetoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_BETO_PROCESS.C
***********************************************************/

ANSC_STATUS
AnscBetoRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscBetoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


#endif
