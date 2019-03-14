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

    module:	ccsp_ifo_ccd.h

        For CCSP Common Components

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the component common data model.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Tom Chang

    ---------------------------------------------------------------

    revision:

        06/14/11    initial revision.

**********************************************************************/

#ifndef  _CCSP_IFO_CCD_
#define  _CCSP_IFO_CCD_

/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/**************************************************************
  CCSP COMPONENT COMMON DATA MODEL INTERFACE OBJECT DEFINITION
**************************************************************/


/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  CCSP_CCD_INTERFACE_NAME                    "CcspComponentCommonDatamodelIf"
#define  CCSP_CCD_INTERFACE_ID                      0x00000002

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  BOOL
(*PFN_CCSPCCDM_GET_VALUE_BOOL)
    (
        ANSC_HANDLE                     hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CCSPCCDM_SET_VALUE_BOOL)
    (
        ANSC_HANDLE                     hThisObject,
        BOOL                            bValue
    );

typedef  ULONG
(*PFN_CCSPCCDM_GET_VALUE_ULONG)
    (
        ANSC_HANDLE                     hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CCSPCCDM_SET_VALUE_ULONG)
    (
        ANSC_HANDLE                     hThisObject,
        ULONG                           ulValue
    );

typedef  char*
(*PFN_CCSPCCDM_GET_VALUE_STRING)
    (
        ANSC_HANDLE                     hThisObject
    );

typedef  ANSC_STATUS
(*PFN_CCSPCCDM_APPLY_CHANGES)
    (
        ANSC_HANDLE                     hThisObject
    );

/*
 * The Interface exposed for component common data model.
 */
#define  CCSP_CCD_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_CCSPCCDM_GET_VALUE_STRING            GetComponentName;                               \
    PFN_CCSPCCDM_GET_VALUE_ULONG             GetComponentVersion;                            \
    PFN_CCSPCCDM_GET_VALUE_STRING            GetComponentAuthor;                             \
    PFN_CCSPCCDM_GET_VALUE_ULONG             GetComponentHealth;                             \
    PFN_CCSPCCDM_GET_VALUE_ULONG             GetComponentState;                              \
    PFN_CCSPCCDM_GET_VALUE_BOOL              GetLoggingEnabled;                              \
    PFN_CCSPCCDM_SET_VALUE_BOOL              SetLoggingEnabled;                              \
    PFN_CCSPCCDM_GET_VALUE_ULONG             GetLoggingLevel;                                \
    PFN_CCSPCCDM_SET_VALUE_ULONG             SetLoggingLevel;                                \
    PFN_CCSPCCDM_GET_VALUE_ULONG             GetMemMaxUsage;                                 \
    PFN_CCSPCCDM_GET_VALUE_ULONG             GetMemMinUsage;                                 \
    PFN_CCSPCCDM_GET_VALUE_ULONG             GetMemConsumed;                                 \
    PFN_CCSPCCDM_APPLY_CHANGES               ApplyChanges;                                   \
    /* end of object class content */                                                       \

typedef  struct
_CCSP_CCD_INTERFACE
{
    CCSP_CCD_INTERFACE_CLASS_CONTENT
}
CCSP_CCD_INTERFACE,  *PCCSP_CCD_INTERFACE;

#define  ACCESS_CCSP_CCD_INTERFACE(p)               \
         ACCESS_CONTAINER(p, CCSP_CCD_INTERFACE, Linkage)


#endif
