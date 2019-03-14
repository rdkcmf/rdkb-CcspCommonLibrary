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

    module:	http_hco_global.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        the Http Helper Container implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_HCO_GLOBAL_
#define  _HTTP_HCO_GLOBAL_


#include "ansc_platform.h"
#include "ansc_object_mapper_interface.h"
#include "ansc_object_mapper_external_api.h"
#include "ansc_ipc_interface.h"
#include "ansc_ipc_external_api.h"
#include "ansc_ipc_common_params.h"
#include "ansc_oco_interface.h"
#include "ansc_oco_external_api.h"
#include "ansc_smtco_interface.h"
#include "ansc_smtco_external_api.h"

#include "http_co_oid.h"
#include "http_co_name.h"
#include "http_co_type.h"
#include "http_properties.h"

#include "http_hco_interface.h"
#include "http_hco_exported_api.h"
#include "http_hco_internal_api.h"

#include "http_atostatus_interface.h"
#include "http_atostatus_exported_api.h"
#include "http_atoheader_interface.h"
#include "http_atoheader_exported_api.h"
#include "http_atomethod_interface.h"
#include "http_atomethod_exported_api.h"
#include "http_atocgienv_interface.h"
#include "http_atocgienv_exported_api.h"
#include "http_atofxcat1_interface.h"
#include "http_atofxcat1_exported_api.h"
#include "http_atofxcat2_interface.h"
#include "http_atofxcat2_exported_api.h"


#endif
