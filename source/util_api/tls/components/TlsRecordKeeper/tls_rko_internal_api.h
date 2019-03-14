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

    module:	tls_rko_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Record Keeper
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


#ifndef  _TLS_RKO_INTERNAL_API_
#define  _TLS_RKO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_RKO_STATES.C
***********************************************************/

ANSC_HANDLE
TlsRkoGetTlsCbcIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsRkoSetTlsCbcIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
TlsRkoGetTlsMecIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsRkoSetTlsMecIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
TlsRkoGetTlsTsaIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsRkoSetTlsTsaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
TlsRkoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsRkoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsRkoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsRkoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_RKO_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsRkoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsRkoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsRkoChangeStateW
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecordState
    );

ANSC_STATUS
TlsRkoChangeStateR
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecordState
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_RKO_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
TlsRkoGetRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    );

ANSC_STATUS
TlsRkoAddRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRecordClient
    );

ANSC_STATUS
TlsRkoPopRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    );

ANSC_STATUS
TlsRkoDelRecordClient
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type
    );

ANSC_STATUS
TlsRkoDelAllRecordClients
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_RKO_PROCESS.C
***********************************************************/

ANSC_STATUS
TlsRkoRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
TlsRkoSend
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type,
        void*                       buffer,
        ULONG                       ulSize
    );


#endif
