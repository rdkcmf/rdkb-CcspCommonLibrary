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

    MODULE: ansc_asn1_ContentInfo_interface.h

        ASN.1 ANSC Code Generated by Cisco Systems, Inc.

    ---------------------------------------------------------------

    COPYRIGHT:

        Cisco Systems, Inc., 1999 ~ 2003

        All Rights Reserved.

    ---------------------------------------------------------------

    DESCRIPTION:

        The ASN.1 object defined in this file

        *   ANSC_ASN1_CONTENTINFO
        *   ANSC_ASN1_DATA
        *   ANSC_ASN1_SIGNEDDATA
        *   ANSC_ASN1_DIGESTALGORITHMIDENTIFIERS
        *   ANSC_ASN1_CERTIFICATES
        *   ANSC_ASN1_CERTIFICATEREVOCATIONLISTS
        *   ANSC_ASN1_SIGNERINFOS
        *   ANSC_ASN1_SIGNERINFO
        *   ANSC_ASN1_ISSUERANDSERIALNUMBER
        *   ANSC_ASN1_DIGESTENCRYPTIONALGORITHMIDENTIFIER
        *   ANSC_ASN1_ENCRYPTEDDIGEST
        *   ANSC_ASN1_DIGESTINFO
        *   ANSC_ASN1_ENVELOPEDDATA
        *   ANSC_ASN1_RECIPIENTINFOS
        *   ANSC_ASN1_ENCRYPTEDCONTENTINFO
        *   ANSC_ASN1_RECIPIENTINFO
        *   ANSC_ASN1_ENCRYPTEDCONTENT
        *   ANSC_ASN1_PKCS7ENCRYPTEDKEY
        *   ANSC_ASN1_SIGNEDANDENVELOPEDDATA
        *   ANSC_ASN1_DIGESTEDDATA
        *   ANSC_ASN1_DIGEST
        *   ANSC_ASN1_ENCRYPTEDDATA
        *   ANSC_ASN1_CONTENTDATA


    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        ASNMAGIC ANSC CODE GENERATOR 1.0

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   05/07/2002  initial revision
        *   05/08/2002  two functions add added to ContentInfo object
        *   05/09/2002  more funcions are added to SignedData object
        *   08/08/2002  more apis for SignedData object

 **********************************************************************/


#ifndef  _ANSC_ASN1_CONTENTINFO_INTERFACE_H
#define  _ANSC_ASN1_CONTENTINFO_INTERFACE_H

/**********************************************************************

 OBJECT -- ANSC_ASN1_CONTENTINFO

 ContentInfo ::= Sequence 
     {
                       contentType OID 
                           content [CON 0] ContentData OPT
     }

 **********************************************************************/
typedef ANSC_HANDLE
(*PFN_PKCS7_GET_SIGNED_DATA)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef void
(*PFN_PKCS7_SET_TYPE_AND_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType,
        ANSC_HANDLE                 hData
    );

#define  ANSC_ASN1_CONTENTINFO_CLASS_CONTENT                          \
    /* duplication of the base object class content */                \
    ANSC_ASN1_SEQUENCE_CLASS_CONTENT                                  \
    /* start of object class content */                               \
    PFN_PKCS7_GET_SIGNED_DATA       GetSignedData;                    \
    PFN_PKCS7_SET_TYPE_AND_DATA     SetTypeAndData;                   \
    /* end of object class content */                                 \


typedef  struct
_ANSC_ASN1_CONTENTINFO
{
    ANSC_ASN1_CONTENTINFO_CLASS_CONTENT
}
ANSC_ASN1_CONTENTINFO,  *PANSC_ASN1_CONTENTINFO;

#define  ACCESS_ANSC_ASN1_CONTENTINFO(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_CONTENTINFO, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_DATA

 Data ::= OctetString 

 **********************************************************************/

typedef  ANSC_ASN1_OCTETSTRING ANSC_ASN1_DATA,    \
         *PANSC_ASN1_DATA;

#define  ACCESS_ANSC_ASN1_DATA(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_DATA, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_SIGNEDDATA

 SignedData ::= Sequence 
     {
                           version Integer 
                  digestAlgorithms DigestAlgorithmIdentifiers 
                       contentInfo ContentInfo 
                      certificates [CON 0] IMP Certificates OPT
                              crls [CON 1] IMP CertificateRevocationLists OPT
                       signerInfos SignerInfos 
     }

 **********************************************************************/

typedef ANSC_STATUS
(*PFN_SIGNEDDATA_ADD_DIGEST_ALG)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

/***************************************************************
 *
 * This one is important for generating SignedData.
 *
 * We suggest to call this api after you initiate all other fields.
 *
 * If Add certificate only, the pSignProc can be set as NULL;
 * If signature is required, the last 3 parameters should be inputted,
 * a new signerInfo will be created and added.
 *
 ***************************************************************/
typedef ANSC_STATUS
(*PFN_SIGNEDDATA_ADD_CERT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 pCertificate,
        CRYPT_SIGNDATA_PROC         pSignProc,    /* proc to sign the data */
        ANSC_HANDLE                 hParameter,    /* the parameter of above proc */
        HASH_TYPE                   hashType
    );

typedef ANSC_HANDLE
(*PFN_SIGNEDDATA_GET_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_HANDLE
(*PFN_SIGNEDDATA_GET_BYINDEX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef ANSC_STATUS
(*PFN_SIGNEDDATA_SET_HANDLE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSignHandle
    );

typedef ULONG
(*PFN_SIGNEDDATA_GET_TOTALCERT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_STATUS
(*PFN_SIGNEDDATA_GENERATE_MD)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

typedef ANSC_HANDLE
(*PFN_SIGNEDDATA_LOOKFOR_ISSUER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCert
    );

/*
 *  Verify the certificate chain.
 *
 *  It's optional for the verifier to provide the hCACert to
 *  verify the certificate chain.
 *
 *  If failed to find the issuer or verify the signature of certain
 *  cert in the chain, returns the error and put the cert index in
 *  the buffer of pUlQuestionIndex.
 */
typedef ANSC_STATUS
(*PFN_SIGNEDDATA_VERIFY_CERT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCACert,
        PULONG                      pUlQuestionIndex,
        BOOLEAN                     bCheckTime
    );

/*
 *  Verify the Digest in the signerInfos
 *
 *  If failed to verify the digest of certain
 *  cert in the chain, returns the error and put the cert index in
 *  the buffer of pUlQuestionIndex.
 */
typedef ANSC_STATUS
(*PFN_SIGNEDDATA_VERIFY_MD)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pUlQuestionIndex
    );

#define  ANSC_ASN1_SIGNEDDATA_CLASS_CONTENT                           \
    /* duplication of the base object class content */                \
    ANSC_ASN1_SEQUENCE_CLASS_CONTENT                                  \
    /* start of object class content */                               \
    PFN_SIGNEDDATA_ADD_DIGEST_ALG   AddDigestAlgor;                   \
    PFN_SIGNEDDATA_GET_BYINDEX      GetCertificateByIndex;            \
    PFN_SIGNEDDATA_GET_TOTALCERT    GetTotalCerts;                    \
    PFN_SIGNEDDATA_GET_HANDLE       GetSignedHandle;                  \
    PFN_SIGNEDDATA_SET_HANDLE       SetSignedHandle;                  \
    PFN_SIGNEDDATA_ADD_CERT         AddCertificate;                   \
    PFN_SIGNEDDATA_VERIFY_MD        VerifyMessageDigest;              \
    PFN_SIGNEDDATA_VERIFY_CERT      VerifyCertChain;                  \
    PFN_ASN1_GET_CHILD              GetFirstCertOfTheChain;           \
    PFN_ASN1_GET_CHILD              GetLastCertOfTheChain;            \
    PFN_SIGNEDDATA_LOOKFOR_ISSUER   LookingForIssuer;                 \
    /* end of object class content */                                 \


typedef  struct
_ANSC_ASN1_SIGNEDDATA
{
    ANSC_ASN1_SIGNEDDATA_CLASS_CONTENT
}
ANSC_ASN1_SIGNEDDATA,  *PANSC_ASN1_SIGNEDDATA;

#define  ACCESS_ANSC_ASN1_SIGNEDDATA(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_SIGNEDDATA, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_DIGESTALGORITHMIDENTIFIERS

 DigestAlgorithmIdentifiers ::= SetOf DigestAlgorithmIdentifier  {}

 **********************************************************************/

typedef  ANSC_ASN1_SETOF ANSC_ASN1_DIGESTALGORITHMIDENTIFIERS,    \
         *PANSC_ASN1_DIGESTALGORITHMIDENTIFIERS;

#define  ACCESS_ANSC_ASN1_DIGESTALGORITHMIDENTIFIERS(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_DIGESTALGORITHMIDENTIFIERS, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_CERTIFICATES

 Certificates ::= SetOf Certificate  {}

 **********************************************************************/

typedef  ANSC_ASN1_SETOF ANSC_ASN1_CERTIFICATES,    \
         *PANSC_ASN1_CERTIFICATES;

#define  ACCESS_ANSC_ASN1_CERTIFICATES(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_CERTIFICATES, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_CERTIFICATEREVOCATIONLISTS

 CertificateRevocationLists ::= SetOf CRL  {}

 **********************************************************************/

typedef  ANSC_ASN1_SETOF ANSC_ASN1_CERTIFICATEREVOCATIONLISTS,    \
         *PANSC_ASN1_CERTIFICATEREVOCATIONLISTS;

#define  ACCESS_ANSC_ASN1_CERTIFICATEREVOCATIONLISTS(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_CERTIFICATEREVOCATIONLISTS, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_SIGNERINFOS

 SignerInfos ::= SetOf SignerInfo  {}

 **********************************************************************/

typedef  ANSC_ASN1_SETOF ANSC_ASN1_SIGNERINFOS,    \
         *PANSC_ASN1_SIGNERINFOS;

#define  ACCESS_ANSC_ASN1_SIGNERINFOS(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_SIGNERINFOS, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_SIGNERINFO

 SignerInfo ::= Sequence 
     {
                           version Integer 
             issuerAndSerialNumber IssuerAndSerialNumber 
                   digestAlgorithm DigestAlgorithmIdentifier 
           authenticatedAttributes [CON 0] IMP Attributes OPT
         digestEncryptionAlgorithm DigestEncryptionAlgorithmIdentifier 
                   encryptedDigest EncryptedDigest 
         unauthenticatedAttributes [CON 1] IMP Attributes OPT
     }

 **********************************************************************/

#define  ANSC_ASN1_SIGNERINFO_CLASS_CONTENT             ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_SIGNERINFO
{
    ANSC_ASN1_SIGNERINFO_CLASS_CONTENT
}
ANSC_ASN1_SIGNERINFO,  *PANSC_ASN1_SIGNERINFO;

#define  ACCESS_ANSC_ASN1_SIGNERINFO(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_SIGNERINFO, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ISSUERANDSERIALNUMBER

 IssuerAndSerialNumber ::= Sequence 
     {
                            issuer Name 
                      serialNumber Integer 
     }

 **********************************************************************/

#define  ANSC_ASN1_ISSUERANDSERIALNUMBER_CLASS_CONTENT  ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_ISSUERANDSERIALNUMBER
{
    ANSC_ASN1_ISSUERANDSERIALNUMBER_CLASS_CONTENT
}
ANSC_ASN1_ISSUERANDSERIALNUMBER,  *PANSC_ASN1_ISSUERANDSERIALNUMBER;

#define  ACCESS_ANSC_ASN1_ISSUERANDSERIALNUMBER(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_ISSUERANDSERIALNUMBER, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_DIGESTENCRYPTIONALGORITHMIDENTIFIER

 DigestEncryptionAlgorithmIdentifier ::= AlgorithmIdentifier 

 **********************************************************************/

typedef  ANSC_ASN1_ALGORITHMIDENTIFIER ANSC_ASN1_DIGESTENCRYPTIONALGORITHMIDENTIFIER,    \
         *PANSC_ASN1_DIGESTENCRYPTIONALGORITHMIDENTIFIER;

#define  ACCESS_ANSC_ASN1_DIGESTENCRYPTIONALGORITHMIDENTIFIER(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_DIGESTENCRYPTIONALGORITHMIDENTIFIER, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ENCRYPTEDDIGEST

 EncryptedDigest ::= OctetString 

 **********************************************************************/

typedef  ANSC_ASN1_OCTETSTRING ANSC_ASN1_ENCRYPTEDDIGEST,    \
         *PANSC_ASN1_ENCRYPTEDDIGEST;

#define  ACCESS_ANSC_ASN1_ENCRYPTEDDIGEST(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_ENCRYPTEDDIGEST, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_DIGESTINFO

 DigestInfo ::= Sequence 
     {
                         digestAlg DigestAlgorithmIdentifier 
                            digest OctetString 
     }

 **********************************************************************/

#define  ANSC_ASN1_DIGESTINFO_CLASS_CONTENT             ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_DIGESTINFO
{
    ANSC_ASN1_DIGESTINFO_CLASS_CONTENT
}
ANSC_ASN1_DIGESTINFO,  *PANSC_ASN1_DIGESTINFO;

#define  ACCESS_ANSC_ASN1_DIGESTINFO(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_DIGESTINFO, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ENVELOPEDDATA

 EnvelopedData ::= Sequence 
     {
                           version Integer 
                    recipientInfos RecipientInfos 
              encryptedContentInfo EncryptedContentInfo 
     }

 **********************************************************************/

#define  ANSC_ASN1_ENVELOPEDDATA_CLASS_CONTENT          ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_ENVELOPEDDATA
{
    ANSC_ASN1_ENVELOPEDDATA_CLASS_CONTENT
}
ANSC_ASN1_ENVELOPEDDATA,  *PANSC_ASN1_ENVELOPEDDATA;

#define  ACCESS_ANSC_ASN1_ENVELOPEDDATA(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_ENVELOPEDDATA, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_RECIPIENTINFOS

 RecipientInfos ::= SetOf RecipientInfo  {}

 **********************************************************************/

typedef  ANSC_ASN1_SETOF ANSC_ASN1_RECIPIENTINFOS,    \
         *PANSC_ASN1_RECIPIENTINFOS;

#define  ACCESS_ANSC_ASN1_RECIPIENTINFOS(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_RECIPIENTINFOS, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ENCRYPTEDCONTENTINFO

 EncryptedContentInfo ::= Sequence 
     {
                       contentType OID 
        contentEncryptionAlgorithm EncryptionAlgorithmIdentifier 
                           content [CON 0] IMP EncryptedContent OPT
     }

 **********************************************************************/

#define  ANSC_ASN1_ENCRYPTEDCONTENTINFO_CLASS_CONTENT   ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_ENCRYPTEDCONTENTINFO
{
    ANSC_ASN1_ENCRYPTEDCONTENTINFO_CLASS_CONTENT
}
ANSC_ASN1_ENCRYPTEDCONTENTINFO,  *PANSC_ASN1_ENCRYPTEDCONTENTINFO;

#define  ACCESS_ANSC_ASN1_ENCRYPTEDCONTENTINFO(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_ENCRYPTEDCONTENTINFO, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_RECIPIENTINFO

 RecipientInfo ::= Sequence 
     {
                           version Integer 
              issuerAndSerialNumer IssuerAndSerialNumber 
            keyEncryptionAlgorithm EncryptionAlgorithmIdentifier 
                      encryptedKey PKCS7EncryptedKey 
     }

 **********************************************************************/

#define  ANSC_ASN1_RECIPIENTINFO_CLASS_CONTENT          ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_RECIPIENTINFO
{
    ANSC_ASN1_RECIPIENTINFO_CLASS_CONTENT
}
ANSC_ASN1_RECIPIENTINFO,  *PANSC_ASN1_RECIPIENTINFO;

#define  ACCESS_ANSC_ASN1_RECIPIENTINFO(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_RECIPIENTINFO, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ENCRYPTEDCONTENT

 EncryptedContent ::= OctetString 

 **********************************************************************/

typedef  ANSC_ASN1_OCTETSTRING ANSC_ASN1_ENCRYPTEDCONTENT,    \
         *PANSC_ASN1_ENCRYPTEDCONTENT;

#define  ACCESS_ANSC_ASN1_ENCRYPTEDCONTENT(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_ENCRYPTEDCONTENT, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_PKCS7ENCRYPTEDKEY

 PKCS7EncryptedKey ::= OctetString 

 **********************************************************************/

typedef  ANSC_ASN1_OCTETSTRING ANSC_ASN1_PKCS7ENCRYPTEDKEY,    \
         *PANSC_ASN1_PKCS7ENCRYPTEDKEY;

#define  ACCESS_ANSC_ASN1_PKCS7ENCRYPTEDKEY(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_PKCS7ENCRYPTEDKEY, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_SIGNEDANDENVELOPEDDATA

 SignedAndEnvelopedData ::= Sequence 
     {
                           version Integer 
                    recipientInfos RecipientInfos 
                  digestAlgorithms DigestAlgorithmIdentifiers 
              encryptedContentInfo EncryptedContentInfo 
                      certificates [CON 0] IMP Certificates OPT
                              crls [CON 1] IMP CertificateRevocationLists OPT
                       signerInfos SignerInfos 
     }

 **********************************************************************/

#define  ANSC_ASN1_SIGNEDANDENVELOPEDDATA_CLASS_CONTENT ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_SIGNEDANDENVELOPEDDATA
{
    ANSC_ASN1_SIGNEDANDENVELOPEDDATA_CLASS_CONTENT
}
ANSC_ASN1_SIGNEDANDENVELOPEDDATA,  *PANSC_ASN1_SIGNEDANDENVELOPEDDATA;

#define  ACCESS_ANSC_ASN1_SIGNEDANDENVELOPEDDATA(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_SIGNEDANDENVELOPEDDATA, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_DIGESTEDDATA

 DigestedData ::= Sequence 
     {
                           version Integer 
                   digestAlgorithm DigestAlgorithmIdentifier 
                       contentInfo ContentInfo 
                            digest Digest 
     }

 **********************************************************************/

#define  ANSC_ASN1_DIGESTEDDATA_CLASS_CONTENT           ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_DIGESTEDDATA
{
    ANSC_ASN1_DIGESTEDDATA_CLASS_CONTENT
}
ANSC_ASN1_DIGESTEDDATA,  *PANSC_ASN1_DIGESTEDDATA;

#define  ACCESS_ANSC_ASN1_DIGESTEDDATA(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_DIGESTEDDATA, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_DIGEST

 Digest ::= OctetString 

 **********************************************************************/

typedef  ANSC_ASN1_OCTETSTRING ANSC_ASN1_DIGEST,    \
         *PANSC_ASN1_DIGEST;

#define  ACCESS_ANSC_ASN1_DIGEST(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_DIGEST, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_ENCRYPTEDDATA

 EncryptedData ::= Sequence 
     {
                           version Integer 
              encryptedContentInfo EncryptedContentInfo 
     }

 **********************************************************************/

#define  ANSC_ASN1_ENCRYPTEDDATA_CLASS_CONTENT          ANSC_ASN1_SEQUENCE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_ENCRYPTEDDATA
{
    ANSC_ASN1_ENCRYPTEDDATA_CLASS_CONTENT
}
ANSC_ASN1_ENCRYPTEDDATA,  *PANSC_ASN1_ENCRYPTEDDATA;

#define  ACCESS_ANSC_ASN1_ENCRYPTEDDATA(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_ENCRYPTEDDATA, Linkage)

/**********************************************************************

 OBJECT -- ANSC_ASN1_CONTENTDATA

 ContentData ::= Choice 
     {
                              data Data 
                        signedData SignedData 
                      envelopeData EnvelopedData 
            signedAndEnvelopedData SignedAndEnvelopedData 
                        digestData DigestedData 
                     encryptedData EncryptedData 
                          authPack Kb5_AuthPack 
                         dhKeyInfo Kb5_KdcDHKeyInfo 
                      replyKeyPack Kb5_ReplyKeyPack 
      }

 **********************************************************************/

#define  CONTENTDATA_MASK_DATA                                     0x00
#define  CONTENTDATA_MASK_SIGNEDDATA                               0x01
#define  CONTENTDATA_MASK_ENVELOPEDATA                             0x02
#define  CONTENTDATA_MASK_SIGNEDANDENVELOPEDDATA                   0x03
#define  CONTENTDATA_MASK_DIGESTDATA                               0x04
#define  CONTENTDATA_MASK_ENCRYPTEDDATA                            0x05
#define  CONTENTDATA_MASK_AUTHPACK                                 0x06
#define  CONTENTDATA_MASK_DHKEYINFO                                0x07
#define  CONTENTDATA_MASK_REPLYKEYPACK                             0x08
#define  CONTENTDATA_MAXI_MASK                                     0x08

#define  ANSC_ASN1_CONTENTDATA_CLASS_CONTENT            ANSC_ASN1_CHOICE_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_CONTENTDATA
{
    ANSC_ASN1_CONTENTDATA_CLASS_CONTENT
}
ANSC_ASN1_CONTENTDATA,  *PANSC_ASN1_CONTENTDATA;

#define  ACCESS_ANSC_ASN1_CONTENTDATA(p)    \
         ACCESS_CONTAINER(p, ANSC_ASN1_CONTENTDATA, Linkage)

#endif  /*_ANSC_ASN1_CONTENTINFO_INTERFACE_H*/


