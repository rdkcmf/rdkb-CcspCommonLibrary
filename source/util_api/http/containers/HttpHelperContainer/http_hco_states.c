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

    module:	http_hco_states.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Http Basic Message Object.

        *   HttpHcoGetAtoStatus
        *   HttpHcoGetAtoHeader
        *   HttpHcoGetAtoMethod
        *   HttpHcoGetAtoCgiEnv
        *   HttpHcoGetAtoFxCat1
        *   HttpHcoGetAtoFxCat2
        *   HttpHcoGetSmartClock

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#include "http_hco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpHcoGetAtoStatus
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
HttpHcoGetAtoStatus
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hAtoStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpHcoGetAtoHeader
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
HttpHcoGetAtoHeader
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hAtoHeader;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpHcoGetAtoMethod
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
HttpHcoGetAtoMethod
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hAtoMethod;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpHcoGetAtoCgiEnv
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
HttpHcoGetAtoCgiEnv
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hAtoCgiEnv;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpHcoGetAtoFxCat1
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
HttpHcoGetAtoFxCat1
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hAtoFxCat1;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpHcoGetAtoFxCat2
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
HttpHcoGetAtoFxCat2
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hAtoFxCat2;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        HttpHcoGetSmartClock
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
HttpHcoGetSmartClock
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;

    return  pMyObject->hSmartClock;
}
