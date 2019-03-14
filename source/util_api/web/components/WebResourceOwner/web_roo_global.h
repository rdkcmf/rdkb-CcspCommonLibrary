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

    module:	web_roo_global.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Web Resource Owner implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#ifndef  _WEB_ROO_GLOBAL_
#define  _WEB_ROO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_socket.h"
#include "ansc_socket_external_api.h"

#include "http_ifo_wsp.h"
#include "http_ifo_hfp.h"
#include "http_ifo_rcp.h"

#include "http_hco_interface.h"
#include "http_hco_exported_api.h"
#include "http_bmo_interface.h"
#include "http_bmo_exported_api.h"
#include "http_bmoreq_interface.h"
#include "http_bmoreq_exported_api.h"
#include "http_bmorep_interface.h"
#include "http_bmorep_exported_api.h"

#include "web_co_oid.h"
#include "web_co_name.h"
#include "web_co_type.h"
#include "web_properties.h"

#include "web_roo_interface.h"
#include "web_roo_exported_api.h"
#include "web_roo_internal_api.h"

#include "web_gso_interface.h"
#include "web_gso_exported_api.h"
#include "web_smo_interface.h"
#include "web_smo_exported_api.h"

#include "web_authso_interface.h"
#include "web_authso_exported_api.h"


#endif
