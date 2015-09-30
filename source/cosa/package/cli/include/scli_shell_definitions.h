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

    module:	scli_shell_co_type.h

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2005
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object types for the SCLI
        Shell Component Objects.

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


#ifndef  _SCLI_SHELL_DEFINITIONS_
#define  _SCLI_SHELL_DEFINITIONS_

#include "bmc2_definitions.h"
#include "cli_module_custom.h"

#define  SCLI_SHELL_DEF_GREETING_MSG                "SCLI device administration console.\r\n"
#define  SCLI_SHELL_DEF_BYE_MSG                     "logout\r\n"

#define  SCLI_SHELL_LOGIN                           "Login: "
#define  SCLI_SHELL_PASSWORD                        "Password: "

#define  SCLI_SHELL_PRIVILEGED_SIGN                 '#'
#define  SCLI_SHELL_NORMAL_SIGN                     '>'
#define  SCLI_SHELL_HELP_CHAR                       '?'
#define  SCLI_SHELL_CMD_TOKEN_SEPARATOR             ' '
#define  SCLI_SHELL_CMD_VLIST_SEPARATOR             ','

#define  SCLI_SHELL_INVALID_CMD                     "%% invalid command"
#define  SCLI_SHELL_NO_PERMISSION                   "%% no permission for this command"
#define  SCLI_SHELL_INTERNAL_ERROR                  "%% Internal error (code = %d)!"
#define  SCLI_SHELL_INVALID_ARG                     "%% invalid or incomplete argument"
#define  SCLI_SHELL_OPTION_CR                       "<cr>"
#define  SCLI_SHELL_OPTION_CR_DESC                  "Press ENTER to issue the command"
#define  SCLI_SHELL_NO_HELP_FOUND                   "%% command syntax not found or internal error happened"

#define  SCLI_SHELL_AUTH_FAILURE                    "Login incorrect!\r\n"
#define  SCLI_SHELL_LOCKOUT                         "You are locked out due to too many authentication attemps."

#define  SCLI_SHELL_OUTPUT_INDENT                   "    "
#define  SCLI_SHELL_OUTPUT_ERROR                    "^"

/* SCLI TSP built-in commands */
#define  SCLI_SHELL_BICMD_Exit                      "exit"
#define  SCLI_SHELL_BICMD_Clear                     "clear"

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
#define  SCLI_SHELL_BICMD_Shell                     "shell"
#endif

#define  SCLI_SYSTEM_SHELL_TITLE                    "<sh> "
#define  SCLI_SYSTEM_SHELL_EXIT_PROMPT              "Enter 'exit' to go back to CLI"

#ifdef   _ANSC_LINUX
#define  SCLI_LOCAL_SHELL_CMD                       "/bin/sh"
#define  SCLI_LOCAL_TEMP_FILE_TEMPLATE              "/tmp/sh_cout_%u"
#elif defined(_ANSC_WINDOWSNT)
#define  SCLI_LOCAL_SHELL_CMD                       "command"
#define  SCLI_LOCAL_TEMP_FILE_TEMPLATE              "c:/temp/sh_cout_%u"
#endif

#define  SCLI_SHELL_TEXTBOX_LEFT                    "["
#define  SCLI_SHELL_TEXTBOX_RIGHT                   "]"


/* codes of SCLI TSP built-in commands */
#define  SCLI_SHELL_BICODE_Unrecognized             0
#define  SCLI_SHELL_BICODE_Exit                     1
#define  SCLI_SHELL_BICODE_Clear                    2
#define  SCLI_SHELL_BICODE_PrevCmd                  3
#define  SCLI_SHELL_BICODE_NextCmd                  4
#define  SCLI_SHELL_BICODE_CursorBackward           5
#define  SCLI_SHELL_BICODE_CursorForward            6
#define  SCLI_SHELL_BICODE_CursorHome               7
#define  SCLI_SHELL_BICODE_CursorEnd                8
#define  SCLI_SHELL_BICODE_FirstCmd                 9
#define  SCLI_SHELL_BICODE_LastCmd                  10
#define  SCLI_SHELL_BICODE_Backspace                11
#define  SCLI_SHELL_BICODE_Del                      12
#define  SCLI_SHELL_BICODE_Break                    13
#define  SCLI_SHELL_BICODE_AutoCompletion           14
#define  SCLI_SHELL_BICODE_CursorPosReport          15
#define  SCLI_SHELL_BICODE_MoveToNextTextBox        16
#define  SCLI_SHELL_BICODE_MoveToPrevTextBox        17
#define  SCLI_SHELL_BICODE_Help                     18

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
#define  SCLI_SHELL_BICODE_Shell                    19
#endif

#define  SCLI_SHELL_TYPE_Unspecified                0
#define  SCLI_SHELL_TYPE_Local                      1
#define  SCLI_SHELL_TYPE_Telnet                     2
#define  SCLI_SHELL_TYPE_Ssh                        3


typedef  struct
_SCLI_SHELL_CMD_TOKEN
{
    char*                           pValue;             /* value of the token */
    ULONG                           ulStartPos;         /* starting position of the token - the position in user's input */
}
SCLI_SHELL_CMD_TOKEN, *PSCLI_SHELL_CMD_TOKEN;


typedef  struct
_SCLI_SHELL_CMD_TLIST
{
    PSCLI_SHELL_CMD_TOKEN           pTokens;            /* token list */
    ULONG                           ulTokenCount;       /* number of token in the list */
    BOOL                            bEndWithSpace;      /* indicates if a space is the last char of the user input - excluding HTAB or question mark */

    ULONG                           ulCurToken;         /* used internal for matching user input against command syntax */
}
SCLI_SHELL_CMD_TLIST, *PSCLI_SHELL_CMD_TLIST;


typedef  struct
_SCLI_SHELL_ARG_HELP_LIST
{
    PBMC2_CMD_ARG_HELP              pArgHelps;
    ULONG                           ulArgHelpCount;

    ULONG                           ulMaxItemLen;
}
SCLI_SHELL_ARG_HELP_LIST, *PSCLI_SHELL_ARG_HELP_LIST;

#define  SCLI_SHELL_FREE_ARG_HELP_LIST(pHelpList)                   \
    if ( pHelpList )                                                \
    {                                                               \
        ULONG                       i;                              \
        PBMC2_CMD_ARG_HELP          pHelp;                          \
        for (i = 0; i < (pHelpList)->ulArgHelpCount; i ++ )         \
        {                                                           \
            pHelp = &((pHelpList)->pArgHelps[i]);                   \
            if ( pHelp->pItem)                                      \
                AnscFreeMemory(pHelp->pItem);                       \
            if ( pHelp->pDescription )                              \
                AnscFreeMemory(pHelp->pDescription);                \
        }                                                           \
        AnscFreeMemory((pHelpList)->pArgHelps);                     \
        AnscFreeMemory(pHelpList);                                  \
    }

typedef  struct
_SCLI_CMD_ARG_MATCH_RESULT
{
    BOOL                            bMatched;               /* if or not an argument matched */
    BOOL                            bNameMatched;           /* TRUE when user only inputs the name part of a named argument */
    BOOL                            bNamePartialMatched;    /* TRUE when name part of an argument partially matches user input */

    BOOL                            bValueMatched;          /* indicates value matches - data type matches and value(s) is(are) in range */
    BOOL                            bPartialValue;          /* user is typing value of an argument, but not complete yet */
    BOOL                            bMultiMatched;          /* indicates multiple options match argument value, it is normally for string type */

    int                             nMatchStart;            /* matching start position in the command */
    int                             nMatchEnd;              /* matching end position in the command */
    int                             nMatchTokenStart;       /* token matching starts */
    int                             nMatchTokenEnd;         /* token matching ends */

    int                             nErrorPos;              /* where error detected, (ULONG)-1 means no error or not determined yet */
    BOOL                            bTried;                 /* indicates this argment just being tried, used to revert partial match etc. */

    BOOL                            bNamedArg;              /* indicates if matched argument is named or not */
}
SCLI_CMD_ARG_MATCH_RESULT, *PSCLI_CMD_ARG_MATCH_RESULT;


typedef  struct
_SCLI_SHELL_ARG_HELP_MATCH
{
    ULONG                           ulMatchCount;
    PBOOL                           pMatches;
    PULONG                          pArgTypes;
    PBMC2_CMD_SIMPLE_ARG*           pSimpleArgs;
    PULONG                          pHelpCounts;
}
SCLI_SHELL_ARG_HELP_MATCH, *PSCLI_SHELL_ARG_HELP_MATCH;

#define  SCLI_SHELL_FREE_ARG_HELP_MATCH(pHelpMatch)                 \
    if ( pHelpMatch )                                               \
    {                                                               \
        if ( (pHelpMatch)->pMatches )                               \
            AnscFreeMemory((pHelpMatch)->pMatches);                 \
        if ( (pHelpMatch)->pArgTypes )                              \
            AnscFreeMemory((pHelpMatch)->pArgTypes);                \
        if ( (pHelpMatch)->pSimpleArgs )                            \
            AnscFreeMemory((pHelpMatch)->pSimpleArgs);              \
        if ( (pHelpMatch)->pHelpCounts )                            \
            AnscFreeMemory((pHelpMatch)->pHelpCounts);              \
        AnscFreeMemory(pHelpMatch);                                 \
    }

#define  SCLI_SHELL_SKIP_SPACE(pCmdStart)                           \
    while ( *pCmdStart != 0 && *pCmdStart == ' ' )                  \
    {                                                               \
        pCmdStart ++;                                               \
    }


#define  SCLI_SHELL_STRING_DIFF_AT(pStr1, pStr2, nErrorPos)         \
    do {                                                            \
            (nErrorPos) = 0;                                        \
            while ( TRUE )                                          \
            {                                                       \
                if ( ((pStr1)[nErrorPos] == 0)  ||                  \
                     ((pStr2)[nErrorPos] == 0)  ||                  \
                     ((pStr1)[nErrorPos] != (pStr2)[nErrorPos]) )   \
                {                                                   \
                    char            c1, c2;                         \
                                                                    \
                    c1 = (pStr1)[nErrorPos];                        \
                    c2 = (pStr2)[nErrorPos];                        \
                                                                    \
                    if ( c1 <= 'Z' && c1 >= 'A' )                   \
                        c1 = c1 - 'A' + 'a';                        \
                                                                    \
                    if ( c2 <= 'Z' && c2 >= 'A' )                   \
                        c2 = c2 - 'A' + 'a';                        \
                                                                    \
                    if ( c1 != c2 )                                 \
                        break;                                      \
                }                                                   \
                (nErrorPos) ++;                                     \
            }                                                       \
    } while (0)


#endif
