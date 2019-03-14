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

    module:	ssd_ifo_dml.h

        For CCSP Secure Software Download

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the data model library.

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

#ifndef  _SSD_IFO_DML_
#define  _SSD_IFO_DML_

/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  SSD GENERAL BOOTLOADER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SSD_DML_INTERFACE_NAME                    "ssdDataModelLibraryIf"
#define  SSD_DML_INTERFACE_ID                      0x00000001

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  ULONG
(*PFN_SSDDML_GET_NUM_OF_ENTRIES)
    (
        ANSC_HANDLE                         hThisObject
    );

typedef  ANSC_HANDLE
(*PFN_SSDDML_GET_ENTRY_BY_INDEX)
    (
        ANSC_HANDLE                         hThisObject,
        ULONG                               ulIndex,
        PULONG                              pInsNumber
    );

typedef  ANSC_HANDLE
(*PFN_SSDDML_ADD_ENTRY)
    (
        ANSC_HANDLE                         hThisObject,
        PULONG                              pulInsNumber
    );

typedef  ANSC_STATUS
(*PFN_SSDDML_DEL_ENTRY)
    (
        ANSC_HANDLE                         hThisObject,
        ANSC_HANDLE                         hDownloadConfig
    );

typedef  ANSC_STATUS
(*PFN_SSDDML_NOTIFY)
    (
        ANSC_HANDLE                         hThisObject,
        ULONG                               ulEvent,
        ANSC_HANDLE                         hReserved
    );

typedef  ANSC_STATUS
(*PFN_SSDDML_SET_CURRENT_CONFIG)
    (
        ANSC_HANDLE                         hThisObject,
        ANSC_HANDLE                         hDownloadConfig
    );

typedef  ANSC_STATUS
(*PFN_SSDDML_REGSETINFO)
    (
        ANSC_HANDLE                         hThisObject
    );

/*
 * The Interface exposed for data model library.
 */
#define  SSD_DML_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SSDDML_GET_NUM_OF_ENTRIES           GetNumberOfEntries;                             \
    PFN_SSDDML_GET_ENTRY_BY_INDEX           GetEntryByIndex;                                \
    PFN_SSDDML_ADD_ENTRY                    AddEntry;                                       \
    PFN_SSDDML_DEL_ENTRY                    DelEntry;                                       \
    PFN_SSDDML_NOTIFY                       Notify;                                         \
    PFN_SSDDML_SET_CURRENT_CONFIG           SetCurrentConfig;                               \
    PFN_SSDDML_REGSETINFO                   RegSetInfo;                                     \
    /* end of object class content */                                                       \

typedef  struct
_SSD_DML_INTERFACE
{
    SSD_DML_INTERFACE_CLASS_CONTENT
}
SSD_DML_INTERFACE,  *PSSD_DML_INTERFACE;

#define  ACCESS_SSD_DML_INTERFACE(p)               \
         ACCESS_CONTAINER(p, SSD_DML_INTERFACE, Linkage)


typedef struct
_CCSP_FC_CONTEXT
{
    ANSC_HANDLE hCccDmlIf;
    ANSC_HANDLE hCcspCcdIf;
    ANSC_HANDLE hMessageBus;
}
CCSP_FC_CONTEXT, *PCCSP_FC_CONTEXT;

#endif
