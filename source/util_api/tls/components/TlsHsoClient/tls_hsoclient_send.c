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

    module:	tls_hsoclient_send.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Hso Client Object.

        *   TlsHsoClientSendClientHello
        *   TlsHsoClientSendCertificate
        *   TlsHsoClientSendClientKeyExchange
        *   TlsHsoClientSendCertVerify
        *   TlsHsoClientSendFinished

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
        TlsHsoClientSendClientHello
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to construct an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoClientSendClientHello
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams        = (PTLS_CONNECTION_PARAMS       )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )1024;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_CLIENT_HELLO            pTlsHsClientHello  = (PTLS_HS_CLIENT_HELLO         )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    ULONG                           i                  = 0;

    /*
     * When a client first connects to a server it is required to send the client hello as its
     * first message. The client can also send a client hello in response to a hello request or on
     * its own initiative in order to renegotiate the security parameters in an existing connection.
     */
    if ( pMyObject->HsClientState != TLS_HSC_STATE_idle )
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;

        goto  EXIT1;
    }
    else
    {
        ulTlsMsgSize  = 1024;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pTlsHsHeader      = (PTLS_HANDSHAKE_HEADER)pTlsMsgBuffer;
            pTlsHsClientHello = (PTLS_HS_CLIENT_HELLO )TlsHandshakeGetBody(pTlsHsHeader);
        }
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
    returnStatus =
        pTlsCryptoProvider->GetRandomBytes
            (
                (ANSC_HANDLE)pTlsCryptoProvider,
                AnscGetTickInSeconds(),
                pSecurityParams->ClientRandom,
                TLS_OPAQUE_RANDOM_SIZE
            );

    TlsHsClientHelloSetMajorVersion(pTlsHsClientHello, pSecurityParams->MajorVersion);
    TlsHsClientHelloSetMinorVersion(pTlsHsClientHello, pSecurityParams->MinorVersion);
    TlsHsClientHelloSetRandom      (pTlsHsClientHello, pSecurityParams->ClientRandom);
    TlsHsClientHelloSetSessionId   (pTlsHsClientHello, pSessionState->SessionIDSize, pSessionState->SessionID);

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
    TlsHsClientHelloSetCipherSuiteCount(pTlsHsClientHello, pConnParams->CipherSuiteCount);

    for ( i = 0; i < pConnParams->CipherSuiteCount; i++ )
    {
        TlsHsClientHelloSetCipherSuite(pTlsHsClientHello, pConnParams->CipherSuiteArray[i], i);
    }

    /*
     * The client hello includes a list of compression algorithms supported by the client, ordered
     * according to the client's preference.
     *
     *      enum { null(0), (255) } CompressionMethod;
     */
    TlsHsClientHelloSetCompressionCount(pTlsHsClientHello, pConnParams->CompressionCount);

    for ( i = 0; i < pConnParams->CompressionCount; i++ )
    {
        TlsHsClientHelloSetCompression(pTlsHsClientHello, pConnParams->CompressionArray[i], i);
    }

     if ( TRUE )
    {
        ulTlsHsBodySize = TlsHsClientHelloGetBodySize(pTlsHsClientHello);

        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_clientHello);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize               );
    }

    AnscTrace("TlsHsoClient: send Client Hello message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

    returnStatus =
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                pTlsHsHeader,
                TlsHandshakeGetSize(pTlsHsHeader)
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT2;
    }
    else
    {
        pMyObject->HsClientState = TLS_HSC_STATE_sentClientHello;
        pMyObject->HsClientState = TLS_HSC_STATE_waitServerHello;

        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);

        if ( TRUE )
        {
            returnStatus =
                pMyObject->SaveHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        pTlsHsHeader,
                        TlsHandshakeGetSize(pTlsHsHeader)
                    );
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;

    goto  EXIT2;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pTlsHsHeader )
    {
        AnscFreeMemory(pTlsHsHeader);
    }

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientSendCertificate
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to construct an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoClientSendCertificate
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    ANSC_X509_CERTIFICATE_CHAIN*    pX509CertChain     = (ANSC_X509_CERTIFICATE_CHAIN* )NULL;
    ULONG                           ulCertChainSize    = (ULONG                        )0;
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )1024;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_CERTIFICATE             pTlsHsCertificate  = (PTLS_HS_CERTIFICATE          )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    ULONG                           i                  = 0;

    /*
     * This is the first message the client can send after receiving a server hello done message.
     * This message is only sent if the server requests a certificate. If no suitable certificate
     * is available, the client should send a certificate message containing no certificates. If
     * client authentication is required by the server for the handshake to continue, it may res-
     * pond with a fatal handshake failure alert.
     */
    if ( pMyObject->HsClientState != TLS_HSC_STATE_waitServerHelloDone )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( !pSessionState->bCertRequested )
    {
        returnStatus = pMyObject->SendClientKeyExchange((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }
    else
    {
        pX509CertChain =
            (ANSC_X509_CERTIFICATE_CHAIN*)pTlsMecIf->GetCertChain2
                (
                    pTlsMecIf->hOwnerContext,
                    pSessionState->CertID
                );
    }

    if ( !pX509CertChain )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_internalError
                );

        return  returnStatus;
    }
    else
    {
        ulCertChainSize = 0;

        for ( i = 0; i < pX509CertChain->Depth; i++ )
        {
            ulCertChainSize += pX509CertChain->Array[i]->CertSize;
        }
    }

    if ( TRUE )
    {
        ulTlsMsgSize  = 1024 + ulCertChainSize;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pTlsHsHeader      = (PTLS_HANDSHAKE_HEADER)pTlsMsgBuffer;
            pTlsHsCertificate = (PTLS_HS_CERTIFICATE  )TlsHandshakeGetBody(pTlsHsHeader);
        }
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
    TlsHsCertSetLength(pTlsHsCertificate, 0);

    for ( i = 0; i < pX509CertChain->Depth; i++ )
    {
        TlsHsCertAddCert2
            (
                pTlsHsCertificate,
                pX509CertChain->Array[i]->CertData,
                pX509CertChain->Array[i]->CertSize
            );
    }


    if ( TRUE )
    {
        ulTlsHsBodySize = TlsHsCertGetBodySize(pTlsHsCertificate);

        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_certificate);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize               );
    }

    AnscTrace("TlsHsoClient: send Certificate message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

    returnStatus =
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                pTlsHsHeader,
                TlsHandshakeGetSize(pTlsHsHeader)
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        pMyObject->HsClientState = TLS_HSC_STATE_sentCertificate;

        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);

        if ( TRUE )
        {
            returnStatus =
                pMyObject->SaveHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        pTlsHsHeader,
                        TlsHandshakeGetSize(pTlsHsHeader)
                    );
        }

        if ( pTlsHsHeader )
        {
            AnscFreeMemory(pTlsHsHeader);

            pTlsHsHeader = NULL;
        }
    }

    returnStatus = pMyObject->SendClientKeyExchange((ANSC_HANDLE)pMyObject);

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pTlsHsHeader )
    {
        AnscFreeMemory(pTlsHsHeader);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientSendClientKeyExchange
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to construct an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoClientSendClientKeyExchange
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    ANSC_X509_KEY*                  pMyPublicKey       = (ANSC_X509_KEY*               )NULL;
    ANSC_X509_KEY*                  pMyPrivateKey      = (ANSC_X509_KEY*               )NULL;
    ANSC_ASN1_INT*                  pMyDHPrime         = (ANSC_ASN1_INT*               )NULL;
    ANSC_ASN1_INT*                  pMyDHGenerator     = (ANSC_ASN1_INT*               )NULL;
    ANSC_ASN1_INT*                  pMyDHPublic        = (ANSC_ASN1_INT*               )NULL;
    BOOL                            bEmptyKeMsg        = (BOOL                         )FALSE;
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )0;
    PVOID                           pScratchPad        = (PVOID                        )NULL;
    ULONG                           ulScratchPadSize   = (ULONG                        )0;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_CLIENT_KEY_EXCHANGE     pTlsHsClientKe     = (PTLS_HS_CLIENT_KEY_EXCHANGE  )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;

    /*
     * This message is always sent by the client. It will immediately follow the client certificate
     * message, if it is sent. Otherwise it will be the first message sent by the client after it
     * receives the server hello done message.
     */
    if ( (pMyObject->HsClientState != TLS_HSC_STATE_waitServerHelloDone) &&
         (pMyObject->HsClientState != TLS_HSC_STATE_sentCertificate    ) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulTlsMsgSize  = 4096;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pScratchPad      = (PVOID                      )((ULONG)pTlsMsgBuffer + 2048);
            ulScratchPadSize = (ULONG                      )2048;
            pTlsHsHeader     = (PTLS_HANDSHAKE_HEADER      )pTlsMsgBuffer;
            pTlsHsClientKe   = (PTLS_HS_CLIENT_KEY_EXCHANGE)TlsHandshakeGetBody(pTlsHsHeader);
        }
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
    if ( pSessionState->bCertRequested )
    {
        if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DH_DSS) ||
             (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DH_RSA) )
        {
            pMyPublicKey  = pSecurityParams->MyProfile->PubKey;
            pMyPrivateKey = pSecurityParams->MyProfile->PrvKey;

            if ( !pMyPublicKey || !pMyPrivateKey )
            {
                returnStatus =
                    pTlsCbcIf->GenerateAlert
                        (
                            pTlsCbcIf->hOwnerContext,
                            TLS_ALERT_LEVEL_fatal,
                            TLS_ALERT_DESCR_internalError
                        );

                goto  EXIT1;
            }
            else if ( pMyPublicKey->KeyType != ANSC_CERT_keyType_diffieHellman )
            {
                bEmptyKeMsg = FALSE;
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

                goto  EXIT1;
            }
            else
            {
                returnStatus =
                    pTlsCryptoProvider->DecapDHPublicKey
                        (
                            (ANSC_HANDLE)pTlsCryptoProvider,
                            pMyPublicKey,
                            &pMyDHPrime,
                            &pMyDHGenerator,
                            &pMyDHPublic
                        );

                if ( !pMyDHPrime || !pMyDHGenerator || !pMyDHPublic )
                {
                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_internalError
                            );

                    goto  EXIT2;
                }
                else if ( AnscAsn1EqualInts(pMyDHPrime,     pSecurityParams->DHPrime    ) &&
                          AnscAsn1EqualInts(pMyDHGenerator, pSecurityParams->DHGenerator) )
                {
                    AnscCloneX509Key(pMyPublicKey,  pSecurityParams->DHKeyPubC);
                    AnscCloneX509Key(pMyPrivateKey, pSecurityParams->DHKeyPrvC);

                    if ( !pSecurityParams->DHKeyPubC || !pSecurityParams->DHKeyPrvC )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        goto  EXIT2;
                    }

                    bEmptyKeMsg                          = TRUE;
                    pSecurityParams->DHPublicC           = (ANSC_ASN1_INT*)AnscAsn1CloneInt((ANSC_HANDLE)pMyDHPublic);
                    pSecurityParams->PreMasterSecretSize = TLS_PRE_MASTER_SECRET_SIZE;
                    returnStatus                         =
                        pTlsCryptoProvider->AgreeDHSecret
                            (
                                (ANSC_HANDLE)pTlsCryptoProvider,
                                pSecurityParams->DHKeyPubC,
                                pSecurityParams->DHKeyPubS,
                                pSecurityParams->DHKeyPrvC,
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

                        goto  EXIT2;
                    }
                }
                else
                {
                    bEmptyKeMsg = FALSE;
                }

                AnscAsn1FreeInt(pMyDHPrime    );
                AnscAsn1FreeInt(pMyDHGenerator);
                AnscAsn1FreeInt(pMyDHPublic   );
            }
        }
        else
        {
            bEmptyKeMsg = FALSE;
        }
    }
    else
    {
        bEmptyKeMsg = FALSE;
    }

    if ( bEmptyKeMsg )
    {
        ulTlsHsBodySize = 0;
    }
    else
    {
        switch ( pSessionState->KeAlgorithm )
        {
            case    TLS_KE_ALGORITHM_RSA        :
            case    TLS_KE_ALGORITHM_RSA_EXPORT :

                    if ( !pSecurityParams->RsaKeyPub )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        goto  EXIT1;
                    }
                    else
                    {
                        pSecurityParams->PreMasterSecretSize = 48;
                        pSecurityParams->PreMasterSecret[0]  = 3;
                        pSecurityParams->PreMasterSecret[1]  = 1;

                        returnStatus =
                            pTlsCryptoProvider->GetRandomBytes
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    AnscGetTickInMilliSeconds(),
                                    &pSecurityParams->PreMasterSecret[2],
                                    46
                                );
                    }

                    returnStatus =
                        pTlsCryptoProvider->Encrypt_RSA
                            (
                                (ANSC_HANDLE)pTlsCryptoProvider,
                                pSecurityParams->RsaKeyPub,
                                pSecurityParams->PreMasterSecret,
                                48,
                                pScratchPad,
                                &ulScratchPadSize
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

                        goto  EXIT1;
                    }

                    TlsHsClientKeSetEpms2(pTlsHsClientKe, pSecurityParams->MinorVersion, pScratchPad, ulScratchPadSize);

                    if ( pSecurityParams->MinorVersion == 0 )
                    {
                        ulTlsHsBodySize = ulScratchPadSize;
                    }

                    break;

            case    TLS_KE_ALGORITHM_DHE_DSS        :
            case    TLS_KE_ALGORITHM_DHE_DSS_EXPORT :
            case    TLS_KE_ALGORITHM_DHE_RSA        :
            case    TLS_KE_ALGORITHM_DHE_RSA_EXPORT :
            case    TLS_KE_ALGORITHM_DH_DSS         :
            case    TLS_KE_ALGORITHM_DH_RSA         :
            case    TLS_KE_ALGORITHM_DH_ANONYMOUS   :

                    if ( !pSecurityParams->DHPrime || !pSecurityParams->DHGenerator || !pSecurityParams->DHPublicS )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_internalError
                                );

                        goto  EXIT1;
                    }
                    else
                    {
                        returnStatus =
                            pTlsCryptoProvider->GenerateDHKeys2
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    pSecurityParams->DHPrime,
                                    pSecurityParams->DHGenerator,
                                    &pSecurityParams->DHKeyPubC,
                                    &pSecurityParams->DHKeyPrvC
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

                        goto  EXIT1;
                    }
                    else
                    {
                        returnStatus =
                            pTlsCryptoProvider->DecapDHPublicKey
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    pSecurityParams->DHKeyPubC,
                                    &pMyDHPrime,
                                    &pMyDHGenerator,
                                    &pMyDHPublic
                                );

                        if ( !pMyDHPrime || !pMyDHGenerator || !pMyDHPublic )
                        {
                            returnStatus =
                                pTlsCbcIf->GenerateAlert
                                    (
                                        pTlsCbcIf->hOwnerContext,
                                        TLS_ALERT_LEVEL_fatal,
                                        TLS_ALERT_DESCR_internalError
                                    );

                            goto  EXIT2;
                        }
                        else
                        {
                            pSecurityParams->DHPublicC = (ANSC_ASN1_INT*)AnscAsn1CloneInt((ANSC_HANDLE)pMyDHPublic);

                            if ( !pSecurityParams->DHPublicC )
                            {
                                returnStatus =
                                    pTlsCbcIf->GenerateAlert
                                        (
                                            pTlsCbcIf->hOwnerContext,
                                            TLS_ALERT_LEVEL_fatal,
                                            TLS_ALERT_DESCR_internalError
                                        );

                                goto  EXIT2;
                            }
                        }

                        AnscAsn1FreeInt(pMyDHPrime    );
                        AnscAsn1FreeInt(pMyDHGenerator);
                        AnscAsn1FreeInt(pMyDHPublic   );
                    }

                    if ( TRUE )
                    {
                        pSecurityParams->PreMasterSecretSize = TLS_PRE_MASTER_SECRET_SIZE;
                        returnStatus                         =
                            pTlsCryptoProvider->AgreeDHSecret
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    pSecurityParams->DHKeyPubC,
                                    pSecurityParams->DHKeyPubS,
                                    pSecurityParams->DHKeyPrvC,
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

                        goto  EXIT1;
                    }

                    TlsHsClientKeSetDHPublic2(pTlsHsClientKe, pSecurityParams->DHPublicC->Octets, pSecurityParams->DHPublicC->Length);

                    break;

            default :

                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_internalError
                            );

                    goto  EXIT1;

                    break;
        }

        if ( ulTlsHsBodySize == 0 )
        {
            ulTlsHsBodySize = TlsHsClientKeGetBodySize(pTlsHsClientKe, FALSE);
        }
    }

 
    if ( TRUE )
    {
        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_clientKeyExchange);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize                     );
    }

    AnscTrace("TlsHsoClient: send Client Key Exchange message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

    returnStatus =
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                pTlsHsHeader,
                TlsHandshakeGetSize(pTlsHsHeader)
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        pMyObject->HsClientState = TLS_HSC_STATE_sentClientKeyExchange;

        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);

        if ( TRUE )
        {
            returnStatus =
                pMyObject->SaveHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        pTlsHsHeader,
                        TlsHandshakeGetSize(pTlsHsHeader)
                    );
        }

        if ( pTlsHsHeader )
        {
            AnscFreeMemory(pTlsHsHeader);

            pTlsHsHeader = NULL;
        }
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

    returnStatus = pMyObject->SendCertVerify((ANSC_HANDLE)pMyObject);

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pMyDHPrime )
    {
        AnscAsn1FreeInt(pMyDHPrime);
    }

    if ( pMyDHGenerator )
    {
        AnscAsn1FreeInt(pMyDHGenerator);
    }

    if ( pMyDHPublic )
    {
        AnscAsn1FreeInt(pMyDHPublic);
    }

EXIT1:

    if ( pTlsHsHeader )
    {
        AnscFreeMemory(pTlsHsHeader);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientSendCertVerify
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to construct an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoClientSendCertVerify
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    ANSC_X509_CERTIFICATE*          pMyCert            = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X509_KEY*                  pMyPubKey          = (ANSC_X509_KEY*               )NULL;
    ANSC_X509_KEY*                  pMyPrvKey          = (ANSC_X509_KEY*               )NULL;
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )1024;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_CERT_VERIFY             pTlsHsCertVerify   = (PTLS_HS_CERT_VERIFY          )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    ULONG                           ulHashDataSize     = (ULONG                        )0;
    ULONG                           ulTbhDataSize      = (ULONG                        )0;
    ULONG                           ulSignatureSize    = (ULONG                        )0;
    PVOID                           pScratchPad        = (PVOID                        )NULL;
    ULONG                           i                  = (ULONG                        )0;
    ULONG                           tbs_data_size      = (ULONG                        )0;
    UCHAR                           tbs_data[36];

    /*
     * This message is used to provide explicit verification of a client certificate. This message
     * is only sent following a client certificate that has signing capability (i.e. all certifi-
     * cates except those containing fixed Diffie-Hellman parameters). When sent, it will imme-
     * diately follow the client key exchange message.
     */
    if ( pMyObject->HsClientState != TLS_HSC_STATE_sentClientKeyExchange )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( !pSessionState->bCertRequested )
    {
        returnStatus = pMyObject->ChangeCipherW((ANSC_HANDLE)pMyObject);
        returnStatus = pMyObject->SendFinished ((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }
    else if ( !pSecurityParams->MyProfile )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_internalError
                );

        return  returnStatus;
    }
    else
    {
        pMyCert   = pSecurityParams->MyProfile->Certificate;
        pMyPubKey = pSecurityParams->MyProfile->PubKey;
        pMyPrvKey = pSecurityParams->MyProfile->PrvKey;
    }

    if ( !pMyCert || !pMyPubKey || !pMyPrvKey )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_internalError
                );

        return  returnStatus;
    }
    else if ( (pMyPrvKey->KeyType != ANSC_CERT_keyType_rsa) &&
              (pMyPrvKey->KeyType != ANSC_CERT_keyType_dsa) )
    {
        returnStatus = pMyObject->ChangeCipherW((ANSC_HANDLE)pMyObject);
        returnStatus = pMyObject->SendFinished ((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }
    else if ( !(pMyCert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) )
    {
        returnStatus = pMyObject->ChangeCipherW((ANSC_HANDLE)pMyObject);
        returnStatus = pMyObject->SendFinished ((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }
    else
    {
        ulTlsMsgSize  = 2048;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pScratchPad      = (PVOID                )((ULONG)pTlsMsgBuffer + 1024);
            pTlsHsHeader     = (PTLS_HANDSHAKE_HEADER)pTlsMsgBuffer;
            pTlsHsCertVerify = (PTLS_HS_CERT_VERIFY  )TlsHandshakeGetBody(pTlsHsHeader);
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
        if ( pMyPrvKey->KeyType == ANSC_CERT_keyType_rsa )
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

        if ( (pMyPrvKey->KeyType == ANSC_CERT_keyType_rsa) ||
             (pMyPrvKey->KeyType == ANSC_CERT_keyType_dsa) )
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
        if ( pMyPrvKey->KeyType == ANSC_CERT_keyType_rsa )
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

        if ( (pMyPrvKey->KeyType == ANSC_CERT_keyType_rsa) ||
             (pMyPrvKey->KeyType == ANSC_CERT_keyType_dsa) )
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
        if ( pMyPrvKey->KeyType == ANSC_CERT_keyType_rsa )
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

        if ( (pMyPrvKey->KeyType == ANSC_CERT_keyType_rsa) ||
             (pMyPrvKey->KeyType == ANSC_CERT_keyType_dsa) )
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

    if ( pMyPrvKey->KeyType == ANSC_CERT_keyType_rsa )
    {
        tbs_data_size   = ANSC_MD5_OUTPUT_SIZE + ANSC_SHA1_OUTPUT_SIZE;
        ulSignatureSize = 1024;
        returnStatus    =
            pTlsCryptoProvider->Sign
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pMyPrvKey->KeyType,
                    pMyPrvKey,
                    tbs_data,
                    tbs_data_size,
                    pScratchPad,
                    &ulSignatureSize
                );
    }
    else if ( pMyPrvKey->KeyType == ANSC_CERT_keyType_dsa )
    {
        tbs_data_size   = ANSC_SHA1_OUTPUT_SIZE;
        ulSignatureSize = 1024;
        returnStatus    =
            pTlsCryptoProvider->Sign
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pMyPrvKey->KeyType,
                    pMyPrvKey,
                    (PVOID)((ULONG)tbs_data + ANSC_MD5_OUTPUT_SIZE),
                    tbs_data_size,
                    pScratchPad,
                    &ulSignatureSize
                );
    }

    TlsHsCertVerifySetSignature2(pTlsHsCertVerify, pScratchPad, ulSignatureSize);

  
    if ( TRUE )
    {
        ulTlsHsBodySize = TlsHsCertVerifyGetBodySize(pTlsHsCertVerify);

        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_certificateVerify);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize                     );
    }

    AnscTrace("TlsHsoClient: send Certificate Verify message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

    returnStatus =
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                pTlsHsHeader,
                TlsHandshakeGetSize(pTlsHsHeader)
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        pMyObject->HsClientState = TLS_HSC_STATE_sentCertVerify;

        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);

        if ( TRUE )
        {
            returnStatus =
                pMyObject->SaveHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        pTlsHsHeader,
                        TlsHandshakeGetSize(pTlsHsHeader)
                    );
        }

        if ( pTlsHsHeader )
        {
            AnscFreeMemory(pTlsHsHeader);

            pTlsHsHeader = NULL;
        }
    }

    returnStatus = pMyObject->ChangeCipherW((ANSC_HANDLE)pMyObject);
    returnStatus = pMyObject->SendFinished ((ANSC_HANDLE)pMyObject);

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pTlsHsHeader )
    {
        AnscFreeMemory(pTlsHsHeader);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoClientSendFinished
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to construct an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoClientSendFinished
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_CLIENT_OBJECT          pMyObject          = (PTLS_HSO_CLIENT_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams        = (PTLS_CONNECTION_PARAMS       )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )1024;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_FINISHED                pTlsHsFinished     = (PTLS_HS_FINISHED             )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    ULONG                           ulHashDataSize     = (ULONG                        )0;
    ULONG                           ulTbhDataSize      = (ULONG                        )0;
    PVOID                           pScratchPad        = (PVOID                        )NULL;
    ULONG                           i                  = (ULONG                        )0;
    UCHAR                           tbs_data[36];

    /*
     * A finished message is always sent immediately after a change cipher spec message to verify
     * that the key exchange and authentication processes were successful. It is essential that a
     * change cipher spec message be received between the other handshake messages and the Finished
     * message.
     */
    if ( !(pMyObject->ProgressBits & TLS_HSO_PROGRESS_BIT_changeCipherW) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulTlsMsgSize  = 2048;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pScratchPad      = (PVOID                )((ULONG)pTlsMsgBuffer + 1024);
            pTlsHsHeader     = (PTLS_HANDSHAKE_HEADER)pTlsMsgBuffer;
            pTlsHsFinished   = (PTLS_HS_FINISHED     )TlsHandshakeGetBody(pTlsHsHeader);
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

        ulTlsHsBodySize = ANSC_MD5_OUTPUT_SIZE + ANSC_SHA1_OUTPUT_SIZE;

        TlsHsFinishedSetVerifyData(pTlsHsFinished, tbs_data, ANSC_MD5_OUTPUT_SIZE + ANSC_SHA1_OUTPUT_SIZE);
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

        ulTlsHsBodySize = 12;

        TlsHsFinishedSetVerifyData(pTlsHsFinished, pScratchPad, 12);
    }


    if ( TRUE )
    {
        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_finished);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize            );
    }

    AnscTrace("TlsHsoClient: send Finished message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

    returnStatus =
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                pTlsHsHeader,
                TlsHandshakeGetSize(pTlsHsHeader)
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        pMyObject->HsClientState = TLS_HSC_STATE_sentFinished;

        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);

        if ( TRUE )
        {
            returnStatus =
                pMyObject->SaveHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        pTlsHsHeader,
                        TlsHandshakeGetSize(pTlsHsHeader)
                    );
        }

        if ( pTlsHsHeader )
        {
            AnscFreeMemory(pTlsHsHeader);

            pTlsHsHeader = NULL;
        }
    }

    /*
     * Once a side has sent its Finished message and received and validated the Finished message
     * from its peer, it may begin to send and receive application data over the connection.
     */
    if ( pConnParams->bQuickHandshake )
    {
        pMyObject->Establish((ANSC_HANDLE)pMyObject);
    }

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pTlsHsHeader )
    {
        AnscFreeMemory(pTlsHsHeader);
    }

    return  returnStatus;
}
