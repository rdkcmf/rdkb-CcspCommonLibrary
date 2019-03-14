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

    module:	scli_shell_properties.h

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Simple CLI Shell
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/10/05    initial revision.

**********************************************************************/


#ifndef  _SCLI_SHELL_PROPERTIES_
#define  _SCLI_SHELL_PROPERTIES_

#define  SCLI_SHELL_GREETING_LEN                    256
#define  SCLI_SHELL_PROMPT_LEN                      16

#define  SCLI_SHELL_CACHED_CMD_DEF_COUNT            64                  /* default cap of command caching */
#define  SCLI_SHELL_CACHED_CMD_MIN_COUNT            8                   /* hard cap of command caching */
#define  SCLI_SHELL_CACHED_CMD_MAX_COUNT            256                 /* hard cap of command caching */

#define  SCLI_SHELL_RUN_MODE_ASYNC                  0x00000001

typedef  struct 
_SCLI_SHELL_PROPERTY
{
    ULONG                           Mode;                               /* synchronous or asynchronous - obsolete */
    BOOL                            bUserAuth;                          /* user authentication flag */
    ULONG                           CmdCacheMaxCount;                   /* maximum entries of command cache */
    UCHAR                           Greeting[SCLI_SHELL_GREETING_LEN];  /* greeting message */
    UCHAR                           ShellPrompt[SCLI_SHELL_PROMPT_LEN]; /* shell prompt */
    ULONG                           ShellType;                          /* shell type */

    /* TermPermssion is provided by user when user authentication is bypassed */
    ULONG                           TermPermission;
}
SCLI_SHELL_PROPERTY, *PSCLI_SHELL_PROPERTY;


/*
#define  SCLI_SHELL_CMD_COMPLETE(pCmd, nCmdLen, bComplete, bDM)     \
    do                                                              \
    {                                                               \
        bComplete   = FALSE;                                        \
        bDM         = FALSE;                                        \
                                                                    \
        if ( nCmdLen >= 1 )                                         \
        {                                                           \
            PUCHAR                  pLast;                          \
            pLast   = (pCmd) + nCmdLen - 1;                         \
            bComplete   =                                           \
                ( *(pLast) == TELNET_CHAR_CR ||                     \
                  *(pLast) == TELNET_CHAR_LF ||                     \
                  *(pLast) == TELNET_CHAR_NULL );                   \
                                                                    \
            bDM = ( *(pLast) == TELNET_CHAR_BREAK );                \
        }                                                           \
    }                                                               \
    while (0)
*/

#define  SCLI_SHELL_BUFCMD_AVAILABLE(pCmd, nCmdLen, bComplete, bDM) \
    do                                                              \
    {                                                               \
        bComplete   = FALSE;                                        \
        bDM         = FALSE;                                        \
                                                                    \
        if ( nCmdLen >= 1 )                                         \
        {                                                           \
            PUCHAR                  pCr = NULL;                     \
            bDM = ( *(pCmd) == TELNET_CHAR_BREAK );                 \
            pCr = _ansc_memchr(pCmd, TELNET_CHAR_CR, nCmdLen);      \
            bComplete   = (pCr != NULL);                            \
        }                                                           \
    }                                                               \
    while (0)

#define  SCLI_SHELL_CEN_EVENT_CmdBegin              1
#define  SCLI_SHELL_CEN_EVENT_CmdEnd                2


#endif

