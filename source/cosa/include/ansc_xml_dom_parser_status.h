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

    module:	ansc_xml_dom_parser_status.h

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

#ifndef     _ANSC_XML_DOM_PARSER_STATUS
#define     _ANSC_XML_DOM_PARSER_STATUS


#define  ANSC_XML_FAILURE                           0xFF005000L
#define  ANSC_STATUS_XML_ERROR_BASE                 0xFF005000L

#define  ANSC_STATUS_XML_INVALID_HANDLE             ANSC_STATUS_XML_ERROR_BASE  + 0x0001L
#define  ANSC_STATUS_XML_INTERNAL_ERROR             ANSC_STATUS_XML_ERROR_BASE  + 0x0002L
#define  ANSC_STATUS_XML_CANNOT_OPEN_FILE           ANSC_STATUS_XML_ERROR_BASE  + 0x0003L
#define  ANSC_STATUS_XML_INVALID_LENGTH             ANSC_STATUS_XML_ERROR_BASE  + 0x0004L
#define  ANSC_STATUS_XML_CANNOT_READ_FILE           ANSC_STATUS_XML_ERROR_BASE  + 0x0005L
#define  ANSC_STATUS_XML_INVALID_ATTRIBUTE_VALUE    ANSC_STATUS_XML_ERROR_BASE  + 0x0006L
#define  ANSC_STATUS_XML_INVALID_ATTRIBUTE_NAME     ANSC_STATUS_XML_ERROR_BASE  + 0x0007L  
#define  ANSC_STATUS_XML_INVALID_NODE_NAME          ANSC_STATUS_XML_ERROR_BASE  + 0x0008L
#define  ANSC_STATUS_XML_ATTRIBUTE_NOT_EXIST        ANSC_STATUS_XML_ERROR_BASE  + 0x0009L
#define  ANSC_STATUS_XML_INVALID_VERSION            ANSC_STATUS_XML_ERROR_BASE  + 0x000AL
#define  ANSC_STATUS_XML_INVALID_COMMENT_END        ANSC_STATUS_XML_ERROR_BASE  + 0x000BL  
#define  ANSC_STATUS_XML_MISSED_END_TAG             ANSC_STATUS_XML_ERROR_BASE  + 0x000CL
#define  ANSC_STATUS_XML_RESOURCES                  ANSC_STATUS_XML_ERROR_BASE  + 0x000DL
#define  ANSC_STATUS_XML_INVALID_TEXT_VALUE         ANSC_STATUS_XML_ERROR_BASE  + 0x000EL  
#define  ANSC_STATUS_XML_EMPTY_TEXT                 ANSC_STATUS_XML_ERROR_BASE  + 0x000FL  
#define  ANSC_STATUS_XML_INVALID_STRING             ANSC_STATUS_XML_ERROR_BASE  + 0x0010L  
#define  ANSC_STATUS_XML_EQUAL_SIGN_MISSED          ANSC_STATUS_XML_ERROR_BASE  + 0x0011L  
#define  ANSC_STATUS_XML_STRING_SIGN_MISSED         ANSC_STATUS_XML_ERROR_BASE  + 0x0012L
#define  ANSC_STATUS_XML_SPACE_NEEDED               ANSC_STATUS_XML_ERROR_BASE  + 0x0013L
#define  ANSC_STATUS_XML_BUFFER_NOT_ENOUGH          ANSC_STATUS_XML_ERROR_BASE  + 0x0014L
#define  ANSC_STATUS_XML_NODE_MISSED                ANSC_STATUS_XML_ERROR_BASE  + 0x0015L

#endif
