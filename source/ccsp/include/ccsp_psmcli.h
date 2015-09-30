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

#ifndef CCSP_PSMCLI_H
#define CCSP_PSMCLI_H

// for psmcli debug
typedef enum { 
    PSMCLI_DEBUG_PRINT_NONE                          = 0,
    PSMCLI_DEBUG_PRINT_INVOCATION                    = 1,
    PSMCLI_DEBUG_PRINT_SUBROUTINE                    = 2,
    PSMCLI_DEBUG_PRINT_SUB_DETAIL                    = 3,
    PSMCLI_DEBUG_PRINT_SUB_SUB                       = 4,
    PSMCLI_DEBUG_PRINT_SUB_MEMORY                    = 5,
    PSMCLI_DEBUG_PRINT_ALL
}psmcli_debug_level;

// global variable
psmcli_debug_level psmcli_debug_print = PSMCLI_DEBUG_PRINT_NONE;

#endif
