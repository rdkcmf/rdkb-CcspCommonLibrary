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

    module:	bree_srm_process.c

        For BREE Site Resource Manager (BMW 2.0) object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BREE Site Resource Manager Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        01/17/04    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bree_srm_global.h"


ANSC_STATUS
BreeSrmoRegisterCookedResource
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedRes
    )
{
    PBREE_SRM_OBJECT                pMyObject       = (PBREE_SRM_OBJECT  )hThisObject;
    PBREE_SRM_RESOURCE              pCookedRes      = (PBREE_SRM_RESOURCE)hCookedRes;
    PBREE_SRM_RES_ITEM              pRes;
    ULONG                           ulHashIndex;

    if (!pCookedRes)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    if (!pCookedRes->pResPath || !pCookedRes->pResName || !pCookedRes->pResContent)
    {
        return ANSC_STATUS_INTERNAL_ERROR;
    }

    pRes = (PBREE_SRM_RES_ITEM)AnscAllocateMemory(sizeof(BREE_SRM_RES_ITEM));

    if (!pRes)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pRes->pPath         = pCookedRes->pResPath;
    pRes->pName         = pCookedRes->pResName;
    pRes->pContent      = pCookedRes->pResContent;
    if (pCookedRes->pulContentLen)
    {
        pRes->ulContentLen  = *pCookedRes->pulContentLen;
    }
    else
    {
        pRes->ulContentLen  = 0;
    }

    ulHashIndex = AnscHashString((char *)pRes->pPath, AnscSizeOfString((const char *)pRes->pPath), BREE_SRM_RES_TABLE_SIZE);

    AnscSListPushEntry(&pMyObject->ResList[ulHashIndex], &pRes->Linkage);

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BreeSrmoUnregisterCookedResource
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCookedRes
    )
{
    PBREE_SRM_OBJECT                pMyObject       = (PBREE_SRM_OBJECT  )hThisObject;
    PBREE_SRM_RESOURCE              pCookedRes      = (PBREE_SRM_RESOURCE)hCookedRes;
    PBREE_SRM_RES_ITEM              pRes;
    ULONG                           ulHashIndex;
    PSINGLE_LINK_ENTRY              pEntry;

    if (!pCookedRes)
    {
        return ANSC_STATUS_BAD_PARAMETER;
    }

    ulHashIndex = AnscHashString((char *)pCookedRes->pResPath, AnscSizeOfString((const char *)pCookedRes->pResPath), BREE_SRM_RES_TABLE_SIZE);

    pEntry = AnscSListGetFirstEntry(&pMyObject->ResList[ulHashIndex]);

    while (pEntry)
    {
        pRes = ACCESS_BREE_SRM_RES_ITEM_OBJECT(pEntry);

        if (strcasecmp((char *)pRes->pPath, (char *)pCookedRes->pResPath) == 0 )
        {
            AnscSListPopEntryByLink(&pMyObject->ResList[ulHashIndex], &pRes->Linkage);
        }

        pEntry = AnscSListGetNextEntry(pEntry);
    }

    return ANSC_STATUS_SUCCESS;
}


ANSC_HANDLE
BreeSrmoMapCookedResource
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pUrl
    )
{
    PBREE_SRM_OBJECT                pMyObject       = (PBREE_SRM_OBJECT  )hThisObject;
    PBREE_SRM_RES_ITEM              pRes;
    ULONG                           ulHashIndex;
    PSINGLE_LINK_ENTRY              pEntry;

    if (!pUrl)
    {
        return (ANSC_HANDLE)NULL;
    }

    ulHashIndex = AnscHashString((char *)pUrl, AnscSizeOfString((const char *)pUrl), BREE_SRM_RES_TABLE_SIZE);

    pEntry = AnscSListGetFirstEntry(&pMyObject->ResList[ulHashIndex]);

    while (pEntry)
    {
        pRes = ACCESS_BREE_SRM_RES_ITEM_OBJECT(pEntry);

        if (strcasecmp((char *)pRes->pPath, (char *)pUrl) == 0 )
        {
            return (ANSC_HANDLE)pRes;
        }

        pEntry = AnscSListGetNextEntry(pEntry);
    }

    return (ANSC_HANDLE)NULL;
}


ANSC_STATUS
BreeSrmoLoadCookedResources
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREE_SRM_OBJECT                pMyObject       = (PBREE_SRM_OBJECT  )hThisObject;
    PBREE_SRM_RESOURCE              pCookedRes;
    ULONG                           i, ulCount;

    ulCount = Bmw2_SiteRes_Array_Size;

    for (i = 0; i < ulCount; i ++)
    {
        pCookedRes = &Bmw2_SiteRes_Array[i];

        pMyObject->RegisterCookedResource((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pCookedRes);
    }

    return ANSC_STATUS_SUCCESS;
}


