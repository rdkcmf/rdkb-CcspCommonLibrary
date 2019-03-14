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

    module:	ansc_xml_dom_parser_external_api.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the external functions provided by the xml parser (DOM).

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


#ifndef  _ANSC_XML_DOM_PARSER_EXTERNAL_API_
#define  _ANSC_XML_DOM_PARSER_EXTERNAL_API_


/***********************************************************
    FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_PARSER_INTERFACE.C
***********************************************************/

ANSC_HANDLE
AnscCreateXmlDomParser
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_HANDLE
AnscCreateXmlDomNode
    (
        ANSC_HANDLE                 hOwnerContext
    );

/***********************************************************
    FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_PARSER_UTIL.C
***********************************************************/
ANSC_HANDLE
AnscXmlDomParseString
    (
        ANSC_HANDLE                 hOwnerContext,
        PCHAR                       *ppXMLString,
        ULONG                       length
    );


/***********************************************************
       FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_PARSER_BASE.C
***********************************************************/

ANSC_HANDLE
AnscXmlDomParserCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscXmlDomParserRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXmlDomParserReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
    FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_PARSER_OPERATION.C
***********************************************************/

BOOL
AnscXmlDomParserIsQualified
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

BOOL
AnscXmlDomParserIsCompleted
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        ULONG                       ulSize
    );

ULONG
AnscXmlDomParserGetEncodedSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXmlDomParserEncode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscXmlDomParserDecode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );


/***********************************************************
    FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_PARSER_MANAGEMENT.C
***********************************************************/

ANSC_HANDLE
AnscXmlDomParserGetRootNode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXmlDomParserSetRootNode
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );


/***********************************************************
        FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_NODE_BASE.C
***********************************************************/

ANSC_HANDLE
AnscXmlDomNodeCreate
    (
        ANSC_HANDLE                 hOwnerContext
    );

ANSC_STATUS
AnscXmlDomNodeRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXmlDomNodeReset
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
     FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_NODE_OPERATION.C
***********************************************************/

ULONG
AnscXmlDomNodeGetStatus
    (
        ANSC_HANDLE                 hThisObject
    );

ULONG
AnscXmlDomNodeGetEncodedSize
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXmlDomNodeEncode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscXmlDomNodeDecode
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       buffer,
        PULONG                      pulSize
    );


/***********************************************************
    FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_NODE_MANAGEMENT.C
***********************************************************/

char*
AnscXmlDomNodeGetName
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXmlDomNodeSetName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_HANDLE
AnscXmlDomNodeGetParentNode
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
AnscXmlDomNodeSetParentNode
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hParentNode
    );

ANSC_HANDLE
AnscXmlDomNodeAddChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode
    );

ANSC_HANDLE
AnscXmlDomNodeAddChildByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
AnscXmlDomNodeDelChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode
    );

ANSC_STATUS
AnscXmlDomNodeDelChildByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
AnscXmlDomNodeInsertChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hChildNode,
        ANSC_HANDLE                 hAfterChild
    );

ANSC_HANDLE
AnscXmlDomNodeRemoveChild
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscXmlDomNodeGetChildByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_HANDLE
AnscXmlDomNodeGetHeadChild
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_HANDLE
AnscXmlDomNodeGetNextChild
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hThisChild
    );

ANSC_HANDLE
AnscXmlDomNodeGetTailChild
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_PARSER_READ.C
***********************************************************/

ANSC_STATUS
AnscXmlDomNodeGetItemString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscXmlDomNodeGetItemUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    );

ANSC_STATUS
AnscXmlDomNodeGetItemBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    );

ANSC_STATUS
AnscXmlDomNodeGetItemBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

ULONG
AnscXmlDomNodeGetItemSize
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
AnscXmlDomNodeGetAttrString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscXmlDomNodeGetAttrUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    );

ANSC_STATUS
AnscXmlDomNodeGetAttrBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    );

ANSC_STATUS
AnscXmlDomNodeGetAttrBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

ULONG
AnscXmlDomNodeGetAttrSize
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );

ANSC_STATUS
AnscXmlDomNodeGetDataString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

ANSC_STATUS
AnscXmlDomNodeGetDataUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    );

ANSC_STATUS
AnscXmlDomNodeGetDataLong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PLONG                       plTarget
    );

ANSC_STATUS
AnscXmlDomNodeGetDataBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    );

ANSC_STATUS
AnscXmlDomNodeGetDataBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    );

ULONG
AnscXmlDomNodeGetDataSize
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    );


/***********************************************************
      FUNCTIONS IMPLEMENTED IN ANSC_XML_DOM_PARSER_WRITE.C
***********************************************************/

ANSC_STATUS
AnscXmlDomNodeSetAttrString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscXmlDomNodeSetAttrUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulTarget
    );

ANSC_STATUS
AnscXmlDomNodeSetAttrBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bTarget
    );

ANSC_STATUS
AnscXmlDomNodeSetAttrBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscXmlDomNodeSetDataString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

ANSC_STATUS
AnscXmlDomNodeSetDataUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulTarget
    );

ANSC_STATUS
AnscXmlDomNodeSetDataBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bTarget
    );

ANSC_STATUS
AnscXmlDomNodeSetDataBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    );

ANSC_HANDLE
AnscXmlDomNodeGetAttr
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pAttrName
    );

ANSC_HANDLE
AnscXmlDomNodeCopy
    (
        ANSC_HANDLE                 hNode
    );

ANSC_HANDLE
AnscXmlDomNodeGetFirstAttr
    (
        ANSC_HANDLE                 hNode
    );

ANSC_HANDLE
AnscXmlDomNodeGetNextAttr
    (
        ANSC_HANDLE                 hNode,
        ANSC_HANDLE                 hAttr
    );

ULONG
AnscXmlDomNodeGetLevel
    (
        ANSC_HANDLE                 hNode
    );

void
AnscXmlDomNodeSetLevel
    (
        ANSC_HANDLE                 hNode,
        ULONG                       ulLevel
    );

BOOL
AnscXmlDomNodeEnumChild
    (
        ANSC_HANDLE                 hNode,
        PFN_XML_NODE_ENUM_KID_PROC  Proc,
        PVOID                       pData
    );


#endif
