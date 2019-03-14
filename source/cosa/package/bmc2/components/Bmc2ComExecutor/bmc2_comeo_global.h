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

    module:	bmc2_comeo_global.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bmc2 Com Executor
        Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_COMEO_GLOBAL_
#define  _BMC2_COMEO_GLOBAL_

#include "cli_module_custom.h"
#include "ansc_platform.h"

#include "bmc2_co_oid.h"
#include "bmc2_co_name.h"
#include "bmc2_co_type.h"

#include "bmc2_ifo_pec.h"
#include "bmc2_properties.h"

#include "bmc2_comeo_interface.h"
#include "bmc2_comeo_exported_api.h"
#include "bmc2_comeo_internal_api.h"

#include "bmc2_comeo_helper.h"
#include "bmc2_comeo_helper_ext.h"
#include "bmc2_comeo_helper_api.h"

#include "ansc_string_util.h"

#include "bspeng_co_interface.h"
#include "bspeng_co_exported_api.h"

#include "bree_spo_srm.h"
#include "bree_spo_cooked_page.h"

#ifdef   _ANSC_FILE_ZLIB_
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#endif

#endif

