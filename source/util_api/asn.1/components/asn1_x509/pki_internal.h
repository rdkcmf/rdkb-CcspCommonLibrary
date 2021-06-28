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

    MODULE: pki_internal.h

    ---------------------------------------------------------------

    DESCRIPTION:

        Functions defined for ALCERTIFICATE_ATTRIBUTE

        *   PKICreateCertAttribute
        *   PKICreateCertAttributeWithName

        *   PKICertAttributeFree
        *   PKICertAttributeClone

        *   PKICertAttributeSetKeylengthInByte

        *   PKICertAttributeAddCountryName
        *   PKICertAttributeAddOrganizationName
        *   PKICertAttributeAddOrganUnitName
        *   PKICertAttributeAddCommonName
        *   PKICertAttributeAddStateOrProvinceName
        *   PKICertAttributeAddGenerationQualifierName
        *   PKICertAttributeAddLocalityName
        *   PKICertAttributeAddPKCS9Email
        *   PKICertAttributeAddSerialNumber
        *   PKICertAttributeAddDNQualifier
        *   PKICertAttributeAddStreetAddress

        *   PKICertAttributeSetName
        *   PKICertAttributeSetChallengePassword
        *   PKICertAttributeSetSurName
        *   PKICertAttributeSetGivenName
        *   PKICertAttributeSetInitials
        *   PKICertAttributeSetTitle
        *   PKICertAttributeSetDescription
        *   PKICertAttributeSetUnstructedName
        *   PKICertAttributeSetUnstructedAddress        
        *   PKICertAttributeAddDomainName
        *   PKICertAttributeAddEmailAddress
        *   PKICertAttributeAddIPAddress
        *   PKICertAttributeSetSubjectAltName
        *   PKICertAttributeInitRFC2253Encode
        *   PKICertAttributeTrace

        *   PKICertAttributeGetCountryName
        *   PKICertAttributeGetOrganizationName
        *   PKICertAttributeGetOrganUnitName
        *   PKICertAttributeGetCommonName
        *   PKICertAttributeGetStateOrProvinceName
        *   PKICertAttributeGetGenerationQualifierName
        *   PKICertAttributeGetName
        *   PKICertAttributeGetDNQualifier
        *   PKICertAttributeGetLocalityName
        *   PKICertAttributeGetPKCS9Email
        *   PKICertAttributeGetSerialNumber
        *   PKICertAttributeGetChallengePassword
        *   PKICertAttributeGetSurName
        *   PKICertAttributeGetGivenName
        *   PKICertAttributeGetInitials
        *   PKICertAttributeGetTitle
        *   PKICertAttributeGetDescription
        *   PKICertAttributeGetUnstructedName
        *   PKICertAttributeGetUnstructedAddress
        *   PKICertAttributeGetStreetAddress
        *   PKICertAttributeGetSubjectAltName
        *   PKICertAttributeGetFullName

    ---------------------------------------------------------------

    ENVIRONMENT:

        platform independent

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   05/09/2002  initial revision
        *   08/20/2002  PKICertAttributeInitRFC2253Encode was supported;
        *   08/20/2002  PKICertAttributeTrace was supported;
        *   08/29/2002  KB5Principal name was added.
        *   09/15/2003  Add APIs for SubjectAltName.
        *   11/26/2003  Add APIs for StreetAddress 
        *   12/02/2003  Add FullName Support

 **********************************************************************/


#ifndef  _PKI_INTERNAL_H
#define  _PKI_INTERNAL_H

/*******************************************************************
          FUNCTIONS IMPLEMENTED IN PKI_CERT_ATTR.C
********************************************************************/
void
PKICertAttributeFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
PKICertAttributeClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
PKICertAttributeSetKeylengthInByte
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       length
    );

BOOLEAN
PKICertAttributeAddCountryName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddOrganizationName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddOrganUnitName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddCommonName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddStreetAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddStateOrProvinceName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddGenerationQualifierName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddDNQualifier
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddLocalityName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddPKCS9Email
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddSerialNumber
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetChallengePassword
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetSurName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetGivenName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetInitials
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetTitle
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetDescription
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetUnstructedName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetUnstructedAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetSubjectAltName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddDomainName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddEmailAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeAddIPAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    );

BOOLEAN
PKICertAttributeSetKB5Name
    (
        ANSC_HANDLE                 hThisObject,        
        ANSC_HANDLE                 hRealmName,
        ANSC_HANDLE                 hPrinName
    );

BOOLEAN
PKICertAttributeInitRFC2253Encode
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pRFC2253
    );

PCHAR
PKICertAttributeGetCountryName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetOrganizationName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetOrganUnitName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetCommonName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetStateOrProvinceName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetGenerationQualifierName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetDNQualifier
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetLocalityName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetPKCS9Email
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetSerialNumber
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetChallengePassword
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetSurName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetGivenName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetInitials
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetTitle
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetDescription
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetUnstructedName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetUnstructedAddress
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetStreetAddress
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetSubjectAltName
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
PKICertAttributeGetFullName
    (
        ANSC_HANDLE                 hThisObject
    );

VOID
PKICertAttributeTrace
    (
        ANSC_HANDLE                 hThisObject
    );

/*******************************************************************
          FUNCTIONS IMPLEMENTED IN PKI_CERT_ATTR.C
********************************************************************/
ANSC_HANDLE
PKIParsingAltName
    (
        PCHAR                       pString
    );

ULONG
PKIAltNameGetTotalBlock
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
PKIAltNameGetBlockType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PUCHAR
PKIAltNameGetBlockValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );


ANSC_STATUS
PKIAltNameFree
    (
        ANSC_HANDLE                 hThisObject
    );

#endif  /*_PKI_INTERNAL_H*/

