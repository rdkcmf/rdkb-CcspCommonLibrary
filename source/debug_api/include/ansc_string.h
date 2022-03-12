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

    module: ansc_string.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to string operation. Instead
        of defining everything in a single file, we simply route
        every function and macro to the corresponding platform-
        dependent prototype definition.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        01/01/01    initial revision.
        01/19/01    restructure the container object, add and
                    remove a few things
        04/01/01    add user mode wrapper functions

**********************************************************************/

#ifndef  _ANSC_STRING_
#define  _ANSC_STRING_

#include <string.h>

static inline size_t AnscSizeOfString (const char *s)
{
    return strlen(s);
}

static inline void AnscCopyString (char *d, const char *s)
{
    if (!s)
        *d = 0;
    else
        strcpy (d,s);
}

static inline BOOL AnscEqualString1 (char *s1, char *s2, BOOL bCaseSensitive)
{
    if ((s1 == NULL) && (s2 == NULL))
        return TRUE;

    if ((s1 == NULL) || (s2 == NULL))
        return FALSE;

    if (bCaseSensitive)
        return (strcmp (s1, s2) == 0) ? TRUE : FALSE;

    return (strcasecmp (s1, s2) == 0) ? TRUE : FALSE;
}

#define AnscEqualString AnscEqualString1

static inline BOOL AnscEqualString2 (char *s1, char *s2, size_t len, BOOL bCaseSensitive)
{
    if ((s1 == NULL) && (s2 == NULL))
        return TRUE;

    if ((s1 == NULL) || (s2 == NULL))
        return FALSE;

    if (bCaseSensitive)
        return (strncmp (s1, s2, len) == 0) ? TRUE : FALSE;

    return (strncasecmp (s1, s2, len) == 0) ? TRUE : FALSE;
}

static inline char *AnscCatString (char *dest, const char *src)
{
    return strcat (dest, src);
}

static inline int AnscCharInString (const char *s, int c)
{
    return (strchr(s, c)) ? 1 : 0;
}


/***********************************************************
         TEXT-MESSAGE MANIPULATION MACRO DEFINITIONS
***********************************************************/

/*
 * Following data type and macro definitions are used to facilitate the processing of text-based
 * protocol messages, such as HTTP, SIP, RTSP, SMTP and so forth.
 */
#define  ANSC_CARRIAGE_RETURN                       0x0D
#define  ANSC_LINE_FEED                             0x0A
#define  ANSC_HORIZONTAL_TAB                        0x09
#define  ANSC_SPACE                                 0x20
#define  ANSC_DOUBLE_QUOTE                          0x22
#define  ANSC_COLON                                 (UCHAR)':'

#define  AnscIsUpAlpha(single_char)                 ( (single_char >= 'A') && (single_char <= 'Z') )
#define  AnscIsLoAlpha(single_char)                 ( (single_char >= 'a') && (single_char <= 'z') )
#define  AnscIsUpHexAlpha(single_char)              ( (single_char >= 'A') && (single_char <= 'F') )
#define  AnscIsLoHexAlpha(single_char)              ( (single_char >= 'a') && (single_char <= 'f') )
#define  AnscIsAlpha(single_char)                   ( AnscIsUpAlpha(single_char) || AnscIsLoAlpha(single_char) )
#define  AnscIsHexAlpha(single_char)                ( AnscIsUpHexAlpha(single_char) || AnscIsLoHexAlpha(single_char) )
#define  AnscIsDigit(single_char)                   ( (single_char >= '0') && (single_char <= '9') )
#define  AnscIsAlphaOrDigit(single_char)            ( AnscIsAlpha(single_char) || AnscIsDigit(single_char) )
#define  AnscIsHexAlphaOrDigit(single_char)         ( AnscIsHexAlpha(single_char) || AnscIsDigit(single_char) )

#endif
