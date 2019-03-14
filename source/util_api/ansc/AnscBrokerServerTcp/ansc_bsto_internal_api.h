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

    module:	ansc_bsto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Broker Server Tcp
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


#ifndef  _ANSC_BSTO_INTERNAL_API_
#define  _ANSC_BSTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_BSTO_STATES.C
***********************************************************/

PUCHAR
AnscBstoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscBstoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_HANDLE
AnscBstoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

ULONG
AnscBstoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ULONG
AnscBstoGetEngineCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetEngineCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscBstoGetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscBstoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscBstoGetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetSocketTimeOut
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSeconds
    );

ULONG
AnscBstoGetMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
AnscBstoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_BSTO_ACCEPT.C
***********************************************************/

ANSC_STATUS
AnscBstoAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_BSTO_WORKER.C
***********************************************************/

ANSC_STATUS
AnscBstoWorkerTask
    (
        ANSC_HANDLE                 hSocket
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_BSTO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscBstoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoStartEngines
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoStopEngines
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_BSTO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscBstoGetSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

ANSC_HANDLE
AnscBstoAddSocket
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port,
        ANSC_HANDLE                 hClientContext
    );

ANSC_STATUS
AnscBstoDelSocketByAddr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr,
        USHORT                      dst_port,
        USHORT                      src_port
    );

ANSC_STATUS
AnscBstoDelSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscBstoDelAllSockets
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscBstoAcquireSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoReleaseSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscBstoManufactureSocketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoDestroySocketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscBstoAssignEngine
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      dst_addr
    );

ANSC_STATUS
AnscBstoManufactureEnginePool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBstoDestroyEnginePool
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_BSTO_PROCESS.C
***********************************************************/

ULONG
AnscBstoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
AnscBstoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
AnscBstoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );


#endif
