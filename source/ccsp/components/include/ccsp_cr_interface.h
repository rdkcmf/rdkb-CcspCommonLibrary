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

    module:	ccsp_cr_interface.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file defines CCSP_CR object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        06/07/2011    initial revision.

**********************************************************************/

#ifndef  _CCSP_CR_MGR_H
#define  _CCSP_CR_MGR_H

#include "ccsp_base_api.h"

/**************************************************************
 CCSP Const defintions
***************************************************************/
#define CCSP_Component_NotRegistered                0
#define CCSP_Component_RegSuccessful                1
#define CCSP_Component_RegFailed                    2

/**************************************************************
 CCSP CR EVENT APIs
***************************************************************/
typedef int  
(*CCSP_CRIF_SYSREADY)
(
    void*                           hDbusHandle
);

typedef int  
(*CCSP_CRIF_PROFILE)
(
    void*                           hDbusHandle,
    char*                           pCompName, 
    char*                           pDbusPath, 
    BOOL                            isAvailable
);

typedef int  
(*CCSP_CRIF_SESSION)
(
    void*                           hDbusHandle,
     int                            uPriority, 
     int                            uSessionID
);

typedef  struct
_CCSP_CR_EVENT_NOTIFY_PROC
{
    CCSP_CRIF_SYSREADY              SignalSystemReadyProc;
    CCSP_CRIF_PROFILE               SignalProfileChangeProc;
    CCSP_CRIF_SESSION               SignalSessionChangeProc;
}
CCSP_CR_EVENT_NOTIFY_PROC,  *PCCSP_CR_EVENT_NOTIFY_PROC;

/**************************************************************
 CCSP ComponentInfo 
***************************************************************/
typedef  struct
_CCSP_COMPONENT_INFO
{
    SINGLE_LINK_ENTRY               Linkage;     
    char*                           pComponentName;
    ULONG                           uVersion;
    ULONG                           uStatus;
}
CCSP_COMPONENT_INFO,  *PCCSP_COMPONENT_INFO;

#define  ACCESS_CCSP_COMPONENT_INFO(p)              ACCESS_CONTAINER(p, CCSP_COMPONENT_INFO, Linkage)

/**************************************************************
 CCSP RemoteCRInfo
***************************************************************/
typedef  struct
_CCSP_REMOTE_CRINFO
{
    SINGLE_LINK_ENTRY               Linkage;     
    char*                           pPrefix;
    char*                           pID;
}
CCSP_REMOTE_CRINFO,  *PCCSP_REMOTE_CRINFO;

#define  ACCESS_CCSP_REMOTE_CRINFO(p)              ACCESS_CONTAINER(p, CCSP_REMOTE_CRINFO, Linkage)

/**************************************************************
 CCSP RegisterNamespace Info
***************************************************************/
#define CCSP_REG_NAMESPACE         namespaceType_t

/***********************************************************
   CCSP CR Manager Interface APIs
***********************************************************/
typedef BOOL
(*PFN_CCSP_CR_MANAGER_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef BOOL
(*PFN_CCSP_CR_MANAGER_GETBOL)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef int
(*PFN_CCSP_CR_MANAGER_REGST)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        ULONG                       compVersion,
        const char*                 pDbusPath,
        const char*                 pPrefix,
        PVOID*                      pRegNamespace,
        ULONG                       ulSize
    );

typedef int
(*PFN_CCSP_CR_MANAGER_UNREG2)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName
    );

typedef int
(*PFN_CCSP_CR_MANAGER_UNREG)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        const char*                 pNamespace
    );

typedef int
(*PFN_CCSP_CR_MANAGER_CHECK)
    (
        ANSC_HANDLE                 hThisObject,        
        const char*                 pNamespace,
        ULONG                       uType,
        BOOL*                       pbMatch
    );

typedef int
(*PFN_CCSP_CR_MANAGER_DISCV)
    (
        ANSC_HANDLE                 hThisObject,        
        const char*                 pNamespace,
        const char*                 pPrefix,
        BOOL                        bNextLevel,
        PVOID**                     ppComponent,
        ULONG*                      pulSize
    );

typedef int
(*PFN_CCSP_CR_MANAGER_DISCV2)
    (
        ANSC_HANDLE                 hThisObject,        
        const char*                 pNamespace,
        const char*                 pPrefix,
        BOOL                        bNextLevel,
        PVOID*                      pComponent
    );

typedef int
(*PFN_CCSP_CR_MANAGER_REQID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uPriority,
        ULONG*                      pulSessionID
    );

typedef int
(*PFN_CCSP_CR_MANAGER_GETID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG*                      pulPriority,
        ULONG*                      pulSessionID
    );

typedef int
(*PFN_CCSP_CR_MANAGER_ENDID)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSessionID
    );

typedef int
(*PFN_CCSP_CR_MANAGER_NMELOS)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pComponentName
    );

typedef int
(*PFN_CCSP_CR_MANAGER_GETAR)
    (
        ANSC_HANDLE                 hThisObject,        
        PVOID**                     ppComponent,
        ULONG*                      pulSize
    );

typedef int
(*PFN_CCSP_CR_MANAGER_GETAR2)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,                
        PVOID**                     ppNamespace,
        ULONG*                      pulSize
    );

typedef void
(*PFN_CCSP_CR_MANAGER_SETSTR)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPrefix
    );


#define  CCSP_CR_MANAGER_CLASS_CONTENT                                                      \
    /* start of object class content */                                                     \
    char*                           pDeviceName;                                            \
    char*                           pCRName;                                                \
    char*                           pPrefix;                                                \
    ULONG                           uVersion;                                               \
    BOOL                            bSystemReady;                                           \
    ANSC_HANDLE                     hCcspNamespaceMgr;                                      \
    QUEUE_HEADER                    RemoteCRQueue;                                          \
    QUEUE_HEADER                    CompInfoQueue;                                          \
    QUEUE_HEADER                    UnknowCompInfoQueue;                                    \
    ULONG                           uPriority;                                              \
    ULONG                           uSessionID;                                             \
    BOOL                            bInSession;                                             \
    ANSC_HANDLE                     hDbusHandle;                                            \
    CCSP_CR_EVENT_NOTIFY_PROC       SignalProc;                                             \
                                                                                            \
    PFN_CCSP_CR_MANAGER_ACTION      LoadDeviceProfile;                                      \
    PFN_CCSP_CR_MANAGER_ACTION      CleanAll;                                               \
    PFN_CCSP_CR_MANAGER_ACTION      DumpObject;                                             \
    PFN_CCSP_CR_MANAGER_GETBOL      IsSystemReady;                                          \
    PFN_CCSP_CR_MANAGER_REGST       RegisterCapabilities;                                   \
    PFN_CCSP_CR_MANAGER_DISCV       DiscoverComponentSupportingNamespace;                   \
    PFN_CCSP_CR_MANAGER_DISCV2      DiscoverComponentSupportingDynamicTbl;                  \
    PFN_CCSP_CR_MANAGER_UNREG       UnregisterNamespace;                                    \
    PFN_CCSP_CR_MANAGER_UNREG2      UnregisterComponent;                                    \
    PFN_CCSP_CR_MANAGER_CHECK       CheckNamespaceDataType;                                 \
    PFN_CCSP_CR_MANAGER_REQID       RequestSessionID;                                       \
    PFN_CCSP_CR_MANAGER_GETID       GetCurrentSessionID;                                    \
    PFN_CCSP_CR_MANAGER_ENDID       InformEndOfSession;                                     \
    PFN_CCSP_CR_MANAGER_NMELOS      AfterComponentLost;                                     \
    PFN_CCSP_CR_MANAGER_GETAR       GetRegisteredComponents;                                \
    PFN_CCSP_CR_MANAGER_GETAR2      GetNamespaceByComponent;                                \
    PFN_CCSP_CR_MANAGER_SETSTR      SetPrefix;                                              \
    /* end of object class content */                                                       \

typedef  struct
_CCSP_CR_MANAGER_OBJECT
{
    CCSP_CR_MANAGER_CLASS_CONTENT
}
CCSP_CR_MANAGER_OBJECT,  *PCCSP_CR_MANAGER_OBJECT;
  
#endif
