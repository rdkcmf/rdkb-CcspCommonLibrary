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

    module:	scli_tsp_global.h

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


#ifndef  _SCLI_SHELL_GLOBAL_
#define  _SCLI_SHELL_GLOBAL_


#include "ansc_platform.h"

#include "ansc_tso_interface.h"
#include "ansc_tso_external_api.h"

#include "telnet_definitions.h"
#include "telnet_co_oid.h"
#include "telnet_co_name.h"
#include "telnet_co_type.h"
#include "telnet_ifo_tsp.h"
#include "telnet_ifo_tsc.h"
#include "telnet_properties.h"

#include "bmc2_definitions.h"
#include "slap_definitions.h"

#include "scli_shell_oid.h"
#include "scli_shell_name.h"
#include "scli_shell_type.h"
#include "scli_shell_definitions.h"
#include "scli_shell_properties.h"

#include "scli_shell_interface.h"
#include "scli_shell_exported_api.h"
#include "scli_shell_internal_api.h"
#include "scli_shell_ifo_cen.h"
#include "scli_shell_ifo_auth.h"

#include "dslg_cli_cmdlist.h"

#include "scli_shell_trace.h"

#endif
