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

    module:	beep_ifo_ser.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BEEP Service Engine Registration Objects.

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


#ifndef  _BEEP_IFO_SER_
#define  _BEEP_IFO_SER_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
    BEEP SERVICE ENGINE REGISTRATION INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BEEP_SER_INTERFACE_NAME                    "beepServiceEngineRegistrationIf"
#define  BEEP_SER_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BEEPSERIF_ATTACH)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEngineDescr
    );

typedef  ANSC_STATUS
(*PFN_BEEPSERIF_DETACH)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       engine_name
    );

/*
 * Instead of hardcode the list of dynamic Page Execution Engines, BEEP provides the SER interface
 * to allow service modules to dynamically register external execution engines. These registered
 * engines will be instantiated internally as the Beep Service Bean Objects.
 */
#define  BEEP_SER_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BEEPSERIF_ATTACH            AttachEngine;                                           \
    PFN_BEEPSERIF_DETACH            DetachEngine;                                           \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_SER_INTERFACE
{
    BEEP_SER_INTERFACE_CLASS_CONTENT
}
BEEP_SER_INTERFACE,  *PBEEP_SER_INTERFACE;

#define  ACCESS_BEEP_SER_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BEEP_SER_INTERFACE, Linkage)


#endif
