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

    module:	bree_spo_internal_api.h

        For BSP Run-time Execution Engine (BREE) object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by the BREE object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.


    ---------------------------------------------------------------

    last modified:  
    

**********************************************************************/

#ifndef  _BREE_SPO_INTERNAL_API
#define  _BREE_SPO_INTERNAL_API


ANSC_STATUS
BreeSpoSetBeepEngineDesc
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
BreeSpoGetBeepEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BreeSpoCreateBeepEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BreeSpoPreparePage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        void*                       page_data,
        ULONG                       page_size
    );

ANSC_STATUS
BreeSpoExecutePage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBwrmCookedPage,
        ANSC_HANDLE                 hBeepPecIf
    );

ANSC_STATUS
BreeSpoSetStaticResEngineDesc
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_HANDLE
BreeSpoGetStaticResEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BreeSpoCreateStaticResEngineDesc
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
BreeSpoPrepareStaticRes
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        void*                       page_data,
        ULONG                       page_size
    );

ANSC_STATUS
BreeSpoExecuteStaticRes
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBwrmCookedPage,
        ANSC_HANDLE                 hBeepPecIf
    );

BOOL
BreeSpoQualifyPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path,
        ULONG                       page_type
    );

BOOL
BreeSpoConvertPath

    (
        ANSC_HANDLE                 hThisObject,
        char*                       org_page_path,
        char**                      new_page_path
    );

ANSC_STATUS
BreeSpoEngage
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BreeSpoCancel
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BreeSpoSetBeepSerIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
BreeSpoSetBeepCspIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    );

ANSC_STATUS
BreeSpoConfigResFilter
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ResFilterMode,
        PUCHAR                      pSupervisorPage
    );

ANSC_STATUS
BreeSpoRegResFilterDir
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    );

ANSC_STATUS
BreeSpoRegResFilterDirExemption
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    );

ANSC_STATUS
BreeSpoRegResFilterFile
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    );

ANSC_STATUS
BreeSpoRegResFilterFileExemption
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pName
    );

PUCHAR
BreeSpoGetFormalPagePath
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath
    );

BOOL
BreeSpoIsPagePathInFilterList
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath,
        BOOL                        bExemptionList
    );

PUCHAR
BreeSpoPrepareDFName
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pPagePath
    );

ANSC_STATUS
BreeSpoEnforceCache
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepPecIf,
        ULONG                       ulSeconds
    );

ANSC_HANDLE
BreeSpoCreateBreeSoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepPecIf
    );

ANSC_HANDLE
BreeSpoSoaGetSlapObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_HANDLE
BreeSpoSoaGetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        char*                       page_path
    );

ANSC_STATUS
BreeSpoSoaRetCookedPage
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage
    );

ANSC_STATUS
BreeSpoSoaGetCookedPageData
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedPage,
        PUCHAR                      *ppStream,
        PULONG                      pulStreamSize
    );

ANSC_STATUS
BreeSpoSoaInvokeObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBeepObject,
        char*                       method_name,
        SLAP_PARAMETER_LIST*        params_in,
        SLAP_PARAMETER_LIST**       params_out,
        SLAP_VARIABLE**             return_var,
        PULONG                      pulSoaStatus
    );

BOOL
BreeSpoSoaIsBuiltInObject
    (
        ANSC_HANDLE                 hThisObject,
        char*                       obj_name
    );

ANSC_STATUS
BreeSpoSoaWriteAString
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pAsciiStr
    );

ANSC_STATUS
BreeSpoSoaWriteBString
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pBinaryStr,
        ULONG                       ulStrLen
    );

BOOL
BreeSpoSoaIsInterrupted
    (
        ANSC_HANDLE                 hThisObject
    );


#endif

