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

    module: ccsp_ns_comp_operation.c

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This module implements NamespaceComponent external apis.

        *   CcspNsCompUnregisterNamespace
        *   CcspNsCompGetNamespaceCount
        *   CcspNsCompGetNamespaceStringArray
        *   CcspNsCompGetNamespaceStructArray
        *   CcspNsCompBuildFromNamespaceStringArray
        *   CcspNsCompBuildFromNamespaceStructArray
        *   CcspNsCompIsObjectSupported

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
#include "ccsp_ns_comp_internal_api.h"
#include "ccsp_namespace_mgr.h"

#include "ccsp_base_api.h"

/**********************************************************************

    prototype:

        int
        CcspNsCompUnregisterNamespace
            (
                ANSC_HANDLE                 hThisObject,
                const char*                 pNamespace
            );

    description:

        This function is called to unregister specified namespace from the component;
        namespace.

    argument:   
                ANSC_HANDLE                 hThisObject,
                the handle of NamespaceComponent object;

                const char*                 pNamespace
                The specified namespace string;

    return:     The status of the operation;

**********************************************************************/
int
CcspNsCompUnregisterNamespace
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pNamespace
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject       = (PCCSP_NAMESPACE_COMP_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY)NULL;
    PCCSP_NAMESPACE                 pCcspName       = NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ParamspaceQueue);

    while ( pSLinkEntry )
    {
        pCcspName       = ACCESS_CCSP_NAMESPACE(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if(strcmp(pCcspName->pName, (char*)pNamespace) == 0 )
        {
            break;
        }
    }

    if( pCcspName != NULL)
    {
        AnscQueuePopEntryByLink(&pMyObject->ParamspaceQueue, &pCcspName->Linkage);

        CcspNsMgrFreeMemory(pMyObject->pContainerName, pCcspName->pName);
        CcspNsMgrFreeMemory(pMyObject->pContainerName, (PVOID)pCcspName);
    }

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        ULONG
        CcspNsCompGetNamespaceCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the count of registered name space;
        namespace.

    argument:   
                ANSC_HANDLE                 hThisObject,
                the handle of NamespaceComponent object;

    return:     The count of name space;

**********************************************************************/
ULONG
CcspNsCompGetNamespaceCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject       = (PCCSP_NAMESPACE_COMP_OBJECT)hThisObject;

    return pMyObject->ParamspaceQueue.Depth;
}

/**********************************************************************

    prototype:

        int
        CcspNsCompGetNamespaceStringArray
            (
                ANSC_HANDLE                 hThisObject,
                char***                     pppNamespace,  
                ULONG*                      pulSize        
            );

    description:

        This function is called to retrieve the registered
        namespace array.

    argument:   
                ANSC_HANDLE                 hThisObject,
                the handle of NamespaceComponent object;

                char***                     pppNamespace,  
                The output name space string array;

                ULONG*                      pulSize        
                The output of size of array;

    return:     The status of the operation;

**********************************************************************/
int
CcspNsCompGetNamespaceStringArray
    (
        ANSC_HANDLE                 hThisObject,
        char***                     pppNamespace,  /* output of namespace str array */
        ULONG*                      pulSize        /* output of array size */
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject       = (PCCSP_NAMESPACE_COMP_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY)NULL;
    PCCSP_NAMESPACE                 pCcspName       = NULL;
    ULONG                           uCount          = pMyObject->ParamspaceQueue.Depth;
    char**                          ppNamespace     = NULL;
    ULONG                           i               = 0;

    if( uCount == 0)
    {
        *pulSize        = 0;
        *pppNamespace   = NULL;

        return CCSP_SUCCESS;
    }

    ppNamespace = (char**)CcspNsMgrAllocateMemory( pMyObject->pContainerName, uCount * sizeof(char*));

    if( ppNamespace == NULL)
    {
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ParamspaceQueue);

    while ( pSLinkEntry )
    {
        pCcspName       = ACCESS_CCSP_NAMESPACE(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        ppNamespace[i] = CcspNsMgrCloneString( pMyObject->pContainerName, pCcspName->pName);

        i ++;
    }

    *pulSize        = uCount;
    *pppNamespace   = ppNamespace;

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        int
        CcspNsCompGetNamespaceStructArray
            (
                ANSC_HANDLE                 hThisObject,
                PVOID**                     ppNamespace,
                ULONG*                      pulSize        
            );

    description:

        This function is called to retrieve the registered
        namespace array.

    argument:   
                ANSC_HANDLE                 hThisObject,
                the handle of NamespaceComponent object;

                PVOID**                     ppNamespace,
                The output name space struct array;

                ULONG*                      pulSize        
                The output of size of array;

    return:     The status of the operation;

**********************************************************************/
int
CcspNsCompGetNamespaceStructArray
    (
        ANSC_HANDLE                 hThisObject,
        PVOID**                     ppNamespace,
        ULONG*                      pulSize        /* output of array size */
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject       = (PCCSP_NAMESPACE_COMP_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY)NULL;
    PCCSP_NAMESPACE                 pCcspName       = NULL;
    ULONG                           uCount          = pMyObject->ParamspaceQueue.Depth;
    PVOID*                          pNamespace      = NULL;
    name_spaceType_t*               pCompStruct     = (name_spaceType_t*)NULL;        
    ULONG                           i               = 0;

    if( uCount == 0)
    {
        *pulSize        = 0;
        *ppNamespace   = NULL;

        return CCSP_SUCCESS;
    }

    pNamespace = (PVOID*)CcspNsMgrAllocateMemory( pMyObject->pContainerName, uCount * sizeof(PVOID));

    if( pNamespace == NULL)
    {
        return CCSP_ERR_MEMORY_ALLOC_FAIL;
    }

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ParamspaceQueue);

    while ( pSLinkEntry )
    {
        pCcspName       = ACCESS_CCSP_NAMESPACE(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        pNamespace[i] = CcspNsMgrCloneString( pMyObject->pContainerName, pCcspName->pName);

        pCompStruct = (name_spaceType_t*)CcspNsMgrAllocateMemory(pMyObject->pContainerName, sizeof(name_spaceType_t));

        if( pCompStruct != NULL)
        {
            pCompStruct->name_space         = CcspNsMgrCloneString(pMyObject->pContainerName, pCcspName->pName);
            pCompStruct->dataType           = pCcspName->uType;

            pNamespace[i] = (PVOID)pCompStruct;
            i ++;
        }
    }

    *pulSize        = uCount;
    *ppNamespace    = pNamespace;

    return CCSP_SUCCESS;
}

/**********************************************************************

    prototype:

        int
        CcspNsCompBuildFromNamespaceStringArray
            (
                ANSC_HANDLE                 hThisObject,
                char**                      ppNamespace,  
                ULONG                       ulSize
            );

    description:

        This function is called to retrieve the registered
        namespace array.

    argument:   
                ANSC_HANDLE                 hThisObject,
                the handle of NamespaceComponent object;

                char**                      ppNamespace,  
                The input name space array;

                ULONG                       ulSize
                The size of the array;

    return:     The status of the operation;

**********************************************************************/
int
CcspNsCompBuildFromNamespaceStringArray
    (
        ANSC_HANDLE                 hThisObject,
        char**                      ppNamespace,   /* input of namespace str array */
        ULONG                       ulSize         /* input of array size */
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject       = (PCCSP_NAMESPACE_COMP_OBJECT)hThisObject;
    PCCSP_NAMESPACE                 pCcspName       = NULL;
    ULONG                           i               = 0;

    for( i = 0; i < ulSize; i ++)
    {
        pCcspName = CcspNsMgrAllocateMemory(pMyObject->pContainerName, sizeof(CCSP_NAMESPACE));

        if( pCcspName != NULL)
        {
            pCcspName->bWritable    = FALSE;
            pCcspName->uType        = 0;
            pCcspName->pName        = CcspNsMgrCloneString(pMyObject->pContainerName, ppNamespace[i]);

            AnscQueuePushEntry(&pMyObject->ParamspaceQueue, &pCcspName->Linkage);

        }
    }

    return CCSP_SUCCESS;

}

/**********************************************************************

    prototype:

        int
        CcspNsCompBuildFromNamespaceStructArray
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      pNamespace,
                ULONG                       ulSize
            );

    description:

        This function is called to retrieve the registered
        namespace array.

    argument:   
                ANSC_HANDLE                 hThisObject,
                the handle of NamespaceComponent object;

                PVOID*                      pNamespace,
                The input name space struct array;

                ULONG                       ulSize
                The size of the array;

    return:     The status of the operation;

**********************************************************************/
int
CcspNsCompBuildFromNamespaceStructArray
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      pNamespace,
        ULONG                       ulSize     
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject       = (PCCSP_NAMESPACE_COMP_OBJECT)hThisObject;
    PCCSP_NAMESPACE                 pCcspName       = NULL;
    name_spaceType_t**              ppSpaceType     = (name_spaceType_t**)pNamespace;
    ULONG                           i               = 0;

    for( i = 0; i < ulSize; i ++)
    {
        pCcspName = CcspNsMgrAllocateMemory(pMyObject->pContainerName, sizeof(CCSP_NAMESPACE));

        if( pCcspName != NULL)
        {
            pCcspName->bWritable    = FALSE;

            if( ppSpaceType[i] != NULL)
            {
                pCcspName->uType        = (int)ppSpaceType[i]->dataType;
                pCcspName->pName        = CcspNsMgrCloneString(pMyObject->pContainerName, (char*)ppSpaceType[i]->name_space);

                AnscQueuePushEntry(&pMyObject->ParamspaceQueue, &pCcspName->Linkage);
            }
            else  /*RDKB-6136, CID-33095, pluging resource leak if ppSpaceType[i] == NULL */
            {
                CcspNsMgrFreeMemory(pMyObject->pContainerName,(PVOID )pCcspName);
            }
        }
    }

    return CCSP_SUCCESS;

}

/**********************************************************************

    prototype:

        BOOL
        CcspNsCompIsObjectSupported
            (
                ANSC_HANDLE                 hThisObject,
                const char*                 pObjNamespace,
                BOOL                        bNextLevel
            );

    description:

        This function is called to retrieve the registered
        namespace array.

    argument:   
                ANSC_HANDLE                 hThisObject,
                the handle of NamespaceComponent object;

                const char*                 pObjNamespace
                The specified object name space;

                BOOL                        bNextLevel
                next level is true or not;

    return:     TRUE or FALSE;

**********************************************************************/
BOOL
CcspNsCompIsObjectSupported
    (
        ANSC_HANDLE                 hThisObject,
        const char*                 pObjNamespace,
        BOOL                        bNextLevel
    )
{
    PCCSP_NAMESPACE_COMP_OBJECT     pMyObject       = (PCCSP_NAMESPACE_COMP_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = (PSINGLE_LINK_ENTRY)NULL;
    PCCSP_NAMESPACE                 pCcspName       = NULL;
    char*                           pString         = NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->ParamspaceQueue);

    while ( pSLinkEntry )
    {
        pCcspName       = ACCESS_CCSP_NAMESPACE(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        if( _ansc_strstr( pCcspName->pName, (char*)pObjNamespace) == pCcspName->pName)
        {
            if( bNextLevel) /* next level only */
            {
                pString = pCcspName->pName + AnscSizeOfString(pObjNamespace);
                pString = _ansc_strstr(pString, TR69_PARAM_NAME_SEPARATOR);

                /* next parameter */
                if( pString == NULL)
                {
                    return TRUE;
                }

                if(_ansc_strstr(pString,TR69_NAME_TABLE_END) == pString )
                {
                    /* if it's a table */
                    return TRUE;
                }

				pString += AnscSizeOfString(TR69_PARAM_NAME_SEPARATOR);
                pString  = _ansc_strstr(pString, TR69_PARAM_NAME_SEPARATOR);

                /* next object */
                if( pString == NULL)
                {
                    return TRUE;
                }

            }
            else
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}
