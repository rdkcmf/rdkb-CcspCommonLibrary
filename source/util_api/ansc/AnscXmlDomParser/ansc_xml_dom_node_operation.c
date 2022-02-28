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

    module:	ansc_xml_dom_node_operation.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xml Node Object.

        *   AnscXmlDomNodeGetStatus
        *   AnscXmlDomNodeGetEncodedSize
        *   AnscXmlDomNodeEncode
        *   AnscXmlDomNodeDecode

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

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscXmlDomNodeGetStatus
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function gets the current node status.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     node status.

**********************************************************************/

ULONG
AnscXmlDomNodeGetStatus
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscXmlDomNodeGetEncodedSize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function gets the size of the encoded xml data stream.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     xml data stream size.

**********************************************************************/

ULONG
AnscXmlDomNodeGetEncodedSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    ULONG                           ulEncodedSize = 0;

    /*
     * pointer to the underneath items and nodes
     */
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;
    PANSC_XML_ATTRIBUTE             pSonItem    = NULL;

    /***********************************************************
              GO DOWN DEEPLY TO DIG EACH CHILDREN NODE
    ***********************************************************/
    /*
     *  If it's the root node, don't forget to write XML header
     */
    if( pXmlNode->Level == 0 && !pXmlNode->bIgnoreXMLHeader)
    {
        ulEncodedSize   += AnscSizeOfString((LPSTR)XML_HEADER);
        ulEncodedSize   += 1;
    }
    else
    {
        ulEncodedSize   += pXmlNode->Level * SPACES_PER_LEVEL;
    }

    /*
     * write the node name into the file is the very first thing
     */
    ulEncodedSize   += AnscSizeOfString(XML_TAG_BEGIN);
    ulEncodedSize   += AnscSizeOfString(pXmlNode->Name);

    /*
     * loop to write each item into the configuration file
     */
    pSLinkEntry = AnscQueueGetFirstEntry(&pXmlNode->AttributesList);
    while ( pSLinkEntry )
    {
        pSonItem    = ACCESS_XML_ATTRIBUTE(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        ulEncodedSize   += 1;

        ulEncodedSize   += AnscXmlGetAttr2BufSize(pSonItem);
    }

    /***********************************************************
     *
     * To save the space, we will use <tag /> for all the nodes
     * has no text data and any child nodes
     *
     **********************************************************/
    if(AnscXmlDomNodeGetHeadChild(pXmlNode) == NULL)
    {
        if( pXmlNode->DataSize == 0)
        {
            /*
             * we put a space after all the attributes
             */
            ulEncodedSize   += 1;

            ulEncodedSize   += AnscSizeOfString(XML_SIMPLE_END);

            /*
             * we put a line feed at the end of a node
             */
            ulEncodedSize   += 1;

            return  ulEncodedSize;
        }
        else
        {
            /*
             *  Write the right part of node ">";
             */
            ulEncodedSize   += AnscSizeOfString(XML_TAG_END);

            ulEncodedSize   += pXmlNode->DataSize;

            /*
             *  Don't forget to write the end tag here; "</name>"
             */
            ulEncodedSize   += AnscSizeOfString(XML_END_NODE);

            ulEncodedSize   += AnscSizeOfString(pXmlNode->Name);

            ulEncodedSize   += AnscSizeOfString(XML_TAG_END);

            ulEncodedSize   += 1;

            return  ulEncodedSize;
        }
    }

    /*
     *  Write the right part of node ">";
     */
    ulEncodedSize   += AnscSizeOfString(XML_TAG_END);

    /*
     * we put a line feed at the end of a node
     */
    ulEncodedSize   += 1;

    /*
     *  Write Node Text Value here;
     */
    if( pXmlNode->StringData )
    {
        ulEncodedSize   += (pXmlNode->Level + 1) * SPACES_PER_LEVEL;

        ulEncodedSize   += pXmlNode->DataSize;

        ulEncodedSize   += 1;
    }

    /*
     * loop to write each son nodes into the configuration file
     */
    while ( (pChildNode =
                (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeGetNextChild
                    (
                        (ANSC_HANDLE)pXmlNode,
                        (ANSC_HANDLE)pChildNode
                    ))
          )
    {
        ulEncodedSize   += AnscXmlDomNodeGetEncodedSize((ANSC_HANDLE)pChildNode);
    }

    /*
     *  Don't forget to write the end tag here; "</name>"
     */
    ulEncodedSize   += pXmlNode->Level * SPACES_PER_LEVEL;

    ulEncodedSize   += AnscSizeOfString(XML_END_NODE);

    ulEncodedSize   += AnscSizeOfString(pXmlNode->Name);

    ulEncodedSize   += AnscSizeOfString(XML_TAG_END);

    ulEncodedSize   += 1;

    return  ulEncodedSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeEncode
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function encodes the current information in xml format.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the buffer where the encoded data will be put.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the encoded data stream.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeEncode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    PUCHAR                          pBuf          = (PUCHAR)buffer;
    ULONG                           ulCopied      = 0;
    ULONG                           ulContent     = 0;

    /*
     * pointer to the underneath items and nodes
     */
    PSINGLE_LINK_ENTRY              pSLinkEntry = NULL;
    PANSC_XML_ATTRIBUTE             pSonItem    = NULL;

    /***********************************************************
              GO DOWN DEEPLY TO DIG EACH CHILDREN NODE
    ***********************************************************/
    /*
     *  If it's the root node, don't forget to write XML header
     */
    if( pXmlNode->Level == 0 && !pXmlNode->bIgnoreXMLHeader)
    {
        ulContent       = AnscSizeOfString((LPSTR)XML_HEADER);
        ulCopied        = AnscXmlNode2Buf(pBuf, pulSize, (PVOID)XML_HEADER, ulContent);

        if (ulCopied < ulContent)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pBuf            += ulCopied;

        /*
         * we put a line feed at the end of a node
         */
        ulCopied        = AnscXmlNodeEncodeLinefeed(pBuf, pulSize);
        pBuf            += ulCopied;
    }
    else
    {
        ulCopied        = AnscXmlNodeEncodeSpaces(pBuf, pulSize, pXmlNode->Level * SPACES_PER_LEVEL);

        pBuf            += ulCopied;
    }

    /*
     * write the node name into the file is the very first thing
     */
    ulCopied        = AnscXmlNode2Buf(pBuf, pulSize, XML_TAG_BEGIN, AnscSizeOfString(XML_TAG_BEGIN));
    if (ulCopied == 0)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pBuf            += ulCopied;

    ulContent       = AnscSizeOfString(pXmlNode->Name);
    ulCopied        = AnscXmlNode2Buf(pBuf, pulSize, pXmlNode->Name, ulContent);

    if (ulCopied < ulContent)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pBuf            += ulCopied;

    /*
     * loop to write each item into the configuration file
     */

    pSLinkEntry = AnscQueueGetFirstEntry(&pXmlNode->AttributesList);

    while ( pSLinkEntry )
    {
        pSonItem    = ACCESS_XML_ATTRIBUTE(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        ulCopied    = AnscXmlNodeEncodeSpaces(pBuf, pulSize, 1);
        if (ulCopied == 0)
        {
            return ANSC_STATUS_RESOURCES;
        }
        pBuf        += ulCopied;

        ulCopied    = AnscXmlEncodeAttr2Buf(pBuf, pulSize, pSonItem);
        pBuf        += ulCopied;
    }

    /***********************************************************
     *
     * To save the space, we will use <tag /> for all the nodes
     * has no text data and any child nodes
     *
     **********************************************************/
    if(AnscXmlDomNodeGetHeadChild(pXmlNode) == NULL)
    {
        if( pXmlNode->DataSize == 0)
        {
            ulCopied    = AnscXmlNodeEncodeSpaces(pBuf, pulSize, 1);
            if (ulCopied == 0)
            {
                return ANSC_STATUS_RESOURCES;
            }
            pBuf        += ulCopied;

            ulContent       = AnscSizeOfString(XML_SIMPLE_END);
            ulCopied        = AnscXmlNode2Buf(pBuf, pulSize, XML_SIMPLE_END, ulContent);

            if (ulCopied < ulContent)
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf            += ulCopied;

            /*
             * we put a line feed at the end of a node
             */
            ulCopied        = AnscXmlNodeEncodeLinefeed(pBuf, pulSize);
            pBuf            += ulCopied;

            return ANSC_STATUS_SUCCESS;
        }
        else
        {
            /*
             *  Write the right part of node ">";
             */
            ulContent       = AnscSizeOfString(XML_TAG_END);
            ulCopied        = AnscXmlNode2Buf(pBuf, pulSize, XML_TAG_END, ulContent);

            if (ulCopied < ulContent)
            {
                return ANSC_STATUS_RESOURCES;
            }
            pBuf            += ulCopied;

            ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, pXmlNode->StringData, pXmlNode->DataSize);
            if (ulCopied < pXmlNode->DataSize)
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;

            /*
             *  Don't forget to write the end tag here; "</name>"
             */
            ulContent   = AnscSizeOfString(XML_END_NODE);
            ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, XML_END_NODE, ulContent);
            if (ulCopied < ulContent )
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;

            ulContent   = AnscSizeOfString(pXmlNode->Name);
            ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, pXmlNode->Name, ulContent);
            if (ulCopied < ulContent )
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;

            ulContent   = AnscSizeOfString(XML_TAG_END);
            ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, XML_TAG_END, ulContent);
            if (ulCopied < ulContent )
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;

            ulCopied    = AnscXmlNodeEncodeLinefeed(pBuf, pulSize);

            if (ulCopied == 0)
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;

            return ANSC_STATUS_SUCCESS;
        }
    }

    if(TRUE)
    {
        /*
         *  Write the right part of node ">";
         */
        ulContent       = AnscSizeOfString(XML_TAG_END);
        ulCopied        = AnscXmlNode2Buf(pBuf, pulSize, XML_TAG_END, ulContent);
        if (ulCopied < ulContent)
        {
            return ANSC_STATUS_RESOURCES;
        }
        pBuf            += ulCopied;

        /*
         * we put a line feed at the end of a node
         */
        ulCopied        = AnscXmlNodeEncodeLinefeed(pBuf, pulSize);

        if (ulCopied == 0)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pBuf            += ulCopied;

        /*
         *  Write Node Text Value here;
         */
        if( pXmlNode->StringData )
        {
            ulCopied    = AnscXmlNodeEncodeSpaces(pBuf, pulSize, (pXmlNode->Level + 1)* SPACES_PER_LEVEL);
            if (ulCopied == 0)
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;

            ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, pXmlNode->StringData, pXmlNode->DataSize);
            if (ulCopied < pXmlNode->DataSize)
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;

            ulCopied    = AnscXmlNodeEncodeLinefeed(pBuf, pulSize);
            if (ulCopied == 0)
            {
                return ANSC_STATUS_RESOURCES;
            }

            pBuf        += ulCopied;
        }

        /*
         * loop to write each son nodes into the configuration file
         */
        while ( (pChildNode =
                    (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeGetNextChild
                        (
                            (ANSC_HANDLE)pXmlNode,
                            (ANSC_HANDLE)pChildNode
                        )) &&
                (*pulSize != 0) )
        {
            ULONG                       ulSize;

            ulSize  = AnscXmlDomNodeGetEncodedSize((ANSC_HANDLE)pChildNode);
            if ( *pulSize < ulSize )
            {
                return  ANSC_STATUS_RESOURCES;
            }

            returnStatus = AnscXmlDomNodeEncode((ANSC_HANDLE)pChildNode, pBuf, pulSize);

            if ( returnStatus != ANSC_STATUS_SUCCESS )
            {
                return  returnStatus;
            }
            else
            {
                pBuf            += ulSize;
            }
        }

        /*
         *  Don't forget to write the end tag here; "</name>"
         */
        ulCopied    = AnscXmlNodeEncodeSpaces(pBuf, pulSize, pXmlNode->Level * SPACES_PER_LEVEL);
        if (ulCopied != pXmlNode->Level * SPACES_PER_LEVEL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pBuf        += ulCopied;

        ulContent   = AnscSizeOfString(XML_END_NODE);
        ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, XML_END_NODE, ulContent);
        if (ulCopied < ulContent )
        {
            return ANSC_STATUS_RESOURCES;
        }

        pBuf        += ulCopied;

        ulContent   = AnscSizeOfString(pXmlNode->Name);
        ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, pXmlNode->Name, ulContent);
        if (ulCopied < ulContent )
        {
            return ANSC_STATUS_RESOURCES;
        }

        pBuf        += ulCopied;

        ulContent   = AnscSizeOfString(XML_TAG_END);
        ulCopied    = AnscXmlNode2Buf(pBuf, pulSize, XML_TAG_END, ulContent);
        if (ulCopied < ulContent )
        {
            return ANSC_STATUS_RESOURCES;
        }

        pBuf        += ulCopied;

        ulCopied    = AnscXmlNodeEncodeLinefeed(pBuf, pulSize);

        if (ulCopied == 0)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pBuf        += ulCopied;

    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeDecode
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function decodes the xml data stream into the internal
        format.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the buffer where the decoded data is put.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the decoded data stream.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeDecode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
#if 0
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pChildNode    = NULL;
    PVOID                           pChildBuffer  = buffer;
    ULONG                           ulDecodedSize = 0;
    ULONG                           ulBufferSize  = *pulSize;
    ULONG                           ulLeftSize    = ulBufferSize;

    while ( ulBufferSize > 0 )
    {
        pChildNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate(pXmlNode->hOwnerContext);

        if ( !pChildNode )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pChildNode->SetParentNode((ANSC_HANDLE)pChildNode, (ANSC_HANDLE)pXmlNode);
        }

        returnStatus =
            pChildNode->Decode
                (
                    (ANSC_HANDLE)pChildNode,
                    pChildBuffer,
                    &ulBufferSize
                );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return  returnStatus;
        }
        else
        {
            pChildBuffer   = (PVOID)((ULONG)pChildBuffer + ulBufferSize);
            ulDecodedSize += ulBufferSize;
            ulLeftSize    -= ulBufferSize;
            ulBufferSize   = ulLeftSize;
        }

        pXmlNode->AddChild((ANSC_HANDLE)pXmlNode, (ANSC_HANDLE)pChildNode);
    }

    *pulSize = ulDecodedSize;

    return  returnStatus;
#endif

    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * status of operation
     */
    PCHAR                           *ppContent         = (PCHAR *)&buffer;
    LONG                            length             = *pulSize;
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pNode              = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to a new tree node and new tree Attribute
     */
    PANSC_XML_DOM_NODE_OBJECT       pNewNode           = NULL;
    PANSC_XML_ATTRIBUTE             pNewAttribute      = NULL;

    /*
     * pointer to the start point of the Attributes
     */
    PCHAR                           pBackupContent     = *ppContent;
    PCHAR                           pNodesOfNode       = *ppContent;
    PCHAR                           pStartOfAttribute  = NULL;
    PCHAR                           pEndOfAttribute    = NULL;
    PCHAR                           pStartOfAttrValue  = NULL;
    PCHAR                           pEndOfAttrValue    = NULL;
    PCHAR                           pEndOfNode         = NULL;
    PCHAR                           pEndOfNodeName     = NULL;
    PCHAR                           pStartOfText       = NULL;
    PCHAR                           pEndOfText         = NULL;

    /*
     * pointer to the name of the new child node and the name of the new Attribute
     */
    PCHAR                           pNewNodeName      = NULL;
    PCHAR                           pNewAttributeName = NULL;
    ULONG                           ulBufLen          = *pulSize;
    ULONG                           ulPrevBufLen;
    errno_t                         rc                = -1;

    /***********************************************************
              GO DOWN DEEPLY TO DIG EACH CHILDREN NODE
    ***********************************************************/

    /*
     * go through the Node Name
     */
    pNewNodeName = (PCHAR)AnscAllocateMemory(MAXIMUM_NODE_NAME);

    if( pNewNodeName == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }

    pNodesOfNode += AnscSizeOfString(XML_TAG_BEGIN);

    pNodesOfNode  = AnscXmlGotoFirstCharExcept
                    (
                        pNodesOfNode,
                        length - ( pNodesOfNode - pBackupContent),
                        (LPSTR)SPECIAL_TOKEN
                    );
    /*CID: 65081 Dereference before null check*/
    if( !pNodesOfNode )
    {
        ANSC_XML_TRACE("Cannot find the end. \n");
        AnscFreeMemory(pNewNodeName);
        return  ANSC_STATUS_XML_MISSED_END_TAG;
    }

    pEndOfNode    = AnscXmlFindString
                    (
                        pNodesOfNode,
                        (LPSTR)XML_TAG_END
                    );

    /*
     *  If ipEndOfNode is NULL
     */
    if( !pEndOfNode)
    {
        ANSC_XML_TRACE("Cannot find the end. \n");
        AnscFreeMemory(pNewNodeName);
        return  ANSC_STATUS_XML_MISSED_END_TAG;
    }

    pEndOfNodeName    = AnscXmlGotoFirstWantedChar
                            (
                                pNodesOfNode,
                                length - ( pNodesOfNode - pBackupContent),
                                SPECIAL_TOKEN
                            ); /* " \t\n\r"; */

    if( !pEndOfNodeName )
    {
        ANSC_XML_TRACE("Cannot find the end. \n");
        AnscFreeMemory(pNewNodeName);
        return  ANSC_STATUS_XML_MISSED_END_TAG;
    }

    /*
     *  If out of range, it must be Node Name only
     */
    if( pEndOfNodeName > pEndOfNode )
    {
        pEndOfNodeName = pEndOfNode - 1;

        if( pEndOfNodeName[0] != XML_NODE_END[0] ) /* Make sure it's not "/>"; */
        {
            pEndOfNodeName = pEndOfNode;
        }
    }

    /*
     *  Check the node name valid or not
     */
    if ( pEndOfNodeName - pNodesOfNode > MAXIMUM_NODE_NAME )
    {
        ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Node Name is out of size.");
        AnscFreeMemory(pNewNodeName);
        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    AnscCopyMemory( (PVOID)pNewNodeName , (PVOID)pNodesOfNode, (ULONG)(pEndOfNodeName - pNodesOfNode));

    if( !AnscXmlIsValidNodeName( pNewNodeName ))
    {
        ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Invalid Node Name : %s\n", pNewNodeName );
        AnscFreeMemory(pNewNodeName);
        return  ANSC_STATUS_XML_INVALID_NODE_NAME;
    }

    /*
     *  Then set the node name;
     */
    rc = strcpy_s( pNode->Name, sizeof(pNode->Name), pNewNodeName);
    ERR_CHK(rc);

    /*
     *  Maybe it's <node/> only, return success (Seldom occurred, but possible );
     */
    if( pEndOfNodeName == pEndOfNode - 1 &&  pEndOfNodeName[0] == XML_NODE_END[0])
    {
        *pulSize    -= (pEndOfNode + 1 - *ppContent);
		AnscFreeMemory(pNewNodeName);/*RDKB-6189, CID-24309, free unused mem before return*/
        return ANSC_STATUS_SUCCESS;
    }

    /***********************************************************
           ENUMERATE ALL THE AttributeS UNDER THE CURRENT NODE
    ***********************************************************/
    if( pEndOfNodeName < pEndOfNode )
    {
        /*
         * go through all the underneath Attributes and import the data associated with the Attributes
         */
        pStartOfAttribute  = AnscXmlGotoFirstCharExcept
                                (
                                    pEndOfNodeName,
                                    length - ( pEndOfNodeName - pBackupContent),
                                    SPECIAL_TOKEN
                                );

        pNewAttributeName = (PCHAR)AnscAllocateMemory(MAXIMUM_ATTRIBUTE_NAME);

        if( pNewAttributeName == NULL)
        {

            AnscFreeMemory(pNewNodeName);

            return ANSC_STATUS_RESOURCES;
        }

        /*
         *  loop only when the first char is not null && != '>' && != '/';
         */
        while (
                  pStartOfAttribute &&
                  !AnscXmlIsStringStartWith
                      (
                          pStartOfAttribute,
                          length - ( pStartOfAttribute - pBackupContent),
                          XML_NODE_END
                      )
                  &&
                  !AnscXmlIsStringStartWith
                      (
                          pStartOfAttribute,
                          length - ( pStartOfAttribute - pBackupContent),
                          XML_TAG_END
                      )
              )
        {

            /*
             *  Get the attribute Name until ' ' or '=';
             */
            pEndOfAttribute    = AnscXmlGotoFirstWantedChar
                                    (
                                        pStartOfAttribute,
                                        length - ( pStartOfAttribute - pBackupContent),
                                        WANTED_TOKEN
                                    );

            if( !pEndOfAttribute || pEndOfNode <= pEndOfAttribute)
            {
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                return  ANSC_STATUS_XML_EQUAL_SIGN_MISSED;
            }

            if( pEndOfAttribute == pStartOfAttribute)
            {
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                return  ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME;
            }

            if ( pEndOfAttribute - pStartOfAttribute > MAXIMUM_ATTRIBUTE_NAME )
            {
                ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Attribute Name is out of size.");
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                return  ANSC_STATUS_XML_INVALID_LENGTH;
            }

            AnscCopyMemory( pNewAttributeName, pStartOfAttribute, (ULONG)(pEndOfAttribute - pStartOfAttribute ));

            if( !AnscXmlIsValidAttributeName( pNewAttributeName ) )
            {
                ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Invalid Attribute Name : %s\n", pNewAttributeName);
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                return  ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME;
            }

            /*
             * parse the atttribute value
             */
            pStartOfAttrValue = AnscXmlGotoFirstCharExcept
                                    (
                                        pEndOfAttribute,
                                        length - ( pEndOfAttribute - pBackupContent),
                                        SPECIAL_TOKEN
                                    );

            if( ! pStartOfAttrValue ||
                !AnscXmlIsStringStartWith
                (
                    pStartOfAttrValue,
                    length - ( pStartOfAttrValue - pBackupContent),
                    ITEM_EQUAL_MARK
                 )
               ) /* must be '=' */
            {
                ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Equal sign between Attribute Name and Value was expected.\n");
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                return  ANSC_STATUS_XML_EQUAL_SIGN_MISSED;
            }

            pStartOfAttrValue += 1;
            pStartOfAttrValue  = AnscXmlGotoFirstCharExcept
                                    (
                                        pStartOfAttrValue,
                                        length - ( pStartOfAttrValue - pBackupContent),
                                        SPECIAL_TOKEN
                                    );

            if( ! pStartOfAttrValue ||
                !AnscXmlIsStringStartWith
                (
                    pStartOfAttrValue,
                    length - ( pStartOfAttrValue - pBackupContent),
                    STRING_MARK_1
                 )
              ) /* must be "\""; */
            {
                ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- A string literal was expected.\n");
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                return ANSC_STATUS_XML_STRING_SIGN_MISSED;
            }

            pStartOfAttrValue += AnscSizeOfString(STRING_MARK_1);
            pEndOfAttrValue    = AnscXmlFindString
                                    (
                                        pStartOfAttrValue,
                                        STRING_MARK_1
                                    );

            if( !pEndOfAttrValue || pEndOfAttrValue >= pEndOfNode )
            {
                ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- A string literal was expected.\n");
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                return ANSC_STATUS_XML_STRING_SIGN_MISSED;
            }

            /*
             * generate a new tree Attribute
             */
            pNewAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

            if ( !pNewAttribute )
            {
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                ANSC_XML_TRACE("Failed to allocate attribute structure for - '%s'\n", pNode->Name);
                return ANSC_STATUS_XML_RESOURCES;
            }

            pNewAttribute->hParentNode   = (ANSC_HANDLE)pNode;
            pNewAttribute->hXMLContext   = pNode->hXMLContext;

            rc = strcpy_s(pNewAttribute->Name, sizeof(pNewAttribute->Name), pNewAttributeName);
            ERR_CHK(rc);

            if( pEndOfAttrValue >= pStartOfAttrValue)
            {
                pNewAttribute->DataSize      = (ULONG)(pEndOfAttrValue - pStartOfAttrValue);
                pNewAttribute->StringData    = AnscAllocateMemory( pNewAttribute->DataSize + 1);

                if( !pNewAttribute->StringData)
                {
                    AnscFreeMemory(pNewNodeName);
                    AnscFreeMemory(pNewAttributeName);
                    AnscFreeMemory( pNewAttribute );
                    ANSC_XML_TRACE("Failed to allocate attribute value for - '%s'\n", pNode->Name);
                    return ANSC_STATUS_XML_RESOURCES;
                }

                AnscCopyMemory( pNewAttribute->StringData, pStartOfAttrValue, (ULONG)(pEndOfAttrValue - pStartOfAttrValue));
            }
            else
            {
                ANSC_XML_TRACE("Something wrong, cannot be here.\n");
            }

            /*
             *  Check the value valid or not
             */
            if( !AnscXmlIsValidAttributeValue( (PCHAR)pNewAttribute->StringData ))
            {
                ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Invalid Attribute value: '%p'\n", pNewAttribute->StringData);
                AnscFreeMemory(pNewNodeName);
                AnscFreeMemory(pNewAttributeName);
                AnscFreeMemory( pNewAttribute->StringData );
                AnscFreeMemory( pNewAttribute );
                return ANSC_STATUS_XML_INVALID_ATTRIBUTE_VALUE;
            }

            AnscQueuePushEntry(&pNode->AttributesList, &pNewAttribute->Linkage);

            /*
             * move to the next Attribute
             */
            AnscZeroMemory(pNewAttributeName, MAXIMUM_ATTRIBUTE_NAME);

            pEndOfAttrValue   += AnscSizeOfString(STRING_MARK_1);
            pStartOfAttribute  = AnscXmlGotoFirstCharExcept
                                     (
                                        pEndOfAttrValue,
                                        length - ( pEndOfAttrValue - pBackupContent),
                                        SPECIAL_TOKEN
                                     );

            if( pStartOfAttribute == pEndOfAttrValue ) /* No Space after '"'; */
            {
                if( !AnscXmlIsStringStartWith
                        (
                            pStartOfAttribute,
                            length - ( pStartOfAttribute - pBackupContent),
                            XML_TAG_END
                        )
                    &&
                    !AnscXmlIsStringStartWith
                        (
                            pStartOfAttribute,
                            length - ( pStartOfAttribute - pBackupContent),
                            XML_NODE_END
                        )
                 )
                {
                    AnscFreeMemory(pNewNodeName);
                    AnscFreeMemory(pNewAttributeName);
                    ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- A space after attribute was expected.\n");
                    return ANSC_STATUS_XML_SPACE_NEEDED;
                }
            }
        }

        /*
         *  If it's the end of file, Wrong !!!
         */
        if( !pStartOfAttribute)
        {
            ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Failed to find the attribute.\n");
            AnscFreeMemory(pNewNodeName);
            AnscFreeMemory(pNewAttributeName);
            return  ANSC_STATUS_XML_MISSED_END_TAG;
        }

        /*
         *  If it starts with "/>", means the end of Node, we can return successfully.
         */
        if( AnscXmlFindString( pStartOfAttribute, XML_NODE_END ) == pStartOfAttribute )
        {
            ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Load the node successfully : %s \n", pNode->Name);
            AnscFreeMemory(pNewNodeName);
            AnscFreeMemory(pNewAttributeName);

            /*
             *  Don't forget to set the current position of data;
             */
            pStartOfAttribute += AnscSizeOfString(XML_NODE_END);

            *pulSize          -= (ULONG)(pStartOfAttribute - *ppContent);

            *ppContent        = pStartOfAttribute;

            return ANSC_STATUS_SUCCESS;
        }
    }

    /*
     *  Start to read the text
     */
    pEndOfNode ++;

    pStartOfText = AnscXmlGotoFirstCharExcept
                       (
                            pEndOfNode,
                            length - ( pEndOfNode - pBackupContent),
                            SPECIAL_TOKEN
                       );
    /*CID: 55910 Dereference before null check*/
    if( !pStartOfText )
    {
        ANSC_XML_TRACE("Cannot find the end tag. \n");
        AnscFreeMemory(pNewNodeName);
        return ANSC_STATUS_XML_MISSED_END_TAG;
    }
    pEndOfText   = AnscXmlFindString
                       (
                            pStartOfText,
                            XML_TAG_BEGIN
                       );

    /* Remove the chars " \t\n\r" from the end. */
    pEndOfText   = AnscXmlGetStringEndExcept
                       (
                            pStartOfText,
                            (ULONG)(pEndOfText - pStartOfText),
                            SPECIAL_TOKEN
                       );

    if( pStartOfText && pEndOfText && pEndOfText > pStartOfText)  /* Get the text value; */
    {
        pNode->DataSize     = (LONG)(pEndOfText - pStartOfText);
        pNode->StringData   = AnscAllocateMemory(pNode->DataSize + 1);
        if ( !pNode->StringData )
        {
            AnscFreeMemory(pNewNodeName);
            AnscFreeMemory(pNewAttributeName);
            ANSC_XML_TRACE
                (
                    "Failed to allocate %lu memory for text of - '%s'\n",
                    (ULONG)(pEndOfText-pStartOfText),
                    pNode->Name
                );

            return ANSC_STATUS_XML_RESOURCES;
        }

        AnscCopyMemory( pNode->StringData, pStartOfText, (ULONG)(pEndOfText - pStartOfText));
    }

    if (pNewNodeName)
    {
        AnscFreeMemory(pNewNodeName);
        pNewNodeName    = NULL;
    }
    if (pNewAttributeName)
    {
        AnscFreeMemory(pNewAttributeName);
        pNewAttributeName   = NULL;
    }

    /*
     *  Go through all the child nodes
     */
    pEndOfText   = AnscXmlFindString
                       (
                            pEndOfText,
                            XML_TAG_BEGIN
                       );
    pNodesOfNode = pEndOfText;


    while (
              pNodesOfNode &&
              !AnscXmlIsStringStartWith
                 (
                    pNodesOfNode,
                    length - ( pNodesOfNode - pBackupContent),
                    XML_END_NODE
                 )
          )
    {
        returnStatus = AnscXmlFindNode
                           (
                               &pNodesOfNode,
                               length - (pNodesOfNode - pBackupContent),
                               pNode->Level + 1
                           );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            ANSC_XML_TRACE("Failed to find the node.\n");
            return returnStatus;
        }

        if (
               AnscXmlIsStringStartWith
                  (
                      pNodesOfNode,
                      length - ( pNodesOfNode - pBackupContent),
                      XML_END_NODE
                  )
           )
        {
            break;
        }

        /*
         * generate a new tree node
         */
        pNewNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate(pNode->hOwnerContext);

        if ( !pNewNode )
        {
            ANSC_XML_TRACE("Failed to allocate child Node for  - %s\n", pNode->Name);
            return ANSC_STATUS_XML_RESOURCES;
        }

        pNewNode->hParentNode   = (ANSC_HANDLE)pNode;
        pNewNode->Level         = pNode->Level + 1;
        pNewNode->hXMLContext   = pNode->hXMLContext;

        /*
         * feed this child by calling ourselves again and again ...
         */
        ulBufLen      = length - ( pNodesOfNode - pBackupContent);
        ulPrevBufLen  = ulBufLen;

        returnStatus  = AnscXmlDomNodeDecode
                            (
                                (ANSC_HANDLE)pNewNode,
                                pNodesOfNode,
                                &ulBufLen
                            );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- Failed to call AnscXmlDomNodeDecode.\n");
            AnscFreeMemory( pNewNode );

            return returnStatus;
        }

        pNodesOfNode    += (ulPrevBufLen - ulBufLen);

        AnscQueuePushEntry(&pNode->ChildNodeQueue, &pNewNode->Linkage);

        pNodesOfNode = AnscXmlGotoFirstCharExcept
                          (
                              pNodesOfNode,
                              length - ( pNodesOfNode - pBackupContent),
                              SPECIAL_TOKEN
                          );
    }

    if( !pNodesOfNode )
    {
        ANSC_XML_TRACE("Cannot find the end tag. \n");
        return ANSC_STATUS_XML_MISSED_END_TAG;
    }

    /*
     * If we're here, that's the end of the Node "</???>";
     */
    pNodesOfNode +=  AnscSizeOfString( XML_END_NODE );

    /*
     *  Make sure the name in the end tag is the same;
     */
    if (
           !AnscXmlIsStringStartWith
               (
                   pNodesOfNode,
                   length - ( pNodesOfNode - pBackupContent),
                   (PCHAR)pNode->Name
               )
        )
    {
        ANSC_XML_TRACE("AnscXmlDomNodeDecode() -- It is not the same name in the end tag : %s\n", pNode->Name);

        return ANSC_STATUS_XML_MISSED_END_TAG;
    }

    /*
     *  Make sure we find the Node End ">";
     */
    pNodesOfNode += AnscSizeOfString(pNode->Name);
    pNodesOfNode  = AnscXmlGotoFirstCharExcept
                        (
                            pNodesOfNode,
                            length - ( pNodesOfNode - pBackupContent),
                            SPECIAL_TOKEN
                        );

    if( pNodesOfNode[0] != XML_TAG_END[0])
    {
        ANSC_XML_TRACE("The end tag is missed.\n");

        return ANSC_STATUS_XML_MISSED_END_TAG;
    }

    pNodesOfNode += 1;
    pNodesOfNode  = AnscXmlGotoFirstCharExcept
                       (
                           pNodesOfNode,
                           length - ( pNodesOfNode - pBackupContent),
                           SPECIAL_TOKEN
                       );

    *ppContent    = pNodesOfNode;

    *pulSize      -= (pNodesOfNode - pBackupContent);

    return ANSC_STATUS_SUCCESS;

}
