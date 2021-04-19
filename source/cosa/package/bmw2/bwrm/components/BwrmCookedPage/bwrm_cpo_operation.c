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

    module:	bwrm_cpo_operation.c

        For Broadway Web Resource Manager Implementation (BWRM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bwrm Cooked Page Object.

        *   BwrmPmoMatchPath

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/02/03    initial revision.

**********************************************************************/


#include "bwrm_cpo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BwrmCpoMatchPath
            (
                ANSC_HANDLE                 hThisObject,
                char*                       root_path,
                char*                       page_path
            );

    description:

        This function is called to match the complete path of the page.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       root_path
                Specifies the root path of the page to be matched.

                char*                       page_path
                Specifies the file path of the page to be matched.

    return:     status of operation.

**********************************************************************/

BOOL
BwrmCpoMatchPath
    (
        ANSC_HANDLE                 hThisObject,
        char*                       root_path,
        char*                       page_path
    )
{
    PBWRM_COOKED_PAGE_OBJECT        pMyObject    = (PBWRM_COOKED_PAGE_OBJECT)hThisObject;

    if ( !root_path || !page_path )
    {
        return  FALSE;
    }
    else if ( !pMyObject->RootPath || !pMyObject->PagePath )
    {
        return  FALSE;
    }
    else if ( !strcasecmp(root_path, pMyObject->RootPath) == 0 )
    {
        return  FALSE;
    }
    else if ( !strcasecmp(page_path, pMyObject->PagePath) == 0 )
    {
        return  FALSE;
    }

    return  TRUE;
}
