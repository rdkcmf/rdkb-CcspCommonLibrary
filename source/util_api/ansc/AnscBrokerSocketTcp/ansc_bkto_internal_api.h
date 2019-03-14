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

    module:	ansc_bkto_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Broker Socket Tcp
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


#ifndef  _ANSC_BKTO_INTERNAL_API_
#define  _ANSC_BKTO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_BKTO_STATES.C
***********************************************************/

PUCHAR
AnscBktoGetHostAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetHostAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscBktoGetHostPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetHostPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

PUCHAR
AnscBktoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscBktoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_HANDLE
AnscBktoGetBrokerServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetBrokerServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServer
    );

ANSC_HANDLE
AnscBktoGetBrokerEngine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetBrokerEngine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEngine
    );

ANSC_HANDLE
AnscBktoGetClientContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetClientContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClient
    );

ULONG
AnscBktoGetBufferSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetBufferSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscBktoGetBufferContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoSetBufferContext
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hBufferTag
    );

BOOL
AnscBktoIsToBeCleaned
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoToClean
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable,
        ULONG                       ulSeconds
    );

ANSC_STATUS
AnscBktoReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_BKTO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscBktoFinish
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscBktoClose
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_BKTO_PROCESS.C
***********************************************************/

PVOID
AnscBktoGetRecvBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscBktoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscBktoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


#endif
