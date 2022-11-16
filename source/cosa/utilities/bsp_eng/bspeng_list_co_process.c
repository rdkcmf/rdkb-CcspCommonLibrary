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

    module:	bspeng_list_co_process.c

        For BSP Engine List Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BSP engine list Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bspeng_co_global.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateListAddItem
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pText
            )

    description:

        This function is called to parse given script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pText
                BSP script.

    return:     template object created.

**********************************************************************/

ANSC_HANDLE
BspTemplateListAddItem
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pText
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_OBJECT            pTemplate;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    BOOL                            bBadTemplate = FALSE;
    PUCHAR                          pTmplName;

    pTemplate = (PBSP_TEMPLATE_OBJECT)CreateBspEngComponent(NULL, NULL, NULL);

    if ( !pTemplate )
    {
        return (ANSC_HANDLE)NULL;
    }

    pTemplate->SetList((ANSC_HANDLE)pTemplate, (ANSC_HANDLE)pMyObject);

    /* using the new template to parse given text */
    pTemplate->Parse((ANSC_HANDLE)pTemplate, pText, BSP_TEMPLATE_PARSING_DEFAULT_LINE_NO);

    /* check if the name of the newly created template is "unknown" */
    pTmplName       = (PUCHAR)pTemplate->GetTmplName((ANSC_HANDLE)pTemplate);
    bBadTemplate    =
            !pTmplName          || 
            strcmp((char *)pTmplName, BSP_TEMPLATE_DEFAULT_NAME) == 0 ;

    /* or there's already a template has the name */
    if (!bBadTemplate && pMyObject->FindItem(hThisObject, (const char *)pTmplName))
        bBadTemplate = TRUE;

    if (bBadTemplate)
    {
        /* this template has some errors, so destroy the template */
        pTemplate->Remove((ANSC_HANDLE)pTemplate);
        pTemplate = NULL;
    }
    else
    {
        pPtrArray = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
        pPtrArray->Add((ANSC_HANDLE)pPtrArray, pTemplate);
    }

    return (ANSC_HANDLE)pTemplate;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateListUpdateItem
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pOrigText,
                const char                  *pText
            )

    description:

        This function is called to update script.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.
                
                const char                  *pOrigText
                Original script.

                const char                  *pText
                BSP script.

    return:     template object created.

**********************************************************************/

ANSC_HANDLE
BspTemplateListUpdateItem
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pOrigText,
        const char                  *pText
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_OBJECT            pTemplate;

    pTemplate = (PBSP_TEMPLATE_OBJECT)pMyObject->FindItem(hThisObject, pOrigText);

    if (!pTemplate)
        return NULL;

    if (pText)
        pTemplate->Parse((ANSC_HANDLE)pTemplate, pText, BSP_TEMPLATE_PARSING_DEFAULT_LINE_NO);

    return pTemplate;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateListFindItem
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pName
            )

    description:

        This function is called to find template with given name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pName
                Template name.

    return:     handle to template object.

**********************************************************************/

ANSC_HANDLE
BspTemplateListFindItem
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_OBJECT            pTemplate;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    ULONG                           ulSize, i;
    PUCHAR                          pTmplName;

    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
    ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    for (i = 0; i < ulSize; i ++)
    {
        pTemplate   = (PBSP_TEMPLATE_OBJECT)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);

        if ( pTemplate ) /*RDKB-6140, CID-24471; null check before use*/
        {
            pTmplName   = (PUCHAR)pTemplate->GetTmplName((ANSC_HANDLE)pTemplate);

            if (
                    strcmp((char *)pTmplName, (char *)pName) == 0
               )
            {
                return pTemplate;
            }

        }
    }

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateListAddGroup
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hReader,
                BOOL                        bRemoveAll,
                const char                  *pVirtualName
            )

    description:

        This function is called to load templates from given
        reader object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDEL                 hReader
                Reader object.

                BOOL                        bRemoveAll
                Whether to remove all template objects in current list.

                const char                  *pVirtualName
                The virtual BSP script name if the name is not
                specified.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateListAddGroup
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hReader,
        BOOL                        bRemoveAll,
        const char                  *pVirtualName
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_OBJECT            pTemplate;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray;
    ULONG                           ulSize, i;
    BOOL                            bOK = TRUE;
    ULONG                           ulBlockSize = BSP_TEMPLATE_TEXT_INIT_SIZE;
    ULONG                           ulBufSize = ulBlockSize;
    ULONG                           ulRead, ulLeft = ulBlockSize;
    PUCHAR                          pBuf = NULL, pCur = NULL;
    PBSP_TEMPLATE_READER_OBJECT     pReader;
    ULONG                           ulLineNo    = 1;
    ULONG                           ulErrLineNo;
    PUCHAR                          pErrMsg;
    PUCHAR                          pTmplName;
    PANSC_PTR_ARRAY_OBJECT          pLoadedTemplatesList;
    BOOL                            bLoaded         = FALSE;
    
#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Start compiling page <%s>, time = %lu (ms) ...\n", pVirtualName, AnscGetTickInMilliSeconds());
    }
#endif

    pReader = (PBSP_TEMPLATE_READER_OBJECT)hReader;
    if (!pReader)
        return FALSE;

    /* save the reader object just for loading included templates */
    pMyObject->hReader  = hReader;

    pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
    pLoadedTemplatesList    = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hLoadedTemplatesList;

    if (bRemoveAll)
    {
        char                        *pName;

        ulSize      = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

        for (i = 0; i < ulSize; i ++)
        {
            pTemplate = (PBSP_TEMPLATE_OBJECT)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i );
            pTemplate->Remove((ANSC_HANDLE)pTemplate);
        }

        pPtrArray->RemoveAll((ANSC_HANDLE)pPtrArray);

        ulSize      = pLoadedTemplatesList->GetSize((ANSC_HANDLE)pLoadedTemplatesList);

        for (i = 0; i < ulSize; i ++)
        {
            pName   = (char *)pLoadedTemplatesList->GetAt((ANSC_HANDLE)pLoadedTemplatesList, i);
            AnscFreeMemory(pName);
        }
    }

    /*
    pTemplateName   = pReader->GetTemplateName((ANSC_HANDLE)pReader);
    
    bLoaded         = pMyObject->IsTemplateLoaded((ANSC_HANDLE)pMyObject, pTemplateName);   
    */

    bLoaded         = pMyObject->IsTemplateLoaded((ANSC_HANDLE)pMyObject, (char *)pVirtualName);   
    
    if (bLoaded)
    {
        /* this template has been loaded */
        return TRUE;
    }
    else
    {
        char                        *pName;

        /*
        pName   = AnscDupString(pTemplateName);
        */
        pName   = (char *)AnscDupString((PUCHAR)pVirtualName);

        pLoadedTemplatesList->Add((ANSC_HANDLE)pLoadedTemplatesList, pName);
    }

    pBuf = (PUCHAR)AnscAllocateMemory(ulBufSize);
    if (!pBuf)
    {
        return FALSE;
    }

    pCur = pBuf;

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Reading page into memory, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

    /* read whole template script into memory */
    while ((ulRead = pReader->Read((ANSC_HANDLE)pReader, pCur, ulLeft)))
    {
        pCur    += ulRead;
        ulLeft  -= ulRead;

        if (ulLeft == 0)
        {
            ulRead  = pCur - pBuf;
/*
            pBuf    = (PUCHAR)AnscReAllocMemory(pBuf, ulBufSize += ulBlockSize);
*/
            pBuf    = 
                (PUCHAR)AnscMemUtilRealloc
                    (
                        pBuf, 
                        ulBufSize, 
                        ulBufSize + ulBlockSize
                    );
            /* RDKB-6140, CID-24219; return if reallocation failure to avoid mem curruption.
            */
            if(!pBuf)
            {
                return FALSE;
            }
            ulBufSize += ulBlockSize;

            pCur    = pBuf + ulRead;
            ulLeft  = ulBlockSize;
        }
    }

    pBuf[pCur - pBuf] = '\0';
    pCur = pBuf;

#ifdef   _DEBUG
    if ( BspEngIsCallFlowTraced() )
    {
        AnscTrace("Page reading done, time = %lu (ms) ...\n", AnscGetTickInMilliSeconds());
    }
#endif

/* the virtual file will be parsed at one time
    while (pCur && *pCur != '\0')
*/
    if (pCur && *pCur != '\0')
    {
        pTemplate = (PBSP_TEMPLATE_OBJECT)CreateBspEngComponent(NULL, NULL, NULL);
        pTemplate->SetList((ANSC_HANDLE)pTemplate, hThisObject);

        /* set virtual name */
        pTemplate->pName    = (char *)AnscDupString((PUCHAR)pVirtualName);

        if (pVirtualName)
        {
            pTemplate->bVirtual = TRUE;
        }

        pCur        = (PUCHAR)pTemplate->Parse((ANSC_HANDLE)pTemplate, (const char *)pCur, ulLineNo);
        ulErrLineNo = pTemplate->GetErrorLineNo((ANSC_HANDLE)pTemplate);
        pTmplName   = (PUCHAR)pTemplate->GetTmplName((ANSC_HANDLE)pTemplate);

        if (
               !pCur                                            || 
               !pTemplate->GetToken((ANSC_HANDLE)pTemplate)     ||
               ulErrLineNo
           )
        {
            if (ulErrLineNo)
            {
                pErrMsg     = (PUCHAR)pTemplate->GetErrMsg((ANSC_HANDLE)pTemplate);

                AnscTrace
                    (
                        "Template '%s', line %lu: %s\n",
                        pTmplName,
                        ulErrLineNo,
                        pErrMsg
                    );

                bOK = FALSE;
            }

            /* destroy current template */
            pTemplate->Remove((ANSC_HANDLE)pTemplate);

            /*
            break;
            */
            bOK = FALSE;
        }
        else
        {
            if (pMyObject->FindItem(hThisObject, (const char *)pTmplName))
            {
                AnscTrace
                    (
                        "Template '%s', after line %lu: conflict with a previous template\n",
                        pTmplName,
                        ulLineNo
                    );

                bOK = FALSE;
                pTemplate->Remove((ANSC_HANDLE)pTemplate);
                
                /*
                break;
                */
                bOK = FALSE;
            }

            if (bOK)
            {
                ulLineNo = pTemplate->GetLineNo((ANSC_HANDLE)pTemplate);
                pPtrArray->Add((ANSC_HANDLE)pPtrArray, pTemplate);
            }
        }
    }

    AnscFreeMemory(pBuf);

    ulSize = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
    if (ulSize == 0)
    {
        AnscTrace("No templates added\r\n");
        bOK = FALSE;
    }

    return bOK;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateListStore
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hArchive
            )

    description:

        This function is called to save this object into archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArchive
                Archive object.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateListStore
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
    PBSP_TEMPLATE_OBJECT            pTmpl;
    ULONG                           i, ulSize;

    if (!pPtrArray)
        return FALSE;

    ulSize  = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);

    /* first, write the amount of templates into archive */
    pArchive->WriteUlong(hArchive, ulSize);

    /* archive templates one by one */
    for (i = 0; i < ulSize; i ++ )
    {
        pTmpl   = (PBSP_TEMPLATE_OBJECT)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, i);

        pTmpl->Store((ANSC_HANDLE)pTmpl, hArchive);
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateListLoad
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hArchive
            )

    description:

        This function is called to load list object into archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArchive
                Archive object.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateListLoad
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hArchive
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pArchive  = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hArchive;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
    PBSP_TEMPLATE_OBJECT            pTmpl;
    ULONG                           i, ulSize = 0;
    BOOL                            bSucc;

    /* read the amount of templates in archive */
    bSucc = pArchive->ReadUlong(hArchive, &ulSize);

    if (!bSucc)
        return FALSE;

    /* creating templates */
    for (i = 0; i < ulSize; i ++)
    {
        pTmpl   = (PBSP_TEMPLATE_OBJECT)CreateBspEngComponent(NULL, NULL, NULL);

        bSucc   = pTmpl->Load((ANSC_HANDLE)pTmpl, hArchive);

        if (!bSucc)
        {
            pTmpl->Remove((ANSC_HANDLE)pTmpl);
            return FALSE;
        }

        pTmpl->SetList((ANSC_HANDLE)pTmpl, hThisObject);

        pPtrArray->Add((ANSC_HANDLE)pPtrArray, (ANSC_HANDLE)pTmpl);

        pTmpl->SetCmif((ANSC_HANDLE)pTmpl, pMyObject->GetCmif((ANSC_HANDLE)pMyObject));
    }

    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateListIsTemplateLoaded
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pTemplateName
            )

    description:

        This function is called to see if the given template
        has been loaded.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char                        *pTemplateName
                The name of template to be examined.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateListIsTemplateLoaded
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pTemplateName
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pLoadedTemplatesList;
    ULONG                           i, ulSize;
    char                            *pName;

    pLoadedTemplatesList    = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hLoadedTemplatesList;

    ulSize  = pLoadedTemplatesList->GetSize((ANSC_HANDLE)pLoadedTemplatesList);

    for (i = 0; i < ulSize; i ++)
    {
        pName   = (char *)pLoadedTemplatesList->GetAt((ANSC_HANDLE)pLoadedTemplatesList, i);

        if (strcasecmp(pName, pTemplateName) == 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateListSetTemplateLoaded
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pTemplateName
            )

    description:

        This function is called to remeber the given template
        has been loaded.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hArchive
                Archive object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateListSetTemplateLoaded
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pTemplateName
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pLoadedTemplatesList;
    char                            *pName;

    pLoadedTemplatesList    = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hLoadedTemplatesList;

    pName = strdup(pTemplateName);

    if (!pName)
        return ANSC_STATUS_RESOURCES;

    pLoadedTemplatesList->Add
        (
            (ANSC_HANDLE)pLoadedTemplatesList, 
            pName
        );

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateListRegisterTemplate
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pName,
                char                        *pContent,
                ULONG                       ulLen
            )

    description:

        This function is called to save this object into archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char                        *pName
                Template name.

                char                        *pContent
                BSP template content.

                ULONG                       ulLen
                The length of BSP template content.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateListRegisterTemplate
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulLen
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    ULONG                           ulHashValue;
    PBSP_TEMPLATE_REG_PAGE_INFO     pInfo;

    if (!pName || !pContent)
        return ANSC_STATUS_FAILURE;

    ulHashValue = AnscHashString(pName, AnscSizeOfString(pName), BSP_TEMPLATE_LIST_REG_TABLE_SIZE);
    pInfo       = (PBSP_TEMPLATE_REG_PAGE_INFO)AnscAllocateMemory(sizeof(BSP_TEMPLATE_REG_PAGE_INFO));

    if (pInfo)
    {
        pInfo->pName    = (char *)AnscDupString((PUCHAR)pName); /* copy template name */
        pInfo->pContent = pContent;             /* content must be static */
        pInfo->ulLen    = ulLen;

        AnscQueuePushEntry(&pMyObject->RegTemplateTable[ulHashValue], &pInfo->Linkage);
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char *
        BspTemplateListGetRegisteredTemplate
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pName,
                PULONG                      pulLen
            )

    description:

        This function is called to get registered template.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char                        *pName
                Template name.

                PULONG                      *pulLen
                The length of template content.

    return:     The content of the template.

**********************************************************************/

char *
BspTemplateListGetRegisteredTemplate
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        PULONG                      pulLen
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    ULONG                           ulHashValue;
    PSINGLE_LINK_ENTRY              pSLinkEntry;
    PBSP_TEMPLATE_REG_PAGE_INFO     pInfo;

    if (!pName)
        return NULL;

    ulHashValue = AnscHashString(pName, AnscSizeOfString(pName), BSP_TEMPLATE_LIST_REG_TABLE_SIZE);
    pSLinkEntry = AnscQueueGetFirstEntry(&pMyObject->RegTemplateTable[ulHashValue]);

    while (pSLinkEntry)
    {
        pInfo       = ACCESS_BSP_TEMPLATE_REG_PAGE_INFO(pSLinkEntry);

        if (strcasecmp(pInfo->pName, pName) == 0 )
        {
            if (pulLen)
                *pulLen = pInfo->ulLen;

            return pInfo->pContent;
        }

        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);
    }

    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateListSetTemplateSource
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bFileBase
            )

    description:

        This function is called to set the source of templates.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bFileBase
                Specifies the source of templates.

    return:     void.

**********************************************************************/

void
BspTemplateListSetTemplateSource
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bFileBase
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;

    pMyObject->bFileBasedTemplate   = bFileBase;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateListUseArchive
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bArchive
            )

    description:

        This function is called to save this object into archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bArchive
                If archived templates are used.

    return:     void.

**********************************************************************/

void
BspTemplateListUseArchive
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bArchive
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;

    pMyObject->bSourceArchive   = bArchive;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateListAddTemplate
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTemplate
            )

    description:

        This function is called to add a template.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTemplate
                Template object.

    return:     void.

**********************************************************************/

void
BspTemplateListAddTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTemplate
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
    PBSP_TEMPLATE_OBJECT            pTemplate   = (PBSP_TEMPLATE_OBJECT)hTemplate;

    pPtrArray->Add((ANSC_HANDLE)pPtrArray, hTemplate);
    pTemplate->SetList((ANSC_HANDLE)pTemplate, (ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateListGetTemplate
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to get template by index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulIndex
                Index to the template.

    return:     handle to template object.

**********************************************************************/

ANSC_HANDLE
BspTemplateListGetTemplate
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
    ULONG                           ulCount;

    ulCount = pPtrArray->GetSize((ANSC_HANDLE)pPtrArray);
    
    if (ulIndex >= ulCount)
        return (ANSC_HANDLE)NULL;
    else
        return (ANSC_HANDLE)pPtrArray->GetAt((ANSC_HANDLE)pPtrArray, ulIndex);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateListImportTemplates
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pTemplateName,
                ANSC_HANDLE                 hImportList
            )

    description:

        This function is called to import templates from the given
        list object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pTemplateName
                Template name.

                ANSC_HANDLE                 hImportList
                The list object to import templates from.

    return:     void.

**********************************************************************/

void
BspTemplateListImportTemplates
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pTemplateName,
        ANSC_HANDLE                 hImportList
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PBSP_TEMPLATE_LIST_OBJECT       pList       = (PBSP_TEMPLATE_LIST_OBJECT)hImportList;
    PANSC_PTR_ARRAY_OBJECT          pImportList = (PANSC_PTR_ARRAY_OBJECT)pList->hList;
    ULONG                           ulCount, i;
    ANSC_HANDLE                     hTemplate;

    ulCount = pImportList->GetSize((ANSC_HANDLE)pImportList);

    for (i = 0; i < ulCount; i ++)
    {
        hTemplate   = (ANSC_HANDLE)pImportList->GetAt((ANSC_HANDLE)pImportList, i);


        pMyObject->AddTemplate((ANSC_HANDLE)pMyObject, hTemplate);
    }

    pMyObject->SetTemplateLoaded(hThisObject, (char *)pTemplateName);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateListClearTemplates
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to clear all templates.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateListClearTemplates
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pPtrArray   = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;

    pPtrArray->RemoveAll((ANSC_HANDLE)pPtrArray);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateListSetCmif
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCmif
            )

    description:

        This function is called to set CM interface.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCmif
                CM interface.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateListSetCmif
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCmif
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;

    pMyObject->hCmif    = hCmif;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateListgetCmif
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to save this object into archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to CM interface.

**********************************************************************/

ANSC_HANDLE
BspTemplateListGetCmif
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;

    return pMyObject->hCmif;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateListCacheTemplates
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pTemplateName
            )

    description:

        This function is called to cache given template

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pTemplateName
                Template to be cached.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateListCacheTemplates
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pTemplateName
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    PANSC_PTR_ARRAY_OBJECT          pList       = (PANSC_PTR_ARRAY_OBJECT)pMyObject->hList;
    PBSP_TEMPLATE_CACHE_MGR_INTERFACE pCmif     = (PBSP_TEMPLATE_CACHE_MGR_INTERFACE)pMyObject->hCmif;
    PBSP_TEMPLATE_CACHE_ENTRY       pEntry      = NULL;
    PBSP_TEMPLATE_OBJECT            *pTemplates = NULL;
    ULONG                           ulCount, i;
    ANSC_HANDLE                     hTemplate;
    PBSP_TEMPLATE_OBJECT            pTemplate;

    if (!pCmif)
        return ANSC_STATUS_SUCCESS;

    pEntry  = (PBSP_TEMPLATE_CACHE_ENTRY)AnscAllocateMemory(sizeof(BSP_TEMPLATE_CACHE_ENTRY));
    if (!pEntry)
        return ANSC_STATUS_RESOURCES;

    /* CID 59237 fix */
    pEntry->pName = (PUCHAR)strdup((char *)pTemplateName);

    if (pEntry->pName == NULL)
    {
	AnscFreeMemory(pEntry);
        return ANSC_STATUS_RESOURCES;
    }

    ulCount = pList->GetSize((ANSC_HANDLE)pList);
    pTemplates  = (PBSP_TEMPLATE_OBJECT *)AnscAllocateMemory(sizeof(PBSP_TEMPLATE_OBJECT) * ulCount);

    if (!pTemplates)
    {
        AnscFreeMemory(pEntry);

        return ANSC_STATUS_RESOURCES;
    }

    pEntry->ulCount     = ulCount;
    pEntry->hTemplates  = (ANSC_HANDLE)pTemplates;

    for (i = 0; i < ulCount; i ++)
    {
        hTemplate   = (ANSC_HANDLE)pList->GetAt((ANSC_HANDLE)pList, i);
        pTemplate   = (PBSP_TEMPLATE_OBJECT)hTemplate;

        if (pTemplates)
        {
            pTemplates[i]   = pTemplate;
        }
    }

    pCmif->AddTemplate(pCmif->hOwnerContext, (ANSC_HANDLE)pEntry);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateListLoadCacheTemplates
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hCacheEntry
            )

    description:

        This function is called to save this object into archive.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hCacheEntry
                Cache entry.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateListLoadCacheTemplates
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hCacheEntry
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;
    ULONG                           ulCount, i;
    PBSP_TEMPLATE_OBJECT            pTemplate, *pTemplates;
    PBSP_TEMPLATE_CACHE_ENTRY       pCacheEntry = (PBSP_TEMPLATE_CACHE_ENTRY)hCacheEntry;

    ulCount = pCacheEntry->ulCount;
    pTemplates  = (PBSP_TEMPLATE_OBJECT *)pCacheEntry->hTemplates;

    for (i = 0; i < ulCount; i ++)
    {
        pTemplate   = (PBSP_TEMPLATE_OBJECT)pTemplates[i];

        pMyObject->AddTemplate((ANSC_HANDLE)pMyObject, (ANSC_HANDLE)pTemplate);
    }

    pMyObject->SetTemplateLoaded(hThisObject, (char *)pCacheEntry->pName);

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateListSetBspSoaIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            )

    description:

        This function is called to set BEEP PEC interface.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                BSP SOA interface.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateListSetBspSoaIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;

    pMyObject->hBspSoaIf   = hInterface;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateListGetBspSoaIf
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get BSP SOA interface.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle to BSP SOA interface.

**********************************************************************/

ANSC_HANDLE
BspTemplateListGetBspSoaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_LIST_OBJECT       pMyObject   = (PBSP_TEMPLATE_LIST_OBJECT)hThisObject;

    return pMyObject->hBspSoaIf;
}

