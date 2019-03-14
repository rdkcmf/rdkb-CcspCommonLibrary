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

    module:	bwrm_properties.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Bwrm Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/07/03    initial revision.

**********************************************************************/


#ifndef  _BWRM_PROPERTIES_
#define  _BWRM_PROPERTIES_


/*
 * We have to use some of the constant values defined in the Bwrm message definitions, so we
 * include the header file here.
 */
#include  "bwrm_definitions.h"


/***********************************************************
   BWRM ENV CONTROLLER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Bwrm Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * slap object loading, the concept of "Environment Control" is really important.
 */
#define  BWRM_DEF_CACHE_ENTRY_COUNT                 8
#define  BWRM_DEF_CACHE_MEMORY_SIZE                 256 * 1024      /* 256K bytes */
#define  BWRM_DEF_CACHE_TIMEOUT                     5  * 60         /* 5 minutes - caching on embedded system has resource penalty, so keep it short */

typedef  struct
_BWRM_ENV_CONTROLLER_PROPERTY
{
    ULONG                           Dummy;
}
BWRM_ENV_CONTROLLER_PROPERTY,  *PBWRM_ENV_CONTROLLER_PROPERTY;


/***********************************************************
    BWRM FILE MANAGER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Bwrm File Manager Object maps the web file access functions to the underlying file system
 * I/O wrappers. While this object is a reasonable place for implementing binary file caching
 * functionality on top of the basic file access, it's probably better to encapsulate the binary
 * file content as part of a Cooked Page Object, so we can manage the caching control for both Raw
 * and Cooked Page in the same way.
 */
typedef  struct
_BWRM_FILE_MANAGER_PROPERTY
{
    ULONG                           Dummy;
}
BWRM_FILE_MANAGER_PROPERTY,  *PBWRM_FILE_MANAGER_PROPERTY;


/***********************************************************
    BWRM PAGE MANAGER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Bwrm Page Manager Object provides a generic dynamic page caching mechanism. By default, the
 * dynamic pages are identified by the 'page_path' field associated with web resource. It's up to
 * each dynamic Page Execution Page to construct the derived Cooked Page Objects, so they can be
 * cached and managed in the same fashion.
 */
typedef  struct
_BWRM_PAGE_MANAGER_PROPERTY
{
    BOOL                            bCacheEnabled;
    ULONG                           CacheEntryCount;
    ULONG                           CacheMemorySize;
    ULONG                           CacheTimeout;
}
BWRM_PAGE_MANAGER_PROPERTY,  *PBWRM_PAGE_MANAGER_PROPERTY;


#endif
