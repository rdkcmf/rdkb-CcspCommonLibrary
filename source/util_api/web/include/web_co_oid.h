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

    module:	web_co_oid.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Web
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/09/02    initial revision.

**********************************************************************/


#ifndef  _WEB_CO_OID_
#define  _WEB_CO_OID_


/***********************************************************
            GENERAL WEB FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  WEB_FEATURE_OBJECT_OID_BASE                WEB_COMPONENT_OID_BASE      + 0x1000
#define  WEB_GENERAL_FO_OID_BASE                    WEB_FEATURE_OBJECT_OID_BASE + 0x0000

#define  WEB_SIMPLE_CLIENT_OID                      WEB_GENERAL_FO_OID_BASE     + 0x0001
#define  WEB_SIMPLE_SERVER_OID                      WEB_GENERAL_FO_OID_BASE     + 0x0002
#define  WEB_SITE_MANAGER_OID                       WEB_GENERAL_FO_OID_BASE     + 0x0003

#define  WEB_VIRTUAL_HOST_OID                       WEB_GENERAL_FO_OID_BASE     + 0x0004
#define  WEB_RESOURCE_LOCATOR_OID                   WEB_GENERAL_FO_OID_BASE     + 0x0005
#define  WEB_URI_PATH_OID                           WEB_GENERAL_FO_OID_BASE     + 0x0006
#define  WEB_GENERAL_SESSION_OID                    WEB_GENERAL_FO_OID_BASE     + 0x0007

#define  WEB_STATUS_ATOM_TABLE_OID                  WEB_GENERAL_FO_OID_BASE     + 0x0011
#define  WEB_HEADER_ATOM_TABLE_OID                  WEB_GENERAL_FO_OID_BASE     + 0x0012

#define  WEB_RESOURCE_OWNER_OID                     WEB_GENERAL_FO_OID_BASE     + 0x0021
#define  WEB_ROO_FSM_OID                            WEB_GENERAL_FO_OID_BASE     + 0x0022
#define  WEB_ROO_CGI_OID                            WEB_GENERAL_FO_OID_BASE     + 0x0023

#define  WEB_AUTH_SERVER_OID                        WEB_GENERAL_FO_OID_BASE     + 0x0030


#endif
