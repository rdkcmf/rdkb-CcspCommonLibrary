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

    module:	telnet_properties.h

        For TELNET,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines properties that can be
        applied to TELNET protocol. 

        
    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/09/05    initial revision.

**********************************************************************/


#ifndef  _TELNET_PROPERTIES_
#define  _TELNET_PROPERTIES_


/*
 * We have to use some of the constant values defined in the TELNET message definitions, so we
 * include the header file here.
 */
#include  "telnet_definitions.h"


/***********************************************************
   TELNET SIMPLE SERVER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * TELNET is an application-level protocol for distributed, col-
 * laborative, hypermedia information systems. It is a generic, stateless, protocol which can be
 * used for many tasks beyond its use for hypertext, through extension of its request methods,
 * error codes and headers. A feature of TELNET is the typing and negotiation of data representation,
 * allowing systems to be built independently of the data being transferred.
 */
#define  TELNET_SERVER_TYPE_SMALL                   1
#define  TELNET_SERVER_TYPE_MEDIUM                  2
#define  TELNET_SERVER_TYPE_BIG                     3

#define  TELNET_SSERVER_ENGINE_NUMBER               2
#define  TELNET_MSERVER_ENGINE_NUMBER               4
#define  TELNET_BSERVER_ENGINE_NUMBER               16

#define  TELNET_SSERVER_SOCKET_NUMBER               8
#define  TELNET_MSERVER_SOCKET_NUMBER               32
#define  TELNET_BSERVER_SOCKET_NUMBER               128

#define  TELNET_SERVER_MODE_KeepConnection          0x00000001

#define  TELNET_SERVER_OPT_SuppressGA               0x00000001
#define  TELNET_SERVER_OPT_WindowSize               0x00000002
#define  TELNET_SERVER_OPT_Echo                     0x00000004
#define  TELNET_SERVER_OPT_TerminalType             0x00000008

#define  TELNET_SERVER_TTY_NVT                      0                   /* pure char terminal */
#define  TELNET_SERVER_TTY_Ansi                     1                   /* ANSI */
#define  TELNET_SERVER_TTY_Vt100                    2                   /* VT100 */
#define  TELNET_SERVER_TTY_Vt102                    3                   /* VT102 */
#define  TELNET_SERVER_TTY_Xterm                    4                   /* XTERM */


/*
 * TELNET Server is an application program that accepts connections in order to service requests by
 * sending back responses. Any given program may be capable of being both a client and a server;
 * our use of these terms refers only to the role being performed by the program for a particular
 * connection, rather than to the program's capabilities in general. Likewise, any server may act
 * as an origin server, proxy, gateway, or tunnel, switching behavior based on the nature of each
 * request.
 */
#define  TELNET_SSO_MAX_MESSAGE_SIZE                1024                /* 1KB        */
#define  TELNET_SSO_MIN_RECV_BUF_SIZE               128
#define  TELNET_SSO_SOCKET_TIMEOUT                  1800                /* in seconds */
#define  TELNET_SSO_SOCKET_TTC                      3                   /* in seconds */

#define  TELNET_SSO_SHELL_PROMPT_LEN                32    

#define  TELNET_SSO_SHELL_PROMPT_DEFAULT            "> "

#define  TELNET_SSO_MAX_ACK_OPTION                  4

typedef  struct
_TELNET_SIMPLE_SERVER_PROPERTY
{
    ANSC_IPV4_ADDRESS               HostAddress;
    USHORT                          HostPort;
    ULONG                           ServerType;
    ULONG                           ServerMode;
    ULONG                           ServerOptions;
    UCHAR                           ShellPrompt[TELNET_SSO_SHELL_PROMPT_LEN + 1];
}
TELNET_SIMPLE_SERVER_PROPERTY,  *PTELNET_SIMPLE_SERVER_PROPERTY;

/***********************************************************
   TELNET SRV SESSION CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

typedef  struct
_TELNET_SRV_SESSION_OPTIONS
{
    SINGLE_LINK_ENTRY               Linkage;

    UCHAR                           Action;         /* will, wont, do, dont */
    UCHAR                           OptionCode;     /* though 1 byte defined in RFC */

    PUCHAR                          pSubData;       /* sub option data */
    ULONG                           ulSubDataSize;  /* size of sub option data */
}
TELNET_SRV_SESSION_OPTIONS, *PTELNET_SRV_SESSION_OPTIONS;

#define  ACCESS_TELNET_SRV_SESSION_OPTIONS_OBJECT(p)        \
         ACCESS_CONTAINER(p, TELNET_SRV_SESSION_OPTIONS, Linkage)


/***********************************************************
   TELNET SRV SESSION CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

typedef  struct
_TELNET_SRV_SESSION_PROPERTY
{
    ULONG                           SessionOptions;

    BOOL                            bSuppressGA;    
    BOOL                            bEcho;
    ULONG                           WindowWidth;    /* in characters */
    ULONG                           WindowHeight;   /* in characters */
    ULONG                           TerminalType;

    UCHAR                           ShellPrompt[TELNET_SSO_SHELL_PROMPT_LEN + 1];

    BOOL                            bExecutingCmd;
    ULONG                           SignalAck;
}
TELNET_SRV_SESSION_PROPERTY, *PTELNET_SRV_SESSION_PROPERTY;


/***********************************************************
   TELNET CMD EXECUTION CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

#define  TELNET_TSC_EVENT_SessionStart              1
#define  TELNET_TSC_EVENT_SessionEnd                2
#define  TELNET_TSC_EVENT_CmdBegin                  3
#define  TELNET_TSC_EVENT_CmdEnd                    4
#define  TELNET_TSC_EVENT_IdleSession               5
#define  TELNET_TSC_EVENT_Lockout                   6

typedef  struct
_TELNET_CMD_EXECUTION_ENV
{
    /* console window size */
    ULONG                           WindowWidth;    /* in characters */
    ULONG                           WindowHeight;   /* in characters */

    ULONG                           TerminalType;

    /* 
     * bEcho must be checked every time execution engine needs to 
     * output any data. OBSOLETE!
     */
    BOOL                            bEcho;

    ANSC_HANDLE                     hTscIf;             /* TSP service object uses this interface to do content output */
}
TELNET_CMD_EXECUTION_ENV, *PTELNET_CMD_EXECUTION_ENV;


/***********************************************************
 TELNET TELNET TSP SIGNAL NOTIFICATION PARAMETERS DEFINITION
***********************************************************/

#define  TELNET_TSP_SIGNAL_None                     0
#define  TELNET_TSP_SIGNAL_Synch                    1
#define  TELNET_TSP_SIGNAL_Abort                    2
#define  TELNET_TSP_SIGNAL_SessionCreated           3   /* to notify TSP object that a new 
                                                         * session has been established - options negotiation done.
                                                         */
#define  TELNET_TSP_SIGNAL_SessionTerminated        4   /* to notify TSP object that a session has terminated */


/***********************************************************
 TELNET TELNET EXECUTION CONTEXT PARAMETERS DEFINITION
***********************************************************/

typedef  struct
_TELNET_CMD_EXECUTION_CONTEXT
{
    ANSC_HANDLE                     hSrvSession;
    TELNET_CMD_EXECUTION_ENV        ExecProperty;
    ANSC_HANDLE                     hBufferContext;
}
TELNET_CMD_EXECUTION_CONTEXT, *PTELNET_CMD_EXECUTION_CONTEXT;


#endif

