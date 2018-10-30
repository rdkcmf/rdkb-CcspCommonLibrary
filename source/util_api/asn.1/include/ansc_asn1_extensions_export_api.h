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

    MODULE: ansc_asn1_Extensions_export_api.h

        ASN.1 ANSC Code Generated by Cisco Systems, Inc.

    ---------------------------------------------------------------

    COPYRIGHT:

        Cisco Systems, Inc., 1999 ~ 2003

        All Rights Reserved.

    ---------------------------------------------------------------

    DESCRIPTION:

        The Internal functions defined for ASN.1 objects

        *   ANSC_ASN1_EXTENSIONS
        *   ANSC_ASN1_EXTENSION
        *   ANSC_ASN1_AUTHORITYKEYIDENTIFIER
        *   ANSC_ASN1_KEYIDENTIFIER
        *   ANSC_ASN1_GENERALNAMES
        *   ANSC_ASN1_GENERALNAME
        *   ANSC_ASN1_ORADDRESS
        *   ANSC_ASN1_EDIPARTYNAME
        *   ANSC_ASN1_KEYUSAGE
        *   ANSC_ASN1_EXTKEYUSAGESYNTAX
        *   ANSC_ASN1_BASICCONSTRAINTS
        *   ANSC_ASN1_PRIVATEKEYUSAGEPERIOD
        *   ANSC_ASN1_CERTIFICATEPOLICIES
        *   ANSC_ASN1_POLICYINFORMATION
        *   ANSC_ASN1_POLICYQUALIFIERS
        *   ANSC_ASN1_POLICYQUALIFIERINFO
        *   ANSC_ASN1_POLICYQUALIFIER
        *   ANSC_ASN1_USERNOTICE
        *   ANSC_ASN1_NOTICEREFERENCE
        *   ANSC_ASN1_DISPLAYTEXT
        *   ANSC_ASN1_POLICYMAPPINGS
        *   ANSC_ASN1_NAMECONSTRAINTS
        *   ANSC_ASN1_GENERALSUBTREES
        *   ANSC_ASN1_GENERALSUBTREE
        *   ANSC_ASN1_POLICYCONSTRAINTS
        *   ANSC_ASN1_SUBJECTDIRECTORYATTRIBUTES
        *   ANSC_ASN1_CRLNUMBER
        *   ANSC_ASN1_CRLDISTPOINTSSYNTAX
        *   ANSC_ASN1_DISTRIBUTIONPOINT
        *   ANSC_ASN1_DISTRIBUTIONPOINTNAME
        *   ANSC_ASN1_REASONFLAGS
        *   ANSC_ASN1_ISSUINGDISTRIBUTIONPOINT
        *   ANSC_ASN1_AUTHORITYINFOACCESSSYNTAX
        *   ANSC_ASN1_ACCESSDESCRIPTION
        *   ANSC_ASN1_CRLREASON
        *   ANSC_ASN1_HOLDINSTRUCTIONCODE
        *   ANSC_ASN1_INVALIDITYDATE
        *   ANSC_ASN1_POLICYMAPPING
        *   ANSC_ASN1_NOTICENUMBERS
        *   ANSC_ASN1_DELTACRLINDICATOR
        *   ANSC_ASN1_ATTRIBUTESSYNTAX
        *   ANSC_ASN1_BIOMETRICSSYNTAX
        *   ANSC_ASN1_BIOMETRICDATA
        *   ANSC_ASN1_TYPEOFBIOMETRICDATA
        *   ANSC_ASN1_QCSTATEMENTSSYNTAX
        *   ANSC_ASN1_QCSTATEMENT
        *   ANSC_ASN1_ENTRUSTVERSIONINFO
        *   ANSC_ASN1_EXTENSIONVALUE


    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        ASNMAGIC ANSC CODE GENERATOR 1.0

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   05/07/2002  initial revision

 **********************************************************************/


#ifndef  _ANSC_ASN1_EXTENSIONS_EXPORTED_API_H
#define  _ANSC_ASN1_EXTENSIONS_EXPORTED_API_H

/**********************************************************************

 OBJECT -- ANSC_ASN1_EXTENSIONS

 Extensions ::= SequenceOf Extension  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateExtensions
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_EXTENSION

 Extension ::= Sequence 
     {
                            extnID OID 
                          critical BOOL 
                         extnValue OctetString 
         EXTRA:
                     extensionValue ExtensionValue 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateExtension
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1ExtensionGenerate
    (
        ANSC_HANDLE                 hReserved,
        PCHAR                       pOIDSting,
        BOOLEAN                     bIsCrital,
        ANSC_HANDLE                 hExtValue
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_AUTHORITYKEYIDENTIFIER

 AuthorityKeyIdentifier ::= Sequence 
     {
                     keyIdentifier [CON 0] IMP KeyIdentifier OPT
               authorityCertIssuer [CON 1] IMP GeneralNames OPT
                      serialNumber [CON 2] IMP Integer OPT
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateAuthorityKeyIdentifier
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_KEYIDENTIFIER

 KeyIdentifier ::= OctetString 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateKeyIdentifier
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_GENERALNAMES

 GeneralNames ::= SequenceOf GeneralName  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateGeneralNames
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1CreateGeneralNamesWithAltName
    (
        ANSC_HANDLE                 hReserved,
        PCHAR                       pAltName
    );

ANSC_HANDLE
AnscAsn1CreateGeneralNamesWithKerberosName
    (
        ANSC_HANDLE                 hReserved,
        ANSC_HANDLE                 hRealmName,
        ANSC_HANDLE                 hPrinName
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_GENERALNAME

 GeneralName ::= Choice 
     {
                         otherName [CON 0] IMP OtherName 
                        rfc822Name [CON 1] IMP IA5String 
                           dNSName [CON 2] IMP IA5String 
                       x400Address [CON 3] IMP ORAddress 
                     directoryName [CON 4] IMP Name 
                      ediPartyName [CON 5] IMP EDIPartyName 
                               uri [CON 6] IMP IA5String 
                         iPAddress [CON 7] IMP OctetString 
                      registeredID [CON 8] IMP OID 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateGeneralName
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_ORADDRESS

 ORAddress ::= OctetString 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateORAddress
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_EDIPARTYNAME

 EDIPartyName ::= Sequence 
     {
                      nameAssigner [CON 0] IMP DirectoryString OPT
                         partyName [CON 1] IMP DirectoryString 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateEDIPartyName
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_KEYUSAGE

 KeyUsage ::= BitString 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateKeyUsage
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_EXTKEYUSAGESYNTAX

 ExtKeyUsageSyntax ::= SequenceOf OID  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateExtKeyUsageSyntax
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_BASICCONSTRAINTS

 BasicConstraints ::= Sequence 
     {
                                cA BOOL 
                 pathLenConstraint Integer 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateBasicConstraints
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_PRIVATEKEYUSAGEPERIOD

 PrivateKeyUsagePeriod ::= Sequence 
     {
                         notBefore [CON 0] IMP GeneralizedTime OPT
                          notAfter [CON 1] IMP GeneralizedTime OPT
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePrivateKeyUsagePeriod
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_CERTIFICATEPOLICIES

 CertificatePolicies ::= SequenceOf PolicyInformation  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateCertificatePolicies
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_POLICYINFORMATION

 PolicyInformation ::= Sequence 
     {
                  policyIdentifier OID 
                  policyQualifiers PolicyQualifiers 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePolicyInformation
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_POLICYQUALIFIERS

 PolicyQualifiers ::= SequenceOf PolicyQualifierInfo  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePolicyQualifiers
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_POLICYQUALIFIERINFO

 PolicyQualifierInfo ::= Sequence 
     {
                 policyQualifierID OID 
                         qualifier PolicyQualifier 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePolicyQualifierInfo
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_POLICYQUALIFIER

 PolicyQualifier ::= Choice 
     {
                        userNotice UserNotice 
                            cpsuri IA5String 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePolicyQualifier
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_USERNOTICE

 UserNotice ::= Sequence 
     {
                         noticeRef NoticeReference 
                      explicitText DisplayText 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateUserNotice
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_NOTICEREFERENCE

 NoticeReference ::= Sequence 
     {
                      organization DisplayText 
                     noticeNumbers NoticeNumbers 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateNoticeReference
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_DISPLAYTEXT

 DisplayText ::= Choice 
     {
                     visibleString VisibleString 
                         bmpString BMPString 
                        utf8String UTF8String 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateDisplayText
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_POLICYMAPPINGS

 PolicyMappings ::= SequenceOf PolicyMapping  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePolicyMappings
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_NAMECONSTRAINTS

 NameConstraints ::= Sequence 
     {
                 permittedSubtrees [CON 0] IMP GeneralSubtrees OPT
                  excludedSubtrees [CON 1] IMP GeneralSubtrees OPT
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateNameConstraints
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_GENERALSUBTREES

 GeneralSubtrees ::= SequenceOf GeneralSubTree  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateGeneralSubtrees
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_GENERALSUBTREE

 GeneralSubTree ::= Sequence 
     {
                              base GeneralName 
                           minimum [CON 0] IMP Integer DEF
                           maximum [CON 1] IMP Integer OPT
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateGeneralSubTree
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_POLICYCONSTRAINTS

 PolicyConstraints ::= Sequence 
     {
             requireExplicitPolicy [CON 0] IMP Integer OPT
              inhibitPolicyMapping [CON 1] IMP Integer OPT
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePolicyConstraints
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_SUBJECTDIRECTORYATTRIBUTES

 SubjectDirectoryAttributes ::= SequenceOf Attribute  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateSubjectDirectoryAttributes
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_CRLNUMBER

 CRLNumber ::= Integer 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateCRLNumber
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_CRLDISTPOINTSSYNTAX

 CRLDistPointsSyntax ::= SequenceOf DistributionPoint  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateCRLDistPointsSyntax
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_DISTRIBUTIONPOINT

 DistributionPoint ::= Sequence 
     {
                 distributionPoint [CON 0] IMP DistributionPointName OPT
                           reasons [CON 1] IMP ReasonFlags OPT
                         cRLIssuer [CON 2] IMP GeneralNames OPT
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateDistributionPoint
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_DISTRIBUTIONPOINTNAME

 DistributionPointName ::= Choice 
     {
                          fullName [CON 0] IMP GeneralNames 
           nameRelativeToCRLIssuer [CON 1] IMP RelativeDistinguishedName 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateDistributionPointName
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_REASONFLAGS

 ReasonFlags ::= BitString 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateReasonFlags
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_ISSUINGDISTRIBUTIONPOINT

 IssuingDistributionPoint ::= Sequence 
     {
                 distributionPoint [CON 0] IMP DistributionPointName OPT
           onlyConstrainsUserCerts [CON 1] IMP BOOL DEF
             onlyConstrainsCACerts [CON 2] IMP BOOL DEF
                   onlySomeReasons [CON 3] IMP ReasonFlags OPT
                       indirectCRL [CON 4] IMP BOOL DEF
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateIssuingDistributionPoint
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_AUTHORITYINFOACCESSSYNTAX

 AuthorityInfoAccessSyntax ::= SequenceOf AccessDescription  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateAuthorityInfoAccessSyntax
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_ACCESSDESCRIPTION

 AccessDescription ::= Sequence 
     {
                      accessMethod OID 
                    accessLocation GeneralName 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateAccessDescription
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_CRLREASON

 CRLReason ::= Enumerate 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateCRLReason
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_HOLDINSTRUCTIONCODE

 HoldInstructionCode ::= OID 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateHoldInstructionCode
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_INVALIDITYDATE

 InvalidityDate ::= GeneralizedTime 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateInvalidityDate
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_POLICYMAPPING

 PolicyMapping ::= Sequence 
     {
              issuerDomainPolicyID OID 
             subjectDomainPolicyID OID 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreatePolicyMapping
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_NOTICENUMBERS

 NoticeNumbers ::= SequenceOf Integer  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateNoticeNumbers
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_DELTACRLINDICATOR

 DeltaCRLIndicator ::= CRLNumber 

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateDeltaCRLIndicator
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_ATTRIBUTESSYNTAX

 AttributesSyntax ::= SequenceOf Attribute  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateAttributesSyntax
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_BIOMETRICSSYNTAX

 BiometricsSyntax ::= SequenceOf BiometricData  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateBiometricsSyntax
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_BIOMETRICDATA

 BiometricData ::= Sequence 
     {
               typeOfBiometricData TypeOfBiometricData 
                     hashAlgorithm AlgorithmIdentifier 
                 biometricDataHash OctetString 
                     sourceDataUri IA5String 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateBiometricData
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_TYPEOFBIOMETRICDATA

 TypeOfBiometricData ::= Choice 
     {
                    predefinedType Integer 
                  biometricDataOID OID 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateTypeOfBiometricData
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_QCSTATEMENTSSYNTAX

 QCStatementsSyntax ::= SequenceOf QCStatement  {}

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateQCStatementsSyntax
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_QCSTATEMENT

 QCStatement ::= Sequence 
     {
                      statementOID OID 
                     statementInfo OctetString 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateQCStatement
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_ENTRUSTVERSIONINFO

 EntrustVersionInfo ::= Sequence 
     {
                           version GeneralString 
                             usage BitString 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateEntrustVersionInfo
    (
        ANSC_HANDLE                 hReserved
    );

/**********************************************************************

 OBJECT -- ANSC_ASN1_EXTENSIONVALUE

 ExtensionValue ::= Choice 
     {
            authorityKeyIdentifier AuthorityKeyIdentifier 
                          keyUsage KeyUsage 
             privateKeyUsagePeriod PrivateKeyUsagePeriod 
                      certPolicies CertificatePolicies 
                    policyMappings PolicyMappings 
            subDirectoryAttributes SubjectDirectoryAttributes 
                  basicConstraints BasicConstraints 
                   nameConstraints NameConstraints 
                 policyConstraints PolicyConstraints 
               crlDistPointsSyntax CRLDistPointsSyntax 
                 extKeyUsageSyntax ExtKeyUsageSyntax 
              authInfoAccessSyntax AuthorityInfoAccessSyntax 
                         crlNumber CRLNumber 
          issuingDistributionPoint IssuingDistributionPoint 
                         crlReason CRLReason 
               holdInstructionCode HoldInstructionCode 
                    invalidityDate InvalidityDate 
                      isserAltName GeneralNames 
                    subjectAltName GeneralNames 
                 deltaCRLIndicator CRLNumber 
                 certificateIssuer GeneralNames 
              subjectKeyIdentifier OctetString 
                  biometricsSyntax BiometricsSyntax 
                qcStatementsSyntax QCStatementsSyntax 
                  netscapeCertType BitString 
                   netscapeComment IA5String 
                        commonName DirectoryString 
                entrustVersionInfo EntrustVersionInfo 
                caKeyCertIndexPair Integer 
                          msString MSString 
     }

 **********************************************************************/

ANSC_HANDLE 
AnscAsn1CreateExtensionValue
    (
        ANSC_HANDLE                 hReserved
    );


#endif  /*_ANSC_ASN1_EXTENSIONS_EXPORTED_API_H*/


