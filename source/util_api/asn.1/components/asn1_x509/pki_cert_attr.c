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

    MODULE: pki_cert_attr.c

    ---------------------------------------------------------------

    DESCRIPTION:

        Functions implemented for ALCERTIFICATE_ATTRIBUTE

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
        *   09/15/2003  Add APIs to add SubjectAltName.
        *   11/26/2003  Add APIs for StreetAddress 
        *   12/02/2003  Add FullName Support

 **********************************************************************/

#include "ansc_asn1_advanced_local.h"
#include "ansc_string_util.h"
#include "safec_lib_common.h"


ANSC_HANDLE
PKICreateCertAttribute
    (
        ANSC_HANDLE                 hReserved
    )
{
    UNREFERENCED_PARAMETER(hReserved);
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = NULL;

    pMyObject = (PALCERTIFICATE_ATTRIBUTE)AnscAllocateMemory(sizeof(ALCERTIFICATE_ATTRIBUTE));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->SignAlgor            = RSA_MD5_SIGNATURE;
    pMyObject->KeyType              = PKI_RSA_KEY;
    pMyObject->KeyLength            = 128;                      /* default key length */

    pMyObject->Create               = PKICreateCertAttribute;
    pMyObject->Clone                = PKICertAttributeClone;
    pMyObject->Remove               = PKICertAttributeFree;

    pMyObject->SetKeylengthInByte   = PKICertAttributeSetKeylengthInByte;

    pMyObject->GetCountryName       = PKICertAttributeGetCountryName;
    pMyObject->GetChallengePassword = PKICertAttributeGetChallengePassword;
    pMyObject->GetCommonName        = PKICertAttributeGetCommonName;
    pMyObject->GetDescription       = PKICertAttributeGetDescription;
    pMyObject->GetDNQualifier       = PKICertAttributeGetDNQualifier;
    pMyObject->GetGenerationQualifierName
                                    = PKICertAttributeGetGenerationQualifierName;
    pMyObject->GetGivenName         = PKICertAttributeGetGivenName;
    pMyObject->GetInitials          = PKICertAttributeGetInitials;
    pMyObject->GetLocalityName      = PKICertAttributeGetLocalityName;
    pMyObject->GetName              = PKICertAttributeGetName;
    pMyObject->GetOrganizationName  = PKICertAttributeGetOrganizationName;
    pMyObject->GetOrganUnitName     = PKICertAttributeGetOrganUnitName;
    pMyObject->GetPKCS9Email        = PKICertAttributeGetPKCS9Email;
    pMyObject->GetSerialNumber      = PKICertAttributeGetSerialNumber;
    pMyObject->GetStateOrProvinceName   
                                    = PKICertAttributeGetStateOrProvinceName;
    pMyObject->GetSubjectAltName    = PKICertAttributeGetSubjectAltName;
    pMyObject->GetSurName           = PKICertAttributeGetSurName;
    pMyObject->GetTitle             = PKICertAttributeGetTitle;
    pMyObject->GetUnstructedAddress = PKICertAttributeGetUnstructedAddress;
    pMyObject->GetUnstructedName    = PKICertAttributeGetUnstructedName;
    pMyObject->GetStreetAddress     = PKICertAttributeGetStreetAddress;
    pMyObject->InitRFC2253Encode    = PKICertAttributeInitRFC2253Encode;
    pMyObject->GetFullName          = PKICertAttributeGetFullName;

    pMyObject->AddCountryName       = PKICertAttributeAddCountryName;
    pMyObject->AddLocalityName      = PKICertAttributeAddLocalityName;
    pMyObject->AddCommonName        = PKICertAttributeAddCommonName;
    pMyObject->AddOrganizationName  = PKICertAttributeAddOrganizationName;
    pMyObject->AddOrganUnitName     = PKICertAttributeAddOrganUnitName;
    pMyObject->AddPKCS9Email        = PKICertAttributeAddPKCS9Email;
    pMyObject->AddSerialNumber      = PKICertAttributeAddSerialNumber;
    pMyObject->AddStateOrProvinceName   
                                    = PKICertAttributeAddStateOrProvinceName;
    pMyObject->AddDNQualifier       = PKICertAttributeAddDNQualifier;
    pMyObject->AddGenerationQualifierName
                                    = PKICertAttributeAddGenerationQualifierName;
    pMyObject->AddStreetAddress     = PKICertAttributeAddStreetAddress;
    pMyObject->AddEmailAddress      = PKICertAttributeAddEmailAddress;
    pMyObject->AddDomainName        = PKICertAttributeAddDomainName;
    pMyObject->AddIPAddress         = PKICertAttributeAddIPAddress;

    pMyObject->SetDescription       = PKICertAttributeSetDescription;
    pMyObject->SetChallengePassword = PKICertAttributeSetChallengePassword;
    pMyObject->SetGivenName         = PKICertAttributeSetGivenName;
    pMyObject->SetInitials          = PKICertAttributeSetInitials;
    pMyObject->SetName              = PKICertAttributeSetName;
    pMyObject->SetSubjectAltName    = PKICertAttributeSetSubjectAltName;
    pMyObject->SetSurName           = PKICertAttributeSetSurName;
    pMyObject->SetTitle             = PKICertAttributeSetTitle;
    pMyObject->SetUnstructedAddress = PKICertAttributeSetUnstructedAddress;
    pMyObject->SetUnstructedName    = PKICertAttributeSetUnstructedName;
    pMyObject->SetKB5PrincipalName  = PKICertAttributeSetKB5Name;
    pMyObject->Trace                = PKICertAttributeTrace;

    AnscZeroMemory(pMyObject->pFullName, MAXI_FULL_NAME_SIZE);

    return  (ANSC_HANDLE)pMyObject;
}

ANSC_HANDLE
PKICreateCertAttributeWithName
    (
        ANSC_HANDLE                 hReserved,
        PCHAR                       pRFC2253Name
    )
{
    PALCERTIFICATE_ATTRIBUTE        pThisObject;

    pThisObject = (PALCERTIFICATE_ATTRIBUTE)
        PKICreateCertAttribute(hReserved);

    if( pThisObject == NULL)
    {
        return NULL;
    }

    if( pRFC2253Name != NULL && AnscSizeOfString(pRFC2253Name) > 0)
    {
        pThisObject->InitRFC2253Encode
             (
                pThisObject,
                pRFC2253Name
             );
    }

    pThisObject->KeyLength  = 128;
    pThisObject->KeyType    = PKI_RSA_KEY;
    pThisObject->SignAlgor  = RSA_SHA1_SIGNATURE;

    return pThisObject;
}

void
PKICertAttributeFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

#ifndef _NO_PKI_KB5_SUPPORT

    if( pMyObject->hKB5PrinName != NULL)
    {
        AnscKb5PrinNameDelete
            (
                (PKB5_PRINNAME_STRUCTURE)pMyObject->hKB5PrinName
            );
    }

    if( pMyObject->hKB5RealmName != NULL)
    {
        AnscKb5RealmDelete
            (
                (PKB5_REALM_STRUCTURE)pMyObject->hKB5RealmName
            );
    }

#endif

    AnscFreeMemory(pMyObject);
}

ANSC_HANDLE
PKICertAttributeClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    PALCERTIFICATE_ATTRIBUTE        pNewObject = NULL;

    if( pMyObject == NULL)
    {
        return pNewObject;
    }

    pNewObject = (PALCERTIFICATE_ATTRIBUTE)pMyObject->Create(NULL);

    if( pNewObject == NULL)
    {
        return pNewObject;
    }

    AnscCopyMemory
        (
            pNewObject,
            pMyObject,
            sizeof(ALCERTIFICATE_ATTRIBUTE)
        );

    return pNewObject;
}

BOOLEAN
PKICertAttributeSetKeylengthInByte
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       length
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return FALSE;
    }


    if( length < 64 || length > 1028)
    {
        return FALSE;
    }

    pMyObject->KeyLength        = length;

    return TRUE;

}
/**************************************************************
 *
 *  Functions Get/Set/Add attribute values
 *
 ***************************************************************/


static BOOLEAN
addAttributeValue
    (
        PCHAR                       pOldValue,
        ULONG                       maxiSize,
        PCHAR                       pAddedValue
    )
{
    ULONG                           uLength;
    ULONG                           uAdded;
    errno_t                         rc = -1;

    uAdded = AnscSizeOfString(pAddedValue);

    if( uAdded >= maxiSize)
    {
        AnscTrace("Name size is too big: %lu >= %lu\n", uAdded, maxiSize);

        return FALSE;
    }

    uLength = AnscSizeOfString( pOldValue);

    if( uLength == 0 )
    {
        rc = strcpy_s(pOldValue, maxiSize, pAddedValue);
        ERR_CHK(rc);

        return TRUE;
    }

    if( uAdded + uLength + 1 >= maxiSize)
    {
        AnscTrace("Name size is too big: %ld >= %ld\n", uAdded + uLength + 1, maxiSize);

        return FALSE;
    }


    pOldValue[uLength] = ';';

    AnscCopyMemory( (PUCHAR)(pOldValue + uLength + 1), pAddedValue, AnscSizeOfString(pAddedValue));

    return TRUE;
}


BOOLEAN
PKICertAttributeAddCountryName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pCountryName,
                COUNTRY_NAME_SIZE,
                pValue
            );    
}

BOOLEAN
PKICertAttributeAddOrganizationName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pOrganizationName,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );
   
}

BOOLEAN
PKICertAttributeAddOrganUnitName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pOrganizationUnitName,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );

}


BOOLEAN
PKICertAttributeAddCommonName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pCommonName,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );
}
    

BOOLEAN
PKICertAttributeAddStateOrProvinceName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pStateOrProvinceName,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );

}


BOOLEAN
PKICertAttributeAddGenerationQualifierName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pGenerationQualifier,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );

}


BOOLEAN
PKICertAttributeSetName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pName, sizeof(pMyObject->pName), pValue);
    ERR_CHK(rc);

    return TRUE;
}


BOOLEAN
PKICertAttributeAddDNQualifier
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pDNQualifier,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );

}


BOOLEAN
PKICertAttributeAddLocalityName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pLocalityName,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );
}


BOOLEAN
PKICertAttributeAddPKCS9Email
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pPKCS9Email,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );
}


BOOLEAN
PKICertAttributeAddSerialNumber
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pSerialNumber,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );
}


BOOLEAN
PKICertAttributeSetChallengePassword
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pChallengePassword, sizeof(pMyObject->pChallengePassword), pValue);
    ERR_CHK(rc);

    return TRUE;
}


BOOLEAN
PKICertAttributeSetSurName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pSurName, sizeof(pMyObject->pSurName), pValue);
    ERR_CHK(rc);

    return TRUE;
}


BOOLEAN
PKICertAttributeSetGivenName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pGivenName, sizeof(pMyObject->pGivenName), pValue);
    ERR_CHK(rc);

    return TRUE;
}


BOOLEAN
PKICertAttributeSetInitials
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pInitials, sizeof(pMyObject->pInitials), pValue);
    ERR_CHK(rc);

    return TRUE;
}
    

BOOLEAN
PKICertAttributeSetTitle
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pTitle, sizeof(pMyObject->pTitle), pValue);
    ERR_CHK(rc);

    return TRUE;
}


BOOLEAN
PKICertAttributeSetDescription
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pDescription, sizeof(pMyObject->pDescription), pValue);
    ERR_CHK(rc);

    return TRUE;
}


BOOLEAN
PKICertAttributeSetUnstructedName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pUnstructedName, sizeof(pMyObject->pUnstructedName), pValue);
    ERR_CHK(rc);

    return TRUE;
}

BOOLEAN
PKICertAttributeAddStreetAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    return 
        addAttributeValue
            (
                pMyObject->pStreetAddress,
                ATTRIBUTE_BUFFER_SIZE,
                pValue
            );
}


BOOLEAN
PKICertAttributeSetUnstructedAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > ATTRIBUTE_BUFFER_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pUnstructedAddress, sizeof(pMyObject->pUnstructedAddress), pValue);
    ERR_CHK(rc);

    return TRUE;
}


BOOLEAN
PKICertAttributeSetSubjectAltName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    ULONG                           uLength;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    uLength     = AnscSizeOfString( pValue);
    
    if( uLength > SUBJECT_ALT_NAME_SIZE)
    {
        return FALSE;
    }

    rc = strcpy_s( pMyObject->pSubjectAltName, sizeof(pMyObject->pSubjectAltName), pValue);
    ERR_CHK(rc);

    return TRUE;
}

PCHAR
PKICertAttributeGetCountryName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pCountryName;
}

PCHAR
PKICertAttributeGetOrganizationName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pOrganizationName;
}

PCHAR
PKICertAttributeGetOrganUnitName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pOrganizationUnitName;
}


PCHAR
PKICertAttributeGetCommonName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pCommonName;
}


PCHAR
PKICertAttributeGetStateOrProvinceName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pStateOrProvinceName;
}


PCHAR
PKICertAttributeGetGenerationQualifierName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pGenerationQualifier;
}


PCHAR
PKICertAttributeGetName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pName;
}

PCHAR
PKICertAttributeGetDNQualifier
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pDNQualifier;
}

PCHAR
PKICertAttributeGetLocalityName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pLocalityName;
}


PCHAR
PKICertAttributeGetPKCS9Email
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pPKCS9Email;
}


PCHAR
PKICertAttributeGetSerialNumber
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pSerialNumber;
}


PCHAR
PKICertAttributeGetChallengePassword
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pChallengePassword;
}


PCHAR
PKICertAttributeGetSurName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pSurName;
}


PCHAR
PKICertAttributeGetGivenName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pGivenName;
}


PCHAR
PKICertAttributeGetInitials
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pInitials;
}


PCHAR
PKICertAttributeGetTitle
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pTitle;
}

PCHAR
PKICertAttributeGetDescription
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pDescription;
}


PCHAR
PKICertAttributeGetUnstructedName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pUnstructedName;
}


PCHAR
PKICertAttributeGetUnstructedAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pUnstructedAddress;
}

PCHAR
PKICertAttributeGetStreetAddress
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pStreetAddress;
}


PCHAR
PKICertAttributeGetSubjectAltName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pMyObject == NULL )
    {
        return NULL;
    }

    return pMyObject->pSubjectAltName;
}

/**********************************************************************

    String  X.500 AttributeType
    ------------------------------
    CN      commonName
    L       localityName
    ST      stateOrProvinceName
    O       organizationName
    OU      organizationalUnitName
    C       countryName
    STREET  streetAddress
    SN      serialNumber
    Email   emailAddress

    Not supported here:

    DC      domainComponent
    UID     userid

 **********************************************************************/
static BOOLEAN
certAttrParsingOneName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pInput
    )
{
    PALCERTIFICATE_ATTRIBUTE        pThisObject     = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    CHAR                            pName[128]      = { 0 };
    CHAR                            pValue[128]     = { 0 };
    CHAR                            pBuffer[128]    = { 0 };
    PCHAR                           pOneLine;
    PCHAR                           pTemp;
    
    PKICopyStringAfterTrim(pBuffer, pInput);

    pOneLine = pBuffer;

    if( hThisObject == NULL || pOneLine == NULL || AnscSizeOfString(pOneLine) <= 0)
    {
        return FALSE;
    }

    /* find the '=' */
    pTemp = AnscStrChr(pOneLine, '=');

    if( pTemp == NULL)
    {
        return FALSE;
    }

    /* get the name */
    pBuffer[(pTemp - pBuffer)] = 0;
    PKICopyStringAfterTrim(pName, pBuffer);

    /* get the value */
    pTemp ++;
    PKICopyStringAfterTrim(pValue, pTemp);

    if( strcasecmp(pName,  X500_CN) == 0 )
    {
        /* copy name here */
        pThisObject->AddCommonName(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  X500_L) == 0 )
    {
        /* copy locality here */
        pThisObject->AddLocalityName(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  X500_ST) == 0 )
    {
        /* copy state here */
        pThisObject->AddStateOrProvinceName(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  X500_O) == 0 )
    {
        /* copy organization name here */
        pThisObject->AddOrganizationName(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  X500_OU) == 0 )
    {
        /* copy organization unit name here */
        pThisObject->AddOrganUnitName(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  X500_C) == 0 )
    {
        /* copy country name here */
        pThisObject->AddCountryName(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  X500_STREET) == 0 )
    {
        /* copy street address */
        pThisObject->AddStreetAddress(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  X500_SN) == 0 )
    {
        /* copy serial number */
        pThisObject->AddSerialNumber(pThisObject, pValue);
    }
    else if( strcasecmp(pName,  ALT_EMAIL_NAME) == 0 )
    {
        /* copy serial number */
        pThisObject->AddEmailAddress(pThisObject, pValue);
    }    
    else
    {
        AnscTrace("Unknown name '%s'.\n", pName);

        return FALSE;
    }

    return TRUE;
}

BOOLEAN
PKICertAttributeInitRFC2253Encode
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pRFC2253
    )
{
    PALCERTIFICATE_ATTRIBUTE        pThisObject         = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    PCHAR                           pTempBuffer         = NULL;
    BOOLEAN                         bParsingResult      = TRUE;
    PCHAR                           pBuffer;
    ULONG                           uLength;
    ULONG                           nIndex;
    errno_t                         rc                  = -1;

    if( hThisObject == NULL || pRFC2253 == NULL)
    {
        return FALSE;
    }

    uLength = AnscSizeOfString(pRFC2253);

    if( uLength < MAXI_FULL_NAME_SIZE)
    {
        rc = strcpy_s(pThisObject->pFullName, sizeof(pThisObject->pFullName), pRFC2253);
        ERR_CHK(rc);
    }

    /* 
     * sample RFC2253 encoding:
     *
     * CN=Steve Kille,O=Isode Limited,C=GB
     */
    pTempBuffer = (PCHAR)AnscAllocateMemory(uLength + 1);

    if( pTempBuffer == NULL)
    {
        return FALSE;
    }

    rc = strcpy_s(pTempBuffer, (uLength + 1), pRFC2253);
    ERR_CHK(rc);

    pBuffer = pTempBuffer;

    nIndex  = AnscStrCSpn(pBuffer, ";,");

    while(nIndex < uLength)
    {
        pBuffer[nIndex] = 0;

        bParsingResult = certAttrParsingOneName(pThisObject, pBuffer);

        if(!bParsingResult)
        {
            break;
        }
        
        pBuffer = pBuffer + nIndex + 1;

        nIndex  = AnscStrCSpn(pBuffer, ";,");

        uLength = AnscSizeOfString(pBuffer);
    }

    if( bParsingResult && AnscSizeOfString(pBuffer) > 0)
    {
        bParsingResult = certAttrParsingOneName(pThisObject, pBuffer);
    }

    AnscFreeMemory(pTempBuffer);

    return bParsingResult;
}

VOID
PKICertAttributeTrace
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pThisObject         = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pThisObject == NULL)
    {
        return;
    }

    AnscTrace("Trace the certificate attributes:\n");

    if( pThisObject->pCountryName[0] != 0)
    {
        AnscTrace("CountryName = '%s'\n", pThisObject->pCountryName);
    }

    if( pThisObject->pOrganizationName[0] != 0)
    {
        AnscTrace("OrganName = '%s'\n", pThisObject->pOrganizationName);
    }

    if( pThisObject->pOrganizationUnitName[0] != 0)
    {
        AnscTrace("OrganUnitName = '%s'\n", pThisObject->pOrganizationUnitName);
    }

    if( pThisObject->pCommonName[0] != 0)
    {
        AnscTrace("CommonName = '%s'\n", pThisObject->pCommonName);
    }

    if( pThisObject->pStateOrProvinceName[0] != 0)
    {
        AnscTrace("StateName = '%s'\n", pThisObject->pStateOrProvinceName);
    }

    if( pThisObject->pLocalityName[0] != 0)
    {
        AnscTrace("LocalityName = '%s'\n", pThisObject->pLocalityName);
    }

    if( pThisObject->pPKCS9Email[0] != 0)
    {
        AnscTrace("PKCS9Email = '%s'\n", pThisObject->pPKCS9Email);
    }

    if( pThisObject->pChallengePassword[0] != 0)
    {
        AnscTrace("ChallengePassword = '%s'\n", pThisObject->pChallengePassword);
    }

    if( pThisObject->pUnstructedName[0] != 0)
    {
        AnscTrace("UnstructedName = '%s'\n", pThisObject->pUnstructedName);
    }

    if( pThisObject->pUnstructedAddress[0] != 0)
    {
        AnscTrace("UnstructedAddress = '%s'\n", pThisObject->pUnstructedAddress);
    }

    if( pThisObject->pSubjectAltName[0] != 0)
    {
        AnscTrace("SubjectAltName = '%s'\n", pThisObject->pSubjectAltName);
    }
}

/*******************************************************************************
 *
 * Set the kerberos principal name, the called is responsible to free the handle.
 *
 *******************************************************************************/
BOOLEAN
PKICertAttributeSetKB5Name
    (
        ANSC_HANDLE                 hThisObject,        
        ANSC_HANDLE                 hRealmName,
        ANSC_HANDLE                 hPrinName
    )
{
    UNREFERENCED_PARAMETER(hRealmName);
    UNREFERENCED_PARAMETER(hPrinName);
    PALCERTIFICATE_ATTRIBUTE        pThisObject         = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( pThisObject == NULL)
    {
        return FALSE;
    }

#ifndef _NO_PKI_KB5_SUPPORT

    AnscKb5PrinNameClone
        (
            (PKB5_PRINNAME_STRUCTURE)pThisObject->hKB5PrinName,
            (PKB5_PRINNAME_STRUCTURE)hPrinName
        );

    AnscKb5RealmClone
        (
            (PKB5_REALM_STRUCTURE)pThisObject->hKB5RealmName,
            (PKB5_REALM_STRUCTURE)hRealmName
        );

    return TRUE;

#endif

    AnscTrace("We don't support kerberos principal name here.\n");

    return FALSE;
}

BOOLEAN
PKICertAttributeAddDomainName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    CHAR                            pTemp[64]  = { 0 };
    ULONG                           length     = 56;
    ULONG                           addLen;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    if( AnscSizeOfString(pValue) > 56)
    {
        return FALSE;
    }

    length = AnscSizeOfString(pMyObject->pSubjectAltName);

    if( length == 0)
    {
        rc = sprintf_s
            (
                pTemp,
                sizeof(pTemp),
                "dns=%s",
                pValue
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
    }
    else
    {
        rc = sprintf_s
            (
                pTemp,
                sizeof(pTemp),
                ";dns=%s",
                pValue
            );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
    }

    addLen = AnscSizeOfString(pTemp);

    if( length + addLen >= SUBJECT_ALT_NAME_SIZE)
    {
        return FALSE;
    }

    AnscCatString( pMyObject->pSubjectAltName, pTemp);

    return TRUE;
}

BOOLEAN
PKICertAttributeAddEmailAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    CHAR                            pTemp[64]  = { 0 };
    ULONG                           length     = 56;
    ULONG                           addLen;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    if( AnscSizeOfString(pValue) > 56)
    {
        return FALSE;
    }

    length = AnscSizeOfString(pMyObject->pSubjectAltName);

    rc = sprintf_s
        (
            pTemp,
            sizeof(pTemp),
            "%s%s=%s",
            ((length) ? ";" : ""),
            ALT_EMAIL_NAME,
            pValue
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return FALSE;
    }

    addLen = AnscSizeOfString(pTemp);

    if( length + addLen >= SUBJECT_ALT_NAME_SIZE)
    {
        return FALSE;
    }

    AnscCatString( pMyObject->pSubjectAltName, pTemp);

    return TRUE;
}

BOOLEAN
PKICertAttributeAddIPAddress
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pValue
    )
{
    PALCERTIFICATE_ATTRIBUTE        pMyObject  = (PALCERTIFICATE_ATTRIBUTE)hThisObject;
    CHAR                            pTemp[64]  = { 0 };
    ULONG                           length     = 56;
    ULONG                           addLen;
    errno_t                         rc         = -1;

    if( pMyObject == NULL || pValue == NULL)
    {
        return FALSE;
    }

    if( AnscSizeOfString(pValue) > 56)
    {
        return FALSE;
    }

    length = AnscSizeOfString(pMyObject->pSubjectAltName);

    rc = sprintf_s
        (
            pTemp,
            sizeof(pTemp),
            "%sip=%s",
            ((length) ? ";" : ""),
            pValue
        );
    if(rc < EOK)
    {
        ERR_CHK(rc);
        return FALSE;
    }

    addLen = AnscSizeOfString(pTemp);

    if( length + addLen >= SUBJECT_ALT_NAME_SIZE)
    {
        return FALSE;
    }

    AnscCatString( pMyObject->pSubjectAltName, pTemp);

    return TRUE;
}

PCHAR
PKICertAttributeGetFullName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PALCERTIFICATE_ATTRIBUTE        pThisObject         = (PALCERTIFICATE_ATTRIBUTE)hThisObject;

    if( AnscSizeOfString(pThisObject->pFullName) == 0)
    {
        return NULL;
    }

    return pThisObject->pFullName;
}

