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

    module:	telnet_definitions.h

        For TELNET,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the TELNET element parameters that can be
        applied to the TELNET Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/09/06    initial revision.

**********************************************************************/


#ifndef  _TELNET_DEFINITIONS_
#define  _TELNET_DEFINITIONS_


/***********************************************************
        TELNET PROTOCOL COMPONENT PARAMETERS DEFINITION
***********************************************************/

#define  TELNET_CHAR_NULL                           '\0'
#define  TELNET_CHAR_BREAK                          ((UCHAR)3  )
#define  TELNET_CHAR_CTRLD                          ((UCHAR)4  )
#define  TELNET_CHAR_LF                             '\n'
#define  TELNET_CHAR_CR                             '\r'
#define  TELNET_CHAR_BELL                           '\a'
#define  TELNET_CHAR_BACKSPACE                      ((UCHAR)8  )
#define  TELNET_CHAR_HTAB                           '\t'
#define  TELNET_CHAR_VTAB                           ((UCHAR)11 )
#define  TELNET_CHAR_FORMFEED                       ((UCHAR)12 )
#define  TELNET_CHAR_ESC                            ((UCHAR)27 )
#define  TELNET_CHAR_SPACE                          ' '
#define  TELNET_CHAR_SEMICOLON                      ';'
#define  TELNET_CHAR_SBRACKET                       '['
#define  TELNET_CHAR_TILDA                          '~'
#define  TELNET_CHAR_DEL                            ((UCHAR)127)

#define  TELNET_CMD_CODE_SE                         ((UCHAR)240)
#define  TELNET_CMD_CODE_NOP                        ((UCHAR)241)
#define  TELNET_CMD_CODE_DataMark                   ((UCHAR)242)
#define  TELNET_CMD_CODE_Break                      ((UCHAR)243)
#define  TELNET_CMD_CODE_InterruptProcess           ((UCHAR)244)
#define  TELNET_CMD_CODE_AbortOutput                ((UCHAR)245)
#define  TELNET_CMD_CODE_AreYouThere                ((UCHAR)246)
#define  TELNET_CMD_CODE_EraseChar                  ((UCHAR)247)
#define  TELNET_CMD_CODE_EraseLine                  ((UCHAR)248)
#define  TELNET_CMD_CODE_GoAhead                    ((UCHAR)249)
#define  TELNET_CMD_CODE_SB                         ((UCHAR)250)
#define  TELNET_CMD_CODE_Will                       ((UCHAR)251)
#define  TELNET_CMD_CODE_Wont                       ((UCHAR)252)
#define  TELNET_CMD_CODE_Do                         ((UCHAR)253)
#define  TELNET_CMD_CODE_Dont                       ((UCHAR)254)
#define  TELNET_CMD_CODE_Iac                        ((UCHAR)255)

/* supported TELNET option code */
#define  TELNET_OPTION_CODE_Echo                    1
#define  TELNET_OPTION_CODE_SuppressGA              3
#define  TELNET_OPTION_CODE_TerminalType            24
#define  TELNET_OPTION_CODE_WindowSize              31

/* SubOption code */
#define  TELNET_SUBOPTION_TERMINALTYPE_Is           0
#define  TELNET_SUBOPTION_TERMINALTYPE_Send         1


#define  TELNET_SUBOPTION_TERMINALTYPE_ansi         "ansi"
#define  TELNET_SUBOPTION_TERMINALTYPE_vt100        "vt100"
#define  TELNET_SUBOPTION_TERMINALTYPE_vt102        "vt102"
#define  TELNET_SUBOPTION_TERMINALTYPE_xterm        "xterm"


#define  TELNET_SRV_DEFAULT_PORT                    23

/* some ANSI/VT100 erase codes */
#define  TELNET_ANSI_TTY_CODE_Erase_ToEnd           "\033[K"
#define  TELNET_ANSI_TTY_CODE_ClearScreen           "\033[H\033[J"

/* ANSI/VT100 console cursor control codes */
#define  TELNET_ANSI_TTY_CODE_Cursor_Up             "\033[A"
#define  TELNET_ANSI_TTY_CODE_Cursor_Down           "\033[B"
#define  TELNET_ANSI_TTY_CODE_Cursor_Forward        "\033[C"
#define  TELNET_ANSI_TTY_CODE_Cursor_Backward       "\033[D"

#define  TELNET_ANSI_TTY_CODE_ANSI_Cursor_Home      "\033[1~"
#define  TELNET_ANSI_TTY_CODE_ANSI_Cursor_End       "\033[4~"
#define  TELNET_ANSI_TTY_CODE_ANSI_Cursor_Pgup      "\033[5~"
#define  TELNET_ANSI_TTY_CODE_ANSI_Cursor_Pgdn      "\033[6~"

#define  TELNET_ANSI_TTY_CODE_VT100_Cursor_Home     "\033[2~"
#define  TELNET_ANSI_TTY_CODE_VT100_Cursor_End      "\033[5~"
#define  TELNET_ANSI_TTY_CODE_VT100_Cursor_Pgup     "\033[3~"
#define  TELNET_ANSI_TTY_CODE_VT100_Cursor_Pgdn     "\033[6~"
#define  TELNET_ANSI_TTY_CODE_VT100_Cursor_Del      "\033[4~"

#define  TELNET_ANSI_TTY_CODE_VT102_Cursor_Home     "\033OH"
#define  TELNET_ANSI_TTY_CODE_VT102_Cursor_End      "\033OF"
#define  TELNET_ANSI_TTY_CODE_VT102_Cursor_Pgup     "\033[5~"
#define  TELNET_ANSI_TTY_CODE_VT102_Cursor_Pgdn     "\033[6~"
#define  TELNET_ANSI_TTY_CODE_VT102_Cursor_Del      "\033[3~"

#define  TELNET_ANSI_TTY_CODE_Delete_Char           "\033[1P"

#define  TELNET_ANSI_TTY_CODE_Cursor_SetPos         "\033[%d;%df"
#define  TELNET_ANSI_TTY_CODE_Cursor_GetPos         "\033[6n"

#define  TELNET_ANSI_TTY_CODE_Cursor_Save           "\033[s"
#define  TELNET_ANSI_TTY_CODE_Cursor_Restore        "\033[u"

#define  TELNET_ANSI_TTY_CODE_Crlf                  "\r\n"


#endif

