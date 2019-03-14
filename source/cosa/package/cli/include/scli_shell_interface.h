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

    module:	scli_shell_interface.h

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


#ifndef  _SCLI_SHELL_INTERFACE_
#define  _SCLI_SHELL_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */

#include "slap_definitions.h"
#include "bmc2_ifo_ice.h"
#include "bmc2_ifo_scc.h"

#include "telnet_ifo_tsp.h"


/***********************************************************
   PLATFORM INDEPENDENT SCLI TSP OBJECT DEFINITION
***********************************************************/

#define  SCLI_SHELL_MAX_COMMAND_LEN                 512

#define  SCLI_SHELL_USERNAME_LEN                    32 + 1

#define  SCLI_SHELL_STATE_INITIATED                 0
#define  SCLI_SHELL_STATE_WAITING_USERNAME          1
#define  SCLI_SHELL_STATE_WAITING_PASSWORD          2
#define  SCLI_SHELL_STATE_LOGGED_IN                 3

#define  SCLI_SHELL_CACHE_CMD_COUNT                 64      /* default amount of commands to be cached */

typedef  struct _SCLI_SHELL_SESSION_CMD_CACHE
{
    PUCHAR                          *pCmds;                 /* commands array */
    ULONG                           CacheSize;              /* maximum amount of commands in cache */
    ULONG                           NumOfCmds;              /* number of commands cached */
    ULONG                           CacheHead;              /* the head of cache - the buffer can be seen as round buffer */
    ULONG                           Cursor;                 /* current cursor, 0 to CacheSize - 1 */
    BOOL                            bCursorAtTail;          /* cursor is not at tail */
    BOOL                            bCursorAfterTail;       /* cursor is after the last entry */
}
SCLI_SHELL_SESSION_CMD_CACHE, *PSCLI_SHELL_SESSION_CMD_CACHE;


#define  SCLI_SHELL_BUFFERED_COMMAND_MAX_COUNT      16

typedef  struct _SCLI_SHELL_BUFFERED_CMD
{
    ULONG                           ulCmdCode;              /* for builtin command */
    PUCHAR                          pCmd;                   /* non built-in */
}
SCLI_SHELL_BUFFERED_CMD, *PSCLI_SHELL_BUFFERED_CMD;


typedef  struct _SCLI_SHELL_BUFFERED_CMD_LIST
{
    PSCLI_SHELL_BUFFERED_CMD        pCmds;                  /* command array */
    ULONG                           ulSize;                 /* size of command array */
    ULONG                           ulCount;                /* number of commands in array */
}
SCLI_SHELL_BUFFERED_CMD_LIST, *PSCLI_SHELL_BUFFERED_CMD_LIST;


#define  SCLI_SHELL_CURSOR_QUERY_TIMEOUT            5000

#define  SCLI_SHELL_INPUT_MODE_linemode             1
#define  SCLI_SHELL_INPUT_MODE_textbox              2

#define  SCLI_SHELL_INPUT_IN_LINEMODE               (pSession->hActiveTextBox == (ANSC_HANDLE)NULL)
#define  SCLI_SHELL_INPUT_IN_TEXTBOX                (pSession->hActiveTextBox != (ANSC_HANDLE)NULL)

#define  SCLI_SHELL_MAX_AUTH_FAILURES               3

#define  SCLI_SHELL_INACTIVITY_TIMEOUT(startTime, curTime, toPeriod)    \
    ( curTime >= startTime ) ?                                          \
        ( (curTime - startTime) >= toPeriod ) :                         \
        ( ( 0xFFFFFFFF - startTime + curTime ) >= toPeriod )

#define  SCLI_SHELL_IDLE_TIMER_INTERVAL             5000    /* in milliseconds */

typedef  struct _SCLI_SHELL_SESSION_EXEC
{
    SINGLE_LINK_ENTRY               Linkage;

    ANSC_LOCK                       AccessLock;

    ULONG                           SessionId;              /* session id */
    ANSC_HANDLE                     hScliShellObj;          
    BOOL                            bServing;               /* executing command */
    BOOL                            bWillExit;          

#ifdef   _CLI_SYSTEM_CONSOLE_ENABLED
    BOOL                            bOsShellActive;         /* system shell is active */
#endif
    
    ULONG                           NumAuthFailure;

    UCHAR                           Command[SCLI_SHELL_MAX_COMMAND_LEN];
    ULONG                           CommandLen;             /* length of current command */
    ULONG                           CursorPos;              /* cursor position */

    BOOL                            bCursorFwdOutbox;       /* allowed to move cursor out of box temporarily */

    ULONG                           LastRecvTS;             /* timestamp of last recv */

    BOOL                            bAuthenticated;   
    BOOL                            bPrivileged;
    ULONG                           SessionState;
    BOOL                            bGreeting;

    ANSC_HANDLE                     hCeeExecEnv;            /* structure _SCLI_SHELL_EXEC_ENV created after user login */

    ANSC_HANDLE                     hBmc2Terminal;          /* transparent to SCLISH */
    ANSC_HANDLE                     hBmc2IceIf;             /* created specifically for this session */

    UCHAR                           Username[SCLI_SHELL_USERNAME_LEN];

    SCLI_SHELL_SESSION_CMD_CACHE    CmdCache;               /* cache of commands */

    /* buffered commands */
    SCLI_SHELL_BUFFERED_CMD_LIST    BufferedCmd;            /* buffered commands */

    /* text box */
    BOOL                            bWaitInput;             /* waiting for user input */
    ANSC_EVENT                      InputEvent;             /* user has input something */
    ULONG                           InputMode;              /* linemode or textbox */
    ANSC_HANDLE                     hActiveTextBox;         /* active text box */
    ULONG                           ulFirstVisiblePos;      /* to scroll user input in the text box */
}
SCLI_SHELL_SESSION_EXEC, *PSCLI_SHELL_SESSION_EXEC;

#define  ACCESS_SCLI_SHELL_SESSION_EXEC_OBJECT(p)        \
         ACCESS_CONTAINER(p, SCLI_SHELL_SESSION_EXEC, Linkage)


typedef  struct _SCLI_SHELL_ASYNC_TASK_CONTEXT
{
    ANSC_HANDLE                     hScliShell;
    ANSC_HANDLE                     hSrvSession;
    PUCHAR                          pCmd;

    TELNET_CMD_EXECUTION_ENV        ExecEnv;
}
SCLI_SHELL_ASYNC_TASK_CONTEXT, *PSCLI_SHELL_ASYNC_TASK_CONTEXT;


/*
 * Define some const values that will be used in the object mapper object definition.
 */

#define  SCLI_SHELL_TSP_INTERFACE_NAME                "scliShellInterface"
#define  SCLI_SHELL_TSP_INTERFACE_ID                  0                     

#define  SCLI_SHELL_CEN_INTERFACE_NAME                "scliShellCenInterface"
#define  SCLI_SHELL_CEN_INTERFACE_ID                  0                     

#define  SCLI_SHELL_ICE_INTERFACE_NAME                "scliBmc2IceInterface"
#define  SCLI_SHELL_ICE_INTERFACE_ID                  0                     

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SCLISHO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_HANDLE
(*PFN_SCLISHO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_HANDLE
(*PFN_SCLISHO_GET_SESSION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       SessionId
    );

typedef  ANSC_HANDLE
(*PFN_SCLISHO_CREATE_SESSION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       SessionId
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_REMOVE_SESSION)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_START_AUTH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_DO_AUTH)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUserName,
        PUCHAR                      pPassword,
        PULONG                      pulPerm,
        PBOOL                       pbPrivileged,
        PULONG                      pulTermPermission,
        PULONG                      pulIdleTimeout,
        PULONG                      pulLockoutTimeout
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_PROC_CMD_DATA)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PVOID                       pCmd,
        ULONG                       ulCmdLen
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_PROC_CMD_CHAR)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        UCHAR                       ucChar,
        PUCHAR                      pEchoBuf,
        PULONG                      pulEchoSize
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_RUN_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_RUN_BICMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       CmdCode
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_RUN_NBICMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_CACHE_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pCmd
    );

typedef  BOOL
(*PFN_SCLISHO_IS_CMD_MODIFIED)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PUCHAR                      pCmd
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_GET_CMD_CODE)
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pCmd
    );

typedef  PUCHAR
(*PFN_SCLISHO_GET_CACHE_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

typedef  ULONG
(*PFN_SCLISHO_GET_ESC_CODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pCmd,
        ULONG                       ulCmdLen,
        PULONG                      pulEscCodeLen
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_RUN_CMD_ASYNC)
    (
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_RUN_BICMD2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ANSC_HANDLE                 hReserved
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_DO_BUFFERED_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_GET_BUFFERED_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        PULONG                      pulCmdCode,
        PUCHAR                      *ppCmd
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_BUFFER_CMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ULONG                       ulCmdCode,
        PUCHAR                      pCmd
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_CLEAR_BUFCMD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession
    );

typedef  ULONG
(*PFN_SCLISHO_GET_INPUT_MODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSession
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_REDRAW_TEXTINPUT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        ULONG                       ulCursorTBoxPos
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_MOVETO_TEXTBOX)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_SET_SHELL_PROMPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hSrvSession,
        ANSC_HANDLE                 hExecEnv,
        PUCHAR                      pMenuTitle
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_DO_TIMER)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ULONG
(*PFN_SCLISHO_GET_ACT_SESSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SCLISHO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Type
    );

/*
 * This object provides Shell interface to telnet server object and it can be 
 * employed to BSP page execution engine to provide CLI services.
 */
#define  SCLI_SHELL_SERVICE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SCLI_SHELL_PROPERTY             Property;                                               \
    ANSC_HANDLE                     hTspIf;                                                 \
    ANSC_HANDLE                     hCenIf;                                                 \
    ANSC_HANDLE                     hAuthIf;                                                \
    ANSC_HANDLE                     hBmc2SccIf;                                             \
    ANSC_HANDLE                     hTscIf;                                                 \
    ANSC_LOCK                       SessionListLock;                                        \
    SLIST_HEADER                    SessionList;                                            \
    ANSC_HANDLE                     hTdo;                                                   \
    ANSC_HANDLE                     hTdoClient;                                             \
    ULONG                           ShellType;                                              \
    ULONG                           InactivityTimeout;                                      \
    ULONG                           LockoutTimeout;                                         \
    char*                           ShellPrompt;                                            \
                                                                                            \
    PFN_SCLISHO_GET_PROPERTY        GetProperty;                                            \
    PFN_SCLISHO_SET_PROPERTY        SetProperty;                                            \
    PFN_SCLISHO_RESET               Reset;                                                  \
    PFN_SCLISHO_RESET               ResetProperty;                                          \
                                                                                            \
    PFN_SCLISHO_GET_IF              GetTspIf;                                               \
    PFN_SCLISHO_SET_IF              SetTspIf;                                               \
    PFN_SCLISHO_GET_IF              GetBmc2SccIf;                                           \
    PFN_SCLISHO_SET_IF              SetBmc2SccIf;                                           \
    PFN_SCLISHO_SET_IF              SetAuthIf;                                              \
    PFN_SCLISHO_SET_IF              SetTscIf;                                               \
                                                                                            \
    PFN_SCLISHO_GET_SESSION         GetSession;                                             \
    PFN_SCLISHO_CREATE_SESSION      CreateSession;                                          \
    PFN_SCLISHO_REMOVE_SESSION      RemoveSession;                                          \
    PFN_SCLISHO_GET_ACT_SESSION     GetNumActiveSessions;                                   \
                                                                                            \
    PFN_SCLISHO_START_AUTH          StartUserAuth;                                          \
    PFN_SCLISHO_DO_AUTH             DoAuth;                                                 \
                                                                                            \
    PFN_SCLISHO_PROC_CMD_DATA       ProcessCmdData;                                         \
    PFN_SCLISHO_PROC_CMD_CHAR       ProcessCmdChar;                                         \
    PFN_SCLISHO_RUN_CMD             RunCmd;                                                 \
    PFN_SCLISHO_CACHE_CMD           CacheCmd;                                               \
    PFN_SCLISHO_IS_CMD_MODIFIED     IsCmdModified;                                          \
    PFN_SCLISHO_RUN_BICMD           RunBuiltInCmd;                                          \
    PFN_SCLISHO_RUN_NBICMD          RunNonBuiltInCmd;                                       \
    PFN_SCLISHO_RUN_NBICMD          RunShellCmd;                                            \
    PFN_SCLISHO_RUN_NBICMD          ValidateCmdArgs;                                        \
    PFN_SCLISHO_GET_CMD_CODE        GetCmdCode;                                             \
    PFN_SCLISHO_GET_CACHE_CMD       GetPrevCmd;                                             \
    PFN_SCLISHO_GET_CACHE_CMD       GetNextCmd;                                             \
    PFN_SCLISHO_GET_CACHE_CMD       GetFirstCmd;                                            \
    PFN_SCLISHO_GET_CACHE_CMD       GetLastCmd;                                             \
    PFN_SCLISHO_GET_ESC_CODE        GetEscCmdCode;                                          \
                                                                                            \
    PFN_SCLISHO_RUN_CMD_ASYNC       AsyncRunCmd;                                            \
                                                                                            \
    PFN_SCLISHO_RUN_BICMD2          RunBicExit;                                             \
    PFN_SCLISHO_RUN_BICMD2          RunBicClear;                                            \
    PFN_SCLISHO_RUN_BICMD2          RunBicShell;                                            \
    PFN_SCLISHO_RUN_BICMD           RunBicHistory;                                          \
    PFN_SCLISHO_RUN_BICMD2          RunBicCursorBackward;                                   \
    PFN_SCLISHO_RUN_BICMD2          RunBicCursorForward;                                    \
    PFN_SCLISHO_RUN_BICMD2          RunBicCursorHome;                                       \
    PFN_SCLISHO_RUN_BICMD2          RunBicCursorEnd;                                        \
    PFN_SCLISHO_RUN_BICMD2          RunBicDel;                                              \
    PFN_SCLISHO_RUN_BICMD2          RunBicBackspace;                                        \
    PFN_SCLISHO_RUN_BICMD2          RunBicBreak;                                            \
    PFN_SCLISHO_RUN_BICMD2          RunBicAutoCompletion;                                   \
    PFN_SCLISHO_RUN_BICMD2          RunBicHelp;                                             \
                                                                                            \
    PFN_SCLISHO_DO_BUFFERED_CMD     DoBufferedCommands;                                     \
    PFN_SCLISHO_GET_BUFFERED_CMD    GetBufferedCmd;                                         \
    PFN_SCLISHO_BUFFER_CMD          BufferCmd;                                              \
    PFN_SCLISHO_CLEAR_BUFCMD        ClearBufferCmd;                                         \
                                                                                            \
    PFN_SCLISHO_GET_INPUT_MODE      GetInputMode;                                           \
    PFN_SCLISHO_REDRAW_TEXTINPUT    RedrawTextBoxInput;                                     \
    PFN_SCLISHO_MOVETO_TEXTBOX      MoveToTextBox;                                          \
                                                                                            \
    PFN_SCLISHO_SET_SHELL_PROMPT    ShellPromptChanged;                                     \
                                                                                            \
    PFN_SCLISHO_DO_TIMER            InitTdo;                                                \
    PFN_SCLISHO_DO_TIMER            KillTdo;                                                \
    PFN_SCLISHO_DO_TIMER            StartTdo;                                               \
    PFN_SCLISHO_DO_TIMER            StopTdo;                                                \
    /* end of object class content */                                                       \

typedef  struct
_SCLI_SHELL_OBJECT
{
    SCLI_SHELL_SERVICE_CLASS_CONTENT
}
SCLI_SHELL_OBJECT,  *PSCLI_SHELL_OBJECT;

#define  ACCESS_SCLI_SHELL_OBJECT(p)          \
         ACCESS_CONTAINER(p, SCLI_SHELL_OBJECT, Linkage)


#endif
