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

    module:	ansc_xml_parser_operation.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xml Parser Object.

        *   AnscXmlDomParserIsQualified
        *   AnscXmlDomParserIsCompleted
        *   AnscXmlDomParserGetEncodedSize
        *   AnscXmlDomParserEncode
        *   AnscXmlDomParserDecode

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


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscXmlDomParserIsQualified
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function examines the input data stream to see if it's in
        valid xml format.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the buffer where the encoded data will be put.

                ULONG                       ulSize
                Specifies the size of the data stream.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
AnscXmlDomParserIsQualified
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        AnscXmlDomParserIsCompleted
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       buffer,
                ULONG                       ulSize
            );

    description:

        This function examines the input data stream to see if it's in
        completed xml format.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       buffer
                Specifies the buffer where the encoded data will be put.

                ULONG                       ulSize
                Specifies the size of the data stream.

    return:     TRUE or FALSE.

**********************************************************************/

BOOL
AnscXmlDomParserIsCompleted
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(buffer);
    UNREFERENCED_PARAMETER(ulSize);
    return  FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscXmlDomParserGetEncodedSize
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
AnscXmlDomParserGetEncodedSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_PARSER_OBJECT     pXmlParser    = (PANSC_XML_DOM_PARSER_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode     = (PANSC_XML_DOM_NODE_OBJECT)pXmlParser->hRootNode;
    ULONG                           ulEncodedSize = 0;

    if ( pRootNode )
    {
        ulEncodedSize += AnscXmlDomNodeGetEncodedSize((ANSC_HANDLE)pRootNode);
    }

    return  ulEncodedSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomParserEncode
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
AnscXmlDomParserEncode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_PARSER_OBJECT     pXmlParser    = (PANSC_XML_DOM_PARSER_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode     = (PANSC_XML_DOM_NODE_OBJECT)pXmlParser->hRootNode;

    if ( pRootNode )
    {
        returnStatus = AnscXmlDomNodeEncode((ANSC_HANDLE)pRootNode, buffer, pulSize);
    }
    else
    {
        returnStatus = ANSC_STATUS_UNAPPLICABLE;
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomParserDecode
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
AnscXmlDomParserDecode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_PARSER_OBJECT     pXmlParser    = (PANSC_XML_DOM_PARSER_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode     = (PANSC_XML_DOM_NODE_OBJECT)pXmlParser->hRootNode;

    if ( pRootNode )
    {
        returnStatus = AnscXmlDomNodeDecode((ANSC_HANDLE)pRootNode, buffer, pulSize);
    }
    else
    {
#ifndef  _BYPASS_XML_DOM_NODE_DECODING
        /* call XML DOM node's decoding function recursively to build a DOM tree */
        PUCHAR                      pBuf    = (PUCHAR)buffer;
        ULONG                       Length  = *pulSize;

        returnStatus = AnscXmlFindNode
                          (
                              (CHAR**)&pBuf,
                              Length,
                              0
                          );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            return returnStatus;
        }

        pRootNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate((ANSC_HANDLE)pXmlParser);

        if ( !pRootNode )
        {
            return  ANSC_STATUS_RESOURCES;
        }
        else
        {
            pXmlParser->hRootNode = (ANSC_HANDLE)pRootNode;
        }

        AnscXmlDomNodeSetParentNode((ANSC_HANDLE)pRootNode, (ANSC_HANDLE)NULL);
        AnscXmlDomNodeDecode       ((ANSC_HANDLE)pRootNode, pBuf, pulSize);

#else
        /* call a utility function directly (from AL_XML library) */
        ANSC_HANDLE                 hNode;
        PANSC_XML_DOM_NODE_OBJECT   pNode;

        hNode   = AnscXmlDomParseString((ANSC_HANDLE)NULL, (PCHAR*)&buffer, *pulSize);

        if (hNode)
        {
            pXmlParser->hRootNode   = hNode;
            pNode   = (PANSC_XML_DOM_NODE_OBJECT)hNode;
            pNode->SetParentNode(hNode, (ANSC_HANDLE)NULL);
        }
#endif
    }

    return  returnStatus;
}

