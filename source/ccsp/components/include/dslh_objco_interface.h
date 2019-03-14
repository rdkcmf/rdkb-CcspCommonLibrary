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

    module: dslh_objco_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh Obj Controller Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        09/23/2005    initial revision.
        10/07/2010    Bin added DslhTr69If to support future data model library integration.
        09/22/2011    Bin added GetBulkParamValue apis;

**********************************************************************/


#ifndef  _DSLH_OBJCO_INTERFACE_
#define  _DSLH_OBJCO_INTERFACE_

#include "ansc_lco_interface.h"
#include "ansc_lco_external_api.h"

/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */

#define  DSLH_OBJCONTROLLER_PARAM_WRITE_NOTCHANGE               0
#define  DSLH_OBJCONTROLLER_PARAM_WRITE_NO                      1
#define  DSLH_OBJCONTROLLER_PARAM_WRITE_YES                     2

/***********************************************************
       DSLH OBJ CONTROLLER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_DSLHOBJCO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_DSLHOBJCO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_DSLHOBJCO_CHECK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_DSLHOBJCO_CHECK1)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pParamName
    );

typedef  BOOL
(*PFN_DSLHOBJCO_VALID)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pParamName,
        ULONG                       uType
    );

typedef  ULONG
(*PFN_DSLHOBJCO_ACCESS)
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pParamName
    );

typedef  BOOL
(*PFN_DSLHOBJCO_VALIDATE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDslhRvqIf,
        char**                      ppFaultParamName
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCO_COMMIT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDslhRvqIf
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCO_ROLLBACK)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_STATUS
(*PFN_DSLHOBJCO_GETVALUE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pName,
        PSLAP_VARIABLE              pVariable
    );

typedef  ANSC_STATUS
(*PFN_DSLHOBJCO_GETBULK)
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize
    );

/*
 * In some ways, the DslhObjController object reassembles the idea of a SLAP object. Both of them
 * are created in runtime and serve as a bridging layer between the Management Applications and the
 * service modules underneath. However, the DSLH Data Management Model has much less overhead when
 * comparing to the sophisticated SLAP Component Programming Model.
 */
#define  DSLH_OBJ_CONTROLLER_CLASS_CONTENT                                                  \
    /* duplication of the base object class content */                                      \
    ANSCLCO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hDslhObjRecord;                                         \
    ANSC_HANDLE                     hDslhCpeController;                                     \
    ANSC_HANDLE                     hInsContext;                                            \
    BOOL                            bCommitted;                                             \
    ANSC_HANDLE                     hParentInsContext;                                      \
    ANSC_HANDLE                     hParamArray;                                            \
    BOOL                            bCommitting;                                            \
                                                                                            \
    PFN_DSLHOBJCO_GET_CONTEXT       GetInsContext;                                          \
    PFN_DSLHOBJCO_CHECK             CheckInstance;                                          \
    PFN_DSLHOBJCO_CHECK1            CheckParamExistence;                                    \
    PFN_DSLHOBJCO_ACCESS            CheckParamWritability;                                  \
    PFN_DSLHOBJCO_VALID             ValidateHierarchyInterface;                             \
    PFN_DSLHOBJCO_GETVALUE          GetParamValueByName;                                    \
    PFN_DSLHOBJCO_GETBULK           GetBulkParamValue;                                      \
    PFN_DSLHOBJCO_RESET             Reset;                                                  \
                                                                                            \
    PFN_DSLHOBJCO_ENGAGE            Engage;                                                 \
    PFN_DSLHOBJCO_VALIDATE          Validate;                                               \
    PFN_DSLHOBJCO_COMMIT            Commit;                                                 \
    PFN_DSLHOBJCO_ROLLBACK          Rollback;                                               \
                                                                                            \
    ANSC_HANDLE                     hDslhTr69If;                                            \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_OBJ_CONTROLLER_OBJECT
{
    DSLH_OBJ_CONTROLLER_CLASS_CONTENT
}
DSLH_OBJ_CONTROLLER_OBJECT,  *PDSLH_OBJ_CONTROLLER_OBJECT;

#define  ACCESS_DSLH_OBJ_CONTROLLER_OBJECT(p)       \
         ACCESS_CONTAINER(p, DSLH_OBJ_CONTROLLER_OBJECT, Linkage)
#define  ACCESS_DSLH_OBJ_CONTROLLER_FIELD(f)        \
         ACCESS_FIELD_OFFSET(DSLH_OBJ_CONTROLLER_OBJECT, f)


#endif
