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

    module:	tls_hsoclient_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Hso Client
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/01/03    initial revision.

**********************************************************************/


#ifndef  _TLS_HSOCLIENT_INTERNAL_API_
#define  _TLS_HSOCLIENT_INTERNAL_API_


/***********************************************************
       FUNCTIONS IMPLEMENTED IN TLS_HSOCLIENT_STATES.C
***********************************************************/

ANSC_STATUS
TlsHsoClientReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN TLS_HSOCLIENT_CONTROL.C
***********************************************************/

ANSC_STATUS
TlsHsoClientStart
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN TLS_HSOCLIENT_PROCESS.C
***********************************************************/

ANSC_STATUS
TlsHsoClientRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
TlsHsoClientSend
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


/***********************************************************
     FUNCTIONS IMPLEMENTED IN TLS_HSOCLIENT_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsHsoClientStateTimerInvoke
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_HSOCLIENT_RECV.C
***********************************************************/

ANSC_STATUS
TlsHsoClientRecvHelloRequest
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoClientRecvServerHello
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoClientRecvCertificate
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoClientRecvServerKeyExchange
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoClientRecvCertRequest
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoClientRecvServerHelloDone
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );

ANSC_STATUS
TlsHsoClientRecvFinished
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_HSOCLIENT_SEND.C
***********************************************************/

ANSC_STATUS
TlsHsoClientSendClientHello
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoClientSendCertificate
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoClientSendClientKeyExchange
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoClientSendCertVerify
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsHsoClientSendFinished
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
