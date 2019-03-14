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

    module:	ansc_xml_dom_parser_def.h

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


#ifndef  _ANSC_XML_DOM_PARSER_DEF
#define  _ANSC_XML_DOM_PARSER_DEF

/* Defined for XML file */
#define  XML_HEADER                                 "<?xml version=\"1.0\"  encoding=\"UTF-8\" ?>"
#define  XML_HEADER_BEGIN                           "<?"
#define  XML_HEADER_END                             "?>"
#define  XML_VERSION_1                              "xml version=\"1.0\""
#define  XML_VERSION_2                              "xml version='1.0'"
#define  XML_COMMENT_BEGIN                          "<!--"
#define  XML_COMMENT_END                            "-->"
#define  XML_NODE_END                               "/>"
#define  XML_END_NODE                               "</"
#define  XML_SIMPLE_END                             "/>"
#define  XML_STRING                                 "xml"
#define  XML_NAMESPACE                              "http://www.w3.org/XML/1998/namespace"

#define  XML_TAG_BEGIN                              "<"
#define  XML_TAG_END                                ">"

#define  STR_FALSE                                  "false"
#define  STR_TRUE                                   "true"
#define  STR_YES                                    "yes"
#define  STR_NO                                     "no"


/*
 * the following characters are used to seperate tokens that represent certain meaning in the configuration file
 */
#define  SPECIAL_TOKEN	                            " \t\r\n"
#define  WANTED_TOKEN	                            " ="

#define  ITEM_EQUAL_MARK                            "="
#define  STRING_MARK_1                              "\""
#define  STRING_MARK_2                              "'"
#define  SPACE_SEP                                  " "

#define  CHAR_EQUAL		                            '='
#define  CHAR_TAG_BEGIN	                            '<'
#define  CHAR_TAG_END	                            '>'

/* 
 * Defined by the old AL_STORE Library.
 * Not used in XML parse, but keep it here in case.
 *
 */

/*
static  const  char  NODE_START_MARK[] = {'[',  0};
static  const  char  NODE_END_MARK[]   = {']',  0};
static  const  char  ITEM_END_MARK[]   = {';',  0};
static  const  char  TYPE_MARK[]       = {':',  0};
static  const  char  CATENATION_MARK[] = {'\\', 0};
*/

#define  BOOLEAN_TYPE                               "BOOLEAN"
#define  BINARY_TYPE                                "BINARY"
#define  ULONG_TYPE                                 "ULONG"
#define  STRING_TYPE                                "STRING"
#define  SECURED_ITEM                               "1"
#define  PLAIN_ITEM                                 "0"

#define ANSC_XML_TRACE                              AnscTrace

#define SPACES_PER_LEVEL                            2

#endif

