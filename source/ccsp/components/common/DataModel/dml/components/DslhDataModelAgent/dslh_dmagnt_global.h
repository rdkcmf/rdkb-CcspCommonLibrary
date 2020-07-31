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

    module: dslh_dmagnt_global.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Dslh DataModel Agent implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        11/01/2010    initial revision.

**********************************************************************/


#ifndef  _DSLH_DMAGNT_GLOBAL_
#define  _DSLH_DMAGNT_GLOBAL_

#include "ansc_platform.h"
#include "slap_definitions.h"

#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"

#include "dslh_co_oid.h"
#include "dslh_co_name.h"
#include "dslh_co_type.h"
#include "dslh_properties.h"

#include "dslh_cpeco_interface.h"
#include "dslh_cpeco_exported_api.h"
#include "dslh_dmagnt_interface.h"
#include "dslh_dmagnt_exported_api.h"
#include "dslh_dmagnt_internal_api.h"

#include "dslh_wmpdo_interface.h"
#include "dslh_wmpdo_exported_api.h"
#include "dslh_objro_interface.h"
#include "dslh_objro_exported_api.h"
#include "dslh_objeo_interface.h"
#include "dslh_objeo_exported_api.h"

#include "dslh_objco_interface.h"
#include "dslh_objco_exported_api.h"
#include "dslh_objco_table_interface.h"
#include "dslh_objco_table_exported_api.h"

/*
#include "poam_irepdo_interface.h"
#include "poam_irepdo_exported_api.h"
#include "poam_irepfo_interface.h"
#include "poam_irepfo_exported_api.h"
*/

#include "ansc_load_library.h"
#include "cosa_plugin_api.h"

#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"

#include "ansc_xml_dom_parser_interface.h"
#include "ansc_xml_dom_parser_external_api.h"
#include "ansc_xml_dom_parser_status.h"

#include "dslh_ifo_mws.h"
#include "dslh_ifo_tr69.h"
/*
#include "dslh_ifo_mso.h"
#include "slap_ifo_poa.h"
#include "slap_ifo_goa.h"
*/

/*
#include "slap_dslh_dml_interface.h"
#include "poam_dslh_dml_interface.h"
#include "poam_dslh_dml_exported_api.h"
*/

/*
#include "bbhm_srvco_interface.h"
#include "bbhm_sysco_interface.h"
*/
#include "dslh_cpeco_interface.h"
/*
#include "bbhm_mco_interface.h"
*/

#include "ssd_ifo_dml.h"

/*extern  PBBHM_MONO_CONTAINER_OBJECT    pBbhmMonoContainer; */

SLAP_INT
SlapVcoStringToInt
    (
        ANSC_HANDLE                 hThisObject,
        char*                       var_int
    );

#endif
