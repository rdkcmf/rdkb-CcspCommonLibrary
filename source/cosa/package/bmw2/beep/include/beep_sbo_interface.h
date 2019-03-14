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

    module:	beep_sbo_interface.h

        For BSP Execution Environment Plane Implementation (BEEP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Beep Service Bean Object.

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


#ifndef  _BEEP_SBO_INTERFACE_
#define  _BEEP_SBO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "beep_properties.h"
#include "beep_ifo_pes.h"


/***********************************************************
        BEEP SERVICE BEAN COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_BEEPSBO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_BEEPSBO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  char*
(*PFN_BEEPSBO_GET_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_SET_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

typedef  ULONG
(*PFN_BEEPSBO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_SET_TYPE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       type
    );

typedef  BOOL
(*PFN_BEEPSBO_GET_BOOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_SET_BOOL)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  BOOL
(*PFN_BEEPSBO_MATCH1)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       engine_name
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_PROCESS)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpBmoReq,
        ANSC_HANDLE                 hHttpBmoRep,
        char*                       root_path,
        char*                       page_path,
        BOOL                        bOriginal
    );

typedef  ANSC_STATUS
(*PFN_BEEPSBO_PREPARE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHttpBmoReq,
        ANSC_HANDLE                 hHttpBmoRep
    );

/*
 * Beep Service Bean Objet must be instantiated for every registered file extension type. That is,
 * if an external Page Execution Engine registers a 'PageExtensions' field containing multiple file
 * extensions, a separate Beep Service Bean Object must be created for every one of them. Other
 * than that, this object is a classic place-holder object, which serves as a buffer zone between
 * the internal object-oriented environment and the external system.
 */
#define  BEEP_SERVICE_BEAN_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    BEEP_SERVICE_BEAN_PROPERTY      Property;                                               \
    ANSC_HANDLE                     hBeepPemIf;                                             \
    ANSC_HANDLE                     hBeepPesIf;                                             \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_BEEPSBO_GET_IF              GetBeepPemIf;                                           \
    PFN_BEEPSBO_SET_IF              SetBeepPemIf;                                           \
    PFN_BEEPSBO_GET_IF              GetBeepPesIf;                                           \
    PFN_BEEPSBO_SET_IF              SetBeepPesIf;                                           \
    PFN_BEEPSBO_GET_NAME            GetEngineName;                                          \
    PFN_BEEPSBO_SET_NAME            SetEngineName;                                          \
    PFN_BEEPSBO_GET_NAME            GetPageExtensions;                                      \
    PFN_BEEPSBO_SET_NAME            SetPageExtensions;                                      \
    PFN_BEEPSBO_GET_TYPE            GetEngineType;                                          \
    PFN_BEEPSBO_SET_TYPE            SetEngineType;                                          \
    PFN_BEEPSBO_GET_BOOL            GetAggrWrites;                                          \
    PFN_BEEPSBO_SET_BOOL            SetAggrWrites;                                          \
                                                                                            \
    PFN_BEEPSBO_GET_PROPERTY        GetProperty;                                            \
    PFN_BEEPSBO_SET_PROPERTY        SetProperty;                                            \
    PFN_BEEPSBO_RESET               ResetProperty;                                          \
    PFN_BEEPSBO_RESET               Reset;                                                  \
                                                                                            \
    PFN_BEEPSBO_ENGAGE              Engage;                                                 \
    PFN_BEEPSBO_CANCEL              Cancel;                                                 \
    PFN_BEEPSBO_MATCH1              MatchEngineName;                                        \
                                                                                            \
    PFN_BEEPSBO_PROCESS             Process;                                                \
    PFN_BEEPSBO_PREPARE             PrepareRep;                                             \
                                                                                            \
    PFN_BEEPPESIF_PREPARE           PesPreparePage;                                         \
    PFN_BEEPPESIF_EXECUTE           PesExecutePage;                                         \
    PFN_BEEPPESIF_QUALIFY           PesQualifyPage;                                         \
    PFN_BEEPPESIF_CONVERT           PesConvertPath;                                         \
    /* end of object class content */                                                       \

typedef  struct
_BEEP_SERVICE_BEAN_OBJECT
{
    BEEP_SERVICE_BEAN_CLASS_CONTENT
}
BEEP_SERVICE_BEAN_OBJECT,  *PBEEP_SERVICE_BEAN_OBJECT;

#define  ACCESS_BEEP_SERVICE_BEAN_OBJECT(p)         \
         ACCESS_CONTAINER(p, BEEP_SERVICE_BEAN_OBJECT, Linkage)


#endif
