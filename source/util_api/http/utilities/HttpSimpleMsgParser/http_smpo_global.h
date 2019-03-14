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

    module:	http_smpo_global.h

        For HTTP Simple Message Parser (syntax parser),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        HTTP message parser implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        02/22/02    initial revision.

    ---------------------------------------------------------------

    last modified:  
    
        02/22/02

**********************************************************************/


#ifndef  _HTTP_SMPO_GLOBAL_
#define  _HTTP_SMPO_GLOBAL_

#include "ansc_platform.h"

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "http_definitions.h"

#include "ansc_string_util.h"           /* for String <==> Number */
#include "ansc_utilbox_api.h"           /* IANA Media TYPE <==> NAME */

#include "http_smpo_util_def.h"
#include "http_smpo_interface.h"
#include "http_smpo_exported_api.h"
#include "http_smpo_internal_api.h"
#include "http_smpo_parse.h"
#include "http_smpo_parse_helper.h"
#include "http_smpo_util.h"
#include "http_smpo_construct.h"

#include "http_mbo_interface.h"
#include "http_mbo_exported_api.h"

#endif
