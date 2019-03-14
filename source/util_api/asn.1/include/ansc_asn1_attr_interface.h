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

    module: ansc_asn1_attr_interface.h

        For Abstract Syntax Notation One (ASN.1)
        of Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for the
        attribute of an ASN.1 object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/21/02    initial revision.

**********************************************************************/


#ifndef  _ANSC_ASN1_ATTR_INTERFACE_
#define  _ANSC_ASN1_ATTR_INTERFACE_

typedef  enum
_ASN_OBJECT_FORM_TYPE
{
    EMPTY_FORM                                      = 0,
    UNIVERSAL_FORM,
    APPLICATION_FORM,
    CONTEXT_FORM,
    PRIVATE_FORM
}
ASN_OBJECT_FORM_TYPE, *PASN_OBJECT_FORM_TYPE;

#define  IMPLICIT_TYPE                              TRUE       
#define  EXPLICIT_TYPE                              FALSE

/***********************************************************
           GENERAL OBJECT FUNCTIONS DEFINITION
***********************************************************/
/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_ATTR_CREATE)
    (
        ASN_OBJECT_FORM_TYPE        Type,
        ULONG                       ulNumber,
        BOOLEAN                     bImplicit
    );

typedef  ANSC_STATUS
(*PFN_ATTR_FREE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ASN_OBJECT_FORM_TYPE
(*PFN_ATTR_GET_FORM_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_ATTR_GET_CLASS_NUMBER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ATTR_SET_OPTIONAL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bOptional
    );

typedef  BOOLEAN
(*PFN_ATTR_SET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulLength
    );

typedef  ULONG
(*PFN_ATTR_GET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ATTR_CHECK_STATE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ATTR_CLONE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ATTR_EQUALS_TO)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject
    );

typedef  ANSC_STATUS
(*PFN_ATTR_COPY_VALUE_FROM)
    (
        ANSC_HANDLE                 hDestObject,
        ANSC_HANDLE                 hSourceObject
    );

typedef  ANSC_STATUS
(*PFN_ATTR_ENCODING_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding,
        BOOLEAN                     bIsConstructive
    );

typedef  ANSC_STATUS
(*PFN_ATTR_TRACE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef BOOLEAN
(*PFN_ATTR_DUMP)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength
    );

/*
 * The Attribute object
 */
#define  ANSC_ATTR_CLASS_CONTENT                                                            \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    ASN_OBJECT_FORM_TYPE            FormType;                                               \
    ULONG                           ulClassNumber;                                          \
    ULONG                           ulFixedLen;                                             \
    ULONG                           ulMaxiLen;                                              \
    BOOLEAN                         bImplicit;                                              \
                                                                                            \
    PFN_ATTR_FREE                   Remove;                                                 \
                                                                                            \
    PFN_ATTR_CLONE                  Clone;                                                  \
    PFN_ATTR_EQUALS_TO              EqualsTo;                                               \
    PFN_ATTR_GET_LENGTH             GetSizeOfEncodedTag;                                    \
    PFN_ATTR_COPY_VALUE_FROM        CopyVlaueFrom;                                          \
    PFN_ATTR_ENCODING_DATA          EncodingData;                                           \
    PFN_ATTR_DUMP                   Dump;                                                   \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ATTR_OBJECT
{
    ANSC_ATTR_CLASS_CONTENT
}
ANSC_ATTR_OBJECT,  *PANSC_ATTR_OBJECT;

#define  ACCESS_ANSC_ATTR_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_ATTR_OBJECT, Linkage)

#endif /*_ANSC_ASN1_ATTR_INTERFACE_*/
