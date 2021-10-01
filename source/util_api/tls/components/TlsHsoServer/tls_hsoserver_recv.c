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
RFC 2246 The TLS Protocol Version 1.0 January 1999

Full Copyright Statement

Copyright (C) The Internet Society (1999). All Rights Reserved.

This document and translations of it may be copied and furnished to
others, and derivative works that comment on or otherwise explain it
or assist in its implementation may be prepared, copied, published
and distributed, in whole or in part, without restriction of any
kind, provided that the above copyright notice and this paragraph are
included on all such copies and derivative works. However, this
document itself may not be modified in any way, such as by removing
the copyright notice or references to the Internet Society or other
Internet organizations, except as needed for the purpose of
developing Internet standards in which case the procedures for
copyrights defined in the Internet Standards process must be
followed, or as required to translate it into languages other than
English.

The limited permissions granted above are perpetual and will not be
revoked by the Internet Society or its successors or assigns.

This document and the information contained herein is provided on an
"AS IS" basis and THE INTERNET SOCIETY AND THE INTERNET ENGINEERING
TASK FORCE DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO ANY WARRANTY THAT THE USE OF THE INFORMATION
HEREIN WILL NOT INFRINGE ANY RIGHTS OR ANY IMPLIED WARRANTIES OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
**********************************************************************/


/**********************************************************************

    module:	tls_hsoserver_recv.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Hso Server Object.

        *   TlsHsoServerRecvClientHello
        *   TlsHsoServerRecvCertificate
        *   TlsHsoServerRecvClientKeyExchange
        *   TlsHsoServerRecvCertVerify
        *   TlsHsoServerRecvFinished

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


#include "tls_hsoserver_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerRecvClientHello
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the incoming message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoServerRecvClientHello
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject           = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj      = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState       = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams     = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf           = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf           = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider  = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_HSM_INTERFACE              pTlsHsmIf           = (PTLS_HSM_INTERFACE           )pTlsMecIf->GetTlsHsmIf         (pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams         = (PTLS_CONNECTION_PARAMS       )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);
    PTLS_HS_CLIENT_HELLO            pTlsHsClientHello   = (PTLS_HS_CLIENT_HELLO         )buffer;
    ULONG                           ulTlsHsBodySize     = (ULONG                        )ulSize;
    PTLS_HS_SESSION_ID              pTlsHsSessionId     = (PTLS_HS_SESSION_ID           )NULL;
    BOOL                            bCipherSuiteMatched = (BOOL                         )FALSE;
    BOOL                            bCompressionMatched = (BOOL                         )FALSE;
    BOOL                            bValidClientHello   = (BOOL                         )TRUE;
    ULONG                           i                   = 0;

    AnscTrace("TlsHsoServer: recv Client Hello message size = %lu!\n", ulSize);

    /*
     * When a client first connects to a server it is required to send the client hello as its
     * first message. The client can also send a client hello in response to a hello request or on
     * its own initiative in order to renegotiate the security parameters in an existing connection.
     */
    if ( (pMyObject->HsServerState != TLS_HSS_STATE_waitClientHello) &&
         (pMyObject->HsServerState != TLS_HSS_STATE_idle           ) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    TlsHsClientHelloValidate(pTlsHsClientHello, ulTlsHsBodySize, bValidClientHello);

    if ( !bValidClientHello )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decodeError
                );

        return  ANSC_STATUS_FAILURE;
    }

    /*
     * When a client first connects to a server it is required to send the client hello as its
     * first message. The client can also send a client hello in response to a hello request or
     * on its own initiative in order to renegotiate the security parameters in an existing
     * connection.
     *
     *      struct {
     *          ProtocolVersion     client_version;
     *          Random              random;
     *          SessionID           session_id;
     *          CipherSuite         cipher_suites<2..2^16-1>;
     *          CompressionMethod   compression_methods<1..2^8-1>;
     *      } ClientHello;
     */
    if ( TlsHsClientHelloGetMajorVersion(pTlsHsClientHello) != 3 )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_protocolVersion
                );

        return  ANSC_STATUS_FAILURE;
    }
    else if ( (TlsHsClientHelloGetMinorVersion(pTlsHsClientHello) != 1) &&
              (TlsHsClientHelloGetMinorVersion(pTlsHsClientHello) != 0) )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_protocolVersion
                );

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pSecurityParams->MinorVersion = TlsHsClientHelloGetMinorVersion(pTlsHsClientHello);
    }

    /*
     * If the ClientHello.session_id was non-empty, the server will look in its session cache for a
     * match. If a match is found and the server is willing to establish the new connection using
     * the specified session state, the server will respond with the same value as was supplied by
     * the client. This indicates a resumed session and dictates that the parties must proceed
     * directly to the finished messages. The server may return an empty session_id to indicate
     * that the session will not be cached and therefore cannot be resumed. If a session is resumed,
     * it must be resumed using the same cipher suite it was originally negotiated with.
     */
    pTlsHsSessionId = TlsHsClientHelloGetSessionId(pTlsHsClientHello);

    if ( pTlsHsSessionId->Length > 0 )
    {
        returnStatus =
            pTlsHsmIf->GetHandshakeSession
                (
                    pTlsHsmIf->hOwnerContext,
                    pConnParams->HostID,
                    pConnParams->PeerID,
                    pTlsHsSessionId->Identifier,
                    pTlsHsSessionId->Length,
                    (ANSC_HANDLE)pSessionState
                );

        if ( returnStatus == ANSC_STATUS_SUCCESS )
        {
            returnStatus =
                pTlsCbcIf->SetSessionID
                    (
                        pTlsCbcIf->hOwnerContext,
                        pSessionState->SessionID,
                        pSessionState->SessionIDSize
                    );

            pTlsCbcIf->SetQuickHandshake  (pTlsCbcIf->hOwnerContext, TRUE);
            pTlsCbcIf->SetCipherSuiteArray(pTlsCbcIf->hOwnerContext, &pSessionState->CipherSuite, 1);
            pTlsCbcIf->SetCompressionArray(pTlsCbcIf->hOwnerContext, &pSessionState->Compression, 1);
        }
        else
        {
            pTlsCbcIf->SetQuickHandshake(pTlsCbcIf->hOwnerContext, FALSE);
        }
    }
    else
    {
        pTlsCbcIf->SetQuickHandshake(pTlsCbcIf->hOwnerContext, FALSE);
    }

    if ( pConnParams->bQuickHandshake )
    {
        /*
         * If a match is found and the server is willing to establish the new connection using the
         * specified session state, the server will respond with the same value as was supplied by
         * the client. This indicates a resumed session and dictates that the parties must proceed
         * directly to the finished messages.
         */
    }
    else if ( pConnParams->bSessionSharing )
    {
        pSessionState->SessionIDSize = TLS_MAX_SESSION_ID_SIZE;
        returnStatus                 =
            pTlsCryptoProvider->GetRandomBytes
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    (ULONG      )pMyObject,
                    pSessionState->SessionID,
                    TLS_MAX_SESSION_ID_SIZE
                );
    }
    else
    {
        pSessionState->SessionIDSize = 0;
    }

    /*
     * The client hello message includes a random structure, which is used later in the protocol.
     *
     *      struct {
     *          uint32  gmt_unix_time;
     *          opaque  random_bytes[28];
     *      } Random;
     *
     *      gmt_unix_time
     *          The current time and date in standard UNIX 32-bit format (seconds since
     *          the midnight starting Jan 1, 1970, GMT) according to the sender's
     *          internal clock. Clocks are not required to be set correctly by the basic
     *          TLS Protocol; higher level or application protocols may define additional
     *          requirements.
     *
     *      random_bytes
     *          28 bytes generated by a secure random number generator.
     */
    AnscCopyMemory
        (
            pSecurityParams->ClientRandom,
            TlsHsClientHelloGetRandom(pTlsHsClientHello),
            TLS_OPAQUE_RANDOM_SIZE
        );

    /*
     * The CipherSuite list, passed from the client to the server in the client hello message,
     * contains the combinations of cryptographic algorithms supported by the client in order of
     * the client's preference (favorite choice first). Each CipherSuite defines a key exchange
     * algorithm, a bulk encryption algorithm (including secret key length) and a MAC algorithm.
     * The server will select a cipher suite or, if no acceptable choices are presented, return a
     * handshake failure alert and close the connection.
     *
     *      uint8 CipherSuite[2];
     */
    bCipherSuiteMatched = FALSE;

    for ( i = 0; i < pConnParams->CipherSuiteCount; i++ )
    {
        TlsHsClientHelloMatchCipherSuite(pTlsHsClientHello, pConnParams->CipherSuiteArray[i], bCipherSuiteMatched);

        if ( bCipherSuiteMatched )
        {
            break;
        }
    }

    if ( bCipherSuiteMatched )
    {
        USHORT                      cipher_suite_agreed = pConnParams->CipherSuiteArray[i];

        pTlsCbcIf->SetCipherSuiteArray(pTlsCbcIf->hOwnerContext, &cipher_suite_agreed, 1);

        pSessionState->CipherSuite = cipher_suite_agreed;
    }
    else
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_handshakeFailure
                );

        return  ANSC_STATUS_FAILURE;
    }

    /*
     * The client hello includes a list of compression algorithms supported by the client, ordered
     * according to the client's preference.
     *
     *      enum { null(0), (255) } CompressionMethod;
     */
    bCompressionMatched = FALSE;

    for ( i = 0; i < pConnParams->CompressionCount; i++ )
    {
        TlsHsClientHelloMatchCompression(pTlsHsClientHello, pConnParams->CompressionArray[i], bCompressionMatched);

        if ( bCompressionMatched )
        {
            break;
        }
    }

    if ( bCompressionMatched )
    {
        UCHAR                       compression_agreed = pConnParams->CompressionArray[i];

        pTlsCbcIf->SetCompressionArray(pTlsCbcIf->hOwnerContext, &compression_agreed, 1);

        pSessionState->Compression = compression_agreed;
    }
    else
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_handshakeFailure
                );

        return  ANSC_STATUS_FAILURE;
    }

    if ( TRUE )
    {
        pSessionState->KeAlgorithm =
            pTlsCryptoProvider->GetKeAlgorithm
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pSessionState->CipherSuite
                );
    }

    if ( TRUE )
    {
        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    returnStatus = pMyObject->SendServerHello((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerRecvCertificate
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the incoming message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoServerRecvCertificate
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    ULONG                           ulClientCertType   = (ULONG                        )0;
    PTLS_HS_CERTIFICATE             pTlsHsCertificate  = (PTLS_HS_CERTIFICATE          )buffer;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )ulSize;
    BOOL                            bValidCertificate  = (BOOL                         )TRUE;
    ANSC_X509_CERTIFICATE_CHAIN*    pPeerCertChain     = (ANSC_X509_CERTIFICATE_CHAIN* )NULL;
    ANSC_X509_CERTIFICATE*          pPeerCert          = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X509_CERTIFICATE*          pCommonCACert      = (ANSC_X509_CERTIFICATE*       )NULL;
    void*                           peer_cert_data     = (void*                        )NULL;
    ULONG                           peer_cert_size     = (ULONG                        )0;
    ULONG                           ulCertCount        = (ULONG                        )0;
    ULONG                           i                  = 0;

    AnscTrace("TlsHsoServer: recv Certificate message size = %lu!\n", ulSize);

    /*
     * This is the first message the client can send after receiving a server hello done message.
     * This message is only sent if the server requests a certificate. If no suitable certificate
     * is available, the client should send a certificate message containing no certificates. If
     * client authentication is required by the server for the handshake to continue, it may res-
     * pond with a fatal handshake failure alert.
     */
    if ( (pMyObject->HsServerState != TLS_HSS_STATE_waitCertificate      ) &&
         (pMyObject->HsServerState != TLS_HSS_STATE_waitClientKeyExchange) )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        return  ANSC_STATUS_FAILURE;
    }
    else if ( pSecurityParams->PeerCertChain != NULL )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        ulClientCertType =
            pTlsCryptoProvider->GetClientCertType
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pSessionState->CipherSuite
                );
    }

    TlsHsCertValidate(pTlsHsCertificate, ulTlsHsBodySize, bValidCertificate);

    if ( !bValidCertificate )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decodeError
                );

        return  ANSC_STATUS_FAILURE;
    }

    /*
     * This is a sequence (chain) of X.509v3 certificates. The sender's certificate must come first
     * in the list. Each following certificate must directly certify the one preceding it. Because
     * certificate validation requires that root keys be distributed independently, the self-signed
     * certificate which specifies the root certificate authority may optionally be omitted from
     * the chain, under the assumption that the remote end must already possess it in order to
     * validate it in any case.
     *
     *      opaque ASN.1Cert<1..2^24-1>;
     *
     *      struct {
     *          ASN.1Cert certificate_list<0..2^24-1>;
     *      } Certificate;
     */
    TlsHsCertGetCertCount(pTlsHsCertificate, ulCertCount);

    if ( (ulCertCount == 0                        ) ||
         (ulCertCount >= ANSC_CERT_MAX_CHAIN_DEPTH) )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_certificateUnknown
                );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }
    else
    {
        pPeerCertChain = (ANSC_X509_CERTIFICATE_CHAIN*)AnscAllocateMemory(sizeof(ANSC_X509_CERTIFICATE_CHAIN));

        if ( !pPeerCertChain )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pPeerCertChain->Depth = 0;
        }
    }

    for ( i = 0; i < ulCertCount; i++ )
    {
        TlsHsCertGetCert2
            (
                pTlsHsCertificate,
                peer_cert_data,
                peer_cert_size,
                i
            );

        if ( !peer_cert_data || !peer_cert_size )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_badCertificate
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }
        else
        {
            pPeerCert =
                (ANSC_X509_CERTIFICATE*)AnscX509CertGetCertWrap
                    (
                        peer_cert_data,
                        peer_cert_size
                    );

            if ( !pPeerCert )
            {
                returnStatus = ANSC_STATUS_RESOURCES;

                goto  EXIT2;
            }
            else
            {
                pPeerCertChain->Array[pPeerCertChain->Depth++] = pPeerCert;
            }
        }
    }

    /*
     * Because certificate validation requires that root keys be distributed independently, the
     * self-signed certificate which specifies the root certificate authority may optionally be
     * omitted from the chain, under the assumption that the remote end must already possess it in
     * order to validate it in any case.
     */
    if ( pPeerCert->bSelfSigned )
    {
        pCommonCACert =
            (ANSC_X509_CERTIFICATE*)pTlsMecIf->GetCACertByCert
                (
                    pTlsMecIf->hOwnerContext,
                    pPeerCert->CertData,
                    pPeerCert->CertSize
                );

        if ( !pCommonCACert )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_unknownCa
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }
    }
    else
    {
        ANSC_X501_NAME*             issuer_name           = NULL;
        ANSC_ASN1_OCTET_STRING*     issuer_key_identifier = NULL;
        ANSC_ASN1_INT*              issuer_serial_number  = NULL;
        ANSC_X509_CERTIFICATE*      issuer_cert           = NULL;

        issuer_name           = AnscX509CertGetIssuer      (pPeerCert->CertData, pPeerCert->CertSize);
        issuer_key_identifier = AnscX509CertGetIssuerKeyID (pPeerCert->CertData, pPeerCert->CertSize);
        issuer_serial_number  = AnscX509CertGetSerialNumber(pPeerCert->CertData, pPeerCert->CertSize);

        pCommonCACert =
            (ANSC_X509_CERTIFICATE*)pTlsMecIf->GetCACertByIssuer
                (
                    pTlsMecIf->hOwnerContext,
                    (ANSC_HANDLE)issuer_name,
                    (ANSC_HANDLE)issuer_key_identifier,
                    (ANSC_HANDLE)NULL   /* 'issuer_serial_number' is actually not the issuer's serial number*/
                );

        if ( TRUE )
        {
            if ( issuer_name )
            {
                AnscFreeX501Name(issuer_name);
            }

            if ( issuer_key_identifier )
            {
                AnscAsn1FreeOctetString(issuer_key_identifier);
            }

            if ( issuer_serial_number )
            {
                AnscAsn1FreeInt(issuer_serial_number);
            }
        }

        if ( !pCommonCACert )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_unknownCa
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT2;
        }
        else
        {
            AnscCloneX509Certificate(pCommonCACert, issuer_cert);
        }

        if ( !issuer_cert )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
        else
        {
            pPeerCertChain->Array[pPeerCertChain->Depth++] = issuer_cert;
        }
    }

    /*
     * All certificate profiles, key and cryptographic formats are defined by the IETF PKIX working
     * group. When a key usage extension is present, the digitalSignature bit must be set for the
     * key to be eligible for signing, as described above, and the keyEncipherment bit must be pre-
     * sent to allow encryption, as described above. The keyAgreement bit must be set on Diffie-
     * Hellman certificates.
     */
    if ( TRUE )
    {
        pPeerCert = pPeerCertChain->Array[0];

        switch ( ulClientCertType )
        {
            case    TLS_CLIENT_CERT_TYPE_rsaSign :

                    if ( (pPeerCert->Signing                                          != ANSC_CERT_signing_rsa) ||
                         (pPeerCert->KeyType                                          != ANSC_CERT_keyType_rsa) ||
                         ((pPeerCert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) == 0                    ) )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_badCertificate
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT2;
                    }

                    break;

            case    TLS_CLIENT_CERT_TYPE_dssSign :

                    if ( (pPeerCert->Signing                                          != ANSC_CERT_signing_dsa) ||
                         (pPeerCert->KeyType                                          != ANSC_CERT_keyType_dsa) ||
                         ((pPeerCert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) == 0                    ) )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_badCertificate
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT2;
                    }

                    break;

            case    TLS_CLIENT_CERT_TYPE_rsaFixedDh :

                    if ( (pPeerCert->Signing                                      != ANSC_CERT_signing_rsa          ) ||
                         (pPeerCert->KeyType                                      != ANSC_CERT_keyType_diffieHellman) ||
                         ((pPeerCert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement) == 0                              ) )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_badCertificate
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT2;
                    }

                    break;

            case    TLS_CLIENT_CERT_TYPE_dssFixedDh :

                    if ( (pPeerCert->Signing                                      != ANSC_CERT_signing_dsa          ) ||
                         (pPeerCert->KeyType                                      != ANSC_CERT_keyType_diffieHellman) ||
                         ((pPeerCert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement) == 0                              ) )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_badCertificate
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT2;
                    }

                    break;

            default :

                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_internalError
                            );

                    returnStatus = ANSC_STATUS_FAILURE;

                    goto  EXIT2;

                    break;
        }
    }

    /*
     * A user of a security service requiring knowledge of a public key generally needs to obtain
     * and validate a certificate containing the required public key. If the public-key user does
     * not already hold an assured copy of the public key of the CA that signed the certificate,
     * the CA's name, and related information (such as the validity period or name constraints),
     * then it might need an additional certificate to obtain that public key. In general, a chain
     * of multiple certificates may be needed, comprising a certificate of the public key owner
     * (the end entity) signed by one CA, and zero or more additional certificates of CAs signed by
     * other CAs.  Such chains, called certification paths, are required because a public key user
     * is only initialized with a limited number of assured CA public keys.
     */
    if ( !pTlsCryptoProvider->AuthenticateCertChain
            (
                (ANSC_HANDLE)pTlsCryptoProvider,
                (ANSC_HANDLE)pPeerCertChain
            ) )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_badCertificate
                );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }
    else
    {
        pSecurityParams->PeerCertChain = pPeerCertChain;
    }

    if ( TRUE )
    {
        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    /*
     * The client key exchange message is always sent by the client. It will immediately follow the
     * client certificate message, if it is sent. Otherwise it will be the first message sent by
     * the client after it receives the server hello done message.
     */
    pMyObject->HsServerState = TLS_HSS_STATE_waitClientKeyExchange;

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pPeerCertChain )
    {
        AnscFreeX509CertChain(pPeerCertChain);
    }

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerRecvClientKeyExchange
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the incoming message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoServerRecvClientKeyExchange
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_HS_CLIENT_KEY_EXCHANGE     pTlsHsClientKe     = (PTLS_HS_CLIENT_KEY_EXCHANGE  )buffer;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )ulSize;
    BOOL                            bImplicitKeMsg     = (BOOL                         )(ulSize == 0);
    BOOL                            bValidClientKe     = (BOOL                         )TRUE;
    ANSC_X509_CERTIFICATE*          pPeerCert          = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X509_KEY*                  pPeerPublicKey     = (ANSC_X509_KEY*               )NULL;
    ANSC_ASN1_INT*                  pPeerDHPrime       = (ANSC_ASN1_INT*               )NULL;
    ANSC_ASN1_INT*                  pPeerDHGenerator   = (ANSC_ASN1_INT*               )NULL;
    ANSC_ASN1_INT*                  pPeerDHPublic      = (ANSC_ASN1_INT*               )NULL;
    PVOID                           pEncryptedPms      = (PVOID                        )NULL;
    ULONG                           ulEpmsSize         = (ULONG                        )0;
    PVOID                           pTlsDHPublic       = (PVOID                        )NULL;
    ULONG                           ulDHPublicSize     = (ULONG                        )0;

    AnscTrace("TlsHsoServer: recv Client Key Exchange message size = %lu!\n", ulSize);

    /*
     * This message is always sent by the client. It will immediately follow the client certificate
     * message, if it is sent. Otherwise it will be the first message sent by the client after it
     * receives the server hello done message.
     */
    if ( pMyObject->HsServerState != TLS_HSS_STATE_waitClientKeyExchange )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        return  ANSC_STATUS_FAILURE;
    }

    if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA       ) ||
         (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT) )
    {
        TlsHsClientKeValidate
            (
                pTlsHsClientKe,
                ulTlsHsBodySize,
                pSecurityParams->MinorVersion,
                TLS_HS_KE_ALGORITHM_rsa,
                bImplicitKeMsg,
                bValidClientKe
            );
    }
    else
    {
        TlsHsClientKeValidate
            (
                pTlsHsClientKe,
                ulTlsHsBodySize,
                pSecurityParams->MinorVersion,
                TLS_HS_KE_ALGORITHM_diffieHellman,
                bImplicitKeMsg,
                bValidClientKe
            );
    }

    if ( !bValidClientKe )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decodeError
                );

        return  ANSC_STATUS_FAILURE;
    }

    /*
     * With this message, the premaster secret is set, either though direct transmission of the
     * RSA-encrypted secret, or by the transmission of Diffie-Hellman parameters which will allow
     * each side to agree upon the same premaster secret. When the key exchange method is DH_RSA or
     * DH_DSS, client certification has been requested, and the client was able to respond with a
     * certificate which contained a Diffie-Hellman public key whose parameters (group and
     * generator) matched those specified by the server in its certificate, this message will not
     * contain any data.
     */
    if ( bImplicitKeMsg )
    {
        if ( !pSessionState->bCertRequested )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decryptError
                    );

            return  ANSC_STATUS_FAILURE;
        }
        else if ( (pSessionState->KeAlgorithm != TLS_KE_ALGORITHM_DH_DSS) &&
                  (pSessionState->KeAlgorithm != TLS_KE_ALGORITHM_DH_RSA) )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decryptError
                    );

            return  ANSC_STATUS_FAILURE;
        }
        else if ( !pSecurityParams->PeerCertChain || !pSecurityParams->PeerCertChain->Array[0] )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decryptError
                    );

            return  ANSC_STATUS_FAILURE;
        }

        if ( TRUE )
        {
            pPeerCert      = pSecurityParams->PeerCertChain->Array[0];
            pPeerPublicKey =
                AnscX509CertGetPublicKey
                    (
                        pPeerCert->CertData,
                        pPeerCert->CertSize
                    );

            if ( !pPeerPublicKey )
            {
                returnStatus =
                    pTlsCbcIf->GenerateAlert
                        (
                            pTlsCbcIf->hOwnerContext,
                            TLS_ALERT_LEVEL_fatal,
                            TLS_ALERT_DESCR_internalError
                        );

                return  ANSC_STATUS_FAILURE;
            }
            else if ( pPeerPublicKey->KeyType != ANSC_CERT_keyType_diffieHellman )
            {
                returnStatus =
                    pTlsCbcIf->GenerateAlert
                        (
                            pTlsCbcIf->hOwnerContext,
                            TLS_ALERT_LEVEL_fatal,
                            TLS_ALERT_DESCR_decryptError
                        );

                returnStatus = ANSC_STATUS_FAILURE;

                AnscFreeX509Key(pPeerPublicKey);

                goto  EXIT1;
            }
            else if ( !pSecurityParams->DHPrime || !pSecurityParams->DHGenerator || !pSecurityParams->DHPublicS )
            {
                returnStatus =
                    pTlsCbcIf->GenerateAlert
                        (
                            pTlsCbcIf->hOwnerContext,
                            TLS_ALERT_LEVEL_fatal,
                            TLS_ALERT_DESCR_internalError
                        );

                returnStatus = ANSC_STATUS_FAILURE;

                AnscFreeX509Key(pPeerPublicKey);

                goto  EXIT1;
            }
            else
            {
                returnStatus =
                    pTlsCryptoProvider->DecapDHPublicKey
                        (
                            (ANSC_HANDLE)pTlsCryptoProvider,
                            pPeerPublicKey,
                            &pPeerDHPrime,
                            &pPeerDHGenerator,
                            &pPeerDHPublic
                        );

                if ( !pPeerDHPrime || !pPeerDHGenerator || !pPeerDHPublic )
                {
                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_internalError
                            );

                    returnStatus = ANSC_STATUS_FAILURE;

                    AnscFreeX509Key(pPeerPublicKey);

                    goto  EXIT2;
                }
                else if ( AnscAsn1EqualInts(pPeerDHPrime,     pSecurityParams->DHPrime    ) &&
                          AnscAsn1EqualInts(pPeerDHGenerator, pSecurityParams->DHGenerator) )
                {
                    pSecurityParams->DHKeyPubC           = pPeerPublicKey;
                    pSecurityParams->DHPublicC           = (ANSC_ASN1_INT*)AnscAsn1CloneInt((ANSC_HANDLE)pPeerDHPublic);
                    pSecurityParams->PreMasterSecretSize = TLS_PRE_MASTER_SECRET_SIZE;
                    returnStatus                         =
                        pTlsCryptoProvider->AgreeDHSecret
                            (
                                (ANSC_HANDLE)pTlsCryptoProvider,
                                pSecurityParams->DHKeyPubS,
                                pSecurityParams->DHKeyPubC,
                                pSecurityParams->DHKeyPrvS,
                                pSecurityParams->PreMasterSecret,
                                &pSecurityParams->PreMasterSecretSize
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );


                        AnscFreeX509Key(pPeerPublicKey);

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT2;
                    }
                }
                else
                {
                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_decryptError
                            );

                    AnscFreeX509Key(pPeerPublicKey);

                    returnStatus = ANSC_STATUS_FAILURE;

                    goto  EXIT2;
                }

                AnscAsn1FreeInt(pPeerDHPrime    );
                AnscAsn1FreeInt(pPeerDHGenerator);
                AnscAsn1FreeInt(pPeerDHPublic   );

                pPeerDHPrime        = NULL;
                pPeerDHGenerator    = NULL;
                pPeerDHPublic       = NULL;
            }
        }
    }
    else
    {
        switch ( pSessionState->KeAlgorithm )
        {
            case    TLS_KE_ALGORITHM_RSA        :
            case    TLS_KE_ALGORITHM_RSA_EXPORT :

                    if ( !pSecurityParams->RsaKeyPrv )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT1;
                    }
                    else
                    {
                        TlsHsClientKeGetEpms2
                            (
                                pTlsHsClientKe,
                                ulTlsHsBodySize,
                                pSecurityParams->MinorVersion,
                                pEncryptedPms,
                                ulEpmsSize
                            );

                        pSecurityParams->PreMasterSecretSize = 48;

                        returnStatus =
                            pTlsCryptoProvider->Decrypt_RSA
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    pSecurityParams->RsaKeyPrv,
                                    pEncryptedPms,
                                    ulEpmsSize,
                                    pSecurityParams->PreMasterSecret,
                                    &pSecurityParams->PreMasterSecretSize
                                );
                    }

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_decryptError
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT1;
                    }

                    break;

            case    TLS_KE_ALGORITHM_DHE_DSS        :
            case    TLS_KE_ALGORITHM_DHE_DSS_EXPORT :
            case    TLS_KE_ALGORITHM_DHE_RSA        :
            case    TLS_KE_ALGORITHM_DHE_RSA_EXPORT :
            case    TLS_KE_ALGORITHM_DH_DSS         :
            case    TLS_KE_ALGORITHM_DH_RSA         :
            case    TLS_KE_ALGORITHM_DH_ANONYMOUS   :

                    if ( !pSecurityParams->DHKeyPubS || !pSecurityParams->DHKeyPrvS )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT1;
                    }
                    else if ( !pSecurityParams->DHPrime || !pSecurityParams->DHGenerator || !pSecurityParams->DHPublicS )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT1;
                    }
                    else
                    {
                        TlsHsClientKeGetDHPublic2(pTlsHsClientKe, pTlsDHPublic, ulDHPublicSize);

                        pSecurityParams->DHPublicC = AnscAsn1AllocInt(pTlsDHPublic, ulDHPublicSize, FALSE);

                        if ( !pSecurityParams->DHPublicC )
                        {
                            returnStatus =
                                pTlsCbcIf->GenerateAlert
                                    (
                                        pTlsCbcIf->hOwnerContext,
                                        TLS_ALERT_LEVEL_fatal,
                                        TLS_ALERT_DESCR_internalError
                                    );

                            returnStatus = ANSC_STATUS_FAILURE;

                            goto  EXIT1;
                        }

                        pSecurityParams->DHKeyPubC =
                            (ANSC_X509_KEY*)pTlsCryptoProvider->EncapDHPublicKey
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    pSecurityParams->DHPrime,
                                    pSecurityParams->DHGenerator,
                                    pSecurityParams->DHPublicC
                                );

                        if ( !pSecurityParams->DHPublicC )
                        {
                            returnStatus =
                                pTlsCbcIf->GenerateAlert
                                    (
                                        pTlsCbcIf->hOwnerContext,
                                        TLS_ALERT_LEVEL_fatal,
                                        TLS_ALERT_DESCR_decryptError
                                    );

                            returnStatus = ANSC_STATUS_FAILURE;

                            goto  EXIT1;
                        }
                    }

                    if ( TRUE )
                    {
                        pSecurityParams->PreMasterSecretSize = TLS_PRE_MASTER_SECRET_SIZE;
                        returnStatus                         =
                            pTlsCryptoProvider->AgreeDHSecret
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    pSecurityParams->DHKeyPubS,
                                    pSecurityParams->DHKeyPubC,
                                    pSecurityParams->DHKeyPrvS,
                                    pSecurityParams->PreMasterSecret,
                                    &pSecurityParams->PreMasterSecretSize
                                );
                    }

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        returnStatus = ANSC_STATUS_FAILURE;

                        goto  EXIT1;
                    }

                    break;

            default :

                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_internalError
                            );

                    returnStatus = ANSC_STATUS_FAILURE;

                    goto  EXIT1;

                    break;
        }
    }

    if ( TRUE )
    {
        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    /*
     * In order to begin connection protection, the TLS Record Protocol requires specification of a
     * suite of algorithms, a master secret, and the client and server random values. The authen-
     * tication, encryption, and MAC algorithms are determined by the cipher_suite selected by the
     * server and revealed in the server hello message. The compression algorithm is negotiated in
     * the hello messages, and the random values are exchanged in the hello messages. All that
     * remains is to calculate the master secret.
     */
    if ( TRUE )
    {
        pMyObject->Agree((ANSC_HANDLE)pMyObject, FALSE);
    }

    pMyObject->HsServerState = TLS_HSS_STATE_waitCertVerify;

    return  ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pPeerDHPrime )
    {
        AnscAsn1FreeInt(pPeerDHPrime);
    }

    if ( pPeerDHGenerator )
    {
        AnscAsn1FreeInt(pPeerDHGenerator);
    }

    if ( pPeerDHPublic )
    {
        AnscAsn1FreeInt(pPeerDHPublic);
    }

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerRecvCertVerify
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the incoming message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoServerRecvCertVerify
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_HS_CERT_VERIFY             pTlsHsCertVerify   = (PTLS_HS_CERT_VERIFY          )buffer;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )ulSize;
    BOOL                            bValidCertVerify   = (BOOL                         )TRUE;
    ANSC_X509_CERTIFICATE*          pPeerCert          = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X509_KEY*                  pPeerPubKey        = (ANSC_X509_KEY*               )NULL;
    ULONG                           ulHashDataSize     = (ULONG                        )0;
    ULONG                           ulTbhDataSize      = (ULONG                        )0;
    PVOID                           pTlsSignature      = (PVOID                        )NULL;
    ULONG                           ulSignatureSize    = (ULONG                        )0;
    BOOL                            bVerifyResult      = (BOOL                         )FALSE;
    PVOID                           pScratchPad        = (PVOID                        )NULL;
    ULONG                           ulScratchPadSize   = (ULONG                        )0;
    ULONG                           i                  = (ULONG                        )0;
    ULONG                           tbs_data_size      = (ULONG                        )0;
    UCHAR                           tbs_data[36];

    AnscTrace("TlsHsoServer: recv Certificate Verify message size = %lu!\n", ulSize);

    /*
     * This message is used to provide explicit verification of a client certificate. This message
     * is only sent following a client certificate that has signing capability (i.e. all certifi-
     * cates except those containing fixed Diffie-Hellman parameters). When sent, it will imme-
     * diately follow the client key exchange message.
     */
    if ( pMyObject->HsServerState != TLS_HSS_STATE_waitCertVerify )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        return  ANSC_STATUS_FAILURE;
    }
    else if ( !pSessionState->bCertRequested )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        return  ANSC_STATUS_FAILURE;
    }
    else if ( !pSecurityParams->PeerCertChain || !pSecurityParams->PeerCertChain->Array[0] )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_internalError
                );

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pPeerCert   = pSecurityParams->PeerCertChain->Array[0];
        pPeerPubKey = AnscX509CertGetPublicKey(pPeerCert->CertData, pPeerCert->CertSize);

        if ( !pPeerPubKey )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_internalError
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
    }

    if ( (pPeerCert->KeyType != ANSC_CERT_keyType_rsa) &&
         (pPeerCert->KeyType != ANSC_CERT_keyType_dsa) )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }
    else if ( !(pPeerCert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }

    TlsHsCertVerifyValidate(pTlsHsCertVerify, ulTlsHsBodySize, bValidCertVerify);

    if ( !bValidCertVerify )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decodeError
                );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }
    else
    {
        ulScratchPadSize = 1024;
        pScratchPad      = AnscAllocateMemory(ulScratchPadSize);

        if ( !pScratchPad )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    /*
     *  struct {
     *      Signature signature;
     *  } CertificateVerify;
     */
    if ( pSecurityParams->MinorVersion == 0 )
    {
        /*
         * This message is used to provide explicit verification of a client certificate. This
         * message is only sent following any client certificate that has signing capability (i.e.
         * all certificates except those containing fixed Diffie-Hellman parameters).
         *
         *      struct {
         *          Signature signature;
         *      } CertificateVerify;
         *
         *      CertificateVerify.signature.md5_hash
         *          MD5(master_secret + pad_2 + MD5(handshake_messages + master_secret + pad_1));
         *      Certificate.signature.sha_hash
         *          SHA(master_secret + pad_2 + SHA(handshake_messages + master_secret + pad_1));
         *
         * Here handshake_messages refers to all handshake messages starting at client hello up to
         * but not including this message.
         */
        PUCHAR                      pSslPad         = (PUCHAR)pScratchPad;
        ULONG                       ulSslPadSizeMd5 = (ULONG )48;
        ULONG                       ulSslPadSizeSha = (ULONG )40;
        UCHAR                       ucSslPad1Octet  = (UCHAR )0x36;
        UCHAR                       ucSslPad2Octet  = (UCHAR )0x5c;

        /*
         * First Round Hashing:
         *
         *      MD5(handshake_messages + master_secret + pad_1));
         *      SHA(handshake_messages + master_secret + pad_1));
         */
        if ( pPeerPubKey->KeyType == ANSC_CERT_keyType_rsa )
        {
            for ( i = 0; i < ulSslPadSizeMd5; i++ )
            {
                pSslPad[i] = ucSslPad1Octet;
            }

            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSecurityParams->MasterSecret, TLS_MASTER_SECRET_SIZE);
            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSslPad,                       ulSslPadSizeMd5       );

            ulHashDataSize = ANSC_MD5_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_MD5
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        tbs_data,
                        &ulHashDataSize
                    );

            pMyObject->MsgOffset -= TLS_MASTER_SECRET_SIZE;
            pMyObject->MsgOffset -= ulSslPadSizeMd5;
        }

        if ( (pPeerPubKey->KeyType == ANSC_CERT_keyType_rsa) ||
             (pPeerPubKey->KeyType == ANSC_CERT_keyType_dsa) )
        {
            for ( i = 0; i < ulSslPadSizeSha; i++ )
            {
                pSslPad[i] = ucSslPad1Octet;
            }

            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSecurityParams->MasterSecret, TLS_MASTER_SECRET_SIZE);
            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSslPad,                       ulSslPadSizeSha       );

            ulHashDataSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                        &ulHashDataSize
                    );

            pMyObject->MsgOffset -= TLS_MASTER_SECRET_SIZE;
            pMyObject->MsgOffset -= ulSslPadSizeSha;
        }

        /*
         * Second Round Hashing:
         *
         *      MD5(master_secret + pad_2 + MD5(handshake_messages + master_secret + pad_1));
         *      SHA(master_secret + pad_2 + SHA(handshake_messages + master_secret + pad_1));
         */
        if ( pPeerPubKey->KeyType == ANSC_CERT_keyType_rsa )
        {
            ulTbhDataSize = TLS_MASTER_SECRET_SIZE + ulSslPadSizeMd5 + ANSC_MD5_OUTPUT_SIZE;
            pSslPad       = (PUCHAR)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE);

            AnscCopyMemory
                (
                    pScratchPad,
                    pSecurityParams->MasterSecret,
                    TLS_MASTER_SECRET_SIZE
                );

            for ( i = 0; i < ulSslPadSizeMd5; i++ )
            {
                pSslPad[i] = ucSslPad2Octet;
            }

            AnscCopyMemory
                (
                    (PVOID)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE + ulSslPadSizeMd5),
                    tbs_data,
                    ANSC_MD5_OUTPUT_SIZE
                );

            ulHashDataSize = ANSC_MD5_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_MD5
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pScratchPad,
                        ulTbhDataSize,
                        tbs_data,
                        &ulHashDataSize
                    );
        }

        if ( (pPeerPubKey->KeyType == ANSC_CERT_keyType_rsa) ||
             (pPeerPubKey->KeyType == ANSC_CERT_keyType_dsa) )
        {
            ulTbhDataSize = TLS_MASTER_SECRET_SIZE + ulSslPadSizeSha + ANSC_SHA1_OUTPUT_SIZE;
            pSslPad       = (PUCHAR)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE);

            AnscCopyMemory
                (
                    pScratchPad,
                    pSecurityParams->MasterSecret,
                    TLS_MASTER_SECRET_SIZE
                );

            for ( i = 0; i < ulSslPadSizeSha; i++ )
            {
                pSslPad[i] = ucSslPad2Octet;
            }

            AnscCopyMemory
                (
                    (PVOID)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE + ulSslPadSizeSha),
                    (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                    ANSC_SHA1_OUTPUT_SIZE
                );

            ulHashDataSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pScratchPad,
                        ulTbhDataSize,
                        (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                        &ulHashDataSize
                    );
        }
    }
    else
    {
        /*
         * This message is used to provide explicit verification of a client certificate. This
         * message is only sent following a client certificate that has signing capability (i.e.
         * all certificates except those containing fixed Diffie-Hellman parameters). When sent,
         * it will immediately follow the client key exchange message.
         *
         *      struct {
         *          Signature signature;
         *      } CertificateVerify;
         *
         *      CertificateVerify.signature.md5_hash
         *          MD5(handshake_messages);
         *      Certificate.signature.sha_hash
         *          SHA(handshake_messages);
         *
         * Here handshake_messages refers to all handshake messages sent or received starting at
         * client hello up to but not including this message, including the type and length fields
         * of the handshake messages.
         */
        if ( pPeerPubKey->KeyType == ANSC_CERT_keyType_rsa )
        {
            ulHashDataSize = ANSC_MD5_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_MD5
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        tbs_data,
                        &ulHashDataSize
                    );
        }

        if ( (pPeerPubKey->KeyType == ANSC_CERT_keyType_rsa) ||
             (pPeerPubKey->KeyType == ANSC_CERT_keyType_dsa) )
        {
            ulHashDataSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                        &ulHashDataSize
                    );
        }
    }

    TlsHsCertVerifyGetSignature2(pTlsHsCertVerify, pTlsSignature, ulSignatureSize);

    if ( pPeerPubKey->KeyType == ANSC_CERT_keyType_rsa )
    {
        tbs_data_size = ANSC_MD5_OUTPUT_SIZE + ANSC_SHA1_OUTPUT_SIZE;
        bVerifyResult =
            pTlsCryptoProvider->Verify
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pPeerPubKey->KeyType,
                    pPeerPubKey,
                    tbs_data,
                    tbs_data_size,
                    pTlsSignature,
                    ulSignatureSize
                );
    }
    else if ( pPeerPubKey->KeyType == ANSC_CERT_keyType_dsa )
    {
        tbs_data_size = ANSC_SHA1_OUTPUT_SIZE;
        bVerifyResult =
            pTlsCryptoProvider->Verify
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pPeerPubKey->KeyType,
                    pPeerPubKey,
                    (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                    tbs_data_size,
                    pTlsSignature,
                    ulSignatureSize
                );
    }

    if ( !bVerifyResult )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decryptError
                );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT2;
    }

    if ( TRUE )
    {
        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    pMyObject->HsServerState = TLS_HSS_STATE_waitFinished;

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pScratchPad )
    {
        AnscFreeMemory(pScratchPad);
    }

EXIT1:

    if ( pPeerPubKey )
    {
        AnscFreeX509Key(pPeerPubKey);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerRecvFinished
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an incoming message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       buffer
                Specifies the incoming message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoServerRecvFinished
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams        = (PTLS_CONNECTION_PARAMS       )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);
    PTLS_HS_FINISHED                pTlsHsFinished     = (PTLS_HS_FINISHED             )buffer;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )ulSize;
    BOOL                            bValidFinished     = (BOOL                         )TRUE;
    BOOL                            bValidHash         = (BOOL                         )TRUE;
    ULONG                           ulHashDataSize     = (ULONG                        )0;
    ULONG                           ulTbhDataSize      = (ULONG                        )0;
    PVOID                           pScratchPad        = (PVOID                        )NULL;
    ULONG                           ulScratchPadSize   = (ULONG                        )0;
    ULONG                           i                  = (ULONG                        )0;
    UCHAR                           tbs_data[36];

    AnscTrace("TlsHsoServer: recv Finished message size = %lu!\n", ulSize);

    /*
     * A finished message is always sent immediately after a change cipher spec message to verify
     * that the key exchange and authentication processes were successful. It is essential that a
     * change cipher spec message be received between the other handshake messages and the Finished
     * message.
     */
    if ( !(pMyObject->ProgressBits & TLS_HSO_PROGRESS_BIT_changeCipherR) )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        returnStatus = ANSC_STATUS_FAILURE;

        return  returnStatus;
    }

    TlsHsFinishedValidate(pTlsHsFinished, ulTlsHsBodySize, pSecurityParams->MinorVersion, bValidFinished);

    if ( !bValidFinished )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decodeError
                );

        returnStatus = ANSC_STATUS_FAILURE;

        return  returnStatus;
    }
    else
    {
        ulScratchPadSize = 1024;
        pScratchPad      = AnscAllocateMemory(ulScratchPadSize);

        if ( !pScratchPad )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
    }

    /*
     * The finished message is the first protected with the just-negotiated algorithms, keys, and
     * secrets. Recipients of finished messages must verify that the contents are correct. Once a
     * side has sent its Finished message and received and validated the Finished message from its
     * peer, it may begin to send and receive application data over the connection.
     *
     *      struct {
     *          opaque verify_data[12];
     *      } Finished;
     */
    if ( pSecurityParams->MinorVersion == 0 )
    {
        /*
         * A finished message is always sent immediately after a change cipher specs message to
         * verify that the key exchange and authentication processes were successful. The finished
         * message is the first protected with the just-negotiated algorithms, keys, and secrets.
         * No acknowledgment of the finished message is required; parties may begin sending en-
         * crypted data immediately after sending the finished message. Recipients of finished
         * messages must verify that the contents are correct.
         *
         *      enum { client(0x434C4E54), server(0x53525652) } Sender;
         *
         *      struct {
         *          opaque md5_hash[16];
         *          opaque sha_hash[20];
         *      } Finished;
         *
         *      md5_hash            MD5(master_secret + pad2 +
         *                              MD5(handshake_messages + Sender + master_secret + pad1));
         *      sha_hash            SHA(master_secret + pad2 +
         *                              SHA(handshake_messages + Sender + master_secret + pad1));
         *      handshake_messages  All of the data from all handshake messages
         *                          up to but not including this message. This
         *                          is only data visible at the handshake layer
         *                          and does not include record layer headers.
         */
        PUCHAR                      pSslPad         = (PUCHAR)pScratchPad;
        ULONG                       ulSslPadSizeMd5 = (ULONG )48;
        ULONG                       ulSslPadSizeSha = (ULONG )40;
        UCHAR                       ucSslPad1Octet  = (UCHAR )0x36;
        UCHAR                       ucSslPad2Octet  = (UCHAR )0x5c;
        UCHAR                       senderClient[4] = {0x43, 0x4C, 0x4E, 0x54};
        //UCHAR                       senderServer[4] = {0x53, 0x52, 0x56, 0x52};
        
        /*
         * First Round Hashing:
         *
         *      MD5(handshake_messages + Sender + master_secret + pad_1));
         *      SHA(handshake_messages + Sender + master_secret + pad_1));
         */
        if ( TRUE )
        {
            for ( i = 0; i < ulSslPadSizeMd5; i++ )
            {
                pSslPad[i] = ucSslPad1Octet;
            }

            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, senderClient,                  4                     );
            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSecurityParams->MasterSecret, TLS_MASTER_SECRET_SIZE);
            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSslPad,                       ulSslPadSizeMd5       );

            ulHashDataSize = ANSC_MD5_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_MD5
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        tbs_data,
                        &ulHashDataSize
                    );

            pMyObject->MsgOffset -= 4;
            pMyObject->MsgOffset -= TLS_MASTER_SECRET_SIZE;
            pMyObject->MsgOffset -= ulSslPadSizeMd5;
        }

        if ( TRUE )
        {
            for ( i = 0; i < ulSslPadSizeSha; i++ )
            {
                pSslPad[i] = ucSslPad1Octet;
            }

            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, senderClient,                  4                     );
            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSecurityParams->MasterSecret, TLS_MASTER_SECRET_SIZE);
            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, pSslPad,                       ulSslPadSizeSha       );

            ulHashDataSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                        &ulHashDataSize
                    );

            pMyObject->MsgOffset -= 4;
            pMyObject->MsgOffset -= TLS_MASTER_SECRET_SIZE;
            pMyObject->MsgOffset -= ulSslPadSizeSha;
        }

        /*
         * Second Round Hashing:
         *
         *      MD5(master_secret + pad_2 + MD5(handshake_messages + Sender + master_secret + pad_1));
         *      SHA(master_secret + pad_2 + SHA(handshake_messages + Sender + master_secret + pad_1));
         */
        if ( TRUE )
        {
            ulTbhDataSize = TLS_MASTER_SECRET_SIZE + ulSslPadSizeMd5 + ANSC_MD5_OUTPUT_SIZE;
            pSslPad       = (PUCHAR)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE);

            AnscCopyMemory
                (
                    pScratchPad,
                    pSecurityParams->MasterSecret,
                    TLS_MASTER_SECRET_SIZE
                );

            for ( i = 0; i < ulSslPadSizeMd5; i++ )
            {
                pSslPad[i] = ucSslPad2Octet;
            }

            AnscCopyMemory
                (
                    (PVOID)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE + ulSslPadSizeMd5),
                    tbs_data,
                    ANSC_MD5_OUTPUT_SIZE
                );

            ulHashDataSize = ANSC_MD5_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_MD5
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pScratchPad,
                        ulTbhDataSize,
                        tbs_data,
                        &ulHashDataSize
                    );
        }

        if ( TRUE )
        {
            ulTbhDataSize = TLS_MASTER_SECRET_SIZE + ulSslPadSizeSha + ANSC_SHA1_OUTPUT_SIZE;
            pSslPad       = (PUCHAR)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE);

            AnscCopyMemory
                (
                    pScratchPad,
                    pSecurityParams->MasterSecret,
                    TLS_MASTER_SECRET_SIZE
                );

            for ( i = 0; i < ulSslPadSizeSha; i++ )
            {
                pSslPad[i] = ucSslPad2Octet;
            }

            AnscCopyMemory
                (
                    (PVOID)((ULONG)pScratchPad + TLS_MASTER_SECRET_SIZE + ulSslPadSizeSha),
                    (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                    ANSC_SHA1_OUTPUT_SIZE
                );

            ulHashDataSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pScratchPad,
                        ulTbhDataSize,
                        (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                        &ulHashDataSize
                    );
        }

        bValidHash =
            AnscEqualMemory
                (
                    TlsHsFinishedGetVerifyData(pTlsHsFinished),
                    tbs_data,
                    ANSC_MD5_OUTPUT_SIZE + ANSC_SHA1_OUTPUT_SIZE
                );
    }
    else
    {
        /*
         * The finished message is the first protected with the just-negotiated algorithms, keys,
         * and secrets. Recipients of finished messages must verify that the contents are correct.
         * Once a side has sent its Finished message and received and validated the Finished
         * message from its peer, it may begin to send and receive application data over the
         * connection.
         *
         *      struct {
         *          opaque verify_data[12];
         *      } Finished;
         *
         *      verify_data
         *          PRF(master_secret,
         *              finished_label,
         *              MD5(handshake_messages) + SHA-1(handshake_messages)) [0..11];
         *
         *      finished_label
         *          For Finished messages sent by the client, the string "client
         *          finished". For Finished messages sent by the server, the
         *          string "server finished".
         *
         *      handshake_messages
         *          All of the data from all handshake messages up to but not
         *          including this message. This is only data visible at the
         *          handshake layer and does not include record layer headers.
         *          This is the concatenation of all the Handshake structures as
         *          defined in 7.4 exchanged thus far.
         */
        if ( TRUE )
        {
            ulHashDataSize = ANSC_MD5_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_MD5
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        tbs_data,
                        &ulHashDataSize
                    );
        }

        if ( TRUE )
        {
            ulHashDataSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus   =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pMyObject->MsgBuffer,
                        pMyObject->MsgOffset,
                        (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                        &ulHashDataSize
                    );
        }

        returnStatus =
            pTlsCryptoProvider->Compute_PRF31
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pSecurityParams->MasterSecret,
                    TLS_MASTER_SECRET_SIZE,
                    "client finished",
                    tbs_data,
                    ANSC_MD5_OUTPUT_SIZE + ANSC_SHA1_OUTPUT_SIZE,
                    pScratchPad,
                    12
                );

        bValidHash =
            AnscEqualMemory
                (
                    TlsHsFinishedGetVerifyData(pTlsHsFinished),
                    pScratchPad,
                    12
                );
    }

    if ( !bValidHash )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decryptError
                );

        returnStatus = ANSC_STATUS_FAILURE;

        goto  EXIT1;
    }

    if ( TRUE )
    {
        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    /*
     * We couldn't save the 'finished' handshake message before we process it like what we did for
     * other messages because the received 'finished' message cannot be counted in the verification.
     * Note that the input buffer is not the handshake header, but the payload.
     */
    if ( TRUE )
    {
        returnStatus =
            pMyObject->SaveHandshakeMsg
                (
                    (ANSC_HANDLE)pMyObject,
                    (void*      )((ULONG)buffer - sizeof(TLS_HANDSHAKE_HEADER)),
                    ulSize + sizeof(TLS_HANDSHAKE_HEADER)
                );
    }

    if ( pConnParams->bQuickHandshake )
    {
        pMyObject->Establish((ANSC_HANDLE)pMyObject);
    }
    else
    {
        pMyObject->ChangeCipherW((ANSC_HANDLE)pMyObject);
        pMyObject->SendFinished ((ANSC_HANDLE)pMyObject);
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pScratchPad )
    {
        AnscFreeMemory(pScratchPad);
    }

    return  returnStatus;
}
