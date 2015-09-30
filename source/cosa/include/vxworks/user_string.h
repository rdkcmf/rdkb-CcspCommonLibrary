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

    copyright:

        Cisco System  , Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        VxWorks 5.4

    ---------------------------------------------------------------

    author:

        Huaxiang Sun
        Hua Ding

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.

**********************************************************************/


#ifndef  _USER_STRING_
#define  _USER_STRING_


/***********************************************************
        DEFINITION OF BASIC DATA TYPE AND STRUCTURES
***********************************************************/


/***********************************************************
       BASIC OPERATIONS BY MACROS AND INLINE FUNCTIONS
***********************************************************/

#define  UserSizeOfString(s)                        (ULONG)(strlen(s))
#define  UserCatString                              strcat

BOOLEAN
UserEqualString1
    (
        char*                       pString1,
        char*                       pString2,
        BOOLEAN                     bCaseSensitive
    );


BOOLEAN
UserEqualString2
    (
        char*                       pString1,
        char*                       pString2,
        ULONG                       length,
        BOOLEAN                     bCaseSensitive
    );


/*
 *  prototype
 *
VOID
UserCopyString
    (
        char*                       pDest,
        char*                       pSrc
    );
 */
#define  UserCopyString(pDest, pSrc)                strcpy(pDest, pSrc)


/*
 *  prototype
 *
BOOLEAN
UserCharInString
    (
        char*                       pString,
        char                        charToFind
    );
 */
#define  UserCharInString(pString, charToFind)      (strchr(pString, charToFind) != NULL)

#endif
