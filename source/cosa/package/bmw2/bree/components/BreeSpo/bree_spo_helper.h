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

    module:	bree_spo_helper.h

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.

**********************************************************************/

#ifndef  _BREE_SPO_HELPER_DEC_
#define  _BREE_SPO_HELPER_DEC_

#include "bspeng_co_interface.h"


/*
 * This object just defines the interface that template users need to
 * derive from.
 */

typedef
BOOL
(*PFN_BREESPO_READER_VALID)
    (
        ANSC_HANDLE                 hThisObject
    );


/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  BREESPO_READER_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                  \
        BSP_TEMPLATE_READER_CLASS_CONTENT                                               \
        char                        *pName;                                             \
        ANSC_HANDLE                 hFile;                                              \
        BOOL                        bExternalSource;                                    \
        char                        *pContent;                                          \
        ULONG                       ulContentSize;                                      \
        ULONG                       ulCursor;                                           \
        /* APIs */                                                                      \
        PFN_BREESPO_READER_VALID    Valid;                                              \

/* content filtering basic message object  */
typedef  struct
_BREESPO_READER_OBJECT
{
    BREESPO_READER_CLASS_CONTENT
}
BREESPO_READER_OBJECT,    *PBREESPO_READER_OBJECT;


#endif

