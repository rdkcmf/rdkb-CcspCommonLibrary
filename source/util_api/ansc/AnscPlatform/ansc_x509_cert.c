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

    module:	ansc_x509_cert.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to X.509 certificate
        handling.

        *   AnscX509CertGetCertWrap
        *   AnscX509CertGetIssuer
        *   AnscX509CertGetSubject
        *   AnscX509CertGetIssuerKeyID
        *   AnscX509CertGetSubjectKeyID
        *   AnscX509CertGetSerialNumber
        *   AnscX509CertGetPublicKey
        *   AnscX509CertIsSelfSigned
        *   AnscX509CertGetKeyType

        static functions are used to implement above apis:

        *   anscGetCertHandle
        *   anscFreeCertHandle

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/07/03    initial revision.
        07/03/03    implement all the funcitons by Bin Zhu

**********************************************************************/

#include "ansc_global.h"

/*
 *  The pki header files should be included
 *
 *  please get them from "\bisga\isc\asn.1\include"
 */
#include "ansc_pki_global.h"

#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"


/*********************************************************************************
 *
 *   static functions implemented by Bin
 *
 *********************************************************************************/
static ANSC_HANDLE
anscGetCertHandle
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    PUCHAR                          pBack           = (PUCHAR)raw_cert_data;

    if( raw_cert_size < 32 || raw_cert_data == NULL)
    {
        return NULL;
    }

    /* create the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)AnscAsn1CreateCertificate(NULL);

    if( pAsnCert == NULL)
    {
        return NULL;
    }

    /* decode it */
    if( ANSC_STATUS_SUCCESS != pAsnCert->DecodingData(pAsnCert, (PVOID*)&pBack))
    {
        pAsnCert->AsnFree(pAsnCert);

        return NULL;
    }

    return (ANSC_HANDLE)pAsnCert;
}

static void
anscFreeCertHandle
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_CERTIFICATE          pThisObject  = (PANSC_ASN1_CERTIFICATE)hThisObject;

    if( pThisObject != NULL)
    {
        pThisObject->AsnFree(pThisObject);
    }
}

/*********************************************************************************
 *
 *   Utility functions
 *
 *********************************************************************************/
ANSC_X509_CERTIFICATE*
AnscX509CertGetCertWrap
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    ANSC_X509_CERTIFICATE*          pX509Cert       = NULL;
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    ULONG                           keyUsage        = 0;
    ANSC_CRYPTO_HASH                hash;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    /* create the X509 Certificate */
    pX509Cert = (PANSC_X509_CERTIFICATE)AnscAllocateMemory(sizeof(ANSC_X509_CERTIFICATE));

    if( pX509Cert == NULL)
    {
        goto EXIT;
    }

    /* init the pX509Cert */
    pX509Cert->CertSize             = raw_cert_size;
    pX509Cert->CertData             = (PVOID)AnscAllocateMemory(raw_cert_size);

    if( pX509Cert->CertData != NULL)
    {
        AnscCopyMemory( pX509Cert->CertData, raw_cert_data, raw_cert_size);
    }

    pX509Cert->bSelfSigned          = pAsnCert->IsSelfSigned(pAsnCert);

    if( pAsnCert->GetKeyType(pAsnCert) == PKI_RSA_KEY)
    {
        pX509Cert->Signing          = ANSC_CERT_signing_rsa;
        pX509Cert->KeyType          = ANSC_CERT_keyType_rsa;
    }
    else if( pAsnCert->GetKeyType(pAsnCert) == PKI_DSA_KEY)
    {
        pX509Cert->Signing          = ANSC_CERT_signing_dsa;
        pX509Cert->KeyType          = ANSC_CERT_keyType_dsa;
    }
    else
    {
        pX509Cert->KeyType          = ANSC_CERT_keyType_diffieHellman;
    }

    /* get the key length */
    pX509Cert->KeyBits              = pAsnCert->GetKeyBits(pAsnCert);

    /* get the key usage; */
    if( ANSC_STATUS_SUCCESS == pAsnCert->GetKeyUsage(pAsnCert, &keyUsage))
    {
        /* the value has to be reversed */
        pX509Cert->KeyUsage         = 0;

        if( keyUsage >= 0x0100)
        {
            pX509Cert->KeyUsage     +=  ANSC_CERT_keyUsage_deciperOnly;
        }

        keyUsage            = keyUsage & 0x000000FF;

        if( keyUsage & 0x00000001)
        {
            pX509Cert->KeyUsage += 0x000000080;
        }
        if( keyUsage & 0x00000002)
        {
            pX509Cert->KeyUsage += 0x000000040;
        }
        if( keyUsage & 0x00000004)
        {
            pX509Cert->KeyUsage += 0x000000020;
        }
        if( keyUsage & 0x00000008)
        {
            pX509Cert->KeyUsage += 0x000000010;
        }
        if( keyUsage & 0x00000010)
        {
            pX509Cert->KeyUsage += 0x000000008;
        }
        if( keyUsage & 0x00000020)
        {
            pX509Cert->KeyUsage += 0x000000004;
        }
        if( keyUsage & 0x00000040)
        {
            pX509Cert->KeyUsage += 0x000000002;
        }
        if( keyUsage & 0x00000080)
        {
            pX509Cert->KeyUsage += 0x000000001;
        }
    }
    else
    {
        /*
         * For the old cert without KeyUsage extension, we assume it
         * has all the usages
         */
        pX509Cert->KeyUsage = 0x000000FF;
    }

    /* get the md5 hash of the raw stream and set it as the CertID */
    hash.Length = ANSC_MD5_OUTPUT_SIZE;

    hash.Length  = AnscCryptoMd5Digest(raw_cert_data, raw_cert_size, &hash);

    AnscCopyMemory
        (
            pX509Cert->CertID,
            hash.Value,
            hash.Length
        );

#if 1
    if( pAsnCert->IsSelfSigned(pAsnCert))
    {
        if(!pAsnCert->VerifySignature(pAsnCert, NULL))
        {
            AnscTrace("Warning: Failed to verify the self-signed certificate.\n");
        }
    }
#endif

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    return  pX509Cert;
}


ANSC_X501_NAME*
AnscX509CertGetIssuer
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    ANSC_X501_NAME*                 pIssuerName     = NULL;
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    PANSC_ASN1_NAME                 pAsnName        = NULL;
    PUCHAR                          pBack           = (PUCHAR)raw_cert_data;
    ULONG                           length;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    /* get the asn1 issuer name object */
    pAsnName = (PANSC_ASN1_NAME)pAsnCert->GetIssuerHandle(pAsnCert);

    if( pAsnName == NULL || pAsnName->bOptional)
    {
        goto EXIT;
    }

    length = pAsnName->GetSizeOfEncoded(pAsnName);

    /* create the X501Name object */
    pIssuerName = (PANSC_X501_NAME)AnscAllocateMemory(sizeof(ULONG) + length);

    if( pIssuerName == NULL)
    {
        goto EXIT;
    }

    pIssuerName->Length = length;
    pBack = pIssuerName->Data;
    pAsnName->EncodingData(pAsnName, (PVOID*)&pBack);

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    return  pIssuerName;
}


ANSC_X501_NAME*
AnscX509CertGetSubject
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    ANSC_X501_NAME*                 pSubjectName    = NULL;
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    PANSC_ASN1_NAME                 pAsnName        = NULL;
    PUCHAR                          pBack           = (PUCHAR)raw_cert_data;
    ULONG                           length;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    /* get the asn1 subject name object */
    pAsnName = (PANSC_ASN1_NAME)pAsnCert->GetSubjectHandle(pAsnCert);

    if( pAsnName == NULL || pAsnName->bOptional)
    {
        goto EXIT;
    }

    length = pAsnName->GetSizeOfEncoded(pAsnName);

    /* create the X501Name object */
    pSubjectName = (PANSC_X501_NAME)AnscAllocateMemory(sizeof(ULONG) + length);

    if( pSubjectName == NULL)
    {
        goto EXIT;
    }

    pSubjectName->Length = length;
    pBack = pSubjectName->Data;
    pAsnName->EncodingData(pAsnName, (PVOID*)&pBack);

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    return  pSubjectName;
}


ANSC_ASN1_OCTET_STRING*
AnscX509CertGetIssuerKeyID
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    ANSC_ASN1_OCTET_STRING*         pOctets         = NULL;
    PUCHAR                          pKeyID          = NULL;
    ULONG                           length          = 0;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    /* get the authority  identifier */
    if( ANSC_STATUS_SUCCESS !=
            pAsnCert->GetAuthorityIdentifier(pAsnCert, &pKeyID, &length))
    {
        length = 0;
    }

    if( length > 0 && pKeyID != NULL)
    {
        pOctets =
            (ANSC_ASN1_OCTET_STRING*)
            AnscAsn1AllocOctetString( pKeyID, length);
    }

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    if( pKeyID != NULL)
    {
        AnscFreeMemory(pKeyID);
    }

    return  pOctets;
}


ANSC_ASN1_OCTET_STRING*
AnscX509CertGetSubjectKeyID
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    ANSC_ASN1_OCTET_STRING*         pOctets         = NULL;
    PUCHAR                          pKeyID          = NULL;
    ULONG                           length          = 0;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    /* get the subject key  identifier */
    if( ANSC_STATUS_SUCCESS !=
            pAsnCert->GetSubjectKeyIdentifier(pAsnCert, &pKeyID, &length))
    {
        length = 0;
    }

    if( length > 0 && pKeyID != NULL)
    {
        pOctets =
            (ANSC_ASN1_OCTET_STRING*)
            AnscAsn1AllocOctetString( pKeyID, length);
    }

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    if( pKeyID != NULL)
    {
        AnscFreeMemory(pKeyID);
    }

    return  pOctets;
}


ANSC_ASN1_INT*
AnscX509CertGetSerialNumber
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    ANSC_ASN1_INT*                  pIntValue       = NULL;
    PANSC_ASN1_INTEGER              pSerialObj;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    /* get the serial number */
    pSerialObj = (PANSC_ASN1_INTEGER)pAsnCert->GetSerialNumber(pAsnCert);

    if( pSerialObj == NULL || pSerialObj->bOptional)
    {
        goto EXIT;
    }

    pIntValue =
        (ANSC_ASN1_INT*)AnscAsn1AllocInt
            (
                (PUCHAR)pSerialObj->GetValueBuffer(pSerialObj),
                pSerialObj->uLength,
                FALSE
            );

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    return pIntValue;
}


ANSC_X509_KEY*
AnscX509CertGetPublicKey
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    ANSC_X509_KEY*                  pX509Key        = NULL;
    PANSC_CRYPTO_PUB_KEY_GEN_PARAMS pGenParams;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    /* export the public key */
    pGenParams = (PANSC_CRYPTO_PUB_KEY_GEN_PARAMS)
        AnscAllocateMemory(sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS));

    if( pGenParams == NULL)
    {
        goto EXIT;
    }

    if( ANSC_STATUS_SUCCESS !=
            pAsnCert->ExportPublicKey(pAsnCert, pGenParams))
    {
        AnscFreeMemory(pGenParams);

        goto EXIT;
    }

    /* create the X509_KEY */
    pX509Key = (PANSC_X509_KEY)AnscAllocateMemory(sizeof(ANSC_X509_KEY));

    if( pX509Key == NULL)
    {
        AnscFreeMemory(pGenParams);

        goto EXIT;
    }

    pX509Key->KeySize       = sizeof(ANSC_CRYPTO_PUB_KEY_GEN_PARAMS);
    pX509Key->KeyData       = (void*)pGenParams;

    if( pAsnCert->GetKeyType(pAsnCert) == PKI_RSA_KEY)
    {
        pX509Key->KeyType          = ANSC_CERT_keyType_rsa;
    }
    else if( pAsnCert->GetKeyType(pAsnCert) == PKI_DSA_KEY)
    {
        pX509Key->KeyType          = ANSC_CERT_keyType_dsa;
    }

    pX509Key->KeyBits              = pAsnCert->GetKeyBits(pAsnCert);

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    return pX509Key;
}

BOOL
AnscX509CertIsSelfSigned
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    BOOL                            bIsSelfSigned   = FALSE;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    bIsSelfSigned = pAsnCert->IsSelfSigned(pAsnCert);

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    return  bIsSelfSigned;
}


ULONG
AnscX509CertGetKeyType
    (
        void*                       raw_cert_data,
        ULONG                       raw_cert_size,
        PULONG                      cert_signing,
        PULONG                      cert_key_usage,
        PULONG                      cert_key_bits
    )
{
    PANSC_ASN1_CERTIFICATE          pAsnCert        = NULL;
    ULONG                           keyType         = ANSC_CERT_keyType_rsa;
    ULONG                           keyUsage;

    /* decode the asn.1 certificate handle */
    pAsnCert = (PANSC_ASN1_CERTIFICATE)
        anscGetCertHandle
            (
                raw_cert_data,
                raw_cert_size
            );

    if( pAsnCert == NULL)
    {
        goto EXIT;
    }

    if( pAsnCert->GetKeyType(pAsnCert) == PKI_RSA_KEY)
    {
        keyType          = ANSC_CERT_keyType_rsa;
        *cert_signing    = ANSC_CERT_signing_rsa;
    }
    else if( pAsnCert->GetKeyType(pAsnCert) == PKI_DSA_KEY)
    {
        keyType          = ANSC_CERT_keyType_dsa;
        *cert_signing    = ANSC_CERT_signing_dsa;
    }
    else
    {
        keyType          = ANSC_CERT_keyType_diffieHellman;
    }

    *cert_key_bits       = pAsnCert->GetKeyBits(pAsnCert);

    /* get the key usage; */
    if( ANSC_STATUS_SUCCESS == pAsnCert->GetKeyUsage(pAsnCert, &keyUsage))
    {
        if( keyUsage >= 0x0100)
        {
            *cert_key_usage     |=  ANSC_CERT_keyUsage_deciperOnly;
        }

        *cert_key_usage         += keyUsage & 0x000000FF;
    }

EXIT:

    anscFreeCertHandle((ANSC_HANDLE)pAsnCert);

    return  keyType;
}

