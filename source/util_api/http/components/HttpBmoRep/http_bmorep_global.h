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

    module:	http_bmorep_global.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2002
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Http Bmo Rep implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/27/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_BMOREP_GLOBAL_
#define  _HTTP_BMOREP_GLOBAL_


#include "ansc_platform.h"
#include "ansc_socket.h"
#include "ansc_socket_external_api.h"

#include "http_co_oid.h"
#include "http_co_name.h"
#include "http_co_type.h"
#include "http_properties.h"

#include "http_ifo_wam.h"
#include "http_ifo_sbc.h"
#include "http_ifo_cbc.h"
#include "http_ifo_hfp.h"
#include "http_ifo_tmh.h"

#include "http_bmorep_interface.h"
#include "http_bmorep_exported_api.h"
#include "http_bmorep_internal_api.h"

#include "http_hco_interface.h"
#include "http_hco_exported_api.h"
#include "http_spo_interface.h"
#include "http_spo_exported_api.h"


#endif
