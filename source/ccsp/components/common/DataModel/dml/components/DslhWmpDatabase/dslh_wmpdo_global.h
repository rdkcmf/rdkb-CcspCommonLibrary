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

    module:	dslh_wmpdo_global.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Dslh Wmp Database implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_WMPDO_GLOBAL_
#define  _DSLH_WMPDO_GLOBAL_


#include "ansc_platform.h"
#include "slap_definitions.h"
#include "ansc_tso_interface.h"
#include "ansc_tso_external_api.h"

#include "dslh_co_oid.h"
#include "dslh_co_name.h"
#include "dslh_co_type.h"
#include "dslh_properties.h"

#include "dslh_wmpdo_interface.h"
#include "dslh_wmpdo_exported_api.h"
#include "dslh_wmpdo_internal_api.h"
#include "dslh_wmpdo_layout.h"

#include "dslh_cpeco_interface.h"
#include "dslh_cpeco_exported_api.h"

#include "dslh_objro_interface.h"
#include "dslh_objro_exported_api.h"
#include "dslh_objeo_interface.h"
#include "dslh_objeo_exported_api.h"
#include "dslh_varro_interface.h"
#include "dslh_varro_exported_api.h"
#include "dslh_vareo_interface.h"
#include "dslh_vareo_exported_api.h"

#include "dslh_objco_table_interface.h"
#include "dslh_objco_table_exported_api.h"

#include "slap_vco_interface.h"
#include "slap_vco_exported_api.h"
#include "slap_vmo_interface.h"
#include "slap_vmo_exported_api.h"


#endif
