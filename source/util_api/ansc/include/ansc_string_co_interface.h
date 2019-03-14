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

    module:	ansc_string_co_interface.h

        For Ansc String,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        string implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
**********************************************************************/

#ifndef  _ANSC_STRING_INTERFACE_DEC
#define  _ANSC_STRING_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  ANSC_STRING_COMPONENT_NAME                 "StringComponent"
#define  ANSC_STRING_COMPONENT_OID                  ANSC_OBJECT_OID_NULL
#define  ANSC_STRING_COMPONENT_TYPE                 ANSC_OBJECT_TYPE_NULL

#define  ANSC_STRING_RESIZE_MIN_INC                 512

typedef
void
(*PFN_STRINGCO_SET_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue              /* ZERO-terminated string */
    );

typedef
void
(*PFN_STRINGCO_SET_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,            /* no need to ZERO-terminated and 0s are allowed in the buffer */
        ULONG                       ulBufferLen
    );

typedef
void
(*PFN_STRINGCO_COPY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hString             /* another string to be copied */
    );

typedef
ULONG
(*PFN_STRINGCO_GET_LENGTH)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_STRINGCO_IS_EMPTY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_STRINGCO_EMPTY)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
UCHAR
(*PFN_STRINGCO_GET_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef
void
(*PFN_STRINGCO_SET_AT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        UCHAR                       uChar
    );

typedef
PUCHAR
(*PFN_STRINGCO_GET_STRING)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_STRINGCO_EQUAL)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    );

typedef
BOOL
(*PFN_STRINGCO_EQUAL_NO_CASE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    );

typedef
ANSC_HANDLE
(*PFN_STRINGCO_LEFT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef
ANSC_HANDLE
(*PFN_STRINGCO_RIGHT)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

typedef
LONG
(*PFN_STRINGCO_FIND_CHAR)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    );

typedef
LONG
(*PFN_STRINGCO_FIND_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    );

typedef
void
(*PFN_STRINGCO_UPPER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_STRINGCO_LOWER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_STRINGCO_RESIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    );

typedef
void
(*PFN_STRINGCO_TRIM_LEFT_CHAR)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    );

typedef
void
(*PFN_STRINGCO_TRIM_LEFT_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue
    );

typedef
void
(*PFN_STRINGCO_TRIM_RIGHT_CHAR)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    );

typedef
void
(*PFN_STRINGCO_TRIM_RIGHT_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue
    );

typedef
ULONG
(*PFN_STRINGCO_REPLACE_CHAR)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       chOld,
        UCHAR                       chNew
    );

typedef
ULONG
(*PFN_STRINGCO_REPLACE_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pOldStr,
        PUCHAR                      pNewStr
    );

typedef
void
(*PFN_STRINGCO_APPEND_BUFFER)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue,
        ULONG                       ulValueLen
    );

typedef
void
(*PFN_STRINGCO_APPEND_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue
    );

typedef
void
(*PFN_STRINGCO_CONCAT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 String
    );

typedef
char *
(*PFN_STRINGCO_EXTRACT_BUFFER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_STRINGCO_APPEND_STRING_N)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue,
        ULONG                       ulTimes
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  ANSC_STRING_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    PUCHAR                          pValue;                                             \
    ULONG                           ulValueLen;                                         \
    ULONG                           ulValueBufferLen;                                   \
    /* start of methods */                                                              \
    PFN_STRINGCO_SET_STRING         SetString;                                          \
    PFN_STRINGCO_SET_BUFFER         SetBuffer;                                          \
    PFN_STRINGCO_COPY               Copy;                                               \
    PFN_STRINGCO_GET_LENGTH         GetLength;                                          \
    PFN_STRINGCO_IS_EMPTY           IsEmpty;                                            \
    PFN_STRINGCO_EMPTY              Empty;                                              \
    PFN_STRINGCO_GET_AT             GetAt;                                              \
    PFN_STRINGCO_SET_AT             SetAt;                                              \
    PFN_STRINGCO_GET_STRING         GetString;                                          \
    PFN_STRINGCO_EQUAL              Equal;                                              \
    PFN_STRINGCO_EQUAL_NO_CASE      EqualNoCase;                                        \
    PFN_STRINGCO_LEFT               Left;                                               \
    PFN_STRINGCO_RIGHT              Right;                                              \
    PFN_STRINGCO_FIND_CHAR          FindChar;                                           \
    PFN_STRINGCO_FIND_STRING        FindString;                                         \
    PFN_STRINGCO_UPPER              Upper;                                              \
    PFN_STRINGCO_LOWER              Lower;                                              \
    PFN_STRINGCO_RESIZE             Resize;                                             \
    PFN_STRINGCO_TRIM_LEFT_CHAR     TrimLeftChar;                                       \
    PFN_STRINGCO_TRIM_LEFT_STRING   TrimLeftString;                                     \
    PFN_STRINGCO_TRIM_RIGHT_CHAR    TrimRightChar;                                      \
    PFN_STRINGCO_TRIM_RIGHT_STRING  TrimRightString;                                    \
    PFN_STRINGCO_REPLACE_CHAR       ReplaceChar;                                        \
    PFN_STRINGCO_REPLACE_STRING     ReplaceString;                                      \
    PFN_STRINGCO_APPEND_BUFFER      AppendBuffer;                                       \
    PFN_STRINGCO_APPEND_STRING      AppendString;                                       \
    PFN_STRINGCO_CONCAT             Concat;                                             \
    PFN_STRINGCO_EXTRACT_BUFFER     ExtractBuffer;                                      \
    PFN_STRINGCO_APPEND_STRING_N    AppendStringN;                                      \



/* content filtering basic message object  */
typedef  struct
_ANSC_STRING_OBJECT
{
    ANSC_STRING_CLASS_CONTENT
}
ANSC_STRING_OBJECT,    *PANSC_STRING_OBJECT;


#define  ACCESS_ANSC_STRING_OBJECT(p)            \
         ACCESS_CONTAINER(p, ANSC_STRING_OBJECT, Linkage)

#endif

