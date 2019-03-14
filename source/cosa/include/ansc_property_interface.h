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

    module:	ansc_property_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to derived property class
        definitions.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/19/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PROPERTY_INTERFACE_
#define  _ANSC_PROPERTY_INTERFACE_


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  int
(*PFN_PINTO_GET_INT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PINTO_SET_INT)
    (
        ANSC_HANDLE                 hThisObject,
        int                         value
    );

typedef  ULONG
(*PFN_PINTO_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PINTO_SET_ULONG)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       value
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_INTEGER_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    union                                                                                   \
    {                                                                                       \
        int                         ValueInt;                                               \
        ULONG                       ValueUlong;                                             \
    }PropertyValue;                                                                         \
                                                                                            \
    PFN_PINTO_GET_INT               GetValueInt;                                            \
    PFN_PINTO_SET_INT               SetValueInt;                                            \
    PFN_PINTO_GET_ULONG             GetValueUlong;                                          \
    PFN_PINTO_SET_ULONG             SetValueUlong;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_INTEGER_OBJECT
{
    ANSC_PROPERTY_INTEGER_CLASS_CONTENT
}
ANSC_PROPERTY_INTEGER_OBJECT,  *PANSC_PROPERTY_INTEGER_OBJECT;

#define  ACCESS_ANSC_PROPERTY_INTEGER_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_INTEGER_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BOOL
(*PFN_PBOOLO_GET_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PBOOLO_SET_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        value
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_BOOLEAN_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    BOOL                            Value;                                                  \
                                                                                            \
    PFN_PBOOLO_GET_VALUE            GetValue;                                               \
    PFN_PBOOLO_SET_VALUE            SetValue;                                               \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_BOOLEAN_OBJECT
{
    ANSC_PROPERTY_BOOLEAN_CLASS_CONTENT
}
ANSC_PROPERTY_BOOLEAN_OBJECT,  *PANSC_PROPERTY_BOOLEAN_OBJECT;

#define  ACCESS_ANSC_PROPERTY_BOOLEAN_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_BOOLEAN_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  char*
(*PFN_PSTRO_GET_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSTRO_SET_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       value
    );

typedef  ULONG
(*PFN_PSTRO_GET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSTRO_SET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       length
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_STRING_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    char*                           Value;                                                  \
    ULONG                           BufferSize;                                             \
                                                                                            \
    PFN_PSTRO_GET_VALUE             GetValue;                                               \
    PFN_PSTRO_SET_VALUE             SetValue;                                               \
    PFN_PSTRO_GET_LENGTH            GetLength;                                              \
    PFN_PSTRO_SET_LENGTH            SetLength;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_STRING_OBJECT
{
    ANSC_PROPERTY_STRING_CLASS_CONTENT
}
ANSC_PROPERTY_STRING_OBJECT,  *PANSC_PROPERTY_STRING_OBJECT;

#define  ACCESS_ANSC_PROPERTY_STRING_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_STRING_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */
#define  ANSC_PROPERTY_TOKENS_COMMA_SEPARATORS      ",;"
#define  ANSC_PROPERTY_TOKENS_SLASH_SEPARATORS      "/"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  char*
(*PFN_PTKNO_GET_SEPARATORS)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PTKNO_SET_SEPARATORS)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       separators
    );

typedef  ULONG
(*PFN_PTKNO_GET_TOKEN_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_PTKNO_GET_TOKENS_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PTKNO_GET_TOKENS_TEXT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       text
    );

typedef  ANSC_STATUS
(*PFN_PTKNO_SET_TOKENS_TEXT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       text
    );

typedef  ANSC_HANDLE
(*PFN_PTKNO_POP_TOKEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PTKNO_PSH_TOKEN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hToken
    );

typedef  ANSC_STATUS
(*PFN_PTKNO_ADD_TOKEN)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       token
    );

typedef  ANSC_HANDLE
(*PFN_PTKNO_GET_HEAD_TOKEN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_PTKNO_GET_NEXT_TOKEN)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hThisToken
    );

typedef  ANSC_HANDLE
(*PFN_PTKNO_GET_LAST_TOKEN)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_TOKENS_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    char                            Separators[ANSC_OBJECT_NAME_SIZE];                      \
    ANSC_TOKEN_CHAIN                TokenChain;                                             \
                                                                                            \
    PFN_PTKNO_GET_SEPARATORS        GetSeparators;                                          \
    PFN_PTKNO_SET_SEPARATORS        SetSeparators;                                          \
                                                                                            \
    PFN_PTKNO_GET_TOKEN_COUNT       GetTokenCount;                                          \
    PFN_PTKNO_GET_TOKENS_SIZE       GetTokensSize;                                          \
    PFN_PTKNO_GET_TOKENS_TEXT       GetTokensText;                                          \
    PFN_PTKNO_SET_TOKENS_TEXT       SetTokensText;                                          \
                                                                                            \
    PFN_PTKNO_POP_TOKEN             PopToken;                                               \
    PFN_PTKNO_PSH_TOKEN             PshToken;                                               \
    PFN_PTKNO_ADD_TOKEN             AddToken;                                               \
                                                                                            \
    PFN_PTKNO_GET_HEAD_TOKEN        GetHeadToken;                                           \
    PFN_PTKNO_GET_NEXT_TOKEN        GetNextToken;                                           \
    PFN_PTKNO_GET_LAST_TOKEN        GetLastToken;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_TOKENS_OBJECT
{
    ANSC_PROPERTY_TOKENS_CLASS_CONTENT
}
ANSC_PROPERTY_TOKENS_OBJECT,  *PANSC_PROPERTY_TOKENS_OBJECT;

#define  ACCESS_ANSC_PROPERTY_TOKENS_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_TOKENS_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  PUCHAR
(*PFN_PBINO_GET_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PBINO_SET_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      value,
        ULONG                       length
    );

typedef  ULONG
(*PFN_PBINO_GET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PBINO_SET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       length
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_BINARY_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    PUCHAR                          Value;                                                  \
    ULONG                           BufferSize;                                             \
    ULONG                           Length;                                                 \
                                                                                            \
    PFN_PBINO_GET_VALUE             GetValue;                                               \
    PFN_PBINO_SET_VALUE             SetValue;                                               \
    PFN_PBINO_GET_LENGTH            GetLength;                                              \
    PFN_PBINO_SET_LENGTH            SetLength;                                              \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_BINARY_OBJECT
{
    ANSC_PROPERTY_BINARY_CLASS_CONTENT
}
ANSC_PROPERTY_BINARY_OBJECT,  *PANSC_PROPERTY_BINARY_OBJECT;

#define  ACCESS_ANSC_PROPERTY_BINARY_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_BINARY_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  PUCHAR
(*PFN_PIPADDRO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PIPADDRO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_IPADDR_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    union                                                                                   \
    {                                                                                       \
        UCHAR                       Dot[IP_ADDRESS_LENGTH];                                 \
        ULONG                       Value;                                                  \
    }Address;                                                                               \
                                                                                            \
    PFN_PIPADDRO_GET_ADDR           GetAddress;                                             \
    PFN_PIPADDRO_SET_ADDR           SetAddress;                                             \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_IPADDR_OBJECT
{
    ANSC_PROPERTY_IPADDR_CLASS_CONTENT
}
ANSC_PROPERTY_IPADDR_OBJECT,  *PANSC_PROPERTY_IPADDR_OBJECT;

#define  ACCESS_ANSC_PROPERTY_IPADDR_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_IPADDR_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */
#define  ANSC_PROPERTY_SUBNET_ADDR_NAME             "address"
#define  ANSC_PROPERTY_SUBNET_MASK_NAME             "mask"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  PUCHAR
(*PFN_PSUBNETO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSUBNETO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

typedef  PUCHAR
(*PFN_PSUBNETO_GET_MASK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PSUBNETO_SET_MASK)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      mask
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_SUBNET_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    union                                                                                   \
    {                                                                                       \
        UCHAR                       Dot[IP_ADDRESS_LENGTH];                                 \
        ULONG                       Value;                                                  \
    }Address;                                                                               \
    union                                                                                   \
    {                                                                                       \
        UCHAR                       Dot[IP_ADDRESS_LENGTH];                                 \
        ULONG                       Value;                                                  \
    }Mask;                                                                                  \
                                                                                            \
    PFN_PSUBNETO_GET_ADDR           GetAddress;                                             \
    PFN_PSUBNETO_SET_ADDR           SetAddress;                                             \
    PFN_PSUBNETO_GET_MASK           GetMask;                                                \
    PFN_PSUBNETO_SET_MASK           SetMask;                                                \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_SUBNET_OBJECT
{
    ANSC_PROPERTY_SUBNET_CLASS_CONTENT
}
ANSC_PROPERTY_SUBNET_OBJECT,  *PANSC_PROPERTY_SUBNET_OBJECT;

#define  ACCESS_ANSC_PROPERTY_SUBNET_OBJECT(p)      \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_SUBNET_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT PROPERTY OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */
#define  ANSC_PROPERTY_IPRANGE_FROM_NAME            "addressFrom"
#define  ANSC_PROPERTY_IPRANGE_TO_NAME              "addressTo"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  PUCHAR
(*PFN_PIPRANGEO_GET_ADDR)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_PIPRANGEO_SET_ADDR)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      address
    );

/*
 * Each derived property object handles a specific data type, it provides read/write access to
 * the encapsulated property values. Some property objects may encapsulate multiple parameters
 * into a single property since they're always used together. For example, when describing an
 * IP subnet, we always use a pair of IP address and subnet.
 */
#define  ANSC_PROPERTY_IPRANGE_CLASS_CONTENT                                                \
    /* duplication of the base object class content */                                      \
    ANSC_PROPERTY_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    union                                                                                   \
    {                                                                                       \
        UCHAR                       Dot[IP_ADDRESS_LENGTH];                                 \
        ULONG                       Value;                                                  \
    }Address1;                                                                              \
    union                                                                                   \
    {                                                                                       \
        UCHAR                       Dot[IP_ADDRESS_LENGTH];                                 \
        ULONG                       Value;                                                  \
    }Address2;                                                                              \
                                                                                            \
    PFN_PIPRANGEO_GET_ADDR          GetAddressFrom;                                         \
    PFN_PIPRANGEO_SET_ADDR          SetAddressFrom;                                         \
    PFN_PIPRANGEO_GET_ADDR          GetAddressTo;                                           \
    PFN_PIPRANGEO_SET_ADDR          SetAddressTo;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_PROPERTY_IPRANGE_OBJECT
{
    ANSC_PROPERTY_IPRANGE_CLASS_CONTENT
}
ANSC_PROPERTY_IPRANGE_OBJECT,  *PANSC_PROPERTY_IPRANGE_OBJECT;

#define  ACCESS_ANSC_PROPERTY_IPRANGE_OBJECT(p)     \
         ACCESS_CONTAINER(p, ANSC_PROPERTY_IPRANGE_OBJECT, Linkage)


#endif
