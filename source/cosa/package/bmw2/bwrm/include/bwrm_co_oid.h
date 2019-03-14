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

    module:	bwrm_co_oid.h

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Bwrm
        Component Objects.

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


#ifndef  _BWRM_CO_OID_
#define  _BWRM_CO_OID_


/***********************************************************
           GENERAL BWRM FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  BWRM_FEATURE_OBJECT_OID_BASE               BWRM_COMPONENT_OID_BASE      + 0x1000
#define  BWRM_GENERAL_FO_OID_BASE                   BWRM_FEATURE_OBJECT_OID_BASE + 0x0000

#define  BWRM_ENV_CONTROLLER_OID                    BWRM_GENERAL_FO_OID_BASE     + 0x0001
#define  BWRM_COOKED_PAGE_OID                       BWRM_GENERAL_FO_OID_BASE     + 0x0002
#define  BWRM_FILE_MANAGER_OID                      BWRM_GENERAL_FO_OID_BASE     + 0x0003
#define  BWRM_PAGE_MANAGER_OID                      BWRM_GENERAL_FO_OID_BASE     + 0x0004


#endif
