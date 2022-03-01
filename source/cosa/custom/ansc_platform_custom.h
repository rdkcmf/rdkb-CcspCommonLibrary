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

    module: ansc_platform_custom.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file gives the custom module definitions for
        ANSC platform.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Ding Hua

**********************************************************************/

#ifndef  _ANSC_PLATFORM_CUSTOM_H_
#define  _ANSC_PLATFORM_CUSTOM_H_


/***************************************************************
        Conditional compilation flags for ansc_memroy.h
***************************************************************/

/*
 *  _ANSC_MALLOC_METHOD1:   no any memory tracking ability
 *  _ANSC_MALLOC_METHOD2:   
 *  _ANSC_MALLOC_METHOD3:   
 *  _ANSC_MALLOC_METHOD4:   
 *  _ANSC_MALLOC_METHOD5:   memory tracking, plus overwritting detection
 *  _ANSC_MALLOC_METHOD6:   memory tracking, plus memory pooling
 */
    #define  _ANSC_MALLOC_METHOD1


 /***************************************************************
        Conditional compilation flags for ansc_time.h
***************************************************************/

/*
 *  Indicates whether ANSC should maintain the time zone info
 */

/***************************************************************
            Generic Conditional Compilation Flags
***************************************************************/

/*#define _ANSC_TRACE_PACKET_*/

/***************************************************************
            Generic Conditional Compilation Flags
***************************************************************/

#define  _CCSP_CWMP_STUN_ENABLED

#include "dslgm_module_custom.h"

#ifdef   _BBHM_WEB_GUI_REQUIRED
	#define  _ANSC_HTTP_CGI_USED_
	#define  _ANSC_HTTP_FILE_USED_
#endif

#endif

