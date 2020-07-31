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

    module:	tls_cpo_prf.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced pseudo-random functions
        of the TLS Crypto Provider Object.

        *   TlsCpoCompute_PRF30
        *   TlsCpoCompute_PRF31

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/22/03    initial revision.

**********************************************************************/


#include "tls_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoCompute_PRF30
            (
                ANSC_HANDLE                 hThisObject,
                void*                       i_secret,
                ULONG                       i_secret_size,
                void*                       i_seed,
                ULONG                       i_seed_size,
                void*                       o_hash,
                ULONG                       o_hash_size
            );

    description:

        This function is called to compute PRF of SSL 3.0.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       i_secret
                Specifies the secret value for the PRF computation.

                ULONG                       i_secret_size
                Specifies the size of the secret value.

                void*                       i_seed
                Specifies the input seed for the PRF computation.

                ULONG                       i_seed_size
                Specifies the size of the input seed.

                void*                       o_hash
                Specifies the output result for the PRF computation.

                ULONG                       o_hash_size
                Specifies the size of the output result.

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
TlsCpoCompute_PRF30
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_seed,
        ULONG                       i_seed_size,
        void*                       o_hash,
        ULONG                       o_hash_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PUCHAR                          pScratchPad1 = (PUCHAR                     )NULL;
    ULONG                           ulPadSize1   = (ULONG                      )0;
    PUCHAR                          pScratchPad2 = (PUCHAR                     )NULL;
    ULONG                           ulPadSize2   = (ULONG                      )0;
    ULONG                           ulHashOffset = (ULONG                      )0;
    ULONG                           ulTbhSize    = (ULONG                      )0;
    ULONG                           ulHashSize   = (ULONG                      )0;
    ULONG                           ulIteration  = (ULONG                      )o_hash_size / ANSC_MD5_OUTPUT_SIZE;
    ULONG                           i            = 0;
    ULONG                           j            = 0;

    if ( o_hash_size == 0 )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if ( (o_hash_size % ANSC_MD5_OUTPUT_SIZE) > 0 )
    {
        ulIteration += 1;
    }

    if ( ulIteration > 32 )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    ulPadSize1   = i_secret_size + i_seed_size + 32;
    ulPadSize2   = i_secret_size + ANSC_SHA1_OUTPUT_SIZE;
    pScratchPad1 = (PUCHAR)AnscAllocateMemory(ulPadSize1);
    pScratchPad2 = (PUCHAR)AnscAllocateMemory(ulPadSize2);

    if ( !pScratchPad1 || !pScratchPad2 )
    {
        goto  EXIT1;
    }
    else
    {
        AnscCopyMemory(pScratchPad2, i_secret, i_secret_size);
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
    for ( i = 0; i < ulIteration; i++ )
    {
        if ( TRUE )
        {
            ulTbhSize = 0;

            for ( j = 0; j <= i; j++ )
            {
                pScratchPad1[j] = (UCHAR)'A' + (UCHAR)i;
            }

            ulTbhSize += j;
            AnscCopyMemory(&pScratchPad1[ulTbhSize], i_secret, i_secret_size);
            ulTbhSize += i_secret_size;
            AnscCopyMemory(&pScratchPad1[ulTbhSize], i_seed,   i_seed_size  );
            ulTbhSize += i_seed_size;

            ulHashSize   = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus =
                pMyObject->Hash_SHA
                    (
                        (ANSC_HANDLE)pMyObject,
                        pScratchPad1,
                        ulTbhSize,
                        &pScratchPad2[i_secret_size],
                        &ulHashSize
                    );
        }

        if ( TRUE )
        {
            ulTbhSize    = i_secret_size + ulHashSize;
            ulHashSize   = ANSC_MD5_OUTPUT_SIZE;
            returnStatus =
                pMyObject->Hash_MD5
                    (
                        (ANSC_HANDLE)pMyObject,
                        pScratchPad2,
                        ulTbhSize,
                        &pScratchPad1[0],
                        &ulHashSize
                    );
        }

        if ( ulHashOffset >= o_hash_size )
        {
            break;
        }
        else if ( ulHashSize > (o_hash_size - ulHashOffset) )
        {
            ulHashSize = o_hash_size - ulHashOffset;
        }

        AnscCopyMemory
            (
                (PVOID)((ULONG)o_hash + ulHashOffset),
                pScratchPad1,
                ulHashSize
            );

        ulHashOffset += ulHashSize;
    }

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pScratchPad1 )
    {
        AnscFreeMemory(pScratchPad1);
    }

    if ( pScratchPad2 )
    {
        AnscFreeMemory(pScratchPad2);
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoCompute_PRF31
            (
                ANSC_HANDLE                 hThisObject,
                void*                       i_secret,
                ULONG                       i_secret_size,
                char*                       i_label,
                void*                       i_seed,
                ULONG                       i_seed_size,
                void*                       o_hash,
                ULONG                       o_hash_size
            );

    description:

        This function is called to compute PRF of SSL 3.1.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       i_secret
                Specifies the secret value for the PRF computation.

                ULONG                       i_secret_size
                Specifies the size of the secret value.

                char*                       i_label
                Specifies the PRF label to be used in computation.

                void*                       i_seed
                Specifies the input seed for the PRF computation.

                ULONG                       i_seed_size
                Specifies the size of the input seed.

                void*                       o_hash
                Specifies the output result for the PRF computation.

                ULONG                       o_hash_size
                Specifies the size of the output result.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoCompute_PRF31
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        char*                       i_label,
        void*                       i_seed,
        ULONG                       i_seed_size,
        void*                       o_hash,
        ULONG                       o_hash_size
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PUCHAR                          pScratchPad1 = (PUCHAR                     )NULL;
    ULONG                           ulPadSize1   = (ULONG                      )0;
    PUCHAR                          pScratchPad2 = (PUCHAR                     )NULL;
    ULONG                           ulPadSize2   = (ULONG                      )0;
    PUCHAR                          pScratchPad3 = (PUCHAR                     )NULL;
    ULONG                           ulPadSize3   = (ULONG                      )0;
    PUCHAR                          pScratchPad4 = (PUCHAR                     )NULL;
    ULONG                           ulPadSize4   = (ULONG                      )0;
    PUCHAR                          pSecretMd5   = (PUCHAR                     )NULL;
    PUCHAR                          pSecretSha   = (PUCHAR                     )NULL;
    UCHAR                           pHashBack[32]= { 0 };
    ULONG                           ulSecretSize = (ULONG                      )0;
    ULONG                           ulFuncA_Size = (ULONG                      )0;
    ULONG                           ulHashOffset = (ULONG                      )0;
    ULONG                           ulTbhSize    = (ULONG                      )0;
    ULONG                           ulHashSize   = (ULONG                      )0;
    ULONG                           ulIteration  = (ULONG                      )o_hash_size / ANSC_MD5_OUTPUT_SIZE;
    ULONG                           i            = 0;
    ULONG                           j            = 0;

    if ( o_hash_size == 0 )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    /* the i_secret_size could be zero and i_secret could be empty */
    else if ( i_secret_size == 0 || i_secret == NULL)
    {
        i_secret_size = 0;

        /* return  ANSC_STATUS_UNAPPLICABLE; */
    }

    if ( (i_secret_size % 2) > 0 )
    {
        pSecretMd5   = (PUCHAR)i_secret;
        pSecretSha   = (PUCHAR)((ULONG)i_secret + i_secret_size / 2);
        ulSecretSize = i_secret_size / 2 + 1;
    }
    else
    {
        pSecretMd5   = (PUCHAR)i_secret;
        pSecretSha   = (PUCHAR)((ULONG)i_secret + i_secret_size / 2);
        ulSecretSize = i_secret_size / 2;
    }

    ulPadSize1   = AnscGetMax2(ANSC_SHA1_OUTPUT_SIZE, AnscSizeOfString(i_label) + i_seed_size);
    ulPadSize2   = ulPadSize1 * 2;
    ulPadSize3   = o_hash_size;
    ulPadSize4   = o_hash_size;
    pScratchPad1 = (PUCHAR)AnscAllocateMemory(ulPadSize1);
    pScratchPad2 = (PUCHAR)AnscAllocateMemory(ulPadSize2);
    pScratchPad3 = (PUCHAR)AnscAllocateMemory(ulPadSize3);
    pScratchPad4 = (PUCHAR)AnscAllocateMemory(ulPadSize4);

    if ( !pScratchPad1 || !pScratchPad2 || !pScratchPad3 || !pScratchPad4 )
    {
        goto  EXIT1;
    }
    else
    {
        AnscCopyMemory(pScratchPad2, i_secret, i_secret_size);
    }

    /*
     * In order to make the PRF as secure as possible, it uses two hash algorithms in a way which
     * should guarantee its security if either algorithm remains secure. First, we define a data
     * expansion function, P_hash(secret, data) which uses a single hash function to expand a
     * secret and seed into an arbitrary quantity of output:
     *
     *      P_hash(secret, seed) = HMAC_hash(secret, A(1) + seed) +
     *                             HMAC_hash(secret, A(2) + seed) +
     *                             HMAC_hash(secret, A(3) + seed) + ...
     *
     * Where + indicates concatenation.
     *
     * A() is defined as:
     *
     *      A(0) = seed
     *      A(i) = HMAC_hash(secret, A(i-1))
     *
     * P_hash can be iterated as many times as is necessary to produce the required quantity of
     * data. For example, if P_SHA-1 was being used to create 64 bytes of data, it would have to be
     * iterated 4 times (through A(4)), creating 80 bytes of output data; the last 16 bytes of the
     * final iteration would then be discarded, leaving 64 bytes of output data.
     *
     * TLS's PRF is created by splitting the secret into two halves and using one half to generate
     * data with P_MD5 and the other half to generate data with P_SHA-1, then exclusive-or'ing the
     * outputs of these two expansion functions together.
     */
    if ( TRUE )
    {
        ulHashOffset = 0;
        ulIteration  = o_hash_size / ANSC_MD5_OUTPUT_SIZE;

        if ( (o_hash_size % ANSC_MD5_OUTPUT_SIZE) > 0 )
        {
            ulIteration += 1;
        }

        if ( ulIteration > 32 )
        {
            returnStatus = ANSC_STATUS_UNAPPLICABLE;

            goto  EXIT1;
        }

        ulFuncA_Size  = 0;
        AnscCopyMemory(&pScratchPad1[ulFuncA_Size], i_label, AnscSizeOfString(i_label));
        ulFuncA_Size += AnscSizeOfString(i_label);
        AnscCopyMemory(&pScratchPad1[ulFuncA_Size], i_seed,  i_seed_size              );
        ulFuncA_Size += i_seed_size;

        ulHashSize   = ANSC_MD5_OUTPUT_SIZE;
        returnStatus =
            pMyObject->Hmac_MD5
                (
                    (ANSC_HANDLE)pMyObject,
                    pSecretMd5,
                    ulSecretSize,
                    pScratchPad1,
                    ulFuncA_Size,
                    pHashBack,
                    &ulHashSize
                );

        AnscCopyMemory(pScratchPad2, pHashBack, ulHashSize             );
        ulTbhSize  = ulHashSize;

        for ( i = 0; i < ulIteration; i++ )
        {            
            AnscCopyMemory(&pScratchPad2[ulTbhSize], i_label,      AnscSizeOfString(i_label));
            ulTbhSize += AnscSizeOfString(i_label);

            AnscCopyMemory(&pScratchPad2[ulTbhSize], i_seed,       i_seed_size              );
            ulTbhSize += i_seed_size;

            ulHashSize   = ANSC_MD5_OUTPUT_SIZE;
            returnStatus =
                pMyObject->Hmac_MD5
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSecretMd5,
                        ulSecretSize,
                        pScratchPad2,
                        ulTbhSize,
                        pScratchPad2,
                        &ulHashSize
                    );

            if ( ulHashSize > (o_hash_size - ulHashOffset) )
            {
                ulHashSize = o_hash_size - ulHashOffset;
            }

            AnscCopyMemory
                (
                    &pScratchPad3[ulHashOffset],
                    pScratchPad2,
                    ulHashSize
                );

            ulHashOffset += ulHashSize;

            if( ulHashSize != ANSC_MD5_OUTPUT_SIZE)
            {
                break;
            }

            /* Be careful here, A(i) is not above hash result, we have to 
             * calculate again without seeds:
             *
             * A() is defined as:
             *
             *      A(0) = seed
             *      A(i) = HMAC_hash(secret, A(i-1))
             */
            ulHashSize   = ANSC_MD5_OUTPUT_SIZE;
            returnStatus =
                pMyObject->Hmac_MD5
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSecretMd5,
                        ulSecretSize,
                        pHashBack,
                        ulHashSize,
                        pHashBack,
                        &ulHashSize
                    );

            AnscCopyMemory(pScratchPad2, pHashBack, ulHashSize);
            ulTbhSize     = ulHashSize;
        }
    }

    if ( TRUE )
    {
        ulHashOffset = 0;
        ulIteration  = o_hash_size / ANSC_SHA1_OUTPUT_SIZE;

        if ( (o_hash_size % ANSC_SHA1_OUTPUT_SIZE) > 0 )
        {
            ulIteration += 1;
        }

        if ( ulIteration > 32 )
        {
            returnStatus = ANSC_STATUS_UNAPPLICABLE;

            goto  EXIT1;
        }

        ulFuncA_Size  = 0;
        AnscCopyMemory(&pScratchPad1[ulFuncA_Size], i_label, AnscSizeOfString(i_label));
        ulFuncA_Size += AnscSizeOfString(i_label);
        AnscCopyMemory(&pScratchPad1[ulFuncA_Size], i_seed,  i_seed_size              );
        ulFuncA_Size += i_seed_size;

        ulHashSize   = ANSC_SHA1_OUTPUT_SIZE;
        returnStatus =
            pMyObject->Hmac_SHA
                (
                    (ANSC_HANDLE)pMyObject,
                    pSecretSha,
                    ulSecretSize,
                    pScratchPad1,
                    ulFuncA_Size,
                    pHashBack,
                    &ulHashSize
                );

        AnscCopyMemory(pScratchPad2, pHashBack, ulHashSize);
        ulTbhSize  = ulHashSize;

        for ( i = 0; i < ulIteration; i++ )
        {
            AnscCopyMemory(&pScratchPad2[ulTbhSize], i_label,      AnscSizeOfString(i_label));
            ulTbhSize += AnscSizeOfString(i_label);

            AnscCopyMemory(&pScratchPad2[ulTbhSize], i_seed,       i_seed_size              );
            ulTbhSize += i_seed_size;

            ulHashSize   = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus =
                pMyObject->Hmac_SHA
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSecretSha,
                        ulSecretSize,
                        pScratchPad2,
                        ulTbhSize,
                        pScratchPad2,
                        &ulHashSize
                    );

            if ( ulHashSize > (o_hash_size - ulHashOffset) )
            {
                ulHashSize = o_hash_size - ulHashOffset;
            }

            AnscCopyMemory
                (
                    &pScratchPad4[ulHashOffset],
                    pScratchPad2,
                    ulHashSize
                );

            ulHashOffset += ulHashSize;

            if( ulHashSize != ANSC_SHA1_OUTPUT_SIZE)
            {
                break;
            }

            /* Be careful here, A(i) is not above hash result, we have to 
             * calculate again without seeds:
             *
             * A() is defined as:
             *
             *      A(0) = seed
             *      A(i) = HMAC_hash(secret, A(i-1))
             */
            ulHashSize   = ANSC_SHA1_OUTPUT_SIZE;
            returnStatus =
                pMyObject->Hmac_SHA
                    (
                        (ANSC_HANDLE)pMyObject,
                        pSecretSha,
                        ulSecretSize,
                        pHashBack,
                        ulHashSize,
                        pHashBack,
                        &ulHashSize
                    );

            AnscCopyMemory(pScratchPad2, pHashBack, ulHashSize);
            ulTbhSize     = ulHashSize;
        }
    }

    for ( j = 0; j < o_hash_size; j++ )
    {
        *(PUCHAR)((ULONG)o_hash + j) = pScratchPad3[j] ^ pScratchPad4[j];
    }

    returnStatus = ANSC_STATUS_SUCCESS;


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT1:

    if ( pScratchPad1 )
    {
        AnscFreeMemory(pScratchPad1);
    }

    if ( pScratchPad2 )
    {
        AnscFreeMemory(pScratchPad2);
    }

    if ( pScratchPad3 )
    {
        AnscFreeMemory(pScratchPad3);
    }

    if ( pScratchPad4 )
    {
        AnscFreeMemory(pScratchPad4);
    }

    return  returnStatus;
}
