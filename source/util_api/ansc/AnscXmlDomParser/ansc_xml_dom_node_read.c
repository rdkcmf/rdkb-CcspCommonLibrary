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

    module: ansc_xml_dom_node_read.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced functions of the
        General Xml Node Object.

        *   AnscXmlDomNodeGetItemString
        *   AnscXmlDomNodeGetItemUlong
        *   AnscXmlDomNodeGetItemBoolean
        *   AnscXmlDomNodeGetItemBinary
        *   AnscXmlDomNodeGetItemSize
        *   AnscXmlDomNodeGetAttrString
        *   AnscXmlDomNodeGetAttrUlong
        *   AnscXmlDomNodeGetAttrBoolean
        *   AnscXmlDomNodeGetAttrBinary
        *   AnscXmlDomNodeGetAttrSize
        *   AnscXmlDomNodeGetDataString
        *   AnscXmlDomNodeGetDataUlong
        *   AnscXmlDomNodeGetDataLong
        *   AnscXmlDomNodeGetDataBoolean
        *   AnscXmlDomNodeGetDataBinary
        *   AnscXmlDomNodeGetDataSize

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
        AnscXmlDomNodeGetItemString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                char*                       sTarget
                Specifies the output buffer where the information to
                be placed.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the retrieved information.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetItemString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    returnStatus =
    	AnscXmlDomNodeGetAttrString
            (
                (ANSC_HANDLE)pXmlNode,
                name,
                sTarget,
                pulSize
            );

    if ( returnStatus == ANSC_STATUS_CANT_FIND )
    {
        returnStatus =
        	AnscXmlDomNodeGetDataString
                (
                    (ANSC_HANDLE)pXmlNode,
                    name,
                    sTarget,
                    pulSize
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetItemUlong
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PULONG                      pulTarget
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                PULONG                      pulTarget
                Specifies the output buffer where the information to
                be placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetItemUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    returnStatus =
    	AnscXmlDomNodeGetAttrUlong
            (
                (ANSC_HANDLE)pXmlNode,
                name,
                pulTarget
            );

    if ( returnStatus == ANSC_STATUS_CANT_FIND )
    {
        returnStatus =
        	AnscXmlDomNodeGetDataUlong
                (
                    (ANSC_HANDLE)pXmlNode,
                    name,
                    pulTarget
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetItemBoolean
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PBOOL                       pbTarget
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                PBOOL                       pbTarget
                Specifies the output buffer where the information to
                be placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetItemBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    returnStatus =
    	AnscXmlDomNodeGetAttrBoolean
            (
                (ANSC_HANDLE)pXmlNode,
                name,
                pbTarget
            );

    if ( returnStatus == ANSC_STATUS_CANT_FIND )
    {
        returnStatus =
        	AnscXmlDomNodeGetDataBoolean
                (
                    (ANSC_HANDLE)pXmlNode,
                    name,
                    pbTarget
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetItemBinary
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                char*                       sTarget
                Specifies the output buffer where the information to
                be placed.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the retrieved information.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetItemBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    returnStatus =
    	AnscXmlDomNodeGetAttrBinary
            (
                (ANSC_HANDLE)pXmlNode,
                name,
                sTarget,
                pulSize
            );

    if ( returnStatus == ANSC_STATUS_CANT_FIND )
    {
        returnStatus =
        	AnscXmlDomNodeGetDataBinary
                (
                    (ANSC_HANDLE)pXmlNode,
                    name,
                    sTarget,
                    pulSize
                );
    }

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscXmlDomNodeGetItemSize
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

    return:     size of the content.

**********************************************************************/

ULONG
AnscXmlDomNodeGetItemSize
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    ULONG                           ulSize        = 0xFFFFFFFF;

    ulSize =
    	AnscXmlDomNodeGetAttrSize
            (
                (ANSC_HANDLE)pXmlNode,
                name
            );

    if ( ulSize == 0xFFFFFFFF )
    {
        ulSize =
        	AnscXmlDomNodeGetDataSize
                (
                    (ANSC_HANDLE)pXmlNode,
                    name
                );
    }

    return  ulSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetAttrString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                char*                       sTarget
                Specifies the output buffer where the information to
                be placed.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the retrieved information.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetAttrString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_ATTRIBUTE             pAttribute    = NULL;


    /***********************************************************
             FIND THE Attribute UNDER THE CURRENT TREE NODE
    ***********************************************************/

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, name);

    if ( !pAttribute )
    {
        return  ANSC_STATUS_XML_ATTRIBUTE_NOT_EXIST;
    }

    /*
     * read out the data
     */

    if ( *pulSize < pAttribute->DataSize )
    {
        *pulSize = pAttribute->DataSize;

        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    if( sTarget)
    {
        AnscCopyMemory(sTarget, pAttribute->StringData, pAttribute->DataSize);
    }

    if( pulSize )
    {
        *pulSize = pAttribute->DataSize;
    }

    if (*pulSize != 0)
    {
        AnscXmlRemoveCharReference(sTarget, pulSize);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetAttrUlong
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PULONG                      pulTarget
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                PULONG                      pulTarget
                Specifies the output buffer where the information to
                be placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetAttrUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    PANSC_XML_ATTRIBUTE             pAttribute = NULL;
    ULONG                           i;
    char                            c;


    /***********************************************************
             FIND THE Attribute UNDER THE CURRENT TREE NODE
    ***********************************************************/

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, name);

    if ( !pAttribute )
    {
        return  ANSC_STATUS_XML_ATTRIBUTE_NOT_EXIST;
    }

    /* make sure it's a ulong data */
    for( i = 0 ; i < pAttribute->DataSize; i ++)
    {
        c = ((PCHAR)pAttribute->StringData)[i];

        if( ! AnscXmlIsDigit(c))
        {
            return ANSC_STATUS_XML_INVALID_ATTRIBUTE_VALUE;
        }
    }

    /*
     * read out the data
     */
    if( pulTarget )
    {
        *pulTarget = (ULONG)AnscXmlStringToLong((PCHAR)pAttribute->StringData);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetAttrBoolean
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PBOOL                       pbTarget
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                PBOOL                       pbTarget
                Specifies the output buffer where the information to
                be placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetAttrBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    PANSC_XML_ATTRIBUTE             pAttribute = NULL;


    /***********************************************************
             FIND THE Attribute UNDER THE CURRENT TREE NODE
    ***********************************************************/

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, name);

    if ( !pAttribute )
    {
        return  ANSC_STATUS_XML_ATTRIBUTE_NOT_EXIST;
    }

    /*
     * read out the data
     */
    if ( AnscEqualString((LPSTR)(PCHAR)pAttribute->StringData, (LPSTR)STR_FALSE, FALSE))
    {
        *pbTarget = FALSE;
    }
    else if ( AnscEqualString((LPSTR)pAttribute->StringData, (LPSTR)STR_TRUE, FALSE))
    {
        *pbTarget = TRUE;
    }
    else if ( AnscEqualString((LPSTR)(PCHAR)pAttribute->StringData, "0", FALSE))
    {
        *pbTarget = FALSE;
    }
    else if ( AnscEqualString((LPSTR)pAttribute->StringData, "1", FALSE))
    {
        *pbTarget = TRUE;
    }
    else
    {
        return ANSC_STATUS_XML_INVALID_ATTRIBUTE_VALUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetAttrBinary
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                char*                       sTarget
                Specifies the output buffer where the information to
                be placed.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the retrieved information.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetAttrBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * pointer to the new Attribute or an existing Attribute
     */
    PANSC_XML_ATTRIBUTE             pAttribute = NULL;
    ULONG                           i;
    char                            c;


    /***********************************************************
             FIND THE Attribute UNDER THE CURRENT TREE NODE
    ***********************************************************/

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, name);

    if ( !pAttribute )
    {
        return  ANSC_STATUS_XML_ATTRIBUTE_NOT_EXIST;
    }

    /*
     * read out the data
     */
    if ( *pulSize < pAttribute->DataSize )
    {
        *pulSize = pAttribute->DataSize;

        ANSC_XML_TRACE("AnscXmlDomNodeGetAttrBinary() -- Invalid Data Length.\n");

        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    /* make sure it's a binary data */
    pAttribute->DataSize = AnscXmlCombineTokens
                              (
                                  (PCHAR)pAttribute->StringData,
                                  (PCHAR)pAttribute->StringData,
                                  pAttribute->DataSize,
                                  "\t\n"
                              );

    if( pAttribute->DataSize % 2 != 0)
    {
        return ANSC_STATUS_XML_INVALID_ATTRIBUTE_VALUE;
    }

    for( i = 0 ; i < pAttribute->DataSize; i ++)
    {
        c = ((PCHAR)pAttribute->StringData)[i];

        if( !AnscXmlIsValidBinaryChar(c))
        {
            return ANSC_STATUS_XML_INVALID_ATTRIBUTE_VALUE;
        }
    }

    if( sTarget )
    {

        AnscXmlFromASCIIToBinary
            (
                (PCHAR)sTarget,
                pulSize
            );
    }

    *pulSize = pAttribute->DataSize / 2;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscXmlDomNodeGetAttrSize
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

    return:     size of the content.

**********************************************************************/

ULONG
AnscXmlDomNodeGetAttrSize
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;
    PANSC_XML_ATTRIBUTE             pAttribute    = NULL;

    /*
     * try to open this Attribute
     */
    pAttribute = (PANSC_XML_ATTRIBUTE)AnscXmlDomNodeGetAttr(hThisObject, name);

    if ( !pAttribute )
    {
        return  0xFFFFFFFF;
    }

    return pAttribute->DataSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetDataString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                char*                       sTarget
                Specifies the output buffer where the information to
                be placed.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the retrieved information.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetDataString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * check text size;
     */
    if( pXmlNode->DataSize <= 0)
    {
        if ( pulSize )
        {
            *pulSize = 0;
        }

        return ANSC_STATUS_SUCCESS;
    }

    /*
     * read out the data
     */
    if ( sTarget == NULL )
    {
        *pulSize = pXmlNode->DataSize;

        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }
    else if( *pulSize < pXmlNode->DataSize)
    {
        *pulSize = pXmlNode->DataSize;

        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    if( sTarget)
    {
        AnscCopyMemory(sTarget, pXmlNode->StringData, pXmlNode->DataSize);
    }

    if( pulSize )
    {
        *pulSize = pXmlNode->DataSize;
    }

    if (*pulSize != 0)
    {
        AnscXmlRemoveCharReference(sTarget, pulSize);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetDataUlong
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PULONG                      pulTarget
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                PULONG                      pulTarget
                Specifies the output buffer where the information to
                be placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetDataUlong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulTarget
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    ULONG                           i;
    char                            c;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * check text size;
     */
    if( pXmlNode->DataSize <= 0)
    {
        return ANSC_STATUS_XML_EMPTY_TEXT;
    }

    /* make sure it's a ulong data */
    for( i = 0 ; i < pXmlNode->DataSize; i ++)
    {
        c = ((PCHAR)pXmlNode->StringData)[i];

        if( c == '-')
        {
            return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
        }

        if( ! AnscXmlIsDigit(c))
        {
            return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
        }
    }

    /*
     * read out the data
     */
    if( pulTarget )
    {
        *pulTarget = (ULONG)AnscXmlStringToLong((PCHAR)pXmlNode->StringData);
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetDataLong
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PLONG                       plTarget
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                PLONG                      plTarget
                Specifies the output buffer where the information to
                be placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetDataLong
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PLONG                       plTarget
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    ULONG                           i;
    char                            c;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * check text size;
     */
    if( pXmlNode->DataSize <= 0)
    {
        return ANSC_STATUS_XML_EMPTY_TEXT;
    }

    /* make sure it's a ulong data */
    for( i = 0 ; i < pXmlNode->DataSize; i ++)
    {
        c = ((PCHAR)pXmlNode->StringData)[i];

        if( i == 0 && c == '-')
        {
            continue;
        }

        if( ! AnscXmlIsDigit(c))
        {
            return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
        }
    }


    /*
     * read out the data
     */
    if( plTarget )
    {
        *plTarget = (LONG)AnscXmlStringToLong((PCHAR)pXmlNode->StringData);
    }

    if( ((PCHAR)pXmlNode->StringData)[0] != '-' && *plTarget < 0)
    {
        return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
    }

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetDataBoolean
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PBOOL                       pbTarget
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                PBOOL                       pbTarget
                Specifies the output buffer where the information to
                be placed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetDataBoolean
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PBOOL                       pbTarget
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * check text size;
     */
    if( pXmlNode->DataSize <= 0)
    {
        return ANSC_STATUS_XML_EMPTY_TEXT;
    }

    /*
     * read out the data
     */
    if( AnscEqualString((LPSTR)(PCHAR)pXmlNode->StringData, (LPSTR)STR_FALSE, FALSE))
    {
        *pbTarget = FALSE;
    }
    else if ( AnscEqualString((LPSTR)pXmlNode->StringData, (LPSTR)STR_TRUE, FALSE))
    {
        *pbTarget = TRUE;
    }
    else if( AnscEqualString((LPSTR)(PCHAR)pXmlNode->StringData, "0", FALSE))
    {
        *pbTarget = FALSE;
    }
    else if ( AnscEqualString((LPSTR)pXmlNode->StringData, "1", FALSE))
    {
        *pbTarget = TRUE;
    }
    else
    {
        return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscXmlDomNodeGetDataBinary
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                char*                       sTarget,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

                char*                       sTarget
                Specifies the output buffer where the information to
                be placed.

                PULONG                      pulSize
                Specifies the size of the buffer and receives the size
                of the retrieved information.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlDomNodeGetDataBinary
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        char*                       sTarget,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    ULONG                           i;
    char                            c;

    /***********************************************************
             FIND THE Attribute UNDER THE CURRENT TREE NODE
    ***********************************************************/

    /*
     * make sure this node is valid
     */
    if ( !hThisObject )
    {
        return  ANSC_STATUS_XML_INVALID_HANDLE;
    }

    /*
     * check text size;
     */
    if( pXmlNode->DataSize <= 0)
    {
        *pulSize = 0;

        return ANSC_STATUS_XML_EMPTY_TEXT;
    }

    /*
     * read out the data
     */
    if ( sTarget == NULL)
    {
        *pulSize = pXmlNode->DataSize;

        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    /* make sure it's a binary data */
    pXmlNode->DataSize = AnscXmlCombineTokens
                            (
                                (PCHAR)pXmlNode->StringData,
                                (PCHAR)pXmlNode->StringData,
                                pXmlNode->DataSize,
                                "\t\n"
                            );

    if( pXmlNode->DataSize % 2 != 0)
    {
        return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
    }

    for( i = 0 ; i < pXmlNode->DataSize; i ++)
    {
        c = ((PCHAR)pXmlNode->StringData)[i];

        if( !AnscXmlIsValidBinaryChar(c))
        {
            return ANSC_STATUS_XML_INVALID_TEXT_VALUE;
        }
    }

    if( sTarget )
    {
        AnscCopyMemory(sTarget, pXmlNode->StringData, pXmlNode->DataSize);

        AnscXmlFromASCIIToBinary
            (
                (PCHAR)sTarget,
                pulSize
            );
    }

    *pulSize = pXmlNode->DataSize / 2;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscXmlDomNodeGetDataSize
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve information encapsulated
        by the node.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the property to be retrieved.

    return:     size of the content.

**********************************************************************/

ULONG
AnscXmlDomNodeGetDataSize
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;
    PANSC_XML_DOM_NODE_OBJECT       pXmlNode      = (PANSC_XML_DOM_NODE_OBJECT)hThisObject;

    return  pXmlNode->DataSize;
}
