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

    module: ccsp_ns_mgr_operation.c

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This module implements NamespaceMgr external apis.

        *   CcspNsMgrGetCountOfRegNamespace
        *   CcspNsMgrLookforComponent
        *   CcspNsMgrRegisterNamespace
        *   CcspNsMgrBuildNamespace
        *   CcspNsMgrUnregisterNamespace
        *   CcspNsMgrUnregisterComponent
        *   CcspNsMgrDiscoverNamespace
        *   CcspNsMgrCheckNamespaceDataType
        *   CcspNsMgrGetRegisteredComponents
        *   CcspNsMgrGetNamespaceByComponent

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        06/24/2011    initial revision.

**********************************************************************/

#include "ansc_platform.h"

#include "ansc_ato_interface.h"
#include "ansc_ato_external_api.h"

#include "ccsp_cr_definitions.h"
#include "ccsp_cr_interface.h"
#include "ccsp_ns_mgr_internal_api.h"
#include "ccsp_namespace_mgr.h"
#include "safec_lib_common.h"

/**********************************************************************

    prototype:

        ULONG
        CcspNsMgrGetCountOfRegNamespace
            (
                ANSC_HANDLE                 hCcspNsMgr
            );

    description:

        This function is called to retrieve the total count of registered
        namespace.

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

    return:     The count;

**********************************************************************/
ULONG
CcspNsMgrGetCountOfRegNamespace
    (
        ANSC_HANDLE                 hCcspNsMgr
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;

    return pMyObject->uTotalNamespace;
}

/**********************************************************************

    prototype:

        ANSC_HANDLE
        CcspNsMgrLookforComponent
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pCompName
            );

    description:

        This function is called to retrieve the NamespaceComponent handle
        specified by the name.

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pCompName
                The name of the component;

    return:     The object handle;

**********************************************************************/
ANSC_HANDLE
CcspNsMgrLookforComponent
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pCompName
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PCCSP_NAMESPACE_COMP_OBJECT     pCompInfo         = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY)NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ComponentQueue);

    while ( pSLinkEntry )
    {
        pCompInfo       = ACCESS_CCSP_NAMESPACE_COMP_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pCompName != NULL && strcmp(pCompInfo->pCompName, (char*)pCompName) == 0 )
        {
            return  (ANSC_HANDLE)pCompInfo;
        }
    }

    return NULL;
}

/**********************************************************************

    prototype:

        int
        CcspNsMgrRegisterNamespaces
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pCompName,
                const char*                 pDbusPath,
                const char*                 pPrefix,
                PVOID*                      ppNamesapceArray,
                ULONG                       ulSize         
            );

    description:

        This function is called to register new namespace in a component.

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pCompName,
                The component name;

                const char*                 pDbusPath,
                The d-bus path;

                const char*                 pPrefix,
                The subsystem prefix;

                PVOID*                      ppNamesapceArray,
                The namespace array;

                ULONG                       ulSize         
                The size of the array;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrRegisterNamespaces
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pCompName,
        const char*                 pDbusPath,
        const char*                 pPrefix,
        PVOID*                      ppNamespaceArray,
        ULONG                       ulSize         
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    name_spaceType_t**              ppSpaceType       = (name_spaceType_t**)ppNamespaceArray;
    PCCSP_NAMESPACE_COMP_OBJECT     pNSComp           = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;
    PANSC_ATOM_DESCRIPTOR           pAtomItem         = NULL;
    char*                           pString           = NULL;
    ULONG                           i                 = 0;
    errno_t                         rc                = -1;

    /* Validate the name space */
    for( i = 0; i < ulSize; i ++)
    {
        if( ppSpaceType[i] == NULL)
        {
            return CCSP_CR_ERR_INVALID_PARAM;
        }

        pString = (char*)ppSpaceType[i]->name_space;

        if( pString == NULL || AnscSizeOfString(pString) < 4)
        {
            return CCSP_CR_ERR_INVALID_PARAM;
        }

        if( pString[0] == TR69_PARAM_NAME_SEPARATOR_CHAR || _ansc_strstr(pString, "..") != NULL) 
        {
            /* the name cannot start with '.' and has ".." included */
            return CCSP_CR_ERR_INVALID_PARAM;
        }

        if( pString[AnscSizeOfString(pString)-1] == TR69_PARAM_NAME_SEPARATOR_CHAR)
        {
            /* object level name space registration is also not allowed */
            return CCSP_CR_ERR_INVALID_PARAM;
        }

        pAtomItem = (PANSC_ATOM_DESCRIPTOR)pAtomNamespace->GetAtomByName(pAtomNamespace, pString);

        if( pAtomItem != NULL)
        {
            /* check whether the comp name is the same */
            pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)pAtomItem->hContext;

            if (pNSComp && !strcmp(pNSComp->pCompName, (char*)pCompName) == 0 )
            {
                AnscTrace("The namespace '%s' is already registered.\n", pString);
 
                return CCSP_CR_ERR_NAMESPACE_OVERLAP;
            }
            else
            {
                return CCSP_SUCCESS;
            }
        }
    }

    /* add the name space information */
    pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)pMyObject->LookforComponent(pMyObject, pCompName);
    if( pNSComp == NULL)
    {  
        /* create a new NamespaceComponent object */
        pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)CcspCreateNamespaceComponent(pMyObject->pContainerName);

        if( pNSComp == NULL)
        {
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }

        rc = STRCPY_S_NOCLOBBER(pNSComp->pCompName, sizeof(pNSComp->pCompName), (char*)pCompName);
        ERR_CHK(rc);
        rc = STRCPY_S_NOCLOBBER(pNSComp->pDbusPath, sizeof(pNSComp->pDbusPath), (char*)pDbusPath);
        ERR_CHK(rc);

        if( pPrefix != NULL)
        {
            rc = STRCPY_S_NOCLOBBER(pNSComp->pPrefix, sizeof(pNSComp->pPrefix), (char*)pPrefix);
            ERR_CHK(rc);
        }

        AnscQueuePushEntry(&pMyObject->ComponentQueue, &pNSComp->Linkage);
    }

    pNSComp->BuildFromNamespaceStructArray(pNSComp, ppNamespaceArray, ulSize);

    /* add them to the hash table */
    for( i = 0; i < ulSize; i ++)
    {
        if( ppSpaceType[i] != NULL)
        {
            pAtomNamespace->AddAtomByName(pAtomNamespace, (char*)ppSpaceType[i]->name_space, (ULONG)ppSpaceType[i]->dataType, (ANSC_HANDLE)pNSComp);
        }
    }

    /* add the count */
    pMyObject->uTotalNamespace += ulSize;

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        int
        CcspNsMgrBuildNamespaces
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pCompName,
                const char*                 pDbusPath,
                const char*                 pPrefix,
                PVOID*                      ppNamesapceArray,
                ULONG                       ulSize         
            );

    description:

        This function is called to build namespace in a component.
        This maybe called in a PA.

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pCompName,
                The component name;

                const char*                 pDbusPath,
                The d-bus path;

                const char*                 pPrefix,
                The subsystem prefix;

                PVOID*                      ppNamespaceArray,
                The namespace array;

                ULONG                       ulSize         
                The size of the array;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrBuildNamespaces
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pCompName,
        const char*                 pDbusPath,
        const char*                 pPrefix,
        PVOID*                      ppNamespaceArray,
        ULONG                       ulSize         
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    name_spaceType_t**              ppSpaceType       = (name_spaceType_t**)ppNamespaceArray;
    PCCSP_NAMESPACE_COMP_OBJECT     pNSComp           = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;
    name_spaceType_t**              ppSpaceCopy       = (name_spaceType_t**)NULL;
    PANSC_ATOM_DESCRIPTOR           pAtomItem         = NULL;
    ULONG                           i                 = 0;
    ULONG                           uCount            = 0;
    PVOID*                          ppCopy            = NULL;
    errno_t                         rc                = -1;

    if ( !pAtomNamespace )
    {
        pMyObject->hAtomNamespace =
            AnscCreateAtomTable
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pMyObject,
                    NULL
                );
        
        pAtomNamespace = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;

        if ( !pAtomNamespace )
        {
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }
    }

    /* The name space infor comes from CR, we don't need to validate them */
    /* Kang has a new requrement to check the confliction. */
    ppCopy = (PVOID*)AnscAllocateMemory( ulSize * sizeof(PVOID));
    
    if( ppCopy == NULL)
    {
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    ppSpaceCopy       = (name_spaceType_t**)ppCopy;

    for( i = 0; i < ulSize; i ++)
    {
        pAtomItem = (PANSC_ATOM_DESCRIPTOR)pAtomNamespace->GetAtomByName(pAtomNamespace, (char*)ppSpaceType[i]->name_space);

        if( pAtomItem != NULL)
        {
            AnscTrace("Namespace '%s' registered already.\n", (char*)ppSpaceType[i]->name_space);
        }
        else
        {
            ppSpaceCopy[uCount] = ppSpaceType[i];

            uCount ++;
        }
    }

    pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)
        pMyObject->LookforComponent(pMyObject, pCompName);

    if( pNSComp == NULL)
    {
        /* create a new NamespaceComponent object */
        pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)CcspCreateNamespaceComponent(pMyObject->pContainerName);

        if( pNSComp == NULL)
        {
            AnscFreeMemory(ppCopy);

            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }

        AnscQueuePushEntry(&pMyObject->ComponentQueue, &pNSComp->Linkage);
        if(pCompName != NULL)
        {
           rc = STRCPY_S_NOCLOBBER(pNSComp->pCompName, sizeof(pNSComp->pCompName), (char*)pCompName);
           ERR_CHK(rc);
        }
        if(pDbusPath != NULL)
        {
           rc = STRCPY_S_NOCLOBBER(pNSComp->pDbusPath, sizeof(pNSComp->pDbusPath), (char*)pDbusPath);
           ERR_CHK(rc);
        }

        if( pPrefix != NULL)
        {
            rc = STRCPY_S_NOCLOBBER(pNSComp->pPrefix, sizeof(pNSComp->pPrefix), (char*)pPrefix);
            ERR_CHK(rc);
        }
    }

    pNSComp->BuildFromNamespaceStructArray(pNSComp, ppCopy, uCount);

    /* add them to the hash table */
    for( i = 0; i < uCount; i ++)
    {
        pAtomNamespace->AddAtomByName(pAtomNamespace, (char*)ppSpaceCopy[i]->name_space, (ULONG)ppSpaceCopy[i]->dataType, (ANSC_HANDLE)pNSComp);
    }

    /* add the count */
    pMyObject->uTotalNamespace += uCount;

    AnscFreeMemory(ppCopy);

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        int
        CcspNsMgrUnregisterNamespace
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pCompName,
                const char*                 pNamespace
            );

    description:

        This function is called to unregister specified namespace;

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pCompName,
                The component name;

                const char*                 pNamespace,
                The specified namespace to be unregistered;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrUnregisterNamespace
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pCompName,
        const char*                 pNamespace
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PCCSP_NAMESPACE_COMP_OBJECT     pNSComp           = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;
    PANSC_ATOM_DESCRIPTOR           pAtomItem         = NULL;

    pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)
        pMyObject->LookforComponent(pMyObject, pCompName);

    if( pNSComp->ParamspaceQueue.Depth == 0 && pNamespace != NULL)
    {
        return CCSP_CR_ERR_NAMESPACE_MISMATCH;        
    }
    else if( pNSComp->ParamspaceQueue.Depth != 0 && pNamespace == NULL)
    {
        return CCSP_CR_ERR_INVALID_PARAM;
    }

    pAtomItem = (PANSC_ATOM_DESCRIPTOR)pAtomNamespace->GetAtomByName(pAtomNamespace, (char*)pNamespace);

    if( pAtomItem == NULL)
    {
        return CCSP_CR_ERR_NAMESPACE_MISMATCH;
    }

    /* remove the name space from the hash table */
    pAtomNamespace->DelAtomByName(pAtomNamespace, (char*)pNamespace);

    /* remove the namespace from component */
    pMyObject->uTotalNamespace --;

    return pNSComp->UnregisterNamespace(pNSComp, pNamespace);
}

/**********************************************************************

    prototype:

        int
        CcspNsMgrUnregisterComponent
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pCompName
            );

    description:

        This function is called to unregister specified component;

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pCompName,
                The specified component name;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrUnregisterComponent
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pCompName
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PCCSP_NAMESPACE_COMP_OBJECT     pNSComp           = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;
    PCCSP_NAMESPACE                 pNameInfo         = (PCCSP_NAMESPACE)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY)NULL;

    pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)
        pMyObject->LookforComponent(pMyObject, pCompName);

    if( pNSComp == NULL)
    {
        return CCSP_CR_ERR_UNKNOWN_COMPONENT;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pNSComp->ParamspaceQueue);

    while ( pSLinkEntry )
    {
        pNameInfo       = ACCESS_CCSP_NAMESPACE(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        pAtomNamespace->DelAtomByName(pAtomNamespace, pNameInfo->pName);
    }

    pMyObject->uTotalNamespace -= pNSComp->ParamspaceQueue.Depth;

    /* remove the component from queue and free it */
    AnscQueuePopEntryByLink(&pMyObject->ComponentQueue, &pNSComp->Linkage);
    CcspFreeNamespaceComponent(pMyObject->pContainerName, (ANSC_HANDLE)pNSComp);

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        int
        CcspNsMgrDiscoverNamespace
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pNamespace,
                const char*                 pPrefix,
                BOOL                        bNextLevel,
                PVOID**                     ppComponent, 
                ULONG*                      pulSize      
            );

    description:

        This function is called to discover specified namespace.

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pNamespace,
                The specified namespace;

                const char*                 pPrefix
                The specified subsystem prefix;

                BOOL                        bNextLevel,
                next level is true or not;

                PVOID**                     ppComponent
                The output component struct array;

                ULONG*                      pulSize     
                The output of the size of arrray;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrDiscoverNamespace
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pNamespace,
        const char*                 pPrefix,
        BOOL                        bNextLevel,
        PVOID**                     ppComponent,   /* output of component struct array */
        ULONG*                      pulSize        /* output of array size */
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;
    PCCSP_NAMESPACE_COMP_OBJECT     pNSComp           = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;    
    PANSC_ATOM_DESCRIPTOR           pAtomItem         = NULL;
    char*                           pNewSpace         = NULL;
    BOOL                            bChange           = FALSE;
    int                             iStatus           = CCSP_SUCCESS;
    ULONG                           uCount            = 0;
    PVOID*                          pCompArray        = NULL;
    componentStruct_t*              pCompStruct       = (componentStruct_t*)NULL;        
    ANSC_HANDLE                     hArray[64]        = { 0 };
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY)NULL;
    ULONG                           i                 = 0;

    pNewSpace = CcspNsMgrChangeToRegNamespaceName(pMyObject->pContainerName, (char*)pNamespace, &bChange);

    if( pNewSpace == NULL)
    {
        return CCSP_CR_ERR_INVALID_PARAM;
    }

    if( pNewSpace[AnscSizeOfString(pNewSpace) - 1] != TR69_PARAM_NAME_SEPARATOR_CHAR)
    {
        /* parameter level, search for the hash table is fine. */
        pAtomItem = (PANSC_ATOM_DESCRIPTOR)pAtomNamespace->GetAtomByName(pAtomNamespace, (char*)pNewSpace);
        
        if( pAtomItem == NULL && bChange)
        {
            pAtomItem = (PANSC_ATOM_DESCRIPTOR)pAtomNamespace->GetAtomByName(pAtomNamespace, (char*)pNamespace);
        }

        if( pAtomItem == NULL)
        {
            iStatus = CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;

            goto EXIT;
        }

        pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)pAtomItem->hContext;

        /* check the match of prefix */
        if( pPrefix == NULL || AnscSizeOfString(pPrefix) == 0)
        {
            if( AnscSizeOfString(pNSComp->pPrefix) != 0)
            {
                return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
            }
        }
        else if((AnscSizeOfString(pNSComp->pPrefix) != 0  && !strcmp((char*)pPrefix, pNSComp->pPrefix) == 0 ) ||
                (0 == AnscSizeOfString(pMyObject->SubsysPrefix)) )
        {
            return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
        }

        /* For parameter level name space, only one component is possible */
        uCount   = 1;

        pCompArray = (PVOID*)CcspNsMgrAllocateMemory(pMyObject->pContainerName, uCount * sizeof(PVOID));

        if( pCompArray == NULL)
        {
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }

        pCompStruct = (componentStruct_t*)CcspNsMgrAllocateMemory(pMyObject->pContainerName, sizeof(componentStruct_t));

        if( pCompStruct != NULL)
        {
            pCompArray[0] = (PVOID)pCompStruct;
            
            pCompStruct->componentName      = CcspNsMgrCloneString(pMyObject->pContainerName, pNSComp->pCompName);
            pCompStruct->dbusPath           = CcspNsMgrCloneString(pMyObject->pContainerName, pNSComp->pDbusPath);
            pCompStruct->remoteCR_name      = NULL;
            pCompStruct->remoteCR_dbus_path = NULL;
            pCompStruct->type               = pAtomItem->AtomId;
        }
    }
    else
    {
        /* object level */
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ComponentQueue);

        while ( pSLinkEntry )
        {
            pNSComp         = ACCESS_CCSP_NAMESPACE_COMP_OBJECT(pSLinkEntry);
            pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

            if( pNSComp->IsObjectSupported(pNSComp, (char*)pNewSpace, bNextLevel))
            {
                /* check the match of prefix */
                if( pPrefix == NULL || AnscSizeOfString(pPrefix) == 0)
                {
                    if( AnscSizeOfString(pNSComp->pPrefix) == 0)
                    {
                        hArray[uCount] = (ANSC_HANDLE)pNSComp;
                        uCount ++;
                    }
                }
                else if( (AnscSizeOfString(pNSComp->pPrefix) == 0 && (!pMyObject->SubsysPrefix || strcmp((char*)pPrefix, pMyObject->SubsysPrefix) == 0 )) || 
                         strcmp((char*)pPrefix, pNSComp->pPrefix) == 0 )
                {
                    hArray[uCount] = (ANSC_HANDLE)pNSComp;
                    uCount ++;
                }
            }
        }

        if( uCount == 0 && bChange)
        {
            /* run with original name space again */
            pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ComponentQueue);

            while ( pSLinkEntry )
            {
                pNSComp         = ACCESS_CCSP_NAMESPACE_COMP_OBJECT(pSLinkEntry);
                pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

                if( pNSComp->IsObjectSupported(pNSComp, (char*)pNamespace, bNextLevel))
                {
                    /* check the match of prefix */
                    if( pPrefix == NULL || AnscSizeOfString(pPrefix) == 0)
                    {
                        if( AnscSizeOfString(pNSComp->pPrefix) == 0)
                        {
                            hArray[uCount] = (ANSC_HANDLE)pNSComp;
                            uCount ++;
                        }
                    }
                    else if( (AnscSizeOfString(pNSComp->pPrefix) == 0 && (!pMyObject->SubsysPrefix || strcmp((char*)pPrefix, pMyObject->SubsysPrefix) == 0 )) || 
                             strcmp((char*)pPrefix, pNSComp->pPrefix) == 0 )
                    {
                        hArray[uCount] = (ANSC_HANDLE)pNSComp;
                        uCount ++;
                    }
                }
            }
        }

        if( uCount == 0)
        {
             iStatus = CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;

             goto EXIT;
        }

        /* construct the response */
        pCompArray = (PVOID*)CcspNsMgrAllocateMemory(pMyObject->pContainerName, uCount * sizeof(PVOID));

        if( pCompArray == NULL)
        {
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }

        for( i = 0; i < uCount; i ++)
        {
            pNSComp       = (PCCSP_NAMESPACE_COMP_OBJECT)hArray[i];
            pCompStruct   = (componentStruct_t*)CcspNsMgrAllocateMemory(pMyObject->pContainerName, sizeof(componentStruct_t));

            if( pCompStruct == NULL)
            {
                /* RDKB-6138, CID-33187, free unused memory */ 
                if(i>0)
                {

                    for(--i; (int)i>=0;--i)
                    {
                        pCompStruct = (componentStruct_t*)pCompArray[i];

                        if(pCompStruct)
                        {

                            if(pCompStruct->componentName)
                            {
                                CcspNsMgrFreeMemory(pMyObject->pContainerName, pCompStruct->componentName);
                            }

                            if(pCompStruct->dbusPath)
                            {
                                CcspNsMgrFreeMemory(pMyObject->pContainerName, pCompStruct->dbusPath);
                            }

                            CcspNsMgrFreeMemory(pMyObject->pContainerName, pCompStruct);
                        }

                    }

                }

                CcspNsMgrFreeMemory(pMyObject->pContainerName,pCompArray);
                return CCSP_ERR_MEMORY_ALLOC_FAIL;
            }

            pCompArray[i] = (PVOID)pCompStruct;
            
            pCompStruct->componentName      = CcspNsMgrCloneString(pMyObject->pContainerName, pNSComp->pCompName);
            pCompStruct->dbusPath           = CcspNsMgrCloneString(pMyObject->pContainerName, pNSComp->pDbusPath);
            pCompStruct->remoteCR_name      = NULL;
            pCompStruct->remoteCR_dbus_path = NULL;
            pCompStruct->type               = ccsp_none;
        }
    }

    *ppComponent = (PVOID*)pCompArray;
    *pulSize    = uCount;

EXIT:

    if( pNewSpace != NULL)
    {
        CcspNsMgrFreeMemory(pMyObject->pContainerName, pNewSpace);
    }

    return iStatus;


}

/**********************************************************************

    prototype:

        int
        CcspNsMgrCheckNamespaceDataType
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pNamespace,
                ULONG                       uDataType,
                BOOL*                       pbMatch
            );

    description:

        This function is called to discover specified namespace.

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pNamespace,
                The specified namespace;

                ULONG                       uDataType,
                The assumed data type;

                BOOL*                       pbMatch
                The output of match or not;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrCheckNamespaceDataType
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pNamespace,
        ULONG                       uDataType,
        BOOL*                       pbMatch
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PANSC_ATOM_TABLE_OBJECT         pAtomNamespace    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomNamespace;
    PANSC_ATOM_DESCRIPTOR           pAtomItem         = NULL;

    if( pNamespace == NULL)
    {
        return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
    }

    pAtomItem = (PANSC_ATOM_DESCRIPTOR)pAtomNamespace->GetAtomByName(pAtomNamespace, (char*)pNamespace);

    if( pAtomItem == NULL)
    {
        return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
    }

    if( pAtomItem->AtomId != uDataType)
    {
        *pbMatch = FALSE;
    }
    else
    {
        *pbMatch = TRUE;
    }

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        int
        CcspNsMgrGetRegisteredComponents
            (
                ANSC_HANDLE                 hCcspNsMgr,
                PVOID**                     ppCompName,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve the array of registered component 
        information.

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                PVOID**                     ppCompName,
                The output array of component information;

                ULONG*                      pulSize
                The output of the size of the array;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrGetRegisteredComponents
    (
        ANSC_HANDLE                 hCcspNsMgr,
        PVOID**                     ppCompName,
        ULONG*                      pulSize
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PCCSP_NAMESPACE_COMP_OBJECT     pCompInfo         = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY)NULL;
    int                             uCount            = pMyObject->ComponentQueue.Depth;
    int                             i                 = 0;
    PVOID*                          pCompArray        = NULL;
    registeredComponent_t*          pCompStruct       = NULL;

    pCompArray = (PVOID*)CcspNsMgrAllocateMemory(pMyObject->pContainerName, uCount * sizeof(PVOID));

    if( pCompArray == NULL)
    {
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ComponentQueue);

    while ( pSLinkEntry )
    {
        pCompInfo       = ACCESS_CCSP_NAMESPACE_COMP_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        pCompStruct = (registeredComponent_t*)CcspNsMgrAllocateMemory(pMyObject->pContainerName, sizeof(registeredComponent_t));

        if( pCompStruct != NULL)
        {
            pCompStruct->componentName      = CcspNsMgrCloneString(pMyObject->pContainerName,pCompInfo->pCompName);
            pCompStruct->dbusPath           = CcspNsMgrCloneString(pMyObject->pContainerName,pCompInfo->pDbusPath);
            pCompStruct->subsystem_prefix   = CcspNsMgrCloneString(pMyObject->pContainerName,pCompInfo->pPrefix);

            pCompArray[i] = (PVOID)pCompStruct;
            i ++;
        }
    }

    *ppCompName = (PVOID)pCompArray;
    *pulSize    = uCount;

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        int
        CcspNsMgrGetNamespaceByComponent
            (
                ANSC_HANDLE                 hCcspNsMgr,
                const char*                 pCompName,                
                PVOID**                     ppNamespace,
                ULONG*                      pulSize
            );

    description:

        This function is called to retrieve namespace supported
        by the specified component;

    argument:   
                ANSC_HANDLE                 hCcspNsMgr,
                the handle of NamespaceMgr object;

                const char*                 pCompName,                
                The input component name;

                PVOID**                     ppNamespace,
                The output array of namespace;

                ULONG*                      pulSize
                The output of size of array;

    return:     The status of the operation

**********************************************************************/
int
CcspNsMgrGetNamespaceByComponent
    (
        ANSC_HANDLE                 hCcspNsMgr,
        const char*                 pCompName,                
        PVOID**                     ppNamespace,
        ULONG*                      pulSize
    )
{
    PCCSP_NAMESPACE_MGR_OBJECT      pMyObject         = (PCCSP_NAMESPACE_MGR_OBJECT)hCcspNsMgr;
    PCCSP_NAMESPACE_COMP_OBJECT     pNSComp           = (PCCSP_NAMESPACE_COMP_OBJECT)NULL;

    pNSComp = (PCCSP_NAMESPACE_COMP_OBJECT)
        pMyObject->LookforComponent(pMyObject, pCompName);

    if( pNSComp == NULL)
    {
        return CCSP_CR_ERR_UNKNOWN_COMPONENT;
    }

    return pNSComp->GetNamespaceStructArray(pNSComp, ppNamespace, pulSize);
}

