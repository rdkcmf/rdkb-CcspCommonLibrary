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

    module:	dslh_ifo_rvq.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Dslh Runtime Variable Query Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/11/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_IFO_RVQ_
#define  _DSLH_IFO_RVQ_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
      DSLH RUNTIME VARIABLE QUERY INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  DSLH_RVQ_INTERFACE_NAME                    "dslhRuntimeVariableQueryIf"
#define  DSLH_RVQ_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  BOOL
(*PFN_DSLHRVQIF_IS_UPDATED)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHRVQIF_ClEAR_UPDATED)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  SLAP_VARIABLE*
(*PFN_DSLHRVQIF_GET_VALUE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  char*
(*PFN_DSLHRVQIF_GET_STRING)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  int
(*PFN_DSLHRVQIF_GET_INT)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ULONG
(*PFN_DSLHRVQIF_GET_UINT32)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  BOOL
(*PFN_DSLHRVQIF_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  SLAP_UCHAR_ARRAY*
(*PFN_DSLHRVQIF_GET_UCHARA)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  SLAP_UINT32_ARRAY*
(*PFN_DSLHRVQIF_GET_UINT32A)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

typedef  ANSC_HANDLE
(*PFN_DSLHRVQIF_GET_HANDLE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pParamName
    );

/*
 * Setting a parameter value takes several steps. To ensure the flexibility of the framework, we
 * allow each individual ObjController to register TstValue() and SetValue() callbacks for the
 * parameters it wishes to have a closer control. In addition, we also support a simpler (from the
 * ObjController's point of view) yet powerful approach to reduce the complexity of each derived
 * ObjController: if the 'bGroupCommit' flag is not set for an ObjController, the framework will
 * call the TstValue() and SetValue() provided for each registered parameter; otherwise, these two
 * operations are handled internally and the new parameter values will be saved temporarily by each
 * VarRecord object. The DSLH_RVQ interface allows the ObjController to check which parameter has
 * been updated and retrieve the corresponding values in Validate() and Commit().
 */
#define  DSLH_RVQ_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_DSLHRVQIF_IS_UPDATED        IsParamUpdated;                                         \
    PFN_DSLHRVQIF_ClEAR_UPDATED     ClearParamUpdated;                                      \
    PFN_DSLHRVQIF_GET_VALUE         GetParamValue;                                          \
    PFN_DSLHRVQIF_GET_STRING        GetParamValueString;                                    \
    PFN_DSLHRVQIF_GET_INT           GetParamValueInt;                                       \
    PFN_DSLHRVQIF_GET_UINT32        GetParamValueUint32;                                    \
    PFN_DSLHRVQIF_GET_BOOL          GetParamValueBool;                                      \
    PFN_DSLHRVQIF_GET_UINT32A       GetParamValueUint32Array;                               \
    PFN_DSLHRVQIF_GET_UCHARA        GetParamValueUcharArray;                                \
    PFN_DSLHRVQIF_GET_HANDLE        GetParamValueHandle;                                    \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_RVQ_INTERFACE
{
    DSLH_RVQ_INTERFACE_CLASS_CONTENT
}
DSLH_RVQ_INTERFACE,  *PDSLH_RVQ_INTERFACE;

#define  ACCESS_DSLH_RVQ_INTERFACE(p)               \
         ACCESS_CONTAINER(p, DSLH_RVQ_INTERFACE, Linkage)


#endif
