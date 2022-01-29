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

    module: ccsp_cr_mgr_base.c

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This module implements basic NamespaceMgr functions

        *   CcspCreateNamespaceMgr
        *   CcspFreeNamespaceMgr
        *   CcspNsMgrCleanAll
        *   CcspNsMgrDumpObject
        *   CcspNsMgrCloneString
        *   CcspNsMgrChangeToRegNamespaceName
        *   CcspNsMgrAllocateMemory
        *   CcspNsMgrFreeMemory

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

#include "ansc_platform.h"

#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"

#include "ccsp_cr_definitions.h"
#include "ccsp_cr_interface.h"
#include "ccsp_namespace_mgr.h"
#include "ccsp_ns_mgr_internal_api.h"

#include "ccsp_namespace_mgr.h"

#include "ccsp_memory.h"
#include "safec_lib_common.h"

/**********************************************************************

    prototype:

        ANSC_HANDLE
        CcspCreateNamespaceMgr
            (
                char*                       pContainerName
            );

    description:

        This function is called to create a NamespaceComponent object

    argument:   char*                       pContainerName

    return:     The NamespaceComponent handle;

**********************************************************************/
ANSC_HANDLE
CcspCreateNamespaceMgr
    (
        char*                       pContainerName
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pThisObject  = (PCCSP_NAMESPACE_MGR_OBJECT)NULL;
    errno_t                         rc           = -1;

    pThisObject = (PCCSP_NAMESPACE_MGR_OBJECT)
        AnscAllocateMemory(sizeof(CCSP_NAMESPACE_MGR_OBJECT));

    if( pThisObject == NULL)
    {
        return NULL;
    }
    /*
     * Initialize the common variables and functions.
     */

    rc = STRCPY_S_NOCLOBBER(pThisObject->pContainerName, sizeof(pThisObject->pContainerName), pContainerName);
    ERR_CHK(rc);

    pThisObject->uTotalNamespace                    = 0;

    AnscQueueInitializeHeader(&pThisObject->ComponentQueue);

    pThisObject->GetCountOfRegNamespace             = CcspNsMgrGetCountOfRegNamespace;
    pThisObject->LookforComponent                   = CcspNsMgrLookforComponent;
    pThisObject->RegisterNamespaces                 = CcspNsMgrRegisterNamespaces;
    pThisObject->BuildNamespaces                    = CcspNsMgrBuildNamespaces;
    pThisObject->UnregisterNamespace                = CcspNsMgrUnregisterNamespace;
    pThisObject->UnregisterComponent                = CcspNsMgrUnregisterComponent;
    pThisObject->DiscoverNamespace                  = CcspNsMgrDiscoverNamespace;
    pThisObject->CheckNamespaceDataType             = CcspNsMgrCheckNamespaceDataType;
    pThisObject->GetRegisteredComponents            = CcspNsMgrGetRegisteredComponents;
    pThisObject->GetNamespaceByComponent            = CcspNsMgrGetNamespaceByComponent;
    pThisObject->DumpObject                         = CcspNsMgrDumpObject;
    pThisObject->CleanAll                           = CcspNsMgrCleanAll;

    /* create the hash table */
    pThisObject->hAtomNamespace =
        AnscCreateAtomTable
            (
                (ANSC_HANDLE)pThisObject,
                (ANSC_HANDLE)pThisObject,
                NULL
            );


    return (ANSC_HANDLE)pThisObject;
}

/**********************************************************************

    prototype:

        void
        CcspFreeNamespaceMgr
          (
                char*                       pContainerName,
                ANSC_HANDLE                 hNsComp
          );

    description:

        This function is called to release the CCSP CR handle;

    argument:   char*                       pContainerName,
                The name of container component

                ANSC_HANDLE                                 hCcspNsMgr
                the handle of NamespaceComponent;

    return:     None

**********************************************************************/
void
CcspFreeNamespaceMgr
    (
        char*                       pContainerName,
        ANSC_HANDLE                 hCcspNsMgr
    )
{
    UNREFERENCED_PARAMETER(pContainerName);
    PCCSP_NAMESPACE_MGR_OBJECT      pThisObject = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;

    if( pThisObject != NULL)
    {
        pThisObject->CleanAll(pThisObject);

        if ( pThisObject->SubsysPrefix )
        {
            AnscFreeMemory(pThisObject->SubsysPrefix);
        }

        AnscFreeMemory((PVOID)hCcspNsMgr);
    }
}


/**********************************************************************

    prototype:

        BOOL
        CcspNsMgrCleanAll
            (
                ANSC_HANDLE                 hCcspNsMgr
            );

    description:

        This function is called to clean all the memories in the object.

    argument:   ANSC_HANDLE                                 hCcspNsMgr
                the handle of NamespaceComponent;

    return:     The status of the operation;

**********************************************************************/
BOOL
CcspNsMgrCleanAll
    (
        ANSC_HANDLE                 hCcspNsMgr
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY)NULL;
    PCCSP_NAMESPACE_COMP_OBJECT     pNsComp           = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;
    PANSC_ATOM_TABLE_OBJECT         pAtomTable        = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;

    /* free the namespace compoent queue */
    if ( TRUE )
    {
        pSLinkEntry = AnscQueuePopEntry(&pMyObject->ComponentQueue);

        while ( pSLinkEntry )
        {
            pNsComp         = ACCESS_CCSP_NAMESPACE_COMP_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueuePopEntry(&pMyObject->ComponentQueue);

            CcspFreeNamespaceComponent( pMyObject->pContainerName, pNsComp);
        }
    }

    /* free the hash table */
    if( pAtomTable != NULL)
    {
        pAtomTable->Remove(pAtomTable);

        pMyObject->hAtomNamespace = NULL;
    }

    return TRUE;
}

/**********************************************************************

    prototype:

        BOOL
        CcspNsMgrDumpObject
            (
                ANSC_HANDLE                 hCcspNsMgr
            );

    description:

        This function is called to dump the objects information.

    argument:   ANSC_HANDLE                                 hCcspNsMgr
                the handle of NamespaceComponent;

    return:     The status of the operation;

**********************************************************************/
BOOL
CcspNsMgrDumpObject
    (
        ANSC_HANDLE                 hCcspNsMgr
    )
{
    UNREFERENCED_PARAMETER(hCcspNsMgr);
    return TRUE;
}

/**********************************************************************

    prototype:

        char*
        CcspNsMgrCloneString
            (
                char*                       pContainerName,
                char*                       pString
            );

    description:

        This function is called to clone a string in CcspNamespaceMgr object;

    argument:   
                char*                       pContainerName,
                The name of container component;

                char*                       pString
                The source string;

    return:     The cloned string

**********************************************************************/
char*
CcspNsMgrCloneString
  (
        char*                       pContainerName,
        char*                       pString
  )
{
    UNREFERENCED_PARAMETER(pContainerName);
    char*                       pNewString  = NULL;
    errno_t                     rc          = -1;

    if( pString == NULL)
    {
        return NULL;
    }

    pNewString = (char*)AnscAllocateMemory(AnscSizeOfString(pString) + 1);

    if( pNewString != NULL)
    {
        rc = strcpy_s(pNewString, (AnscSizeOfString(pString) + 1), (char*)pString);
        ERR_CHK(rc);
    }

    return pNewString;
}


/**********************************************************************

    prototype:

        char*
        CcspNsMgrChangeToRegNamespaceName
            (
                char*                       pContainerName,
                char*                       pString,
                BOOL*                       pbChange
            );

    description:

         Change TR60 data model name to an internal registered name
         
         If a table instance number presents in the name, it will be 
         replaced with "{i}".
         "Device.AppTable.1.ItemTable.3." to "Device.AppTable.{i}.ItemTable.{i}.
         The change flag will be set to TRUE;
         
         If there's no instance number included, a copy will be returned;
         If invalid name encountered, an empty string will be returned;
         The change flag will be set to false;
         
    argument:   
                char*                       pContainerName,
                The name of container component;

                char*                       pString
                The source string;

                BOOL*                       pbChange  (OUT)
                The string is changed or not.

    return:     The new string

**********************************************************************/
char*
CcspNsMgrChangeToRegNamespaceName
  (
        char*                       pContainerName,
        char*                       pString,
        BOOL*                       pbChange
  )
{
    char*                           pTemp          = pString;
    ULONG                           ulSizeOfStr    = pString? AnscSizeOfString(pString) : 0;
    ULONG                           ulTokenSize    = 0;
    char                            pBackName[256] = { 0 };
    ULONG                           uCurPos        = 0;
    BOOL                            bChange        = FALSE;

    if( pString == NULL || AnscSizeOfString(pString) == 0)
    {
        return NULL;
    }

    if( pString[0] == '.' || _ansc_strstr(pString, "..") != NULL) 
    /* the name cannot start with '.' and has ".." included */
    {
        return NULL;
    }

    if( AnscSizeOfString(pString) > 255)
    {
        return NULL;
    }

    AnscZeroMemory(pBackName, 256);

    pString     = AnscMoveToNextToken(pString, TR69_PARAM_NAME_SEPARATOR);
    /*RDKB-6137, CID-33200, null check before use*/
    if(pString == NULL)
    {
        return NULL;
    }
    else
    {
        ulTokenSize = AnscSizeOfToken(pString, TR69_PARAM_NAME_SEPARATOR, AnscSizeOfString(pString));

        if(*pString == 0 || ulTokenSize == 0)
        {
            return NULL;
        }

    }

    while ( pString && (*pString != 0) && ulTokenSize && ((ULONG)(pString - pTemp) < ulSizeOfStr) )
    {
        if( pString[0] >= '1' && pString[0] <= '9')
        {
            AnscCopyMemory( pBackName + uCurPos, TR69_PARAM_TABLE_SYMBOL, AnscSizeOfString(TR69_PARAM_TABLE_SYMBOL));

            uCurPos += AnscSizeOfString(TR69_PARAM_TABLE_SYMBOL);

            bChange = TRUE;
        }
        else
        {
            AnscCopyMemory( pBackName + uCurPos, pString, ulTokenSize);

            uCurPos += ulTokenSize;
        }

        pString     += ulTokenSize;

        if( pString != NULL && *pString != 0)
        {
            AnscCopyMemory( pBackName + uCurPos, TR69_PARAM_NAME_SEPARATOR, AnscSizeOfString(TR69_PARAM_NAME_SEPARATOR));

            uCurPos += AnscSizeOfString(TR69_PARAM_NAME_SEPARATOR);

        }

        pString     = AnscMoveToNextToken(pString, TR69_PARAM_NAME_SEPARATOR);

        if (pString != NULL)
        {
            ulTokenSize = AnscSizeOfToken(pString, TR69_PARAM_NAME_SEPARATOR, AnscSizeOfString(pString));
        }
    }

    *pbChange = bChange;

    if( bChange)
    {
        return CcspNsMgrCloneString(pContainerName, pBackName);
    }
    else
    {
        return CcspNsMgrCloneString(pContainerName, pTemp);
    }
}


/**********************************************************************

    prototype:

        PVOID
        CcspNsMgrAllocateMemory
            (
                char*                       pContainerName,
                ULONG                       ulSize
            );

    description:

        This function is called to allocate a memory in a component;

    argument:   
                char*                       pContainerName,
                The name of container component;

                ULONG                       ulSize
                The memory size required.

    return:     The new created memory

**********************************************************************/
PVOID
CcspNsMgrAllocateMemory
  (
        char*                       pContainerName,
        ULONG                       ulSize
  )
{
    UNREFERENCED_PARAMETER(pContainerName);
    return (PVOID)AnscAllocateMemory(ulSize);
}


/**********************************************************************

    prototype:

        void
        CcspNsMgrFreeMemory
            (
                char*                       pContainerName,
                PVOID                       pMemory
            );

    description:

        This function is called to Free a memory in a component;

    argument:   
                char*                       pContainerName,
                The name of container component;

                PVOID                       pMemory
                The memory to be freed;

    return:     None

**********************************************************************/
void
CcspNsMgrFreeMemory
  (
        char*                       pContainerName,
        PVOID                       pMemory
  )
{
    UNREFERENCED_PARAMETER(pContainerName);
    AnscFreeMemory(pMemory);
}
