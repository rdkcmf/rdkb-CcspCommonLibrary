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

    module: ansc_asn1_obj_internal.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the internal functions for
        all the ASN.1 basic objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/2002  initial revision.
        03/19/2002  BitString is supported.
        03/28/2002  SETOF and SEQUENCEOF are supported.
        05/07/2002  A new api was added for choice object.
        05/08/2002  Another api SetSelectionByOID was added for Choice
        06/14/2002  PFN_ASN1_GET_VALUE_BUFFER was added for Integer  
                    and STRING objects.
        06/27/2002  GetFirstIntegerChild was added for SEQUENCE object
        06/27/2002  IsChoiceByInt was added fro CHOICE object
        08/02/2002  Add SetAndCreateSelection for CHOICE object;
        09/24/2002  An api to check equaling to OID value or not;
        11/14/2002  Add SetUlongValue for INTEGER object;
        
**********************************************************************/


#ifndef  _ANSC_ASN1_OBJ_INTERNAL_
#define  _ANSC_ASN1_OBJ_INTERNAL_


/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_ANY
          IMPLEMENTED IN ANSC_ASN1_ANY.C
********************************************************************/
ANSC_HANDLE
AnscAsn1AnyCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1AnyFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1AnyInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1AnyClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1AnyEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

BOOLEAN
AnscAsn1AnyCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

LONG
AnscAsn1AnyGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1AnyDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1AnyEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1AnyTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1AnyDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_INTEGER
          IMPLEMENTED IN ANSC_ASN1_INTEGER.C
********************************************************************/
ANSC_HANDLE
AnscAsn1IntegerCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1IntegerFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1IntegerInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1IntegerClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1IntegerCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1IntegerEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1IntegerGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1IntegerDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1IntegerEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1IntegerTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1IntegerDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

PUCHAR
AnscAsn1IntegerGetValueBuffer
    (
        ANSC_HANDLE                 hThisObject
    );

LONG
AnscAsn1IntegerGetIntegerValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1IntegerSetIntegerValue
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lValue
    );

ANSC_STATUS
AnscAsn1IntegerSetUlongValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uValue
    );

BOOLEAN
AnscAsn1IntegerIsBigInteger
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1IntegerSetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData,
        ULONG                       length
    );

ANSC_STATUS
AnscAsn1IntegerSetUnsignedStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData,
        ULONG                       length
    );

ANSC_STATUS
AnscAsn1IntegerGetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     ppData,
        PULONG                      pLength
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_BOOL
          IMPLEMENTED IN ANSC_ASN1_BOOL.C
********************************************************************/
ANSC_HANDLE
AnscAsn1BOOLCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1BOOLInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1BOOLClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1BOOLEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

BOOLEAN
AnscAsn1BOOLCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

LONG
AnscAsn1BOOLGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1BOOLDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1BOOLEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1BOOLTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1BOOLDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

BOOLEAN
AnscAsn1BOOLGetBooleanValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1BOOLSetBooleanValue
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bValue
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_REAL
          IMPLEMENTED IN ANSC_ASN1_REAL.C
********************************************************************/
ANSC_HANDLE
AnscAsn1RealCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1RealInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1RealClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1RealCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1RealEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1RealGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1RealDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1RealEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1RealTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1RealDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

DOUBLE
AnscAsn1RealGetRealValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1RealSetRealValue
    (
        ANSC_HANDLE                 hThisObject,
        DOUBLE                      dValue
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_ENUMERATE
          IMPLEMENTED IN ANSC_ASN1_ENUMERATE.C
********************************************************************/
ANSC_HANDLE
AnscAsn1EnumerateCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1EnumerateFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1EnumerateInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1EnumerateClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1EnumerateCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1EnumerateEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1EnumerateGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1EnumerateDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1EnumerateEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1EnumerateTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1EnumerateDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

LONG
AnscAsn1EnumerateGetIntegerValue
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1EnumerateSetIntegerValue
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lValue
    );


/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_NULL
          IMPLEMENTED IN ANSC_ASN1_NULL.C
********************************************************************/
ANSC_HANDLE
AnscAsn1NULLCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1NULLInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1NULLClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1NULLCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1NULLEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1NULLGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1NULLDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1NULLEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1NULLTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1NULLDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_TIME
          IMPLEMENTED IN ANSC_ASN1_TIME.C
********************************************************************/
ANSC_HANDLE
AnscAsn1TimeCreate
    (
        ANSC_HANDLE                 hReserved,
        ULONG                       uTimeType
    );

ANSC_HANDLE
AnscAsn1UniversalTimeCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1GeneralizedTimeCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1TimeFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1TimeInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1TimeClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1TimeCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1TimeEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1TimeGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1TimeDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1TimeEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1TimeTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1TimeDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

ANSC_STATUS
AnscAsn1TimeSetTimeValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    );

ANSC_STATUS
AnscAsn1TimeGetTimeValue
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pYear,
        PULONG                      pMonth,
        PULONG                      pDay,
        PULONG                      pHour,
        PULONG                      pMinute,
        PULONG                      pSecond
    );

ANSC_STATUS
AnscAsn1TimeSetUniversalTime
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_UNIVERSAL_TIME         uniTime
    );

BOOLEAN
AnscAsn1TimeIsBefore
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    );

BOOLEAN
AnscAsn1TimeIsAfter
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    );

BOOLEAN
AnscAsn1TimeReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );
    
/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_OID
          IMPLEMENTED IN ANSC_ASN1_OID.C
********************************************************************/
ANSC_HANDLE
AnscAsn1OIDCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1OIDFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1OIDInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1OIDClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1OIDCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1OIDEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

PULONG
AnscAsn1OIDGetOIDBuffer
    (
        ANSC_HANDLE                 hThisObject
    );

LONG
AnscAsn1OIDGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1OIDDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1OIDEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1OIDTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1OIDDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

ANSC_STATUS
AnscAsn1OIDGetOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pOIDArray,
        PULONG                      pSize
    );

ANSC_STATUS
AnscAsn1OIDSetOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pOIDUnitArray,
        ULONG                       uSize
    );

ANSC_STATUS
AnscAsn1OIDSetStringOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pStringValue
    );

BOOLEAN
AnscAsn1OIDGetStringOIDValue
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pStringValue
    );

BOOLEAN
AnscAsn1OIDReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1OIDEqualToOIDString
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

/*******************************************************************
          FUNCTIONS FOR ALL THE STRING OBJECTS
          IMPLEMENTED IN ANSC_ASN1_STRING.C
********************************************************************/
ANSC_HANDLE
AnscAsn1StringCreate
    (
        ANSC_HANDLE                 hReserved,
        ULONG                       uTimeType
    );

ANSC_HANDLE
AnscAsn1OctetStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1UTF8StringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1UniversalStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1TeletexStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1IA5StringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1PrintableStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1VideotexStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1VisibleStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1NumericStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1GeneralStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1GraphicStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1BMPStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_HANDLE
AnscAsn1MSStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1StringFree
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1StringInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1StringClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1StringCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1StringEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1StringGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1StringDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1StringEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1StringTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1StringDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

ANSC_STATUS
AnscAsn1StringSetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData,
        ULONG                       length
    );

ANSC_STATUS
AnscAsn1StringGetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     pData,
        PULONG                      pLength
    );

PUCHAR
AnscAsn1StringGetValueBuffer
    (
        ANSC_HANDLE                 hThisObject
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_BITSTRING
          IMPLEMENTED IN ANSC_ASN1_BITSTRING.C
********************************************************************/
ANSC_HANDLE
AnscAsn1BitStringCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1BitStringInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1BitStringClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1BitStringCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1BitStringEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1BitStringGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1BitStringDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1BitStringEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1BitStringDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

ULONG
AnscAsn1BitStringGetUnusedBit
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1BitStringSetUnusedBit
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uBits
    );

ANSC_STATUS
AnscAsn1BitStringSetBitStringValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uBits,
        PUCHAR                      pData,
        ULONG                       length
    );

BOOLEAN
AnscAsn1BitStringReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );


/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_CHOICE
          IMPLEMENTED IN ANSC_ASN1_CHOICE.C
********************************************************************/
ANSC_HANDLE
AnscAsn1ChoiceCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1ChoiceInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1ChoiceFree
    (
        ANSC_HANDLE                 hThisObject
    );
    

ANSC_HANDLE
AnscAsn1ChoiceClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1ChoiceCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1ChoiceEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1ChoiceGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1ChoiceDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1ChoiceEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1ChoiceTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1ChoiceDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );


ANSC_HANDLE
AnscAsn1ChoiceGetSelection
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscAsn1ChoiceGetSelectType
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1ChoiceSetSelection
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType,
        ANSC_HANDLE                 hSelection
    );

ANSC_STATUS
AnscAsn1ChoiceSetAndCreateSelection
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

ANSC_STATUS
AnscAsn1ChoiceSetSelectionByOID
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

BOOLEAN
AnscAsn1ChoiceIsChoiceByOID
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1ChoiceIsChoiceByInt
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1ChoiceCreateSelectionByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

LONG
AnscAsn1ChoiceGetChoiceByOID
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

BOOLEAN
AnscAsn1ChoiceGetChildTagValue
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uIndex,
        PASN_OBJECT_FORM_TYPE       pAttr,
        PULONG                      pTagValue
    );

BOOLEAN
AnscAsn1ChoiceReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );

PANSC_ATTR_OBJECT
AnscAsn1ChoiceCreateSelectionAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1ChoiceGetSelectionName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

PCHAR
AnscAsn1ChoiceGetOIDValueByMask
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_SET
          IMPLEMENTED IN ANSC_ASN1_SET.C
********************************************************************/
ANSC_HANDLE
AnscAsn1SetCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1SetInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SetFree
    (
        ANSC_HANDLE                 hThisObject
    );
    
ANSC_HANDLE
AnscAsn1SetClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SetCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1SetEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

ANSC_STATUS
AnscAsn1SetRemoveAllChildren
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bFreeMemory
    );

LONG
AnscAsn1SetGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SetDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1SetEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1SetTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1SetDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

BOOLEAN
AnscAsn1SetReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SetEnumChildObject
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ASN1_ENUM_CHILD_PROC    hProc,
        PVOID                       pContext
    );
void
AnscAsn1SetCreateAllChildren
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscAsn1SetGetChildCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SetGetChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_STATUS
AnscAsn1SetSetChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild,
        ULONG                       index,
        BOOLEAN                     bFreeOldOne
    );

/*
 *  Function must be implemented in the derived objects.
 */
PANSC_ATTR_OBJECT
AnscAsn1SetCreateChildAttr
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

PCHAR
AnscAsn1SetGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1SetCreateChildObject
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_SEQUENCE
          IMPLEMENTED IN ANSC_ASN1_SEQUENCE.C
********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1SequenceInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SequenceFree
    (
        ANSC_HANDLE                 hThisObject
    );
    

BOOLEAN
AnscAsn1SequenceTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1SequenceDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

ANSC_HANDLE
AnscAsn1SequenceClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SequenceCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

LONG
AnscAsn1SequenceGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SequenceDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1SequenceEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1SequenceReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SequenceGetExtraChild
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SequenceCreateExtraChild
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SequenceSetExtraChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSpecialChild,
        BOOLEAN                     bFreeOldOne
    );

ANSC_HANDLE
AnscAsn1SequenceGetFirstOIDChild
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SequenceGetLastOIDChild
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SequenceGetFirstIntegerChild
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscAsn1SequenceGetLastStringChild
    (
        ANSC_HANDLE                 hThisObject
    );

PCHAR
AnscAsn1SequenceGetExtraChildName
    (
        ANSC_HANDLE                 hThisObject
    );

/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_SETOF
          IMPLEMENTED IN ANSC_ASN1_SETOF.C
********************************************************************/
ANSC_HANDLE
AnscAsn1SetOfCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1SetOfInitialize
    (
        ANSC_HANDLE                 hThisObject
    );
   
ANSC_HANDLE
AnscAsn1SetOfClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SetOfCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1SetOfEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1SetOfGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SetOfDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1SetOfEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1SetOfTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1SetOfDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

BOOLEAN
AnscAsn1SetOfReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SetOfFree
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SetOfEnumChildObject
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ASN1_ENUM_CHILD_PROC    hProc,
        PVOID                       pContext
    );

PCHAR
AnscAsn1SetOfGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1SetOfGetChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ULONG
AnscAsn1SetOfGetChildCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SetOfRemoveAllChildren
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bFreeMemory
    );

ANSC_STATUS
AnscAsn1SetOfRemoveChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index,
        BOOLEAN                     bFreeMemory
    );

ANSC_STATUS
AnscAsn1SetOfAddChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

/*
 *  Function must be implemented in the derived objects.
 */
ANSC_STATUS
AnscAsn1SetOfIsChildValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

ANSC_HANDLE
AnscAsn1SetOfCreateChild
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bAddItIn
    );


/*******************************************************************
          FUNCTIONS FOR ANSC_ASN1_SEQUENCEOF
          IMPLEMENTED IN ANSC_ASN1_SEQUENCEOF.C
********************************************************************/
ANSC_HANDLE
AnscAsn1SequenceOfCreate
    (
        ANSC_HANDLE                 hReserved
    );

ANSC_STATUS
AnscAsn1SequenceOfInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SequenceOfFree
    (
        ANSC_HANDLE                 hThisObject
    );
    
ANSC_HANDLE
AnscAsn1SequenceOfClone
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SequenceOfCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

BOOLEAN
AnscAsn1SequenceOfEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

LONG
AnscAsn1SequenceOfGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SequenceOfDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

ANSC_STATUS
AnscAsn1SequenceOfEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

BOOLEAN
AnscAsn1SequenceOfTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

BOOLEAN
AnscAsn1SequenceOfDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

BOOLEAN
AnscAsn1SequenceOfReadyToEncode
    (
        ANSC_HANDLE                 hThisObject
    );

BOOLEAN
AnscAsn1SequenceOfEnumChildObject
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ASN1_ENUM_CHILD_PROC    hProc,
        PVOID                       pContext
    );

PCHAR
AnscAsn1SequenceOfGetChildName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ANSC_HANDLE
AnscAsn1SequenceOfGetChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

ULONG
AnscAsn1SequenceOfGetChildCount
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscAsn1SequenceOfRemoveAllChildren
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bFreeMemory
    );

ANSC_STATUS
AnscAsn1SequenceOfRemoveChildByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index,
        BOOLEAN                     bFreeMemory
    );

ANSC_STATUS
AnscAsn1SequenceOfAddChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

/*
 *  Function must be implemented in the derived objects.
 */
ANSC_STATUS
AnscAsn1SequenceOfIsChildValid
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

ANSC_HANDLE
AnscAsn1SequenceOfCreateChild
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bAddItIn
    );

#endif /*_ANSC_ASN1_OBJ_INTERNAL_*/
