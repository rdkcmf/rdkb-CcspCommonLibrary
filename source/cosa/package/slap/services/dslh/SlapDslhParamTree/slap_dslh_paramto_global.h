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

    module:	slap_dslh_paramto_global.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Slap Dslh ParamTree implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/20/05    initial revision.

**********************************************************************/


#ifndef  _SLAP_DSLH_PARAMTO_GLOBAL_
#define  _SLAP_DSLH_PARAMTO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"

#include "slap_co_oid.h"
#include "slap_co_name.h"
#include "slap_co_type.h"

#include "slap_properties.h"
#include "slap_ifo_uoa.h"
#include "slap_ifo_bss.h"
#include "slap_ifo_ola.h"
#include "slap_ifo_poa.h"
#include "slap_ifo_goa.h"

#include "slap_dslh_paramto_interface.h"
#include "slap_dslh_paramto_exported_api.h"
#include "slap_dslh_paramto_internal_api.h"

#include "ccsp_message_bus.h"
#include "ccsp_base_api.h"

SLAP_UINT32
SlapVcoStringToUint32
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_uint32
    );

SLAP_BOOL
SlapVcoStringToBool
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_bool
    );

SLAP_INT
SlapVcoStringToInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_int
    );

#endif
