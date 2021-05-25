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

    module:	user_string.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Geng Yang
        Ying Lou

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        09/14/01    adapted to linux kernel by Geng Yang
        04/03/02    adapted to linux user by Ying Lou

**********************************************************************/


#ifndef  _USER_STRING_
#define  _USER_STRING_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#include <string.h>

#define  UserSizeOfString(s)                        (ULONG)(strlen(s))
#define  UserCatString                              strcat

static inline BOOL UserEqualString1 (char *s1, char *s2, BOOL bCaseSensitive)
{
    if ((s1 == NULL) && (s2 == NULL))
        return TRUE;

    if ((s1 == NULL) || (s2 == NULL))
        return FALSE;

    if (bCaseSensitive)
        return (strcmp (s1, s2) == 0) ? TRUE : FALSE;

    return (strcasecmp (s1, s2) == 0) ? TRUE : FALSE;
}

static inline BOOL UserEqualString2 (char *s1, char *s2, ULONG len, BOOL bCaseSensitive)
{
    if ((s1 == NULL) && (s2 == NULL))
        return TRUE;

    if ((s1 == NULL) || (s2 == NULL))
        return FALSE;

    if (bCaseSensitive)
        return (strncmp (s1, s2, len) == 0) ? TRUE : FALSE;

    return (strncasecmp (s1, s2, (unsigned int)len) == 0) ? TRUE : FALSE;
}

__static_inline  void
UserCopyString(char*  destination, char*  source)
{
	if ( !source )
    {
		destination[0] = 0;
    }
	else
    {
        strcpy(destination, source);
    }
}

#define  UserCharInString(s, c)                     (BOOLEAN)(!(!(strchr(s, c))))


#endif

