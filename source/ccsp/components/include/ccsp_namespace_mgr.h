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

    module:	ccsp_namespace_mgr.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file defines the interfaces of CCSP Namespace Manager.
        It will be used by both CR and PA.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        06/08/2011    initial revision.

**********************************************************************/

#ifndef  _CCSP_NAMESPACE_MGR_H
#define  _CCSP_NAMESPACE_MGR_H

#include "ccsp_base_api.h"

/**************************************************************
 CCSP Const Definition
***************************************************************/
#define  CCSP_MAXI_COMPONENT_NAME_LENGTH                    256
#define  CCSP_MAXI_DBUS_PATH_LENGTH                         256
#define  CCSP_MAXI_SUBSYSTEM_PREFIX                         64

#define  TR69_PARAM_NAME_SEPARATOR_CHAR                     '.'
#define  TR69_PARAM_NAME_SEPARATOR                          "."
#define  TR69_PARAM_TABLE_SYMBOL                            "{i}"
#define  TR69_NAME_TABLE_END                                ".{i}."

/**************************************************************
 CCSP Namespace definition
***************************************************************/
typedef  struct
_CCSP_NAMESPACE
{
    SINGLE_LINK_ENTRY               Linkage;     
    char*                           pName;
    int                             uType;
    BOOL                            bWritable;
}
CCSP_NAMESPACE,  *PCCSP_NAMESPACE;

#define  ACCESS_CCSP_NAMESPACE(p)    ACCESS_CONTAINER(p, CCSP_NAMESPACE, Linkage)

/***********************************************************
 CCSP Namespace Component Interface APIs
***********************************************************/

/* register namespace */
typedef int
(*PFN_CCSP_NAMESPACE_COMP_REGIS)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace,
        ULONG                       uDataType,
        BOOL                        bWritable
    );

/* unregister namespace */
typedef int
(*PFN_CCSP_NAMESPACE_COMP_UNREG)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace
    );

typedef ULONG
(*PFN_CCSP_NAMESPACE_COMP_GETCN)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef int
(*PFN_CCSP_NAMESPACE_COMP_GETAR)
    (
        ANSC_HANDLE                 hThisObject,
        char***                     pppNamespace,  /* output of namespace str array */
        ULONG*                      pulSize        /* output of array size */
    );

typedef int
(*PFN_CCSP_NAMESPACE_COMP_GETA2)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID**                     ppNamespace,  /* output of namespace str array */
        ULONG*                      pulSize        /* output of array size */
    );

typedef int
(*PFN_CCSP_NAMESPACE_COMP_BUILD)
    (
        ANSC_HANDLE                 hThisObject,
        char**                      ppNamespace,   /* input of namespace str array */
        ULONG                       ulSize         /* input of array size */
    );

typedef int
(*PFN_CCSP_NAMESPACE_COMP_BUIL2)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      pNamespace,
        ULONG                       ulSize         /* input of array size */
    );

/* check whether an object namespace is supported or not */
typedef BOOL
(*PFN_CCSP_NAMESPACE_COMP_LOOKF)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pObjNamespace,
        BOOL                        bNextLevel
    );

typedef BOOL
(*PFN_CCSP_NAMESPACE_COMP_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

#define  CCSP_NAMESPACE_COMP_CLASS_CONTENT                                                  \
    /* start of object class content */                                                     \
    SINGLE_LINK_ENTRY               Linkage;                                                \
    char*                           pContainerName;                                         \
    char                            pCompName[CCSP_MAXI_COMPONENT_NAME_LENGTH];             \
    char                            pDbusPath[CCSP_MAXI_DBUS_PATH_LENGTH];                  \
    char                            pPrefix[CCSP_MAXI_SUBSYSTEM_PREFIX];                    \
    QUEUE_HEADER                    ParamspaceQueue;                                        \
                                                                                            \
    PFN_CCSP_NAMESPACE_COMP_UNREG   UnregisterNamespace;                                    \
    PFN_CCSP_NAMESPACE_COMP_GETCN   GetNamespaceCount;                                      \
    PFN_CCSP_NAMESPACE_COMP_GETAR   GetNamespaceStringArray;                                \
    PFN_CCSP_NAMESPACE_COMP_GETA2   GetNamespaceStructArray;                                \
    PFN_CCSP_NAMESPACE_COMP_BUILD   BuildFromNamespaceStringArray;                          \
    PFN_CCSP_NAMESPACE_COMP_BUIL2   BuildFromNamespaceStructArray;                          \
    PFN_CCSP_NAMESPACE_COMP_LOOKF   IsObjectSupported;                                      \
    PFN_CCSP_NAMESPACE_COMP_ACTION  DumpObject;                                             \
    PFN_CCSP_NAMESPACE_COMP_ACTION  CleanAll;                                               \
    /* end of object class content */                                                       \

typedef  struct
_CCSP_NAMESPACE_COMP_OBJECT
{
    CCSP_NAMESPACE_COMP_CLASS_CONTENT
}
CCSP_NAMESPACE_COMP_OBJECT,  *PCCSP_NAMESPACE_COMP_OBJECT;

#define  ACCESS_CCSP_NAMESPACE_COMP_OBJECT(p)      \
         ACCESS_CONTAINER(p, CCSP_NAMESPACE_COMP_OBJECT, Linkage)

/***********************************************************
 CCSP Namespace Component External APIs
***********************************************************/
ANSC_HANDLE
CcspCreateNamespaceComponent
    (
        char*                       pContainerName
    );

void
CcspFreeNamespaceComponent
    (
        char*                       pContainerName,
        ANSC_HANDLE                 hNamespaceComp
    );

/***********************************************************
   CCSP Namespace Manager Interface APIs
***********************************************************/
typedef int
(*PFN_CCSP_NAMESPACE_MNGR_REGST)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        const char*                 pDbusPath,
        const char*                 pPrefix,
        PVOID*                      pNamesapceArray,
        ULONG                       ulSize         
    );

typedef int
(*PFN_CCSP_NAMESPACE_MNGR_BUILD)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        const char*                 pDbusPath,
        const char*                 pPrefix,
        PVOID*                      ppNamesapceArray,
        ULONG                       ulSize         
    );

typedef int
(*PFN_CCSP_NAMESPACE_MNGR_UNREG)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        const char*                 pNamespace
    );

typedef int
(*PFN_CCSP_NAMESPACE_MNGR_UNREG2)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName
    );

typedef int
(*PFN_CCSP_NAMESPACE_MNGR_DISCV)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace,
        const char*                 pPrefix,
        BOOL                        bNextLevel,
        PVOID**                     ppComponent,   /* output of component struct array */
        ULONG*                      pulSize        /* output of array size */
    );

/* look for component by name */
typedef ANSC_HANDLE
(*PFN_CCSP_NAMESPACE_MNGR_LOOKF)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName
    );

/* Check whether the data type of given name space matched or not */
typedef int
(*PFN_CCSP_NAMESPACE_MNGR_CHECK)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace,
        ULONG                       uDataType,
        BOOL*                       pbMatch
    );

typedef int
(*PFN_CCSP_NAMESPACE_MNGR_GETAR)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID**                     ppCompName,
        ULONG*                      pulSize
    );

typedef int
(*PFN_CCSP_NAMESPACE_MNGR_GETAR2)
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,                
        PVOID**                     ppNamespace,
        ULONG*                      pulSize
    );


typedef BOOL
(*PFN_CCSP_NAMESPACE_MNGR_ACTION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef ULONG
(*PFN_CCSP_NAMESPACE_MNGR_GETINT)
    (
        ANSC_HANDLE                 hThisObject
    );

#define  CCSP_NAMESPACE_MGR_CLASS_CONTENT                                                   \
    /* start of object class content */                                                     \
    char                            pContainerName[CCSP_MAXI_COMPONENT_NAME_LENGTH];        \
    QUEUE_HEADER                    ComponentQueue;                                         \
    ANSC_HANDLE                     hAtomNamespace;                                         \
    ULONG                           uTotalNamespace;                                        \
    char*                           SubsysPrefix;                                           \
                                                                                            \
    PFN_CCSP_NAMESPACE_MNGR_GETINT  GetCountOfRegNamespace;                                 \
    PFN_CCSP_NAMESPACE_MNGR_LOOKF   LookforComponent;                                       \
    PFN_CCSP_NAMESPACE_MNGR_REGST   RegisterNamespaces;                                     \
    PFN_CCSP_NAMESPACE_MNGR_BUILD   BuildNamespaces;                                        \
    PFN_CCSP_NAMESPACE_MNGR_UNREG   UnregisterNamespace;                                    \
    PFN_CCSP_NAMESPACE_MNGR_UNREG2  UnregisterComponent;                                    \
    PFN_CCSP_NAMESPACE_MNGR_DISCV   DiscoverNamespace;                                      \
    PFN_CCSP_NAMESPACE_MNGR_CHECK   CheckNamespaceDataType;                                 \
    PFN_CCSP_NAMESPACE_MNGR_GETAR   GetRegisteredComponents;                                \
    PFN_CCSP_NAMESPACE_MNGR_GETAR2  GetNamespaceByComponent;                                \
    PFN_CCSP_NAMESPACE_MNGR_ACTION  CleanAll;                                               \
    PFN_CCSP_NAMESPACE_MNGR_ACTION  DumpObject;                                             \
    /* end of object class content */                                                       \

typedef  struct
_CCSP_NAMESPACE_MGR_OBJECT
{
    CCSP_NAMESPACE_MGR_CLASS_CONTENT
}
CCSP_NAMESPACE_MGR_OBJECT,  *PCCSP_NAMESPACE_MGR_OBJECT;


/***********************************************************
   CCSP Namespace Manager External APIs
***********************************************************/
ANSC_HANDLE
CcspCreateNamespaceMgr
    (
        char*                       pContainerName
    );

void
CcspFreeNamespaceMgr
    (
        char*                       pContainerName,
        ANSC_HANDLE                 hNamespaceMgr
    );

char*
CcspNsMgrCloneString
  (
        char*                       pContainerName,
        char*                       pString
  );

/**************************************************************
 *
 * Change TR60 data model name to an internal registered name
 *
 * If a table instance number presents in the name, it will be 
 * replaced with "{i}".
 * "Device.AppTable.1.ItemTable.3" to "Device.AppTable.{i}.ItemTable.{i}.
 * The change flag will be set to TRUE;
 *
 * If there's no instance number included, a copy will be returned;
 * If invalid name encountered, an empty string will be returned;
 * The change flag will be set to false;
 *
 **************************************************************/
char*
CcspNsMgrChangeToRegNamespaceName
  (
        char*                       pContainerName,
        char*                       pFullName,
        BOOL*                       pbChange
  );

PVOID
CcspNsMgrAllocateMemory
  (
        char*                       pContainerName,
        ULONG                       ulSize
  );

void
CcspNsMgrFreeMemory
  (
        char*                       pContainerName,
        PVOID                       pMem
  );



#endif
