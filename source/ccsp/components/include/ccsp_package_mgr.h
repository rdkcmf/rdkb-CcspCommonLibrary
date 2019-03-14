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

    module:	ccsp_package_mgr.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file defines the interfaces of CCSP Package Manager.
        It will be used by Ccsp AppEnvMgr component.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        10/13/2011    initial revision.

**********************************************************************/

#ifndef  _CCSP_PACKAGE_MGR_H
#define  _CCSP_PACKAGE_MGR_H

/**************************************************************
 CCSP PACKAGE definition
***************************************************************/
#define MAXI_LENGTH_PACKAGE_ID                      36
#define MAXI_LENGTH_PACKAGE_VERSION                 36
#define MAXI_LENGTH_PACKAGE_UUID                    48
#define MAXI_LENGTH_PACKAGE                         256
#define MAXI_LENGTH_DATETIME                        36

#define CCSP_PACKAGE_Operation_None                 0
#define CCSP_PACKAGE_Operation_Install              1
#define CCSP_PACKAGE_Operation_Update               2
#define CCSP_PACKAGE_Operation_Remove               3

#define CCSP_PACKAGE_OperationState_None            0
#define CCSP_PACKAGE_OperationState_Requested       1
#define CCSP_PACKAGE_OperationState_Complete        2
#define CCSP_PACKAGE_OperationState_Error           3

#define CCSP_PACKAGE_CurrentState_None              0
#define CCSP_PACKAGE_CurrentState_Installed         1
#define CCSP_PACKAGE_CurrentState_Uninstalled       2
#define CCSP_PACKAGE_CurrentState_Failed            3


typedef  struct
_CCSP_PACKAGE
{
    SINGLE_LINK_ENTRY               Linkage;     
    ULONG                           uInstanceNumber;
    char                            ID[MAXI_LENGTH_PACKAGE_ID];
    BOOL                            Autonomous;
    ULONG                           Operation;
    ULONG                           OperationState;
    char                            URL[MAXI_LENGTH_PACKAGE];
    char                            UUID[MAXI_LENGTH_PACKAGE_UUID];
    char                            Username[MAXI_LENGTH_PACKAGE];
    char                            Password[MAXI_LENGTH_PACKAGE];
    char                            ExecutionEnvRef[MAXI_LENGTH_PACKAGE];
    char                            Version[MAXI_LENGTH_PACKAGE_VERSION];
    char                            DeploymentUnitRef[MAXI_LENGTH_PACKAGE];
    ULONG                           CurrentState;
    BOOL                            Resolved;
    char                            StartTime[MAXI_LENGTH_DATETIME];
    char                            CompleteTime[MAXI_LENGTH_DATETIME];
    int                             Error;
    char*                           ExecutionUnitRefList;
}
CCSP_PACKAGE,  *PCCSP_PACKAGE;

#define  ACCESS_CCSP_PACKAGE(p)    ACCESS_CONTAINER(p, CCSP_PACKAGE, Linkage)

#define  CcspAemInitPackage(eu)                                                 \
         {                                                                      \
            AnscZeroMemory(eu, sizeof(CCSP_PACKAGE));                           \
         } 

#define  CcspAemFreePackage(eu)                                                       \
        {                                                                             \
            if( eu->ExecutionUnitRefList)   AnscFreeMemory(eu->ExecutionUnitRefList); \
            AnscFreeMemory(eu);                                                        \
         } 

/***********************************************************
 CCSP Package Manager Interface APIs
***********************************************************/

/* register namespace */
typedef ULONG
(*PFN_CCSP_PACKAGE_MGR_GET_ULONG)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ANSC_HANDLE
(*PFN_CCSP_PACKAGE_MGR_GET_HANDLE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       uIndex,
        PULONG                      puInsNumber
    );

typedef ANSC_HANDLE
(*PFN_CCSP_PACKAGE_MGR_CREATE)
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      puInsNumber
    );

typedef ANSC_STATUS
(*PFN_CCSP_PACKAGE_MGR_DELETE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPackageHandle
    );

typedef ANSC_STATUS
(*PFN_CCSP_PACKAGE_MGR_COMMIT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPackageHandle
    );

typedef ANSC_STATUS
(*PFN_CCSP_PACKAGE_MGR_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );


#define  CCSP_PACKAGE_MGR_CLASS_CONTENT                    \
    /* start of object class content */                    \
    SINGLE_LINK_ENTRY               Linkage;               \
    ULONG                           uLastInsNumber;        \
    QUEUE_HEADER                    PackageQueue;          \
    ANSC_HANDLE                     hAppEnvMgr;            \
                                                           \
    PFN_CCSP_PACKAGE_MGR_GET_ULONG  GetEntryCount;         \
    PFN_CCSP_PACKAGE_MGR_GET_HANDLE GetEntry;              \
    PFN_CCSP_PACKAGE_MGR_CREATE     CreateEntry;           \
    PFN_CCSP_PACKAGE_MGR_DELETE     DeleteEntry;           \
    PFN_CCSP_PACKAGE_MGR_COMMIT     CommitEntry;           \
    PFN_CCSP_PACKAGE_MGR_ACTION     Remove;                \
    /* end of object class content */                      \

typedef  struct
_CCSP_PACKAGE_MGR_OBJECT
{
    CCSP_PACKAGE_MGR_CLASS_CONTENT
}
CCSP_PACKAGE_MGR_OBJECT,  *PCCSP_PACKAGE_MGR_OBJECT;

#define  ACCESS_CCSP_PACKAGE_MGR_OBJECT(p)                 \
         ACCESS_CONTAINER(p, CCSP_PACKAGE_MGR_OBJECT, Linkage)

/***********************************************************
 CCSP Package Manager External APIs
***********************************************************/
ANSC_HANDLE
CcspCreatePackageManager
    (
    );

void
CcspFreePackageManager
    (
        ANSC_HANDLE                 hPackageManager
    );

#endif
