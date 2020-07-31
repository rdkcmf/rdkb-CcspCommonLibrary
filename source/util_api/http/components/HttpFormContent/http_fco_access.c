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

    module:	http_fco_access.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Form Content Object.

        *   HttpFcoGetUriParams
        *   HttpFcoGetFormInput
        *   HttpFcoGetFormParam
        *   HttpFcoGetFormParam2

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
        HttpFcoGetUriParams
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the HTML form input.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     form input.

**********************************************************************/

ANSC_HANDLE
HttpFcoGetUriParams
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject    = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_INPUT                pUriParams   = (PHTTP_FORM_INPUT         )pMyObject->hUriParams;

    return  (ANSC_HANDLE)pUriParams;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpFcoGetFormInput
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the HTML form input.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     form input.

**********************************************************************/

ANSC_HANDLE
HttpFcoGetFormInput
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject    = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_INPUT                pFormInput   = (PHTTP_FORM_INPUT         )pMyObject->hFormInput;

    return  (ANSC_HANDLE)pFormInput;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpFcoGetFormParam
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the HTML form element.

    return:     form param.

**********************************************************************/

char*
HttpFcoGetFormParam
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject    = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_ELEMENT              pFormElement = NULL;

    pFormElement =
        (PHTTP_FORM_ELEMENT)pMyObject->GetFormElement
            (
                (ANSC_HANDLE)pMyObject,
                name
            );

    if ( pFormElement )
    {
        return  pFormElement->Value;
    }

    return  NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void*
        HttpFcoGetFormParam2
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                PULONG                      pulSize
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the HTML form element.

                PULONG                      pulSize
                Specifies the size of the parameter to be returned.

    return:     form param.

**********************************************************************/

void*
HttpFcoGetFormParam2
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        PULONG                      pulSize
    )
{
    PHTTP_FORM_CONTENT_OBJECT       pMyObject    = (PHTTP_FORM_CONTENT_OBJECT)hThisObject;
    PHTTP_FORM_ELEMENT              pFormElement = NULL;

    pFormElement =
        (PHTTP_FORM_ELEMENT)pMyObject->GetFormElement
            (
                (ANSC_HANDLE)pMyObject,
                name
            );

    if ( pFormElement )
    {
        *pulSize = pFormElement->Size;

        return  pFormElement->Value;
    }
    else
    {
        *pulSize = 0;
    }

    return  NULL;
}
