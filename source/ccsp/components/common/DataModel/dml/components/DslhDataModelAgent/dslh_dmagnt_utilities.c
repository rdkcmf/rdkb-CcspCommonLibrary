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

    module: dslh_dmagnt_utilities.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh DataModelAgent object;

        *   DslhDmagntInitManagerServerInfo
        *   DslhDmagntCreatePluginInfo
        *   DslhDmagntRemovePluginInfo
        *   DslhDmagntParseSourceInfo
        *   DslhDmagntParseSourceName
        *   DslhDmagntRegisterDataModelObject
        *   DslhDmagntCheckApiResolved
        *   DslhDmagntCheckBaseObjNames
        *   DslhDmagntParseObjectInfo
        *   DslhDmagntParseObjFuncInfo
        *   DslhDmagntParseRootFuncInfo
        *   DslhDmagntParseParamArray
        *   DslhDmagntParseParamInfo
        *   DslhDmagntUnloadDataModelObject
        *   GetParamUlongValue_X_CISCO_COM_COSADataModel
        *   GetEntryCount_COSADataModel
        *   GetEntry_COSADataModel
        *   IsUpdated_COSADataModel
        *   Synchronize_COSADataModel
        *   GetParamUlongValue_COSADataModel
        *   GetParamStringValue_COSADataModel
        *   ManagementServer_GetParamBoolValue
        *   ManagementServer_GetParamIntValue
        *   ManagementServer_GetParamUlongValue
        *   ManagementServer_GetParamStringValue
        *   ManagementServer_SetParamBoolValue
        *   ManagementServer_SetParamIntValue
        *   ManagementServer_SetParamUlongValue
        *   ManagementServer_SetParamStringValue
        *   ManagementServer_Validate
        *   ManagementServer_Commit
        *   ManagementServer_Rollback
        *   SupportedDataModel_GetEntryCount
        *   SupportedDataModel_GetEntry
        *   SupportedDataModel_GetParamStringValue
        *   CcspComp_GetParamUlongValue 
        *   CcspComp_GetParamStringValue 
        *   CcspComp_GetBulkParamValues
        *   CcspComp_SetParamBoolValue
        *   CcspLog_GetParamBoolValue
        *   CcspLog_GetParamUlongValue
        *   CcspLog_SetParamBoolValue
        *   CcspLog_SetParamUlongValue
        *   CcspLog_Validate
        *   CcspLog_Commit
        *   CcspLog_Rollback
        *   CcspMem_GetParamUlongValue 
        *   CcspLog_GetBulkParamValues
        *   CcspLog_SetBulkParamValues

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        11/08/2010    initial revision.
        02/08/2011    Bin changed the internal "DataModelLibrary" table
                      to a dynamic table.
        02/15/2011    Bin added "SupportedDataModel" support;

**********************************************************************/


#include "dslh_dmagnt_global.h"
#include "slap_vco_exported_api.h"
#include "ccsp_ifo_ccd.h"
#include "safec_lib_common.h"

#define PNAME_MAX_SIZE   1024
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhDmagntCreatePluginInfo
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to create a new PluginInfo Structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The new info handle

**********************************************************************/
ANSC_HANDLE
DslhDmagntCreatePluginInfo
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PPLUGIN_INFORMATION             pInfo             = (PPLUGIN_INFORMATION)NULL;

    pInfo = (PPLUGIN_INFORMATION)AnscAllocateMemory(sizeof(PLUGIN_INFORMATION));

    if( pInfo == NULL)
    {
        return pInfo;
    }

    pInfo->Version = 1;

    return pInfo;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntRemovePluginInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInfo
            );

    description:

        This function is called to remove the PluginInfo Structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInfo
                The plugin info handle;

    return:     The status of the operation

**********************************************************************/
ANSC_STATUS
DslhDmagntRemovePluginInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hPluginInfo
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PPLUGIN_INFORMATION             pInfo             = (PPLUGIN_INFORMATION)hPluginInfo;
    PANSC_ATOM_TABLE_OBJECT         pAtomFunctions    = (PANSC_ATOM_TABLE_OBJECT)NULL;

    if( !pMyObject || !pInfo)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( pInfo->uLoadStatus == COSA_STATUS_SUCCESS)
    {
        if( pInfo->UnloadProc != NULL)
        {
            pInfo->UnloadProc();
        }
    }

    if( pInfo->hInstance != NULL)
    {
        AnscFreeLibrary(pInfo->hInstance);

        pInfo->hInstance = NULL;
    }

    if( pInfo->SourceName != NULL)
    {
        AnscFreeMemory(pInfo->SourceName);
    }

    if( pInfo->ModuleName != NULL)
    {
        AnscFreeMemory(pInfo->ModuleName);
    }

    if( pInfo->Author != NULL)
    {
        AnscFreeMemory(pInfo->Author);
    }

    if( pInfo->Description != NULL)
    {
        AnscFreeMemory(pInfo->Description);
    }

    pAtomFunctions    = (PANSC_ATOM_TABLE_OBJECT)pInfo->hAtomFunc;
    if( pAtomFunctions != NULL && pAtomFunctions != pMyObject->hAtomFunctions)
    {
        pAtomFunctions->Remove(pAtomFunctions);
    }

    AnscFreeMemory(pInfo);

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntParseSourceInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXMLNode,
                ANSC_HANDLE                 hPluginInfo
            );

    description:

        This function is called to retrieve lib infor from the XML node;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXMLNode,
                The XML node handle;

                ANSC_HANDLE                 hPluginInfo
                The handle of the library information;

    return:     The status of the operation

**********************************************************************/
ANSC_STATUS
DslhDmagntParseSourceInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode        = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode      = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pLibNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PPLUGIN_INFORMATION             pPluginInfo     = (PPLUGIN_INFORMATION)hPluginInfo;
    char                            buffer[1024]    = { 0 };
    ULONG                           uLength         = 1023;
    char*                           errstr          = NULL;
    errno_t                         rc              = -1;

    if( hXMLNode == NULL)
    {
        return COSA_STATUS_FAILURE;
    }

     /* get the version information */
     pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
         pXmlNode->GetChildByName(pXmlNode, COSA_XML_NODE_version);

     if( pChildNode != NULL)
     {
         pChildNode->GetDataUlong(pChildNode, NULL, &pPluginInfo->Version);
     }
     else
     {
         pPluginInfo->Version = 1;
     }

     /* get moduleName */
     uLength = 1023;
     AnscZeroMemory(buffer, 1024);

     pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
         pXmlNode->GetChildByName(pXmlNode, COSA_XML_NODE_moduleName);

     if( pChildNode != NULL)
     {
         pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength);

         pPluginInfo->ModuleName = AnscCloneString(buffer);
         
         AnscTraceWarning(("DM Library module name = %s\n", pPluginInfo->ModuleName));
     }

     /* get Author */
     uLength = 1023;
     AnscZeroMemory(buffer, 1024);
     pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
         pXmlNode->GetChildByName(pXmlNode, COSA_XML_NODE_author);

     if( pChildNode != NULL)
     {
         pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength);

         pPluginInfo->Author = AnscCloneString(buffer);
     }

     /* get Description */
     uLength = 1023;
     AnscZeroMemory(buffer, 1024);
     pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
         pXmlNode->GetChildByName(pXmlNode, COSA_XML_NODE_description);

     if( pChildNode != NULL)
     {
         pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength);

         pPluginInfo->Description = AnscCloneString(buffer);
     }

     /* get the library infor */
     pLibNode = (PANSC_XML_DOM_NODE_OBJECT)
         pXmlNode->GetChildByName(pXmlNode, COSA_XML_NODE_library);

     if( pLibNode != NULL)
     {
         /* get lib_path name */
         uLength = 1023;
         AnscZeroMemory(buffer, 1024);
         pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
             pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_path);

         if( pChildNode != NULL)
         {
             if( pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS && uLength > 0)
             {
                 if (_ansc_strlen(buffer) + 5 < sizeof(buffer))
                 {
                    rc = strcpy_s(buffer+_ansc_strlen(buffer), (sizeof(buffer) - _ansc_strlen(buffer)), ".so");
                    if(rc != EOK)
                    {
                       ERR_CHK(rc);
                       return COSA_STATUS_FAILURE;
                    }
                 }
                 else 
                 {
                    AnscTraceWarning(("library name too long\n"));
                    return COSA_STATUS_ERROR_LOAD_LIBRARY;
                 }
                 pPluginInfo->SourceName = AnscCloneString(buffer);
                 printf("****LOADING DM LIBRARY***************\n");
                 printf("THE LIB NAME =%s\n",pPluginInfo->SourceName);
                 printf("****LOADING DM LIBRARY***************\n");
                AnscTraceWarning(("Loading DM library %s...\n", pPluginInfo->SourceName));
                if( pPluginInfo->SourceName != NULL)
                {
                    /* load the library */
                    pPluginInfo->hInstance = (ANSC_HANDLE)AnscLoadLibrary(pPluginInfo->SourceName);

                    if( pPluginInfo->hInstance == NULL)
                    {
                        AnscTraceWarning(("Unable to load library -- %s\n", pPluginInfo->SourceName));
                        printf("****ERROR LOADING DM LIB %s\n",pPluginInfo->SourceName);
                        errstr = dlerror();
                        printf("CAUSE =%s\n",errstr);
                        AnscTraceWarning(("cause:%s\n",  errstr ));
                        printf("CAUSE =%s\n",errstr);

                        pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_LOAD_LIBRARY;

                        return COSA_STATUS_ERROR_LOAD_LIBRARY;
                    }
                    printf("PLUGIN %s LOADED SUCCESSFULLY\n",pPluginInfo->SourceName);
                }
             }
         }

         /* retrieve "init", "isObjSupported" and "unload" api */
         if( pPluginInfo->SourceName != NULL)
         {
             uLength = 1023;
             AnscZeroMemory(buffer, 1024);
             pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
                 pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_init);

             if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS)
             {
                pPluginInfo->InitProc = (COSAInitProc)
                    AnscGetProcAddress
                    (
                        pPluginInfo->hInstance, 
                        buffer
                    );

                if( pPluginInfo->InitProc == NULL)
                {
                    AnscTraceWarning(("Unable to Get ProcAddress of  '%s'\n", buffer));

                    pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_GET_PROC;

                    return pPluginInfo->uLoadStatus;
                }
             }
             else
             {
                AnscTraceWarning(("No Init Proc defined.\n"));

                pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_GET_PROC;

                return pPluginInfo->uLoadStatus;
             }

             /* unsupported */
             uLength = 1023;
             AnscZeroMemory(buffer, 1024);
             pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
                 pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_support);

             if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS)
             {
                pPluginInfo->SupportProc = (COSASupportProc)
                    AnscGetProcAddress
                    (
                        pPluginInfo->hInstance, 
                        buffer
                    );
             }

             /* async_init */
             uLength = 1023;
             AnscZeroMemory(buffer, 1024);
             pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
                 pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_async_init);

             if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS)
             {
                pPluginInfo->AsyncInitProc = (COSAInitProc)
                    AnscGetProcAddress
                    (
                        pPluginInfo->hInstance, 
                        buffer
                    );
             }

             /* unload */
             uLength = 1023;
             AnscZeroMemory(buffer, 1024);
             pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
                 pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_unload);

             if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS)
             {
                pPluginInfo->UnloadProc = (COSAUnloadProc)
                    AnscGetProcAddress
                    (
                        pPluginInfo->hInstance, 
                        buffer
                    );

                if( pPluginInfo->UnloadProc == NULL)
                {
                    AnscTraceWarning(("Unable to Get ProcAddress of  '%s'\n", buffer));

                    pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_GET_PROC;

                    return pPluginInfo->uLoadStatus;
                }
             }

             /* memorycheck */
             uLength = 1023;
             AnscZeroMemory(buffer, 1024);
             pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
                 pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_memorycheck);

             if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS)
             {
                pPluginInfo->MemoryCheckProc = (COSAMemoryCheckProc)
                    AnscGetProcAddress
                    (
                        pPluginInfo->hInstance, 
                        buffer
                    );

                if( pPluginInfo->MemoryCheckProc == NULL)
                {
                    AnscTraceWarning(("Unable to Get ProcAddress of  '%s'\n", buffer));

                    pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_GET_PROC;

                    return pPluginInfo->uLoadStatus;
                }
             }

             /* memoryusage */
             uLength = 1023;
             AnscZeroMemory(buffer, 1024);
             pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
                 pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_memoryusage);

             if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS)
             {
                pPluginInfo->MemoryUsageProc = (COSAMemoryUsageProc)
                    AnscGetProcAddress
                    (
                        pPluginInfo->hInstance, 
                        buffer
                    );

                if( pPluginInfo->MemoryUsageProc == NULL)
                {
                    AnscTraceWarning(("Unable to Get ProcAddress of  '%s'\n", buffer));

                    pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_GET_PROC;

                    return pPluginInfo->uLoadStatus;
                }
             }

             /* memorytable */
             uLength = 1023;
             AnscZeroMemory(buffer, 1024);
             pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
                 pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_memorytable);

             if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS)
             {
                pPluginInfo->MemoryTableProc = (COSAMemoryTableProc)
                    AnscGetProcAddress
                    (
                        pPluginInfo->hInstance, 
                        buffer
                    );

                if( pPluginInfo->MemoryTableProc == NULL)
                {
                    AnscTraceWarning(("Unable to Get ProcAddress of  '%s'\n", buffer));

                    pPluginInfo->uLoadStatus = COSA_STATUS_ERROR_GET_PROC;

                    return pPluginInfo->uLoadStatus;
                }
             }
         }

     }
     else
     {
        /* from SLAP object */
         /*TODO: CID 61733 Dereference after null check - logic error*/
         pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
             pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_slapObjName);

         uLength = 1023;
         AnscZeroMemory(buffer, 1024);

         if( pChildNode != NULL)
         {
             pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength);

             pPluginInfo->SourceName = AnscCloneString(buffer);
         }

         pPluginInfo->bFromSlap = TRUE;
     }

     return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    prototype:

        ANSC_STATUS
        DslhDmagntParseSourceName
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXMLNode,
                char*                       pName
            );

    description:

        This function is called to retrieve the library file name only;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXMLNode,
                The XML node handle;

                char*                       pName
                The output lib file name

    return:     The status of the operation

**********************************************************************/
ANSC_STATUS
DslhDmagntParseSourceName
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        char*                       pName
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode        = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode      = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pLibNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    char                            buffer[1024]    = { 0 };
    ULONG                           uLength         = 1023;
    errno_t                         rc              = -1;

    if( hXMLNode == NULL)
    {
        return COSA_STATUS_FAILURE;
    }

     /* get the library infor */
     pLibNode = (PANSC_XML_DOM_NODE_OBJECT)
         pXmlNode->GetChildByName(pXmlNode, COSA_XML_NODE_library);

     if( pLibNode != NULL)
     {
         /* get lib_path name */
         pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
             pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_lib_path);

         if( pChildNode != NULL)
         {
             if( pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) == ANSC_STATUS_SUCCESS && uLength > 0)
             {
                 if (_ansc_strlen(buffer) + 5 < sizeof(buffer))
                 {
                    rc = strcpy_s(buffer+_ansc_strlen(buffer), (sizeof(buffer) - _ansc_strlen(buffer)), ".so");
                    if(rc != EOK)
                    {
                        ERR_CHK(rc);
                        return COSA_STATUS_FAILURE;
                    }
                 }
                 else 
                 {
                    AnscTraceWarning(("library name too long\n"));
                    return COSA_STATUS_ERROR_LOAD_LIBRARY;
                 }
                 
                 rc = strcpy_s(pName, PNAME_MAX_SIZE, buffer);
                 if (rc != EOK)
                 {
                     ERR_CHK(rc);
                     return ANSC_STATUS_FAILURE;
                 }

                 return ANSC_STATUS_SUCCESS;
             }
         }
     }
     else
     {
        /* from SLAP object */
         /*TODO CID: 63649 Dereference after null check - Logic error*/
         pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
             pLibNode->GetChildByName(pLibNode, COSA_XML_NODE_slapObjName);

         uLength = 1023;
         AnscZeroMemory(buffer, 1024);

         if( pChildNode != NULL)
         {
             pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength);


             rc = strcpy_s(pName, PNAME_MAX_SIZE, buffer);
             if (rc != EOK) {
                 ERR_CHK(rc);
                 return ANSC_STATUS_FAILURE;
             }

             return ANSC_STATUS_SUCCESS;
         }
     }

     return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntRegisterDataModelObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjectNode,
                char*                       pParentObjName,
                ANSC_HANDLE                 hPluginInfo,
                BOOL                        bPopulateTree
            );

    description:

        This function is called to register the object in the XML node;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjectNode,
                The TR69 object in XML node;

                char*                       pParentObjName,
                The parent object name;

                ANSC_HANDLE                 hPluginInfo
                The handle of PluginInfo;

                BOOL                        bPopulateTree
                Whether repopulate the tree or not;

    return:     TRUE or FALSE

**********************************************************************/
ANSC_STATUS
DslhDmagntRegisterDataModelObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjectNode,
        char*                       pParentObjName,
        ANSC_HANDLE                 hPluginInfo,
        BOOL                        bPopulateTree
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE)pCpeController->GetDslhMprIf(pCpeController);
    PPLUGIN_INFORMATION             pPluginInfo       = (PPLUGIN_INFORMATION)hPluginInfo;
    PANSC_XML_DOM_NODE_OBJECT       pObjNode          = (PANSC_XML_DOM_NODE_OBJECT)hObjectNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pListNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PDSLH_CWMP_OBJECT_DESCR         pObjDesp          = (PDSLH_CWMP_OBJECT_DESCR)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pNameNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PDSLH_CWMP_OBJECT_DESCR         pTableDesp        = (PDSLH_CWMP_OBJECT_DESCR)NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pObjEntity        = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    PDSLH_OBJ_ENTITY_OBJECT         pParentEntity     = (PDSLH_OBJ_ENTITY_OBJECT)NULL;
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    char                            buffer[1024]      = { 0 };
    ULONG                           uLength           = 1023;
    errno_t                         rc                = -1;
    
    pObjDesp = (PDSLH_CWMP_OBJECT_DESCR)AnscAllocateMemory(sizeof(DSLH_CWMP_OBJECT_DESCR));

    if( pObjDesp == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }
     
    returnStatus = 
        DslhDmagntParseObjectInfo(hThisObject, hObjectNode, pParentObjName, pPluginInfo, pObjDesp);

    /* check whether the object is supported or not */
    if( pPluginInfo->SupportProc != NULL)
    {
        if( !pPluginInfo->SupportProc(pObjDesp->Name))
        {
            AnscTraceWarning(("The object '%s' is not supported yet in the library. Ignore it!\n", pObjDesp->Name));
            AnscFreeMemory(pObjDesp->Name); /*RDKB-5789, CID-33000; free allocated resources*/
            AnscFreeMemory(pObjDesp); 
            return ANSC_STATUS_SUCCESS;
        }
    }

    if( pObjDesp->Type == DSLH_CWMP_OBJECT_TYPE_table)
    {
        /* if it's a table, we need to register both the table and entry objects */
        pCpeController->RegisterObject(pCpeController, pObjDesp, NULL);

        /* create another ObjDesp for the entry; */
        pTableDesp = pObjDesp;

        pObjDesp = (PDSLH_CWMP_OBJECT_DESCR)AnscAllocateMemory(sizeof(DSLH_CWMP_OBJECT_DESCR));

        if( pObjDesp == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        AnscCopyMemory(pObjDesp, pTableDesp, sizeof(DSLH_CWMP_OBJECT_DESCR));

        rc = sprintf_s(buffer, sizeof(buffer), "%s{i}.", pTableDesp->Name);
        if(rc < EOK)
        {
           ERR_CHK(rc);
        }

        pObjDesp->Name                      = AnscCloneString(buffer);
        pObjDesp->Type                      = DSLH_CWMP_OBJECT_TYPE_regular;
        pObjDesp->pfnObjcoConstructor       = DslhCreateObjController;

        /* Create DslhTr69If and copy it */
        COSACloneTR69Interface(pTableDesp->hDslhTr69If,pObjDesp->hDslhTr69If);
    }

    /* get the parameter array information */
    pChildNode = pObjNode->GetChildByName(pObjNode, COSA_XML_NODE_parameters);

    if( pChildNode != NULL)
    {
        pObjDesp->hParamArray = DslhDmagntParseParamArray(hThisObject, pChildNode, hPluginInfo);
    }

    /* register the object */
    pCpeController->RegisterObject(pCpeController, pObjDesp, pObjDesp->hParamArray);
    pPluginInfo->uRegObjCount ++;

    /* register the child object one by one */
    pListNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjNode->GetChildByName(pObjNode, COSA_XML_NODE_objects);

    if( pListNode != NULL)
    {
        pChildNode = pListNode->GetHeadChild(pListNode);

        while( pChildNode != NULL)
        {
            returnStatus = 
                DslhDmagntRegisterDataModelObject(hThisObject, pChildNode, pObjDesp->Name, hPluginInfo, FALSE /* no populate */);

            if( returnStatus != ANSC_STATUS_SUCCESS)
            {
                return returnStatus;
            }

            /* goto next one */
            pChildNode = pListNode->GetNextChild(pListNode, pChildNode);
        }
    }

    /* populate the tree if needed */
    if(bPopulateTree)
    {
        if( pTableDesp != NULL)
        {
            pObjDesp = pTableDesp;              
        }

        pObjEntity = pDslhMprIf->GetObjEntity(pDslhMprIf->hOwnerContext, pObjDesp->Name);

        if( pObjEntity != NULL)
        {
            pParentEntity = (PDSLH_OBJ_ENTITY_OBJECT)pObjEntity->hParentObjEntity;

            if( pParentEntity != NULL)
            {
                uLength           = 1023;
                AnscZeroMemory(buffer, 1024);

                /* get the name */
                pNameNode = (PANSC_XML_DOM_NODE_OBJECT)
                    pObjNode->GetChildByName(pObjNode, COSA_XML_NODE_name);

                pNameNode->GetDataString(pNameNode, NULL, buffer, &uLength);

                pParentEntity->PopulateObjRecordByName(pParentEntity, buffer);
            }
        }
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhDmagntCheckApiResolved
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXMLNode,
                ANSC_HANDLE                 hPluginInfo
            );

    description:

        This function is called to check whether all the apis defined
        in the XML file are resolved.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXMLNode,
                The XML node handle;

                ANSC_HANDLE                 hPluginInfo
                The handle of pluginInfo;

    return:     TRUE or FALSE

**********************************************************************/
BOOL
lookforUnresolvedApiInParamList
    (
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    )
{
    PPLUGIN_INFORMATION             pPluginInfo       = (PPLUGIN_INFORMATION)hPluginInfo;
    PANSC_ATOM_TABLE_OBJECT         pAtomFuncs        = (PANSC_ATOM_TABLE_OBJECT)pPluginInfo->hAtomFunc;
    ANSC_HANDLE                     hInstance         = (ANSC_HANDLE)pPluginInfo->hInstance;
    PANSC_XML_DOM_NODE_OBJECT       pListNode         = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pFuncNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    BOOL                            bSuccc            = TRUE;
    char                            buffer[256]       = { 0 };
    ULONG                           uLength           = 255;

    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pListNode->GetHeadChild(pListNode);

    if( pChildNode != NULL)
    {
        /* find the "func_GetValue" if exists */
        pFuncNode = (PANSC_XML_DOM_NODE_OBJECT)pChildNode->GetChildByName(pChildNode, COSA_XML_NODE_param_func_GetValue);

        uLength = 255;

        if( pFuncNode != NULL && ANSC_STATUS_SUCCESS == pFuncNode->GetDataString(pFuncNode, NULL, buffer, &uLength) && uLength > 0)
        {
            if( hInstance != NULL)
            {
                if( AnscGetProcAddress(hInstance, buffer) == NULL)
                {
                    AnscTrace("Found an unresolved api: %s\n", buffer);

                    bSuccc = FALSE;
                }
            }
            else
            {
                if( pAtomFuncs->GetAtomByName(pAtomFuncs, buffer) == NULL)
                {
                    AnscTrace("Found an unresolved api: %s\n", buffer);

                    bSuccc = FALSE;
                }
            }
        }

        /* next one */
        pChildNode = pListNode->GetNextChild(pListNode, pChildNode);
    }

    return bSuccc;
}

BOOL
lookforUnresolvedApiInObject
    (
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    )
{
    PPLUGIN_INFORMATION             pPluginInfo       = (PPLUGIN_INFORMATION)hPluginInfo;
    PANSC_ATOM_TABLE_OBJECT         pAtomFuncs        = (PANSC_ATOM_TABLE_OBJECT)pPluginInfo->hAtomFunc;
    ANSC_HANDLE                     hInstance         = (ANSC_HANDLE)pPluginInfo->hInstance;
    PANSC_XML_DOM_NODE_OBJECT       pObjectNode       = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pListNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pFuncNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    char                            buffer[256]       = { 0 };
    BOOL                            bSuccc            = TRUE;
    ULONG                           uLength           = 255;

    /* get the functions object */
    pListNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_functions);

    if( pListNode != NULL)
    {
        pFuncNode = (PANSC_XML_DOM_NODE_OBJECT)
            pListNode->GetHeadChild(pListNode);

        if( pFuncNode != NULL && ANSC_STATUS_SUCCESS == pFuncNode->GetDataString(pFuncNode, NULL, buffer, &uLength) && uLength > 0)
        {
            if( hInstance != NULL)
            {
                if( AnscGetProcAddress(hInstance, buffer) == NULL)
                {
                    AnscTraceWarning(("Found an unresolved api: %s\n", buffer));

                    bSuccc = FALSE;
                }
            }
            else
            {
                if( pAtomFuncs->GetAtomByName(pAtomFuncs, buffer) == NULL)
                {
                    AnscTraceWarning(("Found an unresolved api: %s\n", buffer));

                    bSuccc = FALSE;
                }
            }
        }

        /* next one */
        pFuncNode = pListNode->GetNextChild(pListNode, pFuncNode);       
    }

    /* check parameterlist  */
    pListNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_parameters);

    if( pListNode != NULL)
    {
        if(!lookforUnresolvedApiInParamList(pListNode, hPluginInfo))
        {
            bSuccc = FALSE;
        }
    }

    /* check the child objects */
    pListNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_objects);

    if( pListNode != NULL)
    {
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
            pListNode->GetHeadChild(pListNode);

        if( pChildNode != NULL)
        {
            if(!lookforUnresolvedApiInObject(pChildNode, hPluginInfo))
            {
                bSuccc = FALSE;
            }

            /* next one */
            pChildNode = pListNode->GetNextChild(pListNode, pChildNode);
        }
    }

    return bSuccc;
}


BOOL
DslhDmagntCheckApiResolved
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_XML_DOM_NODE_OBJECT       pRootNode         = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pObjectsNode      = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pObjectNode       = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    BOOL                            bSuccc            = TRUE;

    pObjectsNode = (PANSC_XML_DOM_NODE_OBJECT)
         pRootNode->GetChildByName(pRootNode, COSA_XML_NODE_objects);

    if( pObjectsNode == NULL)
    {
        return TRUE;
    }

    pObjectNode = (PANSC_XML_DOM_NODE_OBJECT)
         pObjectsNode->GetHeadChild(pObjectsNode);

    while( pObjectNode != NULL)
    {
        if(!lookforUnresolvedApiInObject(pObjectNode, hPluginInfo))
        {
            bSuccc = FALSE;
        }

        /* next one */
        pObjectNode = pObjectNode->GetNextChild(pObjectNode, pObjectNode);
    }

    return bSuccc;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DslhDmagntCheckBaseObjNames
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pRootName,
                ANSC_HANDLE                 hXMLNode
            );

    description:

        This function is called to check whether the base objects are registered or not.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pRootName,
                The root object name;

                ANSC_HANDLE                 hXMLNode,
                The XML node handle;

    return:     TRUE or FALSE

**********************************************************************/
BOOL
DslhDmagntCheckBaseObjNames
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pRootName,
        ANSC_HANDLE                 hXMLNode
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode          = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pObjectsNode      = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pObjectNode       = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pBaseObjNode      = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE        )pCpeController->GetDslhMprIf(pCpeController);
    char                            pFullName[300]    = { 0 };
    char                            buffer[256]       = { 0 };
    ULONG                           uLength           = 255;
    errno_t                         rc                = -1;

    pObjectsNode = (PANSC_XML_DOM_NODE_OBJECT)
         pXmlNode->GetChildByName(pXmlNode, COSA_XML_NODE_objects);

    if( pObjectsNode == NULL)
    {
        return TRUE;
    }

    pObjectNode = (PANSC_XML_DOM_NODE_OBJECT)
         pObjectsNode->GetHeadChild(pObjectsNode);

    while( pObjectNode != NULL)
    {
        /* check the base obj name */
        pBaseObjNode = pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_baseObjName);

        uLength = 255;
        AnscZeroMemory(buffer, uLength);

        if( pBaseObjNode != NULL &&
            ANSC_STATUS_SUCCESS == pBaseObjNode->GetDataString(pBaseObjNode, NULL, buffer, &uLength))
        {
            /* check the object name exist or not */
            if( uLength > 0)
            {
                /* make sure the base object exists */
                AnscTrace("Looking for base object: %s\n", buffer);

                if( _ansc_strstr(buffer, "com.") != NULL || _ansc_strstr(buffer, "CCSP.") != NULL || _ansc_strstr(buffer, pRootName) != NULL)
                {
                    rc = strcpy_s(pFullName, sizeof(pFullName), buffer);
                    if(rc != EOK)
                    {
                       ERR_CHK(rc);
                       return FALSE;
                    }
                }
                else if( buffer[AnscSizeOfString(buffer) - 1] != '.')
                {
                    rc = sprintf_s(pFullName, sizeof(pFullName), "%s%s.", pRootName, buffer);
                    if(rc < EOK)
                    {
                       ERR_CHK(rc);
                       return FALSE;
                    }
                }
                else
                {
                    rc = sprintf_s(pFullName, sizeof(pFullName), "%s%s", pRootName, buffer);
                    if(rc < EOK)
                    {
                       ERR_CHK(rc);
                       return FALSE;
                    }
                }

                if( pDslhMprIf->GetObjEntity(pDslhMprIf->hOwnerContext, pFullName) == NULL)
                {
                    /* try to create this object */
                    AnscTraceWarning(("Try to create empty object : %s\n", pFullName));

                    if( ANSC_STATUS_SUCCESS != pDslhMprIf->RegEmptyObject(pDslhMprIf->hOwnerContext, pFullName))
                    {
                        AnscTrace("Failed to create parent object: %s\n", pFullName);

                        return FALSE;
                    }
                }
            }
        }

        /* next one */
        pObjectNode = pObjectsNode->GetNextChild(pObjectsNode, pObjectNode);
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhDmagntParseObjFuncInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXMLNode,
                ANSC_HANDLE                 hPluginInfo
            );

    description:

        This function is called to retrieve the object functions info from the XML nodes;

    argument:   ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXMLNode,
                The function XML node;

                ANSC_HANDLE                 hPluginInfo
                The handle of pluginInfo;

    return:     The tr069 interface handle;

**********************************************************************/
ANSC_HANDLE
DslhDmagntParseObjFuncInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PPLUGIN_INFORMATION             pPluginInfo       = (PPLUGIN_INFORMATION)hPluginInfo;
    PANSC_ATOM_TABLE_OBJECT         pAtomFuncs        = (PANSC_ATOM_TABLE_OBJECT)pPluginInfo->hAtomFunc;
    ANSC_HANDLE                     hInstance         = (ANSC_HANDLE)pPluginInfo->hInstance;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode          = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PDSLH_TR69_INTERFACE            pDslhTr69If       = (PDSLH_TR69_INTERFACE)NULL;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor   = NULL;
    ANSC_HANDLE                     hProc             = (ANSC_HANDLE)NULL;
    char*                           pName             = NULL;
    char                            buffer[256]       = { 0 };
    ULONG                           uLength           = 255;

    if( hXMLNode == NULL)
    {
        return NULL;
    }

    /* create DslhTr69If interface */
    pDslhTr69If = (PDSLH_TR69_INTERFACE)AnscAllocateMemory(sizeof(DSLH_TR69_INTERFACE));

    if( pDslhTr69If == NULL)
    {
        return NULL;
    }

    pDslhTr69If->bUseFullName  = FALSE;
    pDslhTr69If->hPoamObject   = pPluginInfo->hPoamObj;

    /* go through the apis one by one */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pXmlNode->GetHeadChild(pXmlNode);

    while( pChildNode != NULL)
    {
        uLength           = 255;
        AnscZeroMemory(buffer, 256);

        if( ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) && uLength > 0)
        {
            pName = pChildNode->GetName(pChildNode);

            /* get the proc */
            if( hInstance == NULL)
            {
                pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR)pAtomFuncs->GetAtomByName(pAtomFuncs, buffer);

                if( pAtomDescriptor != NULL)
                {
                    hProc = pAtomDescriptor->hContext;
                }
            }
            else
            {
                hProc = AnscGetProcAddress(hInstance, buffer);
            }


            if( strcmp(pName, COSA_XML_NODE_func_GetEntryCount) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.GetEntryCount = (PFN_DSLHTR69_GET_COUNT)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetEntryStatus) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.GetEntryStatus = (PFN_DSLHTR69_GET_STATUS)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetEntry) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.GetEntry = (PFN_DSLHTR69_GET_ENTRY)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_AddEntry) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.AddEntry = (PFN_DSLHTR69_ADD_ENTRY)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_DelEntry) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.DelEntry = (PFN_DSLHTR69_DEL_ENTRY)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_IsUpdated) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.IsUpdated = (PFN_DSLHTR69_UPDATE)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_CheckInstance) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.CheckInstance = (PFN_DSLHTR69_UPDATE)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_Synchronize) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithLastName.Synchronize = (PFN_DSLHTR69_ACTION)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamBoolValue) == 0 )
            {
                pDslhTr69If->GetEntryParamBool = (PFN_DSLHTR69_GET_BOOL)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamIntValue) == 0 )
            {
                pDslhTr69If->GetEntryParamInt = (PFN_DSLHTR69_GET_INT)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamUlongValue) == 0 )
            {
                pDslhTr69If->GetEntryParamUlong = (PFN_DSLHTR69_GET_ULONG)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamStringValue) == 0 )
            {
                pDslhTr69If->GetEntryParamString = (PFN_DSLHTR69_GET_STRING)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamBoolValue) == 0 )
            {
                pDslhTr69If->SetEntryParamBool = (PFN_DSLHTR69_SET_BOOL)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamIntValue) == 0 )
            {
                pDslhTr69If->SetEntryParamInt = (PFN_DSLHTR69_SET_INT)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamUlongValue) == 0 )
            {
                pDslhTr69If->SetEntryParamUlong = (PFN_DSLHTR69_SET_ULONG)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamStringValue) == 0 )
            {
                pDslhTr69If->SetEntryParamString = (PFN_DSLHTR69_SET_STRING)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_Validate) == 0 )
            {
                pDslhTr69If->Methods.MethodWithLastName.Validate = (PFN_DSLHTR69_VALID)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_Commit) == 0 )
            {
                pDslhTr69If->Methods.MethodWithLastName.Commit = (PFN_DSLHTR69_ACTION)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_Rollback) == 0 )
            {
                pDslhTr69If->Methods.MethodWithLastName.Rollback = (PFN_DSLHTR69_ACTION)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamBulkValues) == 0 )
            {
                 pDslhTr69If->GetEntryBulk = (PFN_DSLHTR69_GET_BULK)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamBulkValues) == 0 )
            {
                 pDslhTr69If->SetEntryBulk = (PFN_DSLHTR69_SET_BULK)hProc;
            }
            else if( pName != NULL)
            {
                AnscTraceWarning(("Unknown function type: %s\n", pName));
            }
        }
        
        /* goto next */
        pChildNode = pXmlNode->GetNextChild(pXmlNode, pChildNode);
    }
    
    return pDslhTr69If;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhDmagntParseRootFuncInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXMLNode,
                ANSC_HANDLE                 hAtomFunc
            );

    description:

        This function is called to retrieve the object functions info defined
        in the XML root node;

    argument:   ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXMLNode,
                The function XML node;

                ANSC_HANDLE                 hAtomFunc
                The registered function atom table

    return:     The tr069 interface handle;

**********************************************************************/
ANSC_HANDLE
DslhDmagntParseRootFuncInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hAtomFunc
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PANSC_ATOM_TABLE_OBJECT         pAtomFuncs        = (PANSC_ATOM_TABLE_OBJECT)hAtomFunc;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode          = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PDSLH_TR69_INTERFACE            pDslhTr69If       = (PDSLH_TR69_INTERFACE)NULL;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor   = NULL;
    ANSC_HANDLE                     hProc             = (ANSC_HANDLE)NULL;
    char*                           pName             = NULL;
    char                            buffer[256]       = { 0 };
    ULONG                           uLength           = 255;

    if( hXMLNode == NULL)
    {
        return NULL;
    }

    /* create DslhTr69If interface */
    pDslhTr69If = (PDSLH_TR69_INTERFACE)AnscAllocateMemory(sizeof(DSLH_TR69_INTERFACE));

    if( pDslhTr69If == NULL)
    {
        return NULL;
    }

    pDslhTr69If->bUseFullName  = TRUE;

    /* go through the apis one by one */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pXmlNode->GetHeadChild(pXmlNode);

    while( pChildNode != NULL)
    {
        uLength           = 255;
        AnscZeroMemory(buffer, 256);

        if( ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) && uLength > 0)
        {
            pName = pChildNode->GetName(pChildNode);

            /* get the proc */
            pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR)pAtomFuncs->GetAtomByName(pAtomFuncs, buffer);

            if( pAtomDescriptor != NULL)
            {
                hProc = pAtomDescriptor->hContext;
            }

            if( strcmp(pName, COSA_XML_NODE_func_GetEntryCount) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithFullName.GetEntryCount = (PFN_DSLHTR69_GET_COUNT1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetEntry) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithFullName.GetEntry = (PFN_DSLHTR69_GET_ENTRY1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_AddEntry) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithFullName.AddEntry = (PFN_DSLHTR69_ADD_ENTRY1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_DelEntry) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithFullName.DelEntry = (PFN_DSLHTR69_DEL_ENTRY1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_IsUpdated) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithFullName.IsUpdated = (PFN_DSLHTR69_UPDATE1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_CheckInstance) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithFullName.CheckInstance = (PFN_DSLHTR69_UPDATE1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_Synchronize) == 0 )
            {
                 pDslhTr69If->Methods.MethodWithFullName.Synchronize = (PFN_DSLHTR69_ACTION1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamBoolValue) == 0 )
            {
                pDslhTr69If->GetEntryParamBool = (PFN_DSLHTR69_GET_BOOL)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamIntValue) == 0 )
            {
                pDslhTr69If->GetEntryParamInt = (PFN_DSLHTR69_GET_INT)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamUlongValue) == 0 )
            {
                pDslhTr69If->GetEntryParamUlong = (PFN_DSLHTR69_GET_ULONG)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamStringValue) == 0 )
            {
                pDslhTr69If->GetEntryParamString = (PFN_DSLHTR69_GET_STRING)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamBoolValue) == 0 )
            {
                pDslhTr69If->SetEntryParamBool = (PFN_DSLHTR69_SET_BOOL)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamIntValue) == 0 )
            {
                pDslhTr69If->SetEntryParamInt = (PFN_DSLHTR69_SET_INT)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamUlongValue) == 0 )
            {
                pDslhTr69If->SetEntryParamUlong = (PFN_DSLHTR69_SET_ULONG)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamStringValue) == 0 )
            {
                pDslhTr69If->SetEntryParamString = (PFN_DSLHTR69_SET_STRING)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_Validate) == 0 )
            {
                pDslhTr69If->Methods.MethodWithFullName.Validate = (PFN_DSLHTR69_VALID1)hProc;

            }
            else if( strcmp(pName, COSA_XML_NODE_func_Commit) == 0 )
            {
                pDslhTr69If->Methods.MethodWithFullName.Commit = (PFN_DSLHTR69_ACTION1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_Rollback) == 0 )
            {
                pDslhTr69If->Methods.MethodWithFullName.Rollback = (PFN_DSLHTR69_ACTION1)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_GetParamBulkValues) == 0 )
            {
                 pDslhTr69If->GetEntryBulk = (PFN_DSLHTR69_GET_BULK)hProc;
            }
            else if( strcmp(pName, COSA_XML_NODE_func_SetParamBulkValues) == 0 )
            {
                 pDslhTr69If->SetEntryBulk = (PFN_DSLHTR69_SET_BULK)hProc;
            }
            else if( pName != NULL)
            {
                AnscTraceWarning(("Unknown function type: %s\n", pName));
            }
        }
        
        /* goto next */
        pChildNode = pXmlNode->GetNextChild(pXmlNode, pChildNode);
    }
    
    return pDslhTr69If;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntParseObjectInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjectNode,
                char*                       pParentObjName,
                ANSC_HANDLE                 hPluginInfo,
                ANSC_HANDLE                 hObjectDesp
            );

    description:

        This function is called to retrieve the object infor from the XML node;

    argument:   ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hObjectNode,
                The object XML node;

                char*                       pParentObjName,
                The parent object name;

                ANSC_HANDLE                 hPluginInfo,
                The handle of PluginInfo;

                ANSC_HANDLE                 hObjectDesp
                The object descriptor handle;

    return:     The status of the operation;

**********************************************************************/
ANSC_STATUS
DslhDmagntParseObjectInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjectNode,
        char*                       pParentObjName,
        ANSC_HANDLE                 hPluginInfo,
        ANSC_HANDLE                 hObjectDesp
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pObjectNode       = (PANSC_XML_DOM_NODE_OBJECT)hObjectNode;
    PDSLH_CWMP_OBJECT_DESCR         pObjectDesp       = (PDSLH_CWMP_OBJECT_DESCR)hObjectDesp;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    char                            pName[512]        = { 0 };
    char                            buffer[1024]      = { 0 };
    char                            buffer2[1024]     = { 0 };
    ULONG                           uLength           = 1023;
    errno_t                         rc                = -1;

    /* get the name */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_name);

    if( pChildNode == NULL || pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) != ANSC_STATUS_SUCCESS || uLength == 0)
    {
        AnscTraceWarning(("Empty object.\n"));

        return ANSC_STATUS_FAILURE;
    }

    uLength = 1023;

    /* get the base object name */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_baseObjName);

    if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer2, &uLength) == ANSC_STATUS_SUCCESS && uLength > 0)
    {
        if( _ansc_strstr(buffer2, "com.") == buffer2    || _ansc_strstr(buffer2, "CCSP.") == buffer2 || 
            _ansc_strstr(buffer2, "Device.") == buffer2 || _ansc_strstr(buffer2, "InternetGatewayDevice.") == buffer2)
        {
            rc = sprintf_s(pName, sizeof(pName), "%s%s.", buffer2, buffer);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( buffer2[AnscSizeOfString(buffer2)-1] != '.') /* not ends with '.' */
        {
            rc = sprintf_s(pName, sizeof(pName), "%s%s.%s.", pParentObjName, buffer2, buffer);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            rc = sprintf_s(pName, sizeof(pName), "%s%s%s.", pParentObjName, buffer2, buffer);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return ANSC_STATUS_FAILURE;
            }
        }

        pObjectDesp->Name = AnscCloneString(pName);
        
        /*for cisco.spvtg.ccsp.XXX. base object, we pass DataModelAgent to ObjController->hInsContext*/
        if (strcmp(buffer2, "com.cisco.spvtg.ccsp.") == 0 )
        {
            AnscTrace(" base object  %s config context\n", pName);
            pObjectDesp->hContextToController = pMyObject;
        }
    }
    else
    {
        rc = sprintf_s(pName, sizeof(pName), "%s.", buffer);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }

        if( !strcmp(pName, pParentObjName) == 0 )
        {
            rc = sprintf_s(pName, sizeof(pName), "%s%s.", pParentObjName, buffer);
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return ANSC_STATUS_FAILURE;
            }
        }

        pObjectDesp->Name       = AnscCloneString(pName);
        
        /*for cisco.spvtg.ccsp.XXX.Logging. & cisco.spvtg.ccsp.XXX.Memory., we pass DataModelAgent to ObjController->hInsContext*/
        if (strcmp(buffer, "Logging") == 0 ||
            strcmp(buffer, "Memory") == 0 )
        {
            unsigned int _size = _ansc_strlen("com.cisco.spvtg.ccsp.");

            if (_ansc_strlen(pParentObjName) > _size)
            {
                if (_ansc_strncmp(pParentObjName, "com.cisco.spvtg.ccsp.", _size) == 0)
                {
                    AnscTrace(" %s config context\n", pName);
                    pObjectDesp->hContextToController = pMyObject;
                }           
            }
        }
    }

    /* get the object type */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_objectType);

    AnscZeroMemory(buffer, 1024);
    uLength = 1023;
    if( pChildNode == NULL || ANSC_STATUS_SUCCESS != pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) || uLength == 0 ||
        strcmp(buffer, COSA_XML_type_object) == 0 )
    {
        /* it's an object */
        pObjectDesp->Type                   = DSLH_CWMP_OBJECT_TYPE_regular;
        pObjectDesp->bWritable              = FALSE;
        pObjectDesp->bInvisible             = FALSE;
        pObjectDesp->bDynamic               = FALSE;
        pObjectDesp->bGroupCommit           = TRUE;
        pObjectDesp->pfnObjcoConstructor    = DslhCreateObjController;
        pObjectDesp->MaxInstanceNumber      = 1;
    }
    else
    {
        pObjectDesp->Type                   = DSLH_CWMP_OBJECT_TYPE_table;
        pObjectDesp->bWritable              = FALSE;
        pObjectDesp->bInvisible             = FALSE;
        pObjectDesp->bDynamic               = FALSE;
        pObjectDesp->bGroupCommit           = TRUE;
        pObjectDesp->pfnObjcoConstructor    = DslhCreateObjcoTable;
        pObjectDesp->MaxInstanceNumber      = 8;

        /* it's an table */
        if( strcmp(buffer, COSA_XML_type_dynamicTable) == 0 )
        {
            pObjectDesp->bDynamic               = TRUE;
        }
        else if( strcmp(buffer, COSA_XML_type_writableTable) == 0 )
        {
            pObjectDesp->bWritable              = TRUE;
        }
        else if( strcmp(buffer, COSA_XML_type_dynWritableTable) == 0 )
        {
            pObjectDesp->bWritable              = TRUE;
            pObjectDesp->bDynamic               = TRUE;
        }

        /* get the maxi instance if exists */
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
            pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_maxInstance);

        if( pChildNode != NULL)
        {
            pChildNode->GetDataUlong(pChildNode, NULL, &pObjectDesp->MaxInstanceNumber);
        }
    }


    /* get the object function apis */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_functions);

    if( pChildNode != NULL)
    {
        pObjectDesp->hDslhTr69If = DslhDmagntParseObjFuncInfo(hThisObject, pChildNode, hPluginInfo);
    }

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhDmagntParseParamArray
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hXMLNode,
                ANSC_HANDLE                 hPluginInfo
            );

    description:

        This function is called to parse the parameter array from XML node;

    argument:   ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hXMLNode,
                The Parameters XML node;

                ANSC_HANDLE                 hPluginInfo
                The registered function atom table

    return:     The handle of parameter array;

**********************************************************************/
ANSC_HANDLE
DslhDmagntParseParamArray
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXMLNode,
        ANSC_HANDLE                 hPluginInfo
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pListNode         = (PANSC_XML_DOM_NODE_OBJECT)hXMLNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_HANDLE                    pParamDescrArray  = NULL;
    PDSLH_CWMP_PARAM_DESCR          pParamDesp        = (PDSLH_CWMP_PARAM_DESCR)NULL;
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    ULONG                           uCount            = 0;
    ULONG                           i                 = 0;
    PPLUGIN_INFORMATION             pPluginInfo       = (PPLUGIN_INFORMATION)hPluginInfo;

    /* get the count of the parameters */
    uCount = AnscQueueQueryDepth(&pListNode->ChildNodeQueue);

    pParamDescrArray = (PANSC_HANDLE)AnscAllocateMemory(sizeof(ANSC_HANDLE) * (uCount + 1));

    if( pParamDescrArray == NULL)
    {
        return NULL;
    }

    /* get the first one */
    pChildNode = pListNode->GetHeadChild(pListNode);

    while(pChildNode)
    {
        pParamDesp = (PDSLH_CWMP_PARAM_DESCR)AnscAllocateMemory(sizeof(DSLH_CWMP_PARAM_DESCR));            

        if( pParamDesp == NULL)
        {
            break;
        }

        returnStatus = 
            DslhDmagntParseParamInfo(hThisObject, pChildNode, (ANSC_HANDLE)pPluginInfo, pParamDesp);

        if( returnStatus == ANSC_STATUS_SUCCESS)
        {
            pParamDescrArray[i] = (ANSC_HANDLE)pParamDesp;
            i ++;
        }
        else
        {
            DslhCwmpFreeParamDescr(pParamDesp);            
        }

        /* goto next one */
        pChildNode = pListNode->GetNextChild(pListNode, pChildNode);
    }

    pPluginInfo->uRegParamCount += i;

    return pParamDescrArray;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhDmagntParseParamInfo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParamNode,
                ANSC_HANDLE                 hPluginInfo,
                ANSC_HANDLE                 hParamDesp
            );

    description:

        This function is called to retrieve the parameter infor from the XML node;

    argument:   ANSC_HANDLE                 hThisObject,
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParamNode,
                The parameter XML node;

                ANSC_HANDLE                 hPluginInfo,
                The handle of plugin info;

                ANSC_HANDLE                 hParamDesp
                The parameter descriptor handle;

    return:     The status of the operation;

**********************************************************************/
ANSC_STATUS
DslhDmagntParseParamInfo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParamNode,
        ANSC_HANDLE                 hPluginInfo,
        ANSC_HANDLE                 hParamDesp
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    PPLUGIN_INFORMATION             pPluginInfo       = (PPLUGIN_INFORMATION)hPluginInfo;
    PANSC_ATOM_TABLE_OBJECT         pAtomFuncs        = (PANSC_ATOM_TABLE_OBJECT)pPluginInfo->hAtomFunc;
    ANSC_HANDLE                     hInstance         = (ANSC_HANDLE)pPluginInfo->hInstance;
    PANSC_XML_DOM_NODE_OBJECT       pParamNode        = (PANSC_XML_DOM_NODE_OBJECT)hParamNode;
    PDSLH_CWMP_PARAM_DESCR          pParamDesp        = (PDSLH_CWMP_PARAM_DESCR)hParamDesp;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor   = NULL;
    char                            buffer[1024]      = { 0 };
    ULONG                           uLength           = 1023;

    /* get the name */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_name);

    if( pChildNode == NULL || pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) != ANSC_STATUS_SUCCESS || uLength == 0)
    {
        AnscTraceWarning(("Empty parameter name.\n"));

        return ANSC_STATUS_FAILURE;
    }

    pParamDesp->Name = AnscCloneString(buffer);

    /* get the type */
    AnscZeroMemory(buffer, 1024);
    uLength           = 1023;

    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_type);

    if( pChildNode == NULL || pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) != ANSC_STATUS_SUCCESS || uLength == 0)
    {
        AnscTraceWarning(("Empty or invalid paramter type.\n"));

        return ANSC_STATUS_FAILURE;
    }

    pParamDesp->DataType = AnscCloneString(buffer);

    /* get the Syntax */
    AnscZeroMemory(buffer, 1024);
    uLength           = 1023;

    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_syntax);

    if( pChildNode == NULL || pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength) != ANSC_STATUS_SUCCESS || uLength == 0)
    {
        AnscTraceWarning(("Empty or invalid paramter syntax.\n"));

        return ANSC_STATUS_FAILURE;
    }

    pParamDesp->Syntax = AnscCloneString(buffer);

    /* get the bsUpdate */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_bsUpdate);

    AnscZeroMemory(buffer, 1024);
    uLength = 1023;

    if( pChildNode != NULL && ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength))
    {
        if( strcmp(buffer, COSA_XML_bsUpdate_firmware) == 0 )
        {
            pParamDesp->bsUpdate = DSLH_CWMP_BS_UPDATE_firmware;
        }
        else if( strcmp(buffer, COSA_XML_bsUpdate_rfcUpdate) == 0 )
        {
            pParamDesp->bsUpdate = DSLH_CWMP_BS_UPDATE_rfcUpdate;
        }
        else
        {
            pParamDesp->bsUpdate = DSLH_CWMP_BS_UPDATE_allUpdate;
        }
    }
    else /* by default bsUpdate is allUpdate */
    {
        pParamDesp->bsUpdate = DSLH_CWMP_BS_UPDATE_allUpdate;
    }

    /* get the writable */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_writable);

    if( pChildNode != NULL)
    {
        pChildNode->GetDataBoolean(pChildNode, NULL, &pParamDesp->bWritable);
    }
    else
    {
        pParamDesp->bWritable = FALSE;
    }

    /* get invisible */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_invisible);

    if( pChildNode != NULL)
    {
        pChildNode->GetDataBoolean(pChildNode, NULL, &pParamDesp->bInvisible);
    }
    else
    {
        pParamDesp->bInvisible = FALSE;
    }

    /* get needReboot */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_rebootNeeded);

    if( pChildNode != NULL)
    {
        pChildNode->GetDataBoolean(pChildNode, NULL, &pParamDesp->bNeedReboot);
    }
    else
    {
        pParamDesp->bNeedReboot = FALSE;
    }

    /* get persistent */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_persistent);

    if( pChildNode != NULL)
    {
        pChildNode->GetDataBoolean(pChildNode, NULL, &pParamDesp->bPersistent);
    }
    else
    {
        pParamDesp->bPersistent = FALSE;
    }

    /* get notification */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_notify);

    AnscZeroMemory(buffer, 1024);
    uLength = 1023;
    if( pChildNode != NULL && ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength))
    {
        if( strcmp(buffer, COSA_XML_notify_off) == 0 || strcmp(buffer, COSA_XML_notify_alwaysOff) == 0 )
        {
            pParamDesp->NotifyStatus = DSLH_CWMP_NOTIFY_STATUS_alwaysOff;
        }
        else if( strcmp(buffer, COSA_XML_notify_on) == 0 || strcmp(buffer, COSA_XML_notify_alwaysOn) == 0 )
        {
            pParamDesp->NotifyStatus = DSLH_CWMP_NOTIFY_STATUS_alwaysOn;
        }
        else if( strcmp(buffer, COSA_XML_notify_configurable) == 0 || strcmp(buffer, COSA_XML_notify_configPassive) == 0 )
        {
            pParamDesp->NotifyStatus = DSLH_CWMP_NOTIFY_STATUS_configurable;
        }
        else
        {
            pParamDesp->NotifyStatus = DSLH_CWMP_NOTIFY_STATUS_configActive;
        }
    }
    else /* Tom add, by default NotifyStatus changed to configActuve */
    {
        pParamDesp->NotifyStatus = DSLH_CWMP_NOTIFY_STATUS_configActive;
    }
    
    /* get notification threshold */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_notifyThrdEnabled);
    if( pChildNode != NULL)
    {
        pChildNode->GetDataBoolean(pChildNode, NULL, &pParamDesp->bThresholdEnabled);
    }
    else
    {
        pParamDesp->bThresholdEnabled = FALSE;
    }
    
    AnscZeroMemory(buffer, 1024);
    uLength = 1023;
    
    if ( pParamDesp->bThresholdEnabled == TRUE )
    {
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
            pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_notifyThresholdMin);
        if( pChildNode != NULL && ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength))
        {
            pParamDesp->NotifyThresholdMin = SlapVcoStringToInt(NULL, buffer);
        }

        AnscZeroMemory(buffer, 1024);
        uLength = 1023;
        
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
            pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_notifyThresholdMax);
        if( pChildNode != NULL && ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength))
        {
            pParamDesp->NotifyThresholdMax = SlapVcoStringToInt(NULL, buffer);
        }

        AnscZeroMemory(buffer, 1024);
        uLength = 1023;
    }
    
    /* get func_GetValue */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_func_GetValue);

    AnscZeroMemory(buffer, 1024);
    uLength = 1023;
    if( pChildNode != NULL && ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength))
    {
        if( hInstance != NULL)
        {
            pParamDesp->CallEntry_GetValue = (ULONG)AnscGetProcAddress(hInstance, buffer);
        }
        else
        {
            pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR)pAtomFuncs->GetAtomByName(pAtomFuncs, buffer);

            if( pAtomDescriptor != NULL)
            {
                pParamDesp->CallEntry_GetValue = (ULONG)pAtomDescriptor->hContext;
            }
        }
    }

    /* Get Charecters to Restrict String */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pParamNode->GetChildByName(pParamNode, COSA_XML_NODE_param_CharsToRestrict);

    AnscZeroMemory(buffer, 1024);
    uLength = 1023;
    if( ( pChildNode != NULL ) && ( ANSC_STATUS_SUCCESS == pChildNode->GetDataString(pChildNode, NULL, buffer, &uLength)) && (buffer[0] != '\0') && (0 < AnscSizeOfString(buffer)) )
    {
        pParamDesp->PreventCharecters = AnscCloneString(buffer); 
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        DslhDmagntUnloadDataModelObject
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hObjectNode,
                char*                       pParentObjName
            );

    description:

        This function is called to unload object defined in a data model.

    argument:   ANSC_HANDLE                 hThisObject,
                The instance handle;

                ANSC_HANDLE                 hObjectNode,
                The XML node of the object;

                char*                       pParentObjName
                Parent object Name;

    return:     The status of the operation

**********************************************************************/
ANSC_STATUS
DslhDmagntUnloadDataModelObject
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hObjectNode,
        char*                       pParentObjName
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pMyObject         = (PDSLH_DATAMODEL_AGENT_OBJECT)hThisObject;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)pMyObject->hDslhCpeController;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE        )pCpeController->GetDslhMprIf(pCpeController);
    PANSC_XML_DOM_NODE_OBJECT       pObjectNode       = (PANSC_XML_DOM_NODE_OBJECT)hObjectNode;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode        = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    BOOL                            bIsTable          = TRUE;
    char                            pFullName[512]    = { 0 };
    char                            pNamebuffer[512]  = { 0 };
    char                            buffer2[512]      = { 0 };
    ULONG                           uLength           = 511;
    errno_t                         rc                = -1;

    /* get the name */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_name);

    if( pChildNode == NULL || pChildNode->GetDataString(pChildNode, NULL, pNamebuffer, &uLength) != ANSC_STATUS_SUCCESS || uLength == 0)
    {
        AnscTraceWarning(("Empty object.\n"));

        return ANSC_STATUS_FAILURE;
    }

    uLength = 511;

    /* get the base object name */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_baseObjName);

    if( pChildNode != NULL && pChildNode->GetDataString(pChildNode, NULL, buffer2, &uLength) == ANSC_STATUS_SUCCESS && uLength > 0)
    {
        rc  = sprintf_s(pFullName, sizeof(pFullName), "%s%s%s.", pParentObjName, buffer2, pNamebuffer);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        rc  = sprintf_s(pFullName, sizeof(pFullName), "%s%s.", pParentObjName, pNamebuffer);
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
        }
    }

    /* get the object type */
    pChildNode = (PANSC_XML_DOM_NODE_OBJECT)
        pObjectNode->GetChildByName(pObjectNode, COSA_XML_NODE_objectType);

    AnscZeroMemory(buffer2, 512);
    uLength = 511;
    if( pChildNode == NULL || ANSC_STATUS_SUCCESS != pChildNode->GetDataString(pChildNode, NULL, buffer2, &uLength) || uLength == 0 ||
        strcmp(buffer2, COSA_XML_type_object) == 0 )
    {
        /* it's an object */
        bIsTable        = FALSE;
    }

    return pDslhMprIf->UnregisterObject(pDslhMprIf->hOwnerContext, pFullName, pNamebuffer, bIsTable);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GetParamUlongValue_X_CISCO_COM_COSADataModel
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      pUlong
            );

    description:

        This function is called to retrieve the param value in object "X_CISCO_COM_COSADataModel".

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      pUlong
                The output ULONG value;

    return:     The status of the operation

**********************************************************************/
BOOL
GetParamUlongValue_X_CISCO_COM_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PDSLH_CPE_CONTROLLER_OBJECT     pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)NULL;
    PDSLH_WMP_DATABASE_OBJECT       pWmpDatabase      = (PDSLH_WMP_DATABASE_OBJECT  )NULL;
    PDSLH_OBJ_RECORD_OBJECT         pObjRecord        = (PDSLH_OBJ_RECORD_OBJECT    )NULL;

    if (strcmp(ParamName, "DataModelLibraryNumberOfEntries") == 0)
    {
        *pUlong = GetEntryCount_COSADataModel(hInsContext);

        return TRUE;
    }

    if (strcmp(ParamName, "TotalParameterNames") == 0)
    {
        *pUlong = GetEntryCount_COSADataModel(hInsContext);

        if(!pDslhDataModelAgent)
        {
            return 0;
        }

        pCpeController    = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDataModelAgent->hDslhCpeController;
        pWmpDatabase      = (PDSLH_WMP_DATABASE_OBJECT)pCpeController->hDslhWmpDatabase;
        pObjRecord        = (PDSLH_OBJ_RECORD_OBJECT)pWmpDatabase->hRootObjRecord;

        if( pObjRecord)
        {
            * pUlong = pObjRecord->GetTotalParamNumber((ANSC_HANDLE)pObjRecord, TRUE, TRUE);
        }

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        GetEntryCount_COSADataModel
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of COSADataModel table;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
GetEntryCount_COSADataModel
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    if(!pDslhDataModelAgent)
    {
        return 0;
    }

    return AnscQueueQueryDepth(&pDslhDataModelAgent->sPluginInfoList);
}

/**********************************************************************

    prototype:

        ANSC_HANDLE
        GetEntry_COSADataModel
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                PULONG                      pInsNumber
            );

    description:

        This function is called to retrieve the COSADataModel entry specified by index;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index;

                PULONG                      pInsNumber
                The output instance number

    return:     The entry handle;

**********************************************************************/
ANSC_HANDLE
GetEntry_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        PULONG                      pInsNumber
    )
{
    PSINGLE_LINK_ENTRY              pSListEntry = NULL;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;	

    pSListEntry = 
        AnscQueueGetEntryByIndex
            (
                &pDslhDataModelAgent->sPluginInfoList,
                nIndex
            );

    if( pSListEntry == NULL)
    {
        return NULL;
    }

    *pInsNumber = nIndex + 1;

    return ACCESS_PLUGIN_INFORMATION(pSListEntry);

}

/**********************************************************************

    prototype:

        BOOL
        IsUpdated_COSADataModel
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to check whether the table is updated or not;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE;

**********************************************************************/
BOOL
IsUpdated_COSADataModel
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    return pDslhDataModelAgent->bPluginUpdated;
}

/**********************************************************************

    prototype:

        ULONG
        Synchronize_COSADataModel
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table at backend.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     the status of the operation;

**********************************************************************/
ULONG
Synchronize_COSADataModel
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    pDslhDataModelAgent->bPluginUpdated = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        GetParamUlongValue_COSADataModel
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      pUlong
            );

    description:

        This function is called to retrieve the param value in entry "COSADataModel".

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      pUlong
                The output ULONG value;

    return:     The status of the operation

**********************************************************************/
BOOL
GetParamUlongValue_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    )
{
    PPLUGIN_INFORMATION             pPluginInfo     = (PPLUGIN_INFORMATION)hInsContext;

    if (strcmp(ParamName, "Version") == 0)
    {
        *pUlong = pPluginInfo->Version;

        return TRUE;
    }
    if (strcmp(ParamName, "LoadStatus") == 0)
    {
        *pUlong = pPluginInfo->uLoadStatus;

        return TRUE;
    }

    if (strcmp(ParamName, "RegisteredObjectCount") == 0)
    {
        *pUlong = pPluginInfo->uRegObjCount;

        return TRUE;
    }

    if (strcmp(ParamName, "RegisteredParameterCount") == 0)
    {
        *pUlong = pPluginInfo->uRegParamCount;

        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        int
        GetParamStringValue_COSADataModel
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve the param string value in entry "COSADataModel".

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue
                The string buffer provided;

                ULONG*                      pUlSize
                The size of the buffer. If it's not big enough, required size will be set here;

    return:     0  - Get string value succeeded;
                1  - Buffer size is not big enough, required size is returned;
                -1 - Failed to get string value. Param name may be incorrect or unsupported.

**********************************************************************/
int
GetParamStringValue_COSADataModel
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PPLUGIN_INFORMATION             pPluginInfo     = (PPLUGIN_INFORMATION)hInsContext;
    char*                           pStringValue    = NULL;
    ULONG                           uLength         = 0;
    errno_t                         rc              = -1;

    if (strcmp(ParamName, "ModuleName") == 0)
    {
        pStringValue = pPluginInfo->ModuleName;
    }
    else if (strcmp(ParamName, "Author") == 0)
    {
        pStringValue = pPluginInfo->Author;
    }
    else if (strcmp(ParamName, "Description") == 0)
    {
        pStringValue = pPluginInfo->Description;
    }
    else if( strcmp(ParamName, "Source") == 0 || strcmp(ParamName, "LibraryPath") == 0 )
    {
        pStringValue = pPluginInfo->SourceName;
    }
    else
    {
        return -1;
    }

    if( pStringValue == NULL || AnscSizeOfString(pStringValue) == 0)
    {
        *pUlSize  = 0;
        pValue[0] = 0;

        return 0;
    }

    uLength = AnscSizeOfString(pStringValue);

    if( *pUlSize <= uLength)
    {
        *pUlSize = uLength + 1;

        return 1;
    }

    rc = strcpy_s(pValue, *pUlSize, pStringValue);
    if (rc != EOK)
    {
        ERR_CHK(rc);
        return -1;
    }

    *pUlSize = uLength;

    return 0;
}

#if 0
/***********************************************************************

 APIs for Object:

    DeviceInfo.SupportedDataModel.{i}.

    *  SupportedDataModel_GetEntryCount
    *  SupportedDataModel_GetEntry
    *  SupportedDataModel_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SupportedDataModel_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
SupportedDataModel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return 1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        SupportedDataModel_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
SupportedDataModel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    *pInsNumber  = nIndex + 1; 
    return (ANSC_HANDLE)1; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        SupportedDataModel_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
SupportedDataModel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController= (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDataModelAgent->hDslhCpeController;
    char*                           pConnRequestUrl   = NULL;
    char*                           pTempBuffer       = NULL;
    char                            pUrlValue[128]    = { 0 };

#if 0
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "URL") == 0)
    {
        /* collect value */
        if( pDslhCfgIf)
        {
            pConnRequestUrl  = pDslhCfgIf->GetConnRequestUrl(pDslhCfgIf->hOwnerContext);

            if ( pConnRequestUrl && _ansc_strstr(pConnRequestUrl, "http://") == pConnRequestUrl)
            {
                /* find the root path and append my path */
                pTempBuffer = (char*)(pConnRequestUrl + AnscSizeOfString("http://"));
                pTempBuffer = _ansc_strstr(pTempBuffer, "/");

                if( pTempBuffer == NULL)
                {
                    _ansc_sprintf(pValue, "%s%s", pConnRequestUrl, _DATA_MODEL_XML_PATH);
                }
                else
                {
                    AnscCopyMemory(pValue, pConnRequestUrl, (ULONG)(pTempBuffer - pConnRequestUrl));
                    _ansc_strcat(pValue, _DATA_MODEL_XML_PATH);
                }

                *pUlSize = AnscSizeOfString(pValue);

                AnscFreeMemory(pConnRequestUrl);
            }
        }

        return 0;
    }

    if (strcmp(ParamName, "URN") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, "urn:broadband-forum-org:tr-157-1-0-0");

        return 0;
    }

    if (strcmp(ParamName, "Features") == 0)
    {
        /* collect value */
        AnscCopyString(pValue, "NAT,Firewall");

        return 0;
    }

#endif
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

#endif

/***********************************************************************

 APIs for CCSP internal base data model:

    com.cisco.spvtg.ccsp.XXX.

    *  CcspComp_GetParamUlongValue 
    *  CcspComp_GetParamStringValue 
    *  CcspComp_GetBulkParamValues
    *  CcspComp_SetParamBoolValue
    *  CcspLog_GetParamBoolValue
    *  CcspLog_GetParamUlongValue
    *  CcspLog_SetParamBoolValue
    *  CcspLog_SetParamUlongValue
    *  CcspLog_Validate
    *  CcspLog_Commit
    *  CcspLog_Rollback
    *  CcspMem_GetParamUlongValue 
    *  CcspLog_GetBulkParamValues
    *  CcspLog_SetBulkParamValues

***********************************************************************/
BOOL
CcspComp_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;

    if( pCcspCcdIf == NULL)
    {
        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Version") == 0)
    {
        *puLong = pCcspCcdIf->GetComponentVersion(NULL);

        return TRUE;
    }

    if (strcmp(ParamName, "State") == 0)
    {
        *puLong = pCcspCcdIf->GetComponentState(NULL);

        return TRUE;
    }

    if (strcmp(ParamName, "Health") == 0)
    {
        *puLong = pCcspCcdIf->GetComponentHealth(NULL);

        return TRUE;
    }

    return FALSE;
}

ULONG
CcspComp_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;
    ULONG                           uSize           = 0;
    errno_t                         rc              = -1;

    if( pCcspCcdIf == NULL)
    {
        return -1;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Name") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pCcspCcdIf->GetComponentName(NULL));
        if ( rc != EOK )
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "Author") == 0)
    {
        rc = strcpy_s(pValue, *pUlSize, pCcspCcdIf->GetComponentAuthor(NULL));
        if ( rc != EOK )
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }

    if (strcmp(ParamName, "DTXml") == 0)
    {
        if( !pDslhDataModelAgent->pDTXml)
        {
            *pUlSize = 0;

            return 0;
        }

        uSize = AnscSizeOfString(pDslhDataModelAgent->pDTXml);

        if( *pUlSize <= uSize)
        {
            *pUlSize = uSize + 1;

            return 1;
        }

        rc = strcpy_s(pValue, *pUlSize, pDslhDataModelAgent->pDTXml);
        if ( rc != EOK )
        {
            ERR_CHK(rc);
            return -1;
        }

        return 0;
    }

    return -1;
}

BOOL
CcspComp_GetBulkParamValues
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;
    ULONG                           i               = 0;
    char*                           pName           = NULL;

    AnscTrace("'CcspComp_GetBulkParamValue' was called.\n");

    for( i = 0; i < ulArraySize; i ++)
    {
        pName = pParamArray[i];

        if( !pName || !ppVarArray[i])
        {
            return FALSE;
        }

        if ( strcmp(pName, "Version") == 0 )
        {
            ppVarArray[i]->Variant.varUint32 = pCcspCcdIf->GetComponentVersion(NULL);
        }
        else if ( strcmp(pName, "State") == 0 )
        {
            ppVarArray[i]->Variant.varUint32 = pCcspCcdIf->GetComponentState(NULL);
        }
        else if ( strcmp(pName, "Health") == 0 )
        {
            ppVarArray[i]->Variant.varUint32 = pCcspCcdIf->GetComponentHealth(NULL);
        }
        else if( strcmp(pName, "LogLevel") == 0 )
        {
            ppVarArray[i]->Variant.varUint32 = pCcspCcdIf->GetLoggingLevel(NULL);
        }
        else if ( strcmp(pName, "Name") == 0 )
        {
            ppVarArray[i]->Variant.varString = AnscCloneString(pCcspCcdIf->GetComponentName(NULL));
        }
        else if ( strcmp(pName, "Author") == 0 )
        {
            ppVarArray[i]->Variant.varString = AnscCloneString(pCcspCcdIf->GetComponentAuthor(NULL));
        }
        else if ( strcmp(pName, "DTXml") == 0 )
        {
            if( NULL == pDslhDataModelAgent->pDTXml)
            {
                 pDslhDataModelAgent->GenerateDTXmlString(pDslhDataModelAgent);
            }    
            if( pDslhDataModelAgent->pDTXml)
            {
                ppVarArray[i]->Variant.varString = AnscCloneString(pDslhDataModelAgent->pDTXml);
            }
        }
        else
        {
            AnscTrace("Unknown param name '%s'\n", pName);

            return FALSE;
        }
    }

    return TRUE;
}

BOOL
CcspComp_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PDSLH_DATAMODEL_AGENT_OBJECT    agent   = pDslhDataModelAgent;
    PCCSP_CCD_INTERFACE             ccdif   = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;
    PDSLH_CPE_CONTROLLER_OBJECT     cpectl  = (PDSLH_CPE_CONTROLLER_OBJECT)pDslhDataModelAgent->hDslhCpeController;
    char                            crName[128], compName[128], compPath[128], *cp;
    char                            *comp;
    ULONG                           nsCnt;
    name_spaceType_t                *nsArr = (name_spaceType_t *)cpectl->hParameterArray;
    int                             err;

    if (strcmp(ParamName, "Register") == 0 && bValue == TRUE)
    {
        comp = ccdif->GetComponentName(NULL);
        fprintf(stderr, "ccdif->GetComponentName %s\n", comp);

        if (agent->RegBaseDataModel(agent, comp) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s: RegBaseDataModel error for %s!!\n", __FUNCTION__, comp));
            fprintf(stderr, "%s: RegBaseDataModel error for %s!!\n", __FUNCTION__, comp);
            return FALSE;
        }

        snprintf(crName, sizeof(crName), "%s%s", agent->pPrefix ? agent->pPrefix : "", "com.cisco.spvtg.ccsp.CR");
        snprintf(compName, sizeof(compName), "%s%s", agent->pPrefix ? agent->pPrefix : "", comp);

        fprintf(stderr, "agent->Prefix %s\n", agent->pPrefix);
        fprintf(stderr, "crName %s\n", crName);
        fprintf(stderr, "compName %s\n", compName);

        if (cpectl->hParameterArray && cpectl->uParameterCount) {
            nsCnt = 0;
            nsArr = (name_spaceType_t *)cpectl->hParameterArray;
            while (nsCnt < cpectl->uParameterCount && nsArr[nsCnt].name_space)
            {
                fprintf(stderr, " [%lu] %s\n", nsCnt, nsArr[nsCnt].name_space);
                nsCnt++;
            }

            snprintf(compPath, sizeof(compPath), "/%s", comp);
            for (cp = &compPath[1]; cp < &compPath[sizeof(compPath)] && *cp != '\0'; cp++)
                if (*cp == '.')
                    *cp = '/';

            fprintf(stderr, "cpectl->hParameterArray %p\n", cpectl->hParameterArray);
            fprintf(stderr, "cpectl->uParameterCount %lu\n", cpectl->uParameterCount);
            fprintf(stderr, "compPath %s\n", compPath);

            if ((err = CcspBaseIf_registerCapabilities(
                        cpectl->hDbusHandle, 
                        crName, 
                        compName, 
                        ccdif->GetComponentVersion(NULL),
                        compPath,
                        agent->pPrefix,
                        cpectl->hParameterArray,
                        nsCnt)) != CCSP_SUCCESS) {
                AnscTraceError(("%s: fail to register DM for %s: %d!!\n", __FUNCTION__, compName, err));
                fprintf(stderr, "%s: fail to register DM for %s: %d!!\n", __FUNCTION__, compName, err);
                return FALSE;
            }
        }

        fprintf(stderr, "%s: DM for %s OK !\n", __FUNCTION__, compName);
        return TRUE;
    }

    fprintf(stderr, "%s: Unknow ParamName %s\n", __FUNCTION__, ParamName);
    return FALSE;
}

BOOL
CcspLog_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;

    if( pCcspCcdIf == NULL)
    {
        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
         *pBool = pCcspCcdIf->GetLoggingEnabled(NULL);

        return TRUE;
    }

    return FALSE;
}

BOOL
CcspLog_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;

    if( pCcspCcdIf == NULL)
    {
        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "LogLevel") == 0)
    {
        *puLong = pCcspCcdIf->GetLoggingLevel(NULL);

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
CcspLog_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf     = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;
    ANSC_STATUS                     returnStatus   = 0;

    if( pCcspCcdIf == NULL)
    {
        return FALSE;
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "Enable") == 0)
    {
        returnStatus = 
            pCcspCcdIf->SetLoggingEnabled
                (
                    NULL, 
                    bValue
                );

        return ( returnStatus == ANSC_STATUS_SUCCESS ) ? TRUE : FALSE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


BOOL
CcspLog_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf     = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;
    ANSC_STATUS                     returnStatus   = 0;

    if( pCcspCcdIf == NULL)
    {
        return FALSE;
    }

    /* check the parameter name and set the corresponding value */
    if (strcmp(ParamName, "LogLevel") == 0)
    {
        returnStatus = 
            pCcspCcdIf->SetLoggingLevel
                (
                    NULL, 
                    uValue
                );

        return ( returnStatus == ANSC_STATUS_SUCCESS ) ? TRUE : FALSE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
CcspLog_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

ULONG
CcspLog_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;

    if( pCcspCcdIf == NULL)
    {
        return 0;
    }

    return pCcspCcdIf->ApplyChanges(NULL);

}

ULONG
CcspLog_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    return 0;
}

BOOL
CcspMem_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;

    if( pCcspCcdIf == NULL)
    {
        return FALSE;
    }

    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "MinUsage") == 0)
    {
        *puLong = pCcspCcdIf->GetMemMinUsage(NULL);

        return TRUE;
    }

    if (strcmp(ParamName, "MaxUsage") == 0)
    {
        *puLong = pCcspCcdIf->GetMemMaxUsage(NULL);

        return TRUE;
    }

    if (strcmp(ParamName, "Consumed") == 0)
    {
        *puLong = pCcspCcdIf->GetMemConsumed(NULL);

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

BOOL
CcspLog_GetBulkParamValues
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;
    ULONG                           i               = 0;
    char*                           pName           = NULL;

    AnscTrace("'CcspLog_GetBulkParamValue' was called.\n");

    for( i = 0; i < ulArraySize; i ++)
    {
        pName = pParamArray[i];

        if( !pName || !ppVarArray[i])
        {
            return FALSE;
        }

        if( strcmp(pName, "Enable") == 0 )
        {
            ppVarArray[i]->Variant.varBool = pCcspCcdIf->GetLoggingEnabled(NULL);

        }
        else if( strcmp(pName, "LogLevel") == 0 )
        {
            ppVarArray[i]->Variant.varUint32 = pCcspCcdIf->GetLoggingLevel(NULL);
        }
        else
        {
            AnscTrace("Unknown param name '%s'\n", pName);

            return FALSE;
        }
    }

    return TRUE;
}

BOOL
CcspLog_SetBulkParamValues
    (
        ANSC_HANDLE                 hInsContext,
        char**                      pParamArray,
        PSLAP_VARIABLE*             ppVarArray,
        ULONG                       ulArraySize,
        PULONG                      pulErrorIndex
    )
{
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent = (PDSLH_DATAMODEL_AGENT_OBJECT)hInsContext;
    PCCSP_CCD_INTERFACE             pCcspCcdIf      = (PCCSP_CCD_INTERFACE)pDslhDataModelAgent->hDslhCcdIf;
    ULONG                           i               = 0;
    char*                           pName           = NULL;

    AnscTrace("'CcspLog_SetBulkParamValue' was called.\n");

    for( i = 0; i < ulArraySize; i ++)
    {
        pName = pParamArray[i];

        if( !pName || !ppVarArray[i])
        {
            return FALSE;
        }

        if( strcmp(pName, "Enable") == 0 )
        {
            pCcspCcdIf->SetLoggingEnabled
                (
                    NULL, 
                    ppVarArray[i]->Variant.varBool
                );
        }
        else if( strcmp(pName, "LogLevel") == 0 )
        {
            pCcspCcdIf->SetLoggingLevel
                (
                    NULL, 
                    ppVarArray[i]->Variant.varUint32
                );
        }
        else
        {
            AnscTrace("Unsupported param name '%s'\n", pName);

            *pulErrorIndex = i;

            return FALSE;
        }
    }

    return TRUE;
}
