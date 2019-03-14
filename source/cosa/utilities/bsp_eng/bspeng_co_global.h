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

    module:	bspeng_co_global.h

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file includes all the header files required by
        string implementation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan   

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/


#ifndef  _BSPENG_CO_GLOBAL_
#define  _BSPENG_CO_GLOBAL_

#include "ansc_platform.h"

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "ansc_memory_complement.h"
#include "ansc_string_util.h"

#include "ansc_ptr_array_co_interface.h"
#include "ansc_ptr_array_co_exported_api.h"

#include "ansc_byte_array_co_interface.h"
#include "ansc_byte_array_co_exported_api.h"

#include "bspeng_co_interface.h"
#include "bspeng_co_exported_api.h"
#include "bspeng_co_internal_api.h"
#include "bspeng_co_cmif.h"
#include "bspeng_var_slap_conv.h"
#include "bspeng_ifo_soa.h"


#ifndef  _BSPENG_NO_DOUBLE
#define  _ansc_strtod               strtod
#endif

#define  _BSP_TEMPLATE_ENGINE_USING_EXTERNAL_FILE

/*
#define  _BSP_SCRIPT_CASE_INSENSITIVE
*/

/*
#define  _BSP_RICH_FEATURE_SET
*/


#ifdef   _DEBUG
BOOL BspEngIsCallFlowTraced();
#endif


#endif

