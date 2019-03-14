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

    module: ansc_asn1_base_interface.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the basic ASN.1 objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.
        04/24/02    AnscAsn1GetTagValue() is added as an API.
        05/23/02    AnscAsn1GetEncodedData() is added as an API.
        05/28/02    AnscAsn1GetMD5FingerPrint() is added as an API.
                    AnscAsn1GetSHA1FingerPrint() is added as an API.
        07/22/02    AnscAsn1GetBAOHandle() is added as an API.
                    BAO APIs are defined by Lina for Kerberoes implementation.

**********************************************************************/


#ifndef  _ANSC_ASN1_BASE_INTERFACE_
#define  _ANSC_ASN1_BASE_INTERFACE_

#include "ansc_asn1_attr_interface.h"

#ifndef AnscSprintfString
#define AnscSprintfString                           sprintf
#endif

#define ANSC_ASN1_OBJECT_NAME_SIZE                  48

/*
 * universal types defined in ASN.1 - they're intended to be encoded in 5 bits (BER, CER & DER)
 */

#define  ASN1_ANY_TYPE                              0x00
#define  ASN1_BOOLEAN_TYPE                          0x01
#define  ASN1_INTEGER_TYPE                          0x02
#define  ASN1_BITSTRING_TYPE                        0x03
#define  ASN1_OCTETSTRING_TYPE                      0x04
#define  ASN1_NULL_TYPE                             0x05
#define  ASN1_OID_TYPE                              0x06
#define  ASN1_OBJECTDESCRIPTOR_TYPE                 0x07
#define  ASN1_EXTERNAL_TYPE                         0x08
#define  ASN1_REAL_TYPE                             0x09
#define  ASN1_ENUMERATE_TYPE                        0x0A
#define  ASN1_UTF8STRING_TYPE                       0x0C
#define  ASN1_SEQUENCE_TYPE                         0x10
#define  ASN1_SET_TYPE                              0x11
#define  ASN1_NUMERICSTRING_TYPE                    0x12
#define  ASN1_PRINTABLESTRING_TYPE                  0x13
#define  ASN1_TELETEXSTRING_TYPE                    0x14
#define  ASN1_VIDEOTEXSTRING_TYPE                   0x15
#define  ASN1_IA5STRING_TYPE                        0x16
#define  ASN1_UNIVERSALTIME_TYPE                    0x17
#define  ASN1_GENERALIZEDTIME_TYPE                  0x18
#define  ASN1_GRAPHICSTRING_TYPE                    0x19
#define  ASN1_VISIBLESTRING_TYPE                    0x1A
#define  ASN1_GENERALSTRING_TYPE                    0x1B
#define  ASN1_UNIVERSALSTRING_TYPE                  0x1C
#define  ASN1_BMPSTRING_TYPE                        0x1E
#define  ASN1_MSSTRING_TYPE                         0x1D
#define  ASN1_CHOICE_TYPE                           0xFF
#define  ASN1_USER_DEFINED                          0xFE

/*
 * encoding forms used in BER encoding rules - primitive / constructed
 */
#define  PRIMITIVE_FORM                             0x00            /* 00000000B*/
#define  CONSTRUCTED_FORM                           0x20            /* 00100000B*/

/*
 * encoding bit masks used in BER encoding
 */
#define  TAG_MASK                                   0xC0            /* 11000000B*/
#define  FORM_MASK                                  0x20            /* 00100000B*/
#define  TYPE_MASK                                  0x1F            /* 00011111B*/

/***********************************************************
           GENERAL OBJECT FUNCTIONS DEFINITION
***********************************************************/
/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ASN1_CREATE)
    (
        ANSC_HANDLE                 hContainerContext
    );

typedef  ANSC_STATUS
(*PFN_ASN1_FREE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_INITIALIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_CLONE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ASN1_COPY_TO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    );

typedef  BOOLEAN
(*PFN_ASN1_EQUALS_TO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_GET_OWNER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_GET_BAOHANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_ASN1_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pName
    );

typedef  ULONG
(*PFN_ASN1_GET_OID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  void
(*PFN_ASN1_ADD_ATTRIBUTE)
    (
        ANSC_HANDLE                 hThisObject,
        PANSC_ATTR_OBJECT           pAttrObject,
        BOOLEAN                     bAppend
    );

typedef  ULONG
(*PFN_ASN1_GET_RID)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ASN1_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ASN1_IS_OPTIONAL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ASN1_IS_CONSTRUCTIVE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  VOID
(*PFN_ASN1_SET_OPTIONAL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bOptional
    );

typedef  VOID
(*PFN_ASN1_SET_OWNER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOwnerObject
    );

typedef  UCHAR
(*PFN_ASN1_GET_FIRST_OCTET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ASN1_READY_TO_ENCODE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  LONG
(*PFN_ASN1_GET_SIZE_OF_ENCODED)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_DECODING_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

typedef  ANSC_STATUS
(*PFN_ASN1_ENCODING_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

typedef  PUCHAR
(*PFN_ASN1_GET_ENCODED_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pLength
    );

typedef  ANSC_STATUS
(*PFN_ASN1_GET_FINGERPRINT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 pHashObject
    );


typedef BOOLEAN
(*PFN_ASN1_DUMP_OBJECT)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    );

typedef BOOLEAN
(*PFN_ASN1_TRACE_OBJECT)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    );

typedef  ANSC_STATUS
(*PFN_ASN1_ACTION_FUNCTION)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    );

typedef  void
(*PFN_ASN1_GET_TAG_VALUE) 
    (
        ANSC_HANDLE                 hThisObject,
        PASN_OBJECT_FORM_TYPE       pAttr,
        PULONG                      pTagValue
    );

/*
 * Define some const values that will be used in the container object definition.
 */
#define  ANSC_ASN1_OBJECT_NAME                      "ANSC_ASN1_OBJECT"
#define  ANSC_ASN1_OBJECT_ID                        0xFFFF6000

/*
 * The ASN.1 Objects are the extensions to the basic container. Any software module that is
 * shared between two or more extension objects or controller objects shall be implemented as a
 * Container Object. The container itself actually can leverage some of functionalities provided
 * by the Container Objects.
 */
#define  ANSC_ASN1_CLASS_CONTENT                                                            \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    SLIST_HEADER                    sAttrList;                                              \
    ANSC_HANDLE                     hContainerContext;                                      \
    ANSC_HANDLE                     hOwnerContext;                                          \
    PCHAR                           Name;                                                   \
    PCHAR                           ClassName;                                              \
    ULONG                           uBasicSize;                                             \
    ULONG                           Oid;                                                    \
    ULONG                           uType;                                                  \
    BOOLEAN                         bOptional;                                              \
    BOOLEAN                         bCanBeOptional;                                         \
                                                                                            \
    PFN_ASN1_CREATE                 Create;                                                 \
    PFN_ASN1_FREE                   AsnFree;                                                \
                                                                                            \
    PFN_ASN1_SET_NAME               SetName;                                                \
    PFN_ASN1_SET_NAME               SetClassName;                                           \
    PFN_ASN1_READY_TO_ENCODE        ReadyToEncode;                                          \
    PFN_ASN1_GET_FIRST_OCTET        GetFirstOctet;                                          \
    PFN_ASN1_IS_CONSTRUCTIVE        IsConstructive;                                         \
    PFN_ASN1_ACTION_FUNCTION        BeforeDecoding;                                         \
    PFN_ASN1_ACTION_FUNCTION        AfterDecoding;                                          \
    PFN_ASN1_ACTION_FUNCTION        BeforeEncoding;                                         \
    PFN_ASN1_ACTION_FUNCTION        AfterEncoding;                                          \
    PFN_ASN1_ADD_ATTRIBUTE          AddAttribute;                                           \
    PFN_ASN1_GET_TAG_VALUE          GetEncodeTagValue;                                      \
    PFN_ASN1_DUMP_OBJECT            DumpObject;                                             \
                                                                                            \
    PFN_ASN1_CLONE                  Clone;                                                  \
    PFN_ASN1_COPY_TO                CopyTo;                                                 \
    PFN_ASN1_EQUALS_TO              EqualsTo;                                               \
    PFN_ASN1_GET_SIZE_OF_ENCODED    GetSizeOfEncoded;                                       \
    PFN_ASN1_DECODING_DATA          DecodingData;                                           \
    PFN_ASN1_ENCODING_DATA          EncodingData;                                           \
    PFN_ASN1_GET_ENCODED_DATA       GetEncodedData;                                         \
    PFN_ASN1_GET_BAOHANDLE          GetBAOHandle;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_OBJECT
{
    ANSC_ASN1_CLASS_CONTENT
}
ANSC_ASN1_OBJECT,  *PANSC_ASN1_OBJECT;

#define  ACCESS_ANSC_ASN1_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_ASN1_OBJECT, Linkage)

#endif /*_ANSC_ASN1_BASE_INTERFACE_*/
