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

    module:	tls_cco_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Conn Controller
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/26/03    initial revision.

**********************************************************************/


#ifndef  _TLS_CCO_INTERNAL_API_
#define  _TLS_CCO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CCO_STATES.C
***********************************************************/

ANSC_STATUS
TlsCcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsCcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsCcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_CCO_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsCcoAcqAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsCcoRelAccess
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsCcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsCcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsCcoConfig
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnParams
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CCO_CONTROL.C
***********************************************************/

ANSC_STATUS
TlsCcoStartHandshake
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsCcoCloseSession
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_CCO_PROCESS.C
***********************************************************/

ANSC_STATUS
TlsCcoRecvTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
TlsCcoSendAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN TLS_CCO_CBCIF.C
***********************************************************/

ANSC_STATUS
TlsCcoCbcNotifyEvent
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulEvent,
        ULONG                       ulError,
        ANSC_HANDLE                 hEventContext
    );

ANSC_HANDLE
TlsCcoCbcGetConnParams
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsCcoCbcSetSessionID
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      session_id,
        ULONG                       session_id_size
    );

ANSC_STATUS
TlsCcoCbcSetSessionSharing
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bSessionSharing
    );

ANSC_STATUS
TlsCcoCbcSetQuickHandshake
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bQuickHandshake
    );

ANSC_STATUS
TlsCcoCbcSetCipherSuiteArray
    (
        ANSC_HANDLE                 hThisObject,
        void*                       item_array,
        ULONG                       item_count
    );

ANSC_STATUS
TlsCcoCbcSetCompressionArray
    (
        ANSC_HANDLE                 hThisObject,
        void*                       item_array,
        ULONG                       item_count
    );

ANSC_STATUS
TlsCcoCbcGenerateAlert
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       alert_level,
        UCHAR                       alert_descr
    );

ANSC_STATUS
TlsCcoCbcChangeCipherSpec
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
