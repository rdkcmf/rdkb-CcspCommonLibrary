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

    module:	http_atomethod_operation.c

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Ato Method Object.

        *   HttpAtoMethodCreateCodeTable
        *   HttpAtoMethodCreateNameTable
        *   HttpAtoMethodGetAtomCode
        *   HttpAtoMethodGetAtomName

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/15/02    initial revision.

**********************************************************************/


#include "http_atomethod_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAtoMethodCreateCodeTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the atom table for HTTP CGI environment
        variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAtoMethodCreateCodeTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ATO_METHOD_OBJECT         pMyObject    = (PHTTP_ATO_METHOD_OBJECT)hThisObject;

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_OPTIONS,
            HTTP_METHOD_CODE_OPTIONS,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_GET,
            HTTP_METHOD_CODE_GET,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_HEAD,
            HTTP_METHOD_CODE_HEAD,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_POST,
            HTTP_METHOD_CODE_POST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_PUT,
            HTTP_METHOD_CODE_PUT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_DELETE,
            HTTP_METHOD_CODE_DELETE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_TRACE,
            HTTP_METHOD_CODE_TRACE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomByName
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_NAME_CONNECT,
            HTTP_METHOD_CODE_CONNECT,
            (ANSC_HANDLE)NULL
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpAtoMethodCreateNameTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function creates the atom table for HTTP CGI environment
        variables.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpAtoMethodCreateNameTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_ATO_METHOD_OBJECT         pMyObject    = (PHTTP_ATO_METHOD_OBJECT)hThisObject;

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_OPTIONS,
            HTTP_METHOD_NAME_OPTIONS,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_GET,
            HTTP_METHOD_NAME_GET,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_HEAD,
            HTTP_METHOD_NAME_HEAD,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_POST,
            HTTP_METHOD_NAME_POST,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_PUT,
            HTTP_METHOD_NAME_PUT,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_DELETE,
            HTTP_METHOD_NAME_DELETE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_TRACE,
            HTTP_METHOD_NAME_TRACE,
            (ANSC_HANDLE)NULL
        );

    pMyObject->AddAtomById
        (
            (ANSC_HANDLE)pMyObject,
            HTTP_METHOD_CODE_CONNECT,
            HTTP_METHOD_NAME_CONNECT,
            (ANSC_HANDLE)NULL
        );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        HttpAtoMethodGetAtomCode
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve the method code for
        the corresponding method name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the method name to be matched.

    return:     method code.

**********************************************************************/

ULONG
HttpAtoMethodGetAtomCode
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PHTTP_ATO_METHOD_OBJECT         pMyObject       = (PHTTP_ATO_METHOD_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pMyObject->GetAtomByName
            (
                (ANSC_HANDLE)pMyObject,
                name
            );

    if ( !pAtomDescriptor )
    {
        return  0;
    }

    return  pAtomDescriptor->AtomId;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        HttpAtoMethodGetAtomName
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       code
            );

    description:

        This function is called to retrieve the method name for
        the corresponding method code.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       code
                Specifies the method code to be matched.

    return:     method name.

**********************************************************************/

char*
HttpAtoMethodGetAtomName
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       code
    )
{
    PHTTP_ATO_METHOD_OBJECT         pMyObject       = (PHTTP_ATO_METHOD_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pMyObject->GetAtomById
            (
                (ANSC_HANDLE)pMyObject,
                code
            );

    if ( !pAtomDescriptor )
    {
        return  NULL;
    }

    return  pAtomDescriptor->AtomName;
}
