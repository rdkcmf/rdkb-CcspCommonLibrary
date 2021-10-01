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

    module:	tls_hsoserver_send.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Hso Server Object.

        *   TlsHsoServerSendHelloRequest
        *   TlsHsoServerSendServerHello
        *   TlsHsoServerSendCertificate
        *   TlsHsoServerSendServerKeyExchange
        *   TlsHsoServerSendCertRequest
        *   TlsHsoServerSendServerHelloDone
        *   TlsHsoServerSendFinished

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
        TlsHsoServerSendHelloRequest
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
TlsHsoServerSendHelloRequest
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    TLS_HANDSHAKE_HEADER            tlsHandshakeHeader;

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
    if ( pMyObject->HsServerState != TLS_HSS_STATE_idle )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    /*
     * After sending a hello request, servers should not repeat the request until the subsequent
     * handshake negotiation is complete.
     *
     *      struct { } HelloRequest;
     */
    TlsHandshakeSetMsgType((&tlsHandshakeHeader), TLS_HANDSHAKE_TYPE_helloRequest);
    TlsHandshakeSetLength ((&tlsHandshakeHeader), 0                              );

    AnscTrace("TlsHsoServer: send Hello Request message size = %lu!\n", TlsHandshakeGetSize((&tlsHandshakeHeader)));

    if ( TRUE )
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }
    else
    {
        returnStatus =
            pTlsRecordKeeper->Send
                (
                    (ANSC_HANDLE)pTlsRecordKeeper,
                    pMyObject->RecordType,
                    &tlsHandshakeHeader,
                    TlsHandshakeGetSize((&tlsHandshakeHeader))
                );
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        pMyObject->HsServerState = TLS_HSS_STATE_sentHelloRequest;
        pMyObject->HsServerState = TLS_HSS_STATE_waitClientHello;

        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerSendServerHello
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
TlsHsoServerSendServerHello
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
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
    PTLS_HS_SERVER_HELLO            pTlsHsServerHello  = (PTLS_HS_SERVER_HELLO         )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    
    /*
     * The server will send this message in response to a client hello message when it was able to
     * find an acceptable set of algorithms. If it cannot find such a match, it will respond with a
     * handshake failure alert.
     */
    if ( pMyObject->HsServerState != TLS_HSS_STATE_waitClientHello )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        ulTlsMsgSize  = 1024;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pTlsHsHeader      = (PTLS_HANDSHAKE_HEADER)pTlsMsgBuffer;
            pTlsHsServerHello = (PTLS_HS_SERVER_HELLO )TlsHandshakeGetBody(pTlsHsHeader);
        }
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
    returnStatus =
        pTlsCryptoProvider->GetRandomBytes
            (
                (ANSC_HANDLE)pTlsCryptoProvider,
                AnscGetTickInSeconds(),
                pSecurityParams->ServerRandom,
                TLS_OPAQUE_RANDOM_SIZE
            );

    TlsHsServerHelloSetMajorVersion(pTlsHsServerHello, pSecurityParams->MajorVersion);
    TlsHsServerHelloSetMinorVersion(pTlsHsServerHello, pSecurityParams->MinorVersion);
    TlsHsServerHelloSetRandom      (pTlsHsServerHello, pSecurityParams->ServerRandom);
    TlsHsServerHelloSetSessionId   (pTlsHsServerHello, pSessionState->SessionIDSize, pSessionState->SessionID);

    /*
     * The single cipher suite selected by the server from the list in ClientHello.cipher_suites.
     * For resumed sessions this field is the value from the state of the session being resumed.
     */
    TlsHsServerHelloSetCipherSuite(pTlsHsServerHello, pConnParams->CipherSuiteArray[0]);

    TlsHsServerHelloSetCompression(pTlsHsServerHello, pConnParams->CompressionArray[0]);

 
    if ( TRUE )
    {
        ulTlsHsBodySize = TlsHsServerHelloGetBodySize(pTlsHsServerHello);

        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_serverHello);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize               );
    }

    AnscTrace("TlsHsoServer: send Server Hello message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

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
        pMyObject->HsServerState = TLS_HSS_STATE_sentServerHello;

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

    if ( pConnParams->bQuickHandshake )
    {
        pMyObject->Agree        ((ANSC_HANDLE)pMyObject, TRUE);
        pMyObject->ChangeCipherW((ANSC_HANDLE)pMyObject);

        returnStatus = pMyObject->SendFinished((ANSC_HANDLE)pMyObject);
    }
    else
    {
        returnStatus = pMyObject->SendCertificate((ANSC_HANDLE)pMyObject);
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


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerSendCertificate
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
TlsHsoServerSendCertificate
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    ANSC_X509_CERTIFICATE_CHAIN*    pX509CertChain     = (ANSC_X509_CERTIFICATE_CHAIN* )NULL;
    ANSC_X509_PROFILE*              pX509Profile       = (ANSC_X509_PROFILE*           )NULL;
    ULONG                           ulCertConfigBit    = (ULONG                        )0;
    ULONG                           ulCertChainSize    = (ULONG                        )0;
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )1024;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_CERTIFICATE             pTlsHsCertificate  = (PTLS_HS_CERTIFICATE          )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    ULONG                           i                  = 0;

    /*
     * The server must send a certificate whenever the agreed-upon key exchange method is not an
     * anonymous one. This message will always immediately follow the server hello message.
     */
    if ( pMyObject->HsServerState != TLS_HSS_STATE_sentServerHello )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
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

    if ( ulCertConfigBit == 0 )
    {
        returnStatus = pMyObject->SendServerKeyExchange((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }
    else
    {
        pX509CertChain =
            (ANSC_X509_CERTIFICATE_CHAIN*)pTlsMecIf->GetCertChain1
                (
                    pTlsMecIf->hOwnerContext,
                    ulCertConfigBit,
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
        pX509Profile =
            (ANSC_X509_PROFILE*)pTlsMecIf->GetProfile
                (
                    pTlsMecIf->hOwnerContext,
                    pSessionState->CertID
                );
    }

    if ( !pX509Profile )
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
        pSecurityParams->MyProfile = pX509Profile;
    }

    if ( TRUE )
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

    AnscTrace("TlsHsoServer: send Certificate message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

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
        pMyObject->HsServerState = TLS_HSS_STATE_sentCertificate;

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

    returnStatus = pMyObject->SendServerKeyExchange((ANSC_HANDLE)pMyObject);

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
        TlsHsoServerSendServerKeyExchange
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
TlsHsoServerSendServerKeyExchange
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS         )&pSessionState->SecurityParams;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    ANSC_X509_PROFILE*              pX509Profile       = (ANSC_X509_PROFILE*           )pSecurityParams->MyProfile;
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )1024;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_SERVER_KEY_EXCHANGE     pTlsHsServerKe     = (PTLS_HS_SERVER_KEY_EXCHANGE  )NULL;
    PTLS_HS_PARAMETER               pTlsHsParamSig     = (PTLS_HS_PARAMETER            )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    BOOL                            bSignatureNeeded   = (BOOL                         )FALSE;
    void*                           pTlsHsSignature    = (void*                        )NULL;
    ULONG                           ulSignatureSize    = (ULONG                        )0;
    void*                           pHashData          = (void*                        )NULL;
    ULONG                           ulTbhSize          = (ULONG                        )0;
    ULONG                           ulHashResultSize   = (ULONG                        )0;
    ULONG                           tbs_data_size      = (ULONG                        )0;
    UCHAR                           tbs_data[36];

    /*
     * This message will be sent immediately after the server certificate message (or the server
     * hello message, if this is an anonymous negotiation).
     */
    if ( (pMyObject->HsServerState != TLS_HSS_STATE_sentServerHello) &&
         (pMyObject->HsServerState != TLS_HSS_STATE_sentCertificate) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
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
    switch ( pSessionState->KeAlgorithm )
    {
        case    TLS_KE_ALGORITHM_RSA :

                AnscCloneX509Key(pX509Profile->PubKey, pSecurityParams->RsaKeyPub);
                AnscCloneX509Key(pX509Profile->PrvKey, pSecurityParams->RsaKeyPrv);

                if ( !pSecurityParams->RsaKeyPub || !pSecurityParams->RsaKeyPrv )
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
                    returnStatus =
                        pTlsCryptoProvider->DecapRSAPublicKey
                            (
                                (ANSC_HANDLE)pTlsCryptoProvider,
                                pSecurityParams->RsaKeyPub,
                                &pSecurityParams->RsaModulus,
                                &pSecurityParams->RsaExponent
                            );
                }

                returnStatus = pMyObject->SendCertRequest((ANSC_HANDLE)pMyObject);

                return  returnStatus;

                break;

        case    TLS_KE_ALGORITHM_RSA_EXPORT :

                if ( (pX509Profile->Certificate->KeyBits <= 512                               ) &&
                     (pX509Profile->Certificate->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment) )
                {
                    AnscCloneX509Key(pX509Profile->PubKey, pSecurityParams->RsaKeyPub);
                    AnscCloneX509Key(pX509Profile->PrvKey, pSecurityParams->RsaKeyPrv);

                    if ( !pSecurityParams->RsaKeyPub || !pSecurityParams->RsaKeyPrv )
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
                        returnStatus =
                            pTlsCryptoProvider->DecapRSAPublicKey
                                (
                                    (ANSC_HANDLE)pTlsCryptoProvider,
                                    pSecurityParams->RsaKeyPub,
                                    &pSecurityParams->RsaModulus,
                                    &pSecurityParams->RsaExponent
                                );
                    }

                    returnStatus = pMyObject->SendCertRequest((ANSC_HANDLE)pMyObject);

                    return  returnStatus;
                }

                break;

        case    TLS_KE_ALGORITHM_DH_DSS :
        case    TLS_KE_ALGORITHM_DH_RSA :

                AnscCloneX509Key(pX509Profile->PubKey, pSecurityParams->DHKeyPubS);
                AnscCloneX509Key(pX509Profile->PrvKey, pSecurityParams->DHKeyPrvS);

                if ( !pSecurityParams->DHKeyPubS || !pSecurityParams->DHKeyPrvS )
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
                    returnStatus =
                        pTlsCryptoProvider->DecapDHPublicKey
                            (
                                (ANSC_HANDLE)pTlsCryptoProvider,
                                pSecurityParams->DHKeyPubS,
                                &pSecurityParams->DHPrime,
                                &pSecurityParams->DHGenerator,
                                &pSecurityParams->DHPublicS
                            );
                }

                returnStatus = pMyObject->SendCertRequest((ANSC_HANDLE)pMyObject);

                return  returnStatus;

                break;

        default :

                break;
    }

    /*
     * This message conveys cryptographic information to allow the client to communicate the pre-
     * master secret: either an RSA public key to encrypt the premaster secret with, or a Diffie-
     * Hellman public key with which the client can complete a key exchange (with the result being
     * the premaster secret.)
     */
    if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT )
    {
        returnStatus =
            pTlsCryptoProvider->GenerateRSAKeys
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    512,
                    &pSecurityParams->RsaKeyPub,
                    &pSecurityParams->RsaKeyPrv
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

            return  returnStatus;
        }
        else
        {
            returnStatus =
                pTlsCryptoProvider->DecapRSAPublicKey
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pSecurityParams->RsaKeyPub,
                        &pSecurityParams->RsaModulus,
                        &pSecurityParams->RsaExponent
                    );
        }
    }
    else
    {
        returnStatus =
            pTlsCryptoProvider->GenerateDHKeys1
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    &pSecurityParams->DHKeyPubS,
                    &pSecurityParams->DHKeyPrvS
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

            return  returnStatus;
        }
        else
        {
            returnStatus =
                pTlsCryptoProvider->DecapDHPublicKey
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pSecurityParams->DHKeyPubS,
                        &pSecurityParams->DHPrime,
                        &pSecurityParams->DHGenerator,
                        &pSecurityParams->DHPublicS
                    );
        }
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

        return  returnStatus;
    }
    else
    {
        ulTlsMsgSize  = 8192;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pTlsHsHeader   = (PTLS_HANDSHAKE_HEADER      )((ULONG)pTlsMsgBuffer + 128);
            pTlsHsServerKe = (PTLS_HS_SERVER_KEY_EXCHANGE)TlsHandshakeGetBody(pTlsHsHeader);
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
        TlsHsServerKeSetRSAModulus2 (pTlsHsServerKe, pSecurityParams->RsaModulus ->Octets, pSecurityParams->RsaModulus ->Length);
        TlsHsServerKeSetRSAExponent2(pTlsHsServerKe, pSecurityParams->RsaExponent->Octets, pSecurityParams->RsaExponent->Length);
    }
    else
    {
        TlsHsServerKeSetDHPrime2    (pTlsHsServerKe, pSecurityParams->DHPrime    ->Octets, pSecurityParams->DHPrime    ->Length);
        TlsHsServerKeSetDHGenerator2(pTlsHsServerKe, pSecurityParams->DHGenerator->Octets, pSecurityParams->DHGenerator->Length);
        TlsHsServerKeSetDHPublic2   (pTlsHsServerKe, pSecurityParams->DHPublicS  ->Octets, pSecurityParams->DHPublicS  ->Length);
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
        pHashData  = (PVOID)((ULONG)pTlsHsServerKe - TLS_OPAQUE_RANDOM_SIZE * 2);

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
        pTlsHsSignature = AnscAllocateMemory(512);

        if ( !pTlsHsSignature )
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
            ulSignatureSize = 512;
            returnStatus    =
                pTlsCryptoProvider->Sign
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pSecurityParams->MyProfile->PrvKey->KeyType,
                        pSecurityParams->MyProfile->PrvKey,
                        tbs_data,
                        tbs_data_size,
                        pTlsHsSignature,
                        &ulSignatureSize
                    );
        }

        if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT )
        {
            TlsHsServerKeSetSignature2
                (
                    pTlsHsServerKe,
                    TLS_HS_KE_ALGORITHM_rsa,
                    pTlsHsSignature,
                    ulSignatureSize
                );
        }
        else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS       ) ||
                  (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS_EXPORT) ||
                  (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA       ) ||
                  (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA_EXPORT) )
        {
            TlsHsServerKeSetSignature2
                (
                    pTlsHsServerKe,
                    TLS_HS_KE_ALGORITHM_diffieHellman,
                    pTlsHsSignature,
                    ulSignatureSize
                );
        }

        AnscFreeMemory(pTlsHsSignature);
    }

    
    if ( TRUE )
    {
        if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_RSA_EXPORT )
        {
            TlsHsServerKeGetBodySize(pTlsHsServerKe, TLS_HS_KE_ALGORITHM_rsa, TLS_HS_SIG_ALGORITHM_rsa, ulTlsHsBodySize);
        }
        else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS       ) ||
                  (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_DSS_EXPORT) )
        {
            TlsHsServerKeGetBodySize(pTlsHsServerKe, TLS_HS_KE_ALGORITHM_diffieHellman, TLS_HS_SIG_ALGORITHM_dsa, ulTlsHsBodySize);
        }
        else if ( (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA       ) ||
                  (pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DHE_RSA_EXPORT) )
        {
            TlsHsServerKeGetBodySize(pTlsHsServerKe, TLS_HS_KE_ALGORITHM_diffieHellman, TLS_HS_SIG_ALGORITHM_rsa, ulTlsHsBodySize);
        }
        else
        {
            TlsHsServerKeGetBodySize(pTlsHsServerKe, TLS_HS_KE_ALGORITHM_diffieHellman, TLS_HS_SIG_ALGORITHM_anonymous, ulTlsHsBodySize);
        }

        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_serverKeyExchange);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize                     );
    }

    AnscTrace("TlsHsoServer: send Server Key Exchange message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

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
        pMyObject->HsServerState = TLS_HSS_STATE_sentServerKeyExchange;

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

        if ( pTlsMsgBuffer )
        {
            AnscFreeMemory(pTlsMsgBuffer);

            pTlsMsgBuffer = NULL;
        }
    }

    returnStatus = pMyObject->SendCertRequest((ANSC_HANDLE)pMyObject);

    goto  EXIT1;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pTlsMsgBuffer )
    {
        AnscFreeMemory(pTlsMsgBuffer);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerSendCertRequest
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
TlsHsoServerSendCertRequest
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT  )pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS           )pTlsMecIf->GetModuleParams     (pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams        = (PTLS_CONNECTION_PARAMS       )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);
    ULONG                           ulClientCertType   = (ULONG                        )0;
    ANSC_X509_CERTIFICATE*          pX509CACert        = (ANSC_X509_CERTIFICATE*       )NULL;
    ANSC_X501_NAME*                 pX501CAName        = (ANSC_X501_NAME*              )NULL;
    PVOID                           pTlsMsgBuffer      = (PVOID                        )NULL;
    ULONG                           ulTlsMsgSize       = (ULONG                        )1024;
    PTLS_HANDSHAKE_HEADER           pTlsHsHeader       = (PTLS_HANDSHAKE_HEADER        )NULL;
    PTLS_HS_CERT_REQUEST            pTlsHsCertRequest  = (PTLS_HS_CERT_REQUEST         )NULL;
    ULONG                           ulTlsHsBodySize    = (ULONG                        )0;
    ULONG                           i                  = 0;

    /*
     * A non-anonymous server can optionally request a certificate from the client, if appropriate
     * for the selected cipher suite. This message, if sent, will immediately follow the Server Key
     * Exchange message (if it is sent; otherwise, the Server Certificate message).
     */
    if ( (pMyObject->HsServerState != TLS_HSS_STATE_sentCertificate      ) &&
         (pMyObject->HsServerState != TLS_HSS_STATE_sentServerKeyExchange) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else if ( pSessionState->KeAlgorithm == TLS_KE_ALGORITHM_DH_ANONYMOUS )
    {
        returnStatus = pMyObject->SendServerHelloDone((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }
    else if ( !pConnParams->bReqCertificate )
    {
        returnStatus = pMyObject->SendServerHelloDone((ANSC_HANDLE)pMyObject);

        return  returnStatus;
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

    if ( ulClientCertType == TLS_CLIENT_CERT_TYPE_unknown )
    {
        returnStatus = pMyObject->SendServerHelloDone((ANSC_HANDLE)pMyObject);

        return  returnStatus;
    }
    else
    {
        ulTlsMsgSize  = 8192;
        pTlsMsgBuffer = AnscAllocateMemory(ulTlsMsgSize);

        if ( !pTlsMsgBuffer )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pTlsHsHeader      = (PTLS_HANDSHAKE_HEADER)pTlsMsgBuffer;
            pTlsHsCertRequest = (PTLS_HS_CERT_REQUEST )TlsHandshakeGetBody(pTlsHsHeader);
        }

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
    TlsHsCertReqSetTypeCount   (pTlsHsCertRequest, 0                      );
    TlsHsCertReqAddType        (pTlsHsCertRequest, (UCHAR)ulClientCertType);
    TlsHsCertReqSetDnListLength(pTlsHsCertRequest, 0                      );

    for ( i = 0; i < pModuleParams->CACertCount; i++ )
    {
        pX509CACert = pModuleParams->CACertArray[i];
        pX501CAName =
            AnscX509CertGetSubject
                (
                    pX509CACert->CertData,
                    pX509CACert->CertSize
                );

        TlsHsCertReqAddDn2(pTlsHsCertRequest, pX501CAName->Data, pX501CAName->Length);

        AnscFreeX501Name(pX501CAName);
    }

 
    if ( TRUE )
    {
        ulTlsHsBodySize = TlsHsCertReqGetBodySize(pTlsHsCertRequest);

        TlsHandshakeSetMsgType(pTlsHsHeader, TLS_HANDSHAKE_TYPE_certificateRequest);
        TlsHandshakeSetLength (pTlsHsHeader, ulTlsHsBodySize                      );
    }

    AnscTrace("TlsHsoServer: send Certificate Request message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

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
        pMyObject->HsServerState = TLS_HSS_STATE_sentCertRequest;

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

    returnStatus = pMyObject->SendServerHelloDone((ANSC_HANDLE)pMyObject);

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
        TlsHsoServerSendServerHelloDone
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
TlsHsoServerSendServerHelloDone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT    )pMyObject->hTlsRecordKeeper;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    TLS_HANDSHAKE_HEADER            tlsHandshakeHeader;

    /*
     * The server hello done message is sent by the server to indicate the end of the server hello
     * and associated messages. After sending this message the server will wait for a client response.
     */
    if ( (pMyObject->HsServerState != TLS_HSS_STATE_sentCertificate      ) &&
         (pMyObject->HsServerState != TLS_HSS_STATE_sentServerKeyExchange) &&
         (pMyObject->HsServerState != TLS_HSS_STATE_sentCertRequest      ) )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    /*
     * This message means that the server is done sending messages to support the key exchange, and
     * the client can proceed with its phase of the key exchange. Upon receipt of the server hello
     * done message the client should verify that the server provided a valid certificate if
     * required and check that the server hello parameters are acceptable.
     *
     *      struct { } ServerHelloDone;
     */
    TlsHandshakeSetMsgType((&tlsHandshakeHeader), TLS_HANDSHAKE_TYPE_serverHelloDone);
    TlsHandshakeSetLength ((&tlsHandshakeHeader), 0                                 );

    AnscTrace("TlsHsoServer: send Server Hello Done message size = %lu!\n", TlsHandshakeGetSize((&tlsHandshakeHeader)));

    returnStatus =
        pTlsRecordKeeper->Send
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                pMyObject->RecordType,
                &tlsHandshakeHeader,
                TlsHandshakeGetSize((&tlsHandshakeHeader))
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }
    else
    {
        pMyObject->HsServerState = TLS_HSS_STATE_sentServerHelloDone;

        if ( pSessionState->bCertRequested )
        {
            pMyObject->HsServerState = TLS_HSS_STATE_waitCertificate;
        }
        else
        {
            pMyObject->HsServerState = TLS_HSS_STATE_waitClientKeyExchange;
        }

        pStateTimerObj->Stop       ((ANSC_HANDLE)pStateTimerObj);
        pStateTimerObj->SetInterval((ANSC_HANDLE)pStateTimerObj, TLS_HSO_DEF_STATE_TIMEOUT);
        pStateTimerObj->Start      ((ANSC_HANDLE)pStateTimerObj);

        if ( TRUE )
        {
            returnStatus =
                pMyObject->SaveHandshakeMsg
                    (
                        (ANSC_HANDLE)pMyObject,
                        &tlsHandshakeHeader,
                        TlsHandshakeGetSize((&tlsHandshakeHeader))
                    );
        }
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoServerSendFinished
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
TlsHsoServerSendFinished
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HSO_SERVER_OBJECT          pMyObject          = (PTLS_HSO_SERVER_OBJECT       )hThisObject;
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
        //UCHAR                       senderClient[4] = {0x43, 0x4C, 0x4E, 0x54};
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
                    "server finished",
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

    AnscTrace("TlsHsoServer: send Finished message size = %lu!\n", TlsHandshakeGetSize(pTlsHsHeader));

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
        pMyObject->HsServerState = TLS_HSS_STATE_sentFinished;

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
    if ( !pConnParams->bQuickHandshake )
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
