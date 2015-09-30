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

    module:	kernel_runtime.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the commonly used C runtime
        library functions.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/25/02    initial revision.

**********************************************************************/


#ifndef  _KERNEL_RUNTIME_
#define  _KERNEL_RUNTIME_


/***********************************************************
    PLATFORM DEPENDENT DATA TYPE AND MACRO DEFINITIONS
***********************************************************/

/*
 * We provide a platform-independent wrapper for all the C runtime library functions. However,
 * since the original C library functions are pretty stupid and ugly, instead of wrapping around
 * the runtime functions with better names, we simplify the interface a little bit.
 */
#ifndef  _ANSC_RUNTIME_LIBRARY_

    #define  _ansc_min(x,y)                         ((x)<(y)?(x):(y))
    #define  _ansc_max(x,y)                         ((x)>(y)?(x):(y))

    #define  _ansc_abs                              abs
    #define  _ansc_atof                             atof
    #define  _ansc_atoi                             atoi
    #define  _ansc_atol                             atol
    #define  _ansc_itoa                             _itoa
    #define  _ansc_ultoa                            ultoa
    #define  _ansc_rand                             rand
    #define  _ansc_srand                            srand

    #define  _ansc_memccpy                          _memccpy
    #define  _ansc_memchr                           memchr
    #define  _ansc_memicmp                          _memicmp

    #define  _ansc_memcmp                           memcmp
    #define  _ansc_memcpy                           memcpy
    #define  _ansc_memset                           memset

    #define  _ansc_strset                           _strset
    #define  _ansc_strcpy                           strcpy
    #define  _ansc_strncpy                          strncpy
    #define  _ansc_strcat                           strcat
    #define  _ansc_strcmp                           strcmp
    #define  _ansc_strncmp                          strncmp
    #define  _ansc_strlen                           strlen

    #define  _ansc_strchr                           strchr
    #define  _ansc_strcmpi                          _strcmpi
    #define  _ansc_stricmp                          stricmp
    #define  _ansc_strcoll                          strcoll
    #define  _ansc_strcspn                          strcspn
    #define  _ansc_strdup                           _strdup
    #define  _ansc_strspn                           strspn
    #define  _ansc_strstr                           strstr
    #define  _ansc_strtok                           strtok

    #define  _ansc_sprintf                          sprintf
    #define  _ansc_snprintf                         snprintf
    #define  _ansc_vsprintf                         vsprintf
    #define  _ansc_sscanf                           sscanf

    #define  atol(x)                                simple_strtoul(x,NULL,0)
    #define  _memicmp                               strnicmp

    static __inline__ char*
    _itoa
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

                        printk("_itoa -- unsupported radix %d!!! *** !!!\n", radix);
            }
        }

        return string;
    }

    static __inline__ char*
    ultoa
        (
            unsigned long               value,
            char*                       pBuf,
            int                         radix
        )
    {

        int                             counter = 0;
        unsigned long                   result  = value;
        unsigned long                   length  = 0;

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


    static __inline__ char*
    _strdup
        (
            char*                       pSrc
        )
    {
        char*                           pTemp;
        unsigned long                   ulLen;

        ulLen = strlen(pSrc) + 1;

        pTemp = kmalloc(ulLen, GFP_ATOMIC);

        strcpy(pTemp, pSrc);

        return pTemp;
    }

#ifndef _ANSC_LINUX_2_6_
    static __inline__ int
    strcspn(char *s1, char *s2)
    {
        const char *p1, *p2;
        char       c;

        if (!s1)
            return -1;

        c = *s1;

        for (p1 = s1; ;c = *++p1)
        {
            p2 = s2;
            do
            {
                if (*p2 == c || !c )
                {
                    return p1 - s1;
                }
            } while (*p2++);
        }
     }
#endif

    static __inline__ int
    stricmp(const char *s1, const char *s2)
    {
        int c1, c2;

        do {
            c1 = tolower(*s1++);
            c2 = tolower(*s2++);
        } while (c1 == c2 && c1 != 0);
        return c1 - c2;
    }

    /*
     *  Linux kernel has memchr
     *
    static __inline__ void *memchr(const void *s, int c, size_t n)
    {
        const unsigned char *p = s;
        while (n-- != 0) {
                if ((unsigned char)c == *p++) {
                return (void *)(p-1);
            }
        }
        return NULL;
    }
     */

#endif

#endif
