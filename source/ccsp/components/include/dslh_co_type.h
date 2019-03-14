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

    module:	dslh_co_type.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object types for the Dslh
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/30/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_CO_TYPE_
#define  _DSLH_CO_TYPE_


/***********************************************************
            GENERAL DSLH FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  DSLH_CPE_CONTROLLER_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  DSLH_ACS_BROKER_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  DSLH_WMP_PROCESSOR_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  DSLH_WMP_SESSION_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  DSLH_ACS_CONNECTION_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  DSLH_WMP_DATABASE_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  DSLH_SOAP_PARSER_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  DSLH_FILE_MANAGER_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  DSLH_COM_HELPER_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  DSLH_DIAG_MANAGER_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  DSLH_UPNP_MANAGER_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  DSLH_TR64_MANAGER_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  DSLH_STUN_MANAGER_TYPE                     ANSC_OBJECT_TYPE_NULL

#define  DSLH_OBJ_ENTITY_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  DSLH_OBJ_RECORD_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  DSLH_OBJ_CONTROLLER_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  DSLH_OBJCO_TABLE_TYPE                      "table"

#define  DSLH_VAR_ENTITY_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  DSLH_VAR_RECORD_TYPE                       ANSC_OBJECT_TYPE_NULL


#endif
