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

    module:	tls_cpo_encrypt.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced encryption crypto
        functions of the TLS Crypto Provider Object.

        *   TlsCpoEncrypt
        *   TlsCpoEncrypt_DES
        *   TlsCpoEncrypt_3DES
        *   TlsCpoEncrypt_IDEA
        *   TlsCpoEncrypt_RC2
        *   TlsCpoEncrypt_RC4
        *   TlsCpoEncrypt_AES

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
        TlsCpoEncrypt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       cipher_algorithm,
                ANSC_CRYPTO_KEY*            i_key,
                ANSC_CRYPTO_IV*             i_iv,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_data,
                PULONG                      o_data_size
            );

    description:

        This function is called to perform the specified symmetric
        encryption algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       cipher_algorithm
                Specifies the encryption algorithm to be performed.

                ANSC_CRYPTO_KEY*            i_key
                Specifies the encryption key to be used.

                ANSC_CRYPTO_IV*             i_iv
                Specifies the encryption iv to be used.

                void*                       i_data
                Specifies the data buffer to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be encrypted.

                void*                       o_data
                Specifies the output buffer of encryption result.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cipher_algorithm,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;

    switch ( cipher_algorithm )
    {
        case    TLS_BULK_CIPHER_ALGORITHM_rc4 :

                return
                    pMyObject->Encrypt_RC4
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_key,
                            i_iv,
                            i_data,
                            i_data_size,
                            o_data,
                            o_data_size
                        );

                break;

        case    TLS_BULK_CIPHER_ALGORITHM_rc2 :

                return
                    pMyObject->Encrypt_RC2
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_key,
                            i_iv,
                            i_data,
                            i_data_size,
                            o_data,
                            o_data_size
                        );

                break;

        case    TLS_BULK_CIPHER_ALGORITHM_des   :
        case    TLS_BULK_CIPHER_ALGORITHM_des40 :

                return
                    pMyObject->Encrypt_DES
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_key,
                            i_iv,
                            i_data,
                            i_data_size,
                            o_data,
                            o_data_size
                        );

                break;

        case    TLS_BULK_CIPHER_ALGORITHM_3des :

                return
                    pMyObject->Encrypt_3DES
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_key,
                            i_iv,
                            i_data,
                            i_data_size,
                            o_data,
                            o_data_size
                        );

                break;

        case    TLS_BULK_CIPHER_ALGORITHM_idea :

                return
                    pMyObject->Encrypt_IDEA
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_key,
                            i_iv,
                            i_data,
                            i_data_size,
                            o_data,
                            o_data_size
                        );

                break;

        case    TLS_BULK_CIPHER_ALGORITHM_aes :

                return
                    pMyObject->Encrypt_AES
                        (
                            (ANSC_HANDLE)pMyObject,
                            i_key,
                            i_iv,
                            i_data,
                            i_data_size,
                            o_data,
                            o_data_size
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
        TlsCpoEncrypt_DES
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_CRYPTO_KEY*            i_key,
                ANSC_CRYPTO_IV*             i_iv,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_data,
                PULONG                      o_data_size
            );

    description:

        This function is called to perform the specified symmetric
        encryption algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_CRYPTO_KEY*            i_key
                Specifies the encryption key to be used.

                ANSC_CRYPTO_IV*             i_iv
                Specifies the encryption iv to be used.

                void*                       i_data
                Specifies the data buffer to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be encrypted.

                void*                       o_data
                Specifies the output buffer of encryption result.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt_DES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    
    pAnscCrypto->DesEncrypt
            (
                i_data,
                i_data_size,
                o_data,
                i_key,
                i_iv
            );

    *o_data_size = i_data_size;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoEncrypt_3DES
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_CRYPTO_KEY*            i_key,
                ANSC_CRYPTO_IV*             i_iv,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_data,
                PULONG                      o_data_size
            );

    description:

        This function is called to perform the specified symmetric
        encryption algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_CRYPTO_KEY*            i_key
                Specifies the encryption key to be used.

                ANSC_CRYPTO_IV*             i_iv
                Specifies the encryption iv to be used.

                void*                       i_data
                Specifies the data buffer to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be encrypted.

                void*                       o_data
                Specifies the output buffer of encryption result.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt_3DES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    
    pAnscCrypto->TripleDesEncrypt
            (
                i_data,
                i_data_size,
                o_data,
                i_key,
                i_iv
            );

    *o_data_size = i_data_size;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoEncrypt_IDEA
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_CRYPTO_KEY*            i_key,
                ANSC_CRYPTO_IV*             i_iv,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_data,
                PULONG                      o_data_size
            );

    description:

        This function is called to perform the specified symmetric
        encryption algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_CRYPTO_KEY*            i_key
                Specifies the encryption key to be used.

                ANSC_CRYPTO_IV*             i_iv
                Specifies the encryption iv to be used.

                void*                       i_data
                Specifies the data buffer to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be encrypted.

                void*                       o_data
                Specifies the output buffer of encryption result.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt_IDEA
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    
    pAnscCrypto->IdeaEncrypt
            (
                i_data,
                i_data_size,
                o_data,
                i_key,
                i_iv
            );

    *o_data_size = i_data_size;

    return  ANSC_STATUS_SUCCESS;
}


#ifndef _ANSC_CRYPTO_NO_RC2_

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoEncrypt_RC2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_CRYPTO_KEY*            i_key,
                ANSC_CRYPTO_IV*             i_iv,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_data,
                PULONG                      o_data_size
            );

    description:

        This function is called to perform the specified symmetric
        encryption algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_CRYPTO_KEY*            i_key
                Specifies the encryption key to be used.

                ANSC_CRYPTO_IV*             i_iv
                Specifies the encryption iv to be used.

                void*                       i_data
                Specifies the data buffer to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be encrypted.

                void*                       o_data
                Specifies the output buffer of encryption result.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt_RC2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    
    pAnscCrypto->Rc2Encrypt
            (
                i_data,
                i_data_size,
                o_data,
                i_key,
                i_iv
            );

    *o_data_size = i_data_size;

    return  ANSC_STATUS_SUCCESS;
}

#endif


#ifndef _ANSC_CRYPTO_NO_RC4_

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoEncrypt_RC4
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_CRYPTO_KEY*            i_key,
                ANSC_CRYPTO_IV*             i_iv,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_data,
                PULONG                      o_data_size
            );

    description:

        This function is called to perform the specified symmetric
        encryption algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_CRYPTO_KEY*            i_key
                Specifies the encryption key to be used.

                ANSC_CRYPTO_IV*             i_iv
                Specifies the encryption iv to be used.

                void*                       i_data
                Specifies the data buffer to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be encrypted.

                void*                       o_data
                Specifies the output buffer of encryption result.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt_RC4
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    
    pAnscCrypto->TLSRc4Encrypt
            (
                i_data,
                i_data_size,
                o_data,
                i_key,
                i_iv
            );

    *o_data_size = i_data_size;

    return  ANSC_STATUS_SUCCESS;
}

#endif


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoEncrypt_AES
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_CRYPTO_KEY*            i_key,
                ANSC_CRYPTO_IV*             i_iv,
                void*                       i_data,
                ULONG                       i_data_size,
                void*                       o_data,
                PULONG                      o_data_size
            );

    description:

        This function is called to perform the specified symmetric
        encryption algorithm.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_CRYPTO_KEY*            i_key
                Specifies the encryption key to be used.

                ANSC_CRYPTO_IV*             i_iv
                Specifies the encryption iv to be used.

                void*                       i_data
                Specifies the data buffer to be encrypted.

                ULONG                       i_data_size
                Specifies the size of the data buffer to be encrypted.

                void*                       o_data
                Specifies the output buffer of encryption result.

                PULONG                      o_data_size
                As an input parameter, it specifies the size of the
                output buffer; as an output parameter, it specifies
                the size of the real output.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEncrypt_AES
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_CRYPTO_KEY*            i_key,
        ANSC_CRYPTO_IV*             i_iv,
        void*                       i_data,
        ULONG                       i_data_size,
        void*                       o_data,
        PULONG                      o_data_size
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto  = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    
    pAnscCrypto->AesEncrypt
            (
                i_data,
                i_data_size,
                o_data,
                i_key,
                i_iv
            );

    *o_data_size = i_data_size;

    return  ANSC_STATUS_SUCCESS;
}
