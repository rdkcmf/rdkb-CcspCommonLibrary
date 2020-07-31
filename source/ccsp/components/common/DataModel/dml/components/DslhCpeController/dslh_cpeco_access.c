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

    module: dslh_cpeco_access.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Dslh Cpe Controller Object.

        *   DslhCpecoGetDslhMprIf
        *   DslhCpecoGetDslhMpaIf
        *   DslhCpecoSetDbusHandle

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/13/05    initial revision.

**********************************************************************/


#include "dslh_cpeco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhCpecoGetDslhMprIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
DslhCpecoGetDslhMprIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject        = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase = (PDSLH_WMP_DATABASE_OBJECT    )pMyObject->hDslhWmpDatabase;

    return  pDslhWmpDatabase->hDslhMprIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        DslhCpecoGetDslhMpaIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
DslhCpecoGetDslhMpaIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject        = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase = (PDSLH_WMP_DATABASE_OBJECT    )pMyObject->hDslhWmpDatabase;

    return  pDslhWmpDatabase->hDslhMpaIf;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoSetDbusHandle
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHandle
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_STATUS
DslhCpecoSetDbusHandle
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHandle
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject          = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent= (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;

    pDslhDataModelAgent->hDbusHandle = hHandle;
    pMyObject->hDbusHandle           = hHandle;

    return ANSC_STATUS_SUCCESS;
}
