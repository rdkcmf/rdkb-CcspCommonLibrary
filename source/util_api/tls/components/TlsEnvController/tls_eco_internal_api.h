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

    module:	tls_eco_internal_api.h

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the TLS Env Controller
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/18/03    initial revision.

**********************************************************************/


#ifndef  _TLS_ECO_INTERNAL_API_
#define  _TLS_ECO_INTERNAL_API_


/***********************************************************
          FUNCTIONS IMPLEMENTED IN TLS_ECO_STATES.C
***********************************************************/

ANSC_STATUS
TlsEcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsEcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

ANSC_STATUS
TlsEcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsEcoReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_ECO_OPERATION.C
***********************************************************/

ANSC_STATUS
TlsEcoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
TlsEcoCancel
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN TLS_ECO_UTILITIES.C
***********************************************************/

ANSC_STATUS
TlsEcoSsl2ToSsl3ClientHello
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pSrcMessage,
        ULONG                       ulSrcMsgSize,
        void*                       pDstMessage,
        PULONG                      pulDstMsgSize
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN TLS_ECO_SCSIF.C
***********************************************************/

ANSC_STATUS
TlsEcoScsConfigModule
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsModuleParams
    );

ANSC_HANDLE
TlsEcoScsCreateConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsTsaIf
    );

ANSC_STATUS
TlsEcoScsConfigConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hTlsConnParams
    );

ANSC_STATUS
TlsEcoScsRemoveConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

ANSC_STATUS
TlsEcoScsStartConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

ANSC_STATUS
TlsEcoScsCloseConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

BOOL
TlsEcoScsQueryConnection
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection
    );

ANSC_STATUS
TlsEcoScsRecvTlsMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hMessageBdo
    );

ANSC_STATUS
TlsEcoScsSendAppMessage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTlsConnection,
        ANSC_HANDLE                 hMessageBdo
    );


/***********************************************************
           FUNCTIONS IMPLEMENTED IN TLS_ECO_MECIF.C
***********************************************************/

ANSC_HANDLE
TlsEcoMecGetTlsSessionManager
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
TlsEcoMecGetTlsCryptoProvider
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
TlsEcoMecGetTlsHsmIf
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
TlsEcoMecGetModuleParams
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
TlsEcoMecGetCertChain1
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cert_bits,
        PUCHAR                      cert_id
    );

ANSC_HANDLE
TlsEcoMecGetCertChain2
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      cert_id
    );

ANSC_HANDLE
TlsEcoMecGetCertChain3
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       client_cert_type,
        ANSC_HANDLE                 authority_name,
        PUCHAR                      cert_id
    );

ANSC_HANDLE
TlsEcoMecGetProfile
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      cert_id
    );

ANSC_HANDLE
TlsEcoMecGetCACertByCert
    (
        ANSC_HANDLE                 hThisObject,
        void*                       cert_data,
        ULONG                       cert_size
    );

ANSC_HANDLE
TlsEcoMecGetCACertByKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 issuer_name,
        ANSC_HANDLE                 issuer_key
    );

ANSC_HANDLE
TlsEcoMecGetCACertByIssuer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 issuer_name,
        ANSC_HANDLE                 issuer_key_id,
        ANSC_HANDLE                 issuer_serial
    );


#endif
