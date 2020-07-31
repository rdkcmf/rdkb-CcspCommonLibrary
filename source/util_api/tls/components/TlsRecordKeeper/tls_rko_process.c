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

    module:	tls_rko_process.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced process functions
        of the TLS Record Keeper Object.

        *   TlsRkoRecv
        *   TlsRkoSend

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/27/03    initial revision.

**********************************************************************/


#include "tls_rko_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoRecv
            (
                ANSC_HANDLE                 hThisObject,
                void*                       buffer,
                ULONG                       ulSize,
                ANSC_HANDLE                 hReserved
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

                ANSC_HANDLE                 hReserved
                Specifies the message-specific processing context.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsRkoRecv
    (
        ANSC_HANDLE                 hThisObject,
        void*                       buffer,
        ULONG                       ulSize,
        ANSC_HANDLE                 hReserved
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_RECORD_KEEPER_OBJECT       pMyObject          = (PTLS_RECORD_KEEPER_OBJECT  )hThisObject;
    PTLS_RECORD_STATE               pRecordStateR      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateR;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_RECORD_HEADER              pTlsRecordHeader1  = (PTLS_RECORD_HEADER         )buffer;
    PTLS_RECORD_HEADER              pTlsRecordHeader2  = (PTLS_RECORD_HEADER         )NULL;
    PTLS_RECORD_CLIENT_OBJECT       pTlsRecordClient   = (PTLS_RECORD_CLIENT_OBJECT  )NULL;
    BOOL                            bValidTlsRecord    = (BOOL                       )TRUE;
    void*                           pTlsSeqNumber      = (void*                      )NULL;
    void*                           pTlsSeqNumberH     = (void*                      )NULL;
    void*                           pTlsSeqNumberL     = (void*                      )NULL;
    PUCHAR                          pSslPad1           = (PUCHAR                     )NULL;
    PUCHAR                          pSslMacReadSecret  = (PUCHAR                     )NULL;
    ULONG                           ulSslPadSize       = (ULONG                      )0;
    ULONG                           ulSslMacShiftSize  = (ULONG                      )0;
    UCHAR                           ucSslPad1Octet     = (UCHAR                      )0x36;
    UCHAR                           ucSslPad2Octet     = (UCHAR                      )0x5c;
    ULONG                           ulHashDataSize     = (ULONG                      )0;
    void*                           pRecordPayload     = (void*                      )NULL;
    void*                           pRecordChunk       = (void*                      )NULL;
    ULONG                           ulChunkBufSize     = (ULONG                      )0;
    ULONG                           ulChunkSize        = (ULONG                      )TLS_MAX_RECORD_CHUNK_SIZE;
    ULONG                           ulNewChunkSize     = (ULONG                      )TLS_MAX_RECORD_CHUNK_SIZE;
    ULONG                           ulMacHashSize      = (ULONG                      )0;
    ULONG                           ulPaddingLength    = (ULONG                      )0;
    PUCHAR                          pHashResult        = (PUCHAR                     )NULL;
    PUCHAR                          pPaddings          = (PUCHAR                     )NULL;
    ULONG                           j                  = 0;
    UCHAR                           temp_buffer[128];

    TlsRecordValidate(pTlsRecordHeader1, ulSize, bValidTlsRecord);

    if ( !bValidTlsRecord )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        return  returnStatus;
    }
    else
    {
        ulChunkSize = TlsRecordGetLength(pTlsRecordHeader1);
    }

    ulChunkBufSize = (pRecordStateR->CompressionAlgorithm != TLS_COMPRESSION_ALGORITHM_null)? TLS_MAX_RECORD_CHUNK_SIZE + 256 + 128 : ulSize + 256 + 128;
    pRecordChunk   = AnscAllocateMemory(ulChunkBufSize);

    if ( !pRecordChunk )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pTlsRecordHeader2 = (PTLS_RECORD_HEADER)((ULONG)pRecordChunk      + 256);
        pTlsSeqNumber     = (void*             )((ULONG)pTlsRecordHeader2 - 8  );
        pTlsSeqNumberH    = (void*             )((ULONG)pTlsSeqNumber          );
        pTlsSeqNumberL    = (void*             )((ULONG)pTlsSeqNumber     + 4  );

        /*
         * SSL 3.0 specifies a different MAC-generation algorithm. The MAC in SSL 3.0 is generated
         * as following:
         *
         *      hash(MAC_write_secret + pad_2 +
         *           hash(MAC_write_secret + pad_1 + seq_num + SSLCompressed.type +
         *                SSLCompressed.length + SSLCompressed.fragment));
         *
         * where "+" denotes concatenation.
         *
         *      pad_1       The character 0x36 repeated 48 times for MD5 or 40 times for SHA.
         *      pad_2       The character 0x5c repeated 48 times for MD5 or 40 times for SHA.
         *      seq_num     The sequence number for this message.
         *      hash        Hashing algorithm derived from the cipher suite.
         */
        if ( pRecordStateR->MinorVersion == 0 )
        {
            switch ( pRecordStateR->MacAlgorithm )
            {
                case    TLS_MAC_ALGORITHM_md5 :

                        pSslPad1     = (PUCHAR)((ULONG)pTlsSeqNumber - 48);
                        ulSslPadSize = 48;

                        break;

                case    TLS_MAC_ALGORITHM_sha :

                        pSslPad1     = (PUCHAR)((ULONG)pTlsSeqNumber - 40);
                        ulSslPadSize = 40;

                        break;

                default :

                        pSslPad1     = (PUCHAR)((ULONG)pTlsSeqNumber - 48);
                        ulSslPadSize = 48;

                        break;
            }

            pSslMacReadSecret = (PUCHAR)((ULONG)pSslPad1 - pRecordStateR->MacSecret.Length);

            /*
             * As you can see from the above SSL 3.0 MAC definition, the designers have left out
             * the 'SSLCompressed.version' field in the hash calcuation! I wonder whether they did
             * it intentionally. Either way, what a bunch of morons! This leaves us no choice but
             * to shift the data around.
             */
            ulSslMacShiftSize  = (ULONG)pTlsRecordHeader2 - (ULONG)pSslMacReadSecret;
            ulSslMacShiftSize += 3;     /* SSLCompressed.type + SSLCompressed.version */
        }
    }

    /*
     * The encryption and MAC functions translate a TLSCompressed structure into a
     * TLSCiphertext. The decryption functions reverse the process. The MAC of the record also
     * includes a sequence number so that missing, extra or repeated messages are detectable.
     *
     *      struct {
     *          ContentType         type;
     *          ProtocolVersion     version;
     *          uint16              length;
     *          select (CipherSpec.cipher_type) {
     *              case stream:    GenericStreamCipher;
     *              case block:     GenericBlockCipher;
     *          } fragment;
     *      } TLSCiphertext;
     */
    if ( (pRecordStateR->BulkEncryptionAlgorithm != TLS_BULK_CIPHER_ALGORITHM_null) &&
         (ulChunkSize                            >  0                             ) )
    {
        returnStatus  =
            pTlsCryptoProvider->Decrypt
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pRecordStateR->BulkEncryptionAlgorithm,
                    &pRecordStateR->CipherKey,
                    &pRecordStateR->CipherIV,
                    pTlsRecordHeader1->Fragment,
                    ulChunkSize,
                    pTlsRecordHeader1->Fragment,
                    &ulChunkSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decryptionFailed
                    );

            goto  EXIT1;
        }

        switch ( pRecordStateR->CipherType )
        {
            case    TLS_CIPHER_TYPE_stream :

                    /*
                     * Stream ciphers (including BulkCipherAlgorithm.null - see Appendix A.6)
                     * convert TLSCompressed.fragment structures to and from stream
                     * TLSCiphertext.fragment structures.
                     *
                     *      stream-ciphered struct {
                     *          opaque  content[TLSCompressed.length];
                     *          opaque  MAC[CipherSpec.hash_size];
                     *      } GenericStreamCipher;
                     */

                    break;

            case    TLS_CIPHER_TYPE_block :

                    /*
                     * For block ciphers (such as RC2 or DES), the encryption and MAC functions
                     * convert TLSCompressed.fragment structures to and from block
                     * TLSCiphertext.fragment structures.
                     *
                     *      block-ciphered struct {
                     *          opaque  content[TLSCompressed.length             ];
                     *          opaque  MAC    [CipherSpec.hash_size             ];
                     *          uint8   padding[GenericBlockCipher.padding_length];
                     *          uint8   padding_length;
                     *      } GenericBlockCipher;
                     */
                    ulPaddingLength = *(PUCHAR)((ULONG)TlsRecordGetEndOfFragment(pTlsRecordHeader1) - 1);
                    pPaddings       =  (PUCHAR)((ULONG)TlsRecordGetEndOfFragment(pTlsRecordHeader1) - 1 - ulPaddingLength);

                    if ( (ulPaddingLength + 1) > ulChunkSize )
                    {
                        returnStatus =
                            pTlsCbcIf->GenerateAlert
                                (
                                    pTlsCbcIf->hOwnerContext,
                                    TLS_ALERT_LEVEL_fatal,
                                    TLS_ALERT_DESCR_decryptionFailed
                                );

                        goto  EXIT1;
                    }

                    if ( pRecordStateR->MinorVersion == 1 )
                    {
                        for ( j = 0; j < ulPaddingLength + 1; j++ )
                        {
                            if ( pPaddings[j] != (UCHAR)ulPaddingLength )
                            {
                                returnStatus =
                                    pTlsCbcIf->GenerateAlert
                                        (
                                            pTlsCbcIf->hOwnerContext,
                                            TLS_ALERT_LEVEL_fatal,
                                            TLS_ALERT_DESCR_decryptionFailed
                                        );

                                goto  EXIT1;
                            }
                        }
                    }

                    ulChunkSize -= ulPaddingLength + 1;

                    TlsRecordSetLength(pTlsRecordHeader1, (USHORT)ulChunkSize);

                    break;

            default :

                    break;
        }
    }

    /*
     * The MAC is generated as:
     *
     *      HMAC_hash(MAC_write_secret,
     *                seq_num + TLSCompressed.type + TLSCompressed.version +
     *                TLSCompressed.length + TLSCompressed.fragment));
     *
     * where "+" denotes concatenation.
     *
     *      seq_num
     *          The sequence number for this record.
     *      hash
     *          The hashing algorithm specified by SecurityParameters.mac_algorithm.
     *
     * Note that the MAC is computed before encryption. The stream cipher encrypts the entire
     * block, including the MAC. For stream ciphers that do not use a synchronization vector
     * (such as RC4), the stream cipher state from the end of one record is simply used on the
     * subsequent packet. If the CipherSuite is TLS_NULL_WITH_NULL_NULL, encryption consists of
     * the identity operation (i.e., the data is not encrypted and the MAC size is zero
     * implying that no MAC is used).
     */
    if ( pRecordStateR->MacAlgorithm != TLS_MAC_ALGORITHM_null )
    {
        if ( ulChunkSize < pRecordStateR->MacHashSize )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_badRecordMac
                    );

            goto  EXIT1;
        }
        else
        {
            AnscCopyMemory
                (
                    pTlsRecordHeader2,
                    pTlsRecordHeader1,
                    TlsRecordGetSize(pTlsRecordHeader1)
                );

            TlsRecordSetLength(pTlsRecordHeader2, (USHORT)(ulChunkSize - pRecordStateR->MacHashSize));
        }

        if ( TRUE )
        {
            AnscWriteUlong(pTlsSeqNumberH, AnscUlongFromHToN(pRecordStateR->SeqNumber.HiPart));
            AnscWriteUlong(pTlsSeqNumberL, AnscUlongFromHToN(pRecordStateR->SeqNumber.LoPart));
        }

        if ( pRecordStateR->MinorVersion == 1 )
        {
            pHashResult    = temp_buffer;
            ulHashDataSize = TlsRecordGetSize(pTlsRecordHeader2) + 8;
            ulMacHashSize  = pRecordStateR->MacHashSize;
            returnStatus   =
                pTlsCryptoProvider->Hmac
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pRecordStateR->MacAlgorithm,
                        pRecordStateR->MacSecret.Value[0],
                        pRecordStateR->MacSecret.Length,
                        pTlsSeqNumber,
                        ulHashDataSize,
                        pHashResult,
                        &ulMacHashSize
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                returnStatus =
                    pTlsCbcIf->GenerateAlert
                        (
                            pTlsCbcIf->hOwnerContext,
                            TLS_ALERT_LEVEL_fatal,
                            TLS_ALERT_DESCR_badRecordMac
                        );

                goto  EXIT1;
            }
        }
        else
        {
            /*
             * SSL 3.0 specifies a different MAC-generation algorithm. The MAC in SSL 3.0 is
             * generated as following:
             *
             *      hash(MAC_write_secret + pad_2 +
             *           hash(MAC_write_secret + pad_1 + seq_num + SSLCompressed.type +
             *                SSLCompressed.length + SSLCompressed.fragment));
             *
             * where "+" denotes concatenation.
             *
             *      pad_1       The character 0x36 repeated 48 times for MD5 or 40
             *                  times for SHA.
             *      pad_2       The character 0x5c repeated 48 times for MD5 or 40
             *                  times for SHA.
             *      seq_num     The sequence number for this message.
             *      hash        Hashing algorithm derived from the cipher suite.
             */
            if ( TRUE )
            {
                for ( j = 0; j < ulSslPadSize; j++ )
                {
                    pSslPad1[j] = ucSslPad1Octet;
                }

                AnscCopyMemory
                    (
                        pSslMacReadSecret,
                        pRecordStateR->MacSecret.Value[0],
                        pRecordStateR->MacSecret.Length
                    );
            }

            if ( TRUE )
            {
                ulHashDataSize = (ULONG)pTlsRecordHeader2 - (ULONG)pSslMacReadSecret + TlsRecordGetSize(pTlsRecordHeader2) - 2;

                /* RDKB-6311, CID-24223, defining upper and lower boundary 
                ** If pSslMacWriteSecret larger than Chunk size or less than 2(result in wrap around of the variable value, on subtracting 2)
                ** leads to out of bound memory access.
                */
                if((ulChunkBufSize > ulSslMacShiftSize) && (ulSslMacShiftSize > 2))
                {
                    AnscCopyMemory(pRecordChunk,          pSslMacReadSecret, ulSslMacShiftSize    );
                    AnscCopyMemory(pSslMacReadSecret + 2, pRecordChunk,      ulSslMacShiftSize - 2);
                }
                else
                {
                    returnStatus = ANSC_STATUS_FAILURE;
                    goto  EXIT1;
                }

                pHashResult   = temp_buffer + pRecordStateR->MacSecret.Length + ulSslPadSize;
                ulMacHashSize = pRecordStateR->MacHashSize;
                returnStatus  =
                    pTlsCryptoProvider->Hash
                        (
                            (ANSC_HANDLE)pTlsCryptoProvider,
                            pRecordStateR->MacAlgorithm,
                            pSslMacReadSecret + 2,
                            ulHashDataSize,
                            pHashResult,
                            &ulMacHashSize
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_badRecordMac
                            );

                    goto  EXIT1;
                }
                else
                {
                    /*
                     * Now let's restore the original header fields ... Actually we don't have
                     * to copy the whole 'shift block' back, we only need to restore the
                     * 'SSLCompressed.type' and 'SSLCompressed.version' fields. But we do it
                     * anyway for the sake of completeness.
                     */
                    AnscCopyMemory(pSslMacReadSecret, pRecordChunk, ulSslMacShiftSize);
                }
            }

            if ( TRUE )
            {
                AnscCopyMemory
                    (
                        temp_buffer,
                        pRecordStateR->MacSecret.Value[0],
                        pRecordStateR->MacSecret.Length
                    );

                for ( j = 0; j < ulSslPadSize; j++ )
                {
                    temp_buffer[pRecordStateR->MacSecret.Length + j] = ucSslPad2Octet;
                }

                pHashResult    = temp_buffer;
                ulHashDataSize = pRecordStateR->MacSecret.Length + ulSslPadSize + ulMacHashSize;
                ulMacHashSize  = pRecordStateR->MacHashSize;
                returnStatus   =
                    pTlsCryptoProvider->Hash
                        (
                            (ANSC_HANDLE)pTlsCryptoProvider,
                            pRecordStateR->MacAlgorithm,
                            temp_buffer,
                            ulHashDataSize,
                            pHashResult,
                            &ulMacHashSize
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    returnStatus =
                        pTlsCbcIf->GenerateAlert
                            (
                                pTlsCbcIf->hOwnerContext,
                                TLS_ALERT_LEVEL_fatal,
                                TLS_ALERT_DESCR_badRecordMac
                            );

                    goto  EXIT1;
                }
            }
        }

        /*
         * We have computed the MAC independently, now it's time for the moment of truth: we need
         * to compare the MAC we have calculated to the MAC hash value contained in the received
         * message. If these two don't match, we MUST generate a 'bad_record_mac' alert.
         */
        if ( !AnscEqualMemory
                (
                    (PVOID)((ULONG)TlsRecordGetEndOfFragment(pTlsRecordHeader1) - pRecordStateR->MacHashSize),
                    pHashResult,
                    pRecordStateR->MacHashSize
                ) )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_badRecordMac
                    );

            goto  EXIT1;
        }
        else
        {
            ulChunkSize -= pRecordStateR->MacHashSize;

            TlsRecordSetLength(pTlsRecordHeader1, (USHORT)ulChunkSize);
        }
    }

    /*
     * All records are compressed using the compression algorithm defined in the current
     * session state. There is always an active compression algorithm; however, initially it is
     * defined as CompressionMethod.null. The compression algorithm translates a TLSPlaintext
     * structure into a TLSCompressed structure. Compression functions are initialized with
     * default state information whenever a connection state is made active.
     *
     * Compression must be lossless and may not increase the content length by more than 1024
     * bytes. If the decompression function encounters a TLSCompressed.fragment that would
     * decompress to a length in excess of 2^14 bytes, it should report a fatal decompression
     * failure error.
     *
     *      struct {
     *          ContentType         type;
     *          ProtocolVersion     version;
     *          uint16              length;
     *          opaque              fragment[TLSCompressed.length];
     *      } TLSCompressed;
     */
    if ( (pRecordStateR->CompressionAlgorithm != TLS_COMPRESSION_ALGORITHM_null) &&
         (ulChunkSize                         >  0                             ) )
    {
        ulNewChunkSize = TLS_RECORD_MAX_FRAGMENT_SIZE_COMPRESSED;
        returnStatus   =
            pTlsCryptoProvider->Decompress
                (
                    (ANSC_HANDLE)pTlsCryptoProvider,
                    pRecordStateR->CompressionAlgorithm,
                    pTlsRecordHeader1->Fragment,
                    ulChunkSize,
                    pTlsRecordHeader2->Fragment,
                    &ulNewChunkSize
                );

        if ( (returnStatus   != ANSC_STATUS_SUCCESS                    ) ||
             (ulNewChunkSize >  TLS_RECORD_MAX_FRAGMENT_SIZE_COMPRESSED) )
        {
            returnStatus =
                pTlsCbcIf->GenerateAlert
                    (
                        pTlsCbcIf->hOwnerContext,
                        TLS_ALERT_LEVEL_fatal,
                        TLS_ALERT_DESCR_decompressionFailure
                    );

            returnStatus = ANSC_STATUS_FAILURE;

            goto  EXIT1;
        }
        else
        {
            pRecordPayload = pTlsRecordHeader2->Fragment;
            ulChunkSize    = ulNewChunkSize;
        }
    }
    else
    {
        pRecordPayload = pTlsRecordHeader1->Fragment;
    }

    AnscIncUint64((&pRecordStateR->SeqNumber));

    /*
     * Four record protocol clients are specified in RFC 2246: the handshake protocol, the alert
     * protocol, the change cipher spec protocol, and the application data protocol. In order to
     * allow extension of the TLS protocol, additional record types can be supported by the record
     * protocol. The TLS_CBC interface centralizes the connection control, including exception
     * handling, session management, and coordination.
     */
    pTlsRecordClient =
        (PTLS_RECORD_CLIENT_OBJECT)pMyObject->GetRecordClient
            (
                (ANSC_HANDLE)pMyObject,
                TlsRecordGetContentType(pTlsRecordHeader1)
            );

    if ( !pTlsRecordClient )
    {
        returnStatus =
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_unexpectedMessage
                );

        goto  EXIT1;
    }
    else
    {
        returnStatus =
            pTlsRecordClient->Recv
                (
                    (ANSC_HANDLE)pTlsRecordClient,
                    pRecordPayload,
                    ulChunkSize,
                    hReserved
                );
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pRecordChunk )
    {
        AnscFreeMemory(pRecordChunk);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsRkoSend
            (
                ANSC_HANDLE                 hThisObject,
                UCHAR                       record_type,
                void*                       buffer,
                ULONG                       ulSize
            );

    description:

        This function is called to process an outgoing message.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                UCHAR                       record_type
                Specifies the record type of the message.

                void*                       buffer
                Specifies the outgoing message to be processed.

                ULONG                       ulSize
                Specifies the size of the message to be processed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsRkoSend
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       record_type,
        void*                       buffer,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PTLS_RECORD_KEEPER_OBJECT       pMyObject          = (PTLS_RECORD_KEEPER_OBJECT  )hThisObject;
    PTLS_RECORD_STATE               pRecordStateW      = (PTLS_RECORD_STATE          )&pMyObject->RecordStateW;
    PTLS_CBC_INTERFACE              pTlsCbcIf          = (PTLS_CBC_INTERFACE         )pMyObject->hTlsCbcIf;
    PTLS_MEC_INTERFACE              pTlsMecIf          = (PTLS_MEC_INTERFACE         )pMyObject->hTlsMecIf;
    PTLS_TSA_INTERFACE              pTlsTsaIf          = (PTLS_TSA_INTERFACE         )pMyObject->hTlsTsaIf;
    PTLS_CRYPTO_PROVIDER_OBJECT     pTlsCryptoProvider = (PTLS_CRYPTO_PROVIDER_OBJECT)pTlsMecIf->GetTlsCryptoProvider(pTlsMecIf->hOwnerContext);
    PTLS_RECORD_HEADER              pTlsRecordHeader   = (PTLS_RECORD_HEADER         )NULL;
    void*                           pTlsSeqNumber      = (void*                      )NULL;
    void*                           pTlsSeqNumberH     = (void*                      )NULL;
    void*                           pTlsSeqNumberL     = (void*                      )NULL;
    PUCHAR                          pSslPad1           = (PUCHAR                     )NULL;
    PUCHAR                          pSslMacWriteSecret = (PUCHAR                     )NULL;
    ULONG                           ulSslPadSize       = (ULONG                      )0;
    ULONG                           ulSslMacShiftSize  = (ULONG                      )0;
    UCHAR                           ucSslPad1Octet     = (UCHAR                      )0x36;
    UCHAR                           ucSslPad2Octet     = (UCHAR                      )0x5c;
    ULONG                           ulHashDataSize     = (ULONG                      )0;
    void*                           pRecordChunk       = (void*                      )NULL;
    ULONG                           ulChunkBufSize     = (ULONG                      )0;
    ULONG                           ulChunkSize        = (ULONG                      )TLS_MAX_RECORD_CHUNK_SIZE;
    ULONG                           ulChunkCount       = (ULONG                      )0;
    void*                           pCurChunk          = (void*                      )buffer;
    ULONG                           ulMacHashSize      = (ULONG                      )0;
    ULONG                           ulPaddingLength    = (ULONG                      )0;
    PUCHAR                          pHashResult        = (PUCHAR                     )NULL;
    PUCHAR                          pPaddings          = (PUCHAR                     )NULL;
    PANSC_BUFFER_DESCRIPTOR         pTbpBdo            = (PANSC_BUFFER_DESCRIPTOR    )NULL;
    BOOL                            bSocketError       = FALSE;
    ULONG                           i                  = 0;
    ULONG                           j                  = 0;
    UCHAR                           temp_buffer[128];

    /*
     * The record layer fragments information blocks into TLSPlaintext records carrying data in
     * chunks of 2^14 bytes or less. Client message boundaries are not preserved in the record
     * layer (i.e., multiple client messages of the same ContentType may be coalesced into a single
     * TLSPlaintext record, or a single message may be fragmented across several records).
     */
    if ( ulSize == 0 )
    {
        ulChunkCount = 1;
    }
    else
    {
        if ( (ulSize % TLS_MAX_RECORD_FRAGMENT_SIZE) == 0 )
        {
            ulChunkCount = ulSize / TLS_MAX_RECORD_FRAGMENT_SIZE;
        }
        else
        {
            ulChunkCount = ulSize / TLS_MAX_RECORD_FRAGMENT_SIZE + 1;
        }
    }

    ulChunkBufSize = (ulSize >= TLS_MAX_RECORD_FRAGMENT_SIZE)? TLS_MAX_RECORD_CHUNK_SIZE + 256 + 128 : ulSize + 2048 + 256 + 128;
    pRecordChunk   = AnscAllocateMemory(ulChunkBufSize);

    if ( !pRecordChunk )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pTlsRecordHeader = (PTLS_RECORD_HEADER)((ULONG)pRecordChunk     + 256);
        pTlsSeqNumber    = (void*             )((ULONG)pTlsRecordHeader - 8  );
        pTlsSeqNumberH   = (void*             )((ULONG)pTlsSeqNumber         );
        pTlsSeqNumberL   = (void*             )((ULONG)pTlsSeqNumber    + 4  );

        /*
         * SSL 3.0 specifies a different MAC-generation algorithm. The MAC in SSL 3.0 is generated
         * as following:
         *
         *      hash(MAC_write_secret + pad_2 +
         *           hash(MAC_write_secret + pad_1 + seq_num + SSLCompressed.type +
         *                SSLCompressed.length + SSLCompressed.fragment));
         *
         * where "+" denotes concatenation.
         *
         *      pad_1       The character 0x36 repeated 48 times for MD5 or 40 times for SHA.
         *      pad_2       The character 0x5c repeated 48 times for MD5 or 40 times for SHA.
         *      seq_num     The sequence number for this message.
         *      hash        Hashing algorithm derived from the cipher suite.
         */
        if ( pRecordStateW->MinorVersion == 0 )
        {
            switch ( pRecordStateW->MacAlgorithm )
            {
                case    TLS_MAC_ALGORITHM_md5 :

                        pSslPad1     = (PUCHAR)((ULONG)pTlsSeqNumber - 48);
                        ulSslPadSize = 48;

                        break;

                case    TLS_MAC_ALGORITHM_sha :

                        pSslPad1     = (PUCHAR)((ULONG)pTlsSeqNumber - 40);
                        ulSslPadSize = 40;

                        break;

                default :

                        pSslPad1     = (PUCHAR)((ULONG)pTlsSeqNumber - 48);
                        ulSslPadSize = 48;

                        break;
            }

            pSslMacWriteSecret = (PUCHAR)((ULONG)pSslPad1 - pRecordStateW->MacSecret.Length);

            /*
             * As you can see from the above SSL 3.0 MAC definition, the designers have left out
             * the 'SSLCompressed.version' field in the hash calcuation! I wonder whether they did
             * it intentionally. Either way, what a bunch of morons! This leaves us no choice but
             * to shift the data around.
             */
            ulSslMacShiftSize  = (ULONG)pTlsRecordHeader - (ULONG)pSslMacWriteSecret;
            ulSslMacShiftSize += 3;     /* SSLCompressed.type + SSLCompressed.version */
        }
    }

    /*
     *      struct {
     *          ContentType         type;
     *          ProtocolVersion     version;
     *          uint16              length;
     *          opaque              fragment[TLSPlaintext.length];
     *      } TLSPlaintext;
     */
    for ( i = 0; i < ulChunkCount; i++ )
    {
        if ( i == (ulChunkCount - 1) )
        {
            ulChunkSize = (ulSize == 0)? 0: ulSize % TLS_MAX_RECORD_FRAGMENT_SIZE;
        }
        else
        {
            ulChunkSize = (ulSize == 0)? 0: TLS_MAX_RECORD_FRAGMENT_SIZE;
        }

        /*
         * Each connection state contains a sequence number, which is maintained separately for
         * read and write states. The sequence number must be set to zero whenever a connection
         * state is made the active state. Sequence numbers are of type uint64 and may not exceed
         * 2^64-1. A sequence number is incremented after each record: specifically, the first
         * record which is transmitted under a particular connection state should use sequence
         * number 0.
         */
        if ( TRUE )
        {
            AnscWriteUlong(pTlsSeqNumberH, AnscUlongFromHToN(pRecordStateW->SeqNumber.HiPart));
            AnscWriteUlong(pTlsSeqNumberL, AnscUlongFromHToN(pRecordStateW->SeqNumber.LoPart));

            AnscIncUint64((&pRecordStateW->SeqNumber));
        }

        TlsRecordSetContentType (pTlsRecordHeader, record_type                );
        TlsRecordSetMajorVersion(pTlsRecordHeader, pRecordStateW->MajorVersion);
        TlsRecordSetMinorVersion(pTlsRecordHeader, pRecordStateW->MinorVersion);
        TlsRecordSetLength      (pTlsRecordHeader, (USHORT)ulChunkSize        );
        TlsRecordSetFragment    (pTlsRecordHeader, pCurChunk                  );

        pCurChunk = (void*)((ULONG)pCurChunk + ulChunkSize);

        /*
         * All records are compressed using the compression algorithm defined in the current
         * session state. There is always an active compression algorithm; however, initially it is
         * defined as CompressionMethod.null. The compression algorithm translates a TLSPlaintext
         * structure into a TLSCompressed structure. Compression functions are initialized with
         * default state information whenever a connection state is made active.
         *
         * Compression must be lossless and may not increase the content length by more than 1024
         * bytes. If the decompression function encounters a TLSCompressed.fragment that would
         * decompress to a length in excess of 2^14 bytes, it should report a fatal decompression
         * failure error.
         *
         *      struct {
         *          ContentType         type;
         *          ProtocolVersion     version;
         *          uint16              length;
         *          opaque              fragment[TLSCompressed.length];
         *      } TLSCompressed;
         */
        if ( (pRecordStateW->CompressionAlgorithm != TLS_COMPRESSION_ALGORITHM_null) &&
             (ulChunkSize                         >  0                             ) )
        {
            returnStatus =
                pTlsCryptoProvider->Compress
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pRecordStateW->CompressionAlgorithm,
                        pTlsRecordHeader->Fragment,
                        ulChunkSize,
                        pTlsRecordHeader->Fragment,
                        &ulChunkSize
                    );

            if ( (returnStatus != ANSC_STATUS_SUCCESS                    ) ||
                 (ulChunkSize  >  TLS_RECORD_MAX_FRAGMENT_SIZE_COMPRESSED) )
            {
                returnStatus = ANSC_STATUS_FAILURE;

                goto  EXIT1;
            }
            else
            {
                TlsRecordSetLength(pTlsRecordHeader, (USHORT)ulChunkSize);
            }
        }

        /*
         * The MAC is generated as:
         *
         *      HMAC_hash(MAC_write_secret,
         *                seq_num + TLSCompressed.type + TLSCompressed.version +
         *                TLSCompressed.length + TLSCompressed.fragment));
         *
         * where "+" denotes concatenation.
         *
         *      seq_num
         *          The sequence number for this record.
         *      hash
         *          The hashing algorithm specified by SecurityParameters.mac_algorithm.
         *
         * Note that the MAC is computed before encryption. The stream cipher encrypts the entire
         * block, including the MAC. For stream ciphers that do not use a synchronization vector
         * (such as RC4), the stream cipher state from the end of one record is simply used on the
         * subsequent packet. If the CipherSuite is TLS_NULL_WITH_NULL_NULL, encryption consists of
         * the identity operation (i.e., the data is not encrypted and the MAC size is zero
         * implying that no MAC is used).
         */
        if ( pRecordStateW->MacAlgorithm != TLS_MAC_ALGORITHM_null )
        {
            if ( pRecordStateW->MinorVersion == 1 )
            {
                pHashResult    = temp_buffer;
                ulHashDataSize = TlsRecordGetSize(pTlsRecordHeader) + 8;
                ulMacHashSize  = pRecordStateW->MacHashSize;
                returnStatus   =
                    pTlsCryptoProvider->Hmac
                        (
                            (ANSC_HANDLE)pTlsCryptoProvider,
                            pRecordStateW->MacAlgorithm,
                            pRecordStateW->MacSecret.Value[0],
                            pRecordStateW->MacSecret.Length,
                            pTlsSeqNumber,
                            ulHashDataSize,
                            pHashResult,
                            &ulMacHashSize
                        );

                if ( returnStatus != ANSC_STATUS_SUCCESS )
                {
                    goto  EXIT1;
                }
                else
                {
                    ulChunkSize += ulMacHashSize;

                    TlsRecordAppendData(pTlsRecordHeader, pHashResult, ulMacHashSize);
                }
            }
            else
            {
                /*
                 * SSL 3.0 specifies a different MAC-generation algorithm. The MAC in SSL 3.0 is
                 * generated as following:
                 *
                 *      hash(MAC_write_secret + pad_2 +
                 *           hash(MAC_write_secret + pad_1 + seq_num + SSLCompressed.type +
                 *                SSLCompressed.length + SSLCompressed.fragment));
                 *
                 * where "+" denotes concatenation.
                 *
                 *      pad_1       The character 0x36 repeated 48 times for MD5 or 40
                 *                  times for SHA.
                 *      pad_2       The character 0x5c repeated 48 times for MD5 or 40
                 *                  times for SHA.
                 *      seq_num     The sequence number for this message.
                 *      hash        Hashing algorithm derived from the cipher suite.
                 */
                if ( TRUE )
                {
                    for ( j = 0; j < ulSslPadSize; j++ )
                    {
                        pSslPad1[j] = ucSslPad1Octet;
                    }

                    AnscCopyMemory
                        (
                            pSslMacWriteSecret,
                            pRecordStateW->MacSecret.Value[0],
                            pRecordStateW->MacSecret.Length
                        );
                }

                if ( TRUE )
                {
                    ulHashDataSize = (ULONG)pTlsRecordHeader - (ULONG)pSslMacWriteSecret + TlsRecordGetSize(pTlsRecordHeader) - 2;

                    /* RDKB-6311, CID-24097, defining upper and lower boundary 
                    ** If pSslMacWriteSecret larger than Chunk size or less than 2(result in wrap around of the variable value, on subtracting 2)
                    ** may leads to out of bound memory access.
                    */
                    if((ulChunkBufSize > ulSslMacShiftSize) && (ulSslMacShiftSize > 2))
                    {
                        AnscCopyMemory(pRecordChunk,           pSslMacWriteSecret, ulSslMacShiftSize    );
                        AnscCopyMemory(pSslMacWriteSecret + 2, pRecordChunk,       ulSslMacShiftSize - 2);
                    }
                    else
                    {
                        returnStatus = ANSC_STATUS_FAILURE;
                        goto  EXIT1;
                    }

                    pHashResult   = temp_buffer + pRecordStateW->MacSecret.Length + ulSslPadSize;
                    ulMacHashSize = pRecordStateW->MacHashSize;
                    returnStatus  =
                        pTlsCryptoProvider->Hash
                            (
                                (ANSC_HANDLE)pTlsCryptoProvider,
                                pRecordStateW->MacAlgorithm,
                                pSslMacWriteSecret + 2,
                                ulHashDataSize,
                                pHashResult,
                                &ulMacHashSize
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        goto  EXIT1;
                    }
                    else
                    {
                        /*
                         * Now let's restore the original header fields ... Actually we don't have
                         * to copy the whole 'shift block' back, we only need to restore the
                         * 'SSLCompressed.type' and 'SSLCompressed.version' fields. But we do it
                         * anyway for the sake of completeness.
                         */
                        AnscCopyMemory(pSslMacWriteSecret, pRecordChunk, ulSslMacShiftSize);
                    }
                }

                if ( TRUE )
                {
                    AnscCopyMemory
                        (
                            temp_buffer,
                            pRecordStateW->MacSecret.Value[0],
                            pRecordStateW->MacSecret.Length
                        );

                    for ( j = 0; j < ulSslPadSize; j++ )
                    {
                        temp_buffer[pRecordStateW->MacSecret.Length + j] = ucSslPad2Octet;
                    }

                    pHashResult    = temp_buffer;
                    ulHashDataSize = pRecordStateW->MacSecret.Length + ulSslPadSize + ulMacHashSize;
                    ulMacHashSize  = pRecordStateW->MacHashSize;
                    returnStatus   =
                        pTlsCryptoProvider->Hash
                            (
                                (ANSC_HANDLE)pTlsCryptoProvider,
                                pRecordStateW->MacAlgorithm,
                                temp_buffer,
                                ulHashDataSize,
                                pHashResult,
                                &ulMacHashSize
                            );

                    if ( returnStatus != ANSC_STATUS_SUCCESS )
                    {
                        goto  EXIT1;
                    }
                    else
                    {
                        ulChunkSize += ulMacHashSize;

                        TlsRecordAppendData(pTlsRecordHeader, pHashResult, ulMacHashSize);
                    }
                }
            }
        }
        else
        {
            ulMacHashSize = 0;
        }

        /*
         * The encryption and MAC functions translate a TLSCompressed structure into a
         * TLSCiphertext. The decryption functions reverse the process. The MAC of the record also
         * includes a sequence number so that missing, extra or repeated messages are detectable.
         *
         *      struct {
         *          ContentType         type;
         *          ProtocolVersion     version;
         *          uint16              length;
         *          select (CipherSpec.cipher_type) {
         *              case stream:    GenericStreamCipher;
         *              case block:     GenericBlockCipher;
         *          } fragment;
         *      } TLSCiphertext;
         */
        if ( (pRecordStateW->BulkEncryptionAlgorithm != TLS_BULK_CIPHER_ALGORITHM_null) &&
             (ulChunkSize                            >  0                             ) )
        {
            switch ( pRecordStateW->CipherType )
            {
                case    TLS_CIPHER_TYPE_stream :

                        /*
                         * Stream ciphers (including BulkCipherAlgorithm.null - see Appendix A.6)
                         * convert TLSCompressed.fragment structures to and from stream
                         * TLSCiphertext.fragment structures.
                         *
                         *      stream-ciphered struct {
                         *          opaque  content[TLSCompressed.length];
                         *          opaque  MAC[CipherSpec.hash_size];
                         *      } GenericStreamCipher;
                         */

                        break;

                case    TLS_CIPHER_TYPE_block :

                        /*
                         * For block ciphers (such as RC2 or DES), the encryption and MAC functions
                         * convert TLSCompressed.fragment structures to and from block
                         * TLSCiphertext.fragment structures.
                         *
                         *      block-ciphered struct {
                         *          opaque  content[TLSCompressed.length             ];
                         *          opaque  MAC    [CipherSpec.hash_size             ];
                         *          uint8   padding[GenericBlockCipher.padding_length];
                         *          uint8   padding_length;
                         *      } GenericBlockCipher;
                         */
                        if ( ((ulChunkSize + 1) % pRecordStateW->CipherBlockSize) == 0 )
                        {
                            ulPaddingLength = 0;
                        }
                        else
                        {
                            ulPaddingLength = pRecordStateW->CipherBlockSize - (ulChunkSize + 1) % pRecordStateW->CipherBlockSize;
                        }

                        if ( TRUE )
                        {
                            pPaddings = temp_buffer;

                            for ( j = 0; j < ulPaddingLength + 1; j++ )
                            {
                                pPaddings[j] = (UCHAR)ulPaddingLength;
                            }
                        }

                        ulChunkSize += ulPaddingLength + 1;

                        TlsRecordAppendData(pTlsRecordHeader, pPaddings, ulPaddingLength + 1);

                        break;

                default :

                        break;
            }

            returnStatus  =
                pTlsCryptoProvider->Encrypt
                    (
                        (ANSC_HANDLE)pTlsCryptoProvider,
                        pRecordStateW->BulkEncryptionAlgorithm,
                        &pRecordStateW->CipherKey,
                        &pRecordStateW->CipherIV,
                        pTlsRecordHeader->Fragment,
                        ulChunkSize,
                        pTlsRecordHeader->Fragment,
                        &ulChunkSize
                    );

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT1;
            }
        }

        if ( TRUE )
        {
            pTbpBdo = AnscAllocateBdo(0, 0, 0);

            if ( !pTbpBdo )
            {
                continue;
            }
            else
            {
                AnscBdoSetOwner     (pTbpBdo, FALSE                             );
                AnscBdoSetBuffer    (pTbpBdo, pTlsRecordHeader                  );
                AnscBdoSetBufferSize(pTbpBdo, TlsRecordGetSize(pTlsRecordHeader));
                AnscBdoSetOffset    (pTbpBdo, 0                                 );
                AnscBdoSetBlockSize (pTbpBdo, TlsRecordGetSize(pTlsRecordHeader));
            }

            returnStatus =
                pTlsTsaIf->SendTlsMessage
                    (
                        pTlsTsaIf->hOwnerContext,
                        (ANSC_HANDLE)pTbpBdo
                    );
        }

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            bSocketError = TRUE;

            goto  EXIT1;
        }
    }

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pRecordChunk )
    {
        AnscFreeMemory(pRecordChunk);
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        if ( bSocketError )
        {
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_socketError
                );
        }
        else
        {
            pTlsCbcIf->GenerateAlert
                (
                    pTlsCbcIf->hOwnerContext,
                    TLS_ALERT_LEVEL_fatal,
                    TLS_ALERT_DESCR_internalError
                );
        }
    }

    return  returnStatus;
}
