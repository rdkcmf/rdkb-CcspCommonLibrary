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

    module:	web_co_name.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object names for the Web
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


#ifndef  _WEB_CO_NAME_
#define  _WEB_CO_NAME_


/***********************************************************
            GENERAL WEB FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  WEB_SIMPLE_CLIENT_NAME                     "simpleClient"
#define  WEB_SIMPLE_SERVER_NAME                     "simpleServer"
#define  WEB_SITE_MANAGER_NAME                      "siteManager"

#define  WEB_VIRTUAL_HOST_NAME                      "virtualHost"
#define  WEB_RESOURCE_LOCATOR_NAME                  "resourceLocator"
#define  WEB_URI_PATH_NAME                          "uriPath"
#define  WEB_GENERAL_SESSION_NAME                   "generalSession"

#define  WEB_STATUS_ATOM_TABLE_NAME                 "statusAtomTable"
#define  WEB_HEADER_ATOM_TABLE_NAME                 "headerAtomTable"

#define  WEB_RESOURCE_OWNER_NAME                    "resourceOwner"
#define  WEB_ROO_FSM_NAME                           "rooFsm"
#define  WEB_ROO_CGI_NAME                           "rooCgi"

#define  WEB_AUTH_SERVER_NAME                       "authServer"


#endif
