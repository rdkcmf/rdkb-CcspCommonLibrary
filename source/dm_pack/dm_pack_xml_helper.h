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

/*
  DM PACK

  Api used by the code generated data model packing code to 
    create an xml dom tree at runtime.
  The goal is to reduce the package code compiled size thus
    this api allows for a minimilist code representation
    by leveraging the fact that many objects/params use
    default values that can be created inside this api
    code and not explicitely created in the packing code.
*/

#ifndef _DM_PACK_XML_HELPER_
#define _DM_PACK_XML_HELPER_

PANSC_XML_DOM_NODE_OBJECT DMPackCreateObject(PANSC_XML_DOM_NODE_OBJECT P, int type, char* name, char* maxInstance);
void DMPackCreateFunctions(PANSC_XML_DOM_NODE_OBJECT P, char* name, int numFuncs, ...);
void DMPackCreateParam(PANSC_XML_DOM_NODE_OBJECT P, char* name, int typeId);
void DMPackCreateParamTSWN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S,int W,int N);
void DMPackCreateParamTSW(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S,int W);
void DMPackCreateParamTSN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S,int N);
void DMPackCreateParamTS(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S);
void DMPackCreateParamTWN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,int W,int N);
void DMPackCreateParamTW(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,int W);
void DMPackCreateParamTN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,int N);
void DMPackCreateParamT(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T);
void DMPackCreateParamSWN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S,int W,int N);
void DMPackCreateParamSW(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S,int W);
void DMPackCreateParamSN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S,int N);
void DMPackCreateParamS(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S);
void DMPackCreateWN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,int W,int N);
void DMPackCreateW(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,int W);
void DMPackCreateN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,int N);
PANSC_XML_DOM_NODE_OBJECT DMPackCreateNode(PANSC_XML_DOM_NODE_OBJECT pNode, const char* pName, const char* pText, ULONG textSize);
PANSC_XML_DOM_NODE_OBJECT DMPackCreatePNode(PANSC_XML_DOM_NODE_OBJECT pNode, const char* pName);

#if 0
ANSC_STATUS DMPackCreateAttribute(PANSC_XML_DOM_NODE_OBJECT pNode, const char* pNewAttributeName, ULONG dataSize, const char* pNewAttributeData);
#endif

#endif
