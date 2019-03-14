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

    module:	beep_co_oid.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Beep
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/03/03    initial revision.

**********************************************************************/


#ifndef  _BEEP_CO_OID_
#define  _BEEP_CO_OID_


/***********************************************************
           GENERAL BEEP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  BEEP_FEATURE_OBJECT_OID_BASE               BEEP_COMPONENT_OID_BASE      + 0x1000
#define  BEEP_GENERAL_FO_OID_BASE                   BEEP_FEATURE_OBJECT_OID_BASE + 0x0000

#define  BEEP_ENV_CONTROLLER_OID                    BEEP_GENERAL_FO_OID_BASE     + 0x0001
#define  BEEP_MASTER_HOST_OID                       BEEP_GENERAL_FO_OID_BASE     + 0x0002
#define  BEEP_MASTER_SERVLET_OID                    BEEP_GENERAL_FO_OID_BASE     + 0x0003
#define  BEEP_CLIENT_SESSION_OID                    BEEP_GENERAL_FO_OID_BASE     + 0x0004
#define  BEEP_SRV_CONTROLLER_OID                    BEEP_GENERAL_FO_OID_BASE     + 0x0005
#define  BEEP_REQ_CONTROLLER_OID                    BEEP_GENERAL_FO_OID_BASE     + 0x0006
#define  BEEP_PAGE_CONTAINER_OID                    BEEP_GENERAL_FO_OID_BASE     + 0x0007
#define  BEEP_SERVICE_BEAN_OID                      BEEP_GENERAL_FO_OID_BASE     + 0x0008


#endif
