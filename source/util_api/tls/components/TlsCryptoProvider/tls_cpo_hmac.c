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

    module:	tls_cpo_hmac.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced hash/hmac crypto
        functions of the TLS Crypto Provider Object.

        *   TlsCpoHmac
        *   TlsCpoHmac_MD5
        *   TlsCpoHmac_SHA

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/03    initial revision.

**********************************************************************/


#include "tls_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoHmac
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       mac_algorithm,
                void*                       i_secret,
                ULONG                       i_secret_size,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_hash,
                PULONG                      o_hash_size
            );

    description:

        This function is called to perform hashing on the input data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       mac_algorithm
                Specifies the hmac algorithm to be performed.

                void*                       i_secret
                Specifies the hmac secret to be used for hashing.

                ULONG                       i_secret_size
                Specifies the size of the hmac secret to be used.

                void*                       i_data
                Specifies the data buffer to be hashed.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be hashed.

                void*                       o_hash
                Specifies the output buffer of hash result.

                PULONG                      o_hash_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoHmac
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mac_algorithm,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    
    switch ( mac_algorithm )
    {
        case    TLS_MAC_ALGORITHM_md5 :

                return
                    pMyObject->Hmac_MD5
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_secret,
                            i_secret_size,
                            i_data,
                            i_data_size,
                            o_hash,
                            o_hash_size
                        );

                break;

        case    TLS_MAC_ALGORITHM_sha :

                return
                    pMyObject->Hmac_SHA
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_secret,
                            i_secret_size,
                            i_data,
                            i_data_size,
                            o_hash,
                            o_hash_size
                        );

                break;

        default :

                return ANSC_STATUS_UNAPPLICABLE;

                break;
    }
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoHmac_MD5
            (
                ANSC_HANDLE                 hThisObject,
                void*                       i_secret,
                ULONG                       i_secret_size,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_hash,
                PULONG                      o_hash_size
            );

    description:

        This function is called to perform hashing on the input data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       i_secret
                Specifies the hmac secret to be used for hashing.

                ULONG                       i_secret_size
                Specifies the size of the hmac secret to be used.

                void*                       i_data
                Specifies the data buffer to be hashed.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be hashed.

                void*                       o_hash
                Specifies the output buffer of hash result.

                PULONG                      o_hash_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoHmac_MD5
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    ANSC_CRYPTO_HASH                hashResult;
    ANSC_CRYPTO_KEY                 macKey;

    if ( TRUE )
    {
        macKey.KeyNumber   = 1;
        macKey.RoundNumber = 0;
        macKey.Length      = i_secret_size;

        AnscCopyMemory
            (
                macKey.Value[0],
                i_secret,
                i_secret_size
            );
    }

    hashResult.Length = ANSC_MD5_OUTPUT_SIZE;

    pAnscCrypto->HmacMd5Digest
            (
                i_data,
                i_data_size,
                &hashResult,
                &macKey
            );

    AnscCopyMemory
        (
            o_hash,
            hashResult.Value,
            hashResult.Length
        );

    *o_hash_size = hashResult.Length;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoHmac_SHA
            (
                ANSC_HANDLE                 hThisObject,
                void*                       i_secret,
                ULONG                       i_secret_size,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_hash,
                PULONG                      o_hash_size
            );

    description:

        This function is called to perform hashing on the input data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       i_secret
                Specifies the hmac secret to be used for hashing.

                ULONG                       i_secret_size
                Specifies the size of the hmac secret to be used.

                void*                       i_data
                Specifies the data buffer to be hashed.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be hashed.

                void*                       o_hash
                Specifies the output buffer of hash result.

                PULONG                      o_hash_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoHmac_SHA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_secret,
        ULONG                       i_secret_size,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    ANSC_CRYPTO_HASH                hashResult;
    ANSC_CRYPTO_KEY                 macKey;

    if ( TRUE )
    {
        macKey.KeyNumber   = 1;
        macKey.RoundNumber = 0;
        macKey.Length      = i_secret_size;

        AnscCopyMemory
            (
                macKey.Value[0],
                i_secret,
                i_secret_size
            );
    }

    hashResult.Length = ANSC_SHA1_OUTPUT_SIZE;

    pAnscCrypto->HmacSha1Digest
            (
                i_data,
                i_data_size,
                &hashResult,
                &macKey
            );

    AnscCopyMemory
        (
            o_hash,
            hashResult.Value,
            hashResult.Length
        );

    *o_hash_size = hashResult.Length;

    return  ANSC_STATUS_SUCCESS;
}
