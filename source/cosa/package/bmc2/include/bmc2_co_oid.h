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

    module:	bmc2_co_oid.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Bmc2
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/20/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_CO_OID_
#define  _BMC2_CO_OID_


/***********************************************************
           GENERAL BMC2 FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  BMC2_FEATURE_OBJECT_OID_BASE               BMC2_COMPONENT_OID_BASE      + 0x1000
#define  BMC2_GENERAL_FO_OID_BASE                   BMC2_FEATURE_OBJECT_OID_BASE + 0x0000

#define  BMC2_ENV_CONTROLLER_OID                    BMC2_GENERAL_FO_OID_BASE     + 0x0001
#define  BMC2_COM_EXECUTOR_OID                      BMC2_GENERAL_FO_OID_BASE     + 0x0002
#define  BMC2_COM_TERMINAL_OID                      BMC2_GENERAL_FO_OID_BASE     + 0x0003
#define  BMC2_COM_DOMAIN_OID                        BMC2_GENERAL_FO_OID_BASE     + 0x0004
#define  BMC2_REQ_CONTROLLER_OID                    BMC2_GENERAL_FO_OID_BASE     + 0x0005


#endif
