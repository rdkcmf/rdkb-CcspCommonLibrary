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

    module:	user_runtime.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the commonly used C runtime
        library functions.

        *   _ansc_itoa
        *   _ansc_ultoa
        *   _ansc_strdup

    ---------------------------------------------------------------

    environment:

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        04/25/02    initial revision.

**********************************************************************/

#include "ansc_global.h"
#include "user_base.h"
#include "user_runtime.h"
#include "user_debug.h"
#include "user_string.h"

#include <signal.h>
#include <dirent.h>



char*
_ansc_itoa
    (
        int                         value,
        char*                       string,
        int                         radix
    )
{
    if ( string )
    {
        switch ( radix )
        {
            case    8:

                    sprintf(string, "%o", value);
                    break;

            case    10:

                    sprintf(string, "%d", value);
                    break;

            case    16:

                    sprintf(string, "%x", value);
                    break;

            default:

                    UserTrace("_ansc_itoa -- unsupported radix %d!!! *** !!!\n", radix);
        }
    }

    return string;
}


char*
_ansc_ultoa
    (
        unsigned int                value,
        char*                       pBuf,
        int                         radix
    )
{

    ULONG                           counter = 0;
    ULONG                           result  = value;
    ULONG                           length  = 0;

    if( pBuf == NULL)
    {
        return NULL;
    }

    while ( result )
    {
        result = result / radix;
        counter++;
    }

    length = counter;
    result = value;

    if ( length != 0 )
    {
        for ( counter = 0; counter < length; counter++ )
        {
            pBuf[length - 1 - counter] = (char)(result % radix) + '0';
            result                     = result / radix;
        }

        pBuf[length] = 0;
    }
    else
    {
        pBuf[0]  = '0';
        pBuf[1]  = 0;
        length   = 1;
    }

    return  pBuf;
}


char*
_ansc_strdup
    (
        char*                       pSrc
    )
{
    char*                           pTemp;
    ULONG                           ulLen;

    ulLen = _ansc_strlen(pSrc) + 1;

    pTemp = AnscAllocateMemory(ulLen);

    _ansc_strcpy(pTemp, pSrc);

    return pTemp;
}

