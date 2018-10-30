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


#ifndef  _CF_BMCO_CO_INTERFACE_DEC
#define  _CF_BMCO_CO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"

#include "cf_xml_node_name.h"

#define  CF_BMCO_COMPONENT_NAME                     "CfBasicMsgContentComponent"
#define  CF_BMCO_COMPONENT_OID                      ANSC_OBJECT_OID_NULL
#define  CF_BMCO_COMPONENT_TYPE                     ANSC_OBJECT_TYPE_NULL

/* Definitions of default value of node name */

#define  CF_CLIENT_VERSION                          "1.2"
#define  CF_SERVER_VERSION                          "1.0"

/* message types */
typedef  
enum
_CF_MESSAGE_TYPE
{
  CF_UNKNOWN_MESSAGE = 0,
  CF_AR_MESSAGE,
  CF_AP_MESSAGE,
  CF_FR_MESSAGE,
  CF_FP_MESSAGE,
  CF_RR_MESSAGE,
  CF_RP_MESSAGE
}
CF_MESSAGE_TYPE;


/*
 *  The class CfBasicMessage is the based class, it's
 *  derived from ANSC component object.
 *
 *  the derived classes include CfArMessage, CfApMessage, 
 *  CfFrMessage,CfFpMessage, CfRrMessage and CfRpMessage.
 *
 *  Every type of content will include the message type,
 *  version number and user name.
 *
 */
typedef
CF_MESSAGE_TYPE
(*PFN_CF_BMCO_GET_TYPE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_BMCO_GET_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ANSC_HANDLE
(*PFN_CF_BMCO_GET_USER_NAME)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
void
(*PFN_CF_BMCO_SET_USER_NAME)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUserName
    );

typedef
void
(*PFN_CF_BMCO_SET_VERSION)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       Version
    );

typedef
ANSC_HANDLE
(*PFN_CF_BMCO_CREATE_XML_HANDLE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_BMCO_PARSE_XML_NODE)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hXmlNode
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT                                         \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    CF_MESSAGE_TYPE                 Type;                                               \
    ULONG                           Version;                                            \
    /* pointer to an ANSC_STRING_OBJECT */                                              \
    ANSC_HANDLE                     User;                                               \
    /* methods */                                                                       \
    PFN_CF_BMCO_GET_TYPE            GetType;                                            \
    PFN_CF_BMCO_GET_VERSION         GetVersion;                                         \
    PFN_CF_BMCO_GET_USER_NAME       GetUserName;                                        \
    PFN_CF_BMCO_SET_USER_NAME       SetUserName;                                        \
    PFN_CF_BMCO_SET_VERSION         SetVersion;                                         \
    /* overload functions */                                                            \
    PFN_CF_BMCO_CREATE_XML_HANDLE   CreateXmlHandle;                                    \
    PFN_CF_BMCO_PARSE_XML_NODE      ParseXmlNode;                                       \



/* content filtering basic message object  */
typedef  struct
_CF_BASIC_MSG_CONTENT_OBJECT
{
    CF_BASIC_MESSAGE_CONTENT_CLASS_CONTENT
}
CF_BASIC_MSG_CONTENT_OBJECT,    *PCF_BASIC_MSG_CONTENT_OBJECT;


#define  ACCESS_CF_BASIC_MSG_CONTENT_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_BASIC_MSG_CONTENT_OBJECT, Linkage)

#endif

