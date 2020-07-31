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

    module:	tls_cpo_hash.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced hash/hmac crypto
        functions of the TLS Crypto Provider Object.

        *   TlsCpoHash
        *   TlsCpoHash_MD5
        *   TlsCpoHash_SHA

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
        TlsCpoHash
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       hash_algorithm,
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

                ULONG                       hash_algorithm
                Specifies the hash algorithm to be performed.

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
TlsCpoHash
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       hash_algorithm,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    
    switch ( hash_algorithm )
    {
        case    TLS_MAC_ALGORITHM_md5 :

                return
                    pMyObject->Hash_MD5
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_data,
                            i_data_size,
                            o_hash,
                            o_hash_size
                        );

                break;

        case    TLS_MAC_ALGORITHM_sha :

                return
                    pMyObject->Hash_SHA
                        (
                            (ANSC_HANDLE)pMyObject,
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
        TlsCpoHash_MD5
            (
                ANSC_HANDLE                 hThisObject,
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
TlsCpoHash_MD5
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    ANSC_CRYPTO_HASH                hashResult;

    pAnscCrypto->Md5Digest
            (
                i_data,
                i_data_size,
                &hashResult
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
        TlsCpoHash_SHA
            (
                ANSC_HANDLE                 hThisObject,
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
TlsCpoHash_SHA
    (
        ANSC_HANDLE                 hThisObject,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_hash,
        PULONG                      o_hash_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    ANSC_CRYPTO_HASH                hashResult;

    pAnscCrypto->Sha1Digest
            (
                i_data,
                i_data_size,
                &hashResult
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
