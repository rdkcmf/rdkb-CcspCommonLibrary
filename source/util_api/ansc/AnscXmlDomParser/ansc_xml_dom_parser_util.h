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

    module:	ansc_xml_dom_parser_util.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

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

#ifndef  _ANSC_XML_DOM_PARSER_UTIL
#define  _ANSC_XML_DOM_PARSER_UTIL

ULONG
AnscXmlStringToLong
    (
        char*  pString
    );


VOID
AnscXmlLongToString
    (
        LONG value, 
        char* pBuffer
    );

BOOLEAN
AnscXmlIsUpperLetter
    (
        char c
    );

BOOLEAN
AnscXmlIsLowerLetter
    (
        char c
    );

BOOLEAN
AnscXmlIsLetter
    (
        char c
    );

BOOLEAN
AnscXmlIsDigit
    (
        char c
    );

ULONG
AnscXmlCombineTokens
    (
        char*  pOutputString, 
        char*  pInputString, 
        ULONG  ulInputSize, 
        char*  charsToSkip
    );

BOOLEAN
AnscXmlIsValidBinaryChar
    (
        char c
    );

ANSC_STATUS
AnscXmlFromASCIIToBinary
    (
        PVOID                       pCharData,
        PULONG                      pulSize
    );

BOOLEAN
AnscXmlCharInString
    (
        char*  pString, 
        char  charToFind
    );

BOOLEAN
AnscXmlIsValidAttributeName
    (
        PCHAR                       pAttributeName
    );

BOOLEAN
AnscXmlIsValidAttributeValue
    (
        PCHAR                       pAttributeValue
    );

ANSC_STATUS
AnscXmlFromBinaryToASCII
    (
        PVOID                       pBinaryData,
        PULONG                      pulSize
    );

BOOLEAN
AnscXmlIsValidNodeText
    (
        PCHAR                       pTextValue
    );

ANSC_STATUS
AnscXmlFindNode
    (
        PCHAR*                      ppContent,
        LONG                        length,
        ULONG                       ulLayerLevel
    );

ANSC_STATUS
AnscXmlFillContentOfNode
    (
        PCHAR*                      ppContent,
        LONG                        length,
        PANSC_XML_DOM_NODE_OBJECT   pNode
    );

char* 
AnscXmlGotoFirstCharExcept
    (
        char* pInputString, 
        int length, 
        char* pExceptString
    );

char* 
AnscXmlGetStringEndExcept
    (
        char* pInputString, 
        ULONG endPosition, 
        char* pExceptString
    );

BOOLEAN
AnscXmlIsStringStartWith
    (
        char* pInputString, 
        int length, 
        char* pStartWithChars
    );

char* 
AnscXmlGotoFirstWantedChar
    (
        char* pInputString, 
        int length, 
        char* pWantedChars
    );

char*
AnscXmlFindString
    (
        char* pString, 
        char* pSearchString
    );

char*
AnscXmlFindCharacterInRange
    (
        char*  pString, 
        char  charToFind, 
        ULONG  ulRange
    );

ULONG
AnscXmlSizeOfToken
    (
        char*  pString, 
        char*  charsToStop, 
        ULONG  ulRange
    );

char*
AnscXmlMoveToNextToken
    (
        char*  pString, 
        char*  charsToSkip
    );

char*
AnscXmlMoveToPreviousToken
    (
        char*  pString, 
        char*  charsToSkip, 
        PULONG  pulTokenLength
    );

ULONG
AnscXmlNumberOfCharsInString
    (
        char*  pString, 
        char  charToFind
    );

BOOLEAN
AnscXmlIsValidNodeName
    (
        PCHAR                       pNodeName
    );

ULONG
AnscXmlNode2Buf
    (
        PVOID                       pBuffer,
        PULONG                      pulBuffer,
        PVOID                       pContent,
        ULONG                       ulContent
    );

ULONG
AnscXmlNodeEncodeSpaces
    (
        PVOID                       pBuffer,
        PULONG                      pulBufferSize,
        ULONG                       ulTimes
    );

ULONG
AnscXmlNodeEncodeLinefeed
    (
        PVOID                       pBuffer,
        PULONG                      pulBufferSize
    );

ULONG
AnscXmlEncodeAttr2Buf
    (
        PVOID                       pBuffer,
        PULONG                      pulBuffSize,
        PANSC_XML_ATTRIBUTE         pAttribute
    );

ULONG
AnscXmlGetAttr2BufSize
    (
        PANSC_XML_ATTRIBUTE         pAttribute
    );

BOOL
AnscXmlRemoveCharReference
    (
        PCHAR                       pString,
        PULONG                      pStringLen
    );

PCHAR
AnscXmlEscapeChars
    (
        PCHAR                       pInputString,
        ULONG                       length,
        PULONG                      pNewLength
    );

#endif
