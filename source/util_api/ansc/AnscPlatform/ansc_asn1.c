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

    module:	ansc_asn1.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to ASN.1 data handling.

        *   AnscAsn1AllocOctetString
        *   AnscAsn1CloneOctetString
        *   AnscAsn1CompOctetStrings
        *   AnscAsn1AllocOid
        *   AnscAsn1ConvOid
        *   AnscAsn1CloneOid
        *   AnscAsn1ConcOids
        *   AnscAsn1CompOids
        *   AnscAsn1CombOids
        *   AnscAsn1FindOidInArray
        *   AnscAsn1MarkOidInArray
        *   AnscAsn1SortOidArray
        *   AnscAsn1FreeOidArray
        *   AnscAsn1AllocInt
        *   AnscAsn1CloneInt
        *   AnscAsn1CompInts

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/10/02    initial revision.

**********************************************************************/


#include "ansc_global.h"


ANSC_HANDLE
AnscAsn1AllocOctetString
    (
        PUCHAR                      pSrcString,
        ULONG                       ulStrLen
    )
{
    PANSC_ASN1_OCTET_STRING         pNewAsn1OctetString = NULL;
    ULONG                           ulNewArraySize      = ulStrLen * sizeof(UCHAR);

    /*
     * We already reserve one byte (zero) at the end of the octet string. This byte is not counted
     * by the "length" field. Its existence allows to cast the octet string directly into an ASCII
     * string with a NULL terminator.
     */
    pNewAsn1OctetString = (PANSC_ASN1_OCTET_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_OCTET_STRING));

    if ( !pNewAsn1OctetString )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pNewAsn1OctetString->ArraySize = ulNewArraySize;
        pNewAsn1OctetString->Length    = ulStrLen;
        pNewAsn1OctetString->Octets    = (PUCHAR)AnscAllocateMemory(ulNewArraySize + 1);
    }

    if ( !pNewAsn1OctetString->Octets )
    {
        AnscFreeMemory(pNewAsn1OctetString);

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        if ( pSrcString && (ulNewArraySize > 0) )
        {
            AnscCopyMemory
                (
                    pNewAsn1OctetString->Octets,
                    pSrcString,
                    ulNewArraySize
                );
        }
    }

    return  (ANSC_HANDLE)pNewAsn1OctetString;
}


ANSC_HANDLE
AnscAsn1CloneOctetString
    (
        ANSC_HANDLE                 hSrcAsn1OctetString
    )
{
    PANSC_ASN1_OCTET_STRING         pSrcAsn1OctetString = (PANSC_ASN1_OCTET_STRING)hSrcAsn1OctetString;
    PANSC_ASN1_OCTET_STRING         pNewAsn1OctetString = NULL;

    if ( pSrcAsn1OctetString )
    {
        pNewAsn1OctetString =
            (PANSC_ASN1_OCTET_STRING)AnscAsn1AllocOctetString
                (
                    pSrcAsn1OctetString->Octets,
                    pSrcAsn1OctetString->Length
                );
    }
    else
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pNewAsn1OctetString;
}


int
AnscAsn1CompOctetStrings
    (
        ANSC_HANDLE                 hAsn1OctetString1,
        ANSC_HANDLE                 hAsn1OctetString2
    )
{
    PANSC_ASN1_OCTET_STRING         pAsn1OctetString1 = (PANSC_ASN1_OCTET_STRING)hAsn1OctetString1;
    PANSC_ASN1_OCTET_STRING         pAsn1OctetString2 = (PANSC_ASN1_OCTET_STRING)hAsn1OctetString2;
    ULONG                           ulMinStrLength    = AnscGetMin2(pAsn1OctetString1->Length, pAsn1OctetString2->Length);
    ULONG                           i                 = 0;

    for ( i = 0; i < ulMinStrLength; i++ )
    {
        if ( pAsn1OctetString1->Octets[i] == pAsn1OctetString2->Octets[i] )
        {
            continue;
        }
        else if ( pAsn1OctetString1->Octets[i] > pAsn1OctetString2->Octets[i] )
        {
            return  1;
        }
        else
        {
            return  -1;
        }
    }

    if ( pAsn1OctetString1->Length == pAsn1OctetString2->Length )
    {
        return  0;
    }
    else if ( pAsn1OctetString1->Length > pAsn1OctetString2->Length )
    {
        return  1;
    }
    else
    {
        return  -1;
    }

    return  0;
}


ANSC_HANDLE
AnscAsn1AllocOid
    (
        PULONG                      pSrcSequence,
        ULONG                       ulSeqLen
    )
{
    PANSC_ASN1_OID                  pNewAsn1Oid    = NULL;
    ULONG                           ulNewArraySize = ulSeqLen * sizeof(ULONG);

    pNewAsn1Oid = (PANSC_ASN1_OID)AnscAllocateMemory(sizeof(ANSC_ASN1_OID));

    if ( !pNewAsn1Oid )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pNewAsn1Oid->ArraySize   = ulNewArraySize;
        pNewAsn1Oid->Length      = ulSeqLen;
        pNewAsn1Oid->Identifiers = (ulNewArraySize > 0)? (PULONG)AnscAllocateMemory(ulNewArraySize) : NULL;
    }

    if ( !pNewAsn1Oid->Identifiers && (ulNewArraySize > 0) )
    {
        AnscFreeMemory(pNewAsn1Oid);

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        if ( pSrcSequence && (ulNewArraySize > 0) )
        {
            AnscCopyMemory
                (
                    pNewAsn1Oid->Identifiers,
                    pSrcSequence,
                    ulNewArraySize
                );
        }
    }

    return  (ANSC_HANDLE)pNewAsn1Oid;
}


ANSC_HANDLE
AnscAsn1ConvOid
    (
        char*                       pOidString
    )
{
    char*                           pSubOidString     = pOidString;
    ULONG                           ulStringSize      = AnscSizeOfString(pOidString);
    ULONG                           ulIdentifierCount = 0;
    PANSC_ASN1_OID                  pNewAsn1Oid       = NULL;
    ULONG                           i                 = 0;
    ULONG                           j                 = 0;
    char                            digitString[16];

    if ( ulStringSize == 0 )
    {
        return  (ANSC_HANDLE)AnscAsn1AllocOid(NULL, 0);
    }
    else if ( (pOidString[0] == '.') || (pOidString[ulStringSize - 1] == '.') )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        for ( i = 0; i < ulStringSize; i++ )
        {
            if ( pOidString[i] == '.' )
            {
                ulIdentifierCount++;
            }
        }

        ulIdentifierCount++;
    }

    pNewAsn1Oid = (PANSC_ASN1_OID)AnscAsn1AllocOid(NULL, ulIdentifierCount);

    if ( !pNewAsn1Oid )
    {
        return  (ANSC_HANDLE)NULL;
    }

    for ( i = 0; i < ulIdentifierCount; i++ )
    {
        j = 0;

        AnscZeroMemory(digitString, 16);

        while ( (*pSubOidString != '.') && (*pSubOidString != 0) && (j < 16) )
        {
            digitString[j] = *pSubOidString;

            pSubOidString++;
            j            ++;
        }

        pNewAsn1Oid->Identifiers[i] = AnscGetStringUlong(digitString);

        if ( *pSubOidString == '.' )
        {
            pSubOidString++;
        }
        else if ( i != (ulIdentifierCount - 1) )
        {
            break;
        }
    }

    return  (ANSC_HANDLE)pNewAsn1Oid;
}


ANSC_HANDLE
AnscAsn1CloneOid
    (
        ANSC_HANDLE                 hSrcAsn1Oid
    )
{
    PANSC_ASN1_OID                  pSrcAsn1Oid = (PANSC_ASN1_OID)hSrcAsn1Oid;
    PANSC_ASN1_OID                  pNewAsn1Oid = NULL;

    if ( pSrcAsn1Oid )
    {
        pNewAsn1Oid =
            (PANSC_ASN1_OID)AnscAsn1AllocOid
                (
                    pSrcAsn1Oid->Identifiers,
                    pSrcAsn1Oid->Length
                );
    }
    else
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pNewAsn1Oid;
}


BOOL
AnscAsn1ConcOids
    (
        ANSC_HANDLE                 hAsn1Oid1,
        ANSC_HANDLE                 hAsn1Oid2
    )
{
    PANSC_ASN1_OID                  pAsn1Oid1      = (PANSC_ASN1_OID)hAsn1Oid1;
    PANSC_ASN1_OID                  pAsn1Oid2      = (PANSC_ASN1_OID)hAsn1Oid2;
    ULONG                           ulMinSeqLength = AnscGetMin2(pAsn1Oid1->Length, pAsn1Oid2->Length);
    ULONG                           i              = 0;

    if ( pAsn1Oid1->Length < pAsn1Oid2->Length )
    {
        return  FALSE;
    }

    for ( i = 0; i < ulMinSeqLength; i++ )
    {
        if ( pAsn1Oid1->Identifiers[i] != pAsn1Oid2->Identifiers[i] )
        {
            break;
        }
    }

    if ( i >= ulMinSeqLength )
    {
        return  TRUE;
    }

    return  FALSE;
}


int
AnscAsn1CompOids
    (
        ANSC_HANDLE                 hAsn1Oid1,
        ANSC_HANDLE                 hAsn1Oid2
    )
{
    PANSC_ASN1_OID                  pAsn1Oid1      = (PANSC_ASN1_OID)hAsn1Oid1;
    PANSC_ASN1_OID                  pAsn1Oid2      = (PANSC_ASN1_OID)hAsn1Oid2;
    ULONG                           ulMinSeqLength = AnscGetMin2(pAsn1Oid1->Length, pAsn1Oid2->Length);
    ULONG                           i              = 0;

    for ( i = 0; i < ulMinSeqLength; i++ )
    {
        if ( pAsn1Oid1->Identifiers[i] == pAsn1Oid2->Identifiers[i] )
        {
            continue;
        }
        else if ( pAsn1Oid1->Identifiers[i] > pAsn1Oid2->Identifiers[i] )
        {
            return  1;
        }
        else
        {
            return  -1;
        }
    }

    if ( pAsn1Oid1->Length == pAsn1Oid2->Length )
    {
        return  0;
    }
    else if ( pAsn1Oid1->Length > pAsn1Oid2->Length )
    {
        return  1;
    }
    else
    {
        return  -1;
    }

    return  0;
}


ANSC_HANDLE
AnscAsn1CombOids
    (
        ANSC_HANDLE                 hAsn1Oid1,
        ANSC_HANDLE                 hAsn1Oid2
    )
{
    PANSC_ASN1_OID                  pAsn1Oid1      = (PANSC_ASN1_OID)hAsn1Oid1;
    PANSC_ASN1_OID                  pAsn1Oid2      = (PANSC_ASN1_OID)hAsn1Oid2;
    PANSC_ASN1_OID                  pAsn1OidN      = (PANSC_ASN1_OID)NULL;
    ULONG                           ulNewSeqLength = pAsn1Oid1->Length + pAsn1Oid2->Length;
    ULONG                           i              = 0;

    pAsn1OidN =
        (PANSC_ASN1_OID)AnscAsn1AllocOid
            (
                NULL,
                ulNewSeqLength
            );

    if ( !pAsn1OidN )
    {
        return  (ANSC_HANDLE)NULL;
    }

    for ( i = 0; i < pAsn1Oid1->Length; i++ )
    {
        pAsn1OidN->Identifiers[i] = pAsn1Oid1->Identifiers[i];
    }

    for ( i = 0; i < pAsn1Oid2->Length; i++ )
    {
        pAsn1OidN->Identifiers[i + pAsn1Oid1->Length] = pAsn1Oid2->Identifiers[i];
    }

    return  (ANSC_HANDLE)pAsn1OidN;
}


int
AnscAsn1FindOidInArray
    (
        ANSC_ASN1_OID*              asn1_oid,
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < array_size; i++ )
    {
        if ( AnscAsn1CompOids
                (
                    (ANSC_HANDLE)asn1_oid,
                    (ANSC_HANDLE)asn1_oid_array[i]
                ) == 0 )
        {
            break;
        }
    }

    if ( i >= array_size )
    {
        return  -1;
    }

    return  (int)i;
}


BOOL
AnscAsn1MarkOidInArray
    (
        ANSC_ASN1_OID*              asn1_oid,
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size,
        PULONG                      oid_index
    )
{
    BOOL                            oid_matched = FALSE;
    int                             comp_result = 0;
    ULONG                           i           = 0;

    for ( i = 0; i < array_size; i++ )
    {
        comp_result =
            AnscAsn1CompOids
                (
                    (ANSC_HANDLE)asn1_oid,
                    (ANSC_HANDLE)asn1_oid_array[i]
                );

        if ( comp_result > 0 )
        {
            continue;
        }
        else if ( comp_result == 0 )
        {
            oid_matched = TRUE;

            break;
        }
        else
        {
            oid_matched = FALSE;

            break;
        }
    }

    if ( i >= array_size )
    {
        oid_matched = FALSE;
    }

    *oid_index = i;

    return  oid_matched;
}


ANSC_STATUS
AnscAsn1SortOidArray
    (
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size
    )
{
    ANSC_ASN1_OID**                 temp_oid_array = (ANSC_ASN1_OID**)AnscAllocateMemory(sizeof(ANSC_ASN1_OID*) * array_size);
    ANSC_ASN1_OID*                  temp_oid       = (ANSC_ASN1_OID* )NULL;
    ULONG                           i              = 0;
    ULONG                           j              = 0;
    ULONG                           k              = 0;
    ULONG                           temp_size      = 0;

    if ( !temp_oid_array )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    for ( i = 0; i < array_size; i++ )
    {
        temp_oid = asn1_oid_array[i];

        for ( j = 0; j < temp_size; j++ )
        {
            if ( AnscAsn1CompOids
                    (
                        (ANSC_HANDLE)temp_oid,
                        (ANSC_HANDLE)temp_oid_array[j]
                    ) <= 0 )
            {
                break;
            }
        }

        if ( j >= temp_size )
        {
            temp_oid_array[temp_size++] = temp_oid;
        }
        else
        {
            for ( k = temp_size; k > j; k-- )
            {
                temp_oid_array[k] = temp_oid_array[k - 1];
            }

            temp_oid_array[j] = temp_oid;
            temp_size++;
        }
    }

    for ( i = 0; i < array_size; i++ )
    {
        asn1_oid_array[i] = temp_oid_array[i];
    }

    AnscFreeMemory(temp_oid_array);

    return  ANSC_STATUS_SUCCESS;
}


void
AnscAsn1FreeOidArray
    (
        ANSC_ASN1_OID**             asn1_oid_array,
        ULONG                       array_size
    )
{
    ULONG                           i = 0;

    for ( i = 0; i < array_size; i++ )
    {
        if ( asn1_oid_array[i] )
        {
            AnscAsn1FreeOid(asn1_oid_array[i]);

            asn1_oid_array[i] = NULL;
        }
    }

    AnscFreeMemory(asn1_oid_array);

    return;
}


ANSC_HANDLE
AnscAsn1AllocInt
    (
        PUCHAR                      pSrcString,
        ULONG                       ulStrLen,
        BOOL                        bNegative
    )
{
    PANSC_ASN1_INT                  pNewAsn1Int    = NULL;
    ULONG                           ulNewArraySize = ulStrLen * sizeof(UCHAR);

    pNewAsn1Int = (PANSC_ASN1_INT)AnscAllocateMemory(sizeof(ANSC_ASN1_INT));

    if ( !pNewAsn1Int )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pNewAsn1Int->bNegative = bNegative;
        pNewAsn1Int->ArraySize = ulNewArraySize;
        pNewAsn1Int->Length    = ulStrLen;
        pNewAsn1Int->Octets    = (ulNewArraySize > 0)? (PUCHAR)AnscAllocateMemory(ulNewArraySize) : NULL;
    }

    if ( !pNewAsn1Int->Octets && (ulNewArraySize > 0) )
    {
        AnscFreeMemory(pNewAsn1Int);

        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        if ( pSrcString && (ulNewArraySize > 0) )
        {
            AnscCopyMemory
                (
                    pNewAsn1Int->Octets,
                    pSrcString,
                    ulNewArraySize
                );
        }
    }

    return  (ANSC_HANDLE)pNewAsn1Int;
}


ANSC_HANDLE
AnscAsn1CloneInt
    (
        ANSC_HANDLE                 hSrcAsn1Int
    )
{
    PANSC_ASN1_INT                  pSrcAsn1Int = (PANSC_ASN1_INT)hSrcAsn1Int;
    PANSC_ASN1_INT                  pNewAsn1Int = NULL;

    if ( pSrcAsn1Int )
    {
        pNewAsn1Int =
            (PANSC_ASN1_INT)AnscAsn1AllocInt
                (
                    pSrcAsn1Int->Octets,
                    pSrcAsn1Int->Length,
                    pSrcAsn1Int->bNegative
                );
    }
    else
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pNewAsn1Int;
}


int
AnscAsn1CompInts
    (
        ANSC_HANDLE                 hAsn1Int1,
        ANSC_HANDLE                 hAsn1Int2
    )
{
    PANSC_ASN1_INT                  pAsn1Int1      = (PANSC_ASN1_INT)hAsn1Int1;
    PANSC_ASN1_INT                  pAsn1Int2      = (PANSC_ASN1_INT)hAsn1Int2;
    ULONG                           ulMinStrLength = AnscGetMin2(pAsn1Int1->Length, pAsn1Int2->Length);
    ULONG                           i              = 0;

    if ( pAsn1Int1->bNegative != pAsn1Int2->bNegative )
    {
        if ( pAsn1Int1->bNegative )
        {
            return  -1;
        }
        else
        {
            return  1;
        }
    }

    for ( i = 0; i < ulMinStrLength; i++ )
    {
        if ( pAsn1Int1->Octets[i] == pAsn1Int2->Octets[i] )
        {
            continue;
        }
        else if ( pAsn1Int1->Octets[i] > pAsn1Int2->Octets[i] )
        {
            if ( pAsn1Int1->bNegative )
            {
                return  -1;
            }
            else
            {
                return  1;
            }
        }
        else
        {
            if ( pAsn1Int1->bNegative )
            {
                return  1;
            }
            else
            {
                return  -1;
            }
        }
    }

    if ( pAsn1Int1->Length == pAsn1Int2->Length )
    {
        return  0;
    }
    else if ( pAsn1Int1->Length > pAsn1Int2->Length )
    {
        if ( pAsn1Int1->bNegative )
        {
            return  -1;
        }
        else
        {
            return  1;
        }
    }
    else
    {
        if ( pAsn1Int1->bNegative )
        {
            return  1;
        }
        else
        {
            return  -1;
        }
    }

    return  0;
}
