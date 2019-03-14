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

    module:	ansc_string_co_internal_api.h

        For string,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the string component
        object.

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

#ifndef  _ANSC_STRING_CO_INTERNAL_API
#define  _ANSC_STRING_CO_INTERNAL_API


void
AnscStringCOSetString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue              /* ZERO-terminated string */
    );

void
AnscStringCOSetBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBuffer,            /* no need to ZERO-terminated and 0s are allowed in the buffer */
        ULONG                       ulBufferLen
    );

void
AnscStringCOCopy
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hString             /* another string to be copied */
    );

ULONG
AnscStringCOGetLength
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
AnscStringCOIsEmpty
    (
        ANSC_HANDLE                 hThisObject
    );

void
AnscStringCOEmpty
    (
        ANSC_HANDLE                 hThisObject
    );

UCHAR
AnscStringCOGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

void
AnscStringCOSetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        UCHAR                       uChar
    );

PUCHAR
AnscStringCOGetString
    (
        ANSC_HANDLE                 hThisObject
    );

BOOL
AnscStringCOEqual
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    );

BOOL
AnscStringCOEqualNoCase
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    );

ANSC_HANDLE
AnscStringCOLeft
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

ANSC_HANDLE
AnscStringCORight
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCount
    );

LONG
AnscStringCOFindChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    );

LONG
AnscStringCOFindString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pString
    );

void
AnscStringCOUpper
    (
        ANSC_HANDLE                 hThisObject
    );

void
AnscStringCOLower
    (
        ANSC_HANDLE                 hThisObject
    );

void
AnscStringCOResize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize
    );

void
AnscStringCOTrimLeftChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    );

void
AnscStringCOTrimLeftString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue
    );

void
AnscStringCOTrimRightChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       uChar
    );

void
AnscStringCOTrimRightString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue
    );

ULONG
AnscStringCOReplaceChar
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       chOld,
        UCHAR                       chNew
    );

ULONG
AnscStringCOReplaceString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pOldStr,
        PUCHAR                      pNewStr
    );

void
AnscStringCOAppendBuffer
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue,
        ULONG                       ulValueLen
    );

void
AnscStringCOAppendString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue
    );

void
AnscStringCOConcat
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 String
    );

char *
AnscStringCOExtractBuffer
    (
        ANSC_HANDLE                 hThisObject
    );

void
AnscStringCOAppendStringN
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pValue,
        ULONG                       ulTimes
    );

#endif

