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

    module: ansc_xml_dom_node_management.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xml Node Object.

        *   AnscXmlDomNodeGetName
        *   AnscXmlDomNodeSetName
        *   AnscXmlDomNodeGetParentNode
        *   AnscXmlDomNodeSetParentNode
        *   AnscXmlDomNodeAddChild
        *   AnscXmlDomNodeAddChildByName
        *   AnscXmlDomNodeDelChild
        *   AnscXmlDomNodeDelChildByName
        *   AnscXmlDomNodeInsertChild
        *   AnscXmlDomNodeRemoveChild
        *   AnscXmlDomNodeGetChildByName
        *   AnscXmlDomNodeGetHeadChild
        *   AnscXmlDomNodeGetNextChild
        *   AnscXmlDomNodeGetTailChild
        *   AnscXmlDomNodeGetAttr
        *   AnscXmlDomNodeCopy
        *   AnscXmlDomNodeGetFirstAttr
        *   AnscXmlDomNodeGetNextAttr

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

#include "ansc_xml_dom_parser_global.h"
#include "safec_lib_common.h"

#define  AnscXmlWarning(arg...)             fprintf(stderr, arg)

/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        AnscXmlDomNodeGetName
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve node state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     node state.

**********************************************************************/

char*
AnscXmlDomNodeGetName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    return  pXmlNode->Name;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to configure node state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the node state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    errno_t     rc = -1;

    if( name == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( ANSC_OBJECT_NAME_SIZE <= AnscSizeOfString(name))
    {
        AnscXmlWarning
            (
                "Invalid XML node name (too big)- '%s'\n",
                name
            );

        return ANSC_STATUS_FAILURE;
    }

    rc = strcpy_s(pXmlNode->Name, sizeof(pXmlNode->Name), name);
    ERR_CHK(rc);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeGetParentNode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve node state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     node state.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeGetParentNode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    return  pXmlNode->hParentNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetParentNode
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hParentNode
            );

    description:

        This function is called to configure node state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hParentNode
                Specifies the node state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetParentNode
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentNode
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    pXmlNode->hParentNode = hParentNode;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeAddChild
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChildNode
            );

    description:

        This function is called to add a new child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChildNode
                Specifies the child node to be added.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeAddChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = (PANSC_XML_DOM_NODE_OBJECT)hChildNode;

    AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
    AnscQueuePushEntry(&pXmlNode->ChildNodeQueue, &pChildNode->Linkage);
    AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);

    AnscXmlDomNodeSetParentNode((ANSC_HANDLE)pChildNode, (ANSC_HANDLE)pXmlNode);

    AnscXmlDomNodeSetLevel((ANSC_HANDLE)pChildNode, AnscXmlDomNodeGetLevel((ANSC_HANDLE)pXmlNode) + 1);

    return  (ANSC_HANDLE)pChildNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeAddChildByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to add a new child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the child node to be added.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeAddChildByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    BOOL                            bUnique       = FALSE;

    if ( bUnique )
    {
        pChildNode = AnscXmlDomNodeGetChildByName((ANSC_HANDLE)pXmlNode, name);
    }
    else
    {
        pChildNode = NULL;
    }

    if ( pChildNode )
    {
        return  (ANSC_HANDLE)pChildNode;
    }
    else
    {
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate(pXmlNode->hOwnerContext);

        if ( !pChildNode )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
        	AnscXmlDomNodeSetParentNode((ANSC_HANDLE)pChildNode, (ANSC_HANDLE)pXmlNode);
        	AnscXmlDomNodeSetName      ((ANSC_HANDLE)pChildNode, name);
        }

        AnscXmlDomNodeAddChild((ANSC_HANDLE)pXmlNode, (ANSC_HANDLE)pChildNode);

        AnscXmlDomNodeSetLevel((ANSC_HANDLE)pChildNode, AnscXmlDomNodeGetLevel((ANSC_HANDLE)pXmlNode) + 1);
    }

    return  (ANSC_HANDLE)pChildNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeDelChild
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChildNode
            );

    description:

        This function is called to delete an existing child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChildNode
                Specifies the child node to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeDelChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = (PANSC_XML_DOM_NODE_OBJECT)hChildNode;

    AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
    AnscQueuePopEntryByLink(&pXmlNode->ChildNodeQueue, &pChildNode->Linkage);
    AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);

    AnscXmlDomNodeRemove((ANSC_HANDLE)pChildNode);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeDelChildByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to delete an existing child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the child node to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeDelChildByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;

    pChildNode = AnscXmlDomNodeGetChildByName((ANSC_HANDLE)pXmlNode, name);

    if ( pChildNode )
    {
    	AnscXmlDomNodeDelChild((ANSC_HANDLE)pXmlNode, (ANSC_HANDLE)pChildNode);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeInsertChild
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hChildNode,
                ANSC_HANDLE                 hAfterChild
            );

    description:

        This function is called to insert a child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hChildNode
                Specifies the child node to be inserted.

                ANSC_HANDLE                 hAfterChild
                Specifies the child node after which the new child node
                is to be inserted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeInsertChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode,
        ANSC_HANDLE                 hAfterChild
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = (PANSC_XML_DOM_NODE_OBJECT)hChildNode;
    PANSC_XML_DOM_NODE_OBJECT       pAfterChild   = (PANSC_XML_DOM_NODE_OBJECT)hAfterChild;

    if ( pAfterChild )
    {
        AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
        AnscQueuePushEntryAfter(&pXmlNode->ChildNodeQueue, &pChildNode->Linkage, &pAfterChild->Linkage);
        AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);
    }
    else
    {
        AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
        AnscQueuePushEntryAtFront(&pXmlNode->ChildNodeQueue, &pChildNode->Linkage);
        AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);
    }

    AnscXmlDomNodeSetParentNode((ANSC_HANDLE)pChildNode, (ANSC_HANDLE)pXmlNode);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeRemoveChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to remove a child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeRemoveChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    if( AnscQueueQueryDepth(&pXmlNode->ChildNodeQueue) == 0)
    {
        if( pXmlNode->ChildNodeQueue.Next.Next != NULL)
        {
            PANSC_XML_ATTRIBUTE                     pNameAttr = NULL;

            pNameAttr = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetFirstAttr(pXmlNode);

            if( pNameAttr == NULL || pNameAttr->StringData == NULL)
            {
                AnscXmlWarning("The child node queue of XML node is corrupted.\n");
            }
            else
            {
                AnscXmlWarning
                    (
                        "The child node queue of XML node '%s' is corrupted.\n",
                        (PCHAR)pNameAttr->StringData
                    );
            }
        }

        return NULL;
    }

    AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
    pSLinkEntry = AnscQueuePopEntry(&pXmlNode->ChildNodeQueue);
    AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);

    if ( pSLinkEntry )
    {
        pChildNode = ACCESS_ANSC_XML_DOM_NODE_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pChildNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeGetChildByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve a child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the child node to be matched.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeGetChildByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;

    while (( pChildNode =
                (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeGetNextChild
                    (
                        (ANSC_HANDLE)pXmlNode,
                        (ANSC_HANDLE)pChildNode
                    )) )
    {
        if ( strcasecmp(AnscXmlDomNodeGetName((ANSC_HANDLE)pChildNode), name) == 0 )
        {
            return  (ANSC_HANDLE)pChildNode;
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeGetHeadChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the first child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeGetHeadChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    if( AnscQueueQueryDepth(&pXmlNode->ChildNodeQueue) == 0)
    {
        if( pXmlNode->ChildNodeQueue.Next.Next != NULL)
        {
            PANSC_XML_ATTRIBUTE                     pNameAttr = NULL;

            pNameAttr = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetFirstAttr(pXmlNode);

            if( pNameAttr == NULL || pNameAttr->StringData == NULL)
            {
                AnscXmlWarning("The child node queue of XML node is corrupted.\n");
            }
            else
            {
                AnscXmlWarning
                    (
                        "The child node queue of XML node '%s' is corrupted.\n",
                        (PCHAR)pNameAttr->StringData
                    );
            }
        }

        return NULL;
    }

    AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
    pSLinkEntry = AnscQueueGetFirstEntry(&pXmlNode->ChildNodeQueue);
    AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);

    if ( pSLinkEntry )
    {
        pChildNode = ACCESS_ANSC_XML_DOM_NODE_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pChildNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeGetNextChild
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hThisChild
            );

    description:

        This function is called to retrieve the next child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hThisChild
                Specifies the child node after which the next child
                node is to be returned.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeGetNextChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hThisChild
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    PANSC_XML_DOM_NODE_OBJECT       pThisChild    = (PANSC_XML_DOM_NODE_OBJECT)hThisChild;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    if ( pThisChild )
    {
        AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
        pSLinkEntry = AnscQueueGetNextEntry(&pThisChild->Linkage);
        AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);

        if ( pSLinkEntry )
        {
            pChildNode = ACCESS_ANSC_XML_DOM_NODE_OBJECT(pSLinkEntry);
        }
    }
    else
    {
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeGetHeadChild((ANSC_HANDLE)pXmlNode);
    }

    return  (ANSC_HANDLE)pChildNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeGetTailChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the last child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeGetTailChild
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry   = NULL;

    if( AnscQueueQueryDepth(&pXmlNode->ChildNodeQueue) == 0)
    {
        return NULL;
    }

    AnscAcquireXmlLock(&pXmlNode->ChildNodeQueueLock);
    pSLinkEntry = AnscQueueGetLastEntry(&pXmlNode->ChildNodeQueue);
    AnscReleaseXmlLock(&pXmlNode->ChildNodeQueueLock);

    if ( pSLinkEntry )
    {
        pChildNode = ACCESS_ANSC_XML_DOM_NODE_OBJECT(pSLinkEntry);
    }

    return  (ANSC_HANDLE)pChildNode;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomNodeGetTailChild
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the last child node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of the child node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomNodeGetAttr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAttrName
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * status of operation
     */

    /*
     * pointer to the underneath Attributes and nodes
     */
    PSINGLE_LINK_ENTRY              pSLinkEntry      = NULL;
    PANSC_XML_ATTRIBUTE             pSonAttribute    = NULL;

    /*
     * pointer to the current parent node
     */
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;


    /***********************************************************
               SEARCH THE CHILD Attribute BY MATCHING NAME
    ***********************************************************/

    /*
     * the pointer of the parent node cannot be NULL
     */
    if ( !pXmlNode )
    {
        return  (ANSC_HANDLE)NULL;
    }

    AnscAcquireXmlLock(&pXmlNode->AttributesListLock);
    pSLinkEntry = AnscQueueGetFirstEntry(&pXmlNode->AttributesList);
    AnscReleaseXmlLock(&pXmlNode->AttributesListLock);

    while ( pSLinkEntry )
    {
        pSonAttribute    = ACCESS_XML_ATTRIBUTE(pSLinkEntry);

        if ( strcmp(pSonAttribute->Name, (char *)pAttrName) == 0 )
        {
            return  (ANSC_HANDLE)pSonAttribute;
        }

        AnscAcquireXmlLock(&pXmlNode->AttributesListLock);
        pSLinkEntry      = AnscQueueGetNextEntry(pSLinkEntry);
        AnscReleaseXmlLock(&pXmlNode->AttributesListLock);
    }

    return  (ANSC_HANDLE)NULL;
}

ANSC_HANDLE
AnscXmlDomNodeCopy
    (
        ANSC_HANDLE                 hNode
    )
{

    PANSC_XML_DOM_NODE_OBJECT       pOld            = (PANSC_XML_DOM_NODE_OBJECT)hNode;
    PANSC_XML_DOM_NODE_OBJECT       pCopy           = NULL;

    PANSC_XML_ATTRIBUTE             pFirst          = NULL;
    PANSC_XML_ATTRIBUTE             pNewAttribute   = NULL;

    PANSC_XML_DOM_NODE_OBJECT       pFirstNode      = NULL;
    ANSC_HANDLE                     hCopy, hChild;
    errno_t                         rc              = -1;

    if( hNode == NULL)
    {
        return (ANSC_HANDLE)NULL;
    }

    /* create a new node with the node name */
    pCopy = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate(pOld->hOwnerContext);
    if( pCopy == NULL)
    {
        return (ANSC_HANDLE)NULL;
    }

    hCopy = (ANSC_HANDLE)pCopy;

    AnscXmlDomNodeSetName(hCopy, AnscXmlDomNodeGetName(hNode));

    /* copy all the attributes */
    pFirst = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetFirstAttr(hNode);
    while ( pFirst != NULL)
    {
        /*
         * generate a new tree Attribute
         */
        pNewAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

        if ( !pNewAttribute )
        {
        	AnscXmlDomNodeRemove( hCopy );
            ANSC_XML_TRACE("Failed to allocate attribute structure for in ALXMLCopyHandle\n");

            return (ANSC_HANDLE)NULL;
        }

        pNewAttribute->hParentNode   = (ANSC_HANDLE)pCopy;
        pNewAttribute->hXMLContext   = pCopy->hXMLContext;
        pNewAttribute->DataSize      = pFirst->DataSize;
        pNewAttribute->StringData    = AnscAllocateMemory( pFirst->DataSize + 1);

        if( pNewAttribute->StringData != NULL)
        {
            /* copy the attribute name and value */
            rc = strcpy_s(pNewAttribute->Name, sizeof(pNewAttribute->Name), pFirst->Name);
            ERR_CHK(rc);

            if( pFirst->StringData != NULL)
            {
                AnscCopyMemory(pNewAttribute->StringData, pFirst->StringData, pNewAttribute->DataSize);
            }

            /* add the attribute to the list */
            AnscAcquireXmlLock(&pCopy->AttributesListLock);
            AnscQueuePushEntry(&pCopy->AttributesList, &pNewAttribute->Linkage);
            AnscReleaseXmlLock(&pCopy->AttributesListLock);
        }
        else
        {
            AnscFreeMemory(pNewAttribute);
        }

        /* goto next attribute */
        pFirst = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetNextAttr(hNode, (ANSC_HANDLE)pFirst);
    }

     /* copy the text value */
    if( pOld->DataSize > 0 && pOld->StringData != NULL)
    {
        pCopy->DataSize   = pOld->DataSize;
        pCopy->StringData = AnscAllocateMemory( pOld->DataSize + 1);

        if( pCopy->StringData != NULL)
        {
            AnscCopyMemory(pCopy->StringData, pOld->StringData, pOld->DataSize);
        }
        else
        {
            pCopy->DataSize = 0;
        }
    }

    /* create and copy all the child node */
    pFirstNode  = AnscXmlDomNodeGetHeadChild(hNode);

    while( pFirstNode )
    {
        hChild  = AnscXmlDomNodeCopy((ANSC_HANDLE)pFirstNode);
        if (hChild)
        	AnscXmlDomNodeAddChild(hCopy, hChild);

        /* goto the next node */
        pFirstNode = AnscXmlDomNodeGetNextChild(hNode, (ANSC_HANDLE)pFirstNode);
    }

    return hCopy;
}

ANSC_HANDLE
AnscXmlDomNodeGetFirstAttr
    (
        ANSC_HANDLE                 hNode
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * pointer to the current parent node
     */
    PANSC_XML_DOM_NODE_OBJECT       pCurrentNode = (PANSC_XML_DOM_NODE_OBJECT)hNode;

    /*
     * pointer to the first available son node
     */
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;


    /***********************************************************
          GET THE FIRST AVAILABLE CHILD NODE FROM THE LIST
    ***********************************************************/

    /*
     * make sure user didn't pass down invalid handle
     */
    if ( !pCurrentNode )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * retrieve the pointer to the first son node
     */
    pSLinkEntry = AnscQueueGetFirstEntry(&pCurrentNode->AttributesList);

    if ( !pSLinkEntry )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)ACCESS_XML_ATTRIBUTE(pSLinkEntry);
}

ANSC_HANDLE
AnscXmlDomNodeGetNextAttr
    (
        ANSC_HANDLE                 hNode,
        ANSC_HANDLE                 hAttribute
    )
{
    UNREFERENCED_PARAMETER(hNode);
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * pointer to the current parent node
     */
    PANSC_XML_ATTRIBUTE             pCurrentAttr = (PANSC_XML_ATTRIBUTE)hAttribute;

    /*
     * pointer to the first available sibling node
     */
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;


    /***********************************************************
           GET THE NEXT AVAILABLE ATTRIBUTE
    ***********************************************************/

    /*
     * make sure user didn't pass down invalid handle
     */
    if ( !pCurrentAttr )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * retrieve the pointer to the next sibling node
     */
    pSLinkEntry = AnscQueueGetNextEntry(&pCurrentAttr->Linkage);

    if ( !pSLinkEntry )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)ACCESS_XML_ATTRIBUTE(pSLinkEntry);
}

ULONG
AnscXmlDomNodeGetLevel
    (
        ANSC_HANDLE                 hNode
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode    = (PANSC_XML_DOM_NODE_OBJECT)hNode;

    if (pXmlNode)
        return pXmlNode->Level;
    else
        return 0xFFFFFFFF;
}

void
AnscXmlDomNodeSetLevel
    (
        ANSC_HANDLE                 hNode,
        ULONG                       ulLevel
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pKid;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode    = (PANSC_XML_DOM_NODE_OBJECT)hNode;

    if (pXmlNode)
    {
        pXmlNode->Level = ulLevel;

        pKid    = AnscXmlDomNodeGetHeadChild(hNode);

        while (pKid)
        {
        	AnscXmlDomNodeSetLevel((ANSC_HANDLE)pKid, ulLevel + 1);

            pKid    = AnscXmlDomNodeGetNextChild(hNode, (ANSC_HANDLE)pKid);
        }
    }
}

BOOL
AnscXmlDomNodeEnumChild
    (
        ANSC_HANDLE                 hNode,
        PFN_XML_NODE_ENUM_KID_PROC  EnumProc,
        PVOID                       pData
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode    = (PANSC_XML_DOM_NODE_OBJECT)hNode;
    PANSC_XML_DOM_NODE_OBJECT       pKid;

    pKid    = AnscXmlDomNodeGetHeadChild((ANSC_HANDLE)pXmlNode);

    while (pKid)
    {
        if (!EnumProc(pKid, pData))
            return FALSE;

        pKid    = AnscXmlDomNodeGetNextChild((ANSC_HANDLE)pXmlNode, (ANSC_HANDLE)pKid);
    }

    return TRUE;
}


