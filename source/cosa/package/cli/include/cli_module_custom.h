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

    module: cli_module_custom.h

        For DSL Broadband Command Line Interface (CLI),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file gives the custom module definitions for
        CLI.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Kang Quan

**********************************************************************/

#ifndef  _CLI_MODULE_CUSTOM_H_
#define  _CLI_MODULE_CUSTOM_H_


/* 
 * To enable/disable launching system console in CLI - if console
 * is enabled, "shell" becomes a built-in command. On final product,
 * this flag usually should be disabled.
 */

#define   _CLI_SYSTEM_CONSOLE_ENABLED


/* The following symbol is defined if pre-compiled SCP pages are stored in files
 * instead of original script files.
 */

/* Please include the following 3 lines when actually using pre-compiled scp pages
#if !defined(_DEBUG)
#define   _USE_PRECOMPILED_SCP_PAGES
#endif
*/


#define  CLI_SHELL_DEF_PROMPT                       "CCSP-CLI"
#define  CLI_SHELL_TERMIAL_GREETING                 "CLI Subsystem"
#define  CLI_SHELL_LOGOUT_MESSAGE                   "\r\nLogged out.\r\n\r\n"

    #define  CLI_SCP_PAGE_ROOT_PATH                 "./config/bmc2/"

#define  CLI_CMDLIST_FILE_NAME                      "cmdlist.txt"

#endif
