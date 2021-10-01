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

    module: ccsp_component_helper.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the helper functions for CCSP components:

        *   CcspComponentLoadCfg
        *   CcspComponentLoadDmXmlList

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        1/08/2010    initial revision.

**********************************************************************/

#include "ansc_platform.h"

#include "ansc_xml_dom_parser_interface.h"
#include "ansc_xml_dom_parser_external_api.h"
#include "ansc_xml_dom_parser_status.h"

#include "ccsp_component_helper.h"


/**********************************************************************

    function:   CcspComponentLoadCfg

    description:

        This function is called to load the configuration for the 
        component.

    argument:   
        const char*                 pCfgFileName
            Specifies the component configuration file

        PCCSP_COMPONENT_CFG         pCfg
            Returns the configuration loaded

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CcspComponentLoadCfg
    (
        char*                       pCfgFileName,
        PCCSP_COMPONENT_CFG         pCfg
    )
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     pFileHandle        = NULL;
    char*                           pXMLContent        = NULL;
    ULONG                           uFileLength        = 0;
    ULONG                           uBufferSize        = 0;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode          = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode         = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PCHAR                           pBackBuffer        = NULL;
    ULONG                           uLength            = 0; 
    
    /* load from the file */
    pFileHandle =
        AnscOpenFile
            (
                pCfgFileName,
                ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
                ANSC_FILE_S_IREAD
            );

    if( pFileHandle == NULL)
    {
        AnscTraceWarning
            ((
                "CcspComponentLoadCfg -- failed to open component configuration file %s\n",
                pCfgFileName
            ));

        return ANSC_STATUS_FAILURE;
    }
    else
    {
        uFileLength = AnscGetFileSize(pFileHandle);

        pXMLContent = (char*)AnscAllocateMemory( uFileLength + 8);

        if (pXMLContent == NULL)
        {
            /*RDKB-5654, CID-24069, Resource leak*/
            AnscCloseFile(pFileHandle);
            return ANSC_STATUS_RESOURCES;
        }

        uBufferSize = uFileLength + 8;
    }

    if ( AnscReadFile( pFileHandle, pXMLContent, &uBufferSize) != ANSC_STATUS_SUCCESS )
    {
        AnscFreeMemory(pXMLContent);

        AnscTraceWarning
            ((
                "CcspComponentLoadCfg -- failed to read file %s\n",
                pCfgFileName
            ));
        /*RDKB-5654, CID-24069, Resource leak*/
        AnscCloseFile(pFileHandle);
        return ANSC_STATUS_FAILURE;
    }

    AnscCloseFile(pFileHandle);

    /*
     *  Parse the XML content
     */
    pBackBuffer = pXMLContent;

    /*CID: 137197 String not null terminated*/
    pBackBuffer[uBufferSize-1] = '\0';

    pRootNode   = (PANSC_XML_DOM_NODE_OBJECT)
        AnscXmlDomParseString
            (
                (ANSC_HANDLE)NULL,
                (PCHAR*)&pBackBuffer,
                uBufferSize
            );

    if ( pRootNode == NULL )
    {
        AnscTraceWarning(("Failed to parse the retrieve the root note!\n"));

        returnStatus = ANSC_STATUS_FAILURE;

        goto EXIT;
    }
    else
    {
        /* ID */
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)pRootNode->GetChildByName(pRootNode, CCSP_COMP_XML_NODE_Id);

        if( pChildNode == NULL)
        {
            AnscTraceWarning(("Failed to parse " CCSP_COMP_XML_NODE_Id "!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT;
        }
        else
        {
            uLength = sizeof(pCfg->ComponentId);

            returnStatus = pChildNode->GetDataString(pChildNode, NULL, pCfg->ComponentId, &uLength);
            
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT;
            }
        }

        /* Name */
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)pRootNode->GetChildByName(pRootNode, CCSP_COMP_XML_NODE_Name);

        if( pChildNode == NULL)
        {
            AnscTraceWarning(("Failed to parse " CCSP_COMP_XML_NODE_Name "!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT;
        }
        else
        {
            uLength = sizeof(pCfg->ComponentName);

            returnStatus = pChildNode->GetDataString(pChildNode, NULL, pCfg->ComponentName, &uLength);
            
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT;
            }
        }

        /* Version */
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)pRootNode->GetChildByName(pRootNode, CCSP_COMP_XML_NODE_Version);

        if( pChildNode == NULL)
        {
            AnscTraceWarning(("Failed to parse " CCSP_COMP_XML_NODE_Version "!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT;
        }
        else
        {
            returnStatus = pChildNode->GetDataUlong(pChildNode, NULL, &pCfg->Version);
            
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT;
            }
        }

        /* DbusPath */
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)pRootNode->GetChildByName(pRootNode, CCSP_COMP_XML_NODE_DbusPath);

        if( pChildNode == NULL)
        {
            AnscTraceWarning(("Failed to parse " CCSP_COMP_XML_NODE_DbusPath "!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT;
        }
        else
        {
            uLength = sizeof(pCfg->DbusPath);

            returnStatus = pChildNode->GetDataString(pChildNode, NULL, pCfg->DbusPath, &uLength);
            
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT;
            }
        }

        /* DataModelXmlCfg */
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)pRootNode->GetChildByName(pRootNode, CCSP_COMP_XML_NODE_DataModelXmlCfg);

        if( pChildNode == NULL)
        {
            AnscTraceWarning(("Failed to parse " CCSP_COMP_XML_NODE_DataModelXmlCfg "!\n"));

            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT;
        }
        else
        {
            uLength = sizeof(pCfg->DmXmlCfgFileName);

            returnStatus = pChildNode->GetDataString(pChildNode, NULL, pCfg->DmXmlCfgFileName, &uLength);
            
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT;
            }
        }
        
        AnscTraceWarning
            ((
                "Component Configuration:\n\tID=%s\n\tName=%s\n\tVersion=%lu\n\tDbusPath=%s\n\tXmlCfgFile=%s\n",
                pCfg->ComponentId,
                pCfg->ComponentName,
                pCfg->Version,
                pCfg->DbusPath,
                pCfg->DmXmlCfgFileName
            ));
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:

    if( pRootNode != NULL)
    {
        AnscXmlDomNodeRemove(pRootNode);
    }
    AnscFreeMemory(pXMLContent);

    return  returnStatus;
}


/**********************************************************************

    function:   CcspComponentLoadDmXmlList

    description:

        This function is called to load the data model XML file list
        from the specified data model XML configuration file.

    argument:   
        const char*                 pDmXmlCfgFileName
            Specifies the data model XML configuration file

        PCCSP_DM_XML_CFG_LIST*      ppXmlCfgList
            Returns the read data model XML file list.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
CcspComponentLoadDmXmlList
    (
        char*                       pDmXmlCfgFileName,
        PCCSP_DM_XML_CFG_LIST*      ppXmlCfgList
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    ANSC_HANDLE                     pFileHandle     = NULL;
    char*                           pXMLContent     = NULL;
    ULONG                           uFileLength     = 0;
    ULONG                           uBufferSize     = 0;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode       = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode      = (PANSC_XML_DOM_NODE_OBJECT)NULL;
    PCHAR                           pBackBuffer     = NULL;
    ULONG                           uLength         = 0;
    ULONG                           ulCount         = 0;
    PCCSP_DM_XML_CFG_LIST           pXmlCfgList     = NULL;

    /* load from the file */
    pFileHandle =
        AnscOpenFile
            (
                pDmXmlCfgFileName,
                ANSC_FILE_O_BINARY | ANSC_FILE_O_RDONLY,
                ANSC_FILE_S_IREAD
            );

    if( pFileHandle == NULL)
    {
        AnscTraceWarning
            ((
                "CcspComponentLoadDmXmlList -- failed to open data model XML configuration file %s\n",
                pDmXmlCfgFileName
            ));

        return ANSC_STATUS_FAILURE;
    }
    else
    {
        uFileLength = AnscGetFileSize(pFileHandle);

        pXMLContent = (char*)AnscAllocateMemory( uFileLength + 8);

        if (pXMLContent == NULL)
        {
            /*RDKB-5654, CID-24376, Resource leak*/
            AnscCloseFile(pFileHandle);
            return ANSC_STATUS_RESOURCES;
        }

        uBufferSize = uFileLength + 8;
    }

    if ( AnscReadFile(pFileHandle, pXMLContent, &uBufferSize) != ANSC_STATUS_SUCCESS)
    {
        AnscFreeMemory(pXMLContent);

        AnscTraceWarning
            ((
                "CcspComponentLoadDmXmlList -- failed to read file %s\n",
                pDmXmlCfgFileName
            ));

        /*RDKB-5654, CID-24376, Resource leak*/
        AnscCloseFile(pFileHandle);
        return ANSC_STATUS_FAILURE;
    }

    AnscCloseFile(pFileHandle);

    /*
     *  Parse the XML content
     */
    pBackBuffer = pXMLContent;

    /*CID: 137623 String not null terminated*/
    pBackBuffer[uBufferSize-1] = '\0';

    pRootNode   = (PANSC_XML_DOM_NODE_OBJECT)
        AnscXmlDomParseString
            (
                (ANSC_HANDLE)NULL,
                (PCHAR*)&pBackBuffer,
                uBufferSize
            );

    if ( pRootNode == NULL )
    {
        AnscTraceWarning(("Failed to parse the retrieve the root note!\n"));

        returnStatus = ANSC_STATUS_FAILURE;

        goto EXIT;
    }
    else
    {
        /* Get the count */
        pChildNode = pRootNode->GetHeadChild(pRootNode);
        ulCount    = 0;

        while( pChildNode != NULL)
        {
            ulCount++;

            /* goto next one */
            pChildNode = pRootNode->GetNextChild(pRootNode, pChildNode);
        }

        /* Allocate the memory block */
        pXmlCfgList = (PCCSP_DM_XML_CFG_LIST)
            AnscAllocateMemory
                (
                    (sizeof(CCSP_DM_XML_CFG_LIST) + sizeof(pXmlCfgList->FileList[0]) * ulCount)
                );

        if ( pXmlCfgList == NULL )
        {
            returnStatus = ANSC_STATUS_RESOURCES;
            goto  EXIT;
        }

        /* Read all nodes */
        pChildNode = pRootNode->GetHeadChild(pRootNode);
        ulCount    = 0;

        while(pChildNode != NULL)
        {
            uLength = sizeof(pXmlCfgList->FileList[0]);

            returnStatus = pChildNode->GetDataString(pChildNode, NULL, pXmlCfgList->FileList[ulCount], &uLength);
            
            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                goto  EXIT;
            }
            else
            {
                AnscTraceWarning(("Data model XML file %lu = %s\n", ulCount+1, pXmlCfgList->FileList[ulCount]));
                ulCount++;

                /* goto next one */
                pChildNode = pRootNode->GetNextChild(pRootNode, pChildNode);
            }
        }
        
        pXmlCfgList->ulFileCount = ulCount;
        *ppXmlCfgList = pXmlCfgList;        
    }

    returnStatus = ANSC_STATUS_SUCCESS;

EXIT:
    if ( pXmlCfgList && (returnStatus != ANSC_STATUS_SUCCESS))
    {
        AnscFreeMemory(pXmlCfgList);
    }

    if( pRootNode != NULL)
    {
        AnscXmlDomNodeRemove(pRootNode);
    }

    AnscFreeMemory(pXMLContent);

    return  returnStatus;
}

