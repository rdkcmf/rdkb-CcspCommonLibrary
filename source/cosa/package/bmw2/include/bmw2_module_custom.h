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

    module: bmw2_module_custom.h

        For DSL Broadband Model Web (BMW2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file gives the custom module definitions for
        BMW2.

    ---------------------------------------------------------------

    environment:

        platform dependent

    ---------------------------------------------------------------

    author:

        Kang Quan

**********************************************************************/

#ifndef  _BMW2_MODULE_CUSTOM_H_
#define  _BMW2_MODULE_CUSTOM_H_


/* To enable BSP page caching in BMW2, define the following symbol */

/*
#define   _BMW2_BSP_PAGE_CACHING_ENABLED 
*/


/* The following symbol is defined if pre-compiled BSP pages are stored in files
 * instead of original script files.
 */

#define   _USE_PRECOMPILED_BSP_PAGES

#endif



