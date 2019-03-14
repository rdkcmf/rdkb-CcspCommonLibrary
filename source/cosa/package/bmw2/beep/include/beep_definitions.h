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

    module:	beep_definitions.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the BEEP Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/23/03    initial revision.

**********************************************************************/


#ifndef  _BEEP_DEFINITIONS_
#define  _BEEP_DEFINITIONS_


#include "slap_definitions.h"


/***********************************************************
         BEEP STANDARD ELEMENT PARAMETERS DEFINITION
***********************************************************/

/*
 * BEEP is where the BSP control mechanism is implemented. It's a control plane build on top the
 * underlying SLAP layer. Contrast to the amount of knowledge required to author BSP pages, the
 * BEEP interfaces provided to the BSP Run-time Execution Engine (BREE) are surprisingly simple.
 * This is the result of a modulized architecture design.
 */
#define  BEEP_ENGINE_TYPE_utilizeCache              0x00000001
#define  BEEP_ENGINE_TYPE_resourceFilter            0x00000002
#define  BEEP_ENGINE_TYPE_wildcard                  0x00010000

#define  BEEP_PAGE_TYPE_serverPage                  1
#define  BEEP_PAGE_TYPE_staticFile                  2

#define  BEEP_PAGE_EXTENSION_SEPARATORS             ".,;* "

typedef  struct
_BEEP_ENGINE_DESCRIPTOR
{
    char*                           EngineName;                 /* arbitrary engine name, such as "BREE v2.0"        */
    char*                           PageExtensions;             /* associated file extensions, such as ".bsp, .html" */
    ULONG                           EngineType;                 /* specifies engine's operational properties         */
    BOOL                            bAggrWrites;                /* specifies whether write ops should be aggregated  */
    ANSC_HANDLE                     hEngineContext;             /* page execution context, passed back to Execute()  */

    void*                           PreparePage;                /* callback function for preparing the static page   */
    void*                           ExecutePage;                /* execution callback for dynamic page processing    */
    void*                           QualifyPage;                /* qualify page type by examine the page name (.ext) */
    void*                           ConvertPath;                /* web resource filtering by translating page path   */
}
BEEP_ENGINE_DESCRIPTOR,  *PBEEP_ENGINE_DESCRIPTOR;

/*
 * SLAP maintains all the system-created object containers in a one dimensional table sorted by
 * containers' names; while external modules may dynamically create containers in runtime, but
 * these containers may be isolated from the internal management. BEEP creates several different
 * type of containers, each of them has a different scope.
 */
#define  BEEP_SLAP_CONTAINER_APP                    "Beep.Application"
#define  BEEP_SLAP_CONTAINER_SERVER                 "Beep.Server"
#define  BEEP_SLAP_CONTAINER_PAGE                   "Beep.Page"
#define  BEEP_SLAP_CONTAINER_REQUEST                "Beep.Request"
#define  BEEP_SLAP_CONTAINER_SESSION                "Beep.Session"

/*
 * 'Session' and 'Page' objects enable a powerful scripting environment with much finer control
 * over the scope and context in which SLAP objects may live in. In general, all the activities
 * generated by the same machine/host (to the same site) will be processed within the same 'Session'
 * context. Each 'Session' may maintain multiple 'Page' contexts.
 */


#endif
