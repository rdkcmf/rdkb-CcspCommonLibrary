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

    module:	kernel_runtime.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file implements the commonly used C runtime
        library functions.

        *   _ansc_itoa
        *   _ansc_ultoa
        *   _ansc_strdup
        *   _ansc_stricmp

    ---------------------------------------------------------------

    environment:

        eCos 1.3.1

    ---------------------------------------------------------------

    author:

        Hua Ding

    ---------------------------------------------------------------

    revision:

        05/01/06    initial revision.

**********************************************************************/


#include "kernel_base.h"
#include "kernel_runtime.h"
#include "kernel_debug.h"


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

                    KernelTrace("_ansc_itoa -- unsupported radix %d!!! *** !!!\n", radix);
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

    pTemp = malloc(ulLen);

    _ansc_strcpy(pTemp, pSrc);

    return pTemp;
}

/**********************************************************************

    caller:     Any APIs using this call.

    prototype:

        int
        _ansc_stricmp
            (
                char*                       pStr1,
                char*                       pStr2
            );

    description:

        This function is called to lexicographically compares lowercase 
        versions of string1 and string2 and returns a value indicating 
        their relationship.


    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pStr1
                string 1 to be compared, Null-terminated string.

                char*                       pStr2
                string 2 to be compared, Null-terminated string.

    return:     Return Value	            Description	
                    < 0	                string1 less than string2	
                    0	                string1 identical to string2	
                    > 0	                string1 greater than string2	

**********************************************************************/

int
_ansc_stricmp
    (
        char*                       pStr1, 
        char*                       pStr2
    )
{
    int                             len1            = _ansc_strlen(pStr1);
    int                             len2            = _ansc_strlen(pStr2);
    int                             len             = (len1 > len2) ? len2 : len1;
    int                             i               = 0;
    int                             result          = 0;

    char                            lowTemp1, lowTemp2;

        
    for ( i = 0; i < len; i ++ )
    {
        if ( (pStr1[i] >= 'A') && (pStr1[i] <= 'Z') )
        {
            lowTemp1 = pStr1[i] - 'A' + 'a';
        }
        else
        {
            lowTemp1 = pStr1[i];
        }

        if ( (pStr2[i] >= 'A') && (pStr2[i] <= 'Z') )
        {
            lowTemp2 = pStr2[i] - 'A' + 'a';
        }
        else
        {
            lowTemp2 = pStr2[i];
        }

        result = lowTemp1 - lowTemp2;

        if ( result != 0 )
        {
            return result;
        }
    }

    result = len1 - len2;

    return  result;
}
