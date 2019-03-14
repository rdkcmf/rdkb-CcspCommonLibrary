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

    module:	slap_bmc2srvo_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Slap Bmc2 Server Object.

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


#ifndef  _SLAP_BMC2SRVO_INTERFACE_
#define  _SLAP_BMC2SRVO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scostd_interface.h"
#include "slap_scostd_exported_api.h"


/***********************************************************
         SLAP BMC2 SERVER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */
#define  SLAP_BMC2SRVO_UOA_NAME                     "Services.Bmc2.Server"

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BMC2SRVO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2SRVO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BMC2SRVO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BMC2SRVO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_BMC2SRVO_EXECUTE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

typedef  ANSC_STATUS
(*PFN_BMC2SRVO_TRANSFER)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

/*
 * The Slap Bmc2 Server Object is designed for carrying out specific tasks on the server, in
 * particular those that relate to the server's environment and processing activities. For this
 * reason it has only a single property that provides information. However, there are several
 * methods that can be used to format data in server-specific ways, manage execution of other
 * pages, manage execution of external objects and components, and assist in handling errors.
 */
#define  SLAP_BMC2_SERVER_CLASS_CONTENT                                                     \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_STANDARD_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    PFN_BMC2SRVO_EXECUTE            Execute;                                                \
    PFN_BMC2SRVO_TRANSFER           Transfer;                                               \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_BMC2_SERVER_OBJECT
{
    SLAP_BMC2_SERVER_CLASS_CONTENT
}
SLAP_BMC2_SERVER_OBJECT,  *PSLAP_BMC2_SERVER_OBJECT;

#define  ACCESS_SLAP_BMC2_SERVER_OBJECT(p)          \
         ACCESS_CONTAINER(p, SLAP_BMC2_SERVER_OBJECT, Linkage)
#define  ACCESS_SLAP_BMC2_SERVER_FIELD(f)           \
         ACCESS_FIELD_OFFSET(SLAP_BMC2_SERVER_OBJECT, f)


#endif
