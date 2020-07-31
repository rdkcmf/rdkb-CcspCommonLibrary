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

    module:	bwsp_wso_states.c

        For Broadway Web Service Plane Implementation (BWSP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bwsp Web Servlet Object.

        *   BwspWsoGetBwspWshIf
        *   BwspWsoSetBwspWshIf
        *   BwspWsoResetProperty
        *   BwspWsoReset

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


#include "bwsp_wso_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BwspWsoGetBwspWshIf
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
BwspWsoGetBwspWshIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = (PBWSP_WEB_SERVLET_OBJECT  )hThisObject;

    return  pMyObject->hBwspWshIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoSetBwspWshIf
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
BwspWsoSetBwspWshIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = (PBWSP_WEB_SERVLET_OBJECT  )hThisObject;
    PBWSP_WSH_INTERFACE             pBwspWshIf   = (PBWSP_WSH_INTERFACE       )pMyObject->hBwspWshIf;

    *(PBWSP_WSH_INTERFACE)pBwspWshIf = *(PBWSP_WSH_INTERFACE)hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoResetProperty
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
BwspWsoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = (PBWSP_WEB_SERVLET_OBJECT  )hThisObject;

    WebRooResetProperty((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BwspWsoResetProperty
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
BwspWsoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBWSP_WEB_SERVLET_OBJECT        pMyObject    = (PBWSP_WEB_SERVLET_OBJECT  )hThisObject;

    WebRooReset((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
