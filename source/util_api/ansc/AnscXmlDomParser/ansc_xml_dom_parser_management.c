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

    module:	ansc_xml_dom_parser_management.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xml Parser Object.

        *   AnscXmlDomParserGetRootNode
        *   AnscXmlDomParserSetRootNode

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

#define  AnscXmlWarning(arg...)             fprintf(stderr, arg)

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomParserGetRootNode
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the root node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     handle of the root node.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomParserGetRootNode
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_PARSER_OBJECT     pXmlParser    = (PANSC_XML_DOM_PARSER_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode     = (PANSC_XML_DOM_NODE_OBJECT)pXmlParser->hRootNode;

    return  (ANSC_HANDLE)pRootNode;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomParserSetRootNode
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to configure the root node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the node name of the root node.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomParserSetRootNode
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_XML_DOM_PARSER_OBJECT     pXmlParser    = (PANSC_XML_DOM_PARSER_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode     = (PANSC_XML_DOM_NODE_OBJECT)pXmlParser->hRootNode;

    if( name == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( ANSC_OBJECT_NAME_SIZE <= AnscSizeOfString(name))
    {
        AnscXmlWarning
            (
                "Invalid XML root node name (too big)- '%s'\n",
                name
            );

        return ANSC_STATUS_FAILURE;
    }

    if ( pRootNode )
    {
    	AnscXmlDomNodeSetName((ANSC_HANDLE)pRootNode, name);
    }
    else
    {
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
        AnscXmlDomNodeSetName      ((ANSC_HANDLE)pRootNode, name);
    }

    return  ANSC_STATUS_SUCCESS;
}
