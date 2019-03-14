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

    module:	slap_scocol_interface.h

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Slap Sco Collection Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/13/03    initial revision.

**********************************************************************/


#ifndef  _SLAP_SCOCOL_INTERFACE_
#define  _SLAP_SCOCOL_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "slap_scostd_interface.h"
#include "slap_scostd_exported_api.h"


/***********************************************************
       SLAP STANDARD SCO COLLECTION OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SLAPSCOCOL_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCOCOL_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SLAPSCOCOL_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SLAPSCOCOL_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  SLAP_UINT32
(*PFN_SLAPSCOCOL_GET_COUNT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  SLAP_VARIABLE*
(*PFN_SLAPSCOCOL_GET_ITEM1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  SLAP_VARIABLE*
(*PFN_SLAPSCOCOL_SET_ITEM1)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  SLAP_VARIABLE*
(*PFN_SLAPSCOCOL_GET_ITEM2)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

typedef  SLAP_VARIABLE*
(*PFN_SLAPSCOCOL_SET_ITEM2)
    (
        ANSC_HANDLE                 hThisObject,
        SLAP_PARAMETER_LIST*        param_list
    );

/*
 * The SLAP Sco Collection Object is derived from the SLAP Sco Standard Object. It encapsulates the
 * generic data access interface to any collection type of data structure: array, row, table_2d,
 * table_3d... etc. Unlike the SLAP Sco Standard Object, which serves as the base object for most
 * SLAP service objects, the Collection object serves for a more specific purpose.
 */
#define  SLAP_SCO_COLLECTION_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    SLAP_SCO_STANDARD_CLASS_CONTENT                                                         \
    /* start of object class content */                                                     \
    PFN_SLAPSCOCOL_GET_COUNT        GetCount;                                               \
    PFN_SLAPSCOCOL_GET_ITEM1        GetItem;                                                \
    PFN_SLAPSCOCOL_SET_ITEM1        SetItem;                                                \
    PFN_SLAPSCOCOL_GET_ITEM2        GetItemByIndex;                                         \
    PFN_SLAPSCOCOL_SET_ITEM2        SetItemByIndex;                                         \
    /* end of object class content */                                                       \

typedef  struct
_SLAP_SCO_COLLECTION_OBJECT
{
    SLAP_SCO_COLLECTION_CLASS_CONTENT
}
SLAP_SCO_COLLECTION_OBJECT,  *PSLAP_SCO_COLLECTION_OBJECT;

#define  ACCESS_SLAP_SCO_COLLECTION_OBJECT(p)       \
         ACCESS_CONTAINER(p, SLAP_SCO_COLLECTION_OBJECT, Linkage)
#define  ACCESS_SLAP_SCO_COLLECTION_FIELD(f)        \
         ACCESS_FIELD_OFFSET(SLAP_SCO_COLLECTION_OBJECT, f)


#endif
