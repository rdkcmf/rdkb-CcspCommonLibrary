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

    module:	egfwm_definitions_custom.h

        For Enterprised Firewall Engine (EGFWM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file holds all custom definitions for egfwm.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        06/13/2008      initial revision.

**********************************************************************/


#ifndef  _EGFWM_DEFINITIONS_CUSTOM_
#define  _EGFWM_DEFINITIONS_CUSTOM_


#define  EGFWM_MAX_SESSION_NUMBER                   20480
#define  EGFWM_MAX_SESSION_REMOVE                   50

#define  EGFWM_MAX_WAITING_SESSION_NUMBER           32

/*
 *  session hash table size
 */
#define  EGFWM_MAXIMUM_TABLE_SIZE                   3301          /*4001*/

#define  EGFWM_ALG_FTP_PREALLOC_SESSION_NUMBER      16


#endif

