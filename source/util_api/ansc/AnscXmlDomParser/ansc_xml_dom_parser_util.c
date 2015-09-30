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

    module: ansc_xml_dom_parser_util.c

        For Service Creation Environment Container (SCEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the basic construction and removal
        functions of the General Xml Node Object.

        *   AnscXmlDomNodeCreate
        *   AnscXmlDomNodeRemove
        *   AnscXmlDomNodeReset
		*   AnscXmlRemoveControlChars

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
		07/12/12    Add "AnscXmlRemoveControlChars"

**********************************************************************/

#include "ansc_xml_dom_parser_global.h"


static LPSTR        gWrong_String[]   =
{
  "<>", "--->"
};


#define  MAX_LEN_PREDEFINED_ENTITIES                10

typedef  struct
_ALXML_PREDEFINED_ENTITIES
{
    UCHAR                           uCharacter;
    UCHAR                           Encode[MAX_LEN_PREDEFINED_ENTITIES];
}
ALXML_PREDEFINED_ENTITIES;

static ULONG Predefined_Count = 5;
static char  Xml_Chars[]      = "<>&'\"";

static const
ALXML_PREDEFINED_ENTITIES
Predefined_XML_Entities[] =
{
    {'&',    "&amp;"},
    {'<',    "&lt;"},
    {'>',    "&gt;"},
    {'\'',   "&apos;"},
    {'"',    "&quot;"}
};

static const
ULONG
Predefined_XML_char_size[] =
{ 5,4,4,6,6};

/* Bin added on 07/11/2012 to ignore Control chars and parse dec/hex encodings */
/* Please check the UTF8 table at : http://www.utf8-chartable.de/ */
static const char hexEncode[] = "&#x";
static const char decEncode[] = "&#";

static const ULONG hexSize = 3;
static const ULONG decSize = 2;


/*
 * This function is implemented for removing character
 * reference beginning with "&#x" and &#" till the closest
 * semicolon. It also covert predefiend entities:
 *
 *          Entities                Char
 *            &amp;                   &
 *            &lt;                    <
 *            &gt;                    >
 *            &apos;                  '
 *            &quot;                  "
 */



ULONG
AnscXmlStringToLong
    (
        char*  pString
    )
{
    return  (LONG)_ansc_atol(pString);
}


VOID
AnscXmlLongToString
    (
        LONG value,
        char* pBuffer
    )
{
   if( pBuffer == NULL)
   {
      return;
   }

   _ansc_ultoa( value, pBuffer, 10);
}

BOOLEAN
AnscXmlIsUpperLetter
    (
        char c
    )
{
    return c >= 'A' && c <= 'Z';
}

BOOLEAN
AnscXmlIsLowerLetter
    (
        char c
    )
{
    return c >= 'a' && c <= 'z';
}

BOOLEAN
AnscXmlIsLetter
    (
        char c
    )
{
    return AnscXmlIsUpperLetter(c) || AnscXmlIsLowerLetter(c);
}

BOOLEAN
AnscXmlIsDigit
    (
        char c
    )
{
    return c >= '0' && c <= '9';
}

ULONG
AnscXmlCombineTokens
    (
        char*  pOutputString,
        char*  pInputString,
        ULONG  ulInputSize,
        char*  charsToSkip
    )
{
    ULONG  counter = 0;
    ULONG  i       = 0;

    for ( i = 0; i < ulInputSize; i++ )
    {
        if ( !AnscXmlCharInString(charsToSkip, pInputString[i]) )
        {
            pOutputString[counter++] = pInputString[i];
        }
    }

    return  counter;
}

BOOLEAN
AnscXmlIsValidBinaryChar
    (
        char c
    )
{

    return AnscXmlIsDigit(c) || ( c >= 'A' && c <= 'F' ) || ( c >= 'a' && c <= 'f' );
}

UCHAR
AnscXmlGetByte
    (
        PCHAR                       pCharData
    )
{
	ULONG							i = 0;
	UCHAR							tempChar = 0x00;
	UCHAR							uChar    = 0x00;

    for ( i = 0; i < 2; i++ )
    {
        tempChar = ((PCHAR)pCharData)[i];

        if ( tempChar <= '9' && tempChar >= '0' )
        {
            tempChar -= '0';
        }
        else if ( tempChar <= 'F' && tempChar >= 'A' )
        {
            tempChar -= 'A';
            tempChar += (UCHAR)10;
        }
        else if ( tempChar <= 'f' && tempChar >= 'a' )
        {
            tempChar -= 'a';
            tempChar += (UCHAR)10;
        }

        uChar += (i % 2 != 0)? tempChar : tempChar << 4;
    }

	return uChar;
}

ANSC_STATUS
AnscXmlFromASCIIToBinary
    (
        PVOID                       pCharData,
        PULONG                      pulSize
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * status of operation
     */
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    /*
     * temporary counters
     */
    ULONG                           i = 0;

    /*
     * pointer to the converted string
     */
    PUCHAR                          pBinaryData = NULL;
    ULONG                           ulDataSize  = *pulSize / 2;
    UCHAR                           tempChar    = 0;


    /***********************************************************
            MAKE THE CONVERSION FROM ASCII TO BINARY DATA
    ***********************************************************/

    /*
     * each binary octet needs two ASCII chars
     */
    if ( (*pulSize % 2) != 0 )
    {
        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    pBinaryData = (PUCHAR)AnscAllocateMemory(ulDataSize);

    if( pBinaryData == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }

    AnscZeroMemory(pBinaryData, ulDataSize);

    for ( i = 0; i < ulDataSize * 2; i++ )
    {
        tempChar = ((PCHAR)pCharData)[i];

        if ( tempChar <= '9' && tempChar >= '0' )
        {
            tempChar -= '0';
        }
        else if ( tempChar <= 'F' && tempChar >= 'A' )
        {
            tempChar -= 'A';
            tempChar += (UCHAR)10;
        }
        else if ( tempChar <= 'f' && tempChar >= 'a' )
        {
            tempChar -= 'a';
            tempChar += (UCHAR)10;
        }
        else
        {
            AnscFreeMemory(pBinaryData);

            return  ANSC_STATUS_XML_INVALID_ATTRIBUTE_VALUE;
        }

        pBinaryData[i / 2] += (i % 2 != 0)? tempChar : tempChar << 4;
    }

    AnscCopyMemory(pCharData, pBinaryData, ulDataSize);

    *pulSize = ulDataSize;

    AnscFreeMemory(pBinaryData);

    return  ANSC_STATUS_SUCCESS;
}

BOOLEAN
AnscXmlCharInString
    (
        char*  pString,
        char  charToFind
    )
{
    if ( !_ansc_strchr(pString, charToFind) )
    {
        return  FALSE;
    }

    return  TRUE;
}

/***********************************************************************************************
 *
 *   New Added one;
 *
 *  [4]    NameChar    ::=    Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
 *  [5]    Name        ::=    (Letter | '_' | ':') (NameChar)*
 *
 *  Only can start with "Letter" or '_' ;
 *
 ***********************************************************************************************/
BOOLEAN
AnscXmlIsValidAttributeName
    (
        PCHAR                       pAttributeName
    )
{
    ULONG       i, Length;
    char        c;

    if( !pAttributeName)
    {
        return FALSE;
    }

    Length = AnscSizeOfString( pAttributeName);

    if( Length == 0)
    {
        return FALSE;
    }

    for( i = 0 ; i < Length ; i ++)
    {
        c = pAttributeName[i];

        if( i == 0)
        {
            if ( !AnscXmlIsLetter(c) && c != '_')
            {
                return FALSE;
            }
        }
        else
        {
            if( !AnscXmlIsLetter(c) && !AnscXmlIsDigit(c) && c != '.'
                && c != '-' && c != '_' && c != ':')
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/***********************************************************************************************
 *
 *   New Added one;
 *
 *   [10]    AttValue    ::=    '"' ([^<&"] | Reference)* '"' |  "'" ([^<&'] | Reference)* "'"
 *
 *
 *   No '<' or '&' or '"' is permitted
 *   Maybe '&" and other words together will be valid, I just ignore it here.
 *
 ***********************************************************************************************/
BOOLEAN
AnscXmlIsValidAttributeValue
    (
        PCHAR                       pAttributeValue
    )
{
    /* will be escaped, doesn't have to validate */

#if 0

    ULONG       i, Length;
    char        c;

    if( !pAttributeValue)
    {
        return FALSE;
    }

    Length = AnscSizeOfString( pAttributeValue);

    for( i = 0 ; i < Length ; i ++)
    {
        c = pAttributeValue[i];

        if( c == '<' || c == '&' || c == '"')
        {
            return FALSE;
        }
    }

#endif

    return TRUE;
}

/**********************************************************************

    UTILITY FUNCTION

    ANSC_STATUS
    ALXMLFromBinaryToASCII
        (
            PCHAR                       pBinaryData,
            PULONG                      pulSize
        )

    This API function will be called convert a binary string to an
    ascii string.

    argument:   PCHAR                   pBinaryData
                Pointer to the start of the item data.

                PULONG                  pulSize
                Specify the size of the item data.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlFromBinaryToASCII
    (
        PVOID                       pBinaryData,
        PULONG                      pulSize
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * status of operation
     */
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    /*
     * temporary counters
     */
    ULONG                           i = 0;

    /*
     * pointer to the converted string
     */
    PUCHAR                          pASCIIData = NULL;
    ULONG                           ulDataSize = *pulSize * 2;
    UCHAR                           tempChar   = 0;

    PCHAR                           pBuffer    = (PCHAR)pBinaryData;

    /*
     * each binary octet needs two ASCII chars
     */
    pASCIIData = (PUCHAR)AnscAllocateMemory(ulDataSize);

    if( pASCIIData == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }

    AnscZeroMemory(pASCIIData, ulDataSize);

    for ( i = 0; i < ulDataSize / 2; i++ )
    {
        pASCIIData[i * 2]     = (pBuffer[i] & 0xF0) >> 4;
        pASCIIData[i * 2 + 1] = pBuffer[i]  & 0x0F;
    }

    for ( i = 0; i < ulDataSize; i++ )
    {
        if ( pASCIIData[i] <= (UCHAR)9 )
        {
            pASCIIData[i] += '0';
        }
        else
        {
            pASCIIData[i] -= (UCHAR)10;
            pASCIIData[i] += 'A';
        }
    }

    AnscCopyMemory(pBinaryData, pASCIIData, ulDataSize);

    *pulSize = ulDataSize;

    AnscFreeMemory(pASCIIData);

    return  ANSC_STATUS_SUCCESS;
}

/***********************************************************************************************
 *
 *   New Added one;
 *
 *   [10]    AttValue    ::=    '"' ([^<&"] | Reference)* '"' |  "'" ([^<&'] | Reference)* "'"
 *
 *
 *   '<' or '&' are not permitted
 *   Maybe '&" and other words together will be valid, I just ignore it here.
 *
 ***********************************************************************************************/
BOOLEAN
AnscXmlIsValidNodeText
    (
        PCHAR                       pTextValue
    )
{
    /* will be escaped, doesn't have to validate */

#if 0

    ULONG       i, Length;
    char        c;

    if( !pTextValue)
    {
        return FALSE;
    }

    Length = AnscSizeOfString( pTextValue);

    for( i = 0 ; i < Length ; i ++)
    {
        c = pTextValue[i];

        if( c == '<' || c == '&')
        {
            return FALSE;
        }
    }
#endif

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscXmlDomParseString
            (
                PCHAR*                      ppXMLString,
                LONG                        length
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

ANSC_HANDLE
AnscXmlDomParseString
    (
        ANSC_HANDLE                 hOwnContext,
        PCHAR                       *ppXMLString,
        ULONG                       length
    )
{
    /*
     * pointer to the root node of the configuration file
     */
    PCHAR                           pSave     = *ppXMLString;
    PANSC_XML_DOM_NODE_OBJECT       pRootNode = NULL;
    ANSC_STATUS                     returnStatus;

    /*
     *  If null string, return null object
     */
    if( *ppXMLString == NULL)
    {
        return NULL;
    }

    /*
     * import the content of configuration file into the internal structure
     */
    returnStatus = AnscXmlFindNode
                      (
                          ppXMLString,
                          length,
                          0
                      );

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return NULL;
    }

    /*
     *  Check the length;
     */
    if( length == 0)
    {
        length = AnscSizeOfString(pSave);
    }

    pRootNode   = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate(hOwnContext);

    if ( !pRootNode )
    {
        returnStatus = ANSC_STATUS_XML_RESOURCES;

        return NULL;
    }

    /*
     * loop to go through all the nodes in the configuration file
     */
    returnStatus = AnscXmlFillContentOfNode(ppXMLString, length - (*ppXMLString - pSave), pRootNode);

    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        return  (ANSC_HANDLE)pRootNode;
    }

    AnscXmlDomNodeRemove(pRootNode);

    return NULL;
}


/**********************************************************************

    UTILITY FUNCTION

    ANSC_STATUS
    AnscXmlFindNode
        (
            PCHAR*                      ppContent,
            LONG                        length,
            ULONG                       ulLayerLevel
        )

    This API function will be called to find the first node in the
    content with required layer level number.

    argument:   PCHAR*                  ppContent
                Pointer to the start of the content.

                LONG                    length
                The length of the content;

                ULONG                   ulLayerLevel
                Specify the number of layer level of the node we're
                looking for.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlFindNode
    (
        PCHAR*                      ppContent,
        LONG                        length,
        ULONG                       ulLayerLevel
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * status of operation
     */
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    /*
     * pointer to the start point of the Attributes
     */
    PCHAR                           pNameOfNodeStart = NULL;
    PCHAR                           pNameOfNodeEnd   = NULL;
    PCHAR                           pBackContent     = *ppContent;

    /*
     * pointer to the node name extracted from the first available node if any
     */
    PCHAR                           pName      = NULL;
    ULONG                           ulNameSize = 0;
    ULONG                           i, ulWrongStringSize ;


    /***********************************************************
                 SEARCH FOR THE FIRST AVAILABLE NODE
    ***********************************************************/
    /*
     *  check the length
     */
    if( pBackContent == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    if( length <= 0)
    {
        length = AnscSizeOfString(pBackContent);
    }

    /*
     * Goto the first useful char;
     */
    pNameOfNodeStart = AnscXmlGotoFirstCharExcept( *ppContent, length, (LPSTR)SPECIAL_TOKEN);
    if (pNameOfNodeStart == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }


    /*
     *  If it's the root level, Check Wrong String exists or not
     */
    if( ulLayerLevel == 0)
    {
        ulWrongStringSize = sizeof(gWrong_String) / sizeof(gWrong_String[0]);

        for( i = 0 ; i < ulWrongStringSize ; i ++)
        {
            if( AnscXmlFindString( pNameOfNodeStart, gWrong_String[i] ) != NULL)
            {
                ANSC_XML_TRACE("Unacceptable string : %s \n", gWrong_String[i]);

                return ANSC_STATUS_XML_INVALID_STRING;
            }
        }

        /*
         * The file must start with '<'
         */

        if ( !AnscXmlIsStringStartWith
              (
                pNameOfNodeStart,
                length - (pNameOfNodeStart - pBackContent),
                (LPSTR)XML_TAG_BEGIN
              )
           )
        {
            return  ANSC_STATUS_XML_INVALID_VERSION;
        }


        /*
         *  Find the XML header start with "<?"
         */
        while( AnscXmlIsStringStartWith
                (
                    pNameOfNodeStart,
                    length - (pNameOfNodeStart - pBackContent),
                    (LPSTR)XML_HEADER_BEGIN
                )
             )
        {
            /*
             *  Check the end of version "?>"
             */
            pNameOfNodeStart = AnscXmlFindString( pNameOfNodeStart, (LPSTR)XML_HEADER_END);

            if( !pNameOfNodeStart )
            {
                return ANSC_STATUS_XML_INVALID_VERSION;
            }

            pNameOfNodeStart += AnscSizeOfString((LPSTR)XML_HEADER_END);
            pNameOfNodeStart  = AnscXmlGotoFirstCharExcept
                                (
                                    pNameOfNodeStart,
                                    length - (pNameOfNodeStart - pBackContent),
                                    SPECIAL_TOKEN
                                );
        }
        /*
         *  Find the XML header start with "<!--"
         */
        while( AnscXmlIsStringStartWith
                (
                    pNameOfNodeStart,
                    length - (pNameOfNodeStart - pBackContent),
                    (LPSTR)XML_COMMENT_BEGIN
                )
             )
        {
            /*
             *  Check the end of version "-->"
             */
            pNameOfNodeStart = AnscXmlFindString( pNameOfNodeStart, (LPSTR)XML_COMMENT_END);

            if( !pNameOfNodeStart )
            {
                return ANSC_STATUS_XML_INVALID_COMMENT_END;
            }

            pNameOfNodeStart += AnscSizeOfString((LPSTR)XML_COMMENT_END);
            pNameOfNodeStart  = AnscXmlGotoFirstCharExcept
                                (
                                    pNameOfNodeStart,
                                    length - (pNameOfNodeStart - pBackContent),
                                    (LPSTR)SPECIAL_TOKEN
                                );
        }
    }
    else
    {

        pNameOfNodeStart  = AnscXmlGotoFirstCharExcept
                            (
                                pNameOfNodeStart,
                                length - (pNameOfNodeStart - pBackContent),
                                (LPSTR)SPECIAL_TOKEN
                            );

        /*
         *  Find the XML header start with "<!--"
         */
        while( AnscXmlIsStringStartWith
                (
                    pNameOfNodeStart,
                    length - (pNameOfNodeStart - pBackContent),
                    (LPSTR)XML_COMMENT_BEGIN
                )
             )
        {
            /*
             *  Check the end of version "-->"
             */
            pNameOfNodeStart = AnscXmlFindString( pNameOfNodeStart, (LPSTR)XML_COMMENT_END);

            if( !pNameOfNodeStart )
            {
                return ANSC_STATUS_XML_INVALID_COMMENT_END;
            }

            pNameOfNodeStart += AnscSizeOfString((LPSTR)XML_COMMENT_END);
            pNameOfNodeStart  = AnscXmlGotoFirstCharExcept
                                (
                                    pNameOfNodeStart,
                                    length - (pNameOfNodeStart - pBackContent),
                                    (LPSTR)SPECIAL_TOKEN
                                );
        }

        /*
         *  Find first '<'
         */
        pNameOfNodeStart = AnscXmlFindString( pNameOfNodeStart, (LPSTR)XML_TAG_BEGIN);

        if( !pNameOfNodeStart )
        {
            return ANSC_STATUS_XML_MISSED_END_TAG;
        }
    }

    *ppContent = pNameOfNodeStart;

    if( pNameOfNodeStart != pBackContent)
    {
        returnStatus = ANSC_STATUS_SUCCESS;
    }

    return  ANSC_STATUS_SUCCESS;
}




/**********************************************************************

    UTILITY FUNCTION

    ANSC_STATUS
    AnscXmlFillContentOfNode
        (
            PCHAR*                      ppContent,
            LONG                        length,
            PANSC_XML_DOM_NODE_OBJECT   pNode
        )

    This API function will be called to extract all the following Attributes
    and nodes from the content buffer and fill into the current node.

    argument:   PCHAR*                      ppContent
                Pointer to the start of the content.

                LONG                        length
                The length of the content;

                PANSC_XML_DOM_NODE_OBJECT   pNode
                Pointer to the current tree node we're going to fill in
                the content.

      return:   status of operation.

**********************************************************************/

ANSC_STATUS
AnscXmlFillContentOfNode
    (
        PCHAR*                      ppContent,
        LONG                        length,
        PANSC_XML_DOM_NODE_OBJECT   pNode
    )
{
    /***********************************************************
                    DEFINITION OF LOCAL VARIABLES
    ***********************************************************/

    /*
     * status of operation
     */
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;

    /*
     * pointer to a new tree node and new tree Attribute
     */
    PANSC_XML_DOM_NODE_OBJECT       pNewNode = NULL;
    PANSC_XML_ATTRIBUTE             pNewAttribute = NULL;

    /*
     * pointer to the start point of the Attributes
     */
    PCHAR                           pBackupContent     = *ppContent;
    PCHAR                           pAttributesOfNode  = *ppContent;
    PCHAR                           pPreviousAttribute = *ppContent;
    PCHAR                           pNodesOfNode       = *ppContent;
    PCHAR                           pPreviousNode      = *ppContent;
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

    AnscZeroMemory(pNewNodeName, MAXIMUM_NODE_NAME);

    pNodesOfNode += AnscSizeOfString(XML_TAG_BEGIN);

    pNodesOfNode  = AnscXmlGotoFirstCharExcept
                    (
                        pNodesOfNode,
                        length - ( pNodesOfNode - pBackupContent),
                        (LPSTR)SPECIAL_TOKEN
                    );

    pEndOfNode    = AnscXmlFindString
                    (
                        pNodesOfNode,
                        (LPSTR)XML_TAG_END
                    );

    /*
     *  If pNodesOfNode is NULL
     */
    if( !pNodesOfNode || !pEndOfNode)
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
        ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Node Name is out of size.");
        AnscFreeMemory(pNewNodeName);

        return  ANSC_STATUS_XML_INVALID_LENGTH;
    }

    AnscCopyMemory( (PVOID)pNewNodeName , (PVOID)pNodesOfNode, (ULONG)(pEndOfNodeName - pNodesOfNode));

    if( !AnscXmlIsValidNodeName( pNewNodeName ))
    {
        ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Invalid Node Name : %s\n", pNewNodeName );
        AnscFreeMemory(pNewNodeName);

        return  ANSC_STATUS_XML_INVALID_NODE_NAME;
    }

    /*
     *  Then set the node name;
     */
    AnscCopyString( pNode->Name, pNewNodeName);
    AnscFreeMemory(pNewNodeName);

    /*
     *  Maybe it's <node/> only, return success (Seldom occurred, but possible );
     */
    if( pEndOfNodeName == pEndOfNode - 1 &&  pEndOfNodeName[0] == XML_NODE_END[0])
    {
        *ppContent         = (PCHAR)(pEndOfNode + 1);

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
            return ANSC_STATUS_RESOURCES;
        }

        AnscZeroMemory(pNewAttributeName, MAXIMUM_ATTRIBUTE_NAME);

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
                AnscFreeMemory(pNewAttributeName);

                return  ANSC_STATUS_XML_EQUAL_SIGN_MISSED;
            }

            if( pEndOfAttribute == pStartOfAttribute)
            {
                AnscFreeMemory(pNewAttributeName);

                return  ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME;
            }

            if ( pEndOfAttribute - pStartOfAttribute > MAXIMUM_ATTRIBUTE_NAME )
            {
                ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Attribute Name is out of size.");
                AnscFreeMemory(pNewAttributeName);

                return  ANSC_STATUS_XML_INVALID_LENGTH;
            }

            AnscCopyMemory( pNewAttributeName, pStartOfAttribute, (ULONG)(pEndOfAttribute - pStartOfAttribute ));

            if( !AnscXmlIsValidAttributeName( pNewAttributeName ) )
            {
                ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Invalid Attribute Name : %s\n", pNewAttributeName);
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
                ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Equal sign between Attribute Name and Value was expected.\n");
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
                ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- A string literal was expected.\n");
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
                ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- A string literal was expected.\n");
                AnscFreeMemory(pNewAttributeName);

                return ANSC_STATUS_XML_STRING_SIGN_MISSED;
            }

            /*
             * generate a new tree Attribute
             */
            pNewAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

            if ( !pNewAttribute )
            {
                AnscFreeMemory(pNewAttributeName);
                ANSC_XML_TRACE("Failed to allocate attribute structure for - '%s'\n", pNode->Name);

                return ANSC_STATUS_XML_RESOURCES;
            }

            AnscZeroMemory(pNewAttribute, sizeof(ANSC_XML_ATTRIBUTE));
            pNewAttribute->hParentNode   = (ANSC_HANDLE)pNode;
            pNewAttribute->hXMLContext   = pNode->hXMLContext;

            AnscCopyString(pNewAttribute->Name, pNewAttributeName);

            if( pEndOfAttrValue >= pStartOfAttrValue)
            {
                pNewAttribute->DataSize      = (ULONG)(pEndOfAttrValue - pStartOfAttrValue);
                pNewAttribute->StringData    = AnscAllocateMemory( pNewAttribute->DataSize + 1);

                if( pNewAttribute->StringData == NULL)
                {
                    AnscFreeMemory(pNewAttributeName);
                    AnscFreeMemory( pNewAttribute );
                    ANSC_XML_TRACE("Failed to allocate attribute value for - '%s'\n", pNode->Name);

                    return ANSC_STATUS_XML_RESOURCES;
                }

                AnscZeroMemory(pNewAttribute->StringData,pNewAttribute->DataSize + 1);
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
                ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Invalid Attribute value: '%s'\n", pNewAttribute->StringData);
                AnscFreeMemory(pNewAttributeName);
                AnscFreeMemory(pNewAttribute->StringData );
                AnscFreeMemory(pNewAttribute );

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
                    AnscFreeMemory(pNewAttributeName);
                    ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- A space after attribute was expected.\n");

                    return ANSC_STATUS_XML_SPACE_NEEDED;
                }
            }
        }

        /*
         *  If it's the end of file, Wrong !!!
         */
        if( !pStartOfAttribute)
        {
            ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Failed to find the attribute.\n");
            AnscFreeMemory(pNewAttributeName);

            return  ANSC_STATUS_XML_MISSED_END_TAG;
        }

        /*
         *  If it starts with "/>", means the end of Node, we can return successfully.
         */
        if( AnscXmlFindString( pStartOfAttribute, XML_NODE_END ) == pStartOfAttribute )
        {
            AnscFreeMemory(pNewAttributeName);

            /*
             *  Don't forget to set the current position of data;
             */
            pStartOfAttribute += AnscSizeOfString(XML_NODE_END);
            *ppContent         = pStartOfAttribute;

            return ANSC_STATUS_SUCCESS;
        }
    }

    /*
     *  Start to read the text
     */
     if( pNewAttributeName != NULL )
     {
        AnscFreeMemory(pNewAttributeName);

        pNewAttributeName = NULL;
     }

    pEndOfNode ++;

    pStartOfText = AnscXmlGotoFirstCharExcept
                       (
                            pEndOfNode,
                            length - ( pEndOfNode - pBackupContent),
                            SPECIAL_TOKEN
                       );

    if( pStartOfText == NULL)
    {
        ANSC_XML_TRACE
            (
                "End of XML. Incomplete Node '%s'\n",
                pNode->Name
            );

        return ANSC_STATUS_XML_RESOURCES;
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
            ANSC_XML_TRACE
                (
                    "Failed to allocate %d memory for text of - '%s'\n",
                    (ULONG)(pEndOfText-pStartOfText),
                    pNode->Name
                );

            return ANSC_STATUS_XML_RESOURCES;
        }

        AnscZeroMemory((PVOID)pNode->StringData, pNode->DataSize + 1);
        AnscCopyMemory( pNode->StringData, pStartOfText, (ULONG)(pEndOfText - pStartOfText));
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
        returnStatus  = AnscXmlFillContentOfNode
                            (
                                &pNodesOfNode,
                                length - ( pNodesOfNode - pBackupContent),
                                pNewNode
                            );

        if ( returnStatus != ANSC_STATUS_SUCCESS )
        {
            ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- Failed to call AnscXmlFillContentOfNode.\n");

            AnscXmlDomNodeRemove(pNewNode);

            return returnStatus;
        }

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
        ANSC_XML_TRACE("AnscXmlFillContentOfNode() -- It is not the same name in the end tag : %s\n", pNode->Name);

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

    return ANSC_STATUS_SUCCESS;
}


char*
AnscXmlGotoFirstCharExcept
    (
        char* pInputString,
        int length,
        char* pExceptString
    )
{
    int     iNumberOfChars = length;
    int     i;

    if( length == 0)
    {
        return NULL;
    }

    for( i = 0 ; i < iNumberOfChars ; i ++ )
    {
        if( !AnscXmlCharInString( pExceptString, pInputString[i] ))
        {
            return pInputString + i;
        }
    }

    return pInputString + i;
}

char*
AnscXmlGetStringEndExcept
    (
        char* pInputString,
        ULONG endPosition,
        char* pExceptString
    )
{
    int     iNumberOfChars = endPosition;
    int     i;

    for( i = iNumberOfChars - 1; i >= 0 ; i -- )
    {
        if( !AnscXmlCharInString( pExceptString, pInputString[i] ))
        {
            return pInputString + i + 1;
        }
    }

    return pInputString;
}

BOOLEAN
AnscXmlIsStringStartWith
    (
        char* pInputString,
        int length,
        char* pStartWithChars
    )
{
    int  iInput = length;
    int  iStart = AnscSizeOfString( pStartWithChars );
    int  i;

    if( length == 0 || iInput < iStart || iInput == 0 || iStart == 0 )
    {
        return FALSE;
    }

    for( i = 0 ; i < iStart ; i ++ )
    {
        if( pStartWithChars[i] != pInputString[i] )
        {
            return FALSE;
        }
    }
    return TRUE;
}

char*
AnscXmlGotoFirstWantedChar
    (
        char* pInputString,
        int length,
        char* pWantedChars
    )
{
    int     iNumberOfChars = length;
    int     i;

    if( length == 0)
    {
        return NULL;
    }

    for( i = 0 ; i < iNumberOfChars ; i ++ )
    {
        if( AnscXmlCharInString( pWantedChars, pInputString[i] ))
        {
            return pInputString + i;
        }
    }

    return pInputString + i;
}



char*
AnscXmlFindString
    (
        char* pString,
        char* pSearchString
    )
{
    return _ansc_strstr( (const char*)pString, (const char*)pSearchString);
}

char*
AnscXmlFindCharacterInRange
    (
        char*  pString,
        char  charToFind,
        ULONG  ulRange
    )
{
    ULONG i;
    for ( i = 0; i < ulRange; i++ )
    {
        if ( pString[i] == charToFind )
        {
            pString += i;

            return  pString;
        }
    }

    return  NULL;
}


ULONG
AnscXmlSizeOfToken
    (
        char*  pString,
        char*  charsToStop,
        ULONG  ulRange
    )
{
    /* caller is responsible for making sure that string begins with valid non-stop char */
    ULONG  i;
    for ( i = 0; i < ulRange; i++ )
    {
        if ( AnscXmlCharInString(charsToStop, *pString) )
        {
            return  i;
        }

        pString++;
    }

    return  i;
}

char*
AnscXmlMoveToNextToken
    (
        char*  pString,
        char*  charsToSkip
    )
{
    while ( AnscXmlCharInString(charsToSkip, *pString) && *pString != 0 )
    {
        pString++;
    }

    return  pString;
}

char*
AnscXmlMoveToPreviousToken
    (
        char*  pString,
        char*  charsToSkip,
        PULONG  pulTokenLength
    )
{
    int     counter = 0;

    while ( AnscXmlCharInString(charsToSkip, *pString) )
    {
        pString--;
    }

    while ( !AnscXmlCharInString(charsToSkip, *pString) )
    {
        pString--;
        counter++;
    }

    pString++;

    *pulTokenLength = counter;

    return  pString;
}

ULONG
AnscXmlNumberOfCharsInString
    (
        char*  pString,
        char  charToFind
    )
{
    int     iNumberOfChars = AnscSizeOfString(pString);
    int     i;
    ULONG   counter        = 0;

    for ( i = 0; i < iNumberOfChars; i++ )
    {
        if ( pString[i] == charToFind )
        {
            counter++;
        }
    }

    return  counter;
}

/**********************************************************************************************
 *
 *   New Added one;
 *
 *  [4]    NameChar    ::=    Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
 *  [5]    Name        ::=    (Letter | '_' | ':') (NameChar)*
 *
 *  Only can start with "Letter" or '_' ;
 *********************************************************************************************/
BOOLEAN
AnscXmlIsValidNodeName
    (
        PCHAR                       pNodeName
    )
{
    ULONG       i, nodeLength;
    char        c;

    if( !pNodeName)
    {
        return FALSE;
    }

    nodeLength = AnscSizeOfString( pNodeName );

    if( nodeLength == 0)
    {
        return FALSE;
    }

    for ( i = 0 ; i < nodeLength ; i ++)
    {
        c = pNodeName[i];

        if( i == 0)
        {
            if ( !AnscXmlIsLetter(c) && c != '_')
            {
                return FALSE;
            }
        }
        else
        {
            if( !AnscXmlIsLetter(c) && !AnscXmlIsDigit(c) && c != '.'
                && c != '-' && c != '_' && c != ':')
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/**********************************************************************

    UTILITY FUNCTION

    ULONG
    AnscXmlNodeEncodeLinefeed
        (
            PVOID                       pBuffer,
            PULONG                      pulBufferSize
        )

    This API function will be called to write a string into
    the given buffer.

    argument:   PVOID                   pBuffer
                Specify the buffer in which the encoded content is gonna
                be saved.

                PULONG                  pulBuffSize
                The size of the buffer specified by "pBuffer" [IN/OUT].
                After this function returns, this value contains the
                available buffer size.

      return:   status of operation.

**********************************************************************/

ULONG
AnscXmlNode2Buf
    (
        PVOID                       pBuffer,
        PULONG                      pulBufferSize,
        PVOID                       pContent,
        ULONG                       ulContent
    )
{
    /* write given content string to encoding buffer */
    ULONG                           ulCopySize;

    ulCopySize  = ulContent;
    if (ulCopySize > *pulBufferSize)
    {
        ulCopySize = *pulBufferSize;
    }

    if (ulCopySize != 0)
    {
        *pulBufferSize -= ulCopySize;

        AnscCopyMemory(pBuffer, pContent, ulCopySize);
    }

    return ulCopySize;
}

/**********************************************************************

    UTILITY FUNCTION

    ULONG
    AnscXmlNodeEncodeLinefeed
        (
            PVOID                       pBuffer,
            PULONG                      pulBufferSize
        )

    This API function will be called to write the specified number
    of spaces into the speicified buffer.

    argument:   PVOID                   pBuffer
                Specify the buffer in which the encoded content is gonna
                be saved.

                PULONG                  pulBuffSize
                The size of the buffer specified by "pBuffer" [IN/OUT].
                After this function returns, this value contains the
                available buffer size.

      return:   status of operation.

**********************************************************************/

ULONG
AnscXmlNodeEncodeSpaces
    (
        PVOID                       pBuffer,
        PULONG                      pulBufferSize,
        ULONG                       ulTimes
    )
{
   ULONG                            i;
   ULONG                            ulCopied, ulTotalCopied;
   PUCHAR                           pBuf;

   ulTotalCopied    = 0;
   pBuf             = (PUCHAR)pBuffer;

   for ( i = 0 ; i < ulTimes ; i ++)
   {
        ulCopied    = AnscXmlNode2Buf(pBuf, pulBufferSize, " ", 1);
        if (ulCopied == 0)
            break;

        ulTotalCopied   += ulCopied;
        pBuf            += ulCopied;
   }

   return ulTotalCopied;
}

/**********************************************************************

    UTILITY FUNCTION

    ULONG
    AnscXmlNodeEncodeLinefeed
        (
            PVOID                       pBuffer,
            PULONG                      pulBufferSize
        )

    This API function will be called to write a line-feed (0x0A or "\n")
    into the specified buffer.

    argument:   PVOID                   pBuffer
                Specify the buffer in which the encoded content is gonna
                be saved.

                PULONG                  pulBuffSize
                The size of the buffer specified by "pBuffer" [IN/OUT].
                After this function returns, this value contains the
                available buffer size.

      return:   status of operation.

**********************************************************************/

ULONG
AnscXmlNodeEncodeLinefeed
    (
        PVOID                       pBuffer,
        PULONG                      pulBufferSize
    )
{
    return AnscXmlNode2Buf(pBuffer, pulBufferSize, "\n", 1);
}


/**********************************************************************

    UTILITY FUNCTION

    ULONG
    AnscXmlEncodeAttr2Buf
        (
            PVOID                       pBuffer,
            PULONG                      pulBuffSize,
            PANSC_XML_ATTRIBUTE         pAttribute
        )

    This API function will be called to write the content of the attribute
    into the file.

    argument:   PVOID                   pBuffer
                Specify the buffer in which the encoded content is gonna
                be saved.

                PULONG                  pulBuffSize
                The size of the buffer specified by "pBuffer" [IN/OUT].
                After this function returns, this value contains the
                available buffer size.

                PAL_XML_ATTRIBUTE       pAttribute
                Pointer to the attribute we're going to write into the file.

      return:   status of operation.

**********************************************************************/

ULONG
AnscXmlEncodeAttr2Buf
    (
        PVOID                       pBuffer,
        PULONG                      pulBuffSize,
        PANSC_XML_ATTRIBUTE         pAttribute
    )
{
    ULONG                           ulCopied, ulTotalCopied;
    PUCHAR                          pBuf;
    ULONG                           ulContent;

    pBuf            = pBuffer;
    ulTotalCopied   = 0;
    /*
     * write the current Attribute name into the configuration file
     */
    ulContent       = AnscSizeOfString(pAttribute->Name);
    ulCopied        = AnscXmlNode2Buf(pBuf, pulBuffSize, pAttribute->Name, ulContent);

    if (ulCopied < ulContent)
    {
        return ulTotalCopied;
    }

    pBuf            += ulCopied;
    ulTotalCopied   += ulCopied;

    ulContent       = AnscSizeOfString((LPSTR)ITEM_EQUAL_MARK);
    ulCopied        = AnscXmlNode2Buf(pBuf, pulBuffSize, ITEM_EQUAL_MARK, ulContent);

    if (ulCopied < ulContent)
    {
        return ulTotalCopied;
    }

    pBuf            += ulCopied;
    ulTotalCopied   += ulCopied;

    ulContent       = AnscSizeOfString((LPSTR)STRING_MARK_1);
    ulCopied        = AnscXmlNode2Buf(pBuf, pulBuffSize, STRING_MARK_1, ulContent);

    if (ulCopied < ulContent)
    {
        return ulTotalCopied;
    }

    pBuf            += ulCopied;
    ulTotalCopied   += ulCopied;

    ulContent       = AnscSizeOfString((LPSTR)pAttribute->StringData);
    ulCopied        = AnscXmlNode2Buf(pBuf, pulBuffSize, pAttribute->StringData, ulContent);

    if (ulCopied < ulContent)
    {
        return ulTotalCopied;
    }

    pBuf            += ulCopied;
    ulTotalCopied   += ulCopied;

    ulContent       = AnscSizeOfString((LPSTR)STRING_MARK_1);
    ulCopied        = AnscXmlNode2Buf(pBuf, pulBuffSize, STRING_MARK_1, ulContent);

    if (ulCopied < ulContent)
    {
        return ulTotalCopied;
    }

    pBuf            += ulCopied;
    ulTotalCopied   += ulCopied;

    return  ulTotalCopied;
}


/**********************************************************************

    UTILITY FUNCTION

        ULONG
        AnscXmlGetAttr2BufSize
            (
                PANSC_XML_ATTRIBUTE         pAttribute
            )

    This API function will be called to get the encoded size of the
    give XML attribute.

    argument:   PAL_XML_ATTRIBUTE       pAttribute
                Pointer to the attribute we're going to write into the file.

      return:   status of operation.

**********************************************************************/

ULONG
AnscXmlGetAttr2BufSize
    (
        PANSC_XML_ATTRIBUTE         pAttribute
    )
{
    ULONG                           ulEncodeSize = 0;

    ulEncodeSize    += AnscSizeOfString(pAttribute->Name);

    ulEncodeSize    += AnscSizeOfString((LPSTR)ITEM_EQUAL_MARK);

    ulEncodeSize    += AnscSizeOfString((LPSTR)STRING_MARK_1);

    ulEncodeSize    += AnscSizeOfString((LPSTR)pAttribute->StringData);

    ulEncodeSize    += AnscSizeOfString((LPSTR)STRING_MARK_1);

    return  ulEncodeSize;
}

/**********************************************************************

    UTILITY FUNCTION

    BOOL
    AnscXmlRemoveControlChars
        (
            PCHAR                       pString,
            PULONG                      pStringLen
        )

    This API function will be called to remove control chars such as "0xC2" and "0xC3".

    argument:   PCHAR                   pString
                Pointer to the string that we're gonna process

                PULONG                  pStringLen
                Length of the string

      return:   whether or not the string was modified.

**********************************************************************/
BOOL
AnscXmlRemoveControlChars
    (
        PCHAR                       pString,
        PULONG                      pStringLen
    )
{
    ULONG                           i, ulStringLen, ulNewStringLen = 0;
    BOOL                            bPosChanged = FALSE;
    UCHAR                           uChar;
	PCHAR							pTemp = pString;
    int                             index;
    int                             nChar = 0;
	ULONG                           posSemicolon;

    if (!pString || !pStringLen)
    {
        return TRUE;
    }

    ulStringLen = *pStringLen;
    i = 0;

    while (i < ulStringLen)
    {
		pTemp = (PCHAR)(pString + i);
        uChar = pString[i];

		i ++;

		if( uChar == 0xC2 || uChar == 0xC3)
		{
			bPosChanged = TRUE;
		}
		else
		{
			if (bPosChanged)
			{
				pString[ulNewStringLen ++]  = uChar;
			}
			else
			{
				ulNewStringLen ++;
			}
		}
    }

    *pStringLen = ulNewStringLen;

    if( bPosChanged)
    {
        AnscZeroMemory((PCHAR)(pString + ulNewStringLen), ulStringLen - ulNewStringLen);
    }

    return bPosChanged;
}


/**********************************************************************

    UTILITY FUNCTION

    BOOL
    AnscXmlRemoveCharReference
        (
            PCHAR                       pString,
            PULONG                      pStringLen
        )

    This API function will be called to get the encoded size of the
    give XML attribute.

    argument:   PCHAR                   pString
                Pointer to the string that we're gonna process

                PULONG                  pStringLen
                Length of the string

      return:   whether or not the string was modified.

**********************************************************************/

BOOL
AnscXmlRemoveCharReference
    (
        PCHAR                       pString,
        PULONG                      pStringLen
    )
{
    ULONG                           i, ulStringLen, ulNewStringLen = 0;
    BOOL                            bPosChanged = FALSE;
    UCHAR                           uChar;
	PCHAR							pTemp = pString;
    int                             index;
    int                             nChar = 0;
	ULONG                           posSemicolon;

    if (!pString || !pStringLen)
    {
        return TRUE;
    }

    ulStringLen = *pStringLen;
    i = 0;

    while (i < ulStringLen)
    {
		pTemp = (PCHAR)(pString + i);
        uChar = pString[i];

		if (uChar != '&')
		{
			i ++;

			if (bPosChanged)
			{
				pString[ulNewStringLen ++]  = uChar;
			}
			else
			{
				ulNewStringLen ++;
			}
			
			continue;
		}
        else if( _ansc_strstr(pTemp, hexEncode) == pTemp)
		{
			/* heximal encode */
            posSemicolon = i + hexSize;

            while (posSemicolon < ulStringLen)
            {
                if (pString[posSemicolon] == ';')
                {
                    break;
                }
                else
                {
                    posSemicolon ++;
                }
            }

            /*
             * Not a hex value encode
             */
            if (posSemicolon >= ulStringLen)
            {
                continue;
            }

			/* caculate the int value */
			pTemp = (PCHAR)(pString + i + hexSize);

			if( (posSemicolon - i) != 2 + hexSize || !AnscXmlIsValidBinaryChar(pTemp[0]) || !AnscXmlIsValidBinaryChar(pTemp[1]))
			{
                i ++;

                if (bPosChanged)
                {
                    pString[ulNewStringLen ++]  = uChar;
                }
                else
                {
                    ulNewStringLen ++;
                }

				continue;
			}
			else
			{
				bPosChanged = TRUE;
				i           = posSemicolon + 1;     /* goto the character after semicolon */
				uChar       = AnscXmlGetByte(pTemp);
				pString[ulNewStringLen++] = uChar;
			}
		}
		else if( _ansc_strstr(pTemp, decEncode) == pTemp)
		{
			BOOL  bValid  = TRUE;

			/* decimal encode */
            posSemicolon = i + decSize;

            while (posSemicolon < ulStringLen)
            {
                if (pString[posSemicolon] == ';')
                {
                    break;
                }
                else
                {
                    posSemicolon ++;
                }
            }

            if (posSemicolon >= ulStringLen)
            {
                continue;
            }

			/* caculate the int value */
			pTemp = (PCHAR)(pString + i + decSize);

			if( (posSemicolon - i) == 2 + decSize)
			{
				if( !AnscXmlIsDigit(pTemp[0]) || !AnscXmlIsDigit(pTemp[1]))
				{
					bValid = FALSE;
				}
			}
			else if( (posSemicolon - i) == 3 + decSize)
			{
				if( !AnscXmlIsDigit(pTemp[0]) || !AnscXmlIsDigit(pTemp[1])|| !AnscXmlIsDigit(pTemp[2]))
				{
					bValid = FALSE;
				}
			}

			if( !bValid)
			{
                i ++;

                if (bPosChanged)
                {
                    pString[ulNewStringLen ++]  = uChar;
                }
                else
                {
                    ulNewStringLen ++;
                }

				continue;
			}
			else
			{
				bPosChanged = TRUE;

				i           = posSemicolon + 1;     /* goto the character after semicolon */
				uChar       = (UCHAR)_ansc_atoi(pTemp);
				pString[ulNewStringLen++] = uChar;
			}
		}
		else
        {
            /*
             * Needs to be translated
             */
            posSemicolon = i + 1;

            while (posSemicolon < ulStringLen)
            {
                if (pString[posSemicolon] == ';')
                {
                    break;
                }
                else
                {
                    posSemicolon ++;
                }
            }

            /*
             * Not a legal value
             */
            if (posSemicolon >= ulStringLen)
            {
                continue;
            }

            for (index = 0; index < (int)Predefined_Count; index ++)
            {
                if (AnscEqualMemory(&pString[i], (PVOID)Predefined_XML_Entities[index].Encode, posSemicolon - i + 1))
                {
                    pString[ulNewStringLen++] = Predefined_XML_Entities[index].uCharacter;
                    bPosChanged = TRUE;
                    i           = posSemicolon + 1;     /* goto the character after semicolon */

                    break;
                }
            }

            if( index >= (int)Predefined_Count)
            {
                i ++;

                if (bPosChanged)
                {
                    pString[ulNewStringLen ++]  = uChar;
                }
                else
                {
                    ulNewStringLen ++;
                }
            }
        }
    }

    *pStringLen = ulNewStringLen;

    if( bPosChanged)
    {
        AnscZeroMemory((PCHAR)(pString + ulNewStringLen), ulStringLen - ulNewStringLen);
    }

    return bPosChanged;
}


/**********************************************************************

    UTILITY FUNCTION

    PCHAR
    AnscXmlEscapeChars
        (
            PCHAR                       pInputString,
            ULONG                       length,
            PULONG                      pNewLength
        )

    This API function will be called to replace the reserved chars to escaped strings.

    argument:   PCHAR                   pInputString
                The input string;

                ULONG                   length
                the length of the string;

                PULONG                      pNewLength
                the new length;

      return:   whether or not the string was modified.

**********************************************************************/
PCHAR
AnscXmlEscapeChars
    (
        PCHAR                       pInputString,
        ULONG                       length,
        PULONG                      pNewLength
    )
{
    ULONG                 charCount  = 0;
    PCHAR                 pOutString = NULL;
    ULONG                 pos        = 0;
    ULONG                 i          = 0;
    ULONG                 j          = 0;
    char                  c          = '0';
	char*			      pFound     = NULL;

    if( pInputString == NULL)
    {
        if( pNewLength != NULL)
        {
            *pNewLength = 0;
        }

        return NULL;
    }

    if( length == 0)
    {
        if( pNewLength != NULL)
        {
            *pNewLength = 0;
        }

        return NULL;
    }

    /* check how many such chars */
    for( i = 0; i < length; i ++)
    {
        c = pInputString[i];

		pFound = _ansc_strchr(Xml_Chars,c);
        if( pFound != NULL)
        {
            charCount += Predefined_XML_char_size[pFound - Xml_Chars] - 1;
        }
    }

    if( charCount == 0)
    {
        if( pNewLength != NULL)
        {
            *pNewLength = length;
        }

        return AnscCloneString(pInputString);
    }

    /* allocate the new memeory with enough spaces */
    pOutString = (PCHAR)AnscAllocateMemory( length + charCount + 16);

    if( pOutString == NULL)
    {
        if( pNewLength != NULL)
        {
            *pNewLength = 0;
        }

        return NULL;
    }

    for( i = 0 ; i < length; i ++)
    {
        c = pInputString[i];

        for ( j = 0; j < Predefined_Count; j ++)
        {
            if( c == Predefined_XML_Entities[j].uCharacter)
            {
                break;
            }
        }

        if( j == Predefined_Count)
        {
            pOutString[pos++] = c;
        }
        else
        {
            AnscCopyString
                (
                    (PCHAR)(pOutString + pos),
                    (PCHAR)Predefined_XML_Entities[j].Encode
                );

            pos += AnscSizeOfString((PCHAR)Predefined_XML_Entities[j].Encode);
        }
    }

    if( pNewLength != NULL)
    {
        *pNewLength = pos;
    }

    return pOutString;
}

