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

    module:	bwsp_sco_states.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2003
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bwsp Site Creator Object.

        *   BwspScoGetWebHemIf
        *   BwspScoSetWebHemIf
        *   BwspScoGetBwspWsrIf
        *   BwspScoGetBwspCspIf
        *   BwspScoGetProperty
        *   BwspScoSetProperty
        *   BwspScoResetProperty
        *   BwspScoReset

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

        ANSC_HANDLE
        BwspScoGetWebHemIf
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
BwspScoGetWebHemIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    return  pMyObject->hWebHemIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoSetWebHemIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoSetWebHemIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    pMyObject->hWebHemIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwspScoGetBwspWsrIf
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
BwspScoGetBwspWsrIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    return  pMyObject->hBwspWsrIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwspScoGetBwspCspIf
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
BwspScoGetBwspCspIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    return  pMyObject->hBwspCspIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    *(PBWSP_SITE_CREATOR_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    *pProperty = *(PBWSP_SITE_CREATOR_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BwspScoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(BWSP_SITE_CREATOR_PROPERTY));

    pProperty->SessionTimeout  = BWSP_SESSION_TIMEOUT;
    pProperty->PhoCookieMaxAge = BWSP_PHO_COOKIE_MAX_AGE;
    pProperty->LsmCookieMaxAge = BWSP_LSM_COOKIE_MAX_AGE;

    AnscCopyString(pProperty->PhoCookieName, BWSP_PHO_COOKIE_NAME);
    AnscCopyString(pProperty->LsmCookieName, BWSP_LSM_COOKIE_NAME);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspScoResetProperty
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
BwspScoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PBWSP_SITE_CREATOR_OBJECT       pMyObject    = (PBWSP_SITE_CREATOR_OBJECT  )hThisObject;
    PBWSP_SITE_CREATOR_PROPERTY     pProperty    = (PBWSP_SITE_CREATOR_PROPERTY)&pMyObject->Property;

    return  ANSC_STATUS_SUCCESS;
}
