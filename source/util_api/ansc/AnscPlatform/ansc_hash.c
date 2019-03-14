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

    module:	ansc_hash.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to hashing.

        *   AnscHashAddress
        *   AnscHashUshort
        *   AnscHashUlong
        *   AnscHashString
        *   AnscHashString2

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/01/01    initial revision.

**********************************************************************/


#include "ansc_global.h"


ULONG
AnscHashAddress
    (
        PUCHAR                      address,
        ULONG                       ulTableSize
    )
{
    ULONG                           ulHashTemp = 0;

    if ( address[0] == 0 || address[2] == 0 )
    {
        ulHashTemp += address[0] + address[2];
    }
    else
    {
        ulHashTemp += address[0] * address[2] + address[0] + address[2];
    }

    if ( address[1] == 0 || address[3] == 0 )
    {
        ulHashTemp += address[1] + address[3];
    }
    else
    {
        ulHashTemp += address[1] * address[3] + address[1] + address[3];
    }

    return  ulHashTemp % ulTableSize;
}

/*
ULONG
AnscHashUshort
    (
        USHORT                      usValue,
        ULONG                       ulTableSize
    )
{
    ULONG                           ulHashTemp = 0;

    ulHashTemp = (ULONG)usValue;

    return  ulHashTemp % ulTableSize;
}


ULONG
AnscHashUlong
    (
        ULONG                       ulValue,
        ULONG                       ulTableSize
    )
{
    return  ulValue % ulTableSize;
}
*/

ULONG
AnscHashString
    (
        char*                       string,
        ULONG                       ulStringSize,
        ULONG                       ulTableSize
    )
{
    ULONG                           ulHashTemp = 0;
    ULONG                           i          = 0;

    if ( !string )
    {
        return  0;
    }

    for ( i = 0; i < ulStringSize; i++ )
    {
        ulHashTemp += i * (ULONG)string[i];
    }

    return  ulHashTemp % ulTableSize;
}


ULONG
AnscHashString2
    (
        char*                       string,
        ULONG                       ulStringSize,
        ULONG                       ulTableSize
    )
{
    ULONG                           ulHashIndex = 0;
    char*                           lowc_string = (char*)AnscAllocateMemory(ulStringSize * 2);

    if ( !lowc_string )
    {
        return  0;
    }
    else
    {
        AnscStringFromUToL(string, lowc_string, ulStringSize);
    }

    ulHashIndex = AnscHashString(lowc_string, ulStringSize, ulTableSize);

    AnscFreeMemory(lowc_string);

    return  ulHashIndex;
}

