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

        Cisco System  , Inc., 1997 ~ 2006
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the commonly used C runtime
        library functions.

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

    #define  _ansc_min                              min
    #define  _ansc_max                              max

    #define  _ansc_abs                              abs
    #define  _ansc_atof                             atof
    #define  _ansc_atoi                             atoi
    #define  _ansc_atol                             atol

    char*	 _ansc_itoa(int value, char* string, int radix);

    char*    _ansc_ultoa(unsigned int value, char* pBuf, int radix);

    #define  _ansc_rand                             rand
    #define  _ansc_srand                            srand

    /*
    #define  _ansc_memccpy                          _memccpy
     */
    #define  _ansc_memchr                           memchr
    /*
    #define  _ansc_memicmp                          _memicmp
     */
    #define  _ansc_memicmp                          memcmp
    #define  _ansc_memcmp                           memcmp
    #define  _ansc_memcpy                           memcpy
    #define  _ansc_memset                           memset
    #define  _ansc_memmove                          memmove

    /*
    #define  _ansc_strset                           _strset
     */
    #define  _ansc_strcpy                           strcpy
    #define  _ansc_strcat                           strcat
    #define  _ansc_strcmp                           strcmp
    #define  _ansc_strncmp                          strncmp
    #define  _ansc_strlen                           strlen

    #define  _ansc_strchr                           strchr
    /*
    #define  _ansc_strcmpi                          _strcmpi
    #define  _ansc_stricmp                          _stricmp
     */

    int     _ansc_stricmp(char* pStr1, char* pStr2);

    #define  _ansc_strcoll                          strcoll
    #define  _ansc_strcspn                          strcspn

    char*    _ansc_strdup(char* pSrc);

    #define  _ansc_strspn                           strspn
    #define  _ansc_strstr                           strstr
    #define  _ansc_strtok                           strtok

    #define  _ansc_sscanf                           sscanf
    #define  _ansc_sprintf                          sprintf
    #define  _ansc_vsprintf                         vsprintf

#endif


#endif
