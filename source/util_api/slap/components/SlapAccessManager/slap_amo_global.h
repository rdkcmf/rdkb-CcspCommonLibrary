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

    module:	slap_amo_global.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Slap Access Manager implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/03/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_AMO_GLOBAL_
#define  _SLAP_AMO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_lpcco_interface.h"
#include "ansc_lpcco_external_api.h"

#ifdef   _SLAP_IPC_USE_TCP_SOCKET
#include "ansc_lpccotcp_interface.h"
#include "ansc_lpccotcp_external_api.h"
#else
#include "ansc_lpccoudp_interface.h"
#include "ansc_lpccoudp_external_api.h"
#endif

#include "slap_co_oid.h"
#include "slap_co_name.h"
#include "slap_co_type.h"

#include "slap_properties.h"
#include "slap_cfg_system.h"

#include "slap_amo_interface.h"
#include "slap_amo_exported_api.h"
#include "slap_amo_internal_api.h"

#include "slap_eco_interface.h"
#include "slap_eco_exported_api.h"
#include "slap_lco_interface.h"
#include "slap_lco_exported_api.h"
#include "slap_lso_interface.h"
#include "slap_lso_exported_api.h"

#include "slap_spo_interface.h"
#include "slap_spo_exported_api.h"
#include "slap_owo_interface.h"
#include "slap_owo_exported_api.h"
#include "slap_owouoao_interface.h"
#include "slap_owouoao_exported_api.h"
#include "slap_owoloao_interface.h"
#include "slap_owoloao_exported_api.h"
#include "slap_owouoac_interface.h"
#include "slap_owouoac_exported_api.h"
#include "slap_owoloac_interface.h"
#include "slap_owoloac_exported_api.h"


#endif
