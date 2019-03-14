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

    module:	ansc_dkuo_internal_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the Daemon Socket Udp
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


#ifndef  _ANSC_DKUO_INTERNAL_API_
#define  _ANSC_DKUO_INTERNAL_API_


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DKUO_STATES.C
***********************************************************/

PUCHAR
AnscDkuoGetPeerAddress
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoSetPeerAddress
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

USHORT
AnscDkuoGetPeerPort
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoSetPeerPort
    (
        ANSC_HANDLE                 hThisObject,
        USHORT                      usPort
    );

ANSC_HANDLE
AnscDkuoGetDaemonServer
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoSetDaemonServer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hServer
    );

ANSC_HANDLE
AnscDkuoGetDaemonEngine
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoSetDaemonEngine
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEngine
    );

ANSC_HANDLE
AnscDkuoGetClientContext
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoSetClientContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hClient
    );

ANSC_HANDLE
AnscDkuoGetPacket
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoSetPacket
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPacket
    );

ANSC_STATUS
AnscDkuoReturn
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_DKUO_OPERATION.C
***********************************************************/

ANSC_STATUS
AnscDkuoFinish
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoOpen
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoClose
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscDkuoEnableRecv
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    );

ANSC_STATUS
AnscDkuoEnableSend
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnable
    );


/***********************************************************
         FUNCTIONS IMPLEMENTED IN ANSC_DKUO_PROCESS.C
***********************************************************/

ANSC_STATUS
AnscDkuoRecv
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscDkuoSend
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );


#endif
