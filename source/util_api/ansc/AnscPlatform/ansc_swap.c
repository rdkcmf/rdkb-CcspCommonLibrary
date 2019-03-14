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

    module:	ansc_swap.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to memory swapping.

        *   AnscSwapMemory
        *   AnscSwapUlong
        *   AnscSwapUshort

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


void
AnscSwapMemory
    (
        char*                       memory,
        ULONG                       ulSize
    )
{
    char*                           temp = (char*)AnscAllocateMemory(ulSize);
    ULONG                           i    = 0;

    for ( i = 0; i < ulSize; i++ )
    {
        temp[i] = memory[ulSize - 1 - i];
    }

    AnscCopyMemory(memory, temp, ulSize);
    AnscFreeMemory(temp);
}


ULONG
AnscSwapUlong
    (
        ULONG                       ulValue
    )
{
    ULONG                           byte3;
    ULONG                           byte2;
    ULONG                           byte1;
    ULONG                           byte0;

    byte3 = (ulValue & 0xFF000000) >> 24;
    byte2 = (ulValue & 0x00FF0000) >> 8;
    byte1 = (ulValue & 0x0000FF00) << 8;
    byte0 = (ulValue & 0x000000FF) << 24;

    return  byte3 + byte2 + byte1 + byte0;
}


USHORT
AnscSwapUshort(USHORT  usValue)
{
    USHORT                          byte1;
    USHORT                          byte0;

    byte1 = (usValue & 0xFF00) >> 8;
    byte0 = (usValue & 0x00FF) << 8;

    return  byte1 + byte0;
}
