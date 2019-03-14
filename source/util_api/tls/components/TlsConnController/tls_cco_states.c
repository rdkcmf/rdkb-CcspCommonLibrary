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

    module:	tls_cco_states.c

        For Transport Layer Security Implementation (TLS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the TLS Conn Controller Object.

        *   TlsCcoGetProperty
        *   TlsCcoSetProperty
        *   TlsCcoResetProperty

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/26/03    initial revision.

**********************************************************************/


#include "tls_cco_global.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoGetProperty
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
TlsCcoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONN_CONTROLLER_PROPERTY   pProperty    = (PTLS_CONN_CONTROLLER_PROPERTY)&pMyObject->Property;

    *(PTLS_CONN_CONTROLLER_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoSetProperty
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
TlsCcoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONN_CONTROLLER_PROPERTY   pProperty    = (PTLS_CONN_CONTROLLER_PROPERTY)&pMyObject->Property;

    *pProperty = *(PTLS_CONN_CONTROLLER_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        TlsCcoResetProperty
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
TlsCcoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PTLS_CONN_CONTROLLER_OBJECT     pMyObject    = (PTLS_CONN_CONTROLLER_OBJECT  )hThisObject;
    PTLS_CONNECTION_PARAMS          pConnParams  = (PTLS_CONNECTION_PARAMS       )&pMyObject->ConnectionParams;

    TlsInitConnParams(pConnParams);

    return  ANSC_STATUS_SUCCESS;
}

