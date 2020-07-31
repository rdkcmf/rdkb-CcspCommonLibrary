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

    module:	bspeng_list_co_base.c

        For BSP Engine List Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspTemplateListCOCreate
        *   BspTemplateListCORemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BspTemplateListCOCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the String Component object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created string component object.

**********************************************************************/

ANSC_HANDLE
BspTemplateListCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_LIST_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_LIST_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->Remove                       = BspTemplateListCORemove;
    pMyObject->Initialize                   = BspTemplateListCOInitialize;

    pMyObject->Initialize((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateListCORemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateListCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject    = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_READER_OBJECT     pReader      = (PBSP_TEMPLATE_READER_OBJECT)pMyObject->hReader;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    ULONG                           ulSize, i;
    PBSP_TEMPLATE_OBJECT            pTemplate;
    char                            *pName;
    PSINGLE_LINK_ENTRY              pSLinkEntry, pSLinkNext;
    PBSP_TEMPLATE_REG_PAGE_INFO     pInfo;

    if ( pMyObject->hList )
    {
        pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
        ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
        for (i = 0; i < ulSize; i ++)
        {
            pTemplate   = (PBSP_TEMPLATE_OBJECT)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);
            if (pTemplate)
                pTemplate->Remove((ANSC_HANDLE)pTemplate);
        }
        pPtrArray->Remove((ANSC_HANDLE)pPtrArray);
    }

    /* loaded templates list */
    if ( pMyObject->hLoadedTemplatesList )
    {
        pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hLoadedTemplatesList;
        ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
        for (i = 0; i < ulSize; i ++)
        {
            pName   = (char *)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);
            AnscFreeMemory(pName);
        }
        pPtrArray->Remove((ANSC_HANDLE)pPtrArray);
    }

    /* remove registered templates */
    for (i = 0; i < BSP_TEMPLATE_LIST_REG_TABLE_SIZE; i ++)
    {
        pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->RegTemplateTable[i]);

        while (pSLinkEntry)
        {
            pSLinkNext  = AnscQueueGetNextEntry(pSLinkEntry);
            pInfo       = ACCESS_BSP_TEMPLATE_REG_PAGE_INFO(pSLinkEntry);

            AnscFreeMemory(pInfo->pName);
            AnscFreeMemory(pInfo);

            pSLinkEntry = pSLinkNext;
        }
    }

    if (pReader)
        pReader->Close((ANSC_HANDLE)pReader);

    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateListCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject    = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    int                             i;

    if ( !pMyObject )
    {
        return ANSC_STATUS_FAILURE;
    }

    pMyObject->AddItem                      = BspTemplateListAddItem;
    pMyObject->UpdateItem                   = BspTemplateListUpdateItem;
    pMyObject->FindItem                     = BspTemplateListFindItem;
    pMyObject->AddGroup                     = BspTemplateListAddGroup;
    pMyObject->Store                        = BspTemplateListStore;
    pMyObject->Load                         = BspTemplateListLoad;
    pMyObject->IsTemplateLoaded             = BspTemplateListIsTemplateLoaded;
    pMyObject->SetTemplateLoaded            = BspTemplateListSetTemplateLoaded;
    pMyObject->RegisterTemplate             = BspTemplateListRegisterTemplate;
    pMyObject->GetRegisteredTemplate        = BspTemplateListGetRegisteredTemplate;
    pMyObject->SetTemplateSource            = BspTemplateListSetTemplateSource;
    pMyObject->UseArchive                   = BspTemplateListUseArchive;
    pMyObject->AddTemplate                  = BspTemplateListAddTemplate;
    pMyObject->GetTemplate                  = BspTemplateListGetTemplate;
    pMyObject->ImportTemplates              = BspTemplateListImportTemplates;
    pMyObject->ClearTemplates               = BspTemplateListClearTemplates;
    pMyObject->SetCmif                      = BspTemplateListSetCmif;
    pMyObject->GetCmif                      = BspTemplateListGetCmif;
    pMyObject->CacheTemplates               = BspTemplateListCacheTemplates;
    pMyObject->LoadCacheTemplates           = BspTemplateListLoadCacheTemplates;
    pMyObject->Remove                       = BspTemplateListCORemove;
    pMyObject->Initialize                   = BspTemplateListCOInitialize;
    pMyObject->SetBspSoaIf                  = BspTemplateListSetBspSoaIf;
    pMyObject->GetBspSoaIf                  = BspTemplateListGetBspSoaIf;

    pMyObject->hList                = AnscCreatePtrArrayComponent(NULL, NULL, NULL);
    pMyObject->hLoadedTemplatesList = AnscCreatePtrArrayComponent(NULL, NULL, NULL);

    for (i = 0; i < BSP_TEMPLATE_LIST_REG_TABLE_SIZE; i ++)
    {
        AnscQueueInitializeHeader(&pMyObject->RegTemplateTable[i]);
    }

    return ANSC_STATUS_SUCCESS;
}


