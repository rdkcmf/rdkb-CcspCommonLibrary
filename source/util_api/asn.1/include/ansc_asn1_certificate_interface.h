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

    MODULE: ansc_asn1_Certificate_interface.h

        ASN.1 ANSC Code Generated by Cisco Systems, Inc.

    ---------------------------------------------------------------

    DESCRIPTION:

        The ASN.1 object defined in this file

        *   ANSC_ASN1_CERTIFICATE
        *   ANSC_ASN1_TBSCERTIFICATE
        *   ANSC_ASN1_CERTIFICATESERIALNUMBER
        *   ANSC_ASN1_VALIDITY
        *   ANSC_ASN1_TIME
        *   ANSC_ASN1_UNIQUEIDENTIFIER
        *   ANSC_ASN1_SUBJECTPUBLICKEYINFO
        *   ANSC_ASN1_DSASIGNATURE
        *   ANSC_ASN1_ALSTRING
        *   ANSC_ASN1_ATTRIBUTES

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        ASNMAGIC ANSC CODE GENERATOR 1.0

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   05/07/2002  initial revision
        *   05/09/2002  more functions are added for Certificate object.
        *   08/08/2002  more functions are added for Certificate object.
        *   11/25/2003  add function to check the cert expired, in the
                        future or valid now.

 **********************************************************************/


#ifndef  _ANSC_ASN1_CERTIFICATE_INTERFACE_H
#define  _ANSC_ASN1_CERTIFICATE_INTERFACE_H

/**********************************************************************

 OBJECT -- ANSC_ASN1_CERTIFICATE

 Certificate ::= Sequence 
     {
                    tbsCertificate TBSCertificate 
                signatureAlgorithm SignatureAlgorithmIdentifier 
                    signatureValue BitString 
     }

 **********************************************************************/
typedef ANSC_STATUS
(*PFN_CERT_CHECK_VALIDITY)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bCheckTime
    );

typedef ANSC_STATUS
(*PFN_CERT_GET_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTime
    );

typedef BOOLEAN
(*PFN_CERT_IS_SELF_SIGNED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef BOOLEAN
(*PFN_CERT_IS_TIME_VALID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       year,
        ULONG                       month,
        ULONG                       day,
        ULONG                       hour,
        ULONG                       minute,
        ULONG                       second
    );

typedef BOOLEAN
(*PFN_CERT_IS_VALID_NOW)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ULONG
(*PFN_CERT_CHECK_TIME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef PKI_KEY_TYPE
(*PFN_CERT_GET_KEY_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ULONG
(*PFN_CERT_GETKEY_BITS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef SIGNATURE_TYPE
(*PFN_CERT_GET_SIGNATURE_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef BOOLEAN
(*PFN_CERT_SET_SIGNATURE_ALG)
    (
        ANSC_HANDLE                 hThisObject,
        SIGNATURE_TYPE              signType
    );

typedef BOOLEAN
(*PFN_CERT_SET_PUBLIC_KEYINFO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKeyHandle
    );

typedef BOOLEAN
(*PFN_CERT_VERIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPublicKeyHandle
    );

typedef BOOLEAN
(*PFN_CERT_PATH_VALID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       pathLength
    );

typedef ANSC_STATUS
(*PFN_CERT_VERIFY_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildCert
    );

typedef BOOLEAN
(*PFN_CERT_SIGN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hData
    );

typedef BOOLEAN
(*PFN_CERT_SIGN_PARAM)
    (
        ANSC_HANDLE                 hThisObject,
        PKI_KEY_TYPE                keyType,
        ANSC_HANDLE                 hData
    );

typedef BOOLEAN
(*PFN_CERT_IS_ISSUER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hIssuerCert
    );

typedef ANSC_HANDLE
(*PFN_CERT_FIND_EXTENSION_1)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDValueString,
        BOOLEAN                     bReturnValueHandle
    );

typedef ANSC_STATUS
(*PFN_CERT_EXPORT_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKeyGenParams
    );

/*
 *  Get the extension with the type.
 *
 *  The extension type was defined in 
 *  "ansc_asn1_extensions_interface.h"
 */
typedef ANSC_HANDLE
(*PFN_CERT_FIND_EXTENSION_2)
    (
        ANSC_HANDLE                 hThisObject,
        UINT                        uExtentionType,
        BOOLEAN                     bReturnValueHandle
    );

typedef ANSC_HANDLE
(*PFN_CERT_FIND_EXTENSION_EX)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bReturnValueHandle
    );

typedef ANSC_STATUS
(*PFN_CERT_GET_BASIC_CONSTAINT)
    (
        ANSC_HANDLE                 hThisObject,
        PBOOLEAN                    pIsCA,
        PLONG                       pPathLength
    );

typedef ANSC_STATUS
(*PFN_CERT_GET_KEY_IDENTIFIER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     ppBuffer,
        PULONG                      pLength
    );

typedef ANSC_STATUS
(*PFN_CERT_GET_KEY_USAGE)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pKeyUsage
    );

typedef ANSC_STATUS
(*PFN_CERT_ADD_EXTENSION)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDValueString,
        BOOLEAN                     bCritical,
        ANSC_HANDLE                 hExtensionValue
    );    

typedef ANSC_STATUS
(*PFN_CERT_ADD_SUBJECTKEY_IDEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_STATUS
(*PFN_CERT_ADD_AUTHKEY_IDEN)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pKeyIden,
        ULONG                       length
    );

typedef ANSC_STATUS
(*PFN_CERT_SET_KEY_USAGE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulKeyUsage
    );

typedef ANSC_STATUS
(*PFN_CERT_SET_BASIC_CON)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bIsCA,
        LONG                        pathLen
    );

typedef ANSC_STATUS
(*PFN_CERT_SET_SUBJECT_ALTNAME)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pAltName
    );

typedef ANSC_STATUS
(*PFN_CERT_SET_SUBJECT_KB5NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRealnName,
        ANSC_HANDLE                 hPrinName
        
    );

typedef BOOLEAN
(*PFN_CERT_KEY_CHECKING)
    (
        ANSC_HANDLE                 hThisObject,
        PKI_KEY_TYPE                keyType,
        ANSC_HANDLE                 hKeyHandle
    );

typedef BOOLEAN
(*PFN_CERT_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pString,
        PULONG                      pLength
    );


typedef BOOLEAN
(*PFN_CERT_GETVALUE_BYOID)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString,
        PCHAR                       pString,
        PULONG                      pLength
    );


#define                             EXTENSION_CDP_HTTP               0
#define                             EXTENSION_CDP_LDAP               1

typedef ANSC_STATUS
(*PFN_CERT_GET_CDP)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       cdpType,
        PCHAR                       pBuffer,
        PULONG                      pLength
    );

typedef BOOLEAN
(*PFN_CERT_CHECK)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

/**********************************************************************
 *
 * This function is to get the encoding of DN name of the certificate,
 * the subjectAltName will be used if the DN name is empty.
 *
 **********************************************************************/
typedef ANSC_STATUS
(*PFN_CERT_GETNAMEENCODING)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pNameEncoding,
        PULONG                      pulNameEncodingSize
    );

typedef BOOLEAN
(*PFN_CERT_ISRDNEMPTY)
    (
        ANSC_HANDLE                 hThisObject
    );

#define  ANSC_ASN1_CERTIFICATE_CLASS_CONTENT                          \
    /* duplication of the base object class content */                \
    ANSC_ASN1_SEQUENCE_CLASS_CONTENT                                  \
    /* start of object class content */                               \
    PUCHAR                          pSignedData;                      \
    ULONG                           uSignedLength;                    \
    PFN_ASN1_GET_CHILD              GetTbsCertificate;                \
    PFN_ASN1_GET_CHILD              GetSignatureAlgorithm;            \
    PFN_ASN1_GET_CHILD              GetSignatureValue;                \
    PFN_CERT_CHECK_VALIDITY         CheckValidity;                    \
    PFN_CERT_IS_TIME_VALID          IsTimeValid;                      \
    PFN_CERT_IS_VALID_NOW           IsValidNow;                       \
    PFN_CERT_CHECK_TIME             CheckTime;                        \
    PFN_CERT_GET_TIME               GetExpirationTime;                \
    PFN_CERT_GET_TIME               GetNotBeforeTime;                 \
    PFN_CERT_IS_SELF_SIGNED         IsSelfSigned;                     \
    PFN_CERT_GET_KEY_TYPE           GetKeyType;                       \
    PFN_CERT_GETKEY_BITS            GetKeyBits;                       \
    PFN_CERT_GET_SIGNATURE_TYPE     GetSignatureType;                 \
    PFN_CERT_SET_SIGNATURE_ALG      SetSignatureType;                 \
    PFN_CERT_SET_PUBLIC_KEYINFO     SetPublicKeyInfo;                 \
    PFN_CERT_VERIFY                 VerifySignature;                  \
    PFN_CERT_VERIFY_CHILD           VerifyChildCert;                  \
    PFN_ASN1_GET_CHILD              GetSerialNumber;                  \
    PFN_ASN1_GET_CHILD              GetPublicKeyInfo;                 \
    PFN_ASN1_GET_CHILD              GetIssuerHandle;                  \
    PFN_ASN1_GET_CHILD              GetSubjectHandle;                 \
    PFN_ASN1_GET_CHILD              GetExtensionsHandle;              \
    PFN_CERT_SIGN                   SignWithPrivateKeyInfo;           \
    PFN_CERT_SIGN                   SignWithCryptAPI;                 \
    PFN_CERT_SIGN_PARAM             SignWithKeyParam;                 \
                                                                      \
    PFN_CERT_GETNAMEENCODING        GetNameEncoding;                  \
    PFN_CERT_ISRDNEMPTY             IsRDNNameEmpty;                   \
    PFN_CERT_IS_ISSUER              IsThisTheIssuer;                  \
    /* get extension info apis */                                     \
    PFN_CERT_FIND_EXTENSION_1       FindExtensionWithOID;             \
    PFN_CERT_FIND_EXTENSION_2       FindExtensionWithType;            \
    PFN_CERT_GET_BASIC_CONSTAINT    GetBasicConstraint;               \
    PFN_CERT_GET_KEY_IDENTIFIER     GetSubjectKeyIdentifier;          \
    PFN_CERT_GET_KEY_IDENTIFIER     GetAuthorityIdentifier;           \
    PFN_CERT_FIND_EXTENSION_EX      GetSubjectAltName;                \
    PFN_CERT_GET_KEY_USAGE          GetKeyUsage;                      \
    PFN_CERT_PATH_VALID             IsPathLengthValid;                \
    PFN_CERT_GET_CDP                GetCDPString;                     \
    PFN_CERT_CHECK                  IsExtKeyusageIncluded;            \
    /* add extension apis */                                          \
    PFN_CERT_ADD_EXTENSION          AddExtension;                     \
    PFN_CERT_ADD_SUBJECTKEY_IDEN    AddSubjectKeyIdentifier;          \
    PFN_CERT_SET_KEY_USAGE          AddKeyUsage;                      \
    PFN_CERT_ADD_AUTHKEY_IDEN       AddAuthorityIdentifier;           \
    PFN_CERT_SET_BASIC_CON          AddBasicConstraint;               \
    PFN_CERT_SET_SUBJECT_ALTNAME    AddSubjectAltName;                \
    PFN_CERT_SET_SUBJECT_KB5NAME    AddKerberosSubAltName;            \
    /* other name and key related apis */                             \
    PFN_CERT_KEY_CHECKING           IsKeyMatching;                    \
    PFN_CERT_EXPORT_KEY             ExportPublicKey;                  \
    PFN_CERT_GET_NAME               GetCommonName;                    \
    PFN_CERT_GETVALUE_BYOID         GetSubjectNameByOID;              \
    /* end of object class content */                                 \

typedef  struct
_ANSC_ASN1_CERTIFICATE
{
    ANSC_ASN1_CERTIFICATE_CLASS_CONTENT
}
ANSC_ASN1_CERTIFICATE,  *PANSC_ASN1_CERTIFICATE;

#define  ACCESS_ANSC_ASN1_CERTIFICATE(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_CERTIFICATE, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_TBSCERTIFICATE

 TBSCertificate ::= Sequence 
     {
                           version [CON 0] Integer DEF
                      serialNumber CertificateSerialNumber 
                         signature AlgorithmIdentifier 
                            issuer Name 
                          validity Validity 
                           subject Name 
              subjectPublicKeyInfo SubjectPublicKeyInfo 
                    issuerUniqueID [CON 1] IMP UniqueIdentifier OPT
                   subjectUniqueID [CON 2] IMP UniqueIdentifier OPT
                        extensions [CON 3] Extensions OPT
     }

 **********************************************************************/
#define  ANSC_ASN1_TBSCERTIFICATE_CLASS_CONTENT         ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_TBSCERTIFICATE
{
    ANSC_ASN1_TBSCERTIFICATE_CLASS_CONTENT
}
ANSC_ASN1_TBSCERTIFICATE,  *PANSC_ASN1_TBSCERTIFICATE;

#define  ACCESS_ANSC_ASN1_TBSCERTIFICATE(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_TBSCERTIFICATE, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_CERTIFICATESERIALNUMBER

 CertificateSerialNumber ::= Integer 

 **********************************************************************/

typedef  ANSC_ASN1_INTEGER ANSC_ASN1_CERTIFICATESERIALNUMBER, *PANSC_ASN1_CERTIFICATESERIALNUMBER;

#define  ACCESS_ANSC_ASN1_CERTIFICATESERIALNUMBER(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_CERTIFICATESERIALNUMBER, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_VALIDITY

 Validity ::= Sequence 
     {
                         notBefore Time 
                          notAfter Time 
     }

 **********************************************************************/

#define  ANSC_ASN1_VALIDITY_CLASS_CONTENT               ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_VALIDITY
{
    ANSC_ASN1_VALIDITY_CLASS_CONTENT
}
ANSC_ASN1_VALIDITY,  *PANSC_ASN1_VALIDITY;

#define  ACCESS_ANSC_ASN1_VALIDITY(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_VALIDITY, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_TIME

 Time ::= Choice 
     {
                           utcTime UniversalTime 
                       generalTime GeneralizedTime 
     }

 **********************************************************************/

#define  TIME_MASK_UTCTIME                                         0x00
#define  TIME_MASK_GENERALTIME                                     0x01
#define  TIME_MAXI_MASK                                            0x01

#define  ANSC_ASN1_TIME_CLASS_CONTENT                                 \
    /* duplication of the base object class content */                \
    ANSC_ASN1_CHOICE_CLASS_CONTENT                                    \
    /* start of object class content */                               \
    PFN_ASN1_SET_UNIVERSAL_TIME     SetTime;                          \
    /* end of object class content */                                 \

typedef  struct
_ANSC_ASN1_TIME
{
    ANSC_ASN1_TIME_CLASS_CONTENT
}
ANSC_ASN1_TIME,  *PANSC_ASN1_TIME;

#define  ACCESS_ANSC_ASN1_TIME(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_TIME, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_UNIQUEIDENTIFIER

 UniqueIdentifier ::= BitString 

 **********************************************************************/

typedef  ANSC_ASN1_BITSTRING ANSC_ASN1_UNIQUEIDENTIFIER, *PANSC_ASN1_UNIQUEIDENTIFIER;

#define  ACCESS_ANSC_ASN1_UNIQUEIDENTIFIER(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_UNIQUEIDENTIFIER, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_SUBJECTPUBLICKEYINFO

 SubjectPublicKeyInfo ::= Sequence 
     {
                         algorithm AlgorithmIdentifier 
                  subjectPublicKey BitString 
         EXTRA:
                          publicKey PublicKey 
     }

 **********************************************************************/
typedef BOOLEAN
(*PFN_ASN1_SUBJECT_SETKEY)
    (
        ANSC_HANDLE                 hThisObject,
        PKI_KEY_TYPE                keyType,
        ANSC_HANDLE                 hKeyHandle,
        ANSC_HANDLE                 hParamHandle
    );

typedef BOOLEAN
(*PFN_ASN1_INIT_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        PKI_KEY_TYPE                keyType,
        ANSC_HANDLE                 hKeyGenHandle
    );

typedef ANSC_STATUS
(*PFN_ASN1_SUBJECT_VERIFY)
    (
        ANSC_HANDLE                 hThisHandle,
        PUCHAR                      pDataSigned,
        ULONG                       lengthOfData,
        SIGNATURE_TYPE              signType,
        PUCHAR                      pSignature,
        ULONG                       uLength
    );

typedef PKI_KEY_TYPE
(*PFN_ASN1_SUBJECT_GETKEYTYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_STATUS
(*PFN_ASN1_SUBJECT_EXPORT_KEY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hKeyGenParams
    );

typedef ULONG
(*PFN_ASN1_SUBJECT_GETKEYBITS)
    (
        ANSC_HANDLE                 hThisObject
    );

#define  ANSC_ASN1_SUBJECTPUBLICKEYINFO_CLASS_CONTENT                 \
    /* duplication of the base object class content */                \
    ANSC_ASN1_SEQUENCE_CLASS_CONTENT                                  \
    /* start of object class content */                               \
    PFN_ASN1_SUBJECT_GETKEYTYPE     GetPublicKeyType;                 \
    PFN_ASN1_SUBJECT_GETKEYBITS     GetPublicKeyBits;                 \
    PFN_ASN1_SUBJECT_SETKEY         InitPublicKey;                    \
    PFN_ASN1_INIT_KEY               GenerateKey;                      \
    PFN_ASN1_SUBJECT_VERIFY         Verify;                           \
    PFN_CERT_KEY_CHECKING           IsKeyMatching;                    \
    PFN_ASN1_SUBJECT_EXPORT_KEY     ExportPublicKey;                  \
    /* end of object class content */                                 \


typedef  struct
_ANSC_ASN1_SUBJECTPUBLICKEYINFO
{
    ANSC_ASN1_SUBJECTPUBLICKEYINFO_CLASS_CONTENT
}
ANSC_ASN1_SUBJECTPUBLICKEYINFO,  *PANSC_ASN1_SUBJECTPUBLICKEYINFO;

#define  ACCESS_ANSC_ASN1_SUBJECTPUBLICKEYINFO(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_SUBJECTPUBLICKEYINFO, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_DSASIGNATURE

 DSASignature ::= Sequence 
     {
                                 r Integer 
                                 s Integer 
     }

 **********************************************************************/

#define  ANSC_ASN1_DSASIGNATURE_CLASS_CONTENT           ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_DSASIGNATURE
{
    ANSC_ASN1_DSASIGNATURE_CLASS_CONTENT
}
ANSC_ASN1_DSASIGNATURE,  *PANSC_ASN1_DSASIGNATURE;

#define  ACCESS_ANSC_ASN1_DSASIGNATURE(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_DSASIGNATURE, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ALSTRING

 ALString ::=[UNI 15] IMP OctetString 

 **********************************************************************/

typedef  ANSC_ASN1_OCTETSTRING ANSC_ASN1_ALSTRING, *PANSC_ASN1_ALSTRING;

#define  ACCESS_ANSC_ASN1_ALSTRING(p)    ACCESS_CONTAINER(p, ANSC_ASN1_ALSTRING, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ATTRIBUTES

 Attributes ::= SetOf Attribute  {}

 **********************************************************************/

typedef  ANSC_ASN1_SETOF ANSC_ASN1_ATTRIBUTES, *PANSC_ASN1_ATTRIBUTES;

#define  ACCESS_ANSC_ASN1_ATTRIBUTES(p)   ACCESS_CONTAINER(p, ANSC_ASN1_ATTRIBUTES, Linkage)

#endif  /* _ANSC_ASN1_CERTIFICATE_INTERFACE_H */


