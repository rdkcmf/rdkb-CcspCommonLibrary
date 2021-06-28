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

    MODULE:	_ANSC_ASN1_

        by Cisco Systems, Inc.

    ---------------------------------------------------------------

    FUNCTIONAL DESCRIPTION:

        This header file defines the materialized Abstract Syntax
        Notation One ASN.1 - ITU Recommendation X.208, and the
        corresponding DER encoding rules - ITU Recommendation
        X.690.

    ---------------------------------------------------------------

    ENVIRONMENT:

        Platform Independent.

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   09/05/02    initial revision

**********************************************************************/


#ifndef     _ANSC_ASN1_
#define     _ANSC_ASN1_


/***********************************************************
             DEFINITION OF ASN.1 COMMON OBJECTS
***********************************************************/

#define  MAXIMUM_BITSTRING_OCTETS                   64
#define  MAXIMUM_OCTETSTRING_OCTETS                 64
#define  MAXIMUM_OID_DEPTH                          16
#define  MAXIMUM_NUMERICSTRING_OCTETS               32
#define  MAXIMUM_PRINTABLESTRING_OCTETS             32
#define  MAXIMUM_TELETEXSTRING_OCTETS               32
#define  MAXIMUM_VIDEOTEXSTRING_OCTETS              32
#define  MAXIMUM_IA5STRING_OCTETS                   32
#define  MAXIMUM_GRAPHICSTRING_OCTETS               32
#define  MAXIMUM_VISIBLESTRING_OCTETS               32
#define  MAXIMUM_GENERALSTRING_OCTETS               32
#define  MAXIMUM_YEAR_OCTETS                        5
#define  MAXIMUM_MONTH_OCTETS                       3
#define  MAXIMUM_DAY_OCTETS                         3
#define  MAXIMUM_HOUR_OCTETS                        3
#define  MAXIMUM_MINUTE_OCTETS                      3
#define  MAXIMUM_SECOND_OCTETS                      3

/*
 * in most of situations, the contents of encoding should be less than the maximum size we permitted; however, we have
 * to be prepared
 */
typedef  struct
_ASN1_INTEGER
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many octets in this value buffer / value*/
    union
    {
        LONG        Value;
        ULONG       UnsignedValue;
        PVOID       ValueBuffer;                    /* pointer to the dynamic buffer which is holding the integer*/
    }IntValue;
}
ASN1_INTEGER,  *PASN1_INTEGER;

#define  BITSTRING_UNUSED_OCTET                     1

typedef  struct
_ASN1_BITSTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many octets in this bit string*/
    ULONG           UnusedBits;                     /* how many unused bits in the last octet*/
    union
    {
        UCHAR       String[MAXIMUM_BITSTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_BITSTRING,  *PASN1_BITSTRING;

typedef  struct
_ASN1_OCTETSTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this octet string*/
    union
    {
        UCHAR       String[MAXIMUM_OCTETSTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_OCTETSTRING,  *PASN1_OCTETSTRING;

typedef  ASN1_OCTETSTRING  ASN1_UTF8STRING,        *PASN1_UTF8STRING;
typedef  ASN1_OCTETSTRING  ASN1_BMPSTRING,         *PASN1_BMPSTRING;
typedef  ASN1_OCTETSTRING  ASN1_UNIVERSALSTRING,   *PASN1_UNIVERSALSTRING;

typedef  PVOID  ASN1_NULL,  *PASN1_NULL;

typedef  struct
_ASN1_OID
{
    ULONG           Length;                         /* how many sub OIDs in this OID definition*/
    ASN1_INTEGER    OIDChain[MAXIMUM_OID_DEPTH];
}
ASN1_OID,  *PASN1_OID;

typedef  struct
_ASN1_NUMERICSTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_NUMERICSTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_NUMERICSTRING,  *PASN1_NUMERICSTRING;

typedef  struct
_ASN1_PRINTABLESTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_PRINTABLESTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_PRINTABLESTRING,  *PASN1_PRINTABLESTRING;

typedef  struct
_ASN1_TELETEXSTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_TELETEXSTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_TELETEXSTRING,  *PASN1_TELETEXSTRING;

typedef  struct
_ASN1_VIDEOTEXSTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_VIDEOTEXSTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_VIDEOTEXSTRING,  *PASN1_VIDEOTEXSTRING;

typedef  struct
_ASN1_IA5STRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_IA5STRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_IA5STRING,  *PASN1_IA5STRING;

#define  MINIMUM_TIME_ENCODING_LENGTH               13

typedef  struct
_ASN1_UNIVERSALTIME
{
    ULONG           Year;
    UCHAR           YearString[MAXIMUM_YEAR_OCTETS];
    ULONG           Month;
    UCHAR           MonthString[MAXIMUM_MONTH_OCTETS];
    ULONG           Day;
    UCHAR           DayString[MAXIMUM_DAY_OCTETS];
    ULONG           Hour;
    UCHAR           HourString[MAXIMUM_HOUR_OCTETS];
    ULONG           Minute;
    UCHAR           MinuteString[MAXIMUM_MINUTE_OCTETS];
    ULONG           Second;
    UCHAR           SecondString[MAXIMUM_SECOND_OCTETS];
}
ASN1_UNIVERSALTIME,  *PASN1_UNIVERSALTIME;

typedef  struct
_ASN1_GENERALIZEDTIME
{
    ULONG           Year;
    UCHAR           YearString[MAXIMUM_YEAR_OCTETS];
    ULONG           Month;
    UCHAR           MonthString[MAXIMUM_MONTH_OCTETS];
    ULONG           Day;
    UCHAR           DayString[MAXIMUM_DAY_OCTETS];
    ULONG           Hour;
    UCHAR           HourString[MAXIMUM_HOUR_OCTETS];
    ULONG           Minute;
    UCHAR           MinuteString[MAXIMUM_MINUTE_OCTETS];
    ULONG           Second;
    UCHAR           SecondString[MAXIMUM_SECOND_OCTETS];
}
ASN1_GENERALIZEDTIME,  *PASN1_GENERALIZEDTIME;

typedef  struct
_ASN1_TIME
{
    BOOLEAN                     bUTC;
    union
    {
        ASN1_UNIVERSALTIME      UniversalTime;
        ASN1_GENERALIZEDTIME    GeneralizedTime;
    }Value;
}
ASN1_TIME,  *PASN1_TIME;

typedef  struct
_ASN1_GRAPHICSTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_GRAPHICSTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_GRAPHICSTRING,  *PASN1_GRAPHICSTRING;

typedef  struct
_ASN1_VISIBLESTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_VISIBLESTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_VISIBLESTRING,  *PASN1_VISIBLESTRING;

typedef  struct
_ASN1_GENERALSTRING
{
    BOOLEAN         bDynamicEncoding;
    ULONG           Length;                         /* how many bytes in this string*/
    union
    {
        UCHAR       String[MAXIMUM_GENERALSTRING_OCTETS];
        PVOID       StringBuffer;
    }Value;
}
ASN1_GENERALSTRING,  *PASN1_GENERALSTRING;

#endif /* _ANSC_ASN1_ */
