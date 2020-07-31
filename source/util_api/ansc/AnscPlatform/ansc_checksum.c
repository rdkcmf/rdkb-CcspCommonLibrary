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

    module:	ansc_checksum.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        general utility functions related to checksum computation.

        *   AnscReplaceChecksum
        *   AnscCalculateChecksum
        *   AnscCalculateTransportChecksum
        *   AnscGetPackingNumber

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


USHORT
AnscReplaceChecksum
    (
        PVOID                       oldData,
        ULONG                       oldLength,
        PVOID                       newData,
        ULONG                       newLength,
        USHORT                      oldChecksum
    )
{
    PUSHORT                         tempUshort  = 0;
    PUCHAR                          tempUchar   = 0;
    LONG                            newChecksum = 0;

    /*
     * retrieve the shortcut pointer
     */
    tempUshort = (PUSHORT)oldData;
    tempUchar  = (PUCHAR)oldData;

    /*
     * loop to calculate the check sum
     */
    newChecksum = (LONG)oldChecksum;
    newChecksum = ~newChecksum & 0xFFFF;

    while ( oldLength )
    {
        if ( oldLength != 1 )
        {
            newChecksum -= *tempUshort & 0xFFFF;
        }
        else
        {
            newChecksum -= *tempUchar;
        }

        if ( newChecksum <= 0 )
        {
            newChecksum--;
            newChecksum &= 0xFFFF;
        }

        if ( oldLength != 1 )
        {
            tempUshort++;
            tempUchar += 2;
            oldLength -= 2;
        }
        else
        {
            tempUshort++;
            tempUchar += 1;
            oldLength -= 1;
        }
    }

    tempUshort = (PUSHORT)newData;
    tempUchar  = (PUCHAR)newData;

    while ( newLength )
    {
        if ( newLength != 1 )
        {
            newChecksum += *tempUshort & 0xFFFF;
        }
        else
        {
            newChecksum += *tempUchar;
        }

        if ( newChecksum & 0x10000 )
        {
            newChecksum++;
            newChecksum &= 0xFFFF;
        }

        if ( newLength != 1 )
        {
            tempUshort++;
            tempUchar += 2;
            newLength -= 2;
        }
        else
        {
            tempUshort++;
            tempUchar += 1;
            newLength -= 1;
        }
    }

    newChecksum = ~newChecksum & 0xFFFF;

    return  (USHORT)newChecksum;
}


USHORT
AnscCalculateChecksum
    (
        PVOID                       data,
        ULONG                       length
    )
{
    PUSHORT                         tempUshort       = NULL;
    USHORT                          UshortForPadding = 0;
    ULONG                           checksum         = 0;

    /*
     * retrieve the shortcut pointer
     */
    tempUshort = (PUSHORT)data;

    /*
     * loop to calculate the check sum
     */
    while ( length > 1 )
    {
        checksum += *tempUshort;
        tempUshort++;

        /*
         * if high-order bit set, fold
         */
        if ( checksum & 0x80000000 )
        {
            checksum = ( checksum & 0xFFFF ) + ( checksum >> 16 );
        }

        /*
         * modify length
         */
        length -= 2;
    }

    /*
     * take care of left over bytes.
     * note: although it's impossible...
     */
    if ( length )
    {
        UshortForPadding            = 0;
        *(PUCHAR)&UshortForPadding  = *(PUCHAR)tempUshort;
        checksum                   += UshortForPadding;
    }

    /*
     * fold the result checksum
     */
    while ( checksum >> 16 )
    {
        checksum = ( checksum & 0xFFFF ) + ( checksum >> 16 );
    }

    /*
     * return complement of checksum
     */
    return  ~((USHORT)checksum);
}


USHORT
AnscCalculateTransportChecksum
    (
        PVOID                       pseudoHeader,
        ULONG                       headerLength,
        PVOID                       data,
        ULONG                       length
    )
{
    PUSHORT                         tempUshort       = 0;
    USHORT                          UshortForPadding = 0;
    ULONG                           checksum         = 0;

    /*
     * retrieve the shortcut pointer
     */
    tempUshort = (PUSHORT)data;

    /*
     * loop to calculate the check sum
     */
    while ( length > 1 )
    {
        checksum += *tempUshort;
        tempUshort++;

        /*
         * if high-order bit set, fold
         */
        if ( checksum & 0x80000000 )
        {
            checksum = ( checksum & 0xFFFF ) + ( checksum >> 16 );
        }

        /*
         * modify length
         */
        length -= 2;
    }

    /*
     * take care of left over bytes.
     * note: although it's impossible...
     */
    if ( length )
    {
        UshortForPadding            = 0;
        *(PUCHAR)&UshortForPadding  = *(PUCHAR)tempUshort;
        checksum                   += UshortForPadding;
    }

    /*
     * retrieve the shortcut pointer
     */
    tempUshort = (PUSHORT)pseudoHeader;

    /*
     * loop to calculate the check sum
     */
    while ( headerLength > 1 )
    {
        checksum += *tempUshort;
        tempUshort++;

        /*
         * if high-order bit set, fold
         */
        if ( checksum & 0x80000000 )
        {
            checksum = ( checksum & 0xFFFF ) + ( checksum >> 16 );
        }

        /*
         * modify length
         */
        headerLength -= 2;
    }

    /*
     * take care of left over bytes.
     * note: although it's impossible...
     */
    if ( headerLength )
    {
        UshortForPadding            = 0;
        *(PUCHAR)&UshortForPadding  = *(PUCHAR)tempUshort;
        checksum                   += UshortForPadding;
    }

    /*
     * fold the result checksum
     */
    while ( checksum >> 16 )
    {
        checksum = ( checksum & 0xFFFF ) + ( checksum >> 16 );
    }

    /*
     * return complement of checksum
     */
    return  ~((USHORT)checksum);
}


USHORT
AnscGetPackingNumber
    (
        PVOID                       data,
        ULONG                       length
    )
{
    PUSHORT                         tempUshort       = 0;
    USHORT                          UshortForPadding = 0;
    ULONG                           checksum         = 0;

    /*
     * retrieve the shortcut pointer
     */
    tempUshort = (PUSHORT)data;

    /*
     * loop to calculate the check sum
     */
    while ( length > 1 )
    {
        checksum  += AnscReadUshort(tempUshort);
        tempUshort++;

        /*
         * if high-order bit set, fold
         */
        if ( checksum & 0x80000000 )
        {
            checksum = ( checksum & 0xFFFF ) + ( checksum >> 16 );
        }

        /*
         * modify length
         */
        length -= 2;
    }

    /*
     * take care of left over bytes.
     * note: although it's impossible...
     */
    if ( length )
    {
        UshortForPadding            = 0;
        *(PUCHAR)&UshortForPadding  = *(PUCHAR)tempUshort;
        checksum                   += UshortForPadding;
    }

    /*
     * fold the result checksum
     */
    while ( checksum >> 16 )
    {
        checksum = ( checksum & 0xFFFF ) + ( checksum >> 16 );
    }

    return  (USHORT)checksum;
}
