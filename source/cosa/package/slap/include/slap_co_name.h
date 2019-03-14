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

    module:	slap_co_name.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object names for the Slap
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


#ifndef  _SLAP_CO_NAME_
#define  _SLAP_CO_NAME_


/***********************************************************
            GENERAL SLAP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  SLAP_ENV_CONTROLLER_NAME                   "srvComponent"
#define  SLAP_OBJ_MAPPER_NAME                       "objMapper"
#define  SLAP_OBJ_BROKER_NAME                       "objBroker"
#define  SLAP_OBJ_ENTITY_NAME                       "objEntity"
#define  SLAP_OBJ_RECORD_NAME                       "objRecord"
#define  SLAP_OBJ_CONTAINER_NAME                    "objContainer"
#define  SLAP_OBJ_WRAPPER_NAME                      "objWrapper"
#define  SLAP_OBJ_AGENT_NAME                        "objAgent"
#define  SLAP_OBJ_PROXY_NAME                        "objProxy"
#define  SLAP_SRV_COMPONENT_NAME                    "srvComponent"
#define  SLAP_SRV_PROXY_NAME                        "srvProxy"
#define  SLAP_LOAM_CLIENT_NAME                      "loamClient"
#define  SLAP_LOAM_SERVER_NAME                      "loamServer"
#define  SLAP_LOAM_BROKER_NAME                      "loamBroker"
#define  SLAP_ACCESS_MANAGER_NAME                   "accessManager"

#define  SLAP_SCO_STANDARD_NAME                     "scoStandard"
#define  SLAP_SCO_BUFFER_NAME                       "scoBuffer"
#define  SLAP_SCO_COLLECTION_NAME                   "scoCollection"
#define  SLAP_SCO_GALLERY_NAME                      "scoGallery"

#define  SLAP_VAR_ENTITY_NAME                       "varEntity"
#define  SLAP_VAR_CONVERTER_NAME                    "varConverter"
#define  SLAP_VAR_MAPPER_NAME                       "varMapper"
#define  SLAP_VAR_HELPER_NAME                       "varHelper"

#define  SLAP_OWO_UOAO_NAME                         "owoUoao"
#define  SLAP_OWO_LOAO_NAME                         "owoLoao"
#define  SLAP_OWO_UOAC_NAME                         "owoUoac"
#define  SLAP_OWO_LOAC_NAME                         "owoLoac"


#endif
