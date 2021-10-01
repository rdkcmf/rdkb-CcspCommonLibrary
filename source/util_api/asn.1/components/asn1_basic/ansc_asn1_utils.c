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

    module: ansc_asn1_utils.c

        For Abstract Syntax Notation One (ASN.1)
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        Utility functions for ASN1 implementation

        *   IsObjectChoiceType
        *   IsObjectStringType
        *   IsObjectPrimitiveType
        *   IsObjectContextType
        *   IsObjectUserDefined
        *   GetVisibleStringFromBMPString
        *   GetBMPStringFromVisibleString
        *   GetSizeOfEncodedLength
        *   EncodeLength
        *   ASN1WriteUlong
        *   GetPureEncodedLength
        *   GetLengthFromBuffer
        *   GetIntegerValueFromBuffer
        *   LookingForInfiniteEnd
        *   TraceBinaryData
        *   TraceOIDValue
        *   ParsingOIDStringValue
        *   ExportOIDValueToString
        *   TraceBMPString
        *   TraceErrorMessage
        *   TraceTabs
        *   ASN1Type2String
        *   AnscGotoNextTag
        *   AnscGetTLVLength
        *   AnscCheckAsn1Validity

        *   ASN1WriteBinaryToFile
        *   ASN1LoadBinaryFromFile
        *   ASN1WriteASN1HandleToFile
        *   ASN1CreateTLV
        *   AnscAsn1GetMD5FingerPrint
        *   AnscAsn1GetSHA1FingerPrint

        *   PKIInitDHKeyParameter
        *   PKIInitDHKeyParameterQ
        *   PKIIsDHParameterAcceptable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.
        03/14/02    function added: LookingForInfiniteEnd()
        03/18/02    function added: ParsingOIDStringValue()
                    function added: TraceOIDValue()
                    function added: ExportOIDValueToString()
        03/19/02    function added: TraceBMPString()
        03/26/02    function added: TraceErrorMessage()
        03/28/02    function added: TraceTabs()
        04/05/02    function added: ASN1WriteUlong()
        04/14/02    funciton added: ASN1Type2String()
        07/28/02    function added: ASN1WriteBinaryToFile()
        08/15/02    function added: AnscGotoNextTag() (Ignore the TL)
        09/12/02    function added: ASN1LoadBinaryFromFile()
        09/12/02    function added: ASN1WriteASN1HandleToFile()
        10/02/02    function added: AnscGetTLVLength()
        11/12/02    add DH key implementation;
        12/13/02    add function to create TLV;
        03/28/03    add GetIntegerValueFromBuffer();
        12/20/03    add function AnscCheckAsn1Validity()

**********************************************************************/

#include "ansc_asn1_local.h"
#include "ansc_crypto_pub.h"
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#include "safec_lib_common.h"

/***********************************************************
        FUNCTIONS TO CHECK THE OBJECT TYPE
        IMPLEMENTED IN ANSC_ASN1_UTILS.C
***********************************************************/

BOOLEAN
IsObjectChoiceType
    (
        LONG                        uType
    )
{
    return (uType == ASN1_CHOICE_TYPE);
}

BOOLEAN
IsObjectStringType
    (
        LONG                        uType
    )
{
    if(  uType == ASN1_BITSTRING_TYPE               ||
         uType == ASN1_OCTETSTRING_TYPE             ||
         uType == ASN1_UTF8STRING_TYPE              ||
         uType == ASN1_NUMERICSTRING_TYPE           ||
         uType == ASN1_PRINTABLESTRING_TYPE         ||
         uType == ASN1_VIDEOTEXSTRING_TYPE          ||
         uType == ASN1_IA5STRING_TYPE               ||
         uType == ASN1_GRAPHICSTRING_TYPE           ||
         uType == ASN1_VISIBLESTRING_TYPE           ||
         uType == ASN1_GENERALSTRING_TYPE           ||
         uType == ASN1_UNIVERSALSTRING_TYPE         ||
         uType == ASN1_BMPSTRING_TYPE               ||
         uType == ASN1_MSSTRING_TYPE )
    {
        return TRUE;
    }

    return FALSE;
}

BOOLEAN
IsObjectPrimitiveType
    (
        LONG                        uType
    )
{
    return ( uType != ASN1_SEQUENCE_TYPE && uType != ASN1_SET_TYPE &&
             uType != ASN1_CHOICE_TYPE   && uType != ASN1_USER_DEFINED);
}


BOOLEAN
IsObjectContextType
    (
        LONG                        uType
    )
{
    return (uType == ASN1_SEQUENCE_TYPE || uType == ASN1_SET_TYPE);
}

BOOLEAN
IsObjectUserDefined
    (
        LONG                        uType
    )
{
    return (uType == ASN1_USER_DEFINED);
}

PCHAR
ASN1Type2String
    (
        LONG                        uType
    )
{
    switch( uType )
    {
        case  ASN1_BOOLEAN_TYPE:            return "BOOLEAN";
        case  ASN1_INTEGER_TYPE:            return "INTEGER";
        case  ASN1_BITSTRING_TYPE:          return "BITSTRING";
        case  ASN1_OCTETSTRING_TYPE:        return "OCTETSTRING";
        case  ASN1_NULL_TYPE:               return "NULL";
        case  ASN1_OID_TYPE:                return "OID";
        case  ASN1_OBJECTDESCRIPTOR_TYPE:   return "OBJECTDESCRIPTOR";
        case  ASN1_EXTERNAL_TYPE:           return "EXTERNAL";
        case  ASN1_REAL_TYPE:               return "REAL";
        case  ASN1_ENUMERATE_TYPE:          return "ENUMERATE";
        case  ASN1_UTF8STRING_TYPE:         return "UTF8STRING";
        case  ASN1_SEQUENCE_TYPE:           return "SEQUENCE(OF)";
        case  ASN1_SET_TYPE:                return "SET(OF)";
        case  ASN1_NUMERICSTRING_TYPE:      return "NUMERICSTRING";
        case  ASN1_PRINTABLESTRING_TYPE:    return "PRINTABLESTRING";
        case  ASN1_TELETEXSTRING_TYPE:      return "TELETEXSTRING";
        case  ASN1_VIDEOTEXSTRING_TYPE:     return "VIDEOTEXSTRING";
        case  ASN1_IA5STRING_TYPE:          return "IA5STRING";
        case  ASN1_UNIVERSALTIME_TYPE:      return "UNIVERSALTIME";
        case  ASN1_GENERALIZEDTIME_TYPE:    return "GENERALIZEDTIME";
        case  ASN1_GRAPHICSTRING_TYPE:      return "GRAPHICSTRING";
        case  ASN1_VISIBLESTRING_TYPE:      return "VISIBLESTRING";
        case  ASN1_GENERALSTRING_TYPE:      return "GENERALSTRING";
        case  ASN1_UNIVERSALSTRING_TYPE:    return "UNIVERSALSTRING";
        case  ASN1_BMPSTRING_TYPE:          return "BMPSTRING";
        case  ASN1_MSSTRING_TYPE:           return "MSSTRING";
        case  ASN1_CHOICE_TYPE:             return "CHOICE";
        case  ASN1_ANY_TYPE:
        default:
                                            return "ANY";
    }
}

BOOLEAN
GetVisibleStringFromBMPString
    (
        PUCHAR                      pUData,
        ULONG                       length,
        PCHAR                       pBuffer,
        PULONG                      pNewLength
    )
{
    ULONG                           i;

    if( !pUData || !pBuffer || length <= 0 || length  % 2 != 0)
    {
        return FALSE;
    }

    if( pNewLength )
    {
        if( *pNewLength < length/2)
        {
            return FALSE;
        }

        *pNewLength                 = length / 2;
    }

    /* copy the char at location 1,3,5,7... */
    for( i = 0; i < length/2 ; i ++)
    {
        pBuffer[ i ]                = (CHAR)pUData[ i * 2 + 1 ];
    }

    return TRUE;
}


BOOLEAN
GetBMPStringFromVisibleString
    (
        PCHAR                       pData,
        ULONG                       length,
        PUCHAR                      pBuffer,
        PULONG                      pNewLength
    )
{
    ULONG                           i;

    if( !pData || !pBuffer || length <= 0 || length  % 2 != 0)
    {
        return FALSE;
    }

    if( pNewLength )
    {
        if( *pNewLength < length * 2)
        {
            return FALSE;
        }

        *pNewLength                 = length * 2;
    }

    /* copy single char to "00" and char */
    for( i = 0; i < length; i ++ )
    {
        pBuffer[ i * 2 ]            = 0x00;
        pBuffer[ i * 2 + 1 ]        = (UCHAR)pData[ i ];
    }

    return TRUE;

}

ULONG
GetSizeOfEncodedLength
    (
        ULONG                       ulLength
    )
{

    /*
     * size of the encoded content
     */
    ULONG                           ulSizeOfEncodedData     = 1;
    ULONG                           ulTempLength            = ulLength;

    /*
    * temporary counters
    */
    ULONG                           counter                 = 0;


    /***********************************************************
    CALCULATE THE NUMBER OF OCTETS IN THE ENCODED DATA
    ***********************************************************/
     if( (LONG)ulLength < 0)
     {
        return ANSC_ASN1_NOT_READY_TO_ENCODE;
     }

    /*
     * please refer to the ITU-X.690 recommendation
     */
    if ( ulLength <= 0x7F )
    {
        ulSizeOfEncodedData = 1;
    }
    else
    {
        ulSizeOfEncodedData = 1;
        counter             = 1;
        ulTempLength        = ulLength;

        while ( ulTempLength > 0xFF )
        {
            ulTempLength = ulTempLength / (0xFF + 1);
            counter++;
        }

        ulSizeOfEncodedData += counter;
    }

    return  ulSizeOfEncodedData;
}

ANSC_STATUS
EncodeLength
    (
        PVOID*                      ppEncodedData,
        ULONG                       ulLength
    )
{

    /*
    * size of the encoded length content
    */
    ULONG                           ulTempLength          = ulLength;

    /*
    * shortcut pointer to a char array
    */
    PUCHAR                          pCharData             = (PUCHAR)*ppEncodedData;

    /*
    * temporary counters
    */
    ULONG                           counter               = 0;


    /***********************************************************
    APPLY ASN.1 DER/BER ENCODING RULES
    ***********************************************************/

    /*
    * please refer to the ITU-X.690 recommendation
    */
    if ( ulLength <= 0x7F )
    {
        *pCharData = (UCHAR)ulLength;
        pCharData++;
    }
    else
    {
        counter               = 1;
        ulTempLength          = ulLength;

        while ( ulTempLength > 0xFF )
        {
            ulTempLength = ulTempLength / ( 0xFF + 1);
            counter++;
        }


        *pCharData = (UCHAR)counter | 0x80;
        pCharData++;

        ASN1WriteUlong( pCharData, ulLength);
        pCharData += counter;
    }

    *ppEncodedData = (PVOID)pCharData;

    return  ANSC_STATUS_SUCCESS;

}

/**********************************************************************

    FUNCTION TO WRITE ULONG INTO BINARY DAT ( NO ZERO AHEAD)
    NO MATTER IT'S BIG ENDIAN OR SMALL ENDIAN

    ULONG
    ASN1WriteUlong
        (
            PUCHAR                      pData,
            ULONG                       length
        )

    This routine will write ULONG value to binary data.

    argument:   PUCHAR                  pData
                The output buffer. If it's NULL, return the length only.

                ULONG                   length
                The ulong value;

      return:   the length of copied value

**********************************************************************/
ULONG
ASN1WriteUlong
    (
        PUCHAR                      pData,
        ULONG                       length
    )
{
    CHAR                            pTmp[sizeof(ULONG) + 1] = { 0 };
    ULONG                           i                        = 0;
    ULONG                           counter;

    if( length == 0)
    {
        if(pData != NULL)
        {
            pData[0]  = 0x00;
        }

        return 1;
    }

    /*
     *  write to ulong buffer;
     */
    AnscWriteUlong(pTmp, AnscUlongFromHToN(length));

    /* RDKB-6200, CID-24417, out of bount access.
    ** Maximum value pTmp can access is p[0...sizeof(ULONG)].
    */
    while( (pTmp[ i ] == 0x00) && i < sizeof(ULONG)) 
    {
        i ++;
    }

    /*
     * ignore the first 00es
     */
    counter = sizeof(ULONG) - i;

    if(pData != NULL)
    {
        AnscCopyMemory(pData, pTmp + i, counter);
    }

    return counter;
}

#define MaxLengthOctet                              4

LONG
GetPureEncodedLength
    (
        ULONG                       TotalLength
    )
{
    ULONG                           i;
    ULONG                           encode;

    for ( i = 1; i < MaxLengthOctet ; i ++ )
    {
        encode = GetSizeOfEncodedLength(TotalLength - i);

        if(encode == i)
        {
            return  TotalLength - i;
        }
    }

    return -1;
}

ULONG
GetLengthFromBuffer
    (
        PUCHAR                      pBuffer,
        ULONG                       ulLength
    )
{
    UCHAR                           pMyBuffer[sizeof(ULONG)];

    if( !pBuffer || ulLength == 0)
    {
        return 0;
    }

    if( ulLength > sizeof(ULONG))
    {
        ulLength = sizeof(ULONG);
    }

    AnscZeroMemory(pMyBuffer, sizeof(ULONG));
    AnscCopyMemory(pMyBuffer + sizeof(ULONG) - ulLength, pBuffer, ulLength);

    return (ULONG)
           (ULONG)*((PUCHAR)pMyBuffer + 3) +
           (ULONG)*((PUCHAR)pMyBuffer + 2) * 0x100 +
           (ULONG)*((PUCHAR)pMyBuffer + 1) * 0x10000 +
           (ULONG)*((PUCHAR)pMyBuffer + 0) * 0x1000000;
}

LONG
GetIntegerValueFromBuffer
    (
        PUCHAR                      pBuffer,
        ULONG                       ulLength
    )
{
    UCHAR                           pMyBuffer[sizeof(ULONG)];
    ULONG                           uValue;
    LONG                            lValue;

    if( !pBuffer || ulLength == 0)
    {
        return 0;
    }

    if( pBuffer[0] < 0x80)
    {
        return GetLengthFromBuffer(pBuffer, ulLength);
    }

    /* it's a minus value*/

    pMyBuffer[0] = 0xFF;
    pMyBuffer[1] = 0xFF;
    pMyBuffer[2] = 0xFF;
    pMyBuffer[3] = 0xFF;

    AnscCopyMemory(pMyBuffer + sizeof(ULONG) - ulLength, pBuffer, ulLength);

    uValue = (ULONG)
             (ULONG)*((PUCHAR)pMyBuffer + 3) +
             (ULONG)*((PUCHAR)pMyBuffer + 2) * 0x100 +
             (ULONG)*((PUCHAR)pMyBuffer + 1) * 0x10000 +
             (ULONG)*((PUCHAR)pMyBuffer + 0) * 0x1000000;

    lValue = (LONG)(0xFFFFFFFF - uValue + 1) * (-1);

    return lValue;
}

/*
*  Try to find "0x00 0x00" for the infinite end;
*/
PUCHAR
LookingForInfiniteEnd
    (
        PUCHAR                      pBuffer,
        ULONG                       uMaxLength
    )
{
    ULONG                           index       = 0;
    PUCHAR                          pFound      = NULL;

    if( !pBuffer || uMaxLength < 2)
    {
        return pFound;
    }

    while( index < uMaxLength )
    {
        if( pBuffer[index] == 0x00 && pBuffer[index + 1] == 0x00)
        {
            pFound                  = pBuffer + index;

            break;
        }

        if( pBuffer[index + 1] == 0x00)
        {
            index ++;
        }
        else
        {
            index += 2;
        }
    }

    return pFound;
}


void
TraceBinaryData
    (
        PUCHAR                      pBuffer,
        ULONG                       uLength,
        ULONG                       uMaxiLength
    )
{
    ULONG                           i;

    if( !pBuffer || uLength == 0 || uMaxiLength == 0)
    {
        return;
    }

    if( uMaxiLength > uLength)
    {
        uMaxiLength = uLength;
    }

    AnscTrace("0x");

    for( i = 0; i < uMaxiLength; i ++ )
    {
        AnscTrace("%.2X", (UCHAR)pBuffer[i]);
    }

    if( uMaxiLength < uLength)
    {
        AnscTrace("...");
    }
}

void
TraceOIDValue
    (
        PULONG                      pArray,
        ULONG                       uLength
    )
{
    ULONG                           i;

    for( i = 0; i < uLength; i ++)
    {
        if( i != 0)
        {
            AnscTrace(".%lu", pArray[ i ]);
        }
        else
        {
            AnscTrace("%lu", pArray[ i ]);
        }
    }
}

/**********************************************************************

    prototype:

            BOOLEAN
            ParsingOIDStringValue
                (
                    PCHAR                       pInput,
                    PULONG                      pArray,
                    PULONG                      pLength
                )

    description:

        This function is parsing the given oid string value to an array.
        Valid string values as "1.3.8.3234.4" or "1 3 8 3234 4"

    argument:
                PCHAR                       pInput
                The input string value;

                PULONG                       pArray,
                The output ulong array;

                PULONG                      pLength
                The buffer of the array size;

    return:     succeded or not

**********************************************************************/
BOOLEAN
ParsingOIDStringValue
    (
        PCHAR                       pInput,
        PULONG                      pArray,
        PULONG                      pLength
    )
{
    ULONG                           length, i ,counter, lastChar;
    BOOLEAN                         bIsPreviousAChar, bInvalid;
    PCHAR                           pBack, pValue;

    if( pInput == NULL || pLength == NULL)
    {
        return FALSE;
    }

    length                          = AnscSizeOfString(pInput);

    if( length < 3)
    {
        return FALSE;
    }

    /*
     *  Copy the string;
     */
    pBack = strdup(pInput);
    if (! pBack) {
        return FALSE;
    }

    pBack[ length ]                 = 0;

    /*
     *  Analyse the value
     */
    bIsPreviousAChar                = TRUE;
    bInvalid                        = FALSE;

    for( i = 0; i < length; i ++)
    {
        if( bIsPreviousAChar)
        {
            if( pInput[i] < '0' || pInput[i] > '9')
            {
                bInvalid            = TRUE;
                break;
            }

            bIsPreviousAChar        = FALSE;
        }
        else
        {
            if( pInput[i] < '0' || pInput[i] > '9')
            {
                if( pInput[i] == ' ')
                {
                    pInput[i] = '.';
                }

                /* The last one cannot be '.' */
                if( pInput[i] != '.' || i == length - 1)
                {
                    bInvalid            = TRUE;
                    break;
                }
                bIsPreviousAChar        = TRUE;
            }
        }
    }

    /*
     *  check the value;
     */
    if(!bInvalid)
    {
        counter                         = 0;
        pValue                          = pBack;
        lastChar                        = 0;

        for( i = 0; i < length; i ++)
        {
           if( pBack[i] == '.')
           {
                if( pArray != NULL)
                {
                    pValue[ i - lastChar]   = 0;
                    pArray[counter]         = (ULONG)AnscString2Int(pValue);
                    pValue                  = pBack + i + 1;
                    lastChar                = i + 1;
                }

                counter ++;
           }
        }

        if( pArray != NULL)
        {
            pArray[counter]                 = (ULONG)AnscString2Int(pValue);
        }

        counter ++;

        *pLength                        = counter;
    }

    /*
     *  Free the memory;
     */
    AnscFreeMemory( pBack);

    return !bInvalid;
}

BOOLEAN
ExportOIDValueToString
    (
        PULONG                      pArray,
        ULONG                       ulength,
        PCHAR                       pBuffer
    )
{
    ULONG                           i;
    ULONG                           index           = 0;
    ULONG                           uValue;
    CHAR                            pTmpBuffer[16]  = { 0 };


    if( pArray == NULL || ulength == 0 || pBuffer == NULL)
    {
        return FALSE;
    }

    for( i = 0; i < ulength; i ++)
    {
        uValue                      = pArray[i];

        AnscZeroMemory( pTmpBuffer, 16);
        AnscGetUlongString
            (
                (ULONG)uValue,
                pTmpBuffer
            );

        AnscCopyMemory( pBuffer + index , pTmpBuffer, AnscSizeOfString(pTmpBuffer));

        index += AnscSizeOfString(pTmpBuffer);

        if( i < ulength - 1)
        {
            pBuffer[index]          = '.';
        }
        else
        {
            pBuffer[index]          = 0;
        }

        index                       ++;

    }

    return TRUE;
}

void
TraceBMPString
    (
        PUCHAR                      pBuffer,
        ULONG                       uLength
    )
{
    ULONG                           i;

    for( i = 0; i < uLength; i ++)
    {
       if( i >= MAXI_TRACE_BMP_LENGTH)
       {
            AnscTrace("...");
            break;
       }

       if( pBuffer[i] == 0x00)
       {
            AnscTrace("-");
       }
       else
       {
            AnscTrace("%c", pBuffer[i]);
       }
    }
}

/**********************************************************************
               GLOBAL ERROR CODE TO ERROR STRING TABLE
**********************************************************************/
typedef struct
_ERROR_CODE_TO_ERROR_STRING_ENTRY
{
    ULONG               ulErrorCode;
    /*
     *  the resource ID for the error string
     */
    PCHAR               pErrorString;
}
ERROR_CODE_TO_ERROR_STRING_ENTRY, *PERROR_CODE_TO_ERROR_STRING_ENTRY;

ERROR_CODE_TO_ERROR_STRING_ENTRY    gErrorCodeToStringTable[] =
{
    {ANSC_STATUS_SUCCESS,                           "ANSC_STATUS_SUCCESS"},
    {ANSC_ASN1_NULL_OBJCET,                         "ANSC_ASN1_NULL_OBJCET"},
    {ANSC_ASN1_BAD_PARAMETER,                       "ANSC_ASN1_BAD_PARAMETER"},
    {ANSC_ASN1_VALUE_NOT_SET,                       "ANSC_ASN1_VALUE_NOT_SET"},
    {ANSC_ASN1_BAD_OID_VALUE,                       "ANSC_ASN1_BAD_OID_VALUE"},
    {ANSC_ASN1_NULL_PARAMETER,                      "ANSC_ASN1_NULL_PARAMETER"},
    {ANSC_ASN1_NO_SELECTION_IN_CHOICE,              "ANSC_ASN1_NO_SELECTION_IN_CHOICE"},
    {ANSC_ASN1_UNKOWN_OBJECT_TYPE,                  "ANSC_ASN1_UNKOWN_OBJECT_TYPE"},
    {ANSC_ASN1_INVALID_UNUSED_BIT,                  "ANSC_ASN1_INVALID_UNUSED_BIT"},
    {ANSC_ASN1_INVALID_TYPE_IN_SEQOF_OR_SETOF,      "ANSC_ASN1_INVALID_TYPE_IN_SEQOF_OR_SETOF"},
    {ANSC_ASN1_ALREADY_EXIST,                       "ANSC_ASN1_ALREADY_EXIST"},
    {ANSC_ASN1_NOT_EXIST,                           "ANSC_ASN1_NOT_EXIST"},
    {ANSC_ASN1_FORBIDDEN_PLACE,                     "ANSC_ASN1_FORBIDDEN_PLACE"},
    {ANSC_ASN1_UNEXPECTED_TAG,                      "ANSC_ASN1_UNEXPECTED_TAG"},
    {ANSC_ASN1_INVALID_ENCODE_LENGTH,               "ANSC_ASN1_INVALID_ENCODE_LENGTH"},
    {ANSC_ASN1_INFINITE_END_MISSED,                 "ANSC_ASN1_INFINITE_END_MISSED"},

    {ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE,             "ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE"},
    {ANSC_ASN1_INVALID_TIME_VALUE,                  "ANSC_ASN1_INVALID_TIME_VALUE"},
    {ANSC_ASN1_NO_ENOUGH_SIZE,                      "ANSC_ASN1_NO_ENOUGH_SIZE"},
    {ANSC_ASN1_INVALID_VALUE,                       "ANSC_ASN1_INVALID_VALUE"},
    {ANSC_ASN1_NOT_READY_TO_ENCODE,                 "ANSC_ASN1_NOT_READY_TO_ENCODE"},
    {ANSC_ASN1_INVALID_SIGNATURE,                   "ANSC_ASN1_INVALID_SIGNATURE"},
    {ANSC_ASN1_NOT_SUPPORTED,                       "ANSC_ASN1_NOT_SUPPORTED"},
    {ANSC_ASN1_NO_RESOURCES,                        "ANSC_ASN1_NO_RESOURCES"},
    {ANSC_ASN1_INVALID_TIME,                        "ANSC_ASN1_INVALID_TIME"}
};

/**********************************************************************

    void
    TraceErrorMessage
        (
            ANSC_STATUS                         anscStatus
        )

    This routine will trace the error code.

    argument:   ANSC_STATUS                     anscStatus
                status code to get the error message for

      return:   nothing

**********************************************************************/

void
TraceErrorMessage
    (
        ANSC_STATUS                             anscStatus
    )
{
    ULONG                                       i,NumberItem;

    NumberItem =  sizeof(gErrorCodeToStringTable)/sizeof(gErrorCodeToStringTable[0]);

    for( i = 0; i < NumberItem; i++)
    {
        if(gErrorCodeToStringTable[i].ulErrorCode == anscStatus)
        {
            AnscTrace
                (
                    "status code  '0x%lX' = '%s'\n",
                    anscStatus,
                    gErrorCodeToStringTable[i].pErrorString
                );
        }
    }

}

/**********************************************************************

    void
    TraceTabs
        (
            ULONG                          number
        )

    This routine will trace the tabs.

    argument:   ULONG                          number
                the number of tabs.

      return:   nothing

**********************************************************************/
void
TraceTabs
    (
        ULONG                       number
    )
{
    ULONG                           i;

    for( i = 0; i < number; i ++)
    {
        AnscTrace("    ");
    }
}

/**************************************************
*
*  Some times the encoding data is not exactly what
*  is required.
*
*  This function is implemented to get the begining
*  of the next tag which means ignore the (TL) and goto
*  value directly
*
*  PUCHAR
*  AnscGotoNextTag
*      (
*          PUCHAR                      pEncoding, (IN)
*          ULONG                       length,    (IN)
*          PULONG                      pNewLength (OUT)
*      )
*
*  return: The place of the coming tag.
*          If no valid place, return NULL;
*
**************************************************/
PUCHAR
AnscGotoNextTag
    (
        PUCHAR                      pEncoding,
        ULONG                       length,
        PULONG                      pNewLength
    )
{
    PUCHAR                          pReturn      = NULL;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;

    if( pEncoding == NULL || pNewLength == NULL)
    {
        return NULL;
    }

    pTagInfo        = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

    if( pTagInfo == NULL)
    {
        AnscTrace("Failed to Create TagInfo in AnscGotoNextTag.\n");
        return NULL;
    }

    if( ANSC_STATUS_SUCCESS !=
            pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncoding))
    {
        return NULL;
    }

    if( pTagInfo->ulContentLength!= 0)
    {
        pReturn = (PUCHAR)
                    (
                        pEncoding +
                        pTagInfo->GetSizeOfTag(pTagInfo) +
                        pTagInfo->GetSizeOfLength(pTagInfo)
                    );

        if( pTagInfo->bIsInfinite)
        {
            if( length == 0)
            {
                return NULL;
            }

            *pNewLength =
                length - pTagInfo->GetSizeOfTag(pTagInfo) -
                pTagInfo->GetSizeOfLength(pTagInfo) - 2;
        }
        else
        {
            *pNewLength = pTagInfo->ulContentLength;
        }
    }

    pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

    return pReturn;
}

/**************************************************
*
*  If it's valid TLV encoding binary
*  return the length of the bianry.
*  Otherwise, return 0;
*
*  ULONG
*  AnscGetTLVLength
*      (
*          PUCHAR                      pEncoding
*       );
*
**************************************************/
ULONG
AnscGetTLVLength
    (
        PUCHAR                      pEncoding
    )
{
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    ULONG                           length       = 0;

    if( pEncoding == NULL)
    {
        return 0;
    }

    pTagInfo        = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

    if( pTagInfo == NULL)
    {
        return 0;
    }

    if( ANSC_STATUS_SUCCESS ==
            pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncoding))
    {
        if( !pTagInfo->bIsInfinite)
        {
            length =
                pTagInfo->GetSizeOfTag(pTagInfo) +
                pTagInfo->GetSizeOfLength(pTagInfo) +
                pTagInfo->ulContentLength;
        }
    }

    pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

    return length;
}

BOOL
AnscCheckAsn1Validity
    (
        PUCHAR                      pEncoding,
        ULONG                       uLength    
    )
{
    ULONG                           uLengthWanted;

    uLengthWanted = AnscGetTLVLength(pEncoding);

    if( uLengthWanted == 0)
    {
        /* indefinite mode */
        return TRUE;
    }

    return uLength >= uLengthWanted;
}

/**********************************************************************

    BOOLEAN
    ASN1WriteBinaryToFile
        (
            PCHAR                       pFileName,
            PUCHAR                      pEncoding,
            ULONG                       length
        )

    For debugging purpose, we can write the encoding data to file.

    argument:   PCHAR                   pFileName
                the  output file name;

                PUCHAR                      pEncoding,
                the binary encoded data;

                ULONG                       length
                the length of the binary;

      return:   succeeded or not;

**********************************************************************/
BOOLEAN
ASN1WriteBinaryToFile
    (
        PCHAR                       pFileName,
        PUCHAR                      pEncoding,
        ULONG                       length
    )
{
    ANSC_HANDLE                     pFileHandle         = NULL;

    if( pFileName == NULL || pEncoding == NULL || length == 0)
    {
        return FALSE;
    }

    if( AnscSizeOfString(pFileName) == 0)
    {
        return FALSE;
    }

    /* open file to write */
    pFileHandle =
        AnscOpenFile
            (
                pFileName,
                ANSC_FILE_MODE_CREATE | ANSC_FILE_MODE_WRITE | ANSC_FILE_MODE_TRUNC| ANSC_FILE_MODE_CREATE,
                ANSC_FILE_TYPE_READ | ANSC_FILE_TYPE_WRITE
            );

    if( pFileHandle == NULL || (LONG)pFileHandle == -1)
    {
        AnscTrace("Failed to load the file : '%s'\n", pFileName);

        return FALSE;
    }

    if( ANSC_STATUS_SUCCESS != AnscWriteFile( pFileHandle, pEncoding, &length))
    {
        AnscCloseFile(pFileHandle);/*RDKB-6200, CID-24383, free resource before return*/
        return FALSE;
    }

    AnscCloseFile(pFileHandle);

    return TRUE;
}

/************************************************************
*
* Load the binary string from the given file.
*
* The caller is responsible to release the returned memeory
*

    PUCHAR
    ASN1LoadBinaryFromFile
        (
            PCHAR                       pFileName,
            PULONG                      pLength
        )

    Load the binary from the file

    argument:   PCHAR                   pFileName
                the  input file name;

                PULONG                  pLength
                The buffer of returned length;

      return:   The binary returned.

*************************************************************/
PUCHAR
ASN1LoadBinaryFromFile
    (
        PCHAR                       pFileName,
        PULONG                      pLength
    )
{
    ANSC_HANDLE                     pFileHandle         = NULL;
    ULONG                           uFileLength, uBufferSize;
    PUCHAR                          pEncoding;

    if( pFileName == NULL)
    {
        return NULL;
    }

    /* load from the file */
    pFileHandle =
        AnscOpenFile
        (
            pFileName,
            ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
            ANSC_FILE_S_IREAD
        );

    if( pFileHandle == NULL)
    {
        AnscTrace("Failed to load the file : '%s'\n", pFileName);

        return NULL;
    }

    uFileLength = AnscGetFileSize( pFileHandle);

    pEncoding = AnscAllocateMemory( uFileLength + 8);

    if( pEncoding == NULL)
    {
        AnscCloseFile(pFileHandle);/*RDKB-6200, CID-24411, free resources after use */
        return NULL;
    }

    uBufferSize = uFileLength + 8;

    if( AnscReadFile( pFileHandle, pEncoding, &uBufferSize) != ANSC_STATUS_SUCCESS)
    {
        AnscFreeMemory(pEncoding);
        AnscCloseFile(pFileHandle);/*RDKB-6200, CID-24411, free resources after use*/
        return NULL;
    }

    if( pFileHandle != NULL)
    {
        AnscCloseFile(pFileHandle);
    }

    if( pLength != NULL)
    {
        *pLength = uFileLength;
    }

    return pEncoding;
}

/************************************************************

    BOOLEAN
    ASN1WriteASN1HandleToFile
        (
            PCHAR                       pFileName,
            ANSC_HANDLE                 hObject
        );

    Encode the ASN1 Object and write to file

************************************************************/
BOOLEAN
ASN1WriteASN1HandleToFile
    (
        PCHAR                       pFileName,
        ANSC_HANDLE                 hObject
    )
{
    PANSC_ASN1_OBJECT               pObject   = (PANSC_ASN1_OBJECT)hObject;
    PUCHAR                          pEncoding;
    ULONG                           length;
    BOOLEAN                         bResult;

    if( pFileName == NULL || pObject == NULL)
    {
        return FALSE;
    }

    pEncoding = pObject->GetEncodedData(pObject, &length);

    bResult =  ASN1WriteBinaryToFile( pFileName, pEncoding, length);

    if( pEncoding != NULL)
    {
        AnscFreeMemory(pEncoding);
    }

    return bResult;
}

/************************************************************

    PUCHAR
    ASN1CreateTLV
        (
            UCHAR                       uTag,
            ULONG                       length,
            PUCHAR                      pData,
            PULONG                      pNewLength
        );

    Generate the new asn.1 object with given tag and value,
    the caller is responsible to release the returned buffer if
    it's not empty.

    Parameters:
            UCHAR                       uTag,
            The tag value, such as "0x28";

            ULONG                       length,
            The length of the data;

            PUCHAR                      pData
            The value of the data;

            PULONG                      pNewLength
            The buffer of the new length;

    return:

            The new created TLV encoding;

************************************************************/
PUCHAR
ASN1CreateTLV
    (
        UCHAR                           uTag,
        ULONG                           length,
        PUCHAR                          pData,
        PULONG                          pNewLength
    )    
{
    ULONG                               newLength  = length;
    PUCHAR                              pNewBuffer = NULL;
    PUCHAR                              pBackBuffer;

    /* plus the tag length */
    newLength += 1;

    /* plus the size of encoding of the length */
    newLength += GetSizeOfEncodedLength(length);

    if( pNewLength != NULL)
    {
        *pNewLength = newLength;
    }

    /* create the new buffer */
    pNewBuffer = (PUCHAR)AnscAllocateMemory(newLength + 1);

    if( pNewBuffer == NULL)
    {
        return pNewBuffer;
    }

    pNewBuffer[0] = uTag;
    pBackBuffer   = pNewBuffer + 1;

    EncodeLength((PVOID*)&pBackBuffer, length);

    AnscCopyMemory
        (
            pBackBuffer,
            pData,
            length
        );

    return pNewBuffer;
}

/**********************************************************************

    caller:     owner of this object
                All the ASN1 object can share the same function here, you
                don't need to rewrite it, but you have to free the memory 
                cause it's new created.

    prototype:

        ANSC_STATUS
        AnscAsn1GetMD5FingerPrint
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHashObject
            );

    description:

        This function returns the MD5 finger printer of this object

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHashObject
                The hash object which is an output buffer

    return:     the state of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1GetMD5FingerPrint
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHashObject
    )
{
    PANSC_ASN1_OBJECT               pThisObject = (PANSC_ASN1_OBJECT)hThisObject;
    PANSC_CRYPTO_HASH               pHash       = (PANSC_CRYPTO_HASH)hHashObject;
    PANSC_CRYPTO_OBJECT             pCrypto     = NULL;
    PUCHAR                          pNewBuffer;
    ULONG                           length;
    ULONG                           ulHashSize;

    if( pThisObject == NULL || pHash == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pCrypto = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto(NULL);

    if (pCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pNewBuffer = pThisObject->GetEncodedData(pThisObject, &length);

    if( pNewBuffer == NULL || length == 0)
    {
        pCrypto->Remove(pCrypto);

        return ANSC_STATUS_FAILURE;
    }

    pHash->Length = ANSC_MD5_OUTPUT_SIZE;

    ulHashSize  = pCrypto->Md5Digest(pNewBuffer, length, pHash);

    pHash->Length = ulHashSize;

    if( pNewBuffer != NULL)
    {
        AnscFreeMemory(pNewBuffer);
    }

    if( pCrypto != NULL)
    {
        pCrypto->Remove(pCrypto);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object
                All the ASN1 object can share the same function here, you
                don't need to rewrite it, but you have to free the memory 
                cause it's new created.

    prototype:

        ANSC_STATUS
        AnscAsn1GetSHA1FingerPrint
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHashObject
            );

    description:

        This function returns the SHA1 finger printer of this object

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHashObject
                The hash object which is an output buffer

    return:     the state of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1GetSHA1FingerPrint
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHashObject
    )
{
    PANSC_ASN1_OBJECT               pThisObject = (PANSC_ASN1_OBJECT)hThisObject;
    PANSC_CRYPTO_HASH               pHash       = (PANSC_CRYPTO_HASH)hHashObject;
    PANSC_CRYPTO_OBJECT             pCrypto     = NULL;
    PUCHAR                          pNewBuffer;
    ULONG                           length;
    ULONG                           ulHashSize;

    if( pThisObject == NULL || pHash == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pCrypto = (PANSC_CRYPTO_OBJECT)AnscCreateCrypto(NULL);

    if (pCrypto == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    pNewBuffer = pThisObject->GetEncodedData(pThisObject, &length);

    if( pNewBuffer == NULL || length == 0)
    {
        pCrypto->Remove(pCrypto);

        return ANSC_STATUS_FAILURE;
    }

    pHash->Length = ANSC_SHA1_OUTPUT_SIZE;

    ulHashSize  = pCrypto->Sha1Digest(pNewBuffer, length, pHash);

    pHash->Length = ulHashSize;

    if( pNewBuffer != NULL)
    {
        AnscFreeMemory(pNewBuffer);
    }

    if( pCrypto != NULL)
    {
        pCrypto->Remove(pCrypto);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

  PKINIT states that DH parameters SHOULD be taken from the first or 
  second Oakley groups. Additionally, this specification requires that 
  DH groups are used exactly as defined in RFC2409.

  Because RFC2409 does not give values of q ( the p-1 factor) for the groups,
  and these are necessary in order to encode the dhPublicNumber type used 
  in the subjectPublicKeyInfo data structure in PKINIT.
    
 **********************************************************************/
/*******************************************************************************************************
                        Diffie-Hellman Oakley group 1 and 2
 ******************************************************************************************************/
 /* these values are in BIG-ENDIAN */

static const   UCHAR
OakleyGroup1_Prime[] =  /* 2^768 - 2^704 - 1 + 2 ^64 * {[2^638 pi] + 149686} */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
    0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
    0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
    0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
    0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x3A, 0x36, 0x20, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


static const  ULONG
OakleyGroup1_Generator = 2;

static const  UCHAR
OakleyGroup1_Factor[96] =    
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE4, 0x87, 0xED, 0x51, 0x10, 0xB4, 0x61, 0x1A,
    0x62, 0x63, 0x31, 0x45, 0xC0, 0x6E, 0x0E, 0x68, 0x94, 0x81, 0x27, 0x04, 0x45, 0x33, 0xE6, 0x3A,
    0x01, 0x05, 0xDF, 0x53, 0x1D, 0x89, 0xCD, 0x91, 0x28, 0xA5, 0x04, 0x3C, 0xC7, 0x1A, 0x02, 0x6E,
    0xF7, 0xCA, 0x8C, 0xD9, 0xE6, 0x9D, 0x21, 0x8D, 0x98, 0x15, 0x85, 0x36, 0xF9, 0x2F, 0x8A, 0x1B,
    0xA7, 0xF0, 0x9A, 0xB6, 0xB6, 0xA8, 0xE1, 0x22, 0xF2, 0x42, 0xDA, 0xBB, 0x31, 0x2F, 0x3F, 0x63,
    0x7A, 0x26, 0x21, 0x74, 0xD3, 0x1D, 0x1B, 0x10, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


/*
 * SECOND OAKLEY DEFAULT GROUP - MODP
 */
static const  UCHAR
OakleyGroup2_Prime[] =  /* 2^1024 - 2^960 - 1 + 2 ^64 * {[2^894 pi] + 129093} */
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
    0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
    0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
    0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
    0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
    0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6,
    0x49, 0x28, 0x66, 0x51, 0xEC, 0xE6, 0x53, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static const  ULONG
OakleyGroup2_Generator = 2;

static const  UCHAR
OakleyGroup2_Factor[128] =    
{
    /*0     1     2     3     4     5     6     7     8     9     A     B     C     D     E    F*/
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE4, 0x87, 0xED, 0x51, 0x10, 0xB4, 0x61, 0x1A,
    0x62, 0x63, 0x31, 0x45, 0xC0, 0x6E, 0x0E, 0x68, 0x94, 0x81, 0x27, 0x04, 0x45, 0x33, 0xE6, 0x3A,
    0x01, 0x05, 0xDF, 0x53, 0x1D, 0x89, 0xCD, 0x91, 0x28, 0xA5, 0x04, 0x3C, 0xC7, 0x1A, 0x02, 0x6E,
    0xF7, 0xCA, 0x8C, 0xD9, 0xE6, 0x9D, 0x21, 0x8D, 0x98, 0x15, 0x85, 0x36, 0xF9, 0x2F, 0x8A, 0x1B,
    0xA7, 0xF0, 0x9A, 0xB6, 0xB6, 0xA8, 0xE1, 0x22, 0xF2, 0x42, 0xDA, 0xBB, 0x31, 0x2F, 0x3F, 0x63,
    0x7A, 0x26, 0x21, 0x74, 0xD3, 0x1B, 0xF6, 0xB5, 0x85, 0xFF, 0xAE, 0x5B, 0x7A, 0x03, 0x5B, 0xF6, 
    0xF7, 0x1C, 0x35, 0xFD, 0xAD, 0x44, 0xCF, 0xD2, 0xD7, 0x4F, 0x92, 0x08, 0xBE, 0x25, 0x8F, 0xF3, 
    0x24, 0x94, 0x33, 0x28, 0xF6, 0x73, 0x29, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


/**********************************************************************

  Function:

    ANSC_STATUS
    PKIInitDHKeyParameter
        (
            ANSC_HANDLE                 hDHParameter,    (output)
            UINT                        length
        );

  Only 96 and 128 bytes are supported here. 
  If the length is invalid, 128 bytes will be used.

  *******************************************************************/

ANSC_STATUS
PKIInitDHKeyParameter
    (
        ANSC_HANDLE                 hDHParameter,  /* output */
        UINT                        length
    )
{
    PANSC_CRYPTO_DH_PARAMETERS      pDHKey     = (PANSC_CRYPTO_DH_PARAMETERS)hDHParameter;

    if( pDHKey == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( length == 96)
    {
        pDHKey->GroupPrime.Length  = sizeof(OakleyGroup1_Prime)/sizeof(OakleyGroup1_Prime[0]);
        AnscCopyMemory(pDHKey->GroupPrime.Data, (char *)OakleyGroup1_Prime, pDHKey->GroupPrime.Length);
        pDHKey->GroupGenerator.Length  = 1;
        pDHKey->GroupGenerator.Data.ucData[0] = (UCHAR)OakleyGroup1_Generator;
    }
    else
    {
        pDHKey->GroupPrime.Length  = sizeof(OakleyGroup2_Prime)/sizeof(OakleyGroup2_Prime[0]);
        AnscCopyMemory(pDHKey->GroupPrime.Data, (char *)OakleyGroup2_Prime, pDHKey->GroupPrime.Length);
        pDHKey->GroupGenerator.Length  = 1;
        pDHKey->GroupGenerator.Data.ucData[0] = (UCHAR)OakleyGroup2_Generator;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

  Function:

    ANSC_STATUS
    PKIInitDHKeyParameterQ
        (
            ANSC_HANDLE                 hDHParameter, ( input )
            PUCHAR                      pBufferQ,
            PULONG                      pLength
        );

  This function will check the input DHParameter. If it's the first and
  second Oakley group, it outputs the value of ParameterQ respectively;
  Otherwise, it returns status failure.

  *******************************************************************/
ANSC_STATUS
PKIInitDHKeyParameterQ
    (
        ANSC_HANDLE                 hDHParameter, /* input */
        PUCHAR                      pBufferQ,
        PULONG                      pLength
    )
{
    PANSC_CRYPTO_DH_PARAMETERS      pDHKey     = (PANSC_CRYPTO_DH_PARAMETERS)hDHParameter;
    ULONG                           length;

    if( pDHKey == NULL || pBufferQ == NULL || pLength == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    length   = pDHKey->GroupPrime.Length;

    if( *pLength < length)
    {
        return ANSC_STATUS_FAILURE;
    }

    *pLength = length;

    if( length == 96)
    {
        if(!AnscEqualMemory
                (
                    (PVOID)OakleyGroup1_Prime,
                    pDHKey->GroupPrime.Data,
                    length
                )
          )
        {
            AnscTrace("The 96 byte DH key parameter does not match with the first Oakly group.\n");

            return ANSC_STATUS_FAILURE;
        }

        AnscCopyMemory(pBufferQ, (char *)OakleyGroup1_Factor, length);
    }
    else if(length == 128)
    {
        if(!AnscEqualMemory
                (
                    (PVOID)OakleyGroup2_Prime,
                    pDHKey->GroupPrime.Data,
                    length
                )
          )
        {
            AnscTrace("The 128 byte DH key parameter does not match with the second Oakly group.\n");

            return ANSC_STATUS_FAILURE;
        }

        AnscCopyMemory(pBufferQ, (char *)OakleyGroup2_Factor, length);
    }
    else
    {
        AnscTrace("The %lu byte DH key is not supported yet.\n", length);

        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

  Function:

    BOOLEAN
    PKIIsDHParameterAcceptable
        (
            ANSC_HANDLE                 hDHParameter
        );

  This function will check the input DHParameter. If it's the first and
  second Oakley group, it returns TRUE;  otherwise returns FALSE.

  *******************************************************************/
BOOLEAN
PKIIsDHParameterAcceptable
    (
        ANSC_HANDLE                 hDHParameter
    )
{
    PANSC_CRYPTO_DH_PARAMETERS      pDHKey     = (PANSC_CRYPTO_DH_PARAMETERS)hDHParameter;
    ULONG                           length;

    if( pDHKey == NULL)
    {
        return FALSE;
    }

    length   = pDHKey->GroupPrime.Length;

    if( length == 96)
    {
        return
            AnscEqualMemory
                (
                    (PVOID)OakleyGroup1_Prime,
                    pDHKey->GroupPrime.Data,
                    length
                );
    }
    else if(length == 128)
    {
        return
            AnscEqualMemory
                (
                    (PVOID)OakleyGroup2_Prime,
                    pDHKey->GroupPrime.Data,
                    length
                );
    }

    return FALSE;
}

