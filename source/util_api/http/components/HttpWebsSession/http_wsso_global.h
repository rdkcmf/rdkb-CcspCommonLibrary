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

    module:	http_wsso_global.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Http Webs Session implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/08/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_WSSO_GLOBAL_
#define  _HTTP_WSSO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_socket.h"
#include "ansc_socket_external_api.h"

#include "ansc_dsto_interface.h"
#include "ansc_dsto_external_api.h"
#include "ansc_deto_interface.h"
#include "ansc_deto_external_api.h"
#include "ansc_dkto_interface.h"
#include "ansc_dkto_external_api.h"

#include "http_co_oid.h"
#include "http_co_name.h"
#include "http_co_type.h"
#include "http_properties.h"

#include "http_ifo_wsp.h"
#include "http_ifo_hfp.h"

#include "http_wsso_interface.h"
#include "http_wsso_exported_api.h"
#include "http_wsso_internal_api.h"

#include "http_sso_interface.h"
#include "http_sso_exported_api.h"
#include "http_wsto_interface.h"
#include "http_wsto_exported_api.h"

#include "http_bmo_interface.h"
#include "http_bmo_exported_api.h"
#include "http_bmoreq_interface.h"
#include "http_bmoreq_exported_api.h"
#include "http_bmorep_interface.h"
#include "http_bmorep_exported_api.h"

#include "http_mbo_interface.h"
#include "http_mbo_exported_api.h"
#include "http_mboidt_interface.h"
#include "http_mboidt_exported_api.h"
#include "http_mbochk_interface.h"
#include "http_mbochk_exported_api.h"


#endif
