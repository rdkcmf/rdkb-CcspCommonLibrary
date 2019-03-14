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

    module:	ansc_platform.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file only includes other wrapper header files,
        and it's the only wrapper header other project should
        include.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/02/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_PLATFORM_
#define  _ANSC_PLATFORM_

#include "ansc_wrapper_base.h"

#include "ansc_platform_custom.h"

#include "ansc_status.h"
#include "ansc_common_structures.h"
#include "ansc_runtime.h"
#include "ansc_protection.h"
#include "ansc_profiling.h"
//#include "ansc_memory.h"
#include "ccsp_memory.h"
#include "ccsp_trace.h"
#include "ansc_string.h"
#include "ansc_debug.h"
#include "ansc_time.h"
#include "ansc_object.h"
#include "ansc_task.h"
#include "ansc_file_io.h"
#include "ansc_url_parsing.h"
#include "ansc_profiling.h"
#include "ansc_packet_descriptor.h"
#include "ansc_buffer_descriptor.h"
#include "ansc_iana_number.h"
#include "ansc_mime_definitions.h"
#include "ansc_uri_definitions.h"
#include "ansc_asn1_definitions.h"
#include "ansc_cert_definitions.h"
#include "ansc_ipc_definitions.h"
#include "ansc_socket.h"
#include "ansc_wrapper_api.h"

#include "ansc_external_storage.h"
#include "ansc_packet_binary.h"
#include "ansc_network.h"
#include "ansc_crypto.h"
#include "ansc_element_definitions.h"
#include "ansc_popular_definitions.h"

#include "ansc_http_parser.h"

#endif
