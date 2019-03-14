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

    module:	slap_bmc2ino_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Slap Bmc2 Input Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/23/05    initial revision.

**********************************************************************/


#ifndef  _SLAP_BMC2INO_INTERFACE_
#define  _SLAP_BMC2INO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scostd_interface.h"
#include "slap_scostd_exported_api.h"


/***********************************************************
         SLAP BMC2 INPUT COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  SLAP_BMC2INO_UOA_NAME                      "Services.Bmc2.Input"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2INO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2INO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2INO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2INO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BMC2INO_READ)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  char*
(*PFN_BMC2INO_READ2)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pDefaultValue,
        SLAP_BOOL                   bSizeFixed,
        SLAP_BOOL                   bMaskInput,
        SLAP_UINT32                 ulWidth
    );

/*
 * The Slap Bmc2 Input Object is used to read user input during command execution. It's primarily
 * used in building interactive menu system. For example: a GSW (Getting Started Wizard) command
 * will typically ask user a series of questions before committing the changes. Those questions are
 * presented one by one in a Q&A fashion.
 */
#define  SLAP_BMC2_INPUT_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_STANDARD_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    PFN_BMC2INO_READ                Read;                                                   \
    PFN_BMC2INO_READ2               Read2;                                                  \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_BMC2_INPUT_OBJECT
{
    SLAP_BMC2_INPUT_CLASS_CONTENT
}
SLAP_BMC2_INPUT_OBJECT,  *PSLAP_BMC2_INPUT_OBJECT;

#define  ACCESS_SLAP_BMC2_INPUT_OBJECT(p)           \
         ACCESS_CONTAINER(p, SLAP_BMC2_INPUT_OBJECT, Linkage)
#define  ACCESS_SLAP_BMC2_INPUT_FIELD(f)            \
         ACCESS_FIELD_OFFSET(SLAP_BMC2_INPUT_OBJECT, f)


#endif
