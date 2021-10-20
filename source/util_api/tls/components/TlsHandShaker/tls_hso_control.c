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

    module:	tls_hso_control.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the TLS Hand Shaker Object.

        *   TlsHsoStart
        *   TlsHsoAbort
        *   TlsHsoEstablish
        *   TlsHsoAgree
        *   TlsHsoCalKeys30
        *   TlsHsoCalKeys31
        *   TlsHsoCalMasterSecret30
        *   TlsHsoCalMasterSecret31
        *   TlsHsoChangeCipherW
        *   TlsHsoChangeCipherR

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/28/03    initial revision.

**********************************************************************/


#include "tls_hso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoStart
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to start the handshake process.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoStart
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_UNAPPLICABLE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoAbort
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to abort the handshake process.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoAbort
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;

    pTlsCbcIf->GenerateAlert
        (
            pTlsCbcIf->hOwnerContext,
            TLS_ALERT_LEVEL_warning,
            TLS_ALERT_DESCR_userCanceled
        );

    pTlsCbcIf->GenerateAlert
        (
            pTlsCbcIf->hOwnerContext,
            TLS_ALERT_LEVEL_warning,
            TLS_ALERT_DESCR_closeNotify
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoEstablish
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called when the handshake negotiation is
        successfully completed.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoEstablish
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT      )hThisObject;
    PANSC_TIMER_DESCRIPTOR_OBJECT   pStateTimerObj     = (PANSC_TIMER_DESCRIPTOR_OBJECT)pMyObject->hStateTimerObj;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE           )&pMyObject->SessionState;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE           )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE           )pMyObject->hTlsMecIf;
    pTlsMecIf->GetTlsSessionManager(pTlsMecIf->hOwnerContext);
    PTLS_HSM_INTERFACE              pTlsHsmIf          = (PTLS_HSM_INTERFACE           )pTlsMecIf->GetTlsHsmIf         (pTlsMecIf->hOwnerContext);
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS           )pTlsMecIf->GetModuleParams     (pTlsMecIf->hOwnerContext);
    PTLS_CONNECTION_PARAMS          pConnParams        = (PTLS_CONNECTION_PARAMS       )pTlsCbcIf->GetConnParams       (pTlsCbcIf->hOwnerContext);

    pStateTimerObj->Stop((ANSC_HANDLE)pStateTimerObj);

    pTlsCbcIf->NotifyEvent
        (
            pTlsCbcIf->hOwnerContext,
            TLS_CBC_EVENT_HANDSHAKE_SUCCEEDED,
            0,
            (ANSC_HANDLE)pMyObject
        );

    /*
     * If the session is sharable, we can add it to the session pool maintained by the Tls Session
     * Manager. Regardless whether the session state is backed up, we clean all the runtime state
     * information and security parameters.
     */
    if ( pModuleParams->bSessionPooling && pConnParams->bSessionSharing && !pConnParams->bQuickHandshake )
    {
        pTlsHsmIf->AddHandshakeSession
            (
                pTlsHsmIf->hOwnerContext,
                (ANSC_HANDLE)pSessionState
            );
    }

    pMyObject->Reset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoAgree
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bResumed
            );

    description:

        This function is called to conclude the handshake process by
        calculating the same shared secrets.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bResumed
                Specifies whether this is a resumed handshake session.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoAgree
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bResumed
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE         )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS       )&pSessionState->SecurityParams;
    PTLS_RECORD_STATE               pRecordStateW      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateW;
    PTLS_RECORD_STATE               pRecordStateR      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateR;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);

    if ( pSecurityParams->MajorVersion != 3 )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }
    else
    {
        returnStatus =
            pTlsCryptoProvider->GetCipherInfo
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pSessionState->CipherSuite,
                    (ANSC_HANDLE)pSecurityParams
                );
    }

    /*
     * In order to begin connection protection, the TLS Record Protocol requires specification of a
     * suite of algorithms, a master secret, and the client and server random values. The authen-
     * tication, encryption, and MAC algorithms are determined by the cipher_suite selected by the
     * server and revealed in the server hello message. The compression algorithm is negotiated in
     * the hello messages, and the random values are exchanged in the hello messages. All that
     * remains is to calculate the master secret.
     */
    if ( !bResumed )
    {
        AnscTrace("****\npSecurityParams->MinorVersion == %d\n******\n", pSecurityParams->MinorVersion);

        switch ( pSecurityParams->MinorVersion )
        {
            case    0 :

                    returnStatus = pMyObject->CalMasterSecret30((ANSC_HANDLE)pMyObject);

                    break;

            case    1 :

                    returnStatus = pMyObject->CalMasterSecret31((ANSC_HANDLE)pMyObject);

                    break;

            default :

                    returnStatus = ANSC_STATUS_UNAPPLICABLE;

                    break;
        }
    }

    if ( TRUE )
    {
        TlsInitRecordState(pRecordStateW);

        pRecordStateW->MajorVersion            = pSecurityParams->MajorVersion;
        pRecordStateW->MinorVersion            = pSecurityParams->MinorVersion;
        pRecordStateW->KeyBlockSize            = pSecurityParams->KeyBlockSize;
        pRecordStateW->BulkEncryptionAlgorithm = pSecurityParams->BulkEncryptionAlgorithm;
        pRecordStateW->CipherType              = pSecurityParams->CipherType;
        pRecordStateW->CipherKeyNumber         = pSecurityParams->CipherKeyNumber;
        pRecordStateW->CipherKeySize           = pSecurityParams->CipherKeySize;
        pRecordStateW->CipherKeyMaterialSize   = pSecurityParams->CipherKeyMaterialSize;
        pRecordStateW->CipherIVSize            = pSecurityParams->CipherIVSize;
        pRecordStateW->CipherBlockSize         = pSecurityParams->CipherBlockSize;
        pRecordStateW->MacAlgorithm            = pSecurityParams->MacAlgorithm;
        pRecordStateW->MacHashSize             = pSecurityParams->MacHashSize;
        pRecordStateW->CompressionAlgorithm    = pSecurityParams->CompressionAlgorithm;
        pRecordStateW->bExportable             = pSecurityParams->bExportable;
    }

    if ( TRUE )
    {
        TlsInitRecordState(pRecordStateR);

        pRecordStateR->MajorVersion            = pSecurityParams->MajorVersion;
        pRecordStateR->MinorVersion            = pSecurityParams->MinorVersion;
        pRecordStateR->KeyBlockSize            = pSecurityParams->KeyBlockSize;
        pRecordStateR->BulkEncryptionAlgorithm = pSecurityParams->BulkEncryptionAlgorithm;
        pRecordStateR->CipherType              = pSecurityParams->CipherType;
        pRecordStateR->CipherKeyNumber         = pSecurityParams->CipherKeyNumber;
        pRecordStateR->CipherKeySize           = pSecurityParams->CipherKeySize;
        pRecordStateR->CipherKeyMaterialSize   = pSecurityParams->CipherKeyMaterialSize;
        pRecordStateR->CipherIVSize            = pSecurityParams->CipherIVSize;
        pRecordStateR->CipherBlockSize         = pSecurityParams->CipherBlockSize;
        pRecordStateR->MacAlgorithm            = pSecurityParams->MacAlgorithm;
        pRecordStateR->MacHashSize             = pSecurityParams->MacHashSize;
        pRecordStateR->CompressionAlgorithm    = pSecurityParams->CompressionAlgorithm;
        pRecordStateR->bExportable             = pSecurityParams->bExportable;
    }

    switch ( pSecurityParams->MinorVersion )
    {
        case    0 :

                returnStatus = pMyObject->CalKeys30((ANSC_HANDLE)pMyObject);

                break;

        case    1 :

                returnStatus = pMyObject->CalKeys31((ANSC_HANDLE)pMyObject);

                break;

        default :

                returnStatus = ANSC_STATUS_UNAPPLICABLE;

                break;
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pMyObject->ProgressBits |= TLS_HSO_PROGRESS_BIT_calculateKeys;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoCalKeys30
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to calculate keys, IVs, and MAC secrets
        from the security parameters provided by the handshake protocol.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoCalKeys30
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE         )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS       )&pSessionState->SecurityParams;
    PTLS_RECORD_STATE               pRecordStateW      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateW;
    PTLS_RECORD_STATE               pRecordStateR      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateR;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
#ifdef _64BIT_ARCH_SUPPORT_
    ANSC_CRYPTO_KEY*                pMacSecretC        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->MacSecret : &pRecordStateW->MacSecret;
    ANSC_CRYPTO_KEY*                pMacSecretS        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->MacSecret : &pRecordStateR->MacSecret;
    ANSC_CRYPTO_KEY*                pCipherKeyC        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherKey : &pRecordStateW->CipherKey;
    ANSC_CRYPTO_KEY*                pCipherKeyS        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherKey : &pRecordStateR->CipherKey;
    ANSC_CRYPTO_IV*                 pCipherIV_C        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherIV  : &pRecordStateW->CipherIV;
    ANSC_CRYPTO_IV*                 pCipherIV_S        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherIV  : &pRecordStateR->CipherIV;
#else
    ANSC_CRYPTO_KEY*                pMacSecretC        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->MacSecret : &pRecordStateW->MacSecret;
    ANSC_CRYPTO_KEY*                pMacSecretS        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->MacSecret : &pRecordStateR->MacSecret;
    ANSC_CRYPTO_KEY*                pCipherKeyC        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherKey : &pRecordStateW->CipherKey;
    ANSC_CRYPTO_KEY*                pCipherKeyS        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherKey : &pRecordStateR->CipherKey;
    ANSC_CRYPTO_IV*                 pCipherIV_C        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherIV  : &pRecordStateW->CipherIV;
    ANSC_CRYPTO_IV*                 pCipherIV_S        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherIV  : &pRecordStateR->CipherIV;
#endif
    PUCHAR                          pKeyBlock          = (PUCHAR                     )NULL;
    ULONG                           write_key_length   = 0;
    ULONG                           key_block_offset   = 0;
    ULONG                           i                  = 0;
    UCHAR                           random_seed     [64 ];
    UCHAR                           client_write_key[128];
    UCHAR                           server_write_key[128];

    if ( pSecurityParams->KeyBlockSize == 0 )
    {
        pMacSecretC->KeyNumber = 0;
        pMacSecretC->Length    = 0;
        pMacSecretS->KeyNumber = 0;
        pMacSecretS->Length    = 0;

        pCipherKeyC->KeyNumber = 0;
        pCipherKeyC->Length    = 0;
        pCipherKeyS->KeyNumber = 0;
        pCipherKeyS->Length    = 0;

        pCipherIV_C->Length    = 0;
        pCipherIV_S->Length    = 0;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pKeyBlock = (PUCHAR)AnscAllocateMemory(pSecurityParams->KeyBlockSize + 256);

        if ( !pKeyBlock )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    /*
     * When generating keys and MAC secrets, the master secret is used as an entropy source, and
     * the random values provide unencrypted salt material and IVs for exportable ciphers. To
     * generate the key material, compute
     *
     *      key_block =
     *          MD5(master_secret + SHA(`A'   + master_secret + ServerHello.random + ClientHello.random)) +
     *          MD5(master_secret + SHA(`BB'  + master_secret + ServerHello.random + ClientHello.random)) +
     *          MD5(master_secret + SHA(`CCC' + master_secret + ServerHello.random + ClientHello.random)) +
     *          [...];
     *
     * until enough output has been generated. Then the key_block is partitioned as follows.
     *
     *      client_write_MAC_secret[CipherSpec.hash_size   ]
     *      server_write_MAC_secret[CipherSpec.hash_size   ]
     *      client_write_key       [CipherSpec.key_material]
     *      server_write_key       [CipherSpec.key_material]
     *      client_write_IV        [CipherSpec.IV_size     ]
     *      server_write_IV        [CipherSpec.IV_size     ]
     */
    AnscCopyMemory(&random_seed[0                     ], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);
    AnscCopyMemory(&random_seed[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);

    returnStatus =
        pTlsCryptoProvider->Compute_PRF30
            (
                (ANSC_HANDLE)pTlsCryptoProvider,
                pSecurityParams->MasterSecret,
                TLS_MASTER_SECRET_SIZE,
                random_seed,
                TLS_OPAQUE_RANDOM_SIZE * 2,
                pKeyBlock,
                pSecurityParams->KeyBlockSize
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        if ( pSecurityParams->MacHashSize == 0 )
        {
            pMacSecretC->KeyNumber = 0;
            pMacSecretC->Length    = 0;
            pMacSecretS->KeyNumber = 0;
            pMacSecretS->Length    = 0;
        }
        else
        {
            pMacSecretC->KeyNumber = 1;
            pMacSecretC->Length    = pSecurityParams->MacHashSize;
            AnscCopyMemory(pMacSecretC->Value[0], &pKeyBlock[key_block_offset], pSecurityParams->MacHashSize);
            key_block_offset += pSecurityParams->MacHashSize;

            pMacSecretS->KeyNumber = 1;
            pMacSecretS->Length    = pSecurityParams->MacHashSize;
            AnscCopyMemory(pMacSecretS->Value[0], &pKeyBlock[key_block_offset], pSecurityParams->MacHashSize);
            key_block_offset += pSecurityParams->MacHashSize;
        }

        if ( pSecurityParams->CipherKeySize == 0 )
        {
            pCipherKeyC->KeyNumber = 0;
            pCipherKeyC->Length    = 0;
            pCipherKeyS->KeyNumber = 0;
            pCipherKeyS->Length    = 0;
        }
        else
        {
            write_key_length = pSecurityParams->CipherKeyMaterialSize * pSecurityParams->CipherKeyNumber;
            AnscCopyMemory(client_write_key, &pKeyBlock[key_block_offset], write_key_length);

            pCipherKeyC->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyC->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyC->Value[i], &pKeyBlock[key_block_offset], pSecurityParams->CipherKeyMaterialSize);

                key_block_offset += pSecurityParams->CipherKeyMaterialSize;
            }

            AnscCopyMemory(server_write_key, &pKeyBlock[key_block_offset], write_key_length);

            pCipherKeyS->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyS->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyS->Value[i], &pKeyBlock[key_block_offset], pSecurityParams->CipherKeyMaterialSize);

                key_block_offset += pSecurityParams->CipherKeyMaterialSize;
            }
        }

        /*
         * The client_write_IV and server_write_IV are only generated for non-export block ciphers.
         * For exportable block ciphers, the initialization vectors are generated later, as
         * described below. Any extra key_block material is discarded.
         */
        if ( (pSecurityParams->CipherType  == TLS_CIPHER_TYPE_block) &&
             (pSecurityParams->bExportable == FALSE                ) )
        {
            pCipherIV_C->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_C->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;

            pCipherIV_S->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_S->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;
        }
    }

    /*
     * Exportable encryption algorithms (for which CipherSpec.is_exportable is true) require addi-
     * tional processing as follows to derive their final write keys:
     *
     *      final_client_write_key =
     *          MD5(client_write_key + ClientHello.random + ServerHello.random);
     *      final_server_write_key =
     *          MD5(server_write_key + ServerHello.random + ClientHello.random);
     */
    if ( pSecurityParams->bExportable && (pSecurityParams->CipherKeySize > 0) )
    {
        PUCHAR                      hash_data = pKeyBlock;
        ULONG                       data_size = write_key_length + TLS_OPAQUE_RANDOM_SIZE * 2;
        ULONG                       hash_size = ANSC_MD5_OUTPUT_SIZE;

        AnscCopyMemory(&hash_data[0],                                         client_write_key,              write_key_length      );
        AnscCopyMemory(&hash_data[write_key_length],                          pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);
        AnscCopyMemory(&hash_data[write_key_length + TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);

        returnStatus =
            pTlsCryptoProvider->Hash_MD5
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    hash_data,
                    data_size,
                    pKeyBlock,
                    &hash_size
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
        else
        {
            key_block_offset       = 0;
            pCipherKeyC->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyC->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyC->Value[i], &pKeyBlock[key_block_offset], pCipherKeyC->Length);

                key_block_offset += pCipherKeyC->Length;
            }
        }

        AnscCopyMemory(&hash_data[0],                                         server_write_key,              write_key_length      );
        AnscCopyMemory(&hash_data[write_key_length],                          pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);
        AnscCopyMemory(&hash_data[write_key_length + TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);

        returnStatus =
            pTlsCryptoProvider->Hash_MD5
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    hash_data,
                    data_size,
                    pKeyBlock,
                    &hash_size
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
        else
        {
            key_block_offset       = 0;
            pCipherKeyS->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyS->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyS->Value[i], &pKeyBlock[key_block_offset], pCipherKeyS->Length);

                key_block_offset += pCipherKeyS->Length;
            }
        }
    }

    /*
     * Exportable encryption algorithms derive their IVs from the random messages:
     *
     *      client_write_IV = MD5(ClientHello.random + ServerHello.random);
     *      server_write_IV = MD5(ServerHello.random + ClientHello.random);
     *
     * MD5 outputs are trimmed to the appropriate size by discarding the least-significant bytes.
     */
    if ( (pSecurityParams->CipherType   == TLS_CIPHER_TYPE_block) &&
         (pSecurityParams->bExportable  == TRUE                 ) &&
         (pSecurityParams->CipherIVSize != 0                    ) )
    {
        PUCHAR                      hash_data = random_seed;
        ULONG                       data_size = TLS_OPAQUE_RANDOM_SIZE * 2;
        ULONG                       hash_size = ANSC_MD5_OUTPUT_SIZE;

        AnscCopyMemory(&hash_data[0                     ], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);
        AnscCopyMemory(&hash_data[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);

        returnStatus =
            pTlsCryptoProvider->Hash_MD5
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    hash_data,
                    data_size,
                    pKeyBlock,
                    &hash_size
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
        else
        {
            key_block_offset = 0;

            pCipherIV_C->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_C->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;
        }

        AnscCopyMemory(&hash_data[0                     ], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);
        AnscCopyMemory(&hash_data[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);

        returnStatus =
            pTlsCryptoProvider->Hash_MD5
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    hash_data,
                    data_size,
                    pKeyBlock,
                    &hash_size
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
        else
        {
            key_block_offset = 0;

            pCipherIV_S->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_S->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pKeyBlock )
    {
        AnscFreeMemory(pKeyBlock);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoCalKeys31
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to calculate keys, IVs, and MAC secrets
        from the security parameters provided by the handshake protocol.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoCalKeys31
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE         )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS       )&pSessionState->SecurityParams;
    PTLS_RECORD_STATE               pRecordStateW      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateW;
    PTLS_RECORD_STATE               pRecordStateR      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateR;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
#ifdef _64BIT_ARCH_SUPPORT_
    ANSC_CRYPTO_KEY*                pMacSecretC        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->MacSecret : &pRecordStateW->MacSecret;
    ANSC_CRYPTO_KEY*                pMacSecretS        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->MacSecret : &pRecordStateR->MacSecret;
    ANSC_CRYPTO_KEY*                pCipherKeyC        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherKey : &pRecordStateW->CipherKey;
    ANSC_CRYPTO_KEY*                pCipherKeyS        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherKey : &pRecordStateR->CipherKey;
    ANSC_CRYPTO_IV*                 pCipherIV_C        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherIV  : &pRecordStateW->CipherIV;
    ANSC_CRYPTO_IV*                 pCipherIV_S        = (ANSC_CRYPTO_KEY*           )(ULONG)(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherIV  : &pRecordStateR->CipherIV;
#else
    ANSC_CRYPTO_KEY*                pMacSecretC        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->MacSecret : &pRecordStateW->MacSecret;
    ANSC_CRYPTO_KEY*                pMacSecretS        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->MacSecret : &pRecordStateR->MacSecret;
    ANSC_CRYPTO_KEY*                pCipherKeyC        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherKey : &pRecordStateW->CipherKey;
    ANSC_CRYPTO_KEY*                pCipherKeyS        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherKey : &pRecordStateR->CipherKey;
    ANSC_CRYPTO_IV*                 pCipherIV_C        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateR->CipherIV  : &pRecordStateW->CipherIV;
    ANSC_CRYPTO_IV*                 pCipherIV_S        = (ANSC_CRYPTO_KEY*           )(pSecurityParams->ConnectionEnd == TLS_CONNECTION_END_server)? &pRecordStateW->CipherIV  : &pRecordStateR->CipherIV;
#endif
    PUCHAR                          pKeyBlock          = (PUCHAR                     )NULL;
    ULONG                           write_key_length   = 0;
    ULONG                           key_block_offset   = 0;
    ULONG                           i                  = 0;
    UCHAR                           random_seed     [64 ];
    UCHAR                           client_write_key[128];
    UCHAR                           server_write_key[128];

    if ( pSecurityParams->KeyBlockSize == 0 )
    {
        pMacSecretC->KeyNumber = 0;
        pMacSecretC->Length    = 0;
        pMacSecretS->KeyNumber = 0;
        pMacSecretS->Length    = 0;

        pCipherKeyC->KeyNumber = 0;
        pCipherKeyC->Length    = 0;
        pCipherKeyS->KeyNumber = 0;
        pCipherKeyS->Length    = 0;

        pCipherIV_C->Length    = 0;
        pCipherIV_S->Length    = 0;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pKeyBlock = (PUCHAR)AnscAllocateMemory(pSecurityParams->KeyBlockSize + 256);

        if ( !pKeyBlock )
        {
            return  ANSC_STATUS_RESOURCES;
        }
    }

    /*
     * When generating keys and MAC secrets, the master secret is used as an entropy source, and
     * the random values provide unencrypted salt material and IVs for exportable ciphers. To
     * generate the key material, compute
     *
     *      key_block = PRF(SecurityParameters.master_secret,
     *                      "key expansion",
     *                      SecurityParameters.server_random + SecurityParameters.client_random);
     *
     * until enough output has been generated. Then the key_block is partitioned as follows:
     *
     *      client_write_MAC_secret[SecurityParameters.hash_size          ]
     *      server_write_MAC_secret[SecurityParameters.hash_size          ]
     *      client_write_key       [SecurityParameters.key_material_length]
     *      server_write_key       [SecurityParameters.key_material_length]
     *      client_write_IV        [SecurityParameters.IV_size            ]
     *      server_write_IV        [SecurityParameters.IV_size            ]
     */
    AnscCopyMemory(&random_seed[0                     ], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);
    AnscCopyMemory(&random_seed[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);

    returnStatus =
        pTlsCryptoProvider->Compute_PRF31
            (
                (ANSC_HANDLE)pTlsCryptoProvider,
                pSecurityParams->MasterSecret,
                TLS_MASTER_SECRET_SIZE,
                "key expansion",
                random_seed,
                TLS_OPAQUE_RANDOM_SIZE * 2,
                pKeyBlock,
                pSecurityParams->KeyBlockSize
            );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT1;
    }
    else
    {
        if ( pSecurityParams->MacHashSize == 0 )
        {
            pMacSecretC->KeyNumber = 0;
            pMacSecretC->Length    = 0;
            pMacSecretS->KeyNumber = 0;
            pMacSecretS->Length    = 0;
        }
        else
        {
            pMacSecretC->KeyNumber = 1;
            pMacSecretC->Length    = pSecurityParams->MacHashSize;
            AnscCopyMemory(pMacSecretC->Value[0], &pKeyBlock[key_block_offset], pSecurityParams->MacHashSize);
            key_block_offset += pSecurityParams->MacHashSize;

            pMacSecretS->KeyNumber = 1;
            pMacSecretS->Length    = pSecurityParams->MacHashSize;
            AnscCopyMemory(pMacSecretS->Value[0], &pKeyBlock[key_block_offset], pSecurityParams->MacHashSize);
            key_block_offset += pSecurityParams->MacHashSize;
        }

        if ( pSecurityParams->CipherKeySize == 0 )
        {
            pCipherKeyC->KeyNumber = 0;
            pCipherKeyC->Length    = 0;
            pCipherKeyS->KeyNumber = 0;
            pCipherKeyS->Length    = 0;
        }
        else
        {
            write_key_length = pSecurityParams->CipherKeyMaterialSize * pSecurityParams->CipherKeyNumber;
            AnscCopyMemory(client_write_key, &pKeyBlock[key_block_offset], write_key_length);

            pCipherKeyC->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyC->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyC->Value[i], &pKeyBlock[key_block_offset], pSecurityParams->CipherKeyMaterialSize);

                key_block_offset += pSecurityParams->CipherKeyMaterialSize;
            }

            AnscCopyMemory(server_write_key, &pKeyBlock[key_block_offset], write_key_length);

            pCipherKeyS->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyS->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyS->Value[i], &pKeyBlock[key_block_offset], pSecurityParams->CipherKeyMaterialSize);

                key_block_offset += pSecurityParams->CipherKeyMaterialSize;
            }
        }

        /*
         * The client_write_IV and server_write_IV are only generated for non-export block ciphers.
         * For exportable block ciphers, the initialization vectors are generated later, as
         * described below. Any extra key_block material is discarded.
         */
        if ( (pSecurityParams->CipherType  == TLS_CIPHER_TYPE_block) &&
             (pSecurityParams->bExportable == FALSE                ) )
        {
            pCipherIV_C->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_C->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;

            pCipherIV_S->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_S->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;
        }
    }

    /*
     * Exportable encryption algorithms (for which CipherSpec.is_exportable is true) require addi-
     * tional processing as follows to derive their final write keys:
     *
     *      final_client_write_key =
     *          PRF(SecurityParameters.client_write_key,
     *              "client write key",
     *              SecurityParameters.client_random + SecurityParameters.server_random);
     *
     *      final_server_write_key =
     *          PRF(SecurityParameters.server_write_key,
     *              "server write key",
     *              SecurityParameters.client_random + SecurityParameters.server_random);
     */
    if ( pSecurityParams->bExportable && (pSecurityParams->CipherKeySize > 0) )
    {
        AnscCopyMemory(&random_seed[0                     ], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);
        AnscCopyMemory(&random_seed[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);

        returnStatus =
            pTlsCryptoProvider->Compute_PRF31
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    client_write_key,
                    write_key_length,
                    "client write key",
                    random_seed,
                    TLS_OPAQUE_RANDOM_SIZE * 2,
                    pKeyBlock,
                    pSecurityParams->CipherKeySize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
        else
        {
            key_block_offset       = 0;
            pCipherKeyC->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyC->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyC->Value[i], &pKeyBlock[key_block_offset], pCipherKeyC->Length);

                key_block_offset += pCipherKeyC->Length;
            }
        }

        returnStatus =
            pTlsCryptoProvider->Compute_PRF31
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    server_write_key,
                    write_key_length,
                    "server write key",
                    random_seed,
                    TLS_OPAQUE_RANDOM_SIZE * 2,
                    pKeyBlock,
                    pSecurityParams->CipherKeySize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
        else
        {
            key_block_offset       = 0;
            pCipherKeyS->KeyNumber = pSecurityParams->CipherKeyNumber;
            pCipherKeyS->Length    = pSecurityParams->CipherKeySize;

            for ( i = 0; i < pSecurityParams->CipherKeyNumber; i++ )
            {
                AnscCopyMemory(pCipherKeyS->Value[i], &pKeyBlock[key_block_offset], pCipherKeyS->Length);

                key_block_offset += pCipherKeyS->Length;
            }
        }
    }

    /*
     * Exportable encryption algorithms derive their IVs solely from the random values from the
     * hello messages:
     *
     *      iv_block = PRF("",
     *                     "IV block",
     *                     SecurityParameters.client_random + SecurityParameters.server_random);
     *
     * The iv_block is partitioned into two initialization vectors as the key_block was above:
     *
     *      client_write_IV[SecurityParameters.IV_size]
     *      server_write_IV[SecurityParameters.IV_size]
     *
     * Note that the PRF is used without a secret in this case: this just means that the secret has
     * a length of zero bytes and contributes nothing to the hashing in the PRF.
     */
    if ( (pSecurityParams->CipherType   == TLS_CIPHER_TYPE_block) &&
         (pSecurityParams->bExportable  == TRUE                 ) &&
         (pSecurityParams->CipherIVSize != 0                    ) )
    {
        AnscCopyMemory(&random_seed[0                     ], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);
        AnscCopyMemory(&random_seed[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);

        returnStatus =
            pTlsCryptoProvider->Compute_PRF31
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    NULL,
                    0,
                    "IV block",
                    random_seed,
                    TLS_OPAQUE_RANDOM_SIZE * 2,
                    pKeyBlock,
                    pSecurityParams->CipherIVSize * 2
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            goto  EXIT1;
        }
        else
        {
            key_block_offset = 0;

            pCipherIV_C->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_C->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;

            pCipherIV_S->Length = pSecurityParams->CipherIVSize;
            AnscCopyMemory(pCipherIV_S->Value, &pKeyBlock[key_block_offset], pSecurityParams->CipherIVSize);
            key_block_offset += pSecurityParams->CipherIVSize;
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pKeyBlock )
    {
        AnscFreeMemory(pKeyBlock);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoCalMasterSecret30
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to convert the pre_master_secret into
        the master_secret.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoCalMasterSecret30
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE         )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS       )&pSessionState->SecurityParams;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    UCHAR                           random_seed[64];

    /*
     * The asymmetric algorithms are used in the handshake protocol to authenticate parties and to
     * generate shared keys and secrets. For Diffie-Hellman, RSA, and FORTEZZA, the same algorithm
     * is used to convert the pre_master_secret into the master_secret. The pre_master_secret
     * should be deleted from memory once the master_secret has been computed.
     *
     *      master_secret =
     *          MD5(pre_master_secret + SHA('A'   + pre_master_secret + ClientHello.random + ServerHello.random)) +
     *          MD5(pre_master_secret + SHA('BB'  + pre_master_secret + ClientHello.random + ServerHello.random)) +
     *          MD5(pre_master_secret + SHA('CCC' + pre_master_secret + ClientHello.random + ServerHello.random));
     */
    AnscCopyMemory(&random_seed[0                     ], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);
    AnscCopyMemory(&random_seed[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);

    returnStatus =
        pTlsCryptoProvider->Compute_PRF30
            (
                (ANSC_HANDLE)pTlsCryptoProvider,
                pSecurityParams->PreMasterSecret,
                pSecurityParams->PreMasterSecretSize,
                random_seed,
                TLS_OPAQUE_RANDOM_SIZE * 2,
                pSecurityParams->MasterSecret,
                TLS_MASTER_SECRET_SIZE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoCalMasterSecret31
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to convert the pre_master_secret into
        the master_secret.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoCalMasterSecret31
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_SESSION_STATE              pSessionState      = (PTLS_SESSION_STATE         )&pMyObject->SessionState;
    PTLS_SECURITY_PARAMS            pSecurityParams    = (PTLS_SECURITY_PARAMS       )&pSessionState->SecurityParams;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    UCHAR                           random_seed[64];

    /*
     * For all key exchange methods, the same algorithm is used to convert the pre_master_secret
     * into the master_secret. The pre_master_secret should be deleted from memory once the
     * master_secret has been computed.
     *
     *      master_secret = PRF(pre_master_secret,
     *                          "master secret",
     *                          ClientHello.random + ServerHello.random) [0..47];
     *
     * The master secret is always exactly 48 bytes in length. The length of the premaster secret
     * will vary depending on key exchange method.
     */
    AnscCopyMemory(&random_seed[0                     ], pSecurityParams->ClientRandom, TLS_OPAQUE_RANDOM_SIZE);
    AnscCopyMemory(&random_seed[TLS_OPAQUE_RANDOM_SIZE], pSecurityParams->ServerRandom, TLS_OPAQUE_RANDOM_SIZE);

    returnStatus =
        pTlsCryptoProvider->Compute_PRF31
            (
                (ANSC_HANDLE)pTlsCryptoProvider,
                pSecurityParams->PreMasterSecret,
                pSecurityParams->PreMasterSecretSize,
                "master secret",
                random_seed,
                TLS_OPAQUE_RANDOM_SIZE * 2,
                pSecurityParams->MasterSecret,
                TLS_MASTER_SECRET_SIZE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoChangeCipherW
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to instruct the Record Layer to
        immeidately copy the write pending state into the write current
        state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoChangeCipherW
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_RECORD_STATE               pRecordStateW      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateW;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT  )pMyObject->hTlsRecordKeeper;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    
    returnStatus = pTlsCbcIf->ChangeCipherSpec(pTlsCbcIf->hOwnerContext);
    returnStatus =
        pTlsRecordKeeper->ChangeStateW
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                (ANSC_HANDLE)pRecordStateW
            );

    pMyObject->ProgressBits |= TLS_HSO_PROGRESS_BIT_changeCipherW;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsHsoChangeCipherR
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to instruct the Record Layer to
        immeidately copy the read pending state into the read current
        state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsHsoChangeCipherR
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_HAND_SHAKER_OBJECT         pMyObject          = (PTLS_HAND_SHAKER_OBJECT    )hThisObject;
    PTLS_RECORD_STATE               pRecordStateR      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateR;
    PTLS_RECORD_KEEPER_OBJECT       pTlsRecordKeeper   = (PTLS_RECORD_KEEPER_OBJECT  )pMyObject->hTlsRecordKeeper;
    
    returnStatus =
        pTlsRecordKeeper->ChangeStateR
            (
                (ANSC_HANDLE)pTlsRecordKeeper,
                (ANSC_HANDLE)pRecordStateR
            );

    pMyObject->ProgressBits |= TLS_HSO_PROGRESS_BIT_changeCipherR;

    return  returnStatus;
}
