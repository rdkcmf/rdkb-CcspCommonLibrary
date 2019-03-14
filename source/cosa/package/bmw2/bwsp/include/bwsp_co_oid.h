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

    module:	bwsp_co_oid.h

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object ids for the Bwsp
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#ifndef  _BWSP_CO_OID_
#define  _BWSP_CO_OID_


/***********************************************************
           GENERAL BWSP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  BWSP_FEATURE_OBJECT_OID_BASE               BWSP_COMPONENT_OID_BASE      + 0x1000
#define  BWSP_GENERAL_FO_OID_BASE                   BWSP_FEATURE_OBJECT_OID_BASE + 0x0000

#define  BWSP_SITE_CREATOR_OID                      BWSP_GENERAL_FO_OID_BASE     + 0x0001
#define  BWSP_PORTAL_HOST_OID                       BWSP_GENERAL_FO_OID_BASE     + 0x0002
#define  BWSP_WEB_SERVLET_OID                       BWSP_GENERAL_FO_OID_BASE     + 0x0003


#endif
