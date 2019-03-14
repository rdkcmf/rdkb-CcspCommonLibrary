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

    module:	web_co_type.h

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object types for the Web
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


#ifndef  _WEB_CO_TYPE_
#define  _WEB_CO_TYPE_


/***********************************************************
            GENERAL WEB FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  WEB_SIMPLE_CLIENT_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  WEB_SIMPLE_SERVER_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  WEB_SITE_MANAGER_TYPE                      ANSC_OBJECT_TYPE_NULL

#define  WEB_VIRTUAL_HOST_TYPE                      ANSC_OBJECT_TYPE_NULL
#define  WEB_RESOURCE_OWNER_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  WEB_URI_PATH_TYPE                          ANSC_OBJECT_TYPE_NULL
#define  WEB_GENERAL_SESSION_TYPE                   ANSC_OBJECT_TYPE_NULL

#define  WEB_STATUS_ATOM_TABLE_TYPE                 "status"
#define  WEB_HEADER_ATOM_TABLE_TYPE                 "header"

#define  WEB_RESOURCE_LOCATOR_TYPE                  ANSC_OBJECT_TYPE_NULL
#define  WEB_ROO_FSM_TYPE                           "fsm"
#define  WEB_ROO_CGI_TYPE                           "cgi"


#endif
