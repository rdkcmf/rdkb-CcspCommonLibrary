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

    module:	ansc_asn1_definitions.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some of the popular object and
        ASN.1 data structures used across ANSC platform.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/09/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_ASN1_DEFINITIONS_
#define  _ANSC_ASN1_DEFINITIONS_


/***********************************************************
        STANDARD ASN1 FUNDAMANTAL DATTA TYPE DEFINITION
***********************************************************/

/*
 * The "octet string" type represents an unbounded sequence of 8-bit bytes (octets) of binary
 * information.
 */
typedef  struct
_ANSC_ASN1_OCTET_STRING
{
    ULONG                           ArraySize;
    ULONG                           Length;
    PUCHAR                          Octets;
}
ANSC_ASN1_OCTET_STRING,  *PANSC_ASN1_OCTET_STRING;

#define  AnscAsn1EqualOctetStrings(octet_string1, octet_string2)                            \
         ( AnscAsn1CompOctetStrings(octet_string1, octet_string2) == 0 )

#define  AnscAsn1CleanOctetString(tbc_octet_string)                                         \
         {                                                                                  \
            if ( tbc_octet_string )                                                         \
            {                                                                               \
                if ( tbc_octet_string->Octets )                                             \
                {                                                                           \
                    AnscFreeMemory(tbc_octet_string->Octets);                               \
                }                                                                           \
                                                                                            \
                tbc_octet_string->ArraySize = 0;                                            \
                tbc_octet_string->Length    = 0;                                            \
                tbc_octet_string->Octets    = NULL;                                         \
            }                                                                               \
         }

#define  AnscAsn1FreeOctetString(tbf_octet_string)                                          \
         {                                                                                  \
            if ( tbf_octet_string )                                                         \
            {                                                                               \
                AnscAsn1CleanOctetString(tbf_octet_string);                                 \
                AnscFreeMemory          (tbf_octet_string);                                 \
            }                                                                               \
         }

/*
 * The "object identifier" type represents an unbounded sequence of non-negative integers of un-
 * bounded range. Values of this type are used to uniquely identify items for all space and time.
 */
typedef  struct
_ANSC_ASN1_OID
{
    ULONG                           ArraySize;
    ULONG                           Length;
    PULONG                          Identifiers;
}
ANSC_ASN1_OID,  *PANSC_ASN1_OID;

#define  AnscAsn1EqualOids(oid1, oid2)                                                      \
         ( AnscAsn1CompOids(oid1, oid2) == 0 )

#define  AnscAsn1CleanOid(tbc_oid)                                                          \
         {                                                                                  \
            if ( tbc_oid )                                                                  \
            {                                                                               \
                if ( tbc_oid->Identifiers )                                                 \
                {                                                                           \
                    AnscFreeMemory(tbc_oid->Identifiers);                                   \
                }                                                                           \
                                                                                            \
                tbc_oid->ArraySize   = 0;                                                   \
                tbc_oid->Length      = 0;                                                   \
                tbc_oid->Identifiers = NULL;                                                \
            }                                                                               \
         }

#define  AnscAsn1FreeOid(tbf_oid)                                                           \
         {                                                                                  \
            if ( tbf_oid )                                                                  \
            {                                                                               \
                AnscAsn1CleanOid(tbf_oid);                                                  \
                AnscFreeMemory  (tbf_oid);                                                  \
            }                                                                               \
         }

/*
 * The "integer" type represents any negative or non-negative value, no matter how large. In BER
 * encoding, integer values are presented by a twos-complement form of the value.
 */
typedef  struct
_ANSC_ASN1_INT
{
    BOOL                            bNegative;
    ULONG                           ArraySize;
    ULONG                           Length;
    PUCHAR                          Octets;
}
ANSC_ASN1_INT,  *PANSC_ASN1_INT;

#define  AnscAsn1EqualInts(asn1_int1, asn1_int2)                                            \
         ( AnscAsn1CompInts(asn1_int1, asn1_int2) == 0 )

#define  AnscAsn1CleanInt(tbc_int)                                                          \
         {                                                                                  \
            if ( tbc_int )                                                                  \
            {                                                                               \
                if ( tbc_int->Octets )                                                      \
                {                                                                           \
                    AnscFreeMemory(tbc_int->Octets);                                        \
                }                                                                           \
                                                                                            \
                tbc_int->bNegative = FALSE;                                                 \
                tbc_int->ArraySize = 0;                                                     \
                tbc_int->Length    = 0;                                                     \
                tbc_int->Octets    = NULL;                                                  \
            }                                                                               \
         }

#define  AnscAsn1FreeInt(tbf_int)                                                           \
         {                                                                                  \
            if ( tbf_int )                                                                  \
            {                                                                               \
                AnscAsn1CleanInt(tbf_int);                                                  \
                AnscFreeMemory  (tbf_int);                                                  \
            }                                                                               \
         }


#endif
