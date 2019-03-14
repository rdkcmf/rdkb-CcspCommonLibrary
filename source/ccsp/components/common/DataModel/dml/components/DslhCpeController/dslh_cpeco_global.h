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

    module: dslh_cpeco_global.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Dslh Cpe Controller implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        09/09/2005    initial revision.
        11/01/2010    Bin added DataModelAgent module;

**********************************************************************/


#ifndef  _DSLH_CPECO_GLOBAL_
#define  _DSLH_CPECO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_tso_interface.h"
#include "ansc_tso_external_api.h"

#include "slap_ifo_poa.h"
#include "slap_ifo_goa.h"

#include "dslh_co_oid.h"
#include "dslh_co_name.h"
#include "dslh_co_type.h"
#include "dslh_properties.h"

#include "dslh_cpeco_interface.h"
#include "dslh_cpeco_exported_api.h"
#include "dslh_cpeco_internal_api.h"

#include "dslh_objeo_interface.h"
#include "dslh_objro_interface.h"
#include "dslh_vareo_interface.h"
#include "dslh_varro_interface.h"

#include "dslh_dmagnt_exported_api.h"
#include "dslh_dmagnt_interface.h"

/*#include "dslh_acsbo_interface.h"
#include "dslh_acsbo_exported_api.h"
#include "dslh_wmppo_interface.h"
#include "dslh_wmppo_exported_api.h"
#include "dslh_wmpso_interface.h"
#include "dslh_wmpso_exported_api.h"*/
#include "dslh_wmpdo_interface.h"
#include "dslh_wmpdo_exported_api.h"
/*#include "dslh_soappo_interface.h"
#include "dslh_soappo_exported_api.h"
#include "dslh_filemo_interface.h"
#include "dslh_filemo_exported_api.h"
#include "dslh_stunmo_interface.h"
#include "dslh_stunmo_exported_api.h"

#include "dslh_rpcextmgr_interface.h"
#include "dslh_rpcextmgr_exported_api.h"

#ifdef   _DSLH_TCP_CONNREQ_HANDLER
#include "dslh_tcpcrho_interface.h"
#include "dslh_tcpcrho_exported_api.h"
#endif
*/
#include "dslh_varro_interface.h"

/*#include "dslh_ifo_sta.h"
#include "dslh_ifo_dbc.h"
#include "dslh_ifo_vsp.h"
#include "dslh_ifo_ldm.h"
#include "dslh_ifo_wdm.h"
#include "dslh_ifo_l3f.h"
#include "dslh_ifo_wcm.h"
#include "dslh_ifo_eia.h"
#include "dslh_ifo_eim.h"
#include "dslh_ifo_uim.h"
#include "dslh_ifo_wim.h"
#include "dslh_ifo_sia.h"
#include "dslh_ifo_lbc.h"
#include "dslh_ifo_tqm.h"
*/

/*
 * Bin Zhu added more here on 12/01/2006
 */
#include "dslgm_module_custom.h"



#endif
