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

    module:	dslh_co_oid.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Dslh
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


#ifndef  _DSLH_CO_OID_
#define  _DSLH_CO_OID_


/***********************************************************
           GENERAL DSLH FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  DSLH_FEATURE_OBJECT_OID_BASE               DSLH_COMPONENT_OID_BASE      + 0x1000
#define  DSLH_GENERAL_FO_OID_BASE                   DSLH_FEATURE_OBJECT_OID_BASE + 0x0000

#define  DSLH_CPE_CONTROLLER_OID                    DSLH_GENERAL_FO_OID_BASE     + 0x0001
#define  DSLH_ACS_BROKER_OID                        DSLH_GENERAL_FO_OID_BASE     + 0x0002
#define  DSLH_WMP_PROCESSOR_OID                     DSLH_GENERAL_FO_OID_BASE     + 0x0003
#define  DSLH_WMP_SESSION_OID                       DSLH_GENERAL_FO_OID_BASE     + 0x0004
#define  DSLH_ACS_CONNECTION_OID                    DSLH_GENERAL_FO_OID_BASE     + 0x0005
#define  DSLH_WMP_DATABASE_OID                      DSLH_GENERAL_FO_OID_BASE     + 0x0006
#define  DSLH_SOAP_PARSER_OID                       DSLH_GENERAL_FO_OID_BASE     + 0x0007
#define  DSLH_FILE_MANAGER_OID                      DSLH_GENERAL_FO_OID_BASE     + 0x0008
#define  DSLH_COM_HELPER_OID                        DSLH_GENERAL_FO_OID_BASE     + 0x0009
#define  DSLH_DIAG_MANAGER_OID                      DSLH_GENERAL_FO_OID_BASE     + 0x000A
#define  DSLH_UPNP_MANAGER_OID                      DSLH_GENERAL_FO_OID_BASE     + 0x000B
#define  DSLH_TR64_MANAGER_OID                      DSLH_GENERAL_FO_OID_BASE     + 0x000C
#define  DSLH_STUN_MANAGER_OID                      DSLH_GENERAL_FO_OID_BASE     + 0x000D

#define  DSLH_OBJ_ENTITY_OID                        DSLH_GENERAL_FO_OID_BASE     + 0x0011
#define  DSLH_OBJ_RECORD_OID                        DSLH_GENERAL_FO_OID_BASE     + 0x0012
#define  DSLH_OBJ_CONTROLLER_OID                    DSLH_GENERAL_FO_OID_BASE     + 0x0013
#define  DSLH_OBJCO_TABLE_OID                       DSLH_GENERAL_FO_OID_BASE     + 0x0014

#define  DSLH_VAR_ENTITY_OID                        DSLH_GENERAL_FO_OID_BASE     + 0x0021
#define  DSLH_VAR_RECORD_OID                        DSLH_GENERAL_FO_OID_BASE     + 0x0022


#endif
