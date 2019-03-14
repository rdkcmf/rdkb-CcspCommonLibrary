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

    module:	slap_bmc2srvo_global.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Slap Bmc2 Server implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/28/05    initial revision.

**********************************************************************/


#ifndef  _SLAP_BMC2SRVO_GLOBAL_
#define  _SLAP_BMC2SRVO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"

#include "slap_co_oid.h"
#include "slap_co_name.h"
#include "slap_co_type.h"

#include "slap_properties.h"
#include "slap_ifo_uoa.h"
#include "slap_ifo_ola.h"

#include "slap_bmc2srvo_interface.h"
#include "slap_bmc2srvo_exported_api.h"
#include "slap_bmc2srvo_internal_api.h"

#include "bmc2_envco_interface.h"
#include "bmc2_envco_exported_api.h"
#include "bmc2_comeo_interface.h"
#include "bmc2_comeo_exported_api.h"
#include "bmc2_comto_interface.h"
#include "bmc2_comto_exported_api.h"
#include "bmc2_comdo_interface.h"
#include "bmc2_comdo_exported_api.h"
#include "bmc2_reqco_interface.h"
#include "bmc2_reqco_exported_api.h"


#endif
