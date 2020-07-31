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

    module:	bwsp_sco_operation.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Bwsp Site Creator Object.

        *   BwspScoEngage
        *   BwspScoCancel

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/02/03    initial revision.

**********************************************************************/


#include "bwsp_sco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enage the object activity.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PWEB_HEM_INTERFACE              pWebHemIf    = (PWEB_HEM_INTERFACE         )pMyObject->hWebHemIf;

    if ( pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else if ( !pWebHemIf )
    {
        return  ANSC_STATUS_NOT_READY;
    }
    else
    {
        pMyObject->bActive = TRUE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoCancel
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
BwspScoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;

    if ( !pMyObject->bActive )
    {
        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pMyObject->bActive = FALSE;
    }

    return  ANSC_STATUS_SUCCESS;
}
