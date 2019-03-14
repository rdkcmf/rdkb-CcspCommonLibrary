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

    module:	sys_cpo_interface.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Sys CTLV Processor Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/13/06    initial revision.

**********************************************************************/


#ifndef  _SYS_CPO_INTERFACE_
#define  _SYS_CPO_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "sys_cfg_storage.h"
#include "sys_ifo_sfc.h"

/***********************************************************
       SYS INFO REPOSITORY COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 *  All configurable parameters which determines the object's behavior are
 *  put into the property structure.
 */
typedef  struct
_SYS_CTLV_PROCESSOR_PROPERTY
{
    ULONG                           Dummy;
}
SYS_CTLV_PROCESSOR_PROPERTY, *PSYS_CTLV_PROCESSOR_PROPERTY;


/*
 * Define some const values that will be used in the os wrapper object definition.
 */
/*#define  SYS_CTLV_PROCESSOR_OID                     SYS_GENERAL_FO_OID_BASE     + 0x0014*/
#define  SYS_CTLV_PROCESSOR_NAME                    "sysCtlvProcessor"
#define  SYS_CTLV_PROCESSOR_TYPE                    ANSC_OBJECT_TYPE_NULL

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_SYSCPO_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ANSC_HANDLE
(*PFN_SYSCPO_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_SET_IF)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_GET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_SET_PROPERTY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_RESET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_ENGAGE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_CANCEL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_PROVISION_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        PUCHAR                      pCtlvBuffer,
        ULONG                       ulCtlvSize
    );

typedef  ULONG
(*PFN_SYSCPO_QUERY_FOLDER_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        char*                       pFolderName
    );

typedef  ANSC_STATUS
(*PFN_SYSCPO_SERIALIZE_FOLDER)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentFolder,
        char*                       pFolderName,
        PUCHAR                      pCtlvBuffer,
        ULONG                       ulBufferSize
    );

/*
 *  The Sys Cltv Processor Object is responsible for converting System Repository from/to
 *  CTLV based configuration file format.
 */
#define  SYS_CTLV_PROCESSOR_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCCO_CLASS_CONTENT                                                                    \
    /* start of object class content */                                                     \
    SYS_CTLV_PROCESSOR_PROPERTY     Property;                                               \
    ANSC_HANDLE                     hSysIraIf;                                              \
    ANSC_HANDLE                     hSysSfcIf;                                              \
    BOOL                            bActive;                                                \
                                                                                            \
    PFN_SYSCPO_SET_IF               SetSysIraIf;                                            \
    PFN_SYSCPO_GET_IF               GetSysSfcIf;                                            \
                                                                                            \
    PFN_SYSCPO_GET_PROPERTY         GetProperty;                                            \
    PFN_SYSCPO_SET_PROPERTY         SetProperty;                                            \
    PFN_SYSCPO_RESET                ResetProperty;                                          \
    PFN_SYSCPO_RESET                Reset;                                                  \
                                                                                            \
    PFN_SYSCPO_ENGAGE               Engage;                                                 \
    PFN_SYSCPO_CANCEL               Cancel;                                                 \
                                                                                            \
    PFN_SYSSFCIF_VERIFY             SfcVerify;                                              \
    PFN_SYSSFCIF_PROVISION          SfcProvision;                                           \
    PFN_SYSSFCIF_SERIALIZE          SfcSerialize;                                           \
                                                                                            \
    PFN_SYSCPO_PROVISION_FOLDER     ProvisionFolder;                                        \
    PFN_SYSCPO_QUERY_FOLDER_SIZE    QueryFolderSize;                                        \
    PFN_SYSCPO_SERIALIZE_FOLDER     SerializeFolder;                                        \
                                                                                            \
    /* end of object class content */                                                       \

typedef  struct
_SYS_CTLV_PROCESSOR_OBJECT
{
    SYS_CTLV_PROCESSOR_CLASS_CONTENT
}
SYS_CTLV_PROCESSOR_OBJECT,  *PSYS_CTLV_PROCESSOR_OBJECT;

#define  ACCESS_SYS_CTLV_PROCESSOR_OBJECT(p)        \
         ACCESS_CONTAINER(p, SYS_CTLV_PROCESSOR_OBJECT, Linkage)


#endif

