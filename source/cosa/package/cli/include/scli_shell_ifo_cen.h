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

    module:	scli_shell_ifo_cen.h

        For Simple CLI Shell object (SCLISH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the SCLI Shell Command Execution Notification.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/15/05    initial revision.

**********************************************************************/


#ifndef  _SCLI_SHELL_IFO_CEN_
#define  _SCLI_SHELL_IFO_CEN_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
     SCLI COMMAND EXECUTION NOTIFICATION INTERFACE
***********************************************************/

typedef  struct 
_SCLI_SHELL_EXEC_ENV
{
    /* environment variables,  read-only for Cli Execution Engine */
    ANSC_HANDLE                     hSession;               /* used as 'hSrvSession' for calls through TSC interface */
    ANSC_HANDLE                     hTelnetEnv;             /* structure _TELNET_CMD_EXECUTION_ENV */
}
SCLI_SHELL_EXEC_ENV, *PSCLI_SHELL_EXEC_ENV;


/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_SCLICENIF_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        ULONG                       CenEvent
    );

typedef  ANSC_STATUS
(*PFN_SCLICENIF_SET_PROMPT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTerminal,
        PUCHAR                      pPrompt
    );

/*
 * SCLI Shell CEN interface provides routines to output data to client
 * and event notification.
 */
#define  SCLI_SHELL_CEN_INTERFACE_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SCLICENIF_NOTIFY            Notify;                                                 \
    PFN_SCLICENIF_SET_PROMPT        SetShellPrompt;                                         \
    /* end of object class content */                                                       \

typedef  struct
_SCLI_SHELL_CEN_INTERFACE
{
    SCLI_SHELL_CEN_INTERFACE_CLASS_CONTENT
}
SCLI_SHELL_CEN_INTERFACE,  *PSCLI_SHELL_CEN_INTERFACE;

#define  ACCESS_SCLI_SHELL_CEN_INTERFACE(p)                   \
         ACCESS_CONTAINER(p, SCLI_SHELL_CEN_INTERFACE, Linkage)


#endif
