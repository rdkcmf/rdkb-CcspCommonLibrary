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

    MODULE: asn1_pfx_internal.h

        ASN.1 ANSC Code Generated by Cisco Systems, Inc.

    ---------------------------------------------------------------

    COPYRIGHT:

        Cisco Systems, Inc., 1999 ~ 2003

        All Rights Reserved.

    ---------------------------------------------------------------

    DESCRIPTION:

        The Internal functions defined for ASN.1 objects

        *   ASN1_PFX
        *   ASN1_MACDATA
        *   ASN1_AUTHENTICATEDSAFE
        *   ASN1_SAFECONTENTS
        *   ASN1_SAFEBAG
        *   ASN1_PKCS12ATTRIBUTES
        *   ASN1_PKCS12ATTRIBUTE
        *   ASN1_KEYBAG
        *   ASN1_PKCS8SHROUDEDKEYBAG
        *   ASN1_CERTBAG
        *   ASN1_CERTVALUE
        *   ASN1_CRLBAG
        *   ASN1_CRLVALUE
        *   ASN1_SECRETBAG
        *   ASN1_BAGVALUE


    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        ASNMAGIC ANSC CODE GENERATOR 1.0

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   12/13/2002  initial revision

 **********************************************************************/


#ifndef  _ASN1_PFX_INTERNAL_H
#define  _ASN1_PFX_INTERNAL_H

/**********************************************************************

 OBJECT -- ASN1_PFX

 PFX ::= Sequence 
     {
                           version Integer 
                          authSafe ContentInfo 
                           macData MacData OPT
     }

 **********************************************************************/

PANSC_ATTR_OBJECT
AnscAsn1PFXCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1PFXGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1PFXCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1PFXGetVersion
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1PFXGetAuthSafe
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1PFXGetMacData
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 *  manually added functions;
 */
ANSC_STATUS
AnscAsn1PFXHmacAuthenticate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPKCSUtility,
        PCHAR                       pPassword
    );

ANSC_STATUS
AnscAsn1PFXEncryptAndSign
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPKCSUtility,
        PCHAR                       pPassword
    );

ANSC_STATUS
AnscAsn1PFXDecrypt
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPKCSUtility,
        PCHAR                       pPassword
    );

ANSC_STATUS
AnscAsn1PFXAfterDecoding
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1PFXEnumerateInfo
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ENUM_CERT_KEY_PROC      proc,
        ANSC_HANDLE                 hContext
    );

ANSC_STATUS
AnscAsn1PFXExportInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPKIEntity
    );

ANSC_STATUS
AnscAsn1PFXAddCertificate
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pEncoding,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscAsn1PFXAddCertAndKeyInfo
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pCertEncoding,
        ULONG                       ulCertSize,
        PUCHAR                      pKeyEncoding,
        ULONG                       ulKeyInfoSize
    );

ANSC_HANDLE
AnscAsn1PFXLookforKeyInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hLocalKeyID
    );

/**********************************************************************

 OBJECT -- ASN1_MACDATA

 MacData ::= Sequence 
     {
                               mac DigestInfo 
                           macSalt OctetString 
                        iterations Integer DEF
     }

 **********************************************************************/

PANSC_ATTR_OBJECT
AnscAsn1MacDataCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1MacDataGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1MacDataCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

/**********************************************************************

 OBJECT -- ASN1_AUTHENTICATEDSAFE

 AuthenticatedSafe ::= SequenceOf ContentInfo  {}

 **********************************************************************/

ANSC_STATUS
AnscAsn1AuthenticatedSafeIsChildValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

ANSC_HANDLE
AnscAsn1AuthenticatedSafeCreateChild
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bAddItIn
    );

/*
 *  manually added functions;
 */
ANSC_STATUS
AnscAsn1AuthenticatedSafeDecryptContent
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPKCSUtility,
        PCHAR                       pPassword
    );

/**********************************************************************

 OBJECT -- ASN1_SAFECONTENTS

 SafeContents ::= SequenceOf SafeBag  {}

 **********************************************************************/

ANSC_STATUS
AnscAsn1SafeContentsIsChildValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

ANSC_HANDLE
AnscAsn1SafeContentsCreateChild
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bAddItIn
    );

/**********************************************************************

 OBJECT -- ASN1_SAFEBAG

 SafeBag ::= Sequence 
     {
                             bagID OID 
                          bagValue [CON 0] BagValue 
                     bagAttributes PKCS12Attributes OPT
     }

 **********************************************************************/

PANSC_ATTR_OBJECT
AnscAsn1SafeBagCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1SafeBagGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1SafeBagCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1SafeBagGetBagID
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SafeBagGetBagValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SafeBagGetBagAttributes
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Manually added functions
 */
BOOLEAN
AnscAsn1SafeBagIsCertBag
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SafeBagIsKeyInfoBag
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bEncrypted
    );

ANSC_HANDLE
AnscAsn1SafeBagGetLocalKeyID
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SafeBagGetValueHandle
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SafeBagSetLocalKeyID
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uKeyID
    );

ANSC_STATUS
AnscAsn1SafeBagSetCertInfo
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pEncoding,
        ULONG                       uLength
    );

ANSC_STATUS
AnscAsn1SafeBagSetKeyInfo
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pEncoding,
        ULONG                       uLength
    );

/**********************************************************************

 OBJECT -- ASN1_PKCS12ATTRIBUTES

 PKCS12Attributes ::= SetOf PKCS12Attribute  {}

 **********************************************************************/

ANSC_STATUS
AnscAsn1PKCS12AttributesIsChildValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

ANSC_HANDLE
AnscAsn1PKCS12AttributesCreateChild
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bAddItIn
    );

/**********************************************************************

 OBJECT -- ASN1_PKCS12ATTRIBUTE

 PKCS12Attribute ::= Attribute 

 **********************************************************************/

 /* No internal function is required for this object. */    

/**********************************************************************

 OBJECT -- ASN1_KEYBAG

 KeyBag ::= PrivateKeyInfo 

 **********************************************************************/

 /* No internal function is required for this object. */    

/**********************************************************************

 OBJECT -- ASN1_PKCS8SHROUDEDKEYBAG

 PKCS8ShroudedKeyBag ::= EncryptedPrivateKeyInfo 

 **********************************************************************/

 /* No internal function is required for this object. */    

/**********************************************************************

 OBJECT -- ASN1_CERTBAG

 CertBag ::= Sequence 
     {
                         certTypes OID 
                         certValue [CON 0] CertValue 
     }

 **********************************************************************/

PANSC_ATTR_OBJECT
AnscAsn1CertBagCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1CertBagGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1CertBagCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

/*
 * Manually added functions
 */
ANSC_HANDLE
AnscAsn1CertBagGetCertObject
    (
        ANSC_HANDLE                 hThisObject
    );

/**********************************************************************

 OBJECT -- ASN1_CERTVALUE

 CertValue ::= Choice 
     {
                        base64Cert IA5String 
                              cert OctetString 
     }

 **********************************************************************/

ANSC_HANDLE
AnscAsn1CertValueCreateSelection
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PANSC_ATTR_OBJECT
AnscAsn1CertValueCreateSelectionAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

PCHAR
AnscAsn1CertValueGetSelectionName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

LONG
AnscAsn1CertValueGetChoiceByOID
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

PCHAR
AnscAsn1CertValueGetOIDValueByMask
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

/**********************************************************************

 OBJECT -- ASN1_CRLBAG

 CRLBag ::= Sequence 
     {
                           crlType OID 
                          crlValue [CON 0] CRLValue 
     }

 **********************************************************************/

PANSC_ATTR_OBJECT
AnscAsn1CRLBagCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1CRLBagGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1CRLBagCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

/**********************************************************************

 OBJECT -- ASN1_CRLVALUE

 CRLValue ::= OctetString 

 **********************************************************************/

 /* No internal function is required for this object. */    

/**********************************************************************

 OBJECT -- ASN1_SECRETBAG

 SecretBag ::= Sequence 
     {
                        secretType OID 
                       secretValue OctetString 
     }

 **********************************************************************/

PANSC_ATTR_OBJECT
AnscAsn1SecretBagCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1SecretBagGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1SecretBagCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

/**********************************************************************

 OBJECT -- ASN1_BAGVALUE

 BagValue ::= Choice 
     {
                            keyBag KeyBag 
               pkcs8ShroudedKeyBag PKCS8ShroudedKeyBag 
                           certBag CertBag 
                            crlBag CRLBag 
                         secretBag SecretBag 
                       safeContent SafeContents 
     }

 **********************************************************************/

ANSC_HANDLE
AnscAsn1BagValueCreateSelection
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PANSC_ATTR_OBJECT
AnscAsn1BagValueCreateSelectionAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

PCHAR
AnscAsn1BagValueGetSelectionName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

LONG
AnscAsn1BagValueGetChoiceByOID
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

PCHAR
AnscAsn1BagValueGetOIDValueByMask
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );


#endif  /* _ASN1_PFX_INTERNAL_H */

