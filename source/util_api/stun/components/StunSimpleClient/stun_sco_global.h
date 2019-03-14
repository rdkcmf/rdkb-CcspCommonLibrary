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

    module:	stun_sco_global.h

        For Simple Traversal of UDP Through NATs (STUN),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Stun Simple Client implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/30/08    initial revision.

**********************************************************************/


#ifndef  _STUN_SCO_GLOBAL_
#define  _STUN_SCO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_crypto_interface.h"
#include "ansc_crypto_external_api.h"
#include "ansc_tso_interface.h"
#include "ansc_tso_external_api.h"
#include "ansc_scuo_interface.h"
#include "ansc_scuo_external_api.h"

#include "stun_co_oid.h"
#include "stun_co_name.h"
#include "stun_co_type.h"
#include "stun_properties.h"

#include "stun_sco_interface.h"
#include "stun_sco_exported_api.h"
#include "stun_sco_internal_api.h"


#endif
