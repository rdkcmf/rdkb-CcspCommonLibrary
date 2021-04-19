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

    module: dslh_dmagnt_control.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced control functions
        of the Dslh DataModelAgent object;

        *   DslhDmagntLoadDataModelXML
        *   DslhDmagntLoadDataModelXML2
        *   DslhDmagntUnloadDataModelXML
        *   DslhDmagntMemoryUsage
        *   DslhDmagntMemoryTable
        *   DslhDmagntRegisterInternalApi
        *   DslhDmagntGetFcContext
        *   DslhDmagntSetFcContext
        *   DslhDmagntRegBaseDataModel
        *   DslhDmagntGenerateDTXmlString

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        11/08/2010    initial revision.

**********************************************************************/

#include "dslh_dmagnt_global.h"
#include "dslh_vareo_interface.h"

extern  PDSLH_DATAMODEL_AGENT_OBJECT         g_DslhDataModelAgent;

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntLoadDataModelXML
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pXMLContent,
                ULONG                       uXMLLength,
                BOOL                        bExternal,
                BOOL                        bPopulateTree
            );

    description:

        This function is called to load data model from xml file.

    argument:   
                ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                char*                       pXMLContent,
                The content string of the xml file;

                ULONG                       uXMLLength
                The length of the xml file

                BOOL                        bExternal,
                Whether it's internal or external.
                If internal, we will take care of the "RootObjName" node.

                BOOL                        bPopulateTree
                Whether populate tree is required or not;

    return:     status of operation.

**********************************************************************/
ANSC_HANDLE
COSAAcquireFunction
    (
        char*                       pApiName
    )
{
    PANSC_ATOM_TABLE_OBJECT         pAtomFunctions    = g_DslhDataModelAgent->hAtomFunctions;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor   = NULL;

    if( pAtomFunctions == NULL)
    {
        return NULL;
    }

    pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR)pAtomFunctions->GetAtomByName(pAtomFunctions, pApiName);

    if( pAtomDescriptor == NULL)
    {
        return NULL;
    }

    return (ANSC_HANDLE)pAtomDescriptor->hContext;
}

BOOL
COSARegistFunction
    (
        void*                       hContext,    /* this will be the handle passed in by COSA_PLUGIN_INFO */
        char*                       pApiName,
        void*                       hModuleProc                  
    )
{
    PANSC_ATOM_TABLE_OBJECT         pAtomFunctions    = (PANSC_ATOM_TABLE_OBJECT)hContext;
    ANSC_HANDLE                     hAtomItem         = NULL;

    if( pAtomFunctions == NULL)
    {
        return FALSE;
    }

    hAtomItem = pAtomFunctions->AddAtomByName(pAtomFunctions, pApiName, 0, hModuleProc);

    if( hAtomItem != NULL)
    {
        return TRUE;
    }

    return FALSE;
}

ANSC_STATUS
DslhDmagntLoadDataModelXML
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pXMLContent,
        ULONG                       uXMLLength,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    )
{
    PCHAR pBackBuffer = pXMLContent;

    return DslhDmagntLoadDataModelXMLInternal(
      hThisObject,
      AnscXmlDomParseString((ANSC_HANDLE)NULL, (PCHAR*)&pBackBuffer, uXMLLength),
      bExternal,
      bPopulateTree
    );
}

ANSC_STATUS
DslhDmagntLoadDataModelXML2
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pfnXMLLoader,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    )
{
    typedef PVOID (*PFN_ANSC_XML_CODE_GEN_LOAD)();
    return DslhDmagntLoadDataModelXMLInternal(
      hThisObject,
      (*((PFN_ANSC_XML_CODE_GEN_LOAD)pfnXMLLoader))(),
      bExternal,
      bPopulateTree
    );
}

ANSC_STATUS
DslhDmagntLoadDataModelXMLInternal
    (
        ANSC_HANDLE                 hThisObject,
        void*                       pXMLNode,
        BOOL                        bExternal,
        BOOL                        bPopulateTree
    )
{
    UNREFERENCED_PARAMETER(bExternal);
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE        )pCpeController->GetDslhMprIf(pCpeController);
    PANSC_XML_DOM_NODE_OBJECT       pRootNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pListNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PPLUGIN_INFORMATION             pPluginInfo       = (PPLUGIN_INFORMATION)NULL;
    PDSLH_LCB_INTERFACE             pDslhLcbIf        = (PDSLH_LCB_INTERFACE)pMyObject->hDslhLcbIf;
    ANSC_STATUS                     returnStatus      = 0;
    char*                           pRootName         = NULL;
    
    pRootNode = pXMLNode;
    if( pRootNode == NULL)
    {
        AnscTraceWarning(("Failed to parse the data model XML file.\n"));

        returnStatus = ANSC_STATUS_FAILURE;

        goto EXIT;
    }
#if 0
    /*mrollins: debug code to write out the xml.  used to compare xml create from dm pack to original*/
    {
      ULONG size = 0;
      ULONG sizeleft = 0;
      char* buffer = NULL;
      FILE* file;

      size = sizeleft = pRootNode->GetEncodedSize(pRootNode);
      buffer = malloc(size);
      if(pRootNode->Encode(pRootNode, buffer, &sizeleft) == ANSC_STATUS_SUCCESS)
      {
        file = fopen("/tmp/dmdbg.xml", "w");
        fwrite(buffer, 1, size, file);
        fclose(file);
        printf("Encode success size=%lu sizeleft=%lu\n", size, sizeleft);fflush(stdout);
      }
      else
      {
        printf("Encode failed size=%lu sizeleft=%lu\n", size, sizeleft);fflush(stdout);
      }
    }
#endif
    /* get RootObjName */
    pRootName = pDslhMprIf->GetRootObjName(pDslhMprIf->hOwnerContext);


    /* load the library */
    pPluginInfo = (PPLUGIN_INFORMATION)DslhDmagntCreatePluginInfo(hThisObject);

    if( pPluginInfo == NULL)
    {
        returnStatus = ANSC_STATUS_RESOURCES;

        goto EXIT;
    }

    /* add it to the list */
    AnscQueuePushEntry(&pMyObject->sPluginInfoList, &pPluginInfo->Linkage);
    pMyObject->bPluginUpdated  = TRUE;

    returnStatus = 
        DslhDmagntParseSourceInfo(hThisObject, (ANSC_HANDLE)pRootNode, (ANSC_HANDLE)pPluginInfo);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        /* if the DLL instance is loaded. release it */
        if( pPluginInfo->hInstance != NULL)
        {
            AnscFreeLibrary(pPluginInfo->hInstance);

            pPluginInfo->hInstance = NULL;
        }

        goto EXIT;
    }

    if( pPluginInfo->SourceName != NULL)
    {
        /* external dll */
        pPluginInfo->hAtomFunc = 
            AnscCreateAtomTable
                (
                    pMyObject->hContainerContext,
                    (ANSC_HANDLE)pMyObject,
                    NULL
                );
        
        if( pPluginInfo->hAtomFunc == NULL)
        {
            pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_RESOURCES;

            returnStatus             = ANSC_STATUS_RESOURCES;

            goto EXIT;
        }

        pPluginInfo->cosaPlugin.AcquireFunction      = COSAAcquireFunction;
        pPluginInfo->cosaPlugin.RegisterFunction     = COSARegistFunction;
        pPluginInfo->cosaPlugin.hContext             = pPluginInfo->hAtomFunc;
        pPluginInfo->cosaPlugin.hFcContext           = pMyObject->hFcContext;
        pPluginInfo->cosaPlugin.hDmlAgent            = pMyObject;

        /* excute "init" proc of the DLL */
        if( pPluginInfo->InitProc( COSA_CURRENT_SUPPORT_VERSION, &pPluginInfo->cosaPlugin) != 0)
        {
            AnscTraceWarning(("Failed in 'init' proc of the dll.\n"));

            pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_INIT;

            returnStatus             = ANSC_STATUS_FAILURE;

            goto EXIT;
        }
    }
    else
    {
        /* internal module */
        pPluginInfo->cosaPlugin.AcquireFunction      = COSAAcquireFunction;
        pPluginInfo->cosaPlugin.RegisterFunction     = COSARegistFunction;
        pPluginInfo->hAtomFunc                       = pMyObject->hAtomFunctions;
        pPluginInfo->cosaPlugin.hContext             = pPluginInfo->hAtomFunc;
        pPluginInfo->cosaPlugin.hDmlAgent            = pMyObject;

        if( pDslhLcbIf != NULL && pDslhLcbIf->InitLibrary != NULL)
        {
            pDslhLcbIf->InitLibrary( COSA_CURRENT_SUPPORT_VERSION, &pPluginInfo->cosaPlugin);

            /* make sure that the "InitLibrary" only got called once */
            pDslhLcbIf->InitLibrary = NULL;
        }

    }

    /* check whether all the api defined in XML file are resolved or not */
    if( !DslhDmagntCheckApiResolved(hThisObject, pRootNode, (ANSC_HANDLE)pPluginInfo))
    {
        pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_UNRESOLVED_API;
        AnscTraceWarning(("resolving APIs in xml file failed!\n" ));
        returnStatus             = ANSC_STATUS_FAILURE;

        goto EXIT;
    }

    /* check whether the base object name is registered or not */
    if( !DslhDmagntCheckBaseObjNames(hThisObject, pRootName,pRootNode))
    {
        pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_UNKNOWN_OBJ_NAME;

        returnStatus             = ANSC_STATUS_FAILURE;

        goto EXIT;
    }

    /* load the data model */
    pListNode = (PANSC_XML_DOM_NODE_OBJECT)
        pRootNode->GetChildByName(pRootNode, COSA_XML_NODE_objects);

    if( pListNode != NULL)
    {
        pChildNode = pListNode->GetHeadChild(pListNode);

        while( pChildNode != NULL)
        {
            returnStatus = 
                DslhDmagntRegisterDataModelObject(hThisObject, pChildNode, pRootName, pPluginInfo, bPopulateTree);

            if( returnStatus != ANSC_STATUS_SUCCESS)
            {
                pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_REG_FAILURE;

                goto EXIT;
            }

            /* goto next one */
            pChildNode = pListNode->GetNextChild(pListNode, pChildNode);
        }
    }

EXIT:

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscTraceWarning(("DslhDmagntLoadDataModelXML -- failed, error = %lu!\n", returnStatus));
    }
    
    if( pRootNode != NULL)
    {
        pRootNode->Remove(pRootNode);
    }

    return returnStatus;

}   


/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntUnloadDataModelXML
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pXMLContent,
                ULONG                       uXMLLength
            );

    description:

        This function is called to unload data model from xml file.

    argument:   
                ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                char*                       pXMLContent,
                The content string of the xml file;

                ULONG                       uXMLLength
                The length of the xml file

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhDmagntUnloadDataModelXML
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pXMLContent,
        ULONG                       uXMLLength
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE        )pCpeController->GetDslhMprIf(pCpeController);
    PANSC_XML_DOM_NODE_OBJECT       pRootNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pListNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PPLUGIN_INFORMATION             pInfo             = NULL;
    PCHAR                           pBackBuffer       = pXMLContent;
    ANSC_STATUS                     returnStatus      = 0;
    char*                           pRootName         = NULL;
    char                            buffer[1024]      = { 0 };
    
    pRootNode = (PANSC_XML_DOM_NODE_OBJECT)
        AnscXmlDomParseString((ANSC_HANDLE)NULL, (PCHAR*)&pBackBuffer, uXMLLength);

    if( pRootNode == NULL)
    {
        AnscTraceWarning(("Failed to parse the data model XML file.\n"));

        returnStatus = ANSC_STATUS_FAILURE;

        goto EXIT;
    }
   
    /* get RootObjName */
    pRootName = pDslhMprIf->GetRootObjName(pDslhMprIf->hOwnerContext);

    /* unload the data model */
    pListNode = (PANSC_XML_DOM_NODE_OBJECT)
        pRootNode->GetChildByName(pRootNode, COSA_XML_NODE_objects);

    if( pListNode != NULL)
    {
        pChildNode = pListNode->GetHeadChild(pListNode);

        while( pChildNode != NULL)
        {
            returnStatus = 
                DslhDmagntUnloadDataModelObject(hThisObject, pChildNode, pRootName);

            /* goto next one */
            pChildNode = pListNode->GetNextChild(pListNode, pChildNode);
        }
    }

    /* remove the data model library entry if exists */
    if( ANSC_STATUS_SUCCESS == DslhDmagntParseSourceName(hThisObject, pRootNode, buffer))
    {
        pSListEntry =   AnscQueueGetFirstEntry(&pMyObject->sPluginInfoList);

        while( pSListEntry != NULL)
        {
            pInfo       = ACCESS_PLUGIN_INFORMATION(pSListEntry);
            pSListEntry = AnscQueueGetNextEntry(pSListEntry);

            if( pInfo != NULL && pInfo->SourceName != NULL &&
                strcmp(pInfo->SourceName, buffer) == 0 )
            {                
                AnscQueuePopEntryByLink(&pMyObject->sPluginInfoList, &pInfo->Linkage);

                DslhDmagntRemovePluginInfo(pMyObject, pInfo);

                pMyObject->bPluginUpdated   = TRUE;

                break;
            }              
        }
    }

EXIT:

    if( pRootNode != NULL)
    {
        pRootNode->Remove(pRootNode);
    }

    return returnStatus;

}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntMemoryUsage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unload data model from xml file.

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhDmagntMemoryUsage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PPLUGIN_INFORMATION             pInfo             = NULL;
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    
    pSListEntry =   AnscQueueGetFirstEntry(&pMyObject->sPluginInfoList);

    while( pSListEntry != NULL)
    {
        pInfo       = ACCESS_PLUGIN_INFORMATION(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry(pSListEntry);

        if( pInfo != NULL && pInfo->SourceName != NULL )
        {
            if ( pInfo->MemoryUsageProc != NULL )
            {
                AnscTrace("\n --- The memory usage in %s ---\n", pInfo->SourceName);
                pInfo->MemoryUsageProc();
            }
            else
            {
                AnscTrace("\n --- No memory usage supported in %s ---\n", pInfo->SourceName);
            }
        }          
    }

    return returnStatus;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntMemoryTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unload data model from xml file.

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhDmagntMemoryTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PPLUGIN_INFORMATION             pInfo             = NULL;
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    
    pSListEntry =   AnscQueueGetFirstEntry(&pMyObject->sPluginInfoList);

    while( pSListEntry != NULL)
    {
        pInfo       = ACCESS_PLUGIN_INFORMATION(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry(pSListEntry);

        if( pInfo != NULL && pInfo->SourceName != NULL )
        {
            if ( pInfo->MemoryTableProc != NULL )
            {
                AnscTrace("\n --- The memory table in %s ---\n", pInfo->SourceName);
                pInfo->MemoryTableProc();
            }
            else
            {
                AnscTrace("\n --- No memory table supported in %s ---\n", pInfo->SourceName);
            }
        }          
    }

    return returnStatus;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntMemoryCheck
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to unload data model from xml file.

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhDmagntMemoryCheck
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PSINGLE_LINK_ENTRY              pSListEntry       = NULL;
    PPLUGIN_INFORMATION             pInfo             = NULL;
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    
    pSListEntry =   AnscQueueGetFirstEntry(&pMyObject->sPluginInfoList);

    while( pSListEntry != NULL)
    {
        pInfo       = ACCESS_PLUGIN_INFORMATION(pSListEntry);
        pSListEntry = AnscQueueGetNextEntry(pSListEntry);

        if( pInfo != NULL && pInfo->SourceName != NULL )
        {
            if ( pInfo->MemoryCheckProc != NULL)
            {
                AnscTrace("\n --- The memory leak check in %s ---\n", pInfo->SourceName);
                pInfo->MemoryCheckProc();
            }
            else
            {
                AnscTrace("\n --- No memory check supported in %s ---\n", pInfo->SourceName);
            }
        }          
    }

    return returnStatus;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntRegisterInternalApi
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pApiName,
                ANSC_HANDLE                 hRegApi
            );

    description:

        This function is called to register internal apis for data model.

    argument:   
                ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                char*                       pApiName,
                The name of the api;

                ANSC_HANDLE                 hRegApi
                The handle of the function;

    return:     status of operation.

**********************************************************************/
ANSC_STATUS
DslhDmagntRegisterInternalApi
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pApiName,
        ANSC_HANDLE                 hRegApi
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtomFunctions    = (PANSC_ATOM_TABLE_OBJECT)pMyObject->hAtomFunctions;
    ANSC_HANDLE                     hAtomItem         = NULL;

    hAtomItem = pAtomFunctions->AddAtomByName(pAtomFunctions, pApiName, 0, hRegApi);

    if( hAtomItem != NULL)
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;

}


/**********************************************************************

    prototype:

        ANSC_HANDLE
        DslhDmagntGetFcContext
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to register internal apis for data model.

    argument:   
                ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                char*                       pApiName,
                The name of the api;

                ANSC_HANDLE                 hRegApi
                The handle of the function;

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
DslhDmagntGetFcContext
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;

    return pMyObject->hFcContext;
}


/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntSetFcContext
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to register internal apis for data model.

    argument:   
                ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                char*                       pApiName,
                The name of the api;

                ANSC_HANDLE                 hRegApi
                The handle of the function;

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhDmagntSetFcContext
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;

    pMyObject->hFcContext = hContext;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntRegBaseDataModel
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pCompName
            );

    description:

        This function is called to register CCSP base data model.

    argument:   
                ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                char*                       pCompName
                The CCSP component name;

    return:     status of operation.

**********************************************************************/
static const char* base_datamodel_xml =
"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n "
"<dataModelInfo>\n "
"  <version>1</version>\n "
"  <moduleName>CCSP Base Data Model</moduleName>\n "
"  <author></author>\n "
"  <description>CCSP Base Data Model</description>\n "
"  <library/>\n "
"  <objects>\n "
"  <object>\n "
"      <baseObjName>com.cisco.spvtg.ccsp.</baseObjName>\n "
"      <name>%s</name>\n "
"      <objectType>object</objectType>\n "
"      <functions>\n "
"        <func_GetBulkParamValues>CcspComp_GetBulkParamValues</func_GetBulkParamValues>\n "
"        <func_SetParamBoolValue>CcspComp_SetParamBoolValue</func_SetParamBoolValue>\n "
"      </functions>\n "
"      <parameters>\n "
"        <parameter>\n "
"          <name>Name</name>\n "
"          <type>string</type>\n "
"          <syntax>string</syntax>\n "
"        </parameter>\n "
"        <parameter>\n "
"          <name>Version</name>\n "
"          <type>unsignedInt</type>\n "
"          <syntax>uint32</syntax>\n "
"        </parameter>\n "
"        <parameter>\n "
"          <name>Author</name>\n "
"          <type>string(32)</type>\n "
"          <syntax>string</syntax>\n "
"        </parameter>\n "
"        <parameter>\n "
"          <name>Health</name>\n "
"          <type>string: Red(1), Yellow(2), Green(3)</type>\n "
"          <syntax>uint32/mapped</syntax>\n "
"        </parameter>\n "
"        <parameter>\n "
"          <name>State</name>\n "
"          <type>unsignedInt</type>\n "
"          <syntax>uint32</syntax>\n "
"        </parameter>\n "
"        <parameter>\n "
"          <name>DTXml</name>\n "
"          <type>string</type>\n "
"          <syntax>string</syntax>\n "
"        </parameter>\n "
"        <parameter>\n "
"          <name>Register</name>\n "
"          <type>boolean</type>\n "
"          <syntax>bool</syntax>\n "
"          <writable>true</writable>\n "
"        </parameter>\n "
"      </parameters>\n "
"      <objects>\n "
"        <object>\n "
"          <name>Logging</name>\n "
"          <objectType>object</objectType>\n "
"          <functions>\n "
"            <func_GetBulkParamValues>CcspLog_GetBulkParamValues</func_GetBulkParamValues>\n "
"            <func_SetBulkParamValues>CcspLog_SetBulkParamValues</func_SetBulkParamValues>\n "
"            <func_Validate>CcspLog_Validate</func_Validate>\n "
"            <func_Commit>CcspLog_Commit</func_Commit>\n "
"            <func_Rollback>CcspLog_Rollback</func_Rollback>\n "
"          </functions>\n "
"          <parameters>\n "
"          <parameter>\n "
"            <name>Enable</name>\n "
"            <type>boolean</type>\n "
"            <syntax>bool</syntax>\n "
"            <writable>true</writable>\n "
"          </parameter>\n "
"          <parameter>\n "
"            <name>LogLevel</name>\n "
"            <type>unsignedInt[1:7]</type>\n "
"            <syntax>uint32</syntax>\n "
"            <writable>true</writable>\n "
"          </parameter>\n "
"          </parameters>\n "
"        </object>\n "
"       <object>\n "
"          <name>Memory</name>\n "
"          <objectType>object</objectType>\n "
"          <functions>\n "
"            <func_GetParamUlongValue>CcspMem_GetParamUlongValue</func_GetParamUlongValue>\n "
"          </functions>\n "
"          <parameters>\n "
"          <parameter>\n "
"            <name>MinUsage</name>\n "
"            <type>unsignedInt</type>\n "
"            <syntax>uint32</syntax>\n "
"          </parameter>\n "
"          <parameter>\n "
"            <name>MaxUsage</name>\n "
"            <type>unsignedInt</type>\n "
"            <syntax>uint32</syntax>\n "
"          </parameter>\n "
"          <parameter>\n "
"            <name>Consumed</name>\n "
"            <type>unsignedInt</type>\n "
"            <syntax>uint32</syntax>\n "
"          </parameter>\n "
"          </parameters>\n "
"        </object>\n "
"      </objects>\n "
"    </object>\n "
"  </objects>\n "
"</dataModelInfo>\n";



ANSC_STATUS
DslhDmagntRegBaseDataModel
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pCompName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    char                            pBuf[5012]   = { 0 };
    char*                           pLastName    = NULL;
    char                            pPrefix[]    = "com.cisco.spvtg.ccsp.";

    if( pCompName == NULL || AnscSizeOfString(pCompName) == 0)
    {
        return ANSC_STATUS_FAILURE;
    }

    pLastName = _ansc_strstr(pCompName, pPrefix);

    if( pLastName == NULL)
    {
        AnscTrace("Not valid component name: %s\n Failed to register base data model.", pCompName);
    }

	pLastName += AnscSizeOfString(pPrefix);
	
    _ansc_sprintf(pBuf, base_datamodel_xml, pLastName);
    
    /* AnscTrace("Print out the XML file:\n%s\n\n", pBuf); */

    returnStatus = 
        DslhDmagntLoadDataModelXML(hThisObject, pBuf, AnscSizeOfString(pBuf), FALSE, FALSE);

    if( returnStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTrace("Failed to load base data model for component '%s'\n", pCompName);
    }

    return returnStatus;

}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntGenerateDTXmlString
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to generate DT data model XML string;

    argument:   
                ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/
static void
_enum_obj_entities_DT
    (
        ANSC_HANDLE                 hNode,
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pRootNode           = (PANSC_XML_DOM_NODE_OBJECT)hNode;
    PANSC_XML_DOM_NODE_OBJECT       pParentNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode          = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pListNode           = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pEntryNode          = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity          = (PDSLH_OBJ_ENTITY_OBJECT  )hThisObject;
    PDSLH_OBJ_ENTITY_OBJECT         pChildObjEntity     = (PDSLH_OBJ_ENTITY_OBJECT  )NULL;
    PDSLH_VAR_ENTITY_OBJECT         pChildVarEntity     = (PDSLH_VAR_ENTITY_OBJECT  )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry         = (PSINGLE_LINK_ENTRY       )NULL;
    PDSLH_CWMP_PARAM_DESCR          pDescr              = (PDSLH_CWMP_PARAM_DESCR   )NULL;

    if( !pObjEntity->ObjDescr)
    {
        return;
    }

    if ( pObjEntity->ObjDescr && (pObjEntity->ObjDescr->Type == DSLH_CWMP_OBJECT_TYPE_table) )
    {
        pChildObjEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->GetObjEntity3((ANSC_HANDLE)pObjEntity);

        /* enumerate the entries */
        _enum_obj_entities_DT(hNode, pChildObjEntity);

        return;
    }


    /* add node itself */
    pParentNode = (PANSC_XML_DOM_NODE_OBJECT)pRootNode->AddChildByName(pRootNode,"object");

    /* add "ref" attribute */
    pParentNode->SetAttrString(pParentNode, "ref", 
        pObjEntity->ObjDescr->Name, AnscSizeOfString(pObjEntity->ObjDescr->Name));

    /* add "access" attribute */
    if( pObjEntity->ObjDescr->bWritable)
    {
        pParentNode->SetAttrString(pParentNode, "access", "createDelete", AnscSizeOfString("createDelete"));
    }
    else
    {
        pParentNode->SetAttrString(pParentNode, "access", "readOnly", AnscSizeOfString("readOnly"));
    }

    /* add "minEntries" and "maxEntries" attributes */
    if( pObjEntity->ObjDescr->MaxInstanceNumber > 1)
    {
        pParentNode->SetAttrUlong(pParentNode, "minEntries", 0);
    }
    else
    {
        pParentNode->SetAttrUlong(pParentNode, "minEntries", 1);
    }

    pParentNode->SetAttrUlong(pParentNode, "maxEntries", pObjEntity->ObjDescr->MaxInstanceNumber);

    pListNode    = pParentNode;

    /* pParamDescr may not be reliable, we choose another way to enumerate the parameters */
    /* pParamDescr  = (PDSLH_CWMP_PARAM_DESCR)pObjEntity->ObjDescr->hParamArray; */

    pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->VareoTable);
    while ( pSLinkEntry )
    {
        pChildVarEntity = ACCESS_DSLH_VAR_ENTITY_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        pDescr = pChildVarEntity->ParamDescr;

        if( (pDescr == NULL || pDescr->Name == NULL) || (AnscSizeOfString(pDescr->Name)) == 0)
        {
            break;
        }

        if ( pDescr->bInvisible)  /* no invisible parameters */
        {
            continue;
        }

        pEntryNode = (PANSC_XML_DOM_NODE_OBJECT)pListNode->AddChildByName(pListNode,"parameter");

        /* add "ref" attribute */
        pEntryNode->SetAttrString(pEntryNode, "ref", 
            pDescr->Name, AnscSizeOfString(pDescr->Name));

        /* add "access" attribute */
        if( pDescr->bWritable)
        {
            pEntryNode->SetAttrString(pEntryNode, "access", 
                "readWrite",AnscSizeOfString("readWrite"));
        }
        else
        {
            pEntryNode->SetAttrString(pEntryNode, "access", 
                "readOnly",AnscSizeOfString("readOnly"));
        }

        /* add "activeNotify" attribute */
        /*
        pEntryNode->SetAttrString(pEntryNode, "activeNotify", 
            "normal", AnscSizeOfString("normal"));
        */

        /* add "syntax" node */
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)pEntryNode->AddChildByName(pEntryNode,"syntax");

        if( _ansc_strstr(pDescr->DataType, "string") == pDescr->DataType)
        {
            pChildNode->SetDataString(pChildNode, NULL, "string",AnscSizeOfString("string"));
        }
        else if( _ansc_strstr(pDescr->DataType, "unsignedInt") == pDescr->DataType)
        {
            pChildNode->SetDataString(pChildNode, NULL, "unsignedInt",AnscSizeOfString("unsignedInt"));
        }
        else if( _ansc_strstr(pDescr->DataType, "int") == pDescr->DataType)
        {
            pChildNode->SetDataString(pChildNode, NULL, "int",AnscSizeOfString("int"));
        }
        else if( _ansc_strstr(pDescr->DataType, "base64") == pDescr->DataType)
        {
            pChildNode->SetDataString(pChildNode, NULL, "base64",AnscSizeOfString("base64"));
        } 
        else
        {
            pChildNode->SetDataString(pChildNode, NULL, pDescr->DataType,AnscSizeOfString(pDescr->DataType));
        }
    }

    /* enumerate the child object */
    pSLinkEntry = AnscQueueGetFirstEntry(&pObjEntity->ObjeoQueue);

    while ( pSLinkEntry )
    {
        pChildObjEntity  = ACCESS_DSLH_OBJ_ENTITY_OBJECT(pSLinkEntry);
        pSLinkEntry     = AnscQueueGetNextEntry(pSLinkEntry);

        _enum_obj_entities_DT(hNode,(ANSC_HANDLE)pChildObjEntity);
    }
}

ANSC_STATUS
DslhDmagntGenerateDTXmlString
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject          = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode          = (PANSC_XML_DOM_NODE_OBJECT  )NULL;
    PDSLH_MPR_INTERFACE             pDslhMprIf         = (PDSLH_MPR_INTERFACE        )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord         = NULL;
    ULONG                           length             = 0;
    char*                           pXmlString         = NULL;
    ANSC_STATUS                     uStatus            = 0;

    pDslhMprIf = (PDSLH_MPR_INTERFACE)
        pDslhCpeController->GetDslhMprIf(pDslhCpeController);

    pObjRecord = (PDSLH_OBJ_RECORD_OBJECT)
        pDslhMprIf->GetObjRecord
            (
                (ANSC_HANDLE)pDslhMprIf->hOwnerContext,
                DM_ROOTNAME
            );

    if( !pObjRecord || pObjRecord->GetObjRecordCount(pObjRecord) == 0)
    {
        pMyObject->pDTXml = NULL;

        return uStatus;        
    }

    /* create the root node */
    pRootNode = (PANSC_XML_DOM_NODE_OBJECT)AnscCreateXmlDomNode(NULL);

    if( pRootNode)
    {
        pRootNode->bIgnoreXMLHeader = FALSE;
        pRootNode->SetName
            (
                pRootNode,
                "DT"
            );


        _enum_obj_entities_DT(pRootNode,pObjRecord->hDslhObjEntity);
       

        length = AnscXmlDomNodeGetEncodedSize(pRootNode);

        if( length != 0)
        {
            pXmlString = (PCHAR)AnscAllocateMemory(length + 16);

            if( pXmlString != NULL)
            {
               uStatus = AnscXmlDomNodeEncode(pRootNode, (PVOID)pXmlString, &length);
            }
        }

        AnscXmlDomNodeRemove(pRootNode);
    }

    pMyObject->pDTXml = pXmlString;

    return uStatus;
}
