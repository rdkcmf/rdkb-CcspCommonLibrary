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

#include <assert.h>
#include "ansc_platform.h"
#include "ansc_xml_dom_parser_interface.h"
#include "ansc_xml_dom_parser_external_api.h"
#include "ansc_xml_dom_parser_status.h"
#include "dm_pack_xml_helper.h"

#define W_DEFAULT -1
#define N_DEFAULT -1

typedef enum funcNameId {
  func_GetEntryCount,
  func_GetEntryStatus,
  func_GetEntry,
  func_AddEntry,
  func_DelEntry,
  func_IsUpdated,
  func_Synchronize,
  func_Lock,
  func_Unlock,
  func_CheckInstance,
  func_GetParamBoolValue,
  func_GetParamIntValue,
  func_GetParamUlongValue,
  func_GetParamStringValue,
  func_GetBulkParamValues,
  func_SetParamBoolValue,
  func_SetParamIntValue,
  func_SetParamUlongValue,
  func_SetParamStringValue,
  func_SetBulkParamValues,
  func_Validate,
  func_Commit,
  func_Rollback,
  func_Init,
  func_Async_Init,
  func_Unload,
  func_MemoryCheck,
  func_MemoryUsage,
  func_MemoryTable,
  func_IsObjSupported
}funcNameId;

static const char* funcName[] = {
  "GetEntryCount",
  "GetEntryStatus",
  "GetEntry",
  "AddEntry",
  "DelEntry",
  "IsUpdated",
  "Synchronize",
  "Lock",
  "Unlock",
  "CheckInstance",
  "GetParamBoolValue",
  "GetParamIntValue",
  "GetParamUlongValue",
  "GetParamStringValue",
  "GetBulkParamValues",
  "SetParamBoolValue",
  "SetParamIntValue",
  "SetParamUlongValue",
  "SetParamStringValue",
  "SetBulkParamValues",
  "Validate",
  "Commit",
  "Rollback",
  "Init",
  "Async_Init",
  "Unload",
  "MemoryCheck",
  "MemoryUsage",
  "MemoryTable",
  "IsObjSupported"
};
static const unsigned char funcNameLen[] = {
  13,
  14,
  8,
  8,
  8,
  9,
  11,
  4,
  6,
  13,
  17,
  16,
  18,
  19,
  18,
  17,
  16,
  18,
  19,
  18,
  8,
  6,
  8,
  4,
  10,
  6,
  11,
  11,
  11,
  14
};

typedef enum paramTypeId
{
  param_bool,
  param_int,
  param_ulong,
  param_string
}paramTypeId;

typedef enum objectTypeId
{
  ObjObject,
  ObjStaticTable,
  ObjDynamicTable,
  ObjWritableTable,
  ObjDynWritableTable
}objectTypeId;

static const char* paramName[] = {
  "boolean",
  "int",
  "unsignedInt",
  "string"
};
static const unsigned char paramNameLen[] = {
  7,
  3,
  11,
  6
};
static const char* paramSyntax[] = {
  "bool",
  "int",
  "uint32",
  "string"
};
static const unsigned char paramSyntaxLen[] = {
  4,
  3,
  6,
  6
};

static const char* objectType[] = {
  "object",
  "staticTable",
  "dynamicTable",
  "writableTable",
  "dynWritableTable"
};
static const unsigned char objectTypeLen[] = {
  6,
  11,
  12,
  13,
  16
};

PANSC_XML_DOM_NODE_OBJECT pLastObject=0;
PANSC_XML_DOM_NODE_OBJECT pCurrentFunctionsNode=0;

PANSC_XML_DOM_NODE_OBJECT DMPackCreateObject(PANSC_XML_DOM_NODE_OBJECT P, int type, char* name, char* maxInstance)
{
  PANSC_XML_DOM_NODE_OBJECT P1 = NULL;

  P1 = DMPackCreateNode(P,"object",0,0);
  DMPackCreateNode(P1,"name",name,0);
  DMPackCreateNode(P1,"objectType",objectType[type],objectTypeLen[type]);
  if(maxInstance)
    DMPackCreateNode(P1,"maxInstance",maxInstance,0);
  pCurrentFunctionsNode=0;
  return P1;
}

void DMPackCreateFunctions(PANSC_XML_DOM_NODE_OBJECT P, char* name, int numFuncs, ...)
{
  va_list funcIds;
  int funcId;
  int i;
  char buff1[28];
  char buff2[128];

  if(P != pLastObject)
  {
    pLastObject = P;
    pCurrentFunctionsNode = DMPackCreateNode(P,"functions",0,0);
  }

  va_start(funcIds, numFuncs );  
  
  for(i=0; i<numFuncs; ++i)
  {
    funcId = va_arg(funcIds, int); 
#if 1
    /* Optimised version - should be equivalent to the original version below */
    const char *f = funcName[funcId];
    size_t flen = funcNameLen[funcId];
    memcpy(buff1, "func_", 5);
    memcpy(buff1 + 5, f, flen + 1);
    size_t namelen = strlen(name);
    assert((namelen + 1 + flen) < sizeof(buff2));
    memcpy(buff2, name, namelen);
    buff2[namelen] = '_';
    memcpy(buff2 + namelen + 1, f, flen + 1);
    DMPackCreateNode(pCurrentFunctionsNode,buff1,buff2,namelen + 1 + flen);
#else
    snprintf(buff1, sizeof(buff1), "func_%s", funcName[funcId]);
    snprintf(buff2, sizeof(buff2), "%s_%s", name, funcName[funcId]);
    DMPackCreateNode(pCurrentFunctionsNode,buff1,buff2,0);
#endif
  }

  va_end(funcIds);
}

void DMPackCreateParam(PANSC_XML_DOM_NODE_OBJECT P, char* name, int typeId)
{
  PANSC_XML_DOM_NODE_OBJECT P1 = DMPackCreateNode(P,"parameter",0,0);
  DMPackCreateNode(P1,"name",name,0);
  DMPackCreateNode(P1,"type",paramName[typeId],paramNameLen[typeId]);
  DMPackCreateNode(P1,"syntax",paramSyntax[typeId],paramSyntaxLen[typeId]);
}

void DMPackCreateParamEx(PANSC_XML_DOM_NODE_OBJECT P, char* name, int typeId, char* type, char* syntax, int writable, int notify)
{
  PANSC_XML_DOM_NODE_OBJECT P1 = NULL;

  P1 = DMPackCreateNode(P,"parameter",0,0);

  DMPackCreateNode(P1,"name",name,0);

  if(type)
    DMPackCreateNode(P1,"type",type,0);
  else
    DMPackCreateNode(P1,"type",paramName[typeId],paramNameLen[typeId]);

  if(syntax)
    DMPackCreateNode(P1,"syntax",syntax,0);
  else
    DMPackCreateNode(P1,"syntax",paramSyntax[typeId],paramSyntaxLen[typeId]);

  if(writable == 0)
    DMPackCreateNode(P1,"writable","false",5);
  else if(writable == 1)
    DMPackCreateNode(P1,"writable","true",4);

  if(notify == 0)
    DMPackCreateNode(P1,"notify","off",3);
  else if(notify == 1)
    DMPackCreateNode(P1,"notify","on",2); /* CID 280129 fix */
}
void DMPackCreateParamTSWN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S,int W,int N)
{
  DMPackCreateParamEx(P,name,0,T,S,W,N);
}

void DMPackCreateParamTSW(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S,int W)
{
  DMPackCreateParamEx(P,name,0,T,S,W,N_DEFAULT);
}

void DMPackCreateParamTSN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S,int N)
{
  DMPackCreateParamEx(P,name,0,T,S,W_DEFAULT,N);
}

void DMPackCreateParamTS(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,char* S)
{
  DMPackCreateParamEx(P,name,0,T,S,W_DEFAULT,N_DEFAULT);
}

void DMPackCreateParamTWN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,int W,int N)
{
  DMPackCreateParamEx(P,name,0,T,0,W,N);
}

void DMPackCreateParamTW(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,int W)
{
  DMPackCreateParamEx(P,name,0,T,0,W,N_DEFAULT);
}

void DMPackCreateParamTN(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T,int N)
{
  DMPackCreateParamEx(P,name,0,T,0,W_DEFAULT,N);
}

void DMPackCreateParamT(PANSC_XML_DOM_NODE_OBJECT P,char* name,char* T)
{
  DMPackCreateParamEx(P,name,0,T,0,W_DEFAULT,N_DEFAULT);
}

void DMPackCreateParamSWN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S,int W,int N)
{
  DMPackCreateParamEx(P,name,typeId,0,S,W,N);
}

void DMPackCreateParamSW(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S,int W)
{
  DMPackCreateParamEx(P,name,typeId,0,S,W,N_DEFAULT);
}

void DMPackCreateParamSN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S,int N)
{
  DMPackCreateParamEx(P,name,typeId,0,S,W_DEFAULT,N);
}

void DMPackCreateParamS(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,char* S)
{
  DMPackCreateParamEx(P,name,typeId,0,S,W_DEFAULT,N_DEFAULT);
}

void DMPackCreateWN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,int W,int N)
{
  DMPackCreateParamEx(P,name,typeId,0,0,W,N);
}

void DMPackCreateW(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,int W)
{
  DMPackCreateParamEx(P,name,typeId,0,0,W,N_DEFAULT);
}

void DMPackCreateN(PANSC_XML_DOM_NODE_OBJECT P,char* name, int typeId,int N)
{
  DMPackCreateParamEx(P,name,typeId,0,0,W_DEFAULT,N);
}

PANSC_XML_DOM_NODE_OBJECT DMPackCreateNode(PANSC_XML_DOM_NODE_OBJECT pNode, const char* pName, const char* pText, ULONG textSize)
{
  PANSC_XML_DOM_NODE_OBJECT pNewNode = NULL;
  ANSC_HANDLE hOwnerContext = NULL;
  size_t len;

  if(pNode)
    hOwnerContext = pNode->hOwnerContext;

  pNewNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate(hOwnerContext);

  if ( !pNewNode )
  {
    AnscTrace("Failed to allocate child Node for - %s\n", pNode ? pNode->Name : "root node");
    return NULL;
  }

  if(pNode)
  {
    pNewNode->hOwnerContext = pNode->hOwnerContext;
    pNewNode->hParentNode   = (ANSC_HANDLE)pNode;
    pNewNode->Level         = pNode->Level + 1;
    pNewNode->hXMLContext   = pNode->hXMLContext;
  }

  len = strlen(pName);

#if 0
  /* Sanity check - enable for debug builds only */
  if (len >= sizeof(pNewNode->Name))
  {
    AnscTrace("DMPackCreateNode invalid name - %s\n", pName);
    AnscFreeMemory(pNewNode);
    return NULL;
  }
#endif

  memcpy(pNewNode->Name, pName, len + 1);

  if(pText)
  {
    /* Callers may either pass in the string length of pText or pass in 0 and it will be determined here */
    if (textSize == 0)
    {
      textSize = strlen(pText);
    }
    else
    {
#if 0
      /* Sanity check - enable for debug builds only */
      if (textSize != strlen(pText))
      {
        printf("DMPackCreateNode bad length pName=%s pText=%s textSize=%lu\n", pName, pText, textSize);
        textSize = strlen(pText);
      }
#endif
    }

    pNewNode->DataSize     = textSize;
    pNewNode->StringData   = AnscAllocateMemoryNoInit(textSize + 1);

    if ( !pNewNode->StringData )
    {
        AnscTrace
            (
                "Failed to allocate %lu memory for text of - '%s'\n",
                textSize,
                pNewNode->Name
            );
        printf("DMPackCreateNode ANSC_STATUS_XML_RESOURCES pName=%s textSize=%lu\n", pName, textSize);
        fflush(stdout);
        /*CID: 119749 Resource leak*/
	AnscFreeMemory(pNewNode->StringData);
	AnscFreeMemory(pNewNode);
        return NULL;
    }

    memcpy(pNewNode->StringData, pText, textSize + 1);
  }

  if(pNode)
  {
    AnscQueuePushEntry(&pNode->ChildNodeQueue, &pNewNode->Linkage);
  }

  return pNewNode;
}

PANSC_XML_DOM_NODE_OBJECT DMPackCreatePNode(PANSC_XML_DOM_NODE_OBJECT pNode, const char* pName)
{
  PANSC_XML_DOM_NODE_OBJECT pNewNode = NULL;
  ANSC_HANDLE hOwnerContext = NULL;

  if(pNode)
    hOwnerContext = pNode->hOwnerContext;

  pNewNode = (PANSC_XML_DOM_NODE_OBJECT)AnscXmlDomNodeCreate(hOwnerContext);

  if ( !pNewNode )
  {
    if(pNode)
    {
      AnscTrace("Failed to allocate child Node for  - %s\n", pNode->Name);
    }
    else
    {
      AnscTrace("Failed to allocate child Node for  - root node\n");
    }
    return NULL;
  }

  if(pNode)
  {
    pNewNode->hOwnerContext = pNode->hOwnerContext;
    pNewNode->hParentNode   = (ANSC_HANDLE)pNode;
    pNewNode->Level         = pNode->Level + 1;
    pNewNode->hXMLContext   = pNode->hXMLContext;
  }

  AnscCopyString( pNewNode->Name, (char*)pName);

  if(pNode)
  {
    AnscQueuePushEntry(&pNode->ChildNodeQueue, &pNewNode->Linkage);
  }

  return pNewNode;
}

#if 0
ANSC_STATUS DMPackCreateAttribute(PANSC_XML_DOM_NODE_OBJECT pNode, const char* pNewAttributeName, ULONG dataSize, const char* pNewAttributeData)
{
  PANSC_XML_ATTRIBUTE             pNewAttribute = NULL;

  pNewAttribute = (PANSC_XML_ATTRIBUTE)AnscAllocateMemory(sizeof(ANSC_XML_ATTRIBUTE));

  if ( !pNewAttribute )
  {
      AnscFreeMemory((PVOID)pNewAttributeName);
      AnscTrace("Failed to allocate attribute structure for - '%s'\n", pNode->Name);

      return ANSC_STATUS_XML_RESOURCES;
  }

  pNewAttribute->hParentNode   = (ANSC_HANDLE)pNode;
  pNewAttribute->hXMLContext   = pNode->hXMLContext;

  AnscCopyString(pNewAttribute->Name, (char *)pNewAttributeName);

  pNewAttribute->DataSize      = dataSize;
  pNewAttribute->StringData    = AnscAllocateMemory( pNewAttribute->DataSize + 1);

  if( pNewAttribute->StringData == NULL)
  {
      AnscFreeMemory( pNewAttribute );
      AnscTrace("Failed to allocate attribute value for - '%s'\n", pNode->Name);
      return ANSC_STATUS_XML_RESOURCES;
  }
  AnscCopyMemory(pNewAttribute->StringData, (PVOID)pNewAttributeData, pNewAttribute->DataSize);

  AnscQueuePushEntry(&pNode->AttributesList, &pNewAttribute->Linkage);

  return ANSC_STATUS_SUCCESS;
}
#endif

