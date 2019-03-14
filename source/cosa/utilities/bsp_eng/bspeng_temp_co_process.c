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

    module:	bspeng_temp_co_process.c

        For BSP Engine Temp Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BSP engine temp Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateTempClear
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to clear this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateTempClear
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_TEMP_OBJECT       pMyObject = (PBSP_TEMPLATE_TEMP_OBJECT)hThisObject;

    pMyObject->Count    = 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateTempRelease
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to release all branch objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     void.

**********************************************************************/

void
BspTemplateTempRelease
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_TEMP_OBJECT       pMyObject = (PBSP_TEMPLATE_TEMP_OBJECT)hThisObject;
    ULONG                           i;
    PBSP_TEMPLATE_BRANCH_OBJECT     pBr;

    for (i = 0; i < pMyObject->Count; i ++)
    {
        pBr = (PBSP_TEMPLATE_BRANCH_OBJECT)pMyObject->hBranchList[i];
        
        if (pBr)
            BspTemplateBranchCORemove((ANSC_HANDLE)pBr);
    }

    pMyObject->Clear(hThisObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateTempSet
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hBranch
            )

    description:

        This function is called to save the specified branch object
        into branch list.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hBranch
                The branch object to be saved.

    return:     handle to the branch.

**********************************************************************/

ANSC_HANDLE
BspTemplateTempSet
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hBranch
    )
{
    PBSP_TEMPLATE_TEMP_OBJECT       pMyObject = (PBSP_TEMPLATE_TEMP_OBJECT)hThisObject;

    pMyObject->hBranchList[pMyObject->Count ++] = hBranch;

    return hBranch;
}

