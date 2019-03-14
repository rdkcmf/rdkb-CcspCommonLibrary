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

    module:	ansc_cert_definitions.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some of the popular object and
        Certificate data structures used across ANSC platform.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/17/03    initial revision.

**********************************************************************/


#ifndef  _ANSC_CERT_DEFINITIONS_
#define  _ANSC_CERT_DEFINITIONS_


/***********************************************************
        STANDARD CERT FUNDAMANTAL DATA TYPE DEFINITION
***********************************************************/

/*
 * Users of a public key shall be confident that the associated private key is owned by the correct
 * remote subject (person or system) with which an encryption or digital signature mechanism will
 * be used. This confidence is obtained through the use of public key certificates, which are data
 * structures that bind public key values to subjects. The binding is asserted by having a trusted
 * CA digitally sign each certificate. The CA may base this assertion upon technical means (a.k.a.,
 * proof of posession through a challenge-response protocol), presentation of the private key, or
 * on an assertion by the subject. A certificate has a limited valid lifetime which is indicated in
 * its signed contents. Because a certificate's signature and timeliness can be independently
 * checked by a certificate-using client, certificates can be distributed via untrusted communi-
 * cations and server systems, and can be cached in unsecured storage in certificate-using systems.
 */
#define  ANSC_X509_CERT_ID_SIZE                     16

#define  ANSC_CERT_signing_rsa                      1
#define  ANSC_CERT_signing_dsa                      2

#define  ANSC_CERT_keyType_rsa                      1
#define  ANSC_CERT_keyType_dsa                      2
#define  ANSC_CERT_keyType_diffieHellman            3

#define  ANSC_CERT_keyUsage_digitalSignature        0x00000001
#define  ANSC_CERT_keyUsage_nonRepudiation          0x00000002
#define  ANSC_CERT_keyUsage_keyEncipherment         0x00000004
#define  ANSC_CERT_keyUsage_dataEncipherment        0x00000008
#define  ANSC_CERT_keyUsage_keyAgreement            0x00000010
#define  ANSC_CERT_keyUsage_keyCertSign             0x00000020
#define  ANSC_CERT_keyUsage_cRLSign                 0x00000040
#define  ANSC_CERT_keyUsage_enciperOnly             0x00000080
#define  ANSC_CERT_keyUsage_deciperOnly             0x00000100

typedef  struct
_ANSC_X509_CERTIFICATE
{
    BOOL                            bSelfSigned;
    ULONG                           Signing;
    ULONG                           KeyType;
    ULONG                           KeyBits;
    ULONG                           KeyUsage;
    UCHAR                           CertID[ANSC_X509_CERT_ID_SIZE];
    ULONG                           CertSize;
    void*                           CertData;
}
ANSC_X509_CERTIFICATE,  *PANSC_X509_CERTIFICATE;

#define  AnscCloneX509Certificate(src_cert, dst_cert)                                       \
         {                                                                                  \
            if ( !dst_cert )                                                                \
            {                                                                               \
                dst_cert =                                                                  \
                    (PANSC_X509_CERTIFICATE)AnscAllocateMemory                              \
                        (                                                                   \
                            sizeof(ANSC_X509_CERTIFICATE)                                   \
                        );                                                                  \
            }                                                                               \
                                                                                            \
            if ( dst_cert )                                                                 \
            {                                                                               \
                AnscCleanX509Certificate(dst_cert);                                         \
                                                                                            \
                AnscCopyMemory                                                              \
                    (                                                                       \
                        dst_cert->CertID,                                                   \
                        src_cert->CertID,                                                   \
                        ANSC_X509_CERT_ID_SIZE                                              \
                    );                                                                      \
                                                                                            \
                dst_cert->bSelfSigned = src_cert->bSelfSigned;                              \
                dst_cert->Signing     = src_cert->Signing;                                  \
                dst_cert->KeyType     = src_cert->KeyType;                                  \
                dst_cert->KeyBits     = src_cert->KeyBits;                                  \
                dst_cert->KeyUsage    = src_cert->KeyUsage;                                 \
                dst_cert->CertSize    = src_cert->CertSize;                                 \
                dst_cert->CertData    = AnscAllocateMemory(dst_cert->CertSize);             \
                                                                                            \
                if ( dst_cert->CertData )                                                   \
                {                                                                           \
                    AnscCopyMemory                                                          \
                        (                                                                   \
                            dst_cert->CertData,                                             \
                            src_cert->CertData,                                             \
                            src_cert->CertSize                                              \
                        );                                                                  \
                }                                                                           \
            }                                                                               \
         }

#define  AnscCleanX509Certificate(tbc_cert)                                                 \
         {                                                                                  \
            if ( tbc_cert->CertData )                                                       \
            {                                                                               \
                AnscFreeMemory(tbc_cert->CertData);                                         \
                                                                                            \
                tbc_cert->CertSize = 0;                                                     \
                tbc_cert->CertData = NULL;                                                  \
            }                                                                               \
         }

#define  AnscFreeX509Certificate(tbf_cert)                                                  \
         {                                                                                  \
            AnscCleanX509Certificate(tbf_cert);                                             \
            AnscFreeMemory          (tbf_cert);                                             \
         }

/*
 * A user of a security service requiring knowledge of a public key generally needs to obtain and
 * validate a certificate containing the required public key. If the public-key user does not
 * already hold an assured copy of the public key of the CA that signed the certificate, the CA's
 * name, and related information (such as the validity period or name constraints), then it might
 * need an additional certificate to obtain that public key.  In general, a chain of multiple
 * certificates may be needed, comprising a certificate of the public key owner (the end entity)
 * signed by one CA, and zero or more additional certificates of CAs signed by other CAs. Such
 * chains, called certification paths, are required because a public key user is only initialized
 * with a limited number of assured CA public keys.
 */
#define  ANSC_CERT_MAX_CHAIN_DEPTH                  64

typedef  struct
_ANSC_X509_CERTIFICATE_CHAIN
{
    ULONG                           Depth;
    ANSC_X509_CERTIFICATE*          Array[ANSC_CERT_MAX_CHAIN_DEPTH];
}
ANSC_X509_CERTIFICATE_CHAIN,  *PANSC_X509_CERTIFICATE_CHAIN;

typedef  struct  _ANSC_X509_CERTIFICATE_CHAIN  ANSC_X509_CERT_CHAIN,  *PANSC_X509_CERT_CHAIN;

#define  AnscCloneX509CertChain(src_cert_chain, dst_cert_chain)                             \
         {                                                                                  \
            ULONG                   kk = 0;                                                 \
                                                                                            \
            if ( !dst_cert_chain )                                                          \
            {                                                                               \
                dst_cert_chain =                                                            \
                    (PANSC_X509_CERTIFICATE_CHAIN)AnscAllocateMemory                        \
                        (                                                                   \
                            sizeof(ANSC_X509_CERTIFICATE_CHAIN)                             \
                        );                                                                  \
            }                                                                               \
                                                                                            \
            if ( dst_cert_chain )                                                           \
            {                                                                               \
                AnscCleanX509CertChain(dst_cert_chain);                                     \
                                                                                            \
                dst_cert_chain->Depth = src_cert_chain->Depth;                              \
                                                                                            \
                for ( kk = 0; kk < dst_cert_chain->Depth; kk++ )                            \
                {                                                                           \
                    AnscCloneX509Certificate                                                \
                        (                                                                   \
                            src_cert_chain->Array[kk],                                      \
                            dst_cert_chain->Array[kk]                                       \
                        );                                                                  \
                }                                                                           \
            }                                                                               \
         }

#define  AnscCleanX509CertChain(tbc_cert_chain)                                             \
         {                                                                                  \
            ULONG                   kk = 0;                                                 \
                                                                                            \
            for ( kk = 0; kk < tbc_cert_chain->Depth; kk++ )                                \
            {                                                                               \
                if ( tbc_cert_chain->Array[kk] )                                            \
                {                                                                           \
                    AnscFreeX509Certificate(tbc_cert_chain->Array[kk]);                     \
                                                                                            \
                    tbc_cert_chain->Array[kk] = NULL;                                       \
                }                                                                           \
            }                                                                               \
         }

#define  AnscFreeX509CertChain(tbf_cert_chain)                                              \
         {                                                                                  \
            AnscCleanX509CertChain(tbf_cert_chain);                                         \
            AnscFreeMemory        (tbf_cert_chain);                                         \
         }

/*
 * The issuer field identifies the entity who has signed and issued the certificate. The issuer
 * field MUST contain a non-empty distinguished name (DN). The issuer field is defined as the X.501
 * type Name. [X.501] Name is defined by the following ASN.1 structures:
 *
 *      Name                      ::= CHOICE { RDNSequence }
 *      RDNSequence               ::= SEQUENCE OF RelativeDistinguishedName
 *      RelativeDistinguishedName ::= SET OF AttributeTypeAndValue
 *
 *      AttributeTypeAndValue     ::= SEQUENCE {
 *                                      type     AttributeType,
 *                                      value    AttributeValue
 *                                    }
 *
 *      AttributeType             ::= OBJECT IDENTIFIER
 *      AttributeValue            ::= ANY DEFINED BY AttributeType
 *
 *      DirectoryString           ::= CHOICE {
 *                                      teletexString           TeletexString (SIZE (1..MAX)),
 *                                      printableString         PrintableString (SIZE (1..MAX)),
 *                                      universalString         UniversalString (SIZE (1..MAX)),
 *                                      utf8String              UTF8String (SIZE (1.. MAX)),
 *                                      bmpString               BMPString (SIZE (1..MAX))
 *                                    }
 */
typedef  struct
_ANSC_X501_NAME
{
    ULONG                           Length;
    UCHAR                           Data[ANSC_ZERO_ARRAY_SIZE];
}
ANSC_X501_NAME,  *PANSC_X501_NAME;

#define  AnscEqualX501Names(x_name1, x_name2)                                               \
         ( (x_name1->Length == x_name2->Length) && AnscEqualMemory(x_name1->Data, x_name2->Data, AnscGetMin2(x_name1->Length, x_name2->Length)) )

#define  AnscCloneX501Name(src_name, dst_name)                                              \
         {                                                                                  \
            ULONG                   name_size = sizeof(ANSC_X501_NAME) + src_name->Length;  \
                                                                                            \
            dst_name = (PANSC_X501_NAME)AnscAllocateMemory(name_size);                      \
                                                                                            \
            if ( dst_name )                                                                 \
            {                                                                               \
                AnscCopyMemory(dst_name, src_name, name_size);                              \
            }                                                                               \
         }

#define  AnscFreeX501Name(tbf_name)                 AnscFreeMemory(tbf_name)


/***********************************************************
      STANDARD PROFILE FUNDAMANTAL DATA TYPE DEFINITION
***********************************************************/

/*
 * When used for public-key based authentication, a X.509 certificate is useless without the corres-
 * ponding private key. As a matter of fact, the Subject of any certificate must possess the right
 * private key in order to take advantage of the X.509 security offerings. We use the following
 * data structure to store the public/private key pair along with the corresponding certificate.
 */
typedef  struct
_ANSC_X509_KEY
{
    ULONG                           KeyType;
    ULONG                           KeyBits;
    ULONG                           KeySize;
    void*                           KeyData;
}
ANSC_X509_KEY,  *PANSC_X509_KEY;

#define  AnscEqualX509Keys(x_key1, x_key2)                                                  \
         ( (x_key1->KeySize == x_key2->KeySize) && AnscEqualMemory(x_key1->KeyData, x_key2->KeyData, AnscGetMin2(x_key1->KeySize, x_key2->KeySize)) )

#define  AnscCloneX509Key(src_key, dst_key)                                                 \
         {                                                                                  \
            if ( !dst_key )                                                                 \
            {                                                                               \
                dst_key = (PANSC_X509_KEY)AnscAllocateMemory(sizeof(ANSC_X509_KEY));        \
            }                                                                               \
                                                                                            \
            if ( dst_key )                                                                  \
            {                                                                               \
                AnscCleanX509Key(dst_key);                                                  \
                                                                                            \
                dst_key->KeyType = src_key->KeyType;                                        \
                dst_key->KeyBits = src_key->KeyBits;                                        \
                dst_key->KeySize = src_key->KeySize;                                        \
                dst_key->KeyData = AnscAllocateMemory(dst_key->KeySize);                    \
                                                                                            \
                if ( dst_key->KeyData )                                                     \
                {                                                                           \
                    AnscCopyMemory                                                          \
                        (                                                                   \
                            dst_key->KeyData,                                               \
                            src_key->KeyData,                                               \
                            src_key->KeySize                                                \
                        );                                                                  \
                }                                                                           \
            }                                                                               \
         }

#define  AnscCleanX509Key(tbc_key)															\
         {                                                                                  \
            if ( tbc_key->KeyData )                                                         \
            {                                                                               \
                AnscFreeMemory(tbc_key->KeyData);                                           \
                                                                                            \
                tbc_key->KeySize = 0;                                                       \
                tbc_key->KeyData = NULL;                                                    \
            }                                                                               \
         }

#define  AnscFreeX509Key(tbf_key)                                                           \
         {                                                                                  \
            AnscCleanX509Key(tbf_key);                                                      \
            AnscFreeMemory  (tbf_key);                                                      \
         }

/*
 * We define a X.509 Profile as a binding between a Public/Private Key pair and a X.509 certificate.
 * This definition can help us to simplify the internal configuration process. Note that the PKCS
 * standards don't apply to the internal presentation of the certificate, nor the key pairs.
 */
typedef  struct
_ANSC_X509_PROFILE
{
    ANSC_X509_KEY*                  PubKey;
    ANSC_X509_KEY*                  PrvKey;
    ANSC_X509_CERTIFICATE*          Certificate;
}
ANSC_X509_PROFILE,  *PANSC_X509_PROFILE;

#define  AnscCloneX509Profile(s_profile, d_profile)                                         \
         {                                                                                  \
            if ( !d_profile )                                                               \
            {                                                                               \
                d_profile =                                                                 \
                    (PANSC_X509_PROFILE)AnscAllocateMemory                                  \
                        (                                                                   \
                            sizeof(ANSC_X509_PROFILE)                                       \
                        );                                                                  \
                                                                                            \
                if ( d_profile )                                                            \
                {                                                                           \
                    d_profile->PubKey      = NULL;                                          \
                    d_profile->PrvKey      = NULL;                                          \
                    d_profile->Certificate = NULL;                                          \
                }                                                                           \
            }                                                                               \
                                                                                            \
            if ( d_profile )                                                                \
            {                                                                               \
                AnscCleanX509Profile(d_profile);                                            \
                                                                                            \
                AnscCloneX509Key        (s_profile->PubKey,      d_profile->PubKey     );   \
                AnscCloneX509Key        (s_profile->PrvKey,      d_profile->PrvKey     );   \
                AnscCloneX509Certificate(s_profile->Certificate, d_profile->Certificate);   \
            }                                                                               \
         }

#define  AnscCleanX509Profile(tbc_profile)													\
         {                                                                                  \
            if ( tbc_profile->PubKey )                                                      \
            {                                                                               \
                AnscFreeX509Key(tbc_profile->PubKey);                                       \
                                                                                            \
                tbc_profile->PubKey = NULL;                                                 \
            }                                                                               \
                                                                                            \
            if ( tbc_profile->PrvKey )                                                      \
            {                                                                               \
                AnscFreeX509Key(tbc_profile->PrvKey);                                       \
                                                                                            \
                tbc_profile->PrvKey = NULL;                                                 \
            }                                                                               \
                                                                                            \
            if ( tbc_profile->Certificate )                                                 \
            {                                                                               \
                AnscFreeX509Certificate(tbc_profile->Certificate);                          \
                                                                                            \
                tbc_profile->Certificate = NULL;                                            \
            }                                                                               \
         }

#define  AnscFreeX509Profile(tbf_profile)                                                   \
         {                                                                                  \
            AnscCleanX509Profile(tbf_profile);                                              \
            AnscFreeMemory      (tbf_profile);                                              \
         }


#endif
