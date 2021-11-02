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

    module:	tls_cpo_base.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the TLS Crypto Provider Object.

        *   TlsCpoCreate
        *   TlsCpoRemove
        *   TlsCpoEnrollObjects
        *   TlsCpoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/21/03    initial revision.

**********************************************************************/


#include "tls_cpo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        TlsCpoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the TLS Crypto Provider Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
TlsCpoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PTLS_CRYPTO_PROVIDER_OBJECT)AnscAllocateMemory(sizeof(TLS_CRYPTO_PROVIDER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    /* AnscCopyString(pBaseObject->Name, TLS_CRYPTO_PROVIDER_NAME); */

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = TLS_CRYPTO_PROVIDER_OID;
    pBaseObject->Create            = TlsCpoCreate;
    pBaseObject->Remove            = TlsCpoRemove;
    pBaseObject->EnrollObjects     = TlsCpoEnrollObjects;
    pBaseObject->Initialize        = TlsCpoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject      = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto    = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    PANSC_CRYPTO_PUB_OBJECT         pAnscCryptoPub = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;

    if ( pAnscCrypto )
    {
        pAnscCrypto->Remove((ANSC_HANDLE)pAnscCrypto);
    }

    if ( pAnscCryptoPub )
    {
        pAnscCryptoPub->Remove((ANSC_HANDLE)pAnscCryptoPub);
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject      = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;
    PANSC_CRYPTO_OBJECT             pAnscCrypto    = (PANSC_CRYPTO_OBJECT        )pMyObject->hAnscCrypto;
    PANSC_CRYPTO_PUB_OBJECT         pAnscCryptoPub = (PANSC_CRYPTO_PUB_OBJECT    )pMyObject->hAnscCryptoPub;

    if ( !pAnscCrypto )
    {
        pAnscCrypto = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto((ANSC_HANDLE)pMyObject);

        if ( !pAnscCrypto )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hAnscCrypto = (ANSC_HANDLE)pAnscCrypto;
        }
    }

    if ( !pAnscCryptoPub )
    {
        pAnscCryptoPub = (PANSC_CRYPTO_PUB_OBJECT)AnscCreateCryptoPubSSLeay((ANSC_HANDLE)pMyObject);

        if ( !pAnscCryptoPub )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pMyObject->hAnscCryptoPub = (ANSC_HANDLE)pAnscCryptoPub;
        }
    }

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCpoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
TlsCpoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CRYPTO_PROVIDER_OBJECT     pMyObject    = (PTLS_CRYPTO_PROVIDER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented progtlsming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);
    void* pTlsCpoGetExportable                    = TlsCpoGetExportable;
    void* pTlsCpoGetStrongSecurity                = TlsCpoGetStrongSecurity;
    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java progtlsmer perspective, it's the best we can get for universal embedded network
     * progtlsming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                        = TLS_CRYPTO_PROVIDER_OID;
    pMyObject->Create                     = TlsCpoCreate;
    pMyObject->Remove                     = TlsCpoRemove;
    pMyObject->EnrollObjects              = TlsCpoEnrollObjects;
    pMyObject->Initialize                 = TlsCpoInitialize;

    pMyObject->GetCipherInfo              = TlsCpoGetCipherInfo;
    pMyObject->GetCertConfigBit           = TlsCpoGetCertConfigBit;
    pMyObject->GetClientCertType          = TlsCpoGetClientCertType;
    pMyObject->GetKeAlgorithm             = TlsCpoGetKeAlgorithm;
    pMyObject->GetKeyBlockSize            = TlsCpoGetKeyBlockSize;
    pMyObject->GetBulkEncryptionAlgorithm = TlsCpoGetBulkEncryptionAlgorithm;
    pMyObject->GetCipherType              = TlsCpoGetCipherType;
    pMyObject->GetCipherKeyNumber         = TlsCpoGetCipherKeyNumber;
    pMyObject->GetCipherKeySize           = TlsCpoGetCipherKeySize;
    pMyObject->GetCipherKeyMaterialSize   = TlsCpoGetCipherKeyMaterialSize;
    pMyObject->GetCipherIVSize            = TlsCpoGetCipherIVSize;
    pMyObject->GetCipherBlockSize         = TlsCpoGetCipherBlockSize;
    pMyObject->GetMacAlgorithm            = TlsCpoGetMacAlgorithm;
    pMyObject->GetMacHashSize             = TlsCpoGetMacHashSize;
    pMyObject->GetExportable              = (PFN_TLSCPO_GET_BOOL)pTlsCpoGetExportable;
    pMyObject->GetStrongSecurity          = (PFN_TLSCPO_GET_BOOL)pTlsCpoGetStrongSecurity;

    pMyObject->AuthenticateCertChain      = TlsCpoAuthenticateCertChain;

    pMyObject->DecapRSAPublicKey          = TlsCpoDecapRSAPublicKey;
    pMyObject->EncapRSAPublicKey          = TlsCpoEncapRSAPublicKey;
    pMyObject->GenerateRSAKeys            = TlsCpoGenerateRSAKeys;

    pMyObject->DecapDHPublicKey           = TlsCpoDecapDHPublicKey;
    pMyObject->EncapDHPublicKey           = TlsCpoEncapDHPublicKey;
    pMyObject->GenerateDHKeys1            = TlsCpoGenerateDHKeys1;
    pMyObject->GenerateDHKeys2            = TlsCpoGenerateDHKeys2;
    pMyObject->AgreeDHSecret              = TlsCpoAgreeDHSecret;

    pMyObject->GetRandomBytes             = TlsCpoGetRandomBytes;
    pMyObject->Compute_PRF30              = TlsCpoCompute_PRF30;
    pMyObject->Compute_PRF31              = TlsCpoCompute_PRF31;

    pMyObject->Encrypt_RSA                = TlsCpoEncrypt_RSA;
    pMyObject->Decrypt_RSA                = TlsCpoDecrypt_RSA;
    pMyObject->Sign                       = TlsCpoSign;
    pMyObject->Sign_RSA                   = TlsCpoSign_RSA;
    pMyObject->Sign_DSA                   = TlsCpoSign_DSA;
    pMyObject->Verify                     = TlsCpoVerify;
    pMyObject->Verify_RSA                 = TlsCpoVerify_RSA;
    pMyObject->Verify_DSA                 = TlsCpoVerify_DSA;

    pMyObject->Hash                       = TlsCpoHash;
    pMyObject->Hash_MD5                   = TlsCpoHash_MD5;
    pMyObject->Hash_SHA                   = TlsCpoHash_SHA;

    pMyObject->Hmac                       = TlsCpoHmac;
    pMyObject->Hmac_MD5                   = TlsCpoHmac_MD5;
    pMyObject->Hmac_SHA                   = TlsCpoHmac_SHA;

    pMyObject->Encrypt                    = TlsCpoEncrypt;
    pMyObject->Encrypt_DES                = TlsCpoEncrypt_DES;
    pMyObject->Encrypt_3DES               = TlsCpoEncrypt_3DES;
    pMyObject->Encrypt_IDEA               = TlsCpoEncrypt_IDEA;
    pMyObject->Encrypt_AES                = TlsCpoEncrypt_AES;

    pMyObject->Decrypt                    = TlsCpoDecrypt;
    pMyObject->Decrypt_DES                = TlsCpoDecrypt_DES;
    pMyObject->Decrypt_3DES               = TlsCpoDecrypt_3DES;
    pMyObject->Decrypt_IDEA               = TlsCpoDecrypt_IDEA;
    pMyObject->Decrypt_AES                = TlsCpoDecrypt_AES;

#ifndef _ANSC_CRYPTO_NO_RC2_
    pMyObject->Encrypt_RC2                = TlsCpoEncrypt_RC2;
    pMyObject->Decrypt_RC2                = TlsCpoDecrypt_RC2;
#endif    
#ifndef _ANSC_CRYPTO_NO_RC4_
    pMyObject->Encrypt_RC4                = TlsCpoEncrypt_RC4;
    pMyObject->Decrypt_RC4                = TlsCpoDecrypt_RC4;
#endif

    pMyObject->Compress                   = TlsCpoCompress;
    pMyObject->Decompress                 = TlsCpoDecompress;

    return  ANSC_STATUS_SUCCESS;
}
