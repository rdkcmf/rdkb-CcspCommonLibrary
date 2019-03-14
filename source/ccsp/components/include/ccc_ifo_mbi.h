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

    module:	ccc_ifo_mbi.h

        For CCSP Common Components

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for message bus interface(signal).

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

#ifndef  _CCC_IFO_MBI_
#define  _CCC_IFO_MBI_

/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/**************************************************************
  CCSP CC MESSAGE BUS INTERFACE OBJECT DEFINITION
**************************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  CCC_MBI_INTERFACE_NAME                    "CcspCcMessageBusIf"
#define  CCC_MBI_INTERFACE_ID                      0x00000003

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  ANSC_STATUS
(*PFN_CCCMBI_SENDPARAMETERVALUECHANGE_SIGNAL)
    (
        void*         dbusHandle,
        char*         pPamameterName,
        SLAP_VARIABLE * oldValue,
        SLAP_VARIABLE * newValue,
        char          * pAccessList
    );

typedef  ANSC_STATUS
(*PFN_CCCMBI_SENDTRANSFERCOMPLETE_SIGNAL)
    (
        void*         dbusHandle
    );

typedef  ANSC_STATUS
(*PFN_CCCMBI_SENDTRANSFERFAILED_SIGNAL)
    (
        void*         dbusHandle
    );


/*
 * The Interface exposed for Message Bus signaling.
 */


#define  CCC_MBI_INTERFACE_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_CCCMBI_SENDPARAMETERVALUECHANGE_SIGNAL       SendParameterValueChangeSignal;        \
    PFN_CCCMBI_SENDTRANSFERCOMPLETE_SIGNAL           SendTransferCompleteSignal;            \
    PFN_CCCMBI_SENDTRANSFERFAILED_SIGNAL             SendTransferFailedSignal;              \
    /* end of object class content */                                                       \

typedef  struct
_CCC_MBI_INTERFACE
{
    CCC_MBI_INTERFACE_CLASS_CONTENT
}
CCC_MBI_INTERFACE,  *PCCC_MBI_INTERFACE;

#define  ACCESS_CCC_MBI_INTERFACE(p)               \
         ACCESS_CONTAINER(p, CCC_MBI_INTERFACE, Linkage)


#endif
