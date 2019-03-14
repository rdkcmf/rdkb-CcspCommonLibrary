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

    module:	beep_properties.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the Beep Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/07/03    initial revision.

**********************************************************************/


#ifndef  _BEEP_PROPERTIES_
#define  _BEEP_PROPERTIES_


/*
 * We have to use some of the constant values defined in the Beep message definitions, so we
 * include the header file here.
 */
#include  "beep_definitions.h"
#include  "web_properties.h"


/***********************************************************
   BEEP ENV CONTROLLER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * The Beep Env Controller Object is responsible for creating the runtime environment, performing
 * system initialization, invoke a few key system components, and tearing down the system. While
 * it doesn't provide the real binary-level separation and runtime environment management, such as
 * beep object loading, the concept of "Environment Control" is really important.
 */
typedef  struct
_BEEP_ENV_CONTROLLER_PROPERTY
{
    ULONG                           Dummy;
}
BEEP_ENV_CONTROLLER_PROPERTY,  *PBEEP_ENV_CONTROLLER_PROPERTY;


/***********************************************************
    BEEP MASTER HOST CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * Beep Master Host Objet is a derived instance of Bwsp Portal Host Object, though this derived
 * relationship is not very clear duo to the fact that (1) the BWSP_WSR interface separates the
 * internal Web Host implementation from the external modules; (2) the derived host objects only
 * provide session-tracking functionality.
 */
#define  BEEP_DEF_MASTER_HOST_NAME                  "beep.host"

typedef  struct
_BEEP_MASTER_HOST_PROPERTY
{
    char                            HostName[WEB_MAX_HOST_NAME_SIZE];
    USHORT                          HostPort;
    BOOL                            bSessionTracking;
    BOOL                            bDefaultHost;
}
BEEP_MASTER_HOST_PROPERTY,  *PBEEP_MASTER_HOST_PROPERTY;


/***********************************************************
   BEEP MASTER SERVLET CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * Beep Master Servlet Objet is a derived instance of Bwsp Web Servlet Object, though this derived
 * relationship is not very clear duo to the fact that the BWSP_WSR interface separates the
 * internal Web Servlet implementation from the external modules. Note that unlike the BWSP module
 * and the underlying Web Server module, BEEP has intentionally REMOVED the hierarchical structure
 * for request handling. Beep Master Servlet literally presents a a file-based web resource owner
 * interface to the web server.
 */
#define  BEEP_DEF_MASTER_WEBROOT_PATH               "/"
#define  BEEP_DEF_MASTER_SERVLET_PATH               "beep"
#define  BEEP_DEF_MAIN_PAGE_PATH                    "main.bsp"

typedef  struct
_BEEP_MASTER_SERVLET_PROPERTY
{
    char                            WebRootPath[ANSC_MAX_FILE_NAME_SIZE];
    char                            ServletPath[WEB_MAX_PATH_NAME_SIZE ];
    char                            DefPagePath[ANSC_MAX_FILE_NAME_SIZE];
    BOOL                            bMainPage;
    BOOL                            bUpnpSupport;
}
BEEP_MASTER_SERVLET_PROPERTY,  *PBEEP_MASTER_SERVLET_PROPERTY;


/***********************************************************
   BEEP SRV CONTROLLER CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * Beep Srv Controller Objet is responsible for managing multiple runtime Page Execution Engines,
 * which are instantiated by the Beep Service Bean Object. This object dispatches the HTTP requests
 * solely based on the file extensions (e.g., .bsp, .html, .xml and so on). The owner object is
 * responsible for breaking down a registered engine into multiple service beans.
 */
typedef  struct
_BEEP_SRV_CONTROLLER_PROPERTY
{
    ULONG                           Dummy;
}
BEEP_SRV_CONTROLLER_PROPERTY,  *PBEEP_SRV_CONTROLLER_PROPERTY;


/***********************************************************
    BEEP SERVICE BEAN CONFIGURATION PARAMETERS DEFINITION
***********************************************************/

/*
 * Beep Service Bean Objet must be instantiated for every registered file extension type. That is,
 * if an external Page Execution Engine registers a 'PageExtensions' field containing multiple file
 * extensions, a separate Beep Service Bean Object must be created for every one of them. Other
 * than that, this object is a classic place-holder object, which serves as a buffer zone between
 * the internal object-oriented environment and the external system.
 */
#define  BEEP_ENGINE_NAME_SIZE                      32
#define  BEEP_PAGE_NAME_EXT_SIZE                    128

typedef  struct
_BEEP_SERVICE_BEAN_PROPERTY
{
    char                            EngineName    [BEEP_ENGINE_NAME_SIZE  ];
    char                            PageExtensions[BEEP_PAGE_NAME_EXT_SIZE];
    ULONG                           EngineType;
    BOOL                            bAggrWrites;
}
BEEP_SERVICE_BEAN_PROPERTY,  *PBEEP_SERVICE_BEAN_PROPERTY;


#endif
