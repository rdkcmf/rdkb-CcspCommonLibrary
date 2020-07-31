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

    module: dslh_cpeco_operation.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Dslh Cpe Controller Object.

        *   DslhCpecoEngage
        *   DslhCpecoCancel
        *   DslhCpecoSetupEnv
        *   DslhCpecoCloseEnv

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/09/05    initial revision.

**********************************************************************/


#include "dslh_cpeco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase  = (PDSLH_WMP_DATABASE_OBJECT    )pMyObject->hDslhWmpDatabase;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent= (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;
    /*PDSLH_MPA_INTERFACE             pDslhMpaIf        = (PDSLH_MPA_INTERFACE          )pDslhWmpDatabase->hDslhMpaIf; *** RDKB-5788 , CID-33407, removing as this triggers coverity warning, not used*/

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    if ( pDslhWmpDatabase )
    {
        pDslhWmpDatabase->SetDslhCpeController((ANSC_HANDLE)pDslhWmpDatabase, (ANSC_HANDLE)pMyObject);
        pDslhWmpDatabase->Engage              ((ANSC_HANDLE)pDslhWmpDatabase);
    }

    if( pDslhDataModelAgent)
    {
        pDslhDataModelAgent->hDslhCpeController = hThisObject;
        pDslhDataModelAgent->Engage           ((ANSC_HANDLE)pDslhDataModelAgent);
    }
    
    returnStatus = pMyObject->SetupEnv((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        return  returnStatus;
    }

    pMyObject->bActive = TRUE;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus      = ANSC_STATUS_SUCCESS;
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase  = (PDSLH_WMP_DATABASE_OBJECT    )pMyObject->hDslhWmpDatabase;
    PDSLH_DATAMODEL_AGENT_OBJECT    pDslhDataModelAgent= (PDSLH_DATAMODEL_AGENT_OBJECT)pMyObject->hDslhDataModelAgent;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }
    
    if( pDslhDataModelAgent)
    {
        pDslhDataModelAgent->Cancel((ANSC_HANDLE)pDslhDataModelAgent);
    }

    if ( pDslhWmpDatabase )
    {
        pDslhWmpDatabase->Cancel((ANSC_HANDLE)pDslhWmpDatabase);
    }

    returnStatus = pMyObject->CloseEnv((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoSetupEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to setup the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoSetupEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PDSLH_CPE_CONTROLLER_OBJECT     pMyObject         = (PDSLH_CPE_CONTROLLER_OBJECT  )hThisObject;
    PDSLH_WMP_DATABASE_OBJECT       pDslhWmpDatabase  = (PDSLH_WMP_DATABASE_OBJECT    )pMyObject->hDslhWmpDatabase;
    PDSLH_MPR_INTERFACE             pDslhMprIf        = (PDSLH_MPR_INTERFACE          )pDslhWmpDatabase->hDslhMprIf;

    pMyObject->RegisterObject2((ANSC_HANDLE)pMyObject, DM_ROOTNAME,               NULL, NULL);

    /* Let's register CCSP base data model objects */
    if( pDslhMprIf != NULL)
    {
        pDslhMprIf->RegEmptyObject(pDslhMprIf->hOwnerContext, "com.cisco.spvtg.ccsp.");
    }
    else
    {
        pMyObject->RegisterObject2((ANSC_HANDLE)pMyObject, "com.",                  NULL, NULL);
        pMyObject->RegisterObject2((ANSC_HANDLE)pMyObject, "com.cisco.",            NULL, NULL);
        pMyObject->RegisterObject2((ANSC_HANDLE)pMyObject, "com.cisco.spvtg.",      NULL, NULL);
        pMyObject->RegisterObject2((ANSC_HANDLE)pMyObject, "com.cisco.spvtg.ccsp.", NULL, NULL);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        DslhCpecoCloseEnv
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to close the operating environment.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
DslhCpecoCloseEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return  ANSC_STATUS_SUCCESS;
}
