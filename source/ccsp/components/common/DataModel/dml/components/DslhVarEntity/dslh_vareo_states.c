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

    module:	dslh_vareo_states.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Var Entity Object.

        *   DslhVareoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/27/05    initial revision.

**********************************************************************/


#include "dslh_vareo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhVareoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhVareoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_VAR_ENTITY_OBJECT         pMyObject    = (PDSLH_VAR_ENTITY_OBJECT)hThisObject;
    PDSLH_VAREO_TOKEN_VALUE         pTokenValue  = (PDSLH_VAREO_TOKEN_VALUE)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry  = (PSINGLE_LINK_ENTRY     )NULL;

    if ( pMyObject->ParamDescr )
    {
    	DslhCwmpFreeParamDescr(pMyObject->ParamDescr);
    	pMyObject->ParamDescr = NULL;
    }

    if ( pMyObject->TokenTable )
    {
        pSLinkEntry = AnscQueuePopEntry(pMyObject->TokenTable);

        while ( pSLinkEntry )
        {
            pTokenValue = ACCESS_DSLH_VAREO_TOKEN_VALUE(pSLinkEntry);
            pSLinkEntry = AnscQueuePopEntry(pMyObject->TokenTable);

            DslhVareoFreeTokenValue(pTokenValue);
        }

        AnscFreeMemory(pMyObject->TokenTable);

        pMyObject->TokenTable = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
