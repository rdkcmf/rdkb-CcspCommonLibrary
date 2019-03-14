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

    module:	ansc_deuo_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Daemon Engine Udp
        Object.

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


#ifndef  _ANSC_DEUO_INTERNAL_API_
#define  _ANSC_DEUO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DEUO_STATES.C
***********************************************************/

ANSC_HANDLE
AnscDeuoGetDaemonServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoSetDaemonServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServer
    );

ULONG
AnscDeuoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDeuoGetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoSetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ULONG
AnscDeuoGetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoSetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ULONG
AnscDeuoGetControlFlags
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoSetControlFlags
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulFlags
    );

ANSC_STATUS
AnscDeuoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_DEUO_RECV.C
***********************************************************/

ANSC_STATUS
AnscDeuoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_DEUO_SEND.C
***********************************************************/

ANSC_STATUS
AnscDeuoSendTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DEUO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscDeuoStart
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoStop
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoClean
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_DEUO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscDeuoGetPacket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoAddPacket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

ANSC_STATUS
AnscDeuoDelPacket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

ANSC_STATUS
AnscDeuoDelAllPackets
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscDeuoGetSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

ANSC_STATUS
AnscDeuoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDeuoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDeuoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDeuoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );

ANSC_STATUS
AnscDeuoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        BOOL                        bEnable
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DEUO_PROCESS.C
***********************************************************/

ANSC_STATUS
AnscDeuoRecv
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDeuoSend
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


#endif
