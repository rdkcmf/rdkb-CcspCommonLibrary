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

    module:	ansc_xml_parser_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros related to xml parsing operation.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        04/07/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_XML_PARSER_INTERFACE_
#define  _ANSC_XML_PARSER_INTERFACE_


/***********************************************************
     PLATFORM INDEPENDENT XML PARSER OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the xml parser object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_XML_PARSER_CLASS_CONTENT                                                      \
    /* duplication of the base object class content */                                      \
    ANSC_POLICY_PARSER_CLASS_CONTENT                                                        \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_XML_PARSER_OBJECT
{
    ANSC_XML_PARSER_CLASS_CONTENT
}
ANSC_XML_PARSER_OBJECT,  *PANSC_XML_PARSER_OBJECT;

#define  ACCESS_ANSC_XML_PARSER_OBJECT(p)           \
         ACCESS_CONTAINER(p, ANSC_XML_PARSER_OBJECT, Linkage)


/***********************************************************
       PLATFORM INDEPENDENT XML NODE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the xml node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

/*
 * Most policy configuration information is stored and transmitted in xml format. Compared to our
 * proprietary al_store based configuration format, xml not only provides more flexible and
 * powerful hierarchical structure, it can also be translated into other data format, such as html,
 * very easily via XSLT (eXtensible Stylesheet Language Transformation). This Xml Parser Object
 * takes a xml-based data stream as input, and exposes a read/write interface similar to al_store.
 */
#define  ANSC_XML_NODE_CLASS_CONTENT                                                        \
    /* duplication of the base object class content */                                      \
    ANSC_POLICY_NODE_CLASS_CONTENT                                                          \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_XML_NODE_OBJECT
{
    ANSC_XML_NODE_CLASS_CONTENT
}
ANSC_XML_NODE_OBJECT,  *PANSC_XML_NODE_OBJECT;

#define  ACCESS_ANSC_XML_NODE_OBJECT(p)             \
         ACCESS_CONTAINER(p, ANSC_XML_NODE_OBJECT, Linkage)


#endif
