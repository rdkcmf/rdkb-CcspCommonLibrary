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

#ifndef  _DSLG_EGWM_CLI_COMMAND_LIST
#define  _DSLG_EGWM_CLI_COMMAND_LIST

#include "dslg_definitions_user.h"

typedef  struct
_DSLG_EGWM_CLI_CMDLIST
{
    char                            *pDomainNamePattern;
    char                            *pCommand;
    char                            *pScpPath;
    BOOL                            bReadOnly;
    ULONG                           Permission;
}
DSLG_EGWM_CLI_CMDLIST, *PDSLG_EGWM_CLI_CMDLIST;


#define  DSLG_EGWM_CLI_CMD_PERMISSION_Admin         0x00000001
#define  DSLG_EGWM_CLI_CMD_PERMISSION_AnyUser       0x00000002

#define  DSLG_EGWM_CLI_TERM_PERMISSION_Admin        0x00000003
#define  DSLG_EGWM_CLI_TERM_PERMISSION_AnyUser      0x00000002


#endif
