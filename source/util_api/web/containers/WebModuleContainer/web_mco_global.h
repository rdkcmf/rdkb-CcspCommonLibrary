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

    module:	web_mco_global.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Web Module Container implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/15/02    initial revision.

**********************************************************************/


#ifndef  _WEB_MCO_GLOBAL_
#define  _WEB_MCO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#include "ansc_object_mapper_interface.h"
#include "ansc_object_mapper_external_api.h"
#include "ansc_mco_interface.h"
#include "ansc_mco_external_api.h"
#include "ansc_atoproto_interface.h"
#include "ansc_atoproto_external_api.h"
#include "ansc_qio_interface.h"
#include "ansc_qio_external_api.h"

#include "ansc_ifo_csp.h"
#include "ansc_ifo_iqc.h"

#include "http_ifo_wsp.h"
#include "http_ifo_hfp.h"

#include "http_sso_interface.h"
#include "http_sso_exported_api.h"
#include "http_wsso_interface.h"
#include "http_wsso_exported_api.h"
#include "http_wsto_interface.h"
#include "http_wsto_exported_api.h"

#include "http_bmo_interface.h"
#include "http_bmo_exported_api.h"
#include "http_bmoreq_interface.h"
#include "http_bmoreq_exported_api.h"
#include "http_bmorep_interface.h"
#include "http_bmorep_exported_api.h"

#include "http_hco_interface.h"
#include "http_hco_exported_api.h"
#include "http_sso_interface.h"
#include "http_sso_exported_api.h"
#include "http_properties.h"

#include "web_co_oid.h"
#include "web_co_name.h"
#include "web_co_type.h"

#include "web_properties.h"

#include "web_mco_interface.h"
#include "web_mco_exported_api.h"
#include "web_mco_internal_api.h"

#include "web_sso_interface.h"
#include "web_sso_exported_api.h"
#include "web_smo_interface.h"
#include "web_smo_exported_api.h"
#include "web_vho_interface.h"
#include "web_vho_exported_api.h"
#include "web_roo_interface.h"
#include "web_roo_exported_api.h"


#endif
