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

    module: ccsp_cr_profile.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file has all the XML node names defined for CR device profile.
        The XML schema and example XML file are included as well.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu 

    ---------------------------------------------------------------

    revision:

        05/24/2011    initial revision.

**********************************************************************/


#ifndef  _CCSP_CR_PROFILE_H_
#define  _COSA_CR_PROFILE_H_

/**************************************************************************************************

CCSP Component Registrar (CR) Device Profile Definition:

CR Profile is an XML file includes all the CCSP Components required in the system.

CR will only be ready until all the components in the profile are registered successfully.

***************************************************************************************************/
/*
<?xml version="1.0" encoding="UTF-8"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">

  <xs:complexType name="componentInfo">
    <xs:sequence>
      <xs:element name="name" type="xs:string"/>
      <xs:element name="version" type="xs:positiveInteger"/>
    </xs:sequence>
  </xs:complexType>  

  <xs:complexType name="componentArray">
    <xs:sequence>
      <xs:element name="component" type="componentInfo" minOccurs="0" maxOccurs="unbounded" />
    </xs:sequence>
  </xs:complexType>  

  <xs:complexType name="crInfo">
    <xs:sequence>
      <xs:element name="prefix" type="xs:string"/>
      <xs:element name="id" type="xs:string"/>
    </xs:sequence>
  </xs:complexType>  

  <xs:complexType name="crArray">
    <xs:sequence>
      <xs:element name="cr" type="crInfo" minOccurs="0" maxOccurs="unbounded" />
    </xs:sequence>
  </xs:complexType>  

  <xs:element name="deviceProfile">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="version" type="xs:positiveInteger"/>
        <xs:element name="crName" type="xs:string"/>
        <xs:element name="deviceName" type="xs:string"/>
        <xs:element name="remote" type="crArray"/>
        <xs:element name="components" type="componentArray" />
      </xs:sequence>
    </xs:complexType>
  </xs:element>

</xs:schema>

*/
/* CR Profile Example 
<?xml version="1.0" encoding="utf-8" ?>
<deviceProfile>
    <!-- Different CR must have different names -->
    <name>com.cisco.spvtg.ccsp.CR</name>
    <prefix>eMG.</prefix>
    <version>1</version>
    <deviceName>VideoScape EP1.0</deviceName>
    <components>
        <component>
               <name>CCSP_DeviceInfo</name> <version>1</version>
        </component>
        <component>
               <name>CCSP_TimeManager</name> <version>1</version>
        </component>
        <component>
               <name>CCSP_SecureDownload</name> <version>2</version>
        </component>
    </components>   
    <remote>
       <cr><prefix>eMG.</prefix><id>com.cisco.spvtg.ccsp.cr1.</id></cr>
       <cr><prefix>eTG.</prefix><id>com.cisco.spvtg.ccsp.cr2.</id></cr>
    </remote>

</deviceProfile>
*/


/* deviceProfile XML Node Name */
#define  CCSP_CR_XML_NODE_rootName                   "deviceProfile"
#define  CCSP_CR_XML_NODE_deviceVersion              "deviceVersion"
#define  CCSP_CR_XML_NODE_prefix                     "prefix"
#define  CCSP_CR_XML_NODE_crName                     "name"
#define  CCSP_CR_XML_NODE_deviceName                 "deviceName"
#define  CCSP_CR_XML_NODE_remote                     "remote"
#define  CCSP_CR_XML_NODE_cr                         "cr"
#define  CCSP_CR_XML_NODE_cr_prefix                  "prefix"
#define  CCSP_CR_XML_NODE_cr_id                      "id"
#define  CCSP_CR_XML_NODE_components                 "components"
#define  CCSP_CR_XML_NODE_component                  "component"
#define  CCSP_CR_XML_NODE_component_name             "name"
#define  CCSP_CR_XML_NODE_component_version          "version"

#endif
