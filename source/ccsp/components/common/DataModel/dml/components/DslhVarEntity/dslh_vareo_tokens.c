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

    module:	dslh_vareo_tokens.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced tokens-access functions
        of the Dslh Var Entity Object.

        *   DslhVareoCfgTokenTable
        *   DslhVareoGetTokenValueByString
        *   DslhVareoGetTokenValueByCode
        *   DslhVareoAddTokenValue

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/14/05    initial revision.

**********************************************************************/


#include "dslh_vareo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVareoCfgTokenTable
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulTableSize
            );

    description:

        This function is called to configure the token table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulTableSize
                Specifies the size of token table.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVareoCfgTokenTable
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulTableSize
    )
{
    UNREFERENCED_PARAMETER(ulTableSize);
    PDSLH_VAR_ENTITY_OBJECT         pMyObject    = (PDSLH_VAR_ENTITY_OBJECT)hThisObject;

    /*
     * Bin Zhu:
     *
     * It's not necessary to create several hash table to handle the enumeration.
     * I would say one single Queue is good enough.
     */
    pMyObject->TokenTable     = (QUEUE_HEADER*)AnscAllocateMemory(sizeof(QUEUE_HEADER));

    if ( !pMyObject->TokenTable )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    AnscQueueInitializeHeader(pMyObject->TokenTable);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhVareoGetTokenValueByString
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pString
            );

    description:

        This function is called to retrieve a token value by matching
        the token string.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pString
                Specifies the token string to be matched.

    return:     token value.

**********************************************************************/

ANSC_HANDLE
DslhVareoGetTokenValueByString
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pString
    )
{
    PDSLH_VAR_ENTITY_OBJECT         pMyObject    = (PDSLH_VAR_ENTITY_OBJECT)hThisObject;
    PDSLH_VAREO_TOKEN_VALUE         pTokenValue  = (PDSLH_VAREO_TOKEN_VALUE)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(pMyObject->TokenTable);

    while ( pSLinkEntry )
    {
        pTokenValue = ACCESS_DSLH_VAREO_TOKEN_VALUE(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( strcmp(pTokenValue->String,pString) == 0 )
        {
            return  (ANSC_HANDLE)pTokenValue;
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhVareoGetTokenValueByCode
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulCode
            );

    description:

        This function is called to retrieve a token value by matching
        the token code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulCode
                Specifies the token code to be matched.

    return:     token value.

**********************************************************************/

ANSC_HANDLE
DslhVareoGetTokenValueByCode
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulCode
    )
{
    PDSLH_VAR_ENTITY_OBJECT         pMyObject    = (PDSLH_VAR_ENTITY_OBJECT)hThisObject;
    PDSLH_VAREO_TOKEN_VALUE         pTokenValue  = (PDSLH_VAREO_TOKEN_VALUE)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY     )NULL;

    pSLinkEntry = AnscQueueGetFirstEntry(pMyObject->TokenTable);

    while ( pSLinkEntry )
    {
        pTokenValue = ACCESS_DSLH_VAREO_TOKEN_VALUE(pSLinkEntry);
        pSLinkEntry = AnscQueueGetNextEntry(pSLinkEntry);

        if ( pTokenValue->EnumCode == ulCode )
        {
            return  (ANSC_HANDLE)pTokenValue;
        }
    }

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVareoAddTokenValue
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pString,
                ULONG                       ulEnumCode
            );

    description:

        This function is called to add a token value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pString
                Specifies the token string to be added.

                ULONG                       ulEnumCode
                Specifies the token enum code to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVareoAddTokenValue
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pString,
        ULONG                       ulEnumCode
    )
{
    PDSLH_VAR_ENTITY_OBJECT         pMyObject    = (PDSLH_VAR_ENTITY_OBJECT)hThisObject;
    PDSLH_VAREO_TOKEN_VALUE         pTokenValue  = (PDSLH_VAREO_TOKEN_VALUE)NULL;

    pTokenValue = (PDSLH_VAREO_TOKEN_VALUE)AnscAllocateMemory(sizeof(DSLH_VAREO_TOKEN_VALUE));

    if ( !pTokenValue )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pTokenValue->String   = AnscCloneString(pString);
        pTokenValue->EnumCode = ulEnumCode;
    }

    AnscQueuePushEntry(pMyObject->TokenTable, &pTokenValue->Linkage);

    return  ANSC_STATUS_SUCCESS;
}
