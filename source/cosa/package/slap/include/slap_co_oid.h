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

    module:	slap_co_oid.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Slap
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_CO_OID_
#define  _SLAP_CO_OID_


/***********************************************************
           GENERAL SLAP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  SLAP_FEATURE_OBJECT_OID_BASE               SLAP_COMPONENT_OID_BASE      + 0x1000
#define  SLAP_GENERAL_FO_OID_BASE                   SLAP_FEATURE_OBJECT_OID_BASE + 0x0000

#define  SLAP_ENV_CONTROLLER_OID                    SLAP_GENERAL_FO_OID_BASE     + 0x0001
#define  SLAP_OBJ_MAPPER_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0002
#define  SLAP_OBJ_BROKER_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0003
#define  SLAP_OBJ_ENTITY_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0004
#define  SLAP_OBJ_RECORD_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0005
#define  SLAP_OBJ_CONTAINER_OID                     SLAP_GENERAL_FO_OID_BASE     + 0x0006
#define  SLAP_OBJ_WRAPPER_OID                       SLAP_GENERAL_FO_OID_BASE     + 0x0007
#define  SLAP_OBJ_AGENT_OID                         SLAP_GENERAL_FO_OID_BASE     + 0x0008
#define  SLAP_OBJ_PROXY_OID                         SLAP_GENERAL_FO_OID_BASE     + 0x0009
#define  SLAP_SRV_COMPONENT_OID                     SLAP_GENERAL_FO_OID_BASE     + 0x000A
#define  SLAP_SRV_PROXY_OID                         SLAP_GENERAL_FO_OID_BASE     + 0x000B
#define  SLAP_LOAM_CLIENT_OID                       SLAP_GENERAL_FO_OID_BASE     + 0x000C
#define  SLAP_LOAM_SERVER_OID                       SLAP_GENERAL_FO_OID_BASE     + 0x000D
#define  SLAP_LOAM_BROKER_OID                       SLAP_GENERAL_FO_OID_BASE     + 0x000E
#define  SLAP_ACCESS_MANAGER_OID                    SLAP_GENERAL_FO_OID_BASE     + 0x000F

#define  SLAP_SCO_STANDARD_OID                      SLAP_GENERAL_FO_OID_BASE     + 0x0011
#define  SLAP_SCO_BUFFER_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0012
#define  SLAP_SCO_COLLECTION_OID                    SLAP_GENERAL_FO_OID_BASE     + 0x0013
#define  SLAP_SCO_GALLERY_OID                       SLAP_GENERAL_FO_OID_BASE     + 0x0014

#define  SLAP_VAR_ENTITY_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0021
#define  SLAP_VAR_CONVERTER_OID                     SLAP_GENERAL_FO_OID_BASE     + 0x0022
#define  SLAP_VAR_MAPPER_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0023
#define  SLAP_VAR_HELPER_OID                        SLAP_GENERAL_FO_OID_BASE     + 0x0024

#define  SLAP_OWO_UOAO_OID                          SLAP_GENERAL_FO_OID_BASE     + 0x0031
#define  SLAP_OWO_LOAO_OID                          SLAP_GENERAL_FO_OID_BASE     + 0x0032
#define  SLAP_OWO_UOAC_OID                          SLAP_GENERAL_FO_OID_BASE     + 0x0033
#define  SLAP_OWO_LOAC_OID                          SLAP_GENERAL_FO_OID_BASE     + 0x0034


#endif
