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

    module:	sys_cfg_package.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the system- and package-wide compiling
        and configuration settings.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/04/02    initial revision.

**********************************************************************/


#ifndef  _SYS_CFG_PACKAGE_
#define  _SYS_CFG_PACKAGE_


/***********************************************************
           SYS REQUIRED PACKAGE WRAPPER INFORMATION
***********************************************************/

/*
 * The Sys Package Wrapper Object serves as a standard packaging interface for different products.
 * It encapsulates all the API functions that are available for third-party developers to integrate
 * ANSC system modules into different runtime platforms.
 */
#ifndef  BISGA_PACKAGE_INFO

    #define  BISGA_PACKAGE_INFO
    #define  BISGA_PACKAGE_NAME                     "Bisga-RG-Package"
    #define  BISGA_PACKAGE_DESCRIPTION              "Gateway for broadcast access devices - Cisco Systems, Inc."
    #define  BISGA_PACKAGE_MAKER                    "Xuechen Yang"
    #define  BISGA_PACKAGE_TIMESTAMP                "05/04/03 Saturday - 09:33 AM"

#endif


#endif
