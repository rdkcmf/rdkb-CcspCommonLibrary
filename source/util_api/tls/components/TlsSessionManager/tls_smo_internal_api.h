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

    module:	tls_smo_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Session Manager
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/16/03    initial revision.

**********************************************************************/


#ifndef  _TLS_SMO_INTERNAL_API_
#define  _TLS_SMO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_SMO_STATES.C
***********************************************************/

ANSC_STATUS
TlsSmoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsSmoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsSmoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsSmoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_SMO_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsSmoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsSmoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsSmoPatrolTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_SMO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
TlsSmoGetSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

ANSC_STATUS
TlsSmoAddSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

ANSC_STATUS
TlsSmoDelSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

ANSC_STATUS
TlsSmoDelAllSessions
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN TLS_SMO_HSMIF.C
***********************************************************/

ANSC_STATUS
TlsSmoHsmSelHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        ULONG                       conn_end,
        ANSC_HANDLE                 hSessionState
    );

ANSC_STATUS
TlsSmoHsmGetHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size,
        ANSC_HANDLE                 hSessionState
    );

ANSC_STATUS
TlsSmoHsmAddHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSessionState
    );

ANSC_STATUS
TlsSmoHsmDelHandshakeSession
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       host_id,
        ULONG                       peer_id,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

ANSC_STATUS
TlsSmoHsmDelAllHandshakeSessions
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
