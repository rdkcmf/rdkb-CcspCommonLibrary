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

    module: pki_export_utils.c

        For Abstract Syntax Notation One (ASN.1)
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file implements the utility functions for 
        the PKI obejcts.

        *   PKIIsExtensionsObjectValid
        *   PKIFindAttributeWithOIDType
        *   PKIOIDStringToSignatureType
        *   PKISignTypeToOIDString
        *   PKIOIDStringToKeyType
        *   PKIKeyTypeToOIDString
        *   PKICopyStringAfterTrim

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        05/09/02    initial revision.
        08/21/02    add PKICopyStringAfterTrim;

**********************************************************************/

#include "ansc_asn1_advanced_local.h"
#include "ansc_string_util.h"
#include "safec_lib_common.h"

BOOLEAN
PKIIsExtensionsObjectValid
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_EXTENSIONS           pThisObject = (PANSC_ASN1_EXTENSIONS)hThisObject;
    PANSC_ASN1_EXTENSION            pChild, pOtherChild;
    CHAR                            pOIDString[256], pOIDOther[256];
    ULONG                           i, j;

    if( hThisObject == NULL)
    {
        return FALSE;
    }

    if( pThisObject->ClassName != NULL && !AnscEqualString1("ANSC_ASN1_EXTENSIONS",pThisObject->ClassName,FALSE))
    {
        /* it's not the extensions object */
        return FALSE;    
    }

    /*
     *  The EXTENSION of the same type cannot be included twice, so we check it here;
     *
     */    
    if( pThisObject->GetChildCount(pThisObject) <= 1)
    {
        return TRUE;
    }

    AnscZeroMemory( pOIDString, 256);
    AnscZeroMemory( pOIDOther, 256);

    for( i = 0; i < pThisObject->GetChildCount(pThisObject) - 1; i ++)
    {
        pChild      = (PANSC_ASN1_EXTENSION)pThisObject->GetChildByIndex( pThisObject, i);

        if( pChild != NULL)
        {
            pChild->GetExtensionType(pChild, pOIDString);

            for( j = i + 1;  j < pThisObject->GetChildCount(pThisObject); j ++)
            {
                pOtherChild  = (PANSC_ASN1_EXTENSION)pThisObject->GetChildByIndex( pThisObject, j);

                if( pOtherChild != NULL)
                {
                    pOtherChild->GetExtensionType(pOtherChild, pOIDOther);

                    if( AnscEqualString1(pOIDOther, pOIDString, FALSE))
                    {
                        return FALSE;
                    }
                }
            }
        }
    }

    return TRUE;
}

ANSC_HANDLE
PKIFindAttributeWithOIDType
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    )
{
    PANSC_ASN1_ATTRIBUTES           pAttributes = (PANSC_ASN1_ATTRIBUTES)hThisObject;
    PANSC_ASN1_OBJECT               pThisObject = (PANSC_ASN1_OBJECT)hThisObject;
    PANSC_ASN1_ATTRIBUTE            pChild;        
    PANSC_ASN1_OIDEN                pOIDObject;
    ULONG                           i, count;
    CHAR                            pOIDGet[256] = { 0 };

    if(pThisObject->ClassName != NULL &&  !AnscEqualString1("ANSC_ASN1_ATTRIBUTES", pThisObject->ClassName,FALSE))
    {
        return NULL;
    }

    count = pAttributes->GetChildCount(pAttributes);

    if( count == 0)
    {
        return NULL;
    }

    for( i = 0; i < count; i ++)
    {
        pChild      = (PANSC_ASN1_ATTRIBUTE)pAttributes->GetChildByIndex( pAttributes, i);

        pOIDObject  = (PANSC_ASN1_OIDEN)pChild->GetChildByIndex(pChild,0);

        pOIDObject->GetStringOIDValue(pOIDObject, pOIDGet);

        if( AnscEqualString1(pOIDGet, pOIDString,FALSE))
        {
            return pChild->GetChildByIndex(pChild,1);
        }
    }

    return NULL;
}

SIGNATURE_TYPE
PKIOIDStringToSignatureType
    (
        PCHAR                       pOIDString
    )
{
    if( pOIDString == NULL)
    {
        return SIGNATURE_RESERVED;
    }

    if( AnscEqualString1( pOIDString, "1.2.840.10040.4.3", FALSE))
    {
        return DSA_SHA1_SIGNATURE;
    }
    else if( AnscEqualString1( pOIDString, "1.2.840.113549.1.1.5", FALSE))
    {
        return RSA_SHA1_SIGNATURE;
    }
    else if( AnscEqualString1( pOIDString, "1.2.840.113549.1.1.4", FALSE))
    {
        return RSA_MD5_SIGNATURE;
    }
    else if( AnscEqualString1( pOIDString, "1.2.840.113549.1.1.2", FALSE))
    {
        return RSA_MD2_SIGNATURE;
    }

    return SIGNATURE_RESERVED;
}

PCHAR
PKISignTypeToOIDString
    (
        SIGNATURE_TYPE              sigType
    )
{
    switch( sigType)
    {
        case DSA_SHA1_SIGNATURE:

             return "1.2.840.10040.4.3";

        case RSA_SHA1_SIGNATURE:

             return "1.2.840.113549.1.1.5";

        case RSA_MD5_SIGNATURE:

             return "1.2.840.113549.1.1.4";

        case RSA_MD2_SIGNATURE:

             return "1.2.840.113549.1.1.2";

        default:

             return "";

    }
}

PKI_KEY_TYPE
PKIOIDStringToKeyType
    (
        PCHAR                       pOIDString
    )
{
    if( pOIDString == NULL)
    {
        return PKI_KEY_RESERVED;
    }

    if( AnscEqualString1( pOIDString, "1.2.840.10040.4.1", FALSE))
    {
        return PKI_DSA_KEY;
    }
    else if( AnscEqualString1( pOIDString, "1.2.840.113549.1.1.1", FALSE))
    {
        return PKI_RSA_KEY;
    }
    else if( AnscEqualString1( pOIDString, "1.2.840.10046.2.1", FALSE))
    {
        return PKI_DH_KEY;
    }

    return PKI_KEY_RESERVED;
}

PCHAR
PKIKeyTypeToOIDString
    (
        PKI_KEY_TYPE                keyType
    )
{
    switch( keyType)
    {
        case PKI_DSA_KEY:

             return "1.2.840.10040.4.1";

        case PKI_RSA_KEY:

             return "1.2.840.113549.1.1.1";

        case PKI_DH_KEY:

             return "1.2.840.10046.2.1";

        default:

             return "";

    }
}

/**************************************************************
 *  Copy String after trim
 *
 *  Ingore the space at the both side.
 *  Copy "value" from " value  ".
 ***************************************************************/
BOOLEAN
PKICopyStringAfterTrim
    (
        PCHAR                       pDest,
        PCHAR                       pSource
    )
{
    ULONG                           begin    = 0;
    ULONG                           end      = 0;
    ULONG                           length;

    if( pDest == NULL || pSource == NULL)
    {
        return FALSE;
    }

    length = AnscSizeOfString(pSource);

    if( length == 0)
    {
        return FALSE;
    }

    /* left */
    while(pSource[begin] == ' ')
    {
        begin ++;

        if( begin >= length)
        {
            return FALSE;
        }
    }

    /* right */
    end = length - 1;

    while( pSource[end] == ' ')
    {
        if( end == 0)
        {
            return FALSE;
        }

        end --;
    }

    if( begin > end )
    {
        return FALSE;
    }

    AnscCopyMemory
        (
            pDest,
            (PVOID)(pSource + begin),
            (end - begin + 1)
        );

    return TRUE;
}

/*********************************************************************

  OBJECT IMPLEMENTED: PKI_ALTNAME_PARSER

 ********************************************************************/
static BOOLEAN
pkiParsingOneName
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pInput
    )
{
    PPKI_ALTNAME_PARSER             pParser         = (PPKI_ALTNAME_PARSER)hThisObject;
    int                             index           = 0;
    CHAR                            pBuffer[128]    = { 0 };
    PCHAR                           pOneLine;
    int                             IPValue;
    PCHAR                           pTemp;
    
    PKICopyStringAfterTrim(pBuffer, pInput);

    pOneLine = pBuffer;

    if( hThisObject == NULL || pOneLine == NULL || AnscSizeOfString(pOneLine) <= 0)
    {
        return FALSE;
    }

    if( AnscStrStr(pOneLine, ALT_EMAIL_NAME) == pOneLine)
    {       
        pTemp = AnscStrChr(pOneLine, '=');

        if( pTemp == NULL)
        {
            return FALSE;
        }

        pTemp ++;

        if( !PKICopyStringAfterTrim
            (
                pParser->pName[pParser->uTotalBlock],
                pTemp
            ))
        {
            return FALSE;
        }

        pParser->uType[pParser->uTotalBlock] = ALT_TYPE_EMAIL_NAME;
   }
    else if( AnscStrStr(pOneLine, ALT_DNS_NAME) == pOneLine)
    {
        /* load the DNS name here. */
        pTemp = AnscStrChr(pOneLine, '=');

        if( pTemp == NULL)
        {
            return FALSE;
        }

        pTemp ++;

        if(!PKICopyStringAfterTrim
            (
                pParser->pName[pParser->uTotalBlock],
                pTemp
            ))
        {
            return FALSE;
        }

        pParser->uType[pParser->uTotalBlock] = ALT_TYPE_DNS_NAME;

    }
    else if( AnscStrStr(pOneLine, ALT_URI_NAME) == pOneLine)
    {
        /* load the URI name here */
        pTemp = AnscStrChr(pOneLine, '=');

        if( pTemp == NULL)
        {
            return FALSE;
        }

        pTemp ++;

        if(!PKICopyStringAfterTrim
            (
                pParser->pName[pParser->uTotalBlock],
                pTemp
            ))
        {
            return FALSE;
        }

        pParser->uType[pParser->uTotalBlock] = ALT_TYPE_URI_NAME;
    }
    else if( AnscStrStr(pOneLine, ALT_IP_NAME) == pOneLine)
    {
        /* load the IP Address here */
        pTemp = AnscStrChr(pOneLine, '=');

        if( pTemp == NULL)
        {
            return FALSE;
        }

        pOneLine = pTemp + 1;

        /* trim the left space */
        while( pOneLine[0] == ' ')
        {
            pOneLine ++;
            
            if( pOneLine == NULL)
            {
                return FALSE;
            }
        }

        /* get the ip address value */
        pTemp = AnscStrChr(pOneLine, '.');
        if( pTemp == NULL)
        {
            return FALSE;
        }

        pOneLine[pTemp - pOneLine] = 0;
        IPValue = AnscString2Int(pOneLine);

        if( IPValue >= 256)
        {
            return FALSE;
        }

        /* get the value */
        pParser->uType[pParser->uTotalBlock] = ALT_TYPE_IP_NAME;
        index = 1;

        pParser->pName[pParser->uTotalBlock][index] = IPValue;
        index    ++;
        pOneLine = pTemp + 1;

        /* the second one */
        pTemp = AnscStrChr(pOneLine, '.');
        if( pTemp == NULL)
        {
            return FALSE;
        }

        pOneLine[pTemp - pOneLine] = 0;
        IPValue = AnscString2Int(pOneLine);

        if( IPValue >= 256)
        {
            return FALSE;
        }

        pParser->pName[pParser->uTotalBlock][index] = IPValue;
        index    ++;
        pOneLine = pTemp + 1;

        /* the third one */
        pTemp = AnscStrChr(pOneLine, '.');
        if( pTemp == NULL)
        {
            return FALSE;
        }

        pOneLine[pTemp - pOneLine] = 0;
        IPValue = AnscString2Int(pOneLine);

        if( IPValue >= 256)
        {
            return FALSE;
        }

        pParser->pName[pParser->uTotalBlock][index] = IPValue;
        index    ++;
        pOneLine = pTemp + 1;

        /* the last one */
        if( pOneLine == NULL)
        {
            return FALSE;
        }
        IPValue = AnscString2Int(pOneLine);

        if( IPValue >= 256)
        {
            return FALSE;
        }

        pParser->pName[pParser->uTotalBlock][index] = IPValue;
        index    ++;


        /* get the IP length */
        pParser->pName[pParser->uTotalBlock][0] = index - 1;
    }
    else
    {
        return FALSE;
    }

    pParser->uTotalBlock ++;

    return TRUE;
}

ANSC_HANDLE
PKIParsingAltName
    (
        PCHAR                       pString
    )
{
    PPKI_ALTNAME_PARSER             pThisObject         = NULL;
    CHAR                            pTempBuffer[256]    = { 0 };
    BOOLEAN                         bParsingResult      = TRUE;
    PCHAR                           pBack, pBuffer;
    errno_t                         rc                  = -1;

    if( pString == NULL || AnscSizeOfString(pString) < 4)
    {
        return pThisObject;
    }

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pThisObject = (PPKI_ALTNAME_PARSER)AnscAllocateMemory(sizeof(PKI_ALTNAME_PARSER));

    if ( !pThisObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pThisObject->uTotalBlock        = 0;
    pThisObject->Parsing            = PKIParsingAltName;
    pThisObject->Remove             = PKIAltNameFree;
    pThisObject->GetTotalBlock      = PKIAltNameGetTotalBlock;
    pThisObject->GetBlockType       = PKIAltNameGetBlockType;
    pThisObject->GetBlockValue      = PKIAltNameGetBlockValue;

    /*
     *  The format for entering ipsec naming information is "type=value;", 
     *  the ';' are literal and must encapsulate each item, 
     *  type is one of ip, dns, email, or uri
     *
     *  Examples:  
     *  ip=127.0.0.1;dns=spice.entrust.com;email=geek@entrust.com   
     *  ip=127.0.0.1;dns=gonzo.entrust.com;ip=47.97.226.20;dns=blah.cisco.com
     */
    rc = strcpy_s( pTempBuffer, sizeof(pTempBuffer), pString);
    ERR_CHK(rc);

    pBuffer = pTempBuffer;
    pBack   = AnscStrChr(pBuffer, ';');

    while(pBack != NULL)
    {
        pBuffer[pBack - pBuffer] = 0;

        bParsingResult = pkiParsingOneName(pThisObject, pBuffer);

        if(!bParsingResult)
        {
            break;
        }
        
        pBuffer = pBack + 1;

        pBack = AnscStrChr(pBuffer, ';');
    }

    if( bParsingResult && AnscSizeOfString(pBuffer) > 0)
    {
        bParsingResult = pkiParsingOneName(pThisObject, pBuffer);
    }

    if(!bParsingResult)
    {
       pThisObject->Remove(pThisObject);

       return NULL;
    }

    return pThisObject;
}

ULONG
PKIAltNameGetTotalBlock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PPKI_ALTNAME_PARSER             pParser     = (PPKI_ALTNAME_PARSER)hThisObject;

    return pParser->uTotalBlock;
}

ULONG
PKIAltNameGetBlockType
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    PPKI_ALTNAME_PARSER             pParser     = (PPKI_ALTNAME_PARSER)hThisObject;

    return pParser->uType[index];
}

PUCHAR
PKIAltNameGetBlockValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    PPKI_ALTNAME_PARSER             pParser     = (PPKI_ALTNAME_PARSER)hThisObject;

    return (PUCHAR)pParser->pName[index];
}


ANSC_STATUS
PKIAltNameFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PPKI_ALTNAME_PARSER             pThisObject  = (PPKI_ALTNAME_PARSER)hThisObject;

    if( hThisObject == NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    pThisObject->uTotalBlock = 0;

    AnscFreeMemory(hThisObject);

    return ANSC_STATUS_SUCCESS;
}

