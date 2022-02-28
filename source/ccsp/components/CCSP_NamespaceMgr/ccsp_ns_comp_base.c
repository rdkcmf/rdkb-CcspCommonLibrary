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

    module: ccsp_ns_comp_base.c

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This module implements basic NamespaceComponent functions

        *   CcspCreateNamespaceComponent
        *   CcspFreeNamespaceComponent
        *   CcspNsCompCleanAll
        *   CcspNsCompDumpObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        06/22/2011    initial revision.

**********************************************************************/

#include "ansc_platform.h"

#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"

#include "ccsp_cr_definitions.h"
#include "ccsp_cr_interface.h"
#include "ccsp_namespace_mgr.h"
#include "ccsp_ns_comp_internal_api.h"

#include "ccsp_namespace_mgr.h"

#include "ccsp_memory.h"

/**********************************************************************

    prototype:

        ANSC_HANDLE
        CcspCreateNamespaceComponent
            (
                char*                       pContainerName
            );

    description:

        This function is called to create a NamespaceComponent object

    argument:   char*                       pContainerName

    return:     The NamespaceComponent handle;

**********************************************************************/
ANSC_HANDLE
CcspCreateNamespaceComponent
    (
        char*                       pContainerName
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT         pThisObject = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;

    pThisObject = (PCCSP_NAMESPACE_COMP_OBJECT)
        AnscAllocateMemory(sizeof(CCSP_NAMESPACE_COMP_OBJECT));
    

    if( pThisObject == NULL)
    {
        return NULL;
    }
    /*
     * Initialize the common variables and functions.
     */

    pThisObject->pContainerName                     = pContainerName;

    AnscQueueInitializeHeader(&pThisObject->ParamspaceQueue);

    pThisObject->UnregisterNamespace                = CcspNsCompUnregisterNamespace;
    pThisObject->GetNamespaceCount                  = CcspNsCompGetNamespaceCount;
    pThisObject->GetNamespaceStringArray            = CcspNsCompGetNamespaceStringArray;
    pThisObject->GetNamespaceStructArray            = CcspNsCompGetNamespaceStructArray;
    pThisObject->BuildFromNamespaceStringArray      = CcspNsCompBuildFromNamespaceStringArray;
    pThisObject->BuildFromNamespaceStructArray      = CcspNsCompBuildFromNamespaceStructArray;
    pThisObject->IsObjectSupported                  = CcspNsCompIsObjectSupported;
    pThisObject->DumpObject                         = CcspNsCompDumpObject;
    pThisObject->CleanAll                           = CcspNsCompCleanAll;

    return (ANSC_HANDLE)pThisObject;
}

/**********************************************************************

    prototype:

        void
        CcspFreeNamespaceComponent
          (
                char*                       pContainerName,
                ANSC_HANDLE                 hNsComp
          );

    description:

        This function is called to release the CCSP CR handle;

    argument:   char*                       pContainerName,
                The name of container component

                ANSC_HANDLE                                 hCcspNsComp
                the handle of NamespaceComponent;

    return:     None

**********************************************************************/
void
CcspFreeNamespaceComponent
    (
        char*                       pContainerName,
        ANSC_HANDLE                 hCcspNsComp
    )
{
    UNREFERENCED_PARAMETER(pContainerName);
    PCCSP_NAMESPACE_COMP_OBJECT     pThisObject = (PCCSP_NAMESPACE_COMP_OBJECT)hCcspNsComp;

    if( pThisObject != NULL)
    {
        pThisObject->CleanAll(pThisObject);

        AnscFreeMemory((PVOID)hCcspNsComp);
    }
}


/**********************************************************************

    prototype:

        BOOL
        CcspNsCompCleanAll
            (
                ANSC_HANDLE                 hCcspNsComp
            );

    description:

        This function is called to clean all the memories in the object.

    argument:   ANSC_HANDLE                                 hCcspNsComp
                the handle of NamespaceComponent;

    return:     The status of the operation;

**********************************************************************/
BOOL
CcspNsCompCleanAll
    (
        ANSC_HANDLE                 hCcspNsComp
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject         = (PCCSP_NAMESPACE_COMP_OBJECT)hCcspNsComp;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY)NULL;
    PCCSP_NAMESPACE                 pNamespace        = (PCCSP_NAMESPACE)NULL;

    if ( TRUE )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->ParamspaceQueue);

        while ( pSLinkEntry )
        {
            pNamespace      = ACCESS_CCSP_NAMESPACE(pSLinkEntry);
            pSLinkEntry     = AnscQueuePopEntry(&pMyObject->ParamspaceQueue);

            
            AnscFreeMemory(pNamespace->pName);
            AnscFreeMemory(pNamespace);
        }
    }

    return TRUE;
}

/**********************************************************************

    prototype:

        BOOL
        CcspNsCompDumpObject
            (
                ANSC_HANDLE                 hCcspNsComp
            );

    description:

        This function is called to dump the objects information.

    argument:   ANSC_HANDLE                                 hCcspNsComp
                the handle of NamespaceComponent;

    return:     The status of the operation;

**********************************************************************/
BOOL
CcspNsCompDumpObject
    (
        ANSC_HANDLE                 hCcspNsComp
    )
{
    UNREFERENCED_PARAMETER(hCcspNsComp);
    return TRUE;
}
