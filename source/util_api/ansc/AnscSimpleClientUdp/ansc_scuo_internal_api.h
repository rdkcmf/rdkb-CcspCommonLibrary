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

    module:	ansc_scuo_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Simple Client Udp
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/12/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_SCUO_INTERNAL_API_
#define  _ANSC_SCUO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_SCUO_STATES.C
***********************************************************/

PUCHAR
AnscScuoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscScuoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

PUCHAR
AnscScuoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscScuoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_HANDLE
AnscScuoGetWorker
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoSetWorker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hWorker,
        ULONG                       ulSize
    );

ULONG
AnscScuoGetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoSetMaxMessageSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ULONG
AnscScuoGetMode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoSetMode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulMode
    );

ANSC_STATUS
AnscScuoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN ANSC_SCUO_RECV.C
***********************************************************/

ANSC_STATUS
AnscScuoRecvTask
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_SCUO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscScuoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscScuoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_SCUO_PROCESS.C
***********************************************************/

PVOID
AnscScuoGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscScuoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscScuoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscScuoSend2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPeerAddr,
        USHORT                      usPeerPort,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


#endif
