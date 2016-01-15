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


#ifndef  _CF_BMO_CO_INTERFACE_DEC
#define  _CF_BMO_CO_INTERFACE_DEC

#include "ansc_co_interface.h"
#include "ansc_co_external_api.h"


#define  CF_BMO_COMPONENT_NAME                      "CfBasicMsgComponent"
#define  CF_BMO_COMPONENT_OID                       ANSC_OBJECT_OID_NULL
#define  CF_BMO_COMPONENT_TYPE                      ANSC_OBJECT_TYPE_NULL

#define  ACCEPTABLE_AL_MESSAGE_VERSION              1


typedef
ANSC_HANDLE
(*PFN_CF_BMO_GET_MSG_CONTENT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
ULONG
(*PFN_CF_BMO_GET_VERSION)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef
BOOL
(*PFN_CF_BMO_XML_ENCODE)
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       pBufferLen
    );

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_BASIC_MESSAGE_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \
    /* a pointer to "CF_BASIC_MSG_CONTENT_OBJECT" */                                    \
    ANSC_HANDLE                     hMsgContent;                                        \
    ULONG                           Version;                                            \
    /* methods */                                                                       \
    PFN_CF_BMO_GET_MSG_CONTENT      GetMsgContent;                                      \
    PFN_CF_BMO_GET_VERSION          GetVersion;                                         \
    PFN_CF_BMO_XML_ENCODE           XmlEncode;                                          \


/* content filtering basic message object  */
typedef  struct
_CF_BASIC_MESSAGE_OBJECT
{
    CF_BASIC_MESSAGE_CLASS_CONTENT
}
CF_BASIC_MESSAGE_OBJECT,    *PCF_BASIC_MESSAGE_OBJECT;


#define  ACCESS_CF_BASIC_MESSAGE_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_BASIC_MESSAGE_OBJECT, Linkage)



/* =====================================================================================
 *                  Object "CfMsgComponent"
 * ================================================================================== */

/*
 *  CfMsgComponent is the wrapper class of all above defined messages.
 *  It has its own version and the message content;
 */

/*
 * As a standard practice, we encapsulate all functional and feature objects inside an Object
 * Container Object so it will function as a black-box implementation when we have to integrate
 * an Ansc component with other systems. However, that's not the only reason why we need to
 * define the Container Object: we also use container as the building block for constructing
 * larger systems.
 */
#define  CF_MESSAGE_COMPONENT_CLASS_CONTENT                                                 \
    /* duplication of the base object class content */                                  \
    ANSCCO_CLASS_CONTENT                                                                \
    /* start of object class content */                                                 \


/* content filtering basic message object  */
typedef  struct
_CF_MESSAGE_COMPONENT_OBJECT
{
    CF_MESSAGE_COMPONENT_CLASS_CONTENT
}
CF_MESSAGE_COMPONENT_OBJECT,    *PCF_MESSAGE_COMPONENT_OBJECT;


#define  ACCESS_CF_MESSAGE_COMPONENT_OBJECT(p)            \
         ACCESS_CONTAINER(p, CF_MESSAGE_COMPONENT_OBJECT, Linkage)

#endif

