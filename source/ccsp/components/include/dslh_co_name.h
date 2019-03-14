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

    module:	dslh_co_name.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object names for the Dslh
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


#ifndef  _DSLH_CO_NAME_
#define  _DSLH_CO_NAME_


/***********************************************************
           GENERAL DSLH FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  DSLH_CPE_CONTROLLER_NAME                   "cpeController"
#define  DSLH_ACS_BROKER_NAME                       "acsBroker"
#define  DSLH_WMP_PROCESSOR_NAME                    "wmpProcessor"
#define  DSLH_WMP_SESSION_NAME                      "wmpSession"
#define  DSLH_ACS_CONNECTION_NAME                   "acsConnection"
#define  DSLH_WMP_DATABASE_NAME                     "wmpDatabase"
#define  DSLH_SOAP_PARSER_NAME                      "soapParser"
#define  DSLH_FILE_MANAGER_NAME                     "fileManager"
#define  DSLH_COM_HELPER_NAME                       "comHelper"
#define  DSLH_DIAG_MANAGER_NAME                     "diagManager"
#define  DSLH_UPNP_MANAGER_NAME                     "upnpManager"
#define  DSLH_TR64_MANAGER_NAME                     "tr64Manager"
#define  DSLH_STUN_MANAGER_NAME                     "stunManager"

#define  DSLH_OBJ_ENTITY_NAME                       "objEntity"
#define  DSLH_OBJ_RECORD_NAME                       "objRecord"
#define  DSLH_OBJ_CONTROLLER_NAME                   "objController"
#define  DSLH_OBJCO_TABLE_NAME                      "objcoTable"

#define  DSLH_VAR_ENTITY_NAME                       "varEntity"
#define  DSLH_VAR_RECORD_NAME                       "varRecord"


#endif
