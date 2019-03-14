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

    module: ansc_asn1_obj_interface.h

        For Abstract Syntax Notation One (ASN.1),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the ASN.1 object implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/2002  initial revision.
        05/07/2002  A new api was added for choice object.
        05/08/2002  Another api SetSelectionByOID was added for Choice
        06/14/2002  PFN_ASN1_GET_VALUE_BUFFER was added for Integer  
                    and STRING objects.
        06/27/2002  GetFirstIntegerChild was added for SEQUENCE object
        06/27/2002  IsChoiceByInt was added fro CHOICE object
        09/24/2002  an api to check equaling to OID value or not;
        11/14/2002  Add SetUlongValue for INTEGER object;

**********************************************************************/


#ifndef  _ANSC_ASN1_OBJ_INTERFACE_
#define  _ANSC_ASN1_OBJ_INTERFACE_

/***********************************************************
           OTHER OBJECT FUNCTIONS DEFINITION
***********************************************************/
/*
 *  APIs for ASN1_BOOL object
 */
typedef  BOOLEAN
(*PFN_ASN1_GET_BOOLEAN_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_BOOLEAN_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bValue
    );

/*
 *  APIs for ASN.1 Real object
 */
typedef  DOUBLE
(*PFN_ASN1_GET_REAL_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_REAL_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        DOUBLE                      dValue
    );

/*
 *  APIs for ASN.1 INTEGER and ENUMERATE object
 */
typedef  LONG
(*PFN_ASN1_GET_INTEGER_VALUE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_INTEGER_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lValue
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_ULONG_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uValue
    );

/*
 *  API for ASN.1 INTEGER and STRING
 */
typedef  PUCHAR
(*PFN_ASN1_GET_VALUE_BUFFER)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 *  APIs for ASN.1 INTEGER only
 */
typedef  BOOLEAN
(*PFN_ASN1_IS_BIG_INTEGER)
    (
        ANSC_HANDLE                 hThisObject
    );


/*
 *  APIs for all ASN.1 STRING object;
 */
typedef  ANSC_STATUS
(*PFN_ASN1_SET_STRING_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData,
        ULONG                       length
    );

typedef  ANSC_STATUS
(*PFN_ASN1_GET_STRING_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     ppData,
        PULONG                      pLength
    );

/*
 *  APIs for ASN.1 BITSTRING object
 */
typedef  ULONG
(*PFN_ASN1_GET_UNUSED_BIT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_UNUSED_BIT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       bUnused
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_BITSTRING_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       bUnusedBits,
        PUCHAR                      pData,
        ULONG                       length
    );

/*
 *  APIs for ASN.1 CHOICE , SEQUENCE and SET object
 */
typedef  PANSC_ATTR_OBJECT
(*PFN_ASN1_CREATE_CHILD_ATTR)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

typedef  PCHAR
(*PFN_ASN1_GET_CHILD_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       selType
    );

typedef  BOOLEAN
(*PFN_ASN1_GET_CHILD_TAG_VALUE) 
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uIndex,
        PASN_OBJECT_FORM_TYPE       pAttr,
        PULONG                      pTagValue
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_CREATE_CHILD_BY_INDEX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

/*
 *  APIs for ASN.1 CHOICE object
 */
typedef  ANSC_HANDLE
(*PFN_ASN1_GET_SELECTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_SELECTION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index,
        ANSC_HANDLE                 hSelection
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_CREATE_SELECTION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_SELECTION_BY_OID)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

typedef  BOOLEAN
(*PFN_ASN1_CHOICE_CHECKING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  LONG
(*PFN_ASN1_GET_CHOICE_BY_OID) 
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pOIDString
    );

typedef  PCHAR
(*PFN_ASN1_GET_OIDVALUE_BY_MASK) 
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       mask
    );

/*
 *  APIs for ASN.1 OID  object
 */
typedef  ANSC_STATUS
(*PFN_ASN1_SET_OID_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pOIDUnitArray,
        ULONG                       uSize
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_STRING_OID_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pStringValue
    );

typedef  ANSC_STATUS
(*PFN_ASN1_GET_OID_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pOIDArray,
        PULONG                      pSize
    );

typedef  PULONG
(*PFN_ASN1_GET_OID_BUFFER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOLEAN
(*PFN_ASN1_GET_STRING_OID_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pStringValue
    );

typedef BOOLEAN
(*PFN_ASN1_EQUALTO_OID)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pStringValue
    );
/*
 *  APIs for ASN.1 TIME object
 */
typedef  ANSC_STATUS
(*PFN_ASN1_SET_TIME_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    );

typedef  ANSC_STATUS
(*PFN_ASN1_GET_TIME_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pYear,
        PULONG                      pMonth,
        PULONG                      pDay,
        PULONG                      pHour,
        PULONG                      pMinute,
        PULONG                      pSecond
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_UNIVERSAL_TIME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_UNIVERSAL_TIME         uniTime
    );

typedef  BOOLEAN
(*PFN_ASN1_CHECK_TIME_STATE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uYear,
        ULONG                       uMonth,
        ULONG                       uDay,
        ULONG                       uHour,
        ULONG                       uMinute,
        ULONG                       uSecond
    );

/*
 *  APIs for ASN.1 SEQUENCE and SET object
 */
typedef  ANSC_HANDLE
(*PFN_ASN1_GET_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild,
        BOOLEAN                     bFreeOldOne
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_CHILD_BY_INDEX)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild,
        ULONG                       index,
        BOOLEAN                     bFreeOldOne
    );

typedef void
(*PFN_ASN1_CREATE_ALL_CHILDREN)
    (
        ANSC_HANDLE                 hThisObject
    );
/*
 *  APIs for ASN.1 SEQUENCE only
 */
typedef  PCHAR
(*PFN_ASN1_GET_EXTRA_CHILD_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_GET_SPECIAL_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SET_SPECIAL_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSpecialChild,
        BOOLEAN                     bFreeOldOne
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_CREATE_SPECIAL_CHILD)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_ASN1_SEQUENCE_ACTION)
    (
        ANSC_HANDLE                 hThisObject,
        int                         index,
        PVOID*                      ppEncoding
    );

/*
 *  APIs for ASN.1 SEQUENCE, SEQUENCE_OF, SET AND SET_OF object
 */
typedef  BOOLEAN
(*PFN_ASN1_ENUM_CHILD_PROC)
    (
        ANSC_HANDLE                 hChildObject,
        PVOID                       pContext
    );

typedef  BOOLEAN
(*PFN_ASN1_ENUM_CHILD_OBJECT)
    (
        ANSC_HANDLE                 hThisObject,
        PFN_ASN1_ENUM_CHILD_PROC    hProc,
        PVOID                       pContext
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_GET_CHILD_BY_INDEX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    );

typedef  ULONG
(*PFN_ASN1_GET_CHILD_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  PANSC_ATTR_OBJECT
(*PFN_ASN1_CREATE_ATTR)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * APIs for SETOF and SEQUENCEOF only
 */
typedef  ANSC_STATUS
(*PFN_ASN1_REMOVE_ALL_CHILDREN)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bFreeMemory
    );

typedef  ANSC_STATUS
(*PFN_ASN1_REMOVE_CHILD_BY_INDEX)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index,
        BOOLEAN                     bFreeMemory
    );

typedef  ANSC_STATUS
(*PFN_ASN1_ADD_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

typedef  ANSC_STATUS
(*PFN_ASN1_VALID_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChild
    );

typedef  ANSC_HANDLE
(*PFN_ASN1_CREATE_CHILD)
    (
        ANSC_HANDLE                 hThisObject,
        BOOLEAN                     bAddItIn
    );

/***************************************************************
 *
 *  DEFINITION OF ASN1_ANY OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_ANY_CLASS_NAME                   "ANSC_ASN1_ANY"
#define  ANSC_ASN1_ANY_NAME                         "ANY"
#define  ANSC_ASN1_ANY_ID                           ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_ANY_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    PVOID                           pStringBuffer;                                          \
    ULONG                           uLength;                                                \
    PANSC_TAGINFO_OBJECT            pTagInfo;                                               \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_ANY
{
    ANSC_ASN1_ANY_CLASS_CONTENT
}
ANSC_ASN1_ANY,  *PANSC_ASN1_ANY;

#define  ACCESS_ANSC_ASN1_ANY(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_ANY, Linkage)

/***************************************************************
 *
 *  DEFINITION OF ASN1_INTEGER OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_INTEGER_CLASS_NAME               "ANSC_ASN1_INTEGER"
#define  ANSC_ASN1_INTEGER_NAME                     "INTEGER"
#define  ANSC_ASN1_INTEGER_ID                       ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_INTEGER_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ULONG                           uLength;                                                \
    LONG                            lValue;                                                 \
    PUCHAR                          pValueBuffer;                                           \
    BOOLEAN                         bIsDynamic;                                             \
    BOOLEAN                         bFirstZero;                                             \
                                                                                            \
    PFN_ASN1_GET_VALUE_BUFFER       GetValueBuffer;                                         \
    PFN_ASN1_GET_INTEGER_VALUE      GetIntegerValue;                                        \
    PFN_ASN1_SET_INTEGER_VALUE      SetIntegerValue;                                        \
    PFN_ASN1_SET_ULONG_VALUE        SetUlongValue;                                          \
    PFN_ASN1_IS_BIG_INTEGER         IsBigInteger;                                           \
    PFN_ASN1_SET_STRING_VALUE       SetStringValue;                                         \
    PFN_ASN1_SET_STRING_VALUE       SetUnsignedStringValue;                                 \
    PFN_ASN1_GET_STRING_VALUE       GetStringValue;                                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_INTEGER
{
    ANSC_ASN1_INTEGER_CLASS_CONTENT
}
ANSC_ASN1_INTEGER,  *PANSC_ASN1_INTEGER;

#define  ACCESS_ANSC_ASN1_INTEGER(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_INTEGER, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_BOOL OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_BOOL_CLASS_NAME                  "ANSC_ASN1_BOOL"
#define  ANSC_ASN1_BOOL_NAME                        "BOOL"
#define  ANSC_ASN1_BOOL_ID                          ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_BOOL_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    BOOLEAN                         bValue;                                                 \
                                                                                            \
    PFN_ASN1_GET_BOOLEAN_VALUE      GetBooleanValue;                                        \
    PFN_ASN1_SET_BOOLEAN_VALUE      SetBooleanValue;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_BOOL
{
    ANSC_ASN1_BOOL_CLASS_CONTENT
}
ANSC_ASN1_BOOL,  *PANSC_ASN1_BOOL;

#define  ACCESS_ANSC_ASN1_BOOL(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_BOOL, Linkage)

/***************************************************************
 *
 *  DEFINITION OF ASN1_REAL OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_REAL_CLASS_NAME                  "ANSC_ASN1_REAL"
#define  ANSC_ASN1_REAL_NAME                        "REAL"
#define  ANSC_ASN1_REAL_ID                          ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_REAL_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    DOUBLE                          dValue;                                                 \
                                                                                            \
    PFN_ASN1_GET_REAL_VALUE         GetRealValue;                                           \
    PFN_ASN1_SET_REAL_VALUE         SetRealValue;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_REAL
{
    ANSC_ASN1_REAL_CLASS_CONTENT
}
ANSC_ASN1_REAL,  *PANSC_ASN1_REAL;

#define  ACCESS_ANSC_ASN1_REAL(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_REAL, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_ENUMERATE OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_ENUMERATE_CLASS_NAME             "ANSC_ASN1_ENUMERATE"
#define  ANSC_ASN1_ENUMERATE_NAME                   "ENUMERATE"
#define  ANSC_ASN1_ENUMERATE_ID                     ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_ENUMERATE_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ULONG                           uValue;                                                 \
    ULONG                           uLength;                                                \
    BOOLEAN                         bFirstZero;                                             \
                                                                                            \
    PFN_ASN1_GET_INTEGER_VALUE      GetIntegerValue;                                        \
    PFN_ASN1_SET_INTEGER_VALUE      SetIntegerValue;                                        \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_ENUMERATE
{
    ANSC_ASN1_ENUMERATE_CLASS_CONTENT
}
ANSC_ASN1_ENUMERATE,  *PANSC_ASN1_ENUMERATE;

#define  ACCESS_ANSC_ASN1_ENUMERATE(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_ENUMERATE, Linkage)

/***************************************************************
 *
 *  DEFINITION OF ASN1_NULL OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_NULL_CLASS_NAME                  "ANSC_ASN1_ASN1_NULL"
#define  ANSC_ASN1_NULL_NAME                        "NULL"
#define  ANSC_ASN1_NULL_ID                          ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_NULL_CLASS_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_NULL
{
    ANSC_ASN1_NULL_CLASS_CONTENT
}
ANSC_ASN1_NULL,  *PANSC_ASN1_NULL;

#define  ACCESS_ANSC_ASN1_NULL(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_NULL, Linkage)

/***************************************************************
 *
 *  DEFINITION OF ASN1_TIME OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_UNIVERSAL_TIME_CLASS_NAME        "ANSC_ASN1_UNIVERSALTIME"
#define  ANSC_ASN1_UNIVERSAL_TIME_NAME              "UNIVERSALTIME"
#define  ANSC_ASN1_UNIVERSAL_TIME_ID                ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_GENERALIZED_TIME_CLASS_NAME      "ANSC_ASN1_GENERALIZEDTIME"
#define  ANSC_ASN1_GENERALIZED_TIME_NAME            "GENERALIZEDTIME"
#define  ANSC_ASN1_GENERALIZEDTIME_ID               ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_ALTIME_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ANSC_UNIVERSAL_TIME             mTime;                                                  \
    PFN_ASN1_GET_TIME_VALUE         GetTimeValue;                                           \
    PFN_ASN1_SET_TIME_VALUE         SetTimeValue;                                           \
    PFN_ASN1_SET_UNIVERSAL_TIME     SetUniveralTime;                                        \
    PFN_ASN1_CHECK_TIME_STATE       IsBefore;                                               \
    PFN_ASN1_CHECK_TIME_STATE       IsAfter;                                                \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_ALTIME
{
    ANSC_ASN1_ALTIME_CLASS_CONTENT
}
ANSC_ASN1_ALTIME,  *PANSC_ASN1_ALTIME;

typedef  ANSC_ASN1_ALTIME  ANSC_ASN1_UNIVERSALTIME,        *PANSC_ASN1_UNIVERSALTIME;
typedef  ANSC_ASN1_ALTIME  ANSC_ASN1_GENERALIZEDTIME,      *PANSC_ASN1_GENERALIZEDTIME;

#define  ACCESS_ANSC_ASN1_UNIVERSAL_TIME(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_UNIVERSAL_TIME, Linkage)

#define  ACCESS_ANSC_ASN1_GENERALIZED_TIME(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_GENERALIZED_TIME, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_OID OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_OID_CLASS_NAME                   "ANSC_ASN1_OID"
#define  ANSC_ASN1_OID_NAME                         "OID"
#define  ANSC_ASN1_OID_ID                           ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_OID_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ULONG                           uLength;                                                \
    ULONG                           pOIDChain[MAXIMUM_OID_DEPTH];                           \
    PULONG                          pAdvOID;                                                \
                                                                                            \
    PFN_ASN1_GET_OID_BUFFER         GetOIDBuffer;                                           \
    PFN_ASN1_GET_OID_VALUE          GetOIDValue;                                            \
    PFN_ASN1_SET_OID_VALUE          SetOIDValue;                                            \
    PFN_ASN1_SET_STRING_OID_VALUE   SetStringOIDValue;                                      \
    PFN_ASN1_GET_STRING_OID_VALUE   GetStringOIDValue;                                      \
    PFN_ASN1_EQUALTO_OID            EqualToOIDString;                                       \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_OIDEN
{
    ANSC_ASN1_OID_CLASS_CONTENT
}
ANSC_ASN1_OIDEN,  *PANSC_ASN1_OIDEN;

#define  ACCESS_ANSC_ASN1_OIDEN(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_OIDEN, Linkage)

/***************************************************************
 *
 *  DEFINITION OF ASN1_STRING OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_OCTETSTRING_CLASS_NAME           "ANSC_ASN1_OCTETSTRING"
#define  ANSC_ASN1_OCTETSTRING_NAME                 "OCTETSTRING"
#define  ANSC_ASN1_OCTETSTRING_ID                   ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_UTF8STRING_CLASS_NAME            "ANSC_ASN1_UTF8STRING"
#define  ANSC_ASN1_UTF8STRING_NAME                  "UTF8STRING"
#define  ANSC_ASN1_UTF8STRING_ID                    ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_UNIVERSALSTRING_CLASS_NAME      "ANSC_ASN1_UNIVERSALSTRING"
#define  ANSC_ASN1_UNIVERSALSTRING_NAME            "UNIVERSALSTRING"
#define  ANSC_ASN1_UNIVERSALSTRING_ID               ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_NUMERICSTRING_CLASS_NAME         "ANSC_ASN1_NUMERICSTRING"
#define  ANSC_ASN1_NUMERICSTRING_NAME               "NUMERICSTRING"
#define  ANSC_ASN1_NUMERICSTRING_ID                 ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_PRINTABLESTRING_CLASS_NAME       "ANSC_ASN1_PRINTABLESTRING"
#define  ANSC_ASN1_PRINTABLESTRING_NAME             "PRINTABLESTRING"
#define  ANSC_ASN1_PRINTABLESTRING_ID               ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_TELETEXSTRING_CLASS_NAME         "ANSC_ASN1_TELETEXSTRING"
#define  ANSC_ASN1_TELETEXSTRING_NAME               "TELETEXSTRING"
#define  ANSC_ASN1_TELETEXSTRING_ID                 ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_VIDEOTEXSTRING_CLASS_NAME        "ANSC_ASN1_VIDEOTEXSTRING"
#define  ANSC_ASN1_VIDEOTEXSTRING_NAME              "VIDEOTEXSTRING"
#define  ANSC_ASN1_VIDEOTEXSTRING_ID                ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_IA5STRING_CLASS_NAME             "ANSC_ASN1_IA5STRING"
#define  ANSC_ASN1_IA5STRING_NAME                   "IA5STRING"
#define  ANSC_ASN1_IA5STRING_ID                     ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_GENERALSTRING_CLASS_NAME         "ANSC_ASN1_GENERALSTRING"
#define  ANSC_ASN1_GENERALSTRING_NAME               "GENERALSTRING"
#define  ANSC_ASN1_GENERALSTRING_ID                 ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_GRAPHICSTRING_CLASS_NAME         "ANSC_ASN1_GRAPHICSTRING"
#define  ANSC_ASN1_GRAPHICSTRING_NAME               "GRAPHICSTRING"
#define  ANSC_ASN1_GRAPHICSTRING_ID                 ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_VISIBLESTRING_CLASS_NAME         "ANSC_ASN1_VISIBLESTRING"
#define  ANSC_ASN1_VISIBLESTRING_NAME               "VISIBLESTRING"
#define  ANSC_ASN1_VISIBLESTRING_ID                 ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_BMPSTRING_CLASS_NAME             "ANSC_ASN1_BMPSTRING"
#define  ANSC_ASN1_BMPSTRING_NAME                   "BMPSTRING"
#define  ANSC_ASN1_BMPSTRING_ID                     ANSC_ASN1_OBJECT_ID

#define  ANSC_ASN1_MSSTRING_CLASS_NAME              "ANSC_ASN1_MSSTRING"
#define  ANSC_ASN1_MSSTRING_NAME                    "MSSTRING"
#define  ANSC_ASN1_MSSTRING_ID                      ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_STRING_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    UCHAR                           pString[MAXIMUM_OCTETSTRING_OCTETS];                    \
    PVOID                           pStringBuffer;                                          \
    ULONG                           uLength;                                                \
    BOOLEAN                         bIsDynamic;                                             \
                                                                                            \
    PFN_ASN1_GET_VALUE_BUFFER       GetValueBuffer;                                         \
    PFN_ASN1_GET_STRING_VALUE       GetStringValue;                                         \
    PFN_ASN1_SET_STRING_VALUE       SetStringValue;                                         \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_STRING
{
    ANSC_ASN1_STRING_CLASS_CONTENT
}
ANSC_ASN1_STRING,  *PANSC_ASN1_STRING;

typedef  ANSC_ASN1_STRING  ANSC_ASN1_OCTETSTRING,       *PANSC_ASN1_OCTETSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_UTF8STRING,        *PANSC_ASN1_UTF8STRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_UNIVERSALSTRING,   *PANSC_ASN1_UNIVERSALSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_NUMERICSTRING,     *PANSC_ASN1_NUMERICSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_PRINTABLESTRING,   *PANSC_ASN1_PRINTABLESTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_TELETEXSTRING,     *PANSC_ASN1_TELETEXSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_VIDEOTEXSTRING,    *PANSC_ASN1_VIDEOTEXSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_IA5STRING,         *PANSC_ASN1_IA5STRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_GENERALSTRING,     *PANSC_ASN1_GENERALSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_GRAPHICSTRING,     *PANSC_ASN1_GRAPHICSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_VISIBLESTRING,     *PANSC_ASN1_VISIBLESTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_BMPSTRING,         *PANSC_ASN1_BMPSTRING;
typedef  ANSC_ASN1_STRING  ANSC_ASN1_MSSTRING,          *PANSC_ASN1_MSSTRING;


#define  ACCESS_ANSC_ASN1_OCTETSTRING(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_OCTETSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_UTF8STRING(p)            \
         ACCESS_CONTAINER(p, ANSC_ASN1_UTF8STRING, Linkage)

#define  ACCESS_ANSC_ASN1_UNIVERSALSTRING(p)       \
         ACCESS_CONTAINER(p, ANSC_ASN1_UNIVERSALSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_NUMERICSTRING(p)         \
         ACCESS_CONTAINER(p, ANSC_ASN1_NUMERICSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_PRINTABLESTRING(p)       \
         ACCESS_CONTAINER(p, ANSC_ASN1_PRINTABLESTRING, Linkage)

#define  ACCESS_ANSC_ASN1_TELETEXSTRING(p)         \
         ACCESS_CONTAINER(p, ANSC_ASN1_TELETEXSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_VIDEOTEXSTRING(p)        \
         ACCESS_CONTAINER(p, ANSC_ASN1_VIDEOTEXSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_IA5STRING(p)             \
         ACCESS_CONTAINER(p, ANSC_ASN1_IA5STRING, Linkage)

#define  ACCESS_ANSC_ASN1_GENERALSTRING(p)         \
         ACCESS_CONTAINER(p, ANSC_ASN1_GENERALSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_GRAPHICSTRING(p)         \
         ACCESS_CONTAINER(p, ANSC_ASN1_GRAPHICSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_VISIBLESTRING(p)         \
         ACCESS_CONTAINER(p, ANSC_ASN1_VISIBLESTRING, Linkage)

#define  ACCESS_ANSC_ASN1_BMPSTRING(p)         \
         ACCESS_CONTAINER(p, ANSC_ASN1_BMPSTRING, Linkage)

#define  ACCESS_ANSC_ASN1_MSSTRING(p)         \
         ACCESS_CONTAINER(p, ANSC_ASN1_MSSTRING, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_BITSTRING OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_BITSTRING_CLASS_NAME             "ANSC_ASN1_BITSTRING"
#define  ANSC_ASN1_BITSTRING_NAME                   "BITSTRING"
#define  ANSC_ASN1_BITSTRING_ID                     ANSC_ASN1_OBJECT_ID


#define  ANSC_ASN1_BITSTRING_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_STRING_CLASS_CONTENT                                                          \
    /* start of object class content */                                                     \
    ULONG                           uUnusedBit;                                             \
                                                                                            \
    PFN_ASN1_GET_UNUSED_BIT         GetUnusedBit;                                           \
    PFN_ASN1_SET_UNUSED_BIT         SetUnusedBit;                                           \
    PFN_ASN1_SET_BITSTRING_VALUE    SetBitStringValue;                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_BITSTRING
{
    ANSC_ASN1_BITSTRING_CLASS_CONTENT
}
ANSC_ASN1_BITSTRING,  *PANSC_ASN1_BITSTRING;

#define  ACCESS_ANSC_ASN1_BITSTRING(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_BITSTRING, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_CHOICE OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_CHOICE_CLASS_NAME                "ANSC_ASN1_CHOICE"
#define  ANSC_ASN1_CHOICE_NAME                      "CHOICE"
#define  ANSC_ASN1_CHOICE_ID                        ANSC_ASN1_OBJECT_ID + 0x05
#define  ANSC_CHOICE_SEL_NAME                       "selection"

#define  ANSC_ASN1_CHOICE_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSelection;                                             \
    LONG                            lSelect;                                                \
    BOOLEAN                         bChoiceByOID;                                           \
    BOOLEAN                         bChoiceByInt;                                           \
    ULONG                           uTotalChoice;                                           \
                                                                                            \
    PFN_ASN1_GET_CHILD_NAME         GetSelectionName;                                       \
    PFN_ASN1_CREATE_CHILD_ATTR      CreateSelectionAttr;                                    \
    PFN_ASN1_GET_SELECTION          GetSelection;                                           \
    PFN_ASN1_SET_SELECTION          SetSelection;                                           \
    PFN_ASN1_SET_CREATE_SELECTION   SetAndCreateSelection;                                  \
    PFN_ASN1_SET_SELECTION_BY_OID   SetSelectionByOID;                                      \
    PFN_ASN1_CREATE_CHILD_BY_INDEX  CreateSelection;                                        \
    PFN_ASN1_GET_CHOICE_BY_OID      GetChoiceFromOID;                                       \
    PFN_ASN1_GET_CHILD_TAG_VALUE    GetChoiceTagValue;                                      \
    PFN_ASN1_GET_OIDVALUE_BY_MASK   GetOIDValueByMask;                                      \
    PFN_ASN1_CHOICE_CHECKING        IsChoiceByOID;                                          \
    PFN_ASN1_CHOICE_CHECKING        IsChoiceByInt;                                          \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_CHOICE
{
    ANSC_ASN1_CHOICE_CLASS_CONTENT
}
ANSC_ASN1_CHOICE,  *PANSC_ASN1_CHOICE;

#define  ACCESS_ANSC_ASN1_CHOICE(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_CHOICE, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_SET OBJECT
 *
 ***************************************************************/
/*
 *  Define a universal SET/SETOF/SEQUENCE/SEQUENCEOF type
 */
#define  ANSC_ASN1_GROUP_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_CLASS_CONTENT                                                                 \
    /* start of object class content */                                                     \
    SLIST_HEADER                    sChildList;                                             \
    ULONG                           uTotalChild;                                            \
                                                                                            \
    PFN_ASN1_GET_CHILD_BY_INDEX     GetChildByIndex;                                        \
    PFN_ASN1_GET_CHILD_COUNT        GetChildCount;                                          \
    PFN_ASN1_REMOVE_ALL_CHILDREN    RemoveAllChildren;                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_GROUP
{
    ANSC_ASN1_GROUP_CLASS_CONTENT
}
ANSC_ASN1_GROUP,  *PANSC_ASN1_GROUP;


#define  ANSC_ASN1_SET_CLASS_NAME                   "ANSC_ASN1_SET"
#define  ANSC_ASN1_SET_NAME                         "SET"
#define  ANSC_ASN1_SET_ID                           ANSC_ASN1_OBJECT_ID + 0x01

#define  ANSC_ASN1_SET_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_GROUP_CLASS_CONTENT                                                           \
    /* start of object class content */                                                     \
    PFN_ASN1_ENUM_CHILD_OBJECT      EnumChildObject;                                        \
    PFN_ASN1_CREATE_CHILD_ATTR      CreateChildAttr;                                        \
    PFN_ASN1_GET_CHILD_NAME         GetChildName;                                           \
    PFN_ASN1_SET_CHILD_BY_INDEX     SetChildByIndex;                                        \
    PFN_ASN1_CREATE_CHILD_BY_INDEX  CreateChildObject;                                      \
    PFN_ASN1_CREATE_ALL_CHILDREN    CreateAllChildren;                                      \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_SET
{
    ANSC_ASN1_SET_CLASS_CONTENT
}
ANSC_ASN1_SET,  *PANSC_ASN1_SET;

#define  ACCESS_ANSC_ASN1_SET(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_SET, Linkage)

/***************************************************************
 *
 *  DEFINITION OF ASN1_SEQUENCE OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_SEQUENCE_CLASS_NAME              "ANSC_ASN1_SEQUENCE"
#define  ANSC_ASN1_SEQUENCE_NAME                    "SEQUENCE"
#define  ANSC_ASN1_SEQUENCE_ID                       ANSC_ASN1_OBJECT_ID + 0x02
#define  ANSC_SEQUENCE_CHILD_NAME                    "child"
#define  ANSC_SEQUENCE_EXTRA_CHILD_NAME              "extra"

#define  ANSC_ASN1_SEQUENCE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_SET_CLASS_CONTENT                                                             \
    /* start of object class content */                                                     \
    BOOLEAN                         bDecodingExtra;                                         \
    ANSC_HANDLE                     pExtraChild;                                            \
    PFN_ASN1_GET_SPECIAL_CHILD      GetExtraChild;                                          \
    PFN_ASN1_SET_SPECIAL_CHILD      SetExtraChild;                                          \
    PFN_ASN1_GET_SPECIAL_CHILD      GetFirstOIDChild;                                       \
    PFN_ASN1_GET_SPECIAL_CHILD      GetLastOIDChild;                                        \
    PFN_ASN1_GET_SPECIAL_CHILD      GetFirstIntegerChild;                                   \
    PFN_ASN1_GET_SPECIAL_CHILD      GetLastStringChild;                                     \
    PFN_ASN1_GET_EXTRA_CHILD_NAME   GetExtraChildName;                                      \
    PFN_ASN1_CREATE_SPECIAL_CHILD   CreateExtraChild;                                       \
    PFN_ASN1_SEQUENCE_ACTION        BeforeEncodingChild;                                    \
    PFN_ASN1_SEQUENCE_ACTION        AfterEncodingChild;                                     \
    PFN_ASN1_SEQUENCE_ACTION        BeforeDecodingChild;                                    \
    PFN_ASN1_SEQUENCE_ACTION        AfterDecodingChild;                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_SEQUENCE
{
    ANSC_ASN1_SEQUENCE_CLASS_CONTENT
}
ANSC_ASN1_SEQUENCE,  *PANSC_ASN1_SEQUENCE;

#define  ACCESS_ANSC_ASN1_SEQUENCE(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_SEQUENCE, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_SETOF OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_SETOF_CLASS_NAME                 "ANSC_ASN1_SETOF"
#define  ANSC_ASN1_SETOF_NAME                       "SETOF"
#define  ANSC_ASN1_SETOF_ID                         ANSC_ASN1_OBJECT_ID + 0x03
#define  ANSC_SETOF_CHILD_NAME                      "child"

#define  ANSC_ASN1_SETOF_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSC_ASN1_GROUP_CLASS_CONTENT                                                           \
    PFN_ASN1_ENUM_CHILD_OBJECT      EnumChildObject;                                        \
    PFN_ASN1_REMOVE_CHILD_BY_INDEX  RemoveChildByIndex;                                     \
    PFN_ASN1_ADD_CHILD              AddChild;                                               \
    PFN_ASN1_CREATE_CHILD           CreateChild;                                            \
    PFN_ASN1_VALID_CHILD            IsChildValid;                                           \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_ASN1_SETOF
{
    ANSC_ASN1_SETOF_CLASS_CONTENT
}
ANSC_ASN1_SETOF,  *PANSC_ASN1_SETOF;

#define  ACCESS_ANSC_ASN1_SETOF(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_SETOF, Linkage)


/***************************************************************
 *
 *  DEFINITION OF ASN1_SEQUENCEOF OBJECT
 *
 ***************************************************************/
#define  ANSC_ASN1_SEQUENCEOF_CLASS_NAME            "ANSC_ASN1_SEQUENCEOF"
#define  ANSC_ASN1_SEQUENCEOF_NAME                  "SEQUENCEOF"
#define  ANSC_ASN1_SEQUENCEOF_ID                    ANSC_ASN1_OBJECT_ID + 0x04
#define  ANSC_SEQUENCEOF_CHILD_NAME                 "child"

#define  ANSC_ASN1_SEQUENCEOF_CLASS_CONTENT              ANSC_ASN1_SETOF_CLASS_CONTENT

typedef  struct
_ANSC_ASN1_SEQUENCEOF
{
    ANSC_ASN1_SEQUENCEOF_CLASS_CONTENT
}
ANSC_ASN1_SEQUENCEOF,  *PANSC_ASN1_SEQUENCEOF;

#define  ACCESS_ANSC_ASN1_SEQUENCEOF(p)           \
         ACCESS_CONTAINER(p, ANSC_ASN1_SEQUENCEOF, Linkage)

#endif  /*_ANSC_ASN1_OBJ_INTERFACE_*/

