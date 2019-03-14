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

    module:	ansc_dsto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Daemon Server Tcp
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


#ifndef  _ANSC_DSTO_INTERNAL_API_
#define  _ANSC_DSTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSTO_STATES.C
***********************************************************/

PUCHAR
AnscDstoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscDstoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_HANDLE
AnscDstoGetSdtoPatient
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscDstoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

ULONG
AnscDstoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ULONG
AnscDstoGetEngineCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetEngineCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDstoGetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetMinSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDstoGetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetMaxSocketCount
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ULONG
AnscDstoGetMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_HANDLE
AnscDstoGetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoSetTlsScsIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
AnscDstoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSTO_ACCEPT.C
***********************************************************/

ANSC_STATUS
AnscDstoAcceptTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSTO_WORKER.C
***********************************************************/

ANSC_STATUS
AnscDstoWorkerTask
    (
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDstoAsyncJobTask
    (
        ANSC_HANDLE                 hSocket
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DSTO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscDstoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoStartEngines
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoStopEngines
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_DSTO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscDstoGetSocketByAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address,
        USHORT                      port
    );

ANSC_HANDLE
AnscDstoGetSocketByOsocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_SOCKET                 socket
    );

ANSC_HANDLE
AnscDstoAcquireSocket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoReleaseSocket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket
    );

ANSC_STATUS
AnscDstoManufactureSocketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoDestroySocketPool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscDstoAssignEngine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoManufactureEnginePool
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoDestroyEnginePool
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSTO_PROCESS.C
***********************************************************/

ULONG
AnscDstoQuery
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        PANSC_HANDLE                phQueryContext
    );

ANSC_STATUS
AnscDstoProcessSync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );

ANSC_STATUS
AnscDstoProcessAsync
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSocket,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hQueryContext
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DSTO_PATIENT.C
***********************************************************/

ANSC_STATUS
AnscDstoPoOverview
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm
    );

ANSC_STATUS
AnscDstoPoSnapshot
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm,
        ULONG                       ulIndex
    );

ANSC_STATUS
AnscDstoPoTraceTransaction
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDstoPoEngineCloseUp
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hForm,
        ULONG                       ulIndex
    );


#endif
