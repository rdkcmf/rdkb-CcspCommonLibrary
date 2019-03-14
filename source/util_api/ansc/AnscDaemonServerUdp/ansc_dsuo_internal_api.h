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

    module:	ansc_dsuo_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Daemon Server Udp
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


#ifndef  _ANSC_DSUO_INTERNAL_API_
#define  _ANSC_DSUO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSUO_STATES.C
***********************************************************/

PUCHAR
AnscDsuoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscDsuoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_HANDLE
AnscDsuoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

ULONG
AnscDsuoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ULONG
AnscDsuoGetEngineCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetEngineCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDsuoGetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDsuoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDsuoGetMinPacketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetMinPacketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDsuoGetMaxPacketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetMaxPacketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDsuoGetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ULONG
AnscDsuoGetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetPacketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ULONG
AnscDsuoGetMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
AnscDsuoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSUO_ACCEPT.C
***********************************************************/

ANSC_STATUS
AnscDsuoAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSUO_WORKER.C
***********************************************************/

ANSC_STATUS
AnscDsuoWorkerTask
    (
        ANSC_HANDLE                 hSocket
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DSUO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscDsuoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoStartEngines
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoStopEngines
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_DSUO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscDsuoAcquireSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoReleaseSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDsuoManufactureSocketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoDestroySocketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscDsuoAcquirePacket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoReleasePacket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

ANSC_STATUS
AnscDsuoManufacturePacketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoDestroyPacketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscDsuoAssignEngine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

ANSC_STATUS
AnscDsuoManufactureEnginePool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDsuoDestroyEnginePool
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSUO_PROCESS.C
***********************************************************/

ULONG
AnscDsuoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscDsuoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscDsuoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize
    );


#endif
