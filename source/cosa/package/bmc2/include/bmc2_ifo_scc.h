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

    module:	bmc2_ifo_scc.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Shell Console Controller (SCC)
        Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/21/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_IFO_SCC_
#define  _BMC2_IFO_SCC_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
     BMC2 SHELL CONSOLE CONTROLLER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BMC2_SCC_INTERFACE_NAME                    "bmc2ShellConsoleControllerIf"
#define  BMC2_SCC_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BMC2SCCIF_SET_INFO)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRootPath,
        char*                       pRootDomainCommand
    );

typedef  ANSC_STATUS
(*PFN_BMC2SCCIF_SET_CONTROL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_BMC2SCCIF_ADD_COMMAND)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDomainNamePattern,
        char*                       pCommand,
        char*                       pScpPage,           /* could be NULL */
        BOOL                        bReadOnly,
        ULONG                       ulPermissions
    );

typedef  ANSC_HANDLE
(*PFN_BMC2SCCIF_ADD_TERMINAL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminalInfo,
        ANSC_HANDLE                 hBmc2IceIf
    );

typedef  ANSC_STATUS
(*PFN_BMC2SCCIF_DEL_TERMINAL)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal
    );

typedef  ANSC_STATUS
(*PFN_BMC2SCCIF_GET_COMMANDS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommandPrefix,     /* could be NULL */
        SLAP_STRING_ARRAY**         ppStringArray
    );

typedef  BOOL
(*PFN_BMC2SCCIF_IS_VALID)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommand,
        PBOOL                       pbAuthorized
    );

typedef  ANSC_STATUS
(*PFN_BMC2SCCIF_EXECUTE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        ANSC_HANDLE                 hCommandReq,
        ANSC_HANDLE                 hCommandRep
    );

typedef  ANSC_STATUS
(*PFN_BMC2SCCIF_ABORT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal
    );

typedef  ANSC_HANDLE
(*PFN_BMC2SCCIF_GET_CMD_SYNTAX)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        char*                       pCommand
    );

/*
 * The BMC2_SCC interface allows the shell program to create and tear down Virtual Terminal and
 * execute commands within the Virtual Terminal. Each Virtual Terminal presents a separated command
 * execution environment. However, all Virtual Terminals share the same instance of the SLAP layer
 * created underneath.
 */
#define  BMC2_SCC_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BMC2SCCIF_SET_INFO          SetRootInfo;                                            \
    PFN_BMC2SCCIF_SET_CONTROL       SetMultiUserCtrl;                                       \
    PFN_BMC2SCCIF_ADD_COMMAND       AddCommand;                                             \
                                                                                            \
    PFN_BMC2SCCIF_ADD_TERMINAL      AddTerminal;                                            \
    PFN_BMC2SCCIF_DEL_TERMINAL      DelTerminal;                                            \
    PFN_BMC2SCCIF_GET_COMMANDS      GetCommands;                                            \
    PFN_BMC2SCCIF_IS_VALID          IsCommandValid;                                         \
    PFN_BMC2SCCIF_EXECUTE           ExecuteCommand;                                         \
    PFN_BMC2SCCIF_ABORT             AbortExecution;                                         \
    PFN_BMC2SCCIF_GET_CMD_SYNTAX    GetCommandSyntax;                                       \
    /* end of object class content */                                                       \

typedef  struct
_BMC2_SCC_INTERFACE
{
    BMC2_SCC_INTERFACE_CLASS_CONTENT
}
BMC2_SCC_INTERFACE,  *PBMC2_SCC_INTERFACE;

#define  ACCESS_BMC2_SCC_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BMC2_SCC_INTERFACE, Linkage)


/***********************************************************
   BMC2 SCC TERMINAL AND COMMAND CONFIGURATION DEFINITION
***********************************************************/

/*
 * To better render the console-based CLI interface, SCP (Shell Command Page) needs to be aware of
 * some of physical attributes of the Virtual Terminal. For example: if the remote user logs over
 * Telnet, the Telnet client and server will negotiate the terminal window size. SCP developers may
 * need to take the window size into consideration when designing the interactive menu and table
 * output format.
 */
typedef  struct
_BMC2_TERMINAL_INFO
{
    char*                           UserIdentifier;
    ULONG                           UserPermission;
    BOOL                            bReadOnly;
    ULONG                           MaxLineNumber;
    ULONG                           MaxColumnNumber;
}
BMC2_TERMINAL_INFO,  *PBMC2_TERMINAL_INFO;

#define  Bmc2CleanTerminalInfo(term_info)                                                   \
         {                                                                                  \
            if ( term_info->UserIdentifier )                                                \
            {                                                                               \
                AnscFreeMemory(term_info->UserIdentifier);                                  \
                                                                                            \
                term_info->UserIdentifier = NULL;                                           \
            }                                                                               \
         }

#define  Bmc2FreeTerminalInfo(term_info)                                                    \
         {                                                                                  \
            Bmc2CleanTerminalInfo(term_info);                                               \
            AnscFreeMemory       (term_info);                                               \
         }

/*
 * In addition to passing in the command line (containing the command name, along with all input
 * arugments), it's probably a good idea to also pass in the current terminal window information.
 * For example, SCP may need to know the current cursor position to determine whether it should
 * clear the screen before rendering the output. Note that it's always recommended that SCP clears
 * the screen if it's going to display an interactive menu.
 */
typedef  struct
_BMC2_COMMAND_REQUEST
{
    char*                           CommandLine;
    ULONG                           CurLineNumber;      /* the line number on which the command was input */
}
BMC2_COMMAND_REQUEST,  *PBMC2_COMMAND_REQUEST;

#define  Bmc2CleanCommandRequest(com_req)                                                   \
         {                                                                                  \
            if ( com_req->CommandLine )                                                     \
            {                                                                               \
                AnscFreeMemory(com_req->CommandLine);                                       \
                                                                                            \
                com_req->CommandLine = NULL;                                                \
            }                                                                               \
         }

#define  Bmc2FreeCommandRequest(com_req)                                                    \
         {                                                                                  \
            Bmc2CleanCommandRequest(com_req);                                               \
            AnscFreeMemory         (com_req);                                               \
         }

#define  BMC2_TERMINAL_STATE_noChange               0
#define  BMC2_TERMINAL_STATE_enterSubMenu           1
#define  BMC2_TERMINAL_STATE_leaveSubMenu           2
#define  BMC2_TERMINAL_STATE_exitToRoot             3
#define  BMC2_TERMINAL_STATE_exitTerminal           4

#define  BMC2_TERMINAL_ERROR_noError                0
#define  BMC2_TERMINAL_ERROR_invalidCommand         1
#define  BMC2_TERMINAL_ERROR_unauthorized           2
#define  BMC2_TERMINAL_ERROR_resources              3
#define  BMC2_TERMINAL_ERROR_pageNotFound           4
#define  BMC2_TERMINAL_ERROR_internalError          5

typedef  struct
_BMC2_COMMAND_REPLY
{
    BOOL                            bStateChanged;
    char*                           MenuTitle;          /* will be embedded as part of the SHELL command prompt string */
    ULONG                           StateCode;          /* transition from parent menu to child menu or vice versa     */
    ULONG                           ErrorCode;          /* only meaningful if ExecuteCommand() doesn't return success  */
    ANSC_HANDLE                     hReserved;          /* used internally, must be NULL when calling ExecuteCommand   */
}
BMC2_COMMAND_REPLY,  *PBMC2_COMMAND_REPLY;

#define  Bmc2CleanCommandReply(com_rep)                                                     \
         {                                                                                  \
            if ( com_rep->MenuTitle )                                                       \
            {                                                                               \
                AnscFreeMemory(com_rep->MenuTitle);                                         \
                                                                                            \
                com_rep->MenuTitle = NULL;                                                  \
            }                                                                               \
         }

#define  Bmc2FreeCommandReply(com_rep)                                                      \
         {                                                                                  \
            Bmc2CleanCommandReply(com_rep);                                                 \
            AnscFreeMemory       (com_rep);                                                 \
         }


#endif
