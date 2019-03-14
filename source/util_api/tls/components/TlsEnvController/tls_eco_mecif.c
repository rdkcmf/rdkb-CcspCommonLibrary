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

    module:	tls_eco_mecif.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced interface functions
        of the TLS Env Controller Object.

        *   TlsEcoMecGetSessionManager
        *   TlsEcoMecGetCryptoProvider
        *   TlsEcoMecGetHsmIf
        *   TlsEcoMecGetModuleParams
        *   TlsEcoMecGetCertChain1
        *   TlsEcoMecGetCertChain2
        *   TlsEcoMecGetCertChain3
        *   TlsEcoMecGetProfile
        *   TlsEcoMecGetCACertByCert
        *   TlsEcoMecGetCACertByKey
        *   TlsEcoMecGetCACertByIssuer

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/25/03    initial revision.

**********************************************************************/


#include "tls_eco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetTlsSessionManager
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a module element variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     element variable.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetTlsSessionManager
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject     = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hTlsSessionManager;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetTlsCryptoProvider
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a module element variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     element variable.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetTlsCryptoProvider
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject     = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  pMyObject->hTlsCryptoProvider;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetTlsHsmIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a module element variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     element variable.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetTlsHsmIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_SESSION_MANAGER_OBJECT     pTlsSessionManager = (PTLS_SESSION_MANAGER_OBJECT )pMyObject->hTlsSessionManager;
    PTLS_HSM_INTERFACE              pTlsHsmIf          = (PTLS_HSM_INTERFACE          )pTlsSessionManager->hTlsHsmIf;

    return  (ANSC_HANDLE)pTlsHsmIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetModuleParams
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve a module element variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     element variable.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetModuleParams
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;

    return  (ANSC_HANDLE)&pMyObject->ModuleParams;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetCertChain1
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       cert_bits,
                PUCHAR                      cert_id
            );

    description:

        This function is called to retrieve a X.509 certificate chain
        by matching the CertConfigBits.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       cert_bits
                Specifies the CertConfigBits to be matched.

                PUCHAR                      cert_id
                Specifies the output buffer where the CertID is to be
                returned.

    return:     certificate chain.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetCertChain1
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cert_bits,
        PUCHAR                      cert_id
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    ANSC_X509_CERTIFICATE_CHAIN*    pX509CertChain     = (ANSC_X509_CERTIFICATE_CHAIN*)NULL;
    ANSC_X509_CERTIFICATE*          pX509Cert          = (ANSC_X509_CERTIFICATE*      )NULL;
    ULONG                           i                  = 0;

    /*
     * All certificate profiles, key and cryptographic formats are defined by the IETF PKIX working
     * group [PKIX]. When a key usage extension is present, the digitalSignature bit must be set
     * for the key to be eligible for signing, as described above, and the keyEncipherment bit must
     * be present to allow encryption, as described above. The keyAgreement bit must be set on
     * Diffie-Hellman certificates.
     */
    for ( i = 0; i < pModuleParams->CertChainCount; i++ )
    {
        pX509CertChain = pModuleParams->CertChainArray[i];

        if ( !pX509CertChain )
        {
            continue;
        }
        else if ( pX509CertChain->Depth == 0 )
        {
            continue;
        }
        else
        {
            pX509Cert = pX509CertChain->Array[0];
        }

        if ( !pX509Cert )
        {
            continue;
        }
        else if ( cert_bits & TLS_CERT_BIT_RSA_SIGNING_GT512 )
        {
            if ( (pX509Cert->KeyType == ANSC_CERT_keyType_rsa              ) &&
                 (pX509Cert->KeyBits  > 512                                ) &&
                 (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) )
            {
                break;
            }
        }
        else if ( cert_bits & TLS_CERT_BIT_RSA_ENCRYPT_GT512 )
        {
            if ( (pX509Cert->KeyType == ANSC_CERT_keyType_rsa             ) &&
                 (pX509Cert->KeyBits  > 512                               ) &&
                 (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment) )
            {
                break;
            }
        }
        else if ( cert_bits & TLS_CERT_BIT_RSA_SIGNING_LE512 )
        {
            if ( (pX509Cert->KeyType == ANSC_CERT_keyType_rsa              ) &&
                 (pX509Cert->KeyBits <= 512                                ) &&
                 (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) )
            {
                break;
            }
        }
        else if ( cert_bits & TLS_CERT_BIT_RSA_ENCRYPT_LE512 )
        {
            if ( (pX509Cert->KeyType == ANSC_CERT_keyType_rsa             ) &&
                 (pX509Cert->KeyBits <= 512                               ) &&
                 (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_keyEncipherment) )
            {
                break;
            }
        }
        else if ( cert_bits & TLS_CERT_BIT_DSS )
        {
            if ( (pX509Cert->KeyType == ANSC_CERT_keyType_dsa              ) &&
                 (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) )
            {
                break;
            }
        }
        else if ( cert_bits & TLS_CERT_BIT_DH_DSS )
        {
            if ( (pX509Cert->Signing == ANSC_CERT_signing_dsa          ) &&
                 (pX509Cert->KeyType == ANSC_CERT_keyType_diffieHellman) &&
                 (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement) )
            {
                break;
            }
        }
        else if ( cert_bits & TLS_CERT_BIT_DH_RSA )
        {
            if ( (pX509Cert->Signing == ANSC_CERT_signing_rsa          ) &&
                 (pX509Cert->KeyType == ANSC_CERT_keyType_diffieHellman) &&
                 (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement) )
            {
                break;
            }
        }
    }

    if ( i >= pModuleParams->CertChainCount )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        AnscCopyMemory
            (
                cert_id,
                pX509Cert->CertID,
                ANSC_X509_CERT_ID_SIZE
            );
    }

    return  (ANSC_HANDLE)pX509CertChain;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetCertChain2
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      cert_id
            );

    description:

        This function is called to retrieve a X.509 certificate chain
        by matching the CertID.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      cert_id
                Specifies the CertID to be matched.

    return:     certificate chain.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetCertChain2
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      cert_id
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    ANSC_X509_CERTIFICATE_CHAIN*    pX509CertChain     = (ANSC_X509_CERTIFICATE_CHAIN*)NULL;
    ANSC_X509_CERTIFICATE*          pX509Cert          = (ANSC_X509_CERTIFICATE*      )NULL;
    ULONG                           i                  = 0;

    for ( i = 0; i < pModuleParams->CertChainCount; i++ )
    {
        pX509CertChain = pModuleParams->CertChainArray[i];

        if ( !pX509CertChain )
        {
            continue;
        }
        else if ( pX509CertChain->Depth == 0 )
        {
            continue;
        }
        else
        {
            pX509Cert = pX509CertChain->Array[0];
        }

        if ( !pX509Cert )
        {
            continue;
        }
        else if ( AnscEqualMemory
                    (
                        pX509Cert->CertID,
                        cert_id,
                        ANSC_X509_CERT_ID_SIZE
                    ) )
        {
            break;
        }
    }

    if ( i >= pModuleParams->CertChainCount )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pX509CertChain;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetCertChain3
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       client_cert_type,
                ANSC_HANDLE                 authority_name,
                PUCHAR                      cert_id
            );

    description:

        This function is called to retrieve a X.509 certificate chain
        by matching the ClientCertType and the CA's name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       client_cert_type
                Specifies the ClientCertType to be matched.

                ANSC_HANDLE                 authority_name
                Specifies the root CA's name to be matched.

                PUCHAR                      cert_id
                Specifies the output buffer where the CertID is to be
                returned.

    return:     certificate chain.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetCertChain3
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       client_cert_type,
        ANSC_HANDLE                 authority_name,
        PUCHAR                      cert_id
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    ANSC_X501_NAME*                 pIssuerName        = (ANSC_X501_NAME*             )authority_name;
    ANSC_X509_CERTIFICATE_CHAIN*    pX509CertChain     = (ANSC_X509_CERTIFICATE_CHAIN*)NULL;
    ANSC_X509_CERTIFICATE*          pX509Cert          = (ANSC_X509_CERTIFICATE*      )NULL;
    ANSC_X509_CERTIFICATE*          pX509CACert        = (ANSC_X509_CERTIFICATE*      )NULL;
    ANSC_X501_NAME*                 pX501CAName        = (ANSC_X501_NAME*             )NULL;
    BOOL                            bCertFound         = (BOOL                        )FALSE;
    BOOL                            bCertTypeMatched   = (BOOL                        )FALSE;
    ULONG                           i                  = 0;

    /*
     * All certificate profiles, key and cryptographic formats are defined by the IETF PKIX working
     * group [PKIX]. When a key usage extension is present, the digitalSignature bit must be set
     * for the key to be eligible for signing, as described above, and the keyEncipherment bit must
     * be present to allow encryption, as described above. The keyAgreement bit must be set on
     * Diffie-Hellman certificates.
     */
    for ( i = 0; i < pModuleParams->CertChainCount; i++ )
    {
        pX509CertChain = pModuleParams->CertChainArray[i];

        if ( !pX509CertChain )
        {
            continue;
        }
        else if ( pX509CertChain->Depth == 0 )
        {
            continue;
        }
        else
        {
            pX509Cert   = pX509CertChain->Array[0];
            pX509CACert = pX509CertChain->Array[pX509CertChain->Depth - 1];
        }

        if ( !pX509Cert || !pX509CACert )
        {
            continue;
        }
        else
        {
            bCertTypeMatched = FALSE;
        }

        switch ( client_cert_type )
        {
            case    TLS_CLIENT_CERT_TYPE_rsaSign :

                    if ( (pX509Cert->KeyType == ANSC_CERT_keyType_rsa              ) &&
                         (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) )
                    {
                        bCertTypeMatched = TRUE;

                        break;
                    }

                    break;

            case    TLS_CLIENT_CERT_TYPE_dssSign :

                    if ( (pX509Cert->KeyType == ANSC_CERT_keyType_dsa              ) &&
                         (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_digitalSignature) )
                    {
                        bCertTypeMatched = TRUE;

                        break;
                    }

                    break;

            case    TLS_CLIENT_CERT_TYPE_rsaFixedDh :

                    if ( (pX509Cert->Signing == ANSC_CERT_signing_rsa          ) &&
                         (pX509Cert->KeyType == ANSC_CERT_keyType_diffieHellman) &&
                         (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement) )
                    {
                        bCertTypeMatched = TRUE;

                        break;
                    }

                    break;

            case    TLS_CLIENT_CERT_TYPE_dssFixedDh :

                    if ( (pX509Cert->Signing == ANSC_CERT_signing_dsa          ) &&
                         (pX509Cert->KeyType == ANSC_CERT_keyType_diffieHellman) &&
                         (pX509Cert->KeyUsage & ANSC_CERT_keyUsage_keyAgreement) )
                    {
                        bCertTypeMatched = TRUE;

                        break;
                    }

                    break;

            default :

                    break;
        }

        if ( !bCertTypeMatched )
        {
            continue;
        }
        else if ( !pIssuerName )
        {
            bCertFound = TRUE;

            break;
        }
        else
        {
            pX501CAName =
                AnscX509CertGetIssuer
                    (
                        pX509CACert->CertData,
                        pX509CACert->CertSize
                    );

            if ( !pX501CAName )
            {
                continue;
            }
        }

        if ( AnscEqualX501Names(pIssuerName, pX501CAName) )
        {
            bCertFound = TRUE;
        }

        AnscFreeX501Name(pX501CAName);

        if ( bCertFound )
        {
            break;
        }
    }

    if ( i >= pModuleParams->CertChainCount )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        AnscCopyMemory
            (
                cert_id,
                pX509Cert->CertID,
                ANSC_X509_CERT_ID_SIZE
            );
    }

    return  (ANSC_HANDLE)pX509CertChain;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetProfile
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      cert_id
            );

    description:

        This function is called to retrieve a X.509 profile by matching
        the CertID.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      cert_id
                Specifies the CertID to be matched.

    return:     profile.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetProfile
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      cert_id
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    ANSC_X509_PROFILE*              pX509Profile       = (ANSC_X509_PROFILE*          )NULL;
    ANSC_X509_CERTIFICATE*          pX509Cert          = (ANSC_X509_CERTIFICATE*      )NULL;
    ULONG                           i                  = 0;

    for ( i = 0; i < pModuleParams->ProfileCount; i++ )
    {
        pX509Profile = pModuleParams->ProfileArray[i];

        if ( !pX509Profile )
        {
            continue;
        }
        else
        {
            pX509Cert = pX509Profile->Certificate;
        }

        if ( !pX509Cert )
        {
            continue;
        }
        else if ( AnscEqualMemory
                    (
                        pX509Cert->CertID,
                        cert_id,
                        ANSC_X509_CERT_ID_SIZE
                    ) )
        {
            break;
        }
    }

    if ( i > pModuleParams->ProfileCount )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pX509Profile;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetCACertByCert
            (
                ANSC_HANDLE                 hThisObject,
                void*                       cert_data,
                ULONG                       cert_size
            );

    description:

        This function is called to retrieve a X.509 CA's certificate
        by matching the raw certificate data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                void*                       cert_data
                Specifies the raw certificate data to be matched.

                ULONG                       cert_size
                Specifies the raw certificate size to be matched.

    return:     certificate.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetCACertByCert
    (
        ANSC_HANDLE                 hThisObject,
        void*                       cert_data,
        ULONG                       cert_size
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    ANSC_X509_CERTIFICATE*          pX509Cert          = (ANSC_X509_CERTIFICATE*      )NULL;
    ULONG                           i                  = 0;

    for ( i = 0; i < pModuleParams->CACertCount; i++ )
    {
        pX509Cert = pModuleParams->CACertArray[i];

        if ( !pX509Cert )
        {
            continue;
        }
        else if ( pX509Cert->CertSize != cert_size )
        {
            continue;
        }
        else if ( AnscEqualMemory
                    (
                        pX509Cert->CertData,
                        cert_data,
                        cert_size
                    ) )
        {
            break;
        }
    }

    if ( i >= pModuleParams->CACertCount )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pX509Cert;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetCACertByKey
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 issuer_name,
                ANSC_HANDLE                 issuer_key
            );

    description:

        This function is called to retrieve a X.509 CA's certificate
        by matching the issuer's name and public key.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 issuer_name
                Specifies the issuer's name to be matched.

                ANSC_HANDLE                 issuer_key
                Specifies the issuer's key to be matched.

    return:     certificate.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetCACertByKey
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 issuer_name,
        ANSC_HANDLE                 issuer_key
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject          = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pModuleParams      = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    ANSC_X501_NAME*                 pIssuerName        = (ANSC_X501_NAME*             )issuer_name;
    ANSC_X509_KEY*                  pIssuerKey         = (ANSC_X509_KEY*              )issuer_key;
    ANSC_X509_CERTIFICATE*          pX509Cert          = (ANSC_X509_CERTIFICATE*      )NULL;
    ANSC_X501_NAME*                 pX501Name          = (ANSC_X501_NAME*             )NULL;
    ANSC_X509_KEY*                  pX509Key           = (ANSC_X509_KEY*              )NULL;
    ULONG                           i                  = 0;

    for ( i = 0; i < pModuleParams->CACertCount; i++ )
    {
        pX509Cert = pModuleParams->CACertArray[i];

        if ( !pX509Cert )
        {
            continue;
        }
        else
        {
            pX501Name = AnscX509CertGetSubject  (pX509Cert->CertData, pX509Cert->CertSize);
            pX509Key  = AnscX509CertGetPublicKey(pX509Cert->CertData, pX509Cert->CertSize);

            if ( !pX501Name || !pX509Key )
            {
                pX509Cert = NULL;

                goto  EXIT2;
            }
        }

        if ( AnscEqualX501Names(pX501Name, pIssuerName) &&
             AnscEqualX509Keys (pX509Key,  pIssuerKey ) )
        {
            goto  EXIT2;
        }
        else
        {
            AnscFreeX501Name(pX501Name);
            AnscFreeX509Key (pX509Key );
        }
    }

    if ( i >= pModuleParams->CACertCount )
    {
        pX509Cert = NULL;

        goto  EXIT1;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pX501Name )
    {
        AnscFreeX501Name(pX501Name);
    }

    if ( pX509Key )
    {
        AnscFreeX509Key(pX509Key);
    }

EXIT1:

    return  (ANSC_HANDLE)pX509Cert;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        TlsEcoMecGetCACertByIssuer
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 issuer_name,
                ANSC_HANDLE                 issuer_key_id,
                ANSC_HANDLE                 issuer_serial
            );

    description:

        This function is called to retrieve a X.509 CA's certificate
        by matching the issuer's name, key identifier (optional) and
        serial number (optional).

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 issuer_name
                Specifies the issuer's name to be matched.

                ANSC_HANDLE                 issuer_key
                Specifies the issuer's key to be matched.

                ANSC_HANDLE                 issuer_serial
                Specifies the issuer's serial number to be matched.

    return:     certificate.

**********************************************************************/

ANSC_HANDLE
TlsEcoMecGetCACertByIssuer
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 issuer_name,
        ANSC_HANDLE                 issuer_key_id,
        ANSC_HANDLE                 issuer_serial
    )
{
    PTLS_ENV_CONTROLLER_OBJECT      pMyObject           = (PTLS_ENV_CONTROLLER_OBJECT  )hThisObject;
    PTLS_MODULE_PARAMS              pModuleParams       = (PTLS_MODULE_PARAMS          )&pMyObject->ModuleParams;
    ANSC_X501_NAME*                 pIssuerName         = (ANSC_X501_NAME*             )issuer_name;
    ANSC_ASN1_OCTET_STRING*         pIssuerKeyID        = (ANSC_ASN1_OCTET_STRING*     )issuer_key_id;
    ANSC_ASN1_INT*                  pIssuerSerialNumber = (ANSC_ASN1_INT*              )issuer_serial;
    ANSC_X509_CERTIFICATE*          pX509Cert           = (ANSC_X509_CERTIFICATE*      )NULL;
    ANSC_X501_NAME*                 pX501Name           = (ANSC_X501_NAME*             )NULL;
    ANSC_ASN1_OCTET_STRING*         pCertKeyID          = (ANSC_ASN1_OCTET_STRING*     )NULL;
    ANSC_ASN1_INT*                  pCertSerialNumber   = (ANSC_ASN1_INT*              )NULL;
    ULONG                           i                   = 0;

    for ( i = 0; i < pModuleParams->CACertCount; i++ )
    {
        pX509Cert = pModuleParams->CACertArray[i];

        if ( !pX509Cert )
        {
            continue;
        }
        else
        {
            pX501Name         = AnscX509CertGetSubject     (pX509Cert->CertData, pX509Cert->CertSize);
            pCertKeyID        = AnscX509CertGetSubjectKeyID(pX509Cert->CertData, pX509Cert->CertSize);
            pCertSerialNumber = AnscX509CertGetSerialNumber(pX509Cert->CertData, pX509Cert->CertSize);

            if ( !pX501Name || !pCertSerialNumber )
            {
                pX509Cert = NULL;

                goto  EXIT2;
            }
        }

        do
        {
            if ( !AnscEqualX501Names(pX501Name, pIssuerName) )
            {
                break;
            }

            if ( pCertKeyID && pIssuerKeyID )
            {
                 if ( !AnscAsn1EqualOctetStrings(pCertKeyID, pIssuerKeyID) )
                 {
                     break;
                 }
            }

            if ( pIssuerSerialNumber )
            {
                 if ( !AnscAsn1EqualInts(pCertSerialNumber,  pIssuerSerialNumber) )
                 {
                     break;
                 }
            }

            goto  EXIT2;
        }
        while ( FALSE );

        if ( TRUE )
        {
            if ( pX501Name )
            {
                AnscFreeX501Name(pX501Name);

                pX501Name = NULL;
            }

            if ( pCertKeyID )
            {
                AnscAsn1FreeOctetString(pCertKeyID);

                pCertKeyID = NULL;
            }

            if ( pCertSerialNumber )
            {
                AnscAsn1FreeInt(pCertSerialNumber);

                pCertSerialNumber = NULL;
            }
        }
    }

    if ( i >= pModuleParams->CACertCount )
    {
        pX509Cert = NULL;

        goto  EXIT1;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT2:

    if ( pX501Name )
    {
        AnscFreeX501Name(pX501Name);
    }

    if ( pCertKeyID )
    {
        AnscAsn1FreeOctetString(pCertKeyID);
    }

    if ( pCertSerialNumber )
    {
        AnscAsn1FreeInt(pCertSerialNumber);
    }

EXIT1:

    return  (ANSC_HANDLE)pX509Cert;
}
