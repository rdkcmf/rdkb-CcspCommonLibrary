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

    module:	ansc_xml_dom_parser_base.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic construction and removal
        functions of the General Xml Parser Object.

        *   AnscXmlDomParserCreate
        *   AnscXmlDomParserRemove
        *   AnscXmlDomParserReset

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

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscXmlDomParserCreate
            (
                ANSC_HANDLE                 hOwnerContext
            );

    description:

        This function constructs the General Xml Parser Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

    return:     newly created socket object.

**********************************************************************/

ANSC_HANDLE
AnscXmlDomParserCreate
    (
        ANSC_HANDLE                 hOwnerContext
    )
{
    PANSC_XML_DOM_PARSER_OBJECT         pXmlParser    = NULL;

    pXmlParser = (PANSC_XML_DOM_PARSER_OBJECT)AnscAllocateMemory(sizeof(ANSC_XML_DOM_PARSER_OBJECT));

    if ( !pXmlParser )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pXmlParser->hOwnerContext   = hOwnerContext;
    pXmlParser->hRootNode       = (ANSC_HANDLE)NULL;

    pXmlParser->Create          = AnscXmlDomParserCreate;
    pXmlParser->Remove          = AnscXmlDomParserRemove;
    pXmlParser->Reset           = AnscXmlDomParserReset;

    pXmlParser->IsQualified     = AnscXmlDomParserIsQualified;
    pXmlParser->GetEncodedSize  = AnscXmlDomParserGetEncodedSize;
    pXmlParser->Encode          = AnscXmlDomParserEncode;
    pXmlParser->Decode          = AnscXmlDomParserDecode;

    pXmlParser->GetRootNode     = AnscXmlDomParserGetRootNode;
    pXmlParser->SetRootNode     = AnscXmlDomParserSetRootNode;

    return  (ANSC_HANDLE)pXmlParser;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomParserRemove
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
AnscXmlDomParserRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_PARSER_OBJECT     pXmlParser    = (PANSC_XML_DOM_PARSER_OBJECT)hThisObject;

    pXmlParser->Reset((ANSC_HANDLE)pXmlParser);
    AnscFreeMemory(pXmlParser);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomParserReset
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
AnscXmlDomParserReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_XML_DOM_PARSER_OBJECT     pXmlParser    = (PANSC_XML_DOM_PARSER_OBJECT)hThisObject;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode     = (PANSC_XML_DOM_NODE_OBJECT)pXmlParser->hRootNode;

    if ( pRootNode )
    {
    	AnscXmlDomNodeRemove((ANSC_HANDLE)pRootNode);

        pXmlParser->hRootNode = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
