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

    module: ldap_mepo_co_oid.h

        For Lightweight Directory Access Protocol (V3) (LDAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the Object Id for the MEP object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Arun Vijay Mani

    ---------------------------------------------------------------

    revision:

        12/28/05    initial revision.

**********************************************************************/


#ifndef  _LDAP_MEPO_CO_OID_
#define  _LDAP_MEPO_CO_OID_


/****************************************************************
            GENERAL TFTP SIMPLE SERVER FEATURE OBJECTS DEFINITION
*****************************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define	 LDAP_MEPO_COMPONENT_OID_BASE				0x0B040000	
#define  LDAP_MEPO_CO_OID				            LDAP_MEPO_COMPONENT_OID_BASE    + 0x1000

#endif
