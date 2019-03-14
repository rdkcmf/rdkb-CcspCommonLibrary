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

    module:	dslh_definitions.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Dslh Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/30/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_DEFINITIONS_
#define  _DSLH_DEFINITIONS_


#define  DSLH_MAX_URL_SIZE                          256
#define  DSLH_MAX_HOSTNAME_SIZE                     64
#define  DSLH_MAX_CONNNAME_SIZE                     256
#define  DSLH_MAX_ZONENAME_SIZE                     64
#define  DSLH_MAX_LINKNAME_SIZE                     64
#define  DSLH_MAX_USERNAME_SIZE                     256
#define  DSLH_MAX_PASSWORD_SIZE                     256
#define  DSLH_MAX_DESCRIPTION_SIZE                  256
#define  DSLH_MAX_PROVIDER_SIZE                     256
#define  DSLH_MAX_BRIDGENAME_SIZE                   64
#define  DSLH_MAX_INTERFACE_SIZE                    256
#define  DSLH_MAX_APPNAME_SIZE                      64
#define  DSLH_MAX_DHCPOPTION_SIZE                   256
#define  DSLH_MAX_CR_URL_PATH_SIZE                  64

#include "dslh_custom_definitions.h"
#include "dslh_definitions_cwmp.h"
#include "dslh_definitions_database.h"
/*
#include "dslh_definitions_general.h"
#include "dslh_definitions_diagnostics.h"
#include "dslh_definitions_forwarding.h"
#include "dslh_definitions_ethernet.h"
#include "dslh_definitions_usb.h"
#include "dslh_definitions_wlan.h"
#include "dslh_definitions_dsl.h"
#include "dslh_definitions_landevice.h"
#include "dslh_definitions_wandevice.h"
#include "dslh_definitions_igd.h"
#include "dslh_definitions_bridging.h"
#include "dslh_definitions_queuing.h"
#include "dslh_definitions_voice.h"
#include "dslh_definitions_igmp.h"
#include "dslh_definitions_datapath.h"
*/

#endif
