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

    module:	ccsp_aem_mgr.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file defines the interfaces of
        CCSP Application Environment Manager (AEM) object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        11/03/2011    initial revision.

**********************************************************************/

#ifndef  _CCSP_AEM_MANAGER_H
#define  _CCSP_AEM_MANAGER_H

#include "ccsp_aem_plugin.h"

/**************************************************************************************************

CCSP Application Environment Manager Download Unit

***************************************************************************************************/
typedef struct
_CCSP_AEM_DOWNLOAD_ENTRY
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hPackage;
    ULONG                           uInsNumber;
    char*                           pFileName;
}
CCSP_AEM_DOWNLOAD_ENTRY, *PCCSP_AEM_DOWNLOAD_ENTRY;

#define  ACCESS_CCSP_AEM_DOWNLOAD_ENTRY(p)                 \
         ACCESS_CONTAINER(p, CCSP_AEM_DOWNLOAD_ENTRY, Linkage)

/**************************************************************************************************

CCSP Application Environment Manager Install UNIT

***************************************************************************************************/
typedef struct
_CCSP_AEM_INSTALL_UNIT
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hPackage;
    char*                           pFileName;
    ANSC_HANDLE                     hPluginObj;
    ANSC_HANDLE                     hEEHandle;
    ANSC_HANDLE                     hDUHandle;
}
CCSP_AEM_INSTALL_UNIT, *PCCSP_AEM_INSTALL_UNIT;

#define  ACCESS_CCSP_AEM_INSTALL_UNIT(p)                 \
         ACCESS_CONTAINER(p, CCSP_AEM_INSTALL_UNIT, Linkage)

/**************************************************************************************************

CCSP Application Environment Manager (AEM) Definitions

***************************************************************************************************/

typedef ULONG
(*PFN_CCSP_APPENV_MGR_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_HANDLE
(*PFN_CCSP_APPENV_MGR_GET_HANDLE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uIndex,
        PULONG                      puInsNumber
    );

typedef ANSC_STATUS
(*PFN_CCSP_APPENV_MGR_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_STATUS
(*PFN_CCSP_APPENV_MGR_COMMIT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPackage
    );

typedef ANSC_HANDLE
(*PFN_CCSP_APPENV_MGR_GETBY)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    );

typedef ANSC_STATUS
(*PFN_CCSP_APPENV_MGR_GETAR)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pInsArray,
        PULONG                      pulSize
    );

typedef ANSC_STATUS
(*PFN_CCSP_APPENV_MGR_APPLY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPackage,
        char*                       pURL
    );

typedef ANSC_STATUS
(*PFN_CCSP_APPENV_MGR_NOTIFY)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPackage
    );

typedef ANSC_STATUS
(*PFN_CCSP_APPENV_MGR_CONFIG)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pConfig
    );

typedef ANSC_HANDLE
(*PFN_CCSP_APPENV_MGR_GETE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uInsNumber
    );

typedef BOOL
(*PFN_CCSP_APPENV_MGR_PARSE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pFullEEName,
        PULONG                      pInsNumber
    );

typedef LONG
(*PFN_CCSP_APPENV_MGR_COMP)
    (
        ANSC_HANDLE           hThisObject,
        ANSC_HANDLE           hEeHandle,
        char *                OperID,
        char *                UUID,
        char *                DuRef,
        char *                Version,
        ULONG                 CurrentState,
        BOOL                  Resolved,
        char *                EuRefList,
        char *                StartTime,
        char *                CompleteTime,
        ULONG                 ErrorCode,
        char *                ErrorString
    );

#define  CCSP_APPENV_MGR_CLASS_CONTENT                     \
    /* start of object class content */                    \
    SINGLE_LINK_ENTRY               Linkage;               \
    QUEUE_HEADER                    PluginQueue;           \
    QUEUE_HEADER                    DownloadQueue;         \
    QUEUE_HEADER                    InstallQueue;          \
    ULONG                           uTimeStamps;           \
    BOOL                            bEUUpdated;            \
    BOOL                            bDUUpdated;            \
    ANSC_HANDLE                     hNotifyProc;           \
    ANSC_HANDLE                     hBusHandle;            \
    ANSC_HANDLE                     hDmlAgent;             \
    char*                           pSSDCompName;          \
    char*                           pSSDCompPath;          \
    char*                           pPrefix;               \
    ANSC_LOCK                       SynLock;               \
                                                           \
    PFN_CCSP_APPENV_MGR_ACTION      LoadPlugin;            \
                                                           \
    PFN_CCSP_APPENV_MGR_GET_ULONG   GetEEEntryCount;       \
    PFN_CCSP_APPENV_MGR_GET_HANDLE  GetEEEntry;            \
    PFN_CCSP_APPENV_MGR_GET_ULONG   GetDUEntryCount;       \
    PFN_CCSP_APPENV_MGR_GET_HANDLE  GetDUEntry;            \
    PFN_CCSP_APPENV_MGR_GET_ULONG   GetEUEntryCount;       \
    PFN_CCSP_APPENV_MGR_GET_HANDLE  GetEUEntry;            \
    PFN_CCSP_APPENV_MGR_GETBY       GetPluginObjByIndex;   \
    PFN_CCSP_APPENV_MGR_GETAR       GetDownloadInsArray;   \
    PFN_CCSP_APPENV_MGR_COMMIT      CommitPackage;         \
    PFN_CCSP_APPENV_MGR_APPLY       InstallPackage;        \
    PFN_CCSP_APPENV_MGR_NOTIFY      NotifyStateChanged;    \
    PFN_CCSP_APPENV_MGR_ACTION      RecvTransferComplete;  \
    PFN_CCSP_APPENV_MGR_ACTION      InitSSDConnection;     \
    PFN_CCSP_APPENV_MGR_CONFIG      SetSSDComponentName;   \
    PFN_CCSP_APPENV_MGR_CONFIG      SetSSDComponentPath;   \
    PFN_CCSP_APPENV_MGR_CONFIG      SetSubSystemPrefix;    \
    PFN_CCSP_APPENV_MGR_GETE        GetDownEntryByIns;     \
    PFN_CCSP_APPENV_MGR_PARSE       ParseEEFullName;       \
    PFN_CCSP_APPENV_MGR_COMP        ActionCompleted;       \
    PFN_CCSP_APPENV_MGR_ACTION      SynTables;             \
    PFN_CCSP_APPENV_MGR_GETE        GetDUByIns;            \
    PFN_CCSP_APPENV_MGR_GETE        GetEUByIns;            \
                                                           \
    PFN_CCSP_APPENV_MGR_ACTION      Remove;                \
    /* end of object class content */                      \

typedef  struct
_CCSP_APPENV_MGR_OBJECT
{
    CCSP_APPENV_MGR_CLASS_CONTENT
}
CCSP_APPENV_MGR_OBJECT,  *PCCSP_APPENV_MGR_OBJECT;

#define  ACCESS_CCSP_APPENV_MGR_OBJECT(p)                 \
         ACCESS_CONTAINER(p, CCSP_APPENV_MGR_OBJECT, Linkage)

/***********************************************************
 CCSP AEM External APIs
***********************************************************/
ANSC_HANDLE
CcspCreateAppEnvManager
    (
    );

void
CcspFreeAppEnvManager
    (
        ANSC_HANDLE                 hManager
    );

#endif
