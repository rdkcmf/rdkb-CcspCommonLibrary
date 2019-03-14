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

    module:	telnet_sso_co_oid.h

        For TELNET,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Telnet
        Simple Server Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        06/09/05    initial revision.

**********************************************************************/


#ifndef  _TELNET_SSO_CO_OID_
#define  _TELNET_SSO_CO_OID_


/****************************************************************
     GENERAL TELNET SIMPLE SERVER FEATURE OBJECTS DEFINITION
*****************************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */

#define  TELNET_FEATURE_OBJECT_OID_BASE             TELNET_COMPONENT_OID_BASE         + 0x1000
#define  TELNET_GENERAL_FO_OID_BASE                 TELNET_FEATURE_OBJECT_OID_BASE    + 0x0000

#define  TELNET_SIMPLE_SERVER_OID                   TELNET_GENERAL_FO_OID_BASE        + 0x0001
#define  TELNET_SRV_SESSION_OID                     TELNET_GENERAL_FO_OID_BASE        + 0x0002


#endif
