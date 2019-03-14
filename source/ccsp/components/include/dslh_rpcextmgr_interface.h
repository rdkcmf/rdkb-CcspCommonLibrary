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

    module: dslh_rpcextmgr_interface.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for the Dslh RPC Extension
        Manager object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        12/14/2010    initial revision.

**********************************************************************/


#ifndef  _DSLH_RPC_EXT_MGR_INTERFACE_
#define  _DSLH_RPC_EXT_MGR_INTERFACE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"
#include "cosa_rpc_ext_interface.h"                 /* prototypes etc. for Download, Upload, AutonomousTransferComplete */
#include "cosa_pd194_smm_interface.h"               /* protptypes etc. for ChangeDUState */

#define  COSA_RPC_EXT_XML_NODE_Download             "download"
#define  COSA_RPC_EXT_XML_NODE_Upload               "upload"
#define  COSA_RPC_EXT_XML_NODE_AutonTransfter       "autonomousTransfer"
#define  COSA_RPC_EXT_XML_NODE_ChangeDuState        "changeDuState"
#define  COSA_RPC_EXT_XML_NODE_Reboot               "reboot"
#define  COSA_RPC_EXT_XML_NODE_FactoryReset         "factoryReset"

#define  COSA_RPC_EXT_XML_NODE_SlibPath             "slib_path"
#define  COSA_RPC_EXT_XML_NODE_FileType             "file_Type"
#define  COSA_RPC_EXT_XML_NODE_FuncInitDownload     "func_InitDownload"
#define  COSA_RPC_EXT_XML_NODE_FuncDownload         "func_Download"
#define  COSA_RPC_EXT_XML_NODE_FuncInitUpload       "func_InitUpload"
#define  COSA_RPC_EXT_XML_NODE_FuncUpload           "func_Upload"
#define  COSA_RPC_EXT_XML_NODE_FuncGetQueuedTrans   "func_GetQueuedTransfers"
#define  COSA_RPC_EXT_XML_NODE_FuncInitAutonXfer    "func_InitAutonTransfter"
#define  COSA_RPC_EXT_XML_NODE_Func_InitEEM         "func_InitEEM"
#define  COSA_RPC_EXT_XML_NODE_FuncChangeDuState    "func_ChangeDUState"
#define  COSA_RPC_EXT_XML_NODE_FuncUnload           "func_Unload"
#define  COSA_RPC_EXT_XML_NODE_FuncInitReboot       "func_InitReboot"
#define  COSA_RPC_EXT_XML_NODE_FuncReboot           "func_Reboot"
#define  COSA_RPC_EXT_XML_NODE_FuncInitFactoryReset "func_InitFactoryReset"
#define  COSA_RPC_EXT_XML_NODE_FuncFactoryReset     "func_FactoryReset"

/* RPCs that can be extended or customized */
#define  COSA_RPC_EXT_TYPE_Download                 1
#define  COSA_RPC_EXT_TYPE_Upload                   2
#define  COSA_RPC_EXT_TYPE_AutonTransferComplete    3
#define  COSA_RPC_EXT_TYPE_ChangeDUState            4
#define  COSA_RPC_EXT_TYPE_Reboot                   5
#define  COSA_RPC_EXT_TYPE_FactoryReset             6


/* RPC Download or Upload */
typedef  struct
_COSA_RPC_EXT_FileTransfer
{
    char*                           InitFuncName;
    char*                           FileType;
    char*                           OpFuncName;
    char*                           GetQTFuncName;

    /* Function pointer to Upload/Download after shared library loaded 
     * and it either has prototype PFN_COSA_Rpc_Download or
     * PFN_COSA_Rpc_Upload.
     */
    void*                           OpFuncPtr;
    /* function pointer to prototype PFN_COSA_GetQueuedTransfers */
    void*                           OpGetQueuedTransfers;
}
COSA_RPC_EXT_FileTransfer, *PCOSA_RPC_EXT_FileTransfer, 
COSA_RPC_EXT_Download, *PCOSA_RPC_EXT_Download, 
COSA_RPC_EXT_Upload, *PCOSA_RPC_EXT_Upload;

#define  CosaRpcExtFileTransferClean(ext_ft)                                        \
    do {                                                                            \
        if ( ext_ft )                                                               \
        {                                                                           \
            AnscFreeMemory((ext_ft)->InitFuncName);                                 \
            AnscFreeMemory((ext_ft)->FileType);                                     \
            AnscFreeMemory((ext_ft)->OpFuncName);                                   \
            AnscFreeMemory((ext_ft)->GetQTFuncName);                                \
        }                                                                           \
    } while (0)


/* RPC AutonomousTransferComplete */
typedef  struct
_COSA_RPC_EXT_AtonTransCompl
{
    char*                           InitFuncName;
}
COSA_RPC_EXT_AtonTransCompl, *PCOSA_RPC_EXT_AtonTransCompl;

#define  CosaRpcExtAutonTransComplClean(ext_atc)                                    \
    do {                                                                            \
        if ( ext_atc )                                                              \
        {                                                                           \
            AnscFreeMemory((ext_atc)->InitFuncName);                                \
        }                                                                           \
    } while (0)


/* RPC ChangeDUState */
typedef  struct
_COSA_RPC_EXT_ChangeDUState
{
    char*                           InitFuncName;

    char*                           OpFuncName;

    /* Function pointer to Upload/Download after shared library loaded 
     * and it has prototype PFN_COSA_SMM_ChangeDUState.
     */
    void*                           OpFuncPtr; 
}
COSA_RPC_EXT_ChangeDUState, *PCOSA_RPC_EXT_ChangeDUState;

#define  CosaRpcExtCdsClean(ext_cds)                                                \
    do {                                                                            \
        if ( ext_cds )                                                              \
        {                                                                           \
            AnscFreeMemory((ext_cds)->InitFuncName);                                \
            AnscFreeMemory((ext_cds)->OpFuncName);                                  \
        }                                                                           \
    } while (0)


/* RPC Reboot */
typedef  struct
_COSA_RPC_EXT_Reboot
{
    char*                           InitFuncName;

    char*                           OpFuncName;

    /* Function pointer to Reboot after shared library loaded 
     * and it has prototype PFN_COSA_Reboot.
     */
    void*                           OpFuncPtr; 
}
COSA_RPC_EXT_Reboot, *PCOSA_RPC_EXT_Reboot;

#define  CosaRpcExtRebootClean(ext_reboot)                                          \
    do {                                                                            \
        if ( ext_reboot )                                                           \
        {                                                                           \
            AnscFreeMemory((ext_reboot)->InitFuncName);                             \
            AnscFreeMemory((ext_reboot)->OpFuncName);                               \
        }                                                                           \
    } while (0)


/* RPC FactoryReset */
typedef  struct
_COSA_RPC_EXT_FactoryReset
{
    char*                           InitFuncName;

    char*                           OpFuncName;

    /* Function pointer to FactoryReset after shared library loaded 
     * and it has prototype PFN_COSA_FactoryReset.
     */
    void*                           OpFuncPtr; 
}
COSA_RPC_EXT_FactoryReset, *PCOSA_RPC_EXT_FactoryReset;

#define  CosaRpcExtFactoryResetClean(ext_factoryreset)                              \
    do {                                                                            \
        if ( ext_factoryreset )                                                     \
        {                                                                           \
            AnscFreeMemory((ext_factoryreset)->InitFuncName);                       \
            AnscFreeMemory((ext_factoryreset)->OpFuncName);                         \
        }                                                                           \
    } while (0)


/* 
 * RPC extension entity - everything about a RPC extension - a shared
 * library can implement multiple extensions. Multiple shared libraries
 * that implement the same (same file type download or upload e.g.)
 * may cause unexpected result, it's developers' responsibilities to
 * resolve conflicts if there any.
 */
typedef  struct
_COSA_RPC_EXT_ENTITY
{
    SINGLE_LINK_ENTRY               Linkage;            

    ULONG                           RpcExtType;
    char*                           UnloadFuncName;

    union
    {
        COSA_RPC_EXT_Download       Download;
        COSA_RPC_EXT_Upload         Upload;
        COSA_RPC_EXT_AtonTransCompl AutonTransCompl;
        COSA_RPC_EXT_ChangeDUState  ChangeDUState;
        COSA_RPC_EXT_Reboot         Reboot;
        COSA_RPC_EXT_FactoryReset   FactoryReset;
    }
    rpc;

    char*                           SLibPath;
    void*                           hSLibHandle;
}
COSA_RPC_EXT_ENTITY, *PCOSA_RPC_EXT_ENTITY;

#define  ACCESS_COSA_RPC_EXT_ENTITY(p)              \
         ACCESS_CONTAINER(p, COSA_RPC_EXT_ENTITY, Linkage)

#define  CosaRpcExtEntityCreate(ext_entity, type)                                               \
    do {                                                                                           \
        ext_entity = (PCOSA_RPC_EXT_ENTITY)AnscAllocateMemory                                   \
                    (                                                                           \
                        sizeof(COSA_RPC_EXT_ENTITY)                                             \
                    );                                                                          \
        if ( ext_entity )                                                                       \
        {                                                                                       \
            AnscZeroMemory(ext_entity, sizeof(COSA_RPC_EXT_ENTITY));                            \
            ext_entity->RpcExtType = type;                                                      \
        }                                                                                       \
    } while (0)

#define  CosaRpcExtEntityRemove(ext_entity, close_sl)                                           \
    do {                                                                                        \
        if ( ext_entity )                                                                       \
        {                                                                                       \
            switch ( (ext_entity)->RpcExtType )                                                 \
            {                                                                                   \
                case    COSA_RPC_EXT_TYPE_Download:                                             \
                case    COSA_RPC_EXT_TYPE_Upload:                                               \
                        CosaRpcExtFileTransferClean(&((ext_entity)->rpc.Upload));               \
                        break;                                                                  \
                case    COSA_RPC_EXT_TYPE_AutonTransferComplete:                                \
                        CosaRpcExtAutonTransComplClean(&((ext_entity)->rpc.AutonTransCompl));   \
                        break;                                                                  \
                case    COSA_RPC_EXT_TYPE_ChangeDUState:                                        \
                        CosaRpcExtCdsClean(&((ext_entity)->rpc.ChangeDUState));                 \
                        break;                                                                  \
                case    COSA_RPC_EXT_TYPE_Reboot:                                               \
                        CosaRpcExtRebootClean(&((ext_entity)->rpc.Reboot));                     \
                        break;                                                                  \
                case    COSA_RPC_EXT_TYPE_FactoryReset:                                         \
                        CosaRpcExtRebootClean(&((ext_entity)->rpc.FactoryReset));               \
                        break;                                                                  \
            }                                                                                   \
            AnscFreeMemory((ext_entity)->SLibPath);                                             \
            if ( (ext_entity)->UnloadFuncName )                                                 \
            {                                                                                   \
                AnscFreeMemory((ext_entity)->UnloadFuncName);                                   \
            }                                                                                   \
            AnscFreeMemory(ext_entity);                                                         \
        }                                                                                       \
    } while (0)


/***********************************************************
   DSLH RPC EXTENSION MANAGER COMPONENT OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the object mapper object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_DSLHRPCEXTM_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_DSLHRPCEXTM_LOAD)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pXMLContent,
        ULONG                       ulXMLLength
    );

typedef  PCOSA_RPC_EXT_ENTITY
(*PFN_DSLHRPCEXTM_FIND_FT)
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        Download,
        char*                       pFileType
    );

typedef  PCOSA_RPC_EXT_ENTITY
(*PFN_DSLHRPCEXTM_FIND_RPC)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulRpcExtType
    );

typedef  ANSC_HANDLE
(*PFN_DSLHRPCEXTM_GET_IF)
    (
        ANSC_HANDLE                 hThisObject
    );


#define  DSLH_RPC_EXT_MGR_CLASS_CONTENT                                             \
    /* duplication of the base object class content */                              \
    ANSCCO_CLASS_CONTENT                                                            \
    /* start of object class content */                                             \
    ANSC_HANDLE                     hDslhCpeController;                             \
    SLIST_HEADER                    RpcExtEntityList;                               \
    ANSC_HANDLE                     hSlapDslhRpcExtSvc;                             \
    ANSC_HANDLE                     hCosaPuaIf;                                     \
                                                                                    \
    BOOL                            bActive;                                        \
                                                                                    \
    PFN_DSLHRPCEXTM_ACTION          Engage;                                         \
    PFN_DSLHRPCEXTM_ACTION          Cancel;                                         \
                                                                                    \
    PFN_DSLHRPCEXTM_LOAD            LoadRpcExtXML;                                  \
    PFN_DSLHRPCEXTM_ACTION          LoadRpcExtLibs;                                 \
    PFN_DSLHRPCEXTM_ACTION          UnloadRpcExtLibs;                               \
                                                                                    \
    PFN_DSLHRPCEXTM_FIND_FT         FindFtRpcExt;                                   \
    PFN_DSLHRPCEXTM_FIND_RPC        FindRpcExt;                                     \
                                                                                    \
    /* callback functions for RPC extensions */                                     \
    PFN_COSA_Rpc_TransCompl         TransferComplete;                               \
    PFN_COSA_Rpc_AutonTransCompl    AutonomousTransferComplete;                     \
    PFN_COSA_DUStateChangeComplete_CB                                               \
                                    DuStateChangeComplete;                          \
    PFN_COSA_AutonDUStateChangeComplete_CB                                          \
                                    AutonomousDuStateChangeComplete;                \
    PFN_COSA_Register_DMSL_CB       RegsiterDataModelSLib;                          \
    PFN_COSA_Deregister_DMSL_CB     DeregisterDataModelSLib;                        \
                                                                                    \
    PFN_DSLHRPCEXTM_GET_IF          GetCosaPuaIf;                                   \
    /* end of object class content */                                               \

typedef  struct
_DSLH_RPC_EXT_MGR_OBJECT
{
    DSLH_RPC_EXT_MGR_CLASS_CONTENT
}
DSLH_RPC_EXT_MGR_OBJECT,  *PDSLH_RPC_EXT_MGR_OBJECT;

#define  ACCESS_DSLH_RPC_EXT_MGR_OBJECT(p)          \
         ACCESS_CONTAINER(p, DSLH_RPC_EXT_MGR_OBJECT, Linkage)


#endif /* _DSLH_RPC_EXT_MGR_INTERFACE_ */
