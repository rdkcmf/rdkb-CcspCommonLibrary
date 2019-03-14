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

    module:	http_autho_state.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Authentication Object.

        *   HttpAuthoSetHttpAuthAcmIf
        *   HttpAuthoEnable
        *   HttpAuthoIsEnabled
        *   HttpAuthoReset
        *   HttpAuthoGetCryptoObject

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        10/07/05    initial revision.

**********************************************************************/


#include "http_autho_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoEnable
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnabled
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnabled
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoEnable
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    )
{
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT)hThisObject;

    pMyObject->bAuthOn  = bEnabled;

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        HttpAuthoIsEnabled
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to get object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

BOOL
HttpAuthoIsEnabled
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT)hThisObject;

    return pMyObject->bAuthOn;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAuthoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAuthoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_AUTH_OBJECT               pMyObject    = (PHTTP_AUTH_OBJECT)hThisObject;

    pMyObject->bAuthOn  = TRUE;

    return ANSC_STATUS_SUCCESS;
}


