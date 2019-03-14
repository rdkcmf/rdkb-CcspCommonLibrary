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

    module:	bwsp_pho_global.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Bwsp Portal Host implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#ifndef  _BWSP_PHO_GLOBAL_
#define  _BWSP_PHO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_socket.h"
#include "ansc_socket_external_api.h"
#include "ansc_tso_interface.h"
#include "ansc_tso_external_api.h"

#include "http_hco_interface.h"
#include "http_hco_exported_api.h"
#include "http_bmo_interface.h"
#include "http_bmo_exported_api.h"
#include "http_bmoreq_interface.h"
#include "http_bmoreq_exported_api.h"
#include "http_bmorep_interface.h"
#include "http_bmorep_exported_api.h"

#include "bwsp_co_oid.h"
#include "bwsp_co_name.h"
#include "bwsp_co_type.h"
#include "bwsp_properties.h"

#include "bwsp_pho_interface.h"
#include "bwsp_pho_exported_api.h"
#include "bwsp_pho_internal_api.h"


#endif
