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

    module: ccsp_ns_mgr_internal_api.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by CCSP_NAMESPACE_MGR_OBJECT 
        object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        06/23/2011    initial revision.

**********************************************************************/

#ifndef  _CCSP_NS_MGR_INTERNAL_API_H
#define  _CCSP_NS_MGR_INTERNAL_API_H

/***********************************************************
         FUNCTIONS IMPLEMENTED IN CCSP_NS_MGR_OPERATION.C
***********************************************************/
ULONG
CcspNsMgrGetCountOfRegNamespace
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
CcspNsMgrLookforComponent
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName
    );

int
CcspNsMgrRegisterNamespaces
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        const char*                 pDbusPath,
        const char*                 pPrefix,
        PVOID*                      pNamesapceArray,
        ULONG                       ulSize         
    );

int
CcspNsMgrBuildNamespaces
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        const char*                 pDbusPath,
        const char*                 pPrefix,
        PVOID*                      ppNamesapceArray,
        ULONG                       ulSize         
    );

int
CcspNsMgrUnregisterNamespace
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,
        const char*                 pNamespace
    );

int
CcspNsMgrUnregisterComponent
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName
    );

int
CcspNsMgrDiscoverNamespace
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace,
        const char*                 pPrefix,
        BOOL                        bNextLevel,
        PVOID**                     ppComponent,   /* output of component struct array */
        ULONG*                      pulSize        /* output of array size */
    );

/* Check whether the data type of given name space matched or not */
int
CcspNsMgrCheckNamespaceDataType
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace,
        ULONG                       uDataType,
        BOOL*                       pbMatch
    );

int
CcspNsMgrGetRegisteredComponents
    (
        ANSC_HANDLE                 hThisObject,
        PVOID**                     ppCompName,
        ULONG*                      pulSize
    );

int
CcspNsMgrGetNamespaceByComponent
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pCompName,                
        PVOID**                     ppNamespace,
        ULONG*                      pulSize
    );

/***********************************************************
         FUNCTIONS IMPLEMENTED IN CCSP_CR_COMP_BASE.c
***********************************************************/
BOOL
CcspNsMgrCleanAll
    (
        ANSC_HANDLE                 hCcspNsMgr
    );

BOOL
CcspNsMgrDumpObject
    (
        ANSC_HANDLE                 hCcspNsMgr
    );

#endif
