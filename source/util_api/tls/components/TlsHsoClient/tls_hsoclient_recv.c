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

    module:	tls_hsoclient_recv.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Hso Client Object.

        *   TlsHsoClientRecvHelloRequest
        *   TlsHsoClientRecvServerHello
        *   TlsHsoClientRecvCertificate
        *   TlsHsoClientRecvServerKeyExchange
        *   TlsHsoClientRecvCertRequest
        *   TlsHsoClientRecvServerHelloDone
        *   TlsHsoClientRecvFinished

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/02/03    initial revision.

**********************************************************************/


#include "tls_hsoclient_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientRecvHelloRequest
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
TlsHsoClientRecvHelloRequest
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    
    AnscTrace("TlsHsoClient: recv Hello Request message size = %lu!\n", ulSize);

    /*
     * Hello request is a simple notification that the client should begin the negotiation process
     * anew by sending a client hello message when convenient. This message will be ignored by the
     * client if the client is currently negotiating a session. This message may be ignored by the
     * client if it does not wish to renegotiate a session, or the client may, if it wishes,
     * respond with a no_renegotiation alert. Since handshake messages are intended to have trans-
     * mission precedence over application data, it is expected that the negotiation will begin
     * before no more than a few records are received from the client. If the server sends a hello
     * request but does not receive a client hello in response, it may close the connection with a
     * fatal alert.
     */
    AnscAcquireLock(&pMyObject->SyncLock);

    if ( pMyObject->HsClientState == TLS_HSC_STATE_idle )
    {
        returnStatus = pMyObject->SendClientHello((ANSC_HANDLE)pMyObject);
    }

    AnscReleaseLock(&pMyObject->SyncLock);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientRecvServerHello
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
TlsHsoClientRecvServerHello
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus        = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject           = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj      = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState       = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams     = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf           = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf           = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider  = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams         = (PTLS_CONNECTION_PARAMS       )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);
    PTLS_HS_SERVER_HELLO            pTlsHsServerHello   = (PTLS_HS_SERVER_HELLO         )buffer;
    ULONG                           ulTlsHsBodySize     = (ULONG                        )ulSize;
    PTLS_HS_SESSION_ID              pTlsHsSessionId     = (PTLS_HS_SESSION_ID           )NULL;
    BOOL                            bCipherSuiteMatched = (BOOL                         )FALSE;
    BOOL                            bCompressionMatched = (BOOL                         )FALSE;
    BOOL                            bValidServerHello   = (BOOL                         )TRUE;
    ULONG                           i                   = 0;

    AnscTrace("TlsHsoClient: recv Server Hello message size = %lu!\n", ulSize);

    /*
     * The server will send this message in response to a client hello message when it was able to
     * find an acceptable set of algorithms. If it cannot find such a match, it will respond with a
     * handshake failure alert.
     */
    if ( pMyObject->HsClientState != TLS_HSC_STATE_waitServerHello )
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

    TlsHsServerHelloValidate(pTlsHsServerHello, ulTlsHsBodySize, bValidServerHello);

    if ( !bValidServerHello )
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
     * The server will send this message in response to a client hello message when it was able to
     * find an acceptable set of algorithms. If it cannot find such a match, it will respond with a
     * handshake failure alert.
     *
     *      struct {
     *          ProtocolVersion     server_version;
     *          Random              random;
     *          SessionID           session_id;
     *          CipherSuite         cipher_suite;
     *          CompressionMethod   compression_method;
     *      } ServerHello;
     */
    if ( TlsHsServerHelloGetMajorVersion(pTlsHsServerHello) != 3 )
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
    else if ( (TlsHsServerHelloGetMinorVersion(pTlsHsServerHello) != 1) &&
              (TlsHsServerHelloGetMinorVersion(pTlsHsServerHello) != 0) )
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
        pSecurityParams->MinorVersion = TlsHsServerHelloGetMinorVersion(pTlsHsServerHello);
    }

    /*
     * If the ClientHello.session_id was non-empty, the server will look in its session cache for a
     * match. If a match is found and the server is willing to establish the new connection using
     * the specified session state, the server will respond with the same value as was supplied by
     * the client. This indicates a resumed session and dictates that the parties must proceed
     * directly to the finished messages. Otherwise this field will contain a different value
     * identifying the new session. The server may return an empty session_id to indicate that the
     * session will not be cached and therefore cannot be resumed. If a session is resumed, it must
     * be resumed using the same cipher suite it was originally negotiated with.
     */
    pTlsHsSessionId = TlsHsServerHelloGetSessionId(pTlsHsServerHello);

    if ( (pTlsHsSessionId->Length != 0                           ) &&
         (pTlsHsSessionId->Length == pSessionState->SessionIDSize) &&
         AnscEqualMemory
            (
                pTlsHsSessionId->Identifier,
                pSessionState->SessionID,
                pSessionState->SessionIDSize
            ) )
    {
        pTlsCbcIf->SetQuickHandshake(pTlsCbcIf->hOwnerContext, TRUE);
    }
    else
    {
        returnStatus =
            pTlsCbcIf->SetSessionID
                (
                    pTlsCbcIf->hOwnerContext,
                    pTlsHsSessionId->Identifier,
                    pTlsHsSessionId->Length
                );

        pTlsCbcIf->SetQuickHandshake(pTlsCbcIf->hOwnerContext, FALSE);
    }

    /*
     * The server will send this message in response to a client hello message when it was able to
     * find an acceptable set of algorithms. If it cannot find such a match, it will respond with a
     * handshake failure alert.
     *
     *      struct {
     *          ProtocolVersion     server_version;
     *          Random              random;
     *          SessionID           session_id;
     *          CipherSuite         cipher_suite;
     *          CompressionMethod   compression_method;
     *      } ServerHello;
     */
    AnscCopyMemory
        (
            pSecurityParams->ServerRandom,
            TlsHsServerHelloGetRandom(pTlsHsServerHello),
            TLS_OPAQUE_RANDOM_SIZE
        );

    /*
     * The single cipher suite selected by the server from the list in ClientHello.cipher_suites.
     * For resumed sessions this field is the value from the state of the session being resumed.
     */
    bCipherSuiteMatched = FALSE;

    for ( i = 0; i < pConnParams->CipherSuiteCount; i++ )
    {
        TlsHsServerHelloMatchCipherSuite(pTlsHsServerHello, pConnParams->CipherSuiteArray[i], bCipherSuiteMatched);

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
     * The single compression algorithm selected by the server from the list in
     * ClientHello.compression_methods. For resumed sessions this field is the value from the
     * resumed session state.
     */
    bCompressionMatched = FALSE;

    for ( i = 0; i < pConnParams->CompressionCount; i++ )
    {
        TlsHsServerHelloMatchCompression(pTlsHsServerHello, pConnParams->CompressionArray[i], bCompressionMatched);

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

    if ( pConnParams->bQuickHandshake )
    {
        pMyObject->Agree        ((ANSC_HANDLE)pMyObject, TRUE);
        pMyObject->ChangeCipherW((ANSC_HANDLE)pMyObject);

        pMyObject->HsClientState = TLS_HSC_STATE_waitFinished;
    }
    else
    {
        if ( pTlsCryptoProvider->GetCertConfigBit
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pSessionState->CipherSuite
                ) != 0 )
        {
            pMyObject->HsClientState = TLS_HSC_STATE_waitCertificate;
        }
        else
        {
            pMyObject->HsClientState = TLS_HSC_STATE_waitServerKeyExchange;
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientRecvCertificate
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
TlsHsoClientRecvCertificate
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_HS_CERTIFICATE             pTlsHsCertificate  = (PTLS_HS_CERTIFICATE          )buffer;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )ulSize;
    BOOL                            bValidCertificate  = (BOOL                         )TRUE;
    ULONG                           ulCertConfigBit    = (ULONG                        )0;
    ANSC_X509_CERTIFICATE_CHAIN*    pPeerCertChain     = (ANSC_X509_CERTIFICATE_CHAIN* )NULL;
    ANSC_X509_CERTIFICATE*          pPeerCert          = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X509_CERTIFICATE*          pCommonCACert      = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X509_KEY*                  pPeerKeyRsa        = (ANSC_X509_KEY*               )NULL;
    ANSC_X509_KEY*                  pPeerKeyDh         = (ANSC_X509_KEY*               )NULL;
    void*                           peer_cert_data     = (void*                        )NULL;
    ULONG                           peer_cert_size     = (ULONG                        )0;
    ULONG                           ulCertCount        = (ULONG                        )0;
    ULONG                           i                  = 0;

    AnscTrace("TlsHsoClient: recv Certificate message size = %lu!\n", ulSize);

    /*
     * The server must send a certificate whenever the agreed-upon key exchange method is not an
     * anonymous one. This message will always immediately follow the server hello message.
     */
    if ( (pMyObject->HsClientState != TLS_HSC_STATE_waitCertificate      ) &&
         (pMyObject->HsClientState != TLS_HSC_STATE_waitServerKeyExchange) )
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
        ulCertConfigBit =
            pTlsCryptoProvider->GetCertConfigBit
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
    if ( ulCertConfigBit != 0 )
    {
        pPeerCert = pPeerCertChain->Array[0];

        switch ( pSessionState->KeAlgorithm )
        {
            case    TLS_KE_ALGORITHM_RSA :

                    if ( (pPeerCert->Signing                                         != ANSC_CERT_signing_rsa) ||
                         (pPeerCert->KeyType                                         != ANSC_CERT_keyType_rsa) ||
                         ((pPeerCert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment) == 0                    ) )
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
                        pPeerKeyRsa =
                            (ANSC_X509_KEY*)AnscX509CertGetPublicKey
                                (
                                    pPeerCert->CertData,
                                    pPeerCert->CertSize
                                );

                        if ( !pPeerKeyRsa )
                        {
                            returnStatus =
                                pTlsCbcIf->GenerateAlert
                                    (
                                        pTlsCbcIf->hOwnerContext,
                                        TLS_ALERT_LEVEL_fatal,
                                        TLS_ALERT_DESCR_internalError
                                    );

                            returnStatus = ANSC_STATUS_FAILURE;

                            goto  EXIT2;
                        }
                        else
                        {
                            pSecurityParams->RsaKeyPub = pPeerKeyRsa;

                            returnStatus =
                                pTlsCryptoProvider->DecapRSAPublicKey
                                    (
                                        (ANSC_HANDLE)pTlsCryptoProvider,
                                        pPeerKeyRsa,
                                        &pSecurityParams->RsaModulus,
                                        &pSecurityParams->RsaExponent
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

                            goto  EXIT2;
                        }
                    }

                    break;

            case    TLS_KE_ALGORITHM_RSA_EXPORT :

                    if ( (pPeerCert->Signing == ANSC_CERT_signing_rsa) &&
                         (pPeerCert->KeyType == ANSC_CERT_keyType_rsa) )
                    {
                        if ( pPeerCert->KeyBits > 512 )
                        {
                            if ( (pPeerCert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) == 0 )
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
                        }
                        else
                        {
                            if ( ((pPeerCert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) == 0) &&
                                 ((pPeerCert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment ) == 0) )
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
                            else if ( pPeerCert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment )
                            {
                                pPeerKeyRsa =
                                    (ANSC_X509_KEY*)AnscX509CertGetPublicKey
                                        (
                                            pPeerCert->CertData,
                                            pPeerCert->CertSize
                                        );

                                if ( !pPeerKeyRsa )
                                {
                                    returnStatus =
                                        pTlsCbcIf->GenerateAlert
                                            (
                                                pTlsCbcIf->hOwnerContext,
                                                TLS_ALERT_LEVEL_fatal,
                                                TLS_ALERT_DESCR_internalError
                                            );

                                    returnStatus = ANSC_STATUS_FAILURE;

                                    goto  EXIT2;
                                }
                                else
                                {
                                    pSecurityParams->RsaKeyPub = pPeerKeyRsa;

                                    returnStatus =
                                        pTlsCryptoProvider->DecapRSAPublicKey
                                            (
                                                (ANSC_HANDLE)pTlsCryptoProvider,
                                                pPeerKeyRsa,
                                                &pSecurityParams->RsaModulus,
                                                &pSecurityParams->RsaExponent
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

                                    goto  EXIT2;
                                }
                            }
                        }
                    }
                    else
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

            case    TLS_KE_ALGORITHM_DHE_DSS        :
            case    TLS_KE_ALGORITHM_DHE_DSS_EXPORT :

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

            case    TLS_KE_ALGORITHM_DHE_RSA        :
            case    TLS_KE_ALGORITHM_DHE_RSA_EXPORT :

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

            case    TLS_KE_ALGORITHM_DH_DSS :

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
                    else
                    {
                        pPeerKeyDh =
                            (ANSC_X509_KEY*)AnscX509CertGetPublicKey
                                (
                                    pPeerCert->CertData,
                                    pPeerCert->CertSize
                                );

                        if ( !pPeerKeyDh )
                        {
                            returnStatus =
                                pTlsCbcIf->GenerateAlert
                                    (
                                        pTlsCbcIf->hOwnerContext,
                                        TLS_ALERT_LEVEL_fatal,
                                        TLS_ALERT_DESCR_internalError
                                    );

                        returnStatus = ANSC_STATUS_FAILURE;

                            goto  EXIT2;
                        }
                        else
                        {
                            pSecurityParams->DHKeyPubS = pPeerKeyDh;

                            returnStatus =
                                pTlsCryptoProvider->DecapDHPublicKey
                                    (
                                        (ANSC_HANDLE)pTlsCryptoProvider,
                                        pPeerKeyDh,
                                        &pSecurityParams->DHPrime,
                                        &pSecurityParams->DHGenerator,
                                        &pSecurityParams->DHPublicS
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

                            goto  EXIT2;
                        }
                    }

                    break;

            case    TLS_KE_ALGORITHM_DH_RSA :

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
                    else
                    {
                        pPeerKeyDh =
                            (ANSC_X509_KEY*)AnscX509CertGetPublicKey
                                (
                                    pPeerCert->CertData,
                                    pPeerCert->CertSize
                                );

                        if ( !pPeerKeyDh )
                        {
                            returnStatus =
                                pTlsCbcIf->GenerateAlert
                                    (
                                        pTlsCbcIf->hOwnerContext,
                                        TLS_ALERT_LEVEL_fatal,
                                        TLS_ALERT_DESCR_internalError
                                    );

                            returnStatus = ANSC_STATUS_FAILURE;

                            goto  EXIT2;
                        }
                        else
                        {
                            pSecurityParams->DHKeyPubS = pPeerKeyDh;

                            returnStatus =
                                pTlsCryptoProvider->DecapDHPublicKey
                                    (
                                        (ANSC_HANDLE)pTlsCryptoProvider,
                                        pPeerKeyDh,
                                        &pSecurityParams->DHPrime,
                                        &pSecurityParams->DHGenerator,
                                        &pSecurityParams->DHPublicS
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

                            goto  EXIT2;
                        }
                    }

                    break;

            case    TLS_KE_ALGORITHM_DH_ANONYMOUS :

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
     * The server key exchange message is sent by the server only when the server certificate
     * message (if sent) does not contain enough data to allow the client to exchange a premaster
     * secret. This is true for the following key exchange methods:
     *
     *      RSA_EXPORT (if the public key in the server certificate is longer than
     *                  512 bits)
     *      DHE_DSS
     *      DHE_DSS_EXPORT
     *      DHE_RSA
     *      DHE_RSA_EXPORT
     *      DH_anon
     *
     * It is not legal to send the server key exchange message for the following key exchange
     * methods:
     *
     *      RSA
     *      RSA_EXPORT (when the public key in the server certificate is less than or
     *                  equal to 512 bits in length)
     *      DH_DSS
     *      DH_RSA
     */
    if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS       ) ||
         (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS_EXPORT) ||
         (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA       ) ||
         (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA_EXPORT) ||
         (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DH_ANONYMOUS  ) )
    {
        pMyObject->HsClientState = TLS_HSC_STATE_waitServerKeyExchange;
    }
    else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA   ) ||
              (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DH_DSS) ||
              (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DH_RSA) )
    {
        pMyObject->HsClientState = TLS_HSC_STATE_waitCertRequest;
    }
    else if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT )
    {
        pPeerCert = pPeerCertChain->Array[0];

        if ( pPeerCert->KeyBits > 512 )
        {
            pMyObject->HsClientState = TLS_HSC_STATE_waitServerKeyExchange;
        }
        else if ( !(pPeerCert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment) )
        {
            pMyObject->HsClientState = TLS_HSC_STATE_waitServerKeyExchange;
        }
        else
        {
            pMyObject->HsClientState = TLS_HSC_STATE_waitCertRequest;
        }
    }

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
        TlsHsoClientRecvServerKeyExchange
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
TlsHsoClientRecvServerKeyExchange
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_HS_SERVER_KEY_EXCHANGE     pTlsHsServerKe     = (PTLS_HS_SERVER_KEY_EXCHANGE  )buffer;
    PTLS_HS_PARAMETER               pTlsHsParamSig     = (PTLS_HS_PARAMETER            )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )ulSize;
    BOOL                            bValidServerKe     = (BOOL                         )TRUE;
    BOOL                            bSignatureNeeded   = (BOOL                         )FALSE;
    BOOL                            bSignatureValid    = (BOOL                         )FALSE;
    ANSC_X509_CERTIFICATE*          pPeerCert          = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X509_KEY*                  pPeerPublicKey     = (ANSC_X509_KEY*               )NULL;
    void*                           pHashData          = (void*                        )NULL;
    ULONG                           ulTbhSize          = (ULONG                        )0;
    ULONG                           ulHashResultSize   = (ULONG                        )0;
    ULONG                           tbs_data_size      = (ULONG                        )0;
    UCHAR                           tbs_data[36];

    AnscTrace("TlsHsoClient: recv Server Key Exchange message size = %lu!\n", ulSize);

    /*
     * This message will be sent immediately after the server certificate message (or the server
     * hello message, if this is an anonymous negotiation).
     */
    if ( pMyObject->HsClientState != TLS_HSC_STATE_waitServerKeyExchange )
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

    if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT )
    {
        TlsHsServerKeValidate(pTlsHsServerKe, ulTlsHsBodySize, TLS_HS_KE_ALGORITHM_rsa, TLS_HS_SIG_ALGORITHM_rsa, bValidServerKe);
    }
    else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS       ) ||
              (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS_EXPORT) )
    {
        TlsHsServerKeValidate(pTlsHsServerKe, ulTlsHsBodySize, TLS_HS_KE_ALGORITHM_diffieHellman, TLS_HS_SIG_ALGORITHM_dsa, bValidServerKe);
    }
    else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA       ) ||
              (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA_EXPORT) )
    {
        TlsHsServerKeValidate(pTlsHsServerKe, ulTlsHsBodySize, TLS_HS_KE_ALGORITHM_diffieHellman, TLS_HS_SIG_ALGORITHM_rsa, bValidServerKe);
    }
    else
    {
        TlsHsServerKeValidate(pTlsHsServerKe, ulTlsHsBodySize, TLS_HS_KE_ALGORITHM_diffieHellman, TLS_HS_SIG_ALGORITHM_anonymous, bValidServerKe);
    }

    if ( !bValidServerKe )
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
     * For non-anonymous key exchange, server must generate a hash of the corresponding params
     * value, with the signature appropriate to that hash applied.
     *
     *      select (SignatureAlgorithm)
     *      {
     *          case anonymous: struct { };
     *          case rsa:
     *              digitally-signed struct {
     *                  opaque md5_hash[16];
     *                  opaque sha_hash[20];
     *              };
     *          case dsa:
     *              digitally-signed struct {
     *                  opaque sha_hash[20];
     *              };
     *      } Signature;
     */
    if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT )
    {
        bSignatureNeeded = TRUE;

        TlsHsServerKeGetSignature1(pTlsHsServerKe, TLS_HS_KE_ALGORITHM_rsa, pTlsHsParamSig);
    }
    else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS       ) ||
              (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS_EXPORT) ||
              (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA       ) ||
              (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA_EXPORT) )
    {
        bSignatureNeeded = TRUE;

        TlsHsServerKeGetSignature1(pTlsHsServerKe, TLS_HS_KE_ALGORITHM_diffieHellman, pTlsHsParamSig);
    }
    else
    {
        bSignatureNeeded = FALSE;
    }

    if ( bSignatureNeeded )
    {
        ulTbhSize  = (ULONG)pTlsHsParamSig - (ULONG)pTlsHsServerKe;
        ulTbhSize += TLS_OPAQUE_RANDOM_SIZE * 2;
        pHashData  = AnscAllocateMemory(ulTbhSize);

        if ( !pHashData )
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

        AnscCopyMemory
            (
                pHashData,
                pSecurityParams->ClientRandom,
                TLS_OPAQUE_RANDOM_SIZE
            );

        AnscCopyMemory
            (
                (PVOID)((ULONG)pHashData + TLS_OPAQUE_RANDOM_SIZE),
                pSecurityParams->ServerRandom,
                TLS_OPAQUE_RANDOM_SIZE
            );

        AnscCopyMemory
            (
                (PVOID)((ULONG)pHashData + TLS_OPAQUE_RANDOM_SIZE * 2),
                (PVOID)pTlsHsServerKe,
                ulTbhSize - TLS_OPAQUE_RANDOM_SIZE * 2
            );

        if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT    ) ||
             (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA       ) ||
             (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA_EXPORT) )
        {
            ulHashResultSize = ANSC_MD5_OUTPUT_SIZE;
            returnStatus     =
                pTlsCryptoProvider->Hash_MD5
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pHashData,
                        ulTbhSize,
                        tbs_data,
                        &ulHashResultSize
                    );

            ulHashResultSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus     =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pHashData,
                        ulTbhSize,
                        (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                        &ulHashResultSize
                    );

            tbs_data_size = ANSC_MD5_OUTPUT_SIZE + ANSC_SHA1_OUTPUT_SIZE;
        }
        else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS       ) ||
                  (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS_EXPORT) )
        {
            ulHashResultSize = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus     =
                pTlsCryptoProvider->Hash_SHA
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pHashData,
                        ulTbhSize,
                        tbs_data,
                        &ulHashResultSize
                    );

            tbs_data_size = ANSC_SHA1_OUTPUT_SIZE;
        }
    }

    if ( bSignatureNeeded )
    {
        pPeerCert = pSecurityParams->PeerCertChain->Array[0];

        if ( !pPeerCert )
        {
            AnscFreeMemory(pHashData);

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
            pPeerPublicKey =
                AnscX509CertGetPublicKey
                    (
                        pPeerCert->CertData,
                        pPeerCert->CertSize
                    );

            if ( !pPeerPublicKey )
            {
                AnscFreeMemory(pHashData);

                returnStatus =
                    pTlsCbcIf->GenerateAlert
                        (
                            pTlsCbcIf->hOwnerContext,
                            TLS_ALERT_LEVEL_fatal,
                            TLS_ALERT_DESCR_internalError
                        );

                return  ANSC_STATUS_FAILURE;
            }
        }

        bSignatureValid =
            pTlsCryptoProvider->Verify
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pPeerPublicKey->KeyType,
                    pPeerPublicKey,
                    tbs_data,
                    tbs_data_size,
                    TlsHsParamGetOpaque(pTlsHsParamSig),
                    TlsHsParamGetLength(pTlsHsParamSig)
                );

        AnscFreeMemory(pHashData);

        if ( !bSignatureValid )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_internalError
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto EXIT;
        }
    }

    /*
     * According to current US export law, RSA moduli larger than 512 bits may not be used for key
     * exchange in software exported from the US. With this message, the larger RSA keys encoded in
     * certificates may be used to sign temporary shorter RSA keys for the RSA_EXPORT key exchange
     * method.
     *
     *      enum { rsa, diffie_hellman } KeyExchangeAlgorithm;
     *
     *      struct {
     *          opaque rsa_modulus<1..2^16-1>;
     *          opaque rsa_exponent<1..2^16-1>;
     *      } ServerRSAParams;
     *
     *      struct {
     *          opaque dh_p<1..2^16-1>;
     *          opaque dh_g<1..2^16-1>;
     *          opaque dh_Ys<1..2^16-1>;
     *      } ServerDHParams;
     */
    if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT )
    {
        PUCHAR                      rsa_modulus_data  = NULL;
        ULONG                       rsa_modulus_size  = 0;
        PUCHAR                      rsa_exponent_data = NULL;
        ULONG                       rsa_exponent_size = 0;

        TlsHsServerKeGetRSAModulus2 (pTlsHsServerKe, rsa_modulus_data,  rsa_modulus_size );
        TlsHsServerKeGetRSAExponent2(pTlsHsServerKe, rsa_exponent_data, rsa_exponent_size);

        pSecurityParams->RsaModulus  = (ANSC_ASN1_INT*)AnscAsn1AllocInt(rsa_modulus_data,  rsa_modulus_size,  FALSE);
        pSecurityParams->RsaExponent = (ANSC_ASN1_INT*)AnscAsn1AllocInt(rsa_exponent_data, rsa_exponent_size, FALSE);

        if ( !pSecurityParams->RsaModulus || !pSecurityParams->RsaExponent )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_internalError
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto EXIT;
        }
        else
        {
            pSecurityParams->RsaKeyPub =
                (ANSC_X509_KEY*)pTlsCryptoProvider->EncapRSAPublicKey
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pSecurityParams->RsaModulus,
                        pSecurityParams->RsaExponent
                    );
        }

        if ( !pSecurityParams->RsaKeyPub )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_internalError
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto EXIT;
        }
    }
    else
    {
        PUCHAR                      dh_prime_data     = NULL;
        ULONG                       dh_prime_size     = 0;
        PUCHAR                      dh_generator_data = NULL;
        ULONG                       dh_generator_size = 0;
        PUCHAR                      dh_public_data    = NULL;
        ULONG                       dh_public_size    = 0;

        TlsHsServerKeGetDHPrime2    (pTlsHsServerKe, dh_prime_data,     dh_prime_size    );
        TlsHsServerKeGetDHGenerator2(pTlsHsServerKe, dh_generator_data, dh_generator_size);
        TlsHsServerKeGetDHPublic2   (pTlsHsServerKe, dh_public_data,    dh_public_size   );

        pSecurityParams->DHPrime     = (ANSC_ASN1_INT*)AnscAsn1AllocInt(dh_prime_data,     dh_prime_size,     FALSE);
        pSecurityParams->DHGenerator = (ANSC_ASN1_INT*)AnscAsn1AllocInt(dh_generator_data, dh_generator_size, FALSE);
        pSecurityParams->DHPublicS   = (ANSC_ASN1_INT*)AnscAsn1AllocInt(dh_public_data,    dh_public_size,    FALSE);

        if ( !pSecurityParams->DHPrime || !pSecurityParams->DHGenerator || !pSecurityParams->DHPublicS )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_internalError
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto EXIT;
        }
        else
        {
            pSecurityParams->DHKeyPubS =
                (ANSC_X509_KEY*)pTlsCryptoProvider->EncapDHPublicKey
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pSecurityParams->DHPrime,
                        pSecurityParams->DHGenerator,
                        pSecurityParams->DHPublicS
                    );
        }

        if ( !pSecurityParams->DHKeyPubS )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_internalError
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto EXIT;
        }
    }

    if ( TRUE )
    {
        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    pMyObject->HsClientState = TLS_HSC_STATE_waitCertRequest;

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:

    if( pPeerPublicKey != NULL)
    {
       AnscFreeX509Key(pPeerPublicKey);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientRecvCertRequest
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
TlsHsoClientRecvCertRequest
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    ANSC_X509_CERTIFICATE_CHAIN*    pX509CertChain     = (ANSC_X509_CERTIFICATE_CHAIN* )NULL;
    ANSC_X509_PROFILE*              pX509Profile       = (ANSC_X509_PROFILE*           )NULL;
    BOOL                            bCertChainFound    = (BOOL                         )FALSE;
    PTLS_HS_CERT_REQUEST            pTlsHsCertRequest  = (PTLS_HS_CERT_REQUEST         )buffer;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )ulSize;
    BOOL                            bValidCertRequest  = (BOOL                         )TRUE;
    ULONG                           ulClientCertType   = (ULONG                        )0;
    ANSC_X501_NAME*                 pX501CAName        = (ANSC_X501_NAME*              )NULL;
    ULONG                           ulRawDnSize        = (ULONG                        )0;
    void*                           pRawDnData         = (void*                        )NULL;
    ULONG                           ulCertTypeCount    = (ULONG                        )0;
    ULONG                           ulCADnCount        = (ULONG                        )0;
    ULONG                           i                  = 0;
    ULONG                           j                  = 0;

    AnscTrace("TlsHsoClient: recv Certificate Request message size = %lu!\n", ulSize);

    /*
     * A non-anonymous server can optionally request a certificate from the client, if appropriate
     * for the selected cipher suite. This message, if sent, will immediately follow the Server Key
     * Exchange message (if it is sent; otherwise, the Server Certificate message). It is a fatal
     * handshake_failure alert for an anonymous server to request client identification.
     */
    if ( pMyObject->HsClientState != TLS_HSC_STATE_waitCertRequest )
    {
        pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        return  ANSC_STATUS_FAILURE;
    }
    else if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DH_ANONYMOUS )
    {
        pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_handshakeFailure
                );

        return  ANSC_STATUS_FAILURE;
    }

    TlsHsCertReqValidate(pTlsHsCertRequest, ulTlsHsBodySize, bValidCertRequest);

    if ( !bValidCertRequest )
    {
        pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_decodeError
                );

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pSessionState->bCertRequested = TRUE;
    }

    /*
     * A non-anonymous server can optionally request a certificate from the client, if appropriate
     * for the selected cipher suite. This message, if sent, will immediately follow the Server Key
     * Exchange message (if it is sent; otherwise, the Server Certificate message).
     *
     *      enum {
     *          rsa_sign(1), dss_sign(2), rsa_fixed_dh(3), dss_fixed_dh(4), (255)
     *      } ClientCertificateType;
     *
     *      opaque DistinguishedName<1..2^16-1>;
     *
     *      struct {
     *          ClientCertificateType   certificate_types<1..2^8-1>;
     *          DistinguishedName       certificate_authorities<3..2^16-1>;
     *      } CertificateRequest;
     */
    ulCertTypeCount = TlsHsCertReqGetTypeCount(pTlsHsCertRequest);
    TlsHsCertReqGetDnCount(pTlsHsCertRequest, ulCADnCount);

    pX501CAName = (ANSC_X501_NAME*)AnscAllocateMemory(1024);

    if ( !pX501CAName )
    {
        pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_internalError
                );

        return  ANSC_STATUS_FAILURE;
    }

    for ( i = 0; i < ulCertTypeCount; i++ )
    {
        ulClientCertType = TlsHsCertReqGetType(pTlsHsCertRequest, i);

        if ( ulCADnCount == 0 )
        {
            pX509CertChain =
                (ANSC_X509_CERTIFICATE_CHAIN*)pTlsMecIf->GetCertChain3
                    (
                        pTlsMecIf->hOwnerContext,
                        ulClientCertType,
                        (ANSC_HANDLE)NULL,
                        pSessionState->CertID
                    );
        }
        else
        {
            for ( j = 0; j < ulCADnCount; j++ )
            {
                if ( TRUE )
                {
                    TlsHsCertReqGetDn2(pTlsHsCertRequest, pRawDnData, ulRawDnSize, j);

                    if ( ulRawDnSize > (1024 - sizeof(ANSC_X501_NAME)) )
                    {
                        AnscFreeX501Name(pX501CAName);

                        pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        return  ANSC_STATUS_FAILURE;
                    }

                    pX501CAName->Length = ulRawDnSize;

                    AnscCopyMemory
                        (
                            pX501CAName->Data,
                            pRawDnData,
                            ulRawDnSize
                        );
                }

                pX509CertChain =
                    (ANSC_X509_CERTIFICATE_CHAIN*)pTlsMecIf->GetCertChain3
                        (
                            pTlsMecIf->hOwnerContext,
                            ulClientCertType,
                            (ANSC_HANDLE)pX501CAName,
                            pSessionState->CertID
                        );

                if ( pX509CertChain )
                {
                    bCertChainFound = TRUE;

                    break;
                }
            }
        }

        if ( pX509CertChain )
        {
            bCertChainFound = TRUE;

            break;
        }
    }

    AnscFreeX501Name(pX501CAName);

    if ( !bCertChainFound )
    {
        pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_certificateUnknown
                );

        return  ANSC_STATUS_FAILURE;
    }
    else
    {
        pX509Profile =
            (ANSC_X509_PROFILE*)pTlsMecIf->GetProfile
                (
                    pTlsMecIf->hOwnerContext,
                    pSessionState->CertID
                );
    }

    if ( !pX509Profile )
    {
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
        pSecurityParams->MyProfile = pX509Profile;
    }

    if ( TRUE )
    {
        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    pMyObject->HsClientState = TLS_HSC_STATE_waitServerHelloDone;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientRecvServerHelloDone
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
TlsHsoClientRecvServerHelloDone
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(buffer);
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    
    AnscTrace("TlsHsoClient: recv Server Hello Done message size = %lu!\n", ulSize);

    /*
     * This message means that the server is done sending messages to support the key exchange, and
     * the client can proceed with its phase of the key exchange. Upon receipt of the server hello
     * done message the client should verify that the server provided a valid certificate if
     * required and check that the server hello parameters are acceptable.
     *
     *      struct { } ServerHelloDone;
     */
    if ( (pMyObject->HsClientState != TLS_HSC_STATE_waitCertificate      ) &&
         (pMyObject->HsClientState != TLS_HSC_STATE_waitServerKeyExchange) &&
         (pMyObject->HsClientState != TLS_HSC_STATE_waitCertRequest      ) &&
         (pMyObject->HsClientState != TLS_HSC_STATE_waitServerHelloDone  ) )
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
        pMyObject->HsClientState = TLS_HSC_STATE_waitServerHelloDone;
    }

    returnStatus = pMyObject->SendCertificate((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientRecvFinished
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
TlsHsoClientRecvFinished
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
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

    AnscTrace("TlsHsoClient: recv Finished message size = %lu!\n", ulSize);

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

        return  ANSC_STATUS_FAILURE;
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

        return  ANSC_STATUS_FAILURE;
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
        UCHAR                       senderServer[4] = {0x53, 0x52, 0x56, 0x52};

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

            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, senderServer,                  4                     );
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

            pMyObject->SaveHandshakeMsg((ANSC_HANDLE)pMyObject, senderServer,                  4                     );
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
                    "server finished",
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

    if ( !pConnParams->bQuickHandshake )
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
