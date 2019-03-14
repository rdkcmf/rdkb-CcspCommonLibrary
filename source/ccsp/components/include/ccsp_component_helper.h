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

    module: ccsp_component_helper.h

        For Common Component Software Platform (CCSP) Development

    ---------------------------------------------------------------

    description:

        This header file defines the helper APIs for CCSP components:
        component configuration loading...

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        02/19/2013  initial revision.

**********************************************************************/

#ifndef  _CCSP_COMPONENT_HELPER_H_
#define  _CCSP_COMPONENT_HELPER_H_


/**********************************************************************
                    CONSTANT DEFINITIONS
**********************************************************************/

#define  CCSP_COMP_XML_NODE_Component               "Component"
#define  CCSP_COMP_XML_NODE_Id                      "ID"
#define  CCSP_COMP_XML_NODE_Name                    "Name"
#define  CCSP_COMP_XML_NODE_Version                 "Version"
#define  CCSP_COMP_XML_NODE_DbusPath                "DbusPath"
#define  CCSP_COMP_XML_NODE_DataModelXmlCfg         "DataModelXmlCfg"
#define  CCSP_COMP_XML_NODE_DataModels              "DataModels"
#define  CCSP_COMP_XML_NODE_DmXml                   "DmXml"

/**********************************************************************
                    STRUCTURE DEFINITIONS
**********************************************************************/

typedef  struct
_CCSP_COMPONENT_CFG
{
    char                            ComponentId[48];
    char                            ComponentName[48];
    ULONG                           Version;
    char                            DbusPath[48];
    char                            DmXmlCfgFileName[48];
}
CCSP_COMPONENT_CFG, *PCCSP_COMPONENT_CFG;

typedef  struct
_CCSP_DM_XML_CFG_LIST
{
    ULONG                           ulFileCount;
    char                            FileList[ANSC_ZERO_ARRAY_SIZE][64];
}
CCSP_DM_XML_CFG_LIST, *PCCSP_DM_XML_CFG_LIST;

/**********************************************************************
                    EXPORTED APIs
**********************************************************************/

ANSC_STATUS
CcspComponentLoadCfg
    (
        char*                       pCfgFileName,
        PCCSP_COMPONENT_CFG         pCfg
    );

ANSC_STATUS
CcspComponentLoadDmXmlList
    (
        char*                       pDmXmlCfgFileName,
        PCCSP_DM_XML_CFG_LIST*      ppXmlCfgList
    );

#endif  /* _CCSP_COMPONENT_HELPER_H_ */

