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

    module: ccsp_ns_comp_internal_api.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the internal functions provided by CCSP_NAMESPACE_COMP_OBJECT object.

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

#ifndef  _CCSP_NS_COMP_INTERNAL_API_H
#define  _CCSP_NS_COMP_INTERNAL_API_H

/***********************************************************
         FUNCTIONS IMPLEMENTED IN CCSP_NS_COMP_OPERATION.C
***********************************************************/
/* unregister namespace */
int
CcspNsCompUnregisterNamespace
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace
    );

ULONG
CcspNsCompGetNamespaceCount
    (
        ANSC_HANDLE                 hThisObject
    );

int
CcspNsCompGetNamespaceStringArray
    (
        ANSC_HANDLE                 hThisObject,
        char***                     pppNamespace,  /* output of namespace str array */
        ULONG*                      pulSize        /* output of array size */
    );

int
CcspNsCompGetNamespaceStructArray
    (
        ANSC_HANDLE                 hThisObject,
        PVOID**                     ppNamespace,
        ULONG*                      pulSize        /* output of array size */
    );

int
CcspNsCompBuildFromNamespaceStringArray
    (
        ANSC_HANDLE                 hThisObject,
        char**                      ppNamespace,   /* input of namespace str array */
        ULONG                       ulSize         /* input of array size */
    );

int
CcspNsCompBuildFromNamespaceStructArray
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      pNamespace,
        ULONG                       ulSize         /* input of array size */
    );

/* check whether an object namespace is supported or not */
BOOL
CcspNsCompIsObjectSupported
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pObjNamespace,
        BOOL                        bNextLevel
    );

/***********************************************************
         FUNCTIONS IMPLEMENTED IN CCSP_CR_COMP_BASE.c
***********************************************************/
BOOL
CcspNsCompCleanAll
    (
        ANSC_HANDLE                 hNsComp
    );

BOOL
CcspNsCompDumpObject
    (
        ANSC_HANDLE                 hNsComp
    );

#endif
