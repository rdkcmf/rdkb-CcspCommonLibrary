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

    module:	ansc_xml_dom_node_base.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic construction and removal
        functions of the General Xml Node Object.

        *   AnscXmlDomNodeCreate
        *   AnscXmlDomNodeRemove
        *   AnscXmlDomNodeReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu     (Original)
        Kang Quan   (Porting)

    ---------------------------------------------------------------

    revision:

        03/11/02    initial revision.

**********************************************************************/

#define  CHILD_NODE_COOKIE                  0x11223344
#define  ATTR_NODE_COOKIE                   0x99887766

#include "ansc_xml_dom_parser_global.h"

#define  AnscXmlWarning(arg...)             fprintf(stderr, arg)

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeCreate
            (
                ANSC_HANDLE                 hOwnerContext
            );

    description:

        This function constructs the General Xml Node Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

    return:     newly created socket object.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeCreate
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = NULL;

    pXmlNode = (PANSC_XML_DOM_NODE_OBJECT)AnscAllocateMemory(sizeof(ANSC_XML_DOM_NODE_OBJECT) + 4);

    if ( !pXmlNode )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pXmlNode->hOwnerContext   = hOwnerContext;
    pXmlNode->hParentNode     = (ANSC_HANDLE)NULL;

    pXmlNode->Create          = AnscXmlDomNodeCreate;
    pXmlNode->Remove          = AnscXmlDomNodeRemove;
    pXmlNode->Reset           = AnscXmlDomNodeReset;

    pXmlNode->GetStatus       = AnscXmlDomNodeGetStatus;
    pXmlNode->GetEncodedSize  = AnscXmlDomNodeGetEncodedSize;
    pXmlNode->Encode          = AnscXmlDomNodeEncode;
    pXmlNode->Decode          = AnscXmlDomNodeDecode;

    pXmlNode->GetName         = AnscXmlDomNodeGetName;
    pXmlNode->SetName         = AnscXmlDomNodeSetName;
    pXmlNode->GetParentNode   = AnscXmlDomNodeGetParentNode;
    pXmlNode->SetParentNode   = AnscXmlDomNodeSetParentNode;

    pXmlNode->AddChild        = AnscXmlDomNodeAddChild;
    pXmlNode->AddChildByName  = AnscXmlDomNodeAddChildByName;
    pXmlNode->DelChild        = AnscXmlDomNodeDelChild;
    pXmlNode->DelChildByName  = AnscXmlDomNodeDelChildByName;
    pXmlNode->InsertChild     = AnscXmlDomNodeInsertChild;
    pXmlNode->RemoveChild     = AnscXmlDomNodeRemoveChild;

    pXmlNode->GetChildByName  = AnscXmlDomNodeGetChildByName;
    pXmlNode->GetHeadChild    = AnscXmlDomNodeGetHeadChild;
    pXmlNode->GetNextChild    = AnscXmlDomNodeGetNextChild;
    pXmlNode->GetTailChild    = AnscXmlDomNodeGetTailChild;

    pXmlNode->GetItemString   = AnscXmlDomNodeGetItemString;
    pXmlNode->GetItemUlong    = AnscXmlDomNodeGetItemUlong;
    pXmlNode->GetItemBoolean  = AnscXmlDomNodeGetItemBoolean;
    pXmlNode->GetItemBinary   = AnscXmlDomNodeGetItemBinary;
    pXmlNode->GetItemSize     = AnscXmlDomNodeGetItemSize;

    pXmlNode->GetAttrString   = AnscXmlDomNodeGetAttrString;
    pXmlNode->GetAttrUlong    = AnscXmlDomNodeGetAttrUlong;
    pXmlNode->GetAttrBoolean  = AnscXmlDomNodeGetAttrBoolean;
    pXmlNode->GetAttrBinary   = AnscXmlDomNodeGetAttrBinary;
    pXmlNode->GetAttrSize     = AnscXmlDomNodeGetAttrSize;

    pXmlNode->GetDataString   = AnscXmlDomNodeGetDataString;
    pXmlNode->GetDataUlong    = AnscXmlDomNodeGetDataUlong;
    pXmlNode->GetDataLong     = AnscXmlDomNodeGetDataLong;
    pXmlNode->GetDataBoolean  = AnscXmlDomNodeGetDataBoolean;
    pXmlNode->GetDataBinary   = AnscXmlDomNodeGetDataBinary;
    pXmlNode->GetDataSize     = AnscXmlDomNodeGetDataSize;

    pXmlNode->SetAttrString   = AnscXmlDomNodeSetAttrString;
    pXmlNode->SetAttrUlong    = AnscXmlDomNodeSetAttrUlong;
    pXmlNode->SetAttrBoolean  = AnscXmlDomNodeSetAttrBoolean;
    pXmlNode->SetAttrBinary   = AnscXmlDomNodeSetAttrBinary;

    pXmlNode->SetDataString   = AnscXmlDomNodeSetDataString;
    pXmlNode->SetDataUlong    = AnscXmlDomNodeSetDataUlong;
    pXmlNode->SetDataBoolean  = AnscXmlDomNodeSetDataBoolean;
    pXmlNode->SetDataBinary   = AnscXmlDomNodeSetDataBinary;

/*
    pXmlNode->GetAttr         = AnscXmlDomNodeGetAttr;
    pXmlNode->Copy            = AnscXmlDomNodeCopy;
    pXmlNode->GetFirstAttr    = AnscXmlDomNodeGetFirstAttr;
    pXmlNode->GetNextAttr     = AnscXmlDomNodeGetNextAttr;
    pXmlNode->GetLevel        = AnscXmlDomNodeGetLevel;
    pXmlNode->SetLevel        = AnscXmlDomNodeSetLevel;
    pXmlNode->EnumChild       = AnscXmlDomNodeEnumChild;
*/

    /* initialize child node queue and the lock on it */
    AnscQueueInitializeHeader(&pXmlNode->ChildNodeQueue);
    AnscInitializeXmlLock(&pXmlNode->ChildNodeQueueLock);
    pXmlNode->ChildNodeQueueLock = CHILD_NODE_COOKIE;

    /* initialize attribute queue and the lock on it */
    AnscQueueInitializeHeader(&pXmlNode->AttributesList);
    AnscInitializeXmlLock(&pXmlNode->AttributesListLock);
    pXmlNode->AttributesListLock = ATTR_NODE_COOKIE;

    return  (ANSC_HANDLE)pXmlNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeRemove
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
AnscXmlDomNodeRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_ATTRIBUTE             pNameAttr     = NULL;
    PANSC_XML_ATTRIBUTE             pSonAttribute = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    /* sanity check */
    if( pXmlNode->ChildNodeQueueLock != CHILD_NODE_COOKIE)
    {
        pNameAttr = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetFirstAttr(pXmlNode);

        if( pNameAttr == NULL || pNameAttr->StringData == NULL)
        {
            AnscXmlWarning
                (
                    "The child node queue of XML node is corrupted. cookie1= 0x%.8X\n",
                    (unsigned int)pXmlNode->ChildNodeQueueLock
                );
        }
        else
        {
            AnscXmlWarning
                (
#ifdef _64BIT_ARCH_SUPPORT_
                    "The child node queue of XML node '%s' is corrupted.cookie1= %p\n",
#else
                    "The child node queue of XML node '%s' is corrupted.cookie1= 0x%.8X\n",
#endif
                    (char *)pXmlNode->ChildNodeQueueLock,
#ifdef _64BIT_ARCH_SUPPORT_
                    (void*)pNameAttr->StringData
#else
                    (unsigned int)pNameAttr->StringData
#endif
                );
        }

        return ANSC_STATUS_FAILURE;
    }

    if( pXmlNode->AttributesListLock != ATTR_NODE_COOKIE)
    {
        pNameAttr = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetFirstAttr(pXmlNode);

        if( pNameAttr == NULL || pNameAttr->StringData == NULL)
        {
            AnscXmlWarning
                (
#ifdef _64BIT_ARCH_SUPPORT_
                    "The child node queue of XML node is corrupted. cookie2= %p\n",
                    (void*)pXmlNode->AttributesListLock
#else
                    "The child node queue of XML node is corrupted. cookie2= 0x%.8X\n",
                    (unsigned int)pXmlNode->AttributesListLock
#endif
                );
        }
        else
        {
            AnscXmlWarning
                (
#ifdef _64BIT_ARCH_SUPPORT_
                    "The child node queue of XML node '%s' is corrupted.cookie2= %p\n",
#else
                    "The child node queue of XML node '%s' is corrupted.cookie2= 0x%.8X\n",
#endif
                    (char *)pXmlNode->AttributesListLock,
#ifdef _64BIT_ARCH_SUPPORT_
                    (void*)pNameAttr->StringData
#else
                    (unsigned int)pNameAttr->StringData
#endif
                );
        }

        return ANSC_STATUS_FAILURE;
    }

    /* release the value */
    if (pXmlNode->StringData)
    {
        AnscFreeMemory(pXmlNode->StringData);
    }

    /* remove all the child nodes */
    AnscXmlDomNodeReset((ANSC_HANDLE)pXmlNode);
    AnscFreeXmlLock  (&pXmlNode->ChildNodeQueueLock);

    /*
     * loop to free all the children attributes
     */
    while (TRUE)
    {
        AnscAcquireXmlLock(&pXmlNode->AttributesListLock);
        pSLinkEntry = AnscQueuePopEntry(&pXmlNode->AttributesList);
        AnscReleaseXmlLock(&pXmlNode->AttributesListLock);

        if (!pSLinkEntry)
        {
            /* no more entry */
            break;
        }

        pSonAttribute   = ACCESS_XML_ATTRIBUTE(pSLinkEntry);

        if (pSonAttribute->StringData)
        {
            AnscFreeMemory(pSonAttribute->StringData);
        }

        AnscFreeMemory(pSonAttribute);
    }

    AnscFreeXmlLock  (&pXmlNode->AttributesListLock);

    /* To capture double-free error, we reset the cookies here before memory free. */
    pXmlNode->ChildNodeQueueLock = CHILD_NODE_COOKIE;
    pXmlNode->AttributesListLock = ATTR_NODE_COOKIE;

    AnscFreeMemory(pXmlNode);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function resets the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;

    while ( (pChildNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeRemoveChild((ANSC_HANDLE)pXmlNode)) )
    {
    	AnscXmlDomNodeRemove((ANSC_HANDLE)pChildNode);
    }

    return  ANSC_STATUS_SUCCESS;
}
