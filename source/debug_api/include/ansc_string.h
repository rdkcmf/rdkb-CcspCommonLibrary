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

    module:	ansc_string.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

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

#include "user_string.h"

/***********************************************************
            FUNCTION AND MACRO PROTOTYPE ROUTING
***********************************************************/

/*
 * According to the current platform definition, we route the definition to the corresponding
 * header files.
 */
#ifdef   _ANSC_KERNEL

    #define  AnscSizeOfString                       KernelSizeOfString
    #define  AnscCopyString                         KernelCopyString
    /*#define  AnscEqualString1                       KernelEqualString1*/
    /*#define  AnscEqualString2                       KernelEqualString2*/
    #define  AnscEqualString                        AnscEqualString1
    #define  AnscCatString                          KernelCatString
    #define  AnscCharInString                       KernelCharInString

#else

    #define  AnscSizeOfString                       UserSizeOfString
    #define  AnscCopyString                         UserCopyString
    /*#define  AnscEqualString1                       UserEqualString1*/
    /*#define  AnscEqualString2                       UserEqualString2*/
    #define  AnscEqualString                        AnscEqualString1
    #define  AnscCatString                          UserCatString
    #define  AnscCharInString                       UserCharInString

#endif


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

#define  AnscIsCr(single_char)                      ( (UCHAR)single_char == ANSC_CARRIAGE_RETURN )
#define  AnscIsLf(single_char)                      ( (UCHAR)single_char == ANSC_LINE_FEED       )
#define  AnscIsHt(single_char)                      ( (UCHAR)single_char == ANSC_HORIZONTAL_TAB  )
#define  AnscIsSp(single_char)                      ( (UCHAR)single_char == ANSC_SPACE           )
#define  AnscIsDq(single_char)                      ( (UCHAR)single_char == ANSC_DOUBLE_QUOTE    )
#define  AnscIsCl(single_char)                      ( (UCHAR)single_char == ANSC_COLON           )

#define  AnscIsLws(single_char)                     ( AnscIsHt(single_char) || AnscIsSp(single_char) )

#define  AnscIsEol(char_pointer)                                                            \
         ( AnscIsLf(*char_pointer) || (AnscIsCr(*char_pointer) && AnscIsLf(*(char_pointer + 1))) )

#define  AnscIsDigitString(tbe_string, b_result)                                            \
         {                                                                                  \
            ULONG                   ii = 0;                                                 \
                                                                                            \
            b_result = TRUE;                                                                \
                                                                                            \
            for ( ii = 0; ii < AnscSizeOfString(tbe_string); ii++ )                         \
            {                                                                               \
                if ( !AnscIsDigit(tbe_string[ii]) )                                         \
                {                                                                           \
                    b_result = FALSE;                                                       \
                                                                                            \
                    break;                                                                  \
                }                                                                           \
            }                                                                               \
         }


#endif
