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

    module:	bmc2_cmd_property_api.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the command property functions provided by the Bmc2 Env 
        Controller Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/08/10    initial revision.

**********************************************************************/

#ifndef  _BMC2_COMMAND_PROPERTY_API_
#define  _BMC2_COMMAND_PROPERTY_API_

#include "bmc2_definitions.h"

void
Bmc2CleanCommandProperty
    (
        PBMC2_COMMAND_PROPERTY      pCommandProperty
    );

void
Bmc2FreeCommandProperty
    (
        PBMC2_COMMAND_PROPERTY      pCommandProperty
    );

void 
Bmc2CleanCmdArgument
    (
        PBMC2_CMD_ARGUMENT          pCmdArg
    );

void
Bmc2FreeCmdArgument
    (
        PBMC2_CMD_ARGUMENT          pCmdArg
    );

void
Bmc2CleanCmdDepArg
    (
        PBMC2_CMD_DEP_ARG           pCmdDepArg
    );

void
Bmc2CleanCmdDepArgBranch
    (
        PBMC2_CMD_DEP_ARG_BRANCH    pCmdDepArgBranch
    );

void
Bmc2CleanCmdSimpleArg
    (
        PBMC2_CMD_SIMPLE_ARG        pCmdArg
    );

void
Bmc2CleanCmdArgValueRange
    (
        PBMC2_CMD_ARG_VRANGE        pCmdArgVrange
    );

void
Bmc2FreeCmdArgValueRange
    (
        PBMC2_CMD_ARG_VRANGE        pCmdArgVrange
    );

void
Bmc2CleanCmdArgHelp
    (
        PBMC2_CMD_ARG_HELP          pCmdArgHelp
    );

void
Bmc2FreeCmdHelpSyntax
    (
        PBMC2_CMD_HELP_SYNTAX       pCmdHelpSyntax
    );

#endif

