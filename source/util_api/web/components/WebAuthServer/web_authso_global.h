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

    module:	web_authso_global.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Web Authentication Server object implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/12/05    initial revision.

**********************************************************************/


#ifndef  _WEB_AUTHSO_GLOBAL_
#define  _WEB_AUTHSO_GLOBAL_


#include "ansc_platform.h"

#include "web_co_oid.h"
#include "web_co_name.h"
#include "web_co_type.h"
#include "web_properties.h"

#include "http_autho_info.h"

#include "web_authso_interface.h"
#include "web_authso_exported_api.h"
#include "web_authso_internal_api.h"
#include "web_ifo_acm.h"

#include "http_bmoreq_interface.h"
#include "http_bmoreq_exported_api.h"
#include "http_bmorep_interface.h"
#include "http_bmorep_exported_api.h"

#include "web_smo_interface.h"
#include "web_gso_interface.h"

#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"


#endif
