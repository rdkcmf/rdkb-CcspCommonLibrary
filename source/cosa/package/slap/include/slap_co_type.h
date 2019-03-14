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

    module:	slap_co_type.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object types for the Slap
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


#ifndef  _SLAP_CO_TYPE_
#define  _SLAP_CO_TYPE_


/***********************************************************
            GENERAL SLAP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  SLAP_ENV_CONTROLLER_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_MAPPER_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_BROKER_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_ENTITY_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_RECORD_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_CONTAINER_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_WRAPPER_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_AGENT_TYPE                        ANSC_OBJECT_TYPE_NULL
#define  SLAP_OBJ_PROXY_TYPE                        ANSC_OBJECT_TYPE_NULL
#define  SLAP_SRV_COMPONENT_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  SLAP_SRV_PROXY_TYPE                        ANSC_OBJECT_TYPE_NULL
#define  SLAP_LOAM_CLIENT_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  SLAP_LOAM_SERVER_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  SLAP_LOAM_BROKER_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  SLAP_ACCESS_MANAGER_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  SLAP_SCO_STANDARD_TYPE                     "standard"
#define  SLAP_SCO_BUFFER_TYPE                       "buffer"
#define  SLAP_SCO_COLLECTION_TYPE                   "collection"
#define  SLAP_SCO_GALLERY_TYPE                      "gallery"

#define  SLAP_VAR_ENTITY_TYPE                       "varEntity"
#define  SLAP_VAR_CONVERTER_TYPE                    "varConverter"
#define  SLAP_VAR_MAPPER_TYPE                       "varMapper"
#define  SLAP_VAR_HELPER_TYPE                       "varHelper"

#define  SLAP_OWO_UOAO_TYPE                         "uoao"
#define  SLAP_OWO_LOAO_TYPE                         "loao"
#define  SLAP_OWO_UOAC_TYPE                         "uoac"
#define  SLAP_OWO_LOAC_TYPE                         "loac"


#endif
