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

    module:	dslh_varro_layout.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the configuration layout that is
        implemented by the Dslh Var Record in System Repository.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/28/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_VARRO_LAYOUT_
#define  _DSLH_VARRO_LAYOUT_


/***********************************************************
       DSLH VAR RECORD CONFIGURATION LAYOUT DEFINITION
***********************************************************/

/*
 * All the configuration parameters are stored under /Configuration/Setup/this-module, with which
 * a Repository Auditor Interface is associated. Following folders will be created at the root
 * level to categorize the configuration settings:
 *
 *      $ /Configuration/Setup/WmpDatabase/...
 */
#define  DSLH_VARRO_RR_NAME_Value                   "Value"
#define  DSLH_VARRO_RR_NAME_Notification            "Notification"
#define  DSLH_VARRO_RR_NAME_AccessList              "AccessList"


#endif
