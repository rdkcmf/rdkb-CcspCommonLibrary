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

    module:	bmc2_comto_global.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Bmc2 Com Terminal implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_COMTO_GLOBAL_
#define  _BMC2_COMTO_GLOBAL_


#include "ansc_platform.h"

#include "bmc2_co_oid.h"
#include "bmc2_co_name.h"
#include "bmc2_co_type.h"
#include "bmc2_properties.h"

#include "bwrm_ifo_ram.h"
#include "bmc2_ifo_ice.h"
#include "slap_ifo_goa.h"
#include "slap_ifo_uoa.h"

#include "bmc2_comto_interface.h"
#include "bmc2_comto_exported_api.h"
#include "bmc2_comto_internal_api.h"

#include "bmc2_envco_interface.h"
#include "bmc2_envco_exported_api.h"
#include "bmc2_comeo_interface.h"
#include "bmc2_comeo_exported_api.h"
#include "bmc2_comdo_interface.h"
#include "bmc2_comdo_exported_api.h"
#include "bmc2_reqco_interface.h"
#include "bmc2_reqco_exported_api.h"

#include "bwrm_eco_interface.h"
#include "bwrm_eco_exported_api.h"
#include "bwrm_cpo_interface.h"
#include "bwrm_cpo_exported_api.h"

#include "slap_bmc2appo_interface.h"
#include "slap_bmc2appo_exported_api.h"
#include "slap_bmc2srvo_interface.h"
#include "slap_bmc2srvo_exported_api.h"
#include "slap_bmc2termo_interface.h"
#include "slap_bmc2termo_exported_api.h"
#include "slap_bmc2domo_interface.h"
#include "slap_bmc2domo_exported_api.h"
#include "slap_bmc2como_interface.h"
#include "slap_bmc2como_exported_api.h"
#include "slap_bmc2ino_interface.h"
#include "slap_bmc2ino_exported_api.h"
#include "slap_bmc2outo_interface.h"
#include "slap_bmc2outo_exported_api.h"

#include "slap_veo_interface.h"
#include "slap_veo_exported_api.h"
#include "slap_vmo_interface.h"
#include "slap_vmo_exported_api.h"
#include "slap_vco_interface.h"
#include "slap_vco_exported_api.h"

#include "slap_owo_interface.h"

#endif
