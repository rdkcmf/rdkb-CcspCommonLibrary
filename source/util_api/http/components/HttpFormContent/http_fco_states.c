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

    module:	http_fco_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Form Content Object.

        *   HttpFcoGetHfpIf
        *   HttpFcoSetHfpIf
        *   HttpFcoReset

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/17/02    initial revision.

**********************************************************************/


#include "http_fco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpFcoGetHfpIf
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
HttpFcoGetHfpIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject    = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;

    return  pMyObject->hHfpIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpFcoSetHfpIf
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
HttpFcoSetHfpIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject    = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;

    pMyObject->hHfpIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpFcoReset
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
HttpFcoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject    = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_INPUT                pUriParams   = (PHTTP_FORM_INPUT         )pMyObject->hUriParams;
    PHTTP_FORM_INPUT                pFormInput   = (PHTTP_FORM_INPUT         )pMyObject->hFormInput;

    pMyObject->DelAllFeos((ANSC_HANDLE)pMyObject);

    if ( pUriParams )
    {
        HttpFreeFormInput(pUriParams);

        pMyObject->hUriParams = (ANSC_HANDLE)NULL;
    }

    if ( pFormInput )
    {
        HttpFreeFormInput(pFormInput);

        pMyObject->hFormInput = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
