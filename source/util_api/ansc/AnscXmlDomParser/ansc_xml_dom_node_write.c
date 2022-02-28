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

    module: ansc_xml_dom_node_write.c

        For BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xml Node Object.

        *   AnscXmlDomNodeSetAttrString
        *   AnscXmlDomNodeSetAttrUlong
        *   AnscXmlDomNodeSetAttrBoolean
        *   AnscXmlDomNodeSetAttrBinary
        *   AnscXmlDomNodeSetDataString
        *   AnscXmlDomNodeSetDataUlong
        *   AnscXmlDomNodeSetDataBoolean
        *   AnscXmlDomNodeSetDataBinary

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

        ANSC_STATUS
        AnscXmlDomNodeSetAttrString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                ULONG                       ulSize
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                char*                       sTarget
                Specifies the input buffer where the information is
                placed.

                ULONG                       ulSize
                Specifies the size of the target buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetAttrString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    PANSC_XML_ATTRIBUTE             pAttribute = NULL;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     *  make sure it's a valid attribute Name;
     */
    if( !name || !AnscXmlIsValidAttributeName(name))
    {
        if( name)
        {
            ANSC_XML_TRACE
               (
                   "AnscXmlDomNodeSetAttrString() -- Invalid Attribute Name : %s\n",
                   name
               );
        }
        else
        {
            ANSC_XML_TRACE("AnscXmlDomNodeSetAttrString() -- Empty Attribute Name. \n");
        }

        return ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME;
    }

    /*
     *  make sure it's valid string value
     */
    if( !sTarget || !AnscXmlIsValidAttributeValue(sTarget))
    {
        if( sTarget )
        {
            ANSC_XML_TRACE
               (
                   "AnscXmlDomNodeSetAttrString() -- Invalid Attribute Data : %s\n",
                   name
               );
        }
        else
        {
            ANSC_XML_TRACE("AnscXmlDomNodeSetAttrString() -- Empty Attribute Data. \n");
        }
    }

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, (PUCHAR)name);

    if ( !pAttribute )
    {
        pAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

        if ( !pAttribute )
        {
            return  ANSC_STATUS_XML_RESOURCES;
        }

        AnscQueuePushEntry(&pXmlNode->AttributesList, &pAttribute->Linkage);
    }
    else
    {
        if ( pAttribute->StringData != NULL )
        {
            AnscFreeMemory(pAttribute->StringData);
        }
    }

    pAttribute->hXMLContext   = pXmlNode->hXMLContext;
/*
    pAttribute->bEncrypted    = bApplySecurity;
*/
    pAttribute->DataSize      = ulSize;
    pAttribute->StringData    = AnscXmlEscapeChars(sTarget, ulSize, &pAttribute->DataSize);

    AnscCopyString(pAttribute->Name, name);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetAttrUlong
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                ULONG                       ulTarget
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                ULONG                       ulTarget
                Specifies the input buffer where the information is
                placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetAttrUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulTarget
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    PANSC_XML_ATTRIBUTE             pAttribute = NULL;
    char                            pValue[64] = { 0 };

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    if( !name || !AnscXmlIsValidAttributeName(name))
    {
        if( name )
        {
            ANSC_XML_TRACE
               (
                   "AnscXmlDomNodeSetAttrUlong() -- Invalid Attribute Name : %s \n",
                   name
               );
        }
        else
        {
            ANSC_XML_TRACE("AnscXmlDomNodeSetAttrUlong() -- Empty  Attribute Name \n");
        }

        return ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME;
    }

    if ( AnscSizeOfString(name) > MAXIMUM_ATTRIBUTE_NAME )
    {
        return  ANSC_STATUS_XML_INTERNAL_ERROR;
    }

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, (PUCHAR)name);

    if ( !pAttribute )
    {
        pAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

        if ( !pAttribute )
        {
            return  ANSC_STATUS_XML_RESOURCES;
        }

        AnscQueuePushEntry(&pXmlNode->AttributesList, &pAttribute->Linkage);
    }
    else
    {
        if ( pAttribute->StringData != NULL )
        {
            AnscFreeMemory(pAttribute->StringData);
        }
    }

    /*
     * copy the data into the new Attribute
     */
    pAttribute->hXMLContext     = pXmlNode->hXMLContext;
    /*
    pAttribute->bEncrypted      = bApplySecurity;
    */
    AnscXmlLongToString( ulTarget, pValue);

    pAttribute->DataSize        = AnscSizeOfString(pValue);
    pAttribute->StringData      = (PCHAR)AnscAllocateMemory( AnscSizeOfString(pValue) + 1);

    if( pAttribute->StringData == NULL)
    {
        pAttribute->DataSize = 0;
    }
    else
    {
        AnscCopyMemory( (PVOID)pAttribute->StringData, pValue, AnscSizeOfString(pValue));
        AnscCopyString(pAttribute->Name, name);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetAttrBoolean
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                BOOL                        bTarget
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                BOOL                        bTarget
                Specifies the input buffer where the information is
                placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetAttrBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bTarget
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    PANSC_XML_ATTRIBUTE             pAttribute = NULL;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    if( !name || !AnscXmlIsValidAttributeName(name))
    {
        if( name )
        {
            ANSC_XML_TRACE
               (
                   "AnscXmlDomNodeSetAttrBoolean() -- Invalid Attribute Name : %s \n",
                   name
               );
        }
        else
        {
            ANSC_XML_TRACE("AnscXmlDomNodeSetAttrBoolean() -- Empty  Attribute Name \n");
        }

        return ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME;
    }

    if ( AnscSizeOfString(name) > MAXIMUM_ATTRIBUTE_NAME )
    {
        ANSC_XML_TRACE
           (
               "AnscXmlDomNodeSetAttrBoolean() -- Attribute Name's too long : %s\n",
               name
           );

        return  ANSC_STATUS_XML_INTERNAL_ERROR;
    }


    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, (PUCHAR)name);

    if ( !pAttribute )
    {
        pAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

        if ( !pAttribute )
        {
            return  ANSC_STATUS_XML_RESOURCES;
        }

        AnscQueuePushEntry(&pXmlNode->AttributesList, &pAttribute->Linkage);
    }
    else
    {
        if ( pAttribute->StringData != NULL )
        {
            AnscFreeMemory(pAttribute->StringData);
        }
    }

    /*
     * copy the data into the new Attribute
     */
    pAttribute->hXMLContext   = pXmlNode->hXMLContext;
    /*
    pAttribute->bEncrypted    = bApplySecurity;
    */

    if( bTarget )
    {
        pAttribute->StringData    = (PCHAR)AnscAllocateMemory(AnscSizeOfString((LPSTR)STR_TRUE) + 1);

        if( pAttribute->StringData != NULL)
        {
            AnscCopyString((PCHAR)pAttribute->StringData, (LPSTR)STR_TRUE);

            pAttribute->DataSize      = AnscSizeOfString((LPSTR)STR_TRUE);
        }
        else
        {
            pAttribute->DataSize      = 0;
        }
    }
    else
    {
        pAttribute->StringData    = (PCHAR)AnscAllocateMemory(AnscSizeOfString((LPSTR)STR_FALSE) + 1);

        if( pAttribute->StringData != NULL)
        {
            AnscCopyString((PCHAR)pAttribute->StringData, (LPSTR)STR_FALSE);

            pAttribute->DataSize      =    AnscSizeOfString((LPSTR)STR_FALSE);
        }
        else
        {
            pAttribute->DataSize      = 0;
        }
    }

    AnscCopyString(pAttribute->Name, name);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetAttrBinary
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                ULONG                       ulSize
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                char*                       sTarget
                Specifies the input buffer where the information is
                placed.

                ULONG                       ulSize
                Specifies the size of the target buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetAttrBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    )
{
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    PANSC_XML_ATTRIBUTE             pAttribute = NULL;
    ULONG                           length = 0; /*RDKB-6190, CID-24598, initialize before use*/


    /***********************************************************
           CREATE A NEW Attribute UNDER THE CURRENT TREE NODE
    ***********************************************************/

    /*
     * make sure this node is valid
     */
    if ( !hThisObject || !sTarget || ulSize == 0)
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    if( !name || !AnscXmlIsValidAttributeName(name))
    {
        if( name )
        {
            ANSC_XML_TRACE
               (
                   "AnscXmlDomNodeSetAttrBinary() -- Invalid Attribute Name : %s \n",
                   name
               );
        }
        else
        {
            ANSC_XML_TRACE("AnscXmlDomNodeSetAttrBinary() -- Empty  Attribute Name \n");
        }

        return ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME;
    }

    if ( AnscSizeOfString(name) > MAXIMUM_ATTRIBUTE_NAME )
    {
        ANSC_XML_TRACE
           (
               "AnscXmlDomNodeSetAttrBinary() -- SizeOf Attribute Name is too long: %s\n",
               name
           );

        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, (PUCHAR)name);

    if ( !pAttribute )
    {
        pAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

        if ( !pAttribute )
        {
            return  ANSC_STATUS_XML_RESOURCES;
        }

        AnscQueuePushEntry(&pXmlNode->AttributesList, &pAttribute->Linkage);
    }
    else
    {
        if ( pAttribute->StringData != NULL )
        {
            AnscFreeMemory(pAttribute->StringData);
        }
    }

    /*
     * copy the data into the new Attribute
     */
    pAttribute->hXMLContext   = pXmlNode->hXMLContext;
    /*
    pAttribute->bEncrypted    = bApplySecurity;
    */
    pAttribute->DataSize      = ulSize * 2;

    pAttribute->StringData    = (PCHAR)AnscAllocateMemory(ulSize * 2 + 1);

    if ( !pAttribute->StringData )
    {
        ANSC_XML_TRACE("AnscXmlDomNodeSetAttrBinary() -- Failed to allocateMemory .\n");

        return  ANSC_STATUS_XML_RESOURCES;
    }

    AnscCopyMemory(pAttribute->StringData, sTarget, ulSize);

    AnscXmlFromBinaryToASCII
        (
            pAttribute->StringData,
            &length
        );

    AnscCopyString(pAttribute->Name, name);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetDataString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                ULONG                       ulSize
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                char*                       sTarget
                Specifies the input buffer where the information is
                placed.

                ULONG                       ulSize
                Specifies the size of the target buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetDataString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(name);
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * make sure this value is valid;
     */
    if( !AnscXmlIsValidNodeText( sTarget ))
    {
        return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
    }

    /*
     *  Free memory first;
     */
    if ( pXmlNode->StringData != NULL )
    {
        AnscFreeMemory(pXmlNode->StringData);
    }

    pXmlNode->DataSize      = ulSize;
    pXmlNode->StringData    = AnscXmlEscapeChars(sTarget, ulSize, &pXmlNode->DataSize);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetDataUlong
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                ULONG                       ulTarget
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                ULONG                       ulTarget
                Specifies the input buffer where the information is
                placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetDataUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       ulTarget
    )
{
    UNREFERENCED_PARAMETER(name);
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    char                            pValue[64] = { 0 };

    /***********************************************************
           CREATE A NEW Attribute UNDER THE CURRENT TREE NODE
    ***********************************************************/

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    if (pXmlNode->StringData)
    {
        AnscFreeMemory(pXmlNode->StringData);
    }

    /*
     * copy the data
     */

    AnscXmlLongToString(ulTarget, pValue);

    pXmlNode->DataSize   =  AnscSizeOfString(pValue);
    pXmlNode->StringData = (PCHAR)AnscAllocateMemory(AnscSizeOfString(pValue) + 16);

    if( pXmlNode->StringData != NULL)
    {
        AnscCopyMemory((PVOID)pXmlNode->StringData, pValue, AnscSizeOfString(pValue));
    }
    else
    {
        pXmlNode->DataSize = 0;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetDataBoolean
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                BOOL                        bTarget
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                BOOL                        bTarget
                Specifies the input buffer where the information is
                placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetDataBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        BOOL                        bTarget
    )
{
    UNREFERENCED_PARAMETER(name);
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    if ( pXmlNode->StringData != NULL )
    {
        AnscFreeMemory(pXmlNode->StringData);
    }

    if( bTarget )
    {
        pXmlNode->StringData    = (PCHAR)AnscAllocateMemory(AnscSizeOfString((LPSTR)STR_TRUE) + 16);

        if( pXmlNode->StringData != NULL)
        {
            pXmlNode->DataSize      = AnscSizeOfString((LPSTR)STR_TRUE);
            AnscCopyString((PCHAR)pXmlNode->StringData, (LPSTR)STR_TRUE);
        }
        else
        {
            pXmlNode->DataSize      = 0;
        }
    }
    else
    {
        pXmlNode->StringData    = (PCHAR)AnscAllocateMemory(AnscSizeOfString((LPSTR)STR_FALSE) + 16);

        if( pXmlNode->StringData != NULL)
        {
            pXmlNode->DataSize      = AnscSizeOfString((LPSTR)STR_FALSE);
            AnscCopyString((PCHAR)pXmlNode->StringData, (LPSTR)STR_FALSE);
        }
        else
        {
            pXmlNode->DataSize      = 0;
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeSetDataBinary
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                ULONG                       ulSize
            );

    description:

        This function is called to configure information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be configured.

                char*                       sTarget
                Specifies the input buffer where the information is
                placed.

                ULONG                       ulSize
                Specifies the size of the target buffer.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeSetDataBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(name);
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    ULONG                           length;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject || !sTarget || ulSize == 0)
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    if (pXmlNode->StringData != NULL )
    {
        AnscFreeMemory(pXmlNode->StringData);
    }

    /*
     * copy the data into the new Attribute
     */
    pXmlNode->DataSize    = ulSize * 2;
    pXmlNode->StringData  = AnscAllocateMemory(ulSize * 2 + 16);

    if ( !pXmlNode->StringData )
    {
        ANSC_XML_TRACE("AnscXmlDomNodeSetDataBinary() -- Failed to allocateMemory .\n");

        return  ANSC_STATUS_XML_RESOURCES;
    }

    length = ulSize;
    AnscCopyMemory(pXmlNode->StringData, sTarget, ulSize);

    AnscXmlFromBinaryToASCII
        (
            pXmlNode->StringData,
            &length
        );

    return  ANSC_STATUS_SUCCESS;
}
