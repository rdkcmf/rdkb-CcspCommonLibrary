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

    module:	ansc_string_util.h

        For 
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for the
        functions which are called to convert string to number and
        vice versa.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/

#ifndef  _ANSC_STRING_CONVERT
#define  _ANSC_STRING_CONVERT

#include "ansc_memory_complement.h"

#define  AnscStrStr                                 _ansc_strstr
#define  AnscStrChr                                 _ansc_strchr
#define  AnscStrSpn                                 _ansc_strspn
#define  AnscStrCSpn                                _ansc_strcspn

#define  AnscString2Int                             _ansc_atoi
#define  AnscString2Double                          _ansc_atof
#define  AnscInt2String                             _ansc_ultoa


/*
 *  AnscStrGetCharNum counts the specified character in a string
 */
ULONG
AnscStrBufGetCharNum
    (
        PUCHAR                      pString,
        ULONG                       ulStrLen,
        UCHAR                       uChar
    );


ULONG
AnscStrGetCharNum
    (
        PUCHAR                      pString,
        UCHAR                       uChar
    );


PUCHAR
AnscStrUpper
    (
        PUCHAR                      pStr
    );


PUCHAR
AnscStrLower
    (
        PUCHAR                      pStr
    );


PUCHAR
AnscDupString
    (
        PUCHAR                      pStr
    );

PVOID
AnscMemUtilRealloc
    (
        PVOID                       pMem,
        ULONG                       ulSize,
        ULONG                       ulNewSize
    );

BOOL
AnscValidStringCheck
    (
        PUCHAR                     pString
    );

BOOL
is_IpAddress
    (
        PUCHAR                     pString
    );

BOOL
is_Ipv6_address
    (
        PUCHAR                     pString
    );

BOOL is_ValidHost(PUCHAR pString);
BOOL is_ValidPort(PUCHAR pString);
BOOL is_ValidIpAddressv4_port(PUCHAR pString);
BOOL is_ValidIpAddressv6_port(PUCHAR pString);
BOOL is_PrivateIp(PUCHAR pString);

#endif

