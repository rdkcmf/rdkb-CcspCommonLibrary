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

    module:	tls_hsoserver_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Hso Server
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/05/03    initial revision.

**********************************************************************/


#ifndef  _TLS_HSOSERVER_INTERNAL_API_
#define  _TLS_HSOSERVER_INTERNAL_API_


/***********************************************************
       FUNCTIONS IMPLEMENTED IN TLS_HSOSERVER_STATES.C
***********************************************************/

ANSC_STATUS
TlsHsoServerReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN TLS_HSOSERVER_CONTROL.C
***********************************************************/

ANSC_STATUS
TlsHsoServerStart
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN TLS_HSOSERVER_PROCESS.C
***********************************************************/

ANSC_STATUS
TlsHsoServerRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
TlsHsoServerSend
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


/***********************************************************
     FUNCTIONS IMPLEMENTED IN TLS_HSOSERVER_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsHsoServerStateTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_HSOSERVER_RECV.C
***********************************************************/

ANSC_STATUS
TlsHsoServerRecvClientHello
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoServerRecvCertificate
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoServerRecvClientKeyExchange
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoServerRecvCertVerify
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoServerRecvFinished
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_HSOSERVER_SEND.C
***********************************************************/

ANSC_STATUS
TlsHsoServerSendHelloRequest
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoServerSendServerHello
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoServerSendCertificate
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoServerSendServerKeyExchange
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoServerSendCertRequest
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoServerSendServerHelloDone
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoServerSendFinished
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
