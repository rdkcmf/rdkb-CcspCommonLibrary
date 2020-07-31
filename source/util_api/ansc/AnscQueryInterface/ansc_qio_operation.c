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

    module:	ansc_qio_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Query Interface Object.

        *   AnscQioAddIf
        *   AnscQioDelIf
        *   AnscQioQueryIf

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/11/02    initial revision.

**********************************************************************/


#include "ansc_qio_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscQioAddIf
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pIfName,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to add an interface mapping.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pIfName
                Specifies the inteface name to be added.

                ANSC_HANDLE                 hInterface
                Specifies the interface context to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscQioAddIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName,
        ANSC_HANDLE                 hInterface
    )
{
    PANSC_QUERY_INTERFACE_OBJECT    pMyObject       = (PANSC_QUERY_INTERFACE_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoIf          = (PANSC_ATOM_TABLE_OBJECT     )pMyObject->hAtoIf;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoIf->GetAtomByName
            (
                (ANSC_HANDLE)pAtoIf,
                pIfName
            );

    if ( pAtomDescriptor )
    {
        pAtomDescriptor->AtomId   = (ULONG)hInterface;
        pAtomDescriptor->hContext = hInterface;

        return  ANSC_STATUS_SUCCESS;
    }
    else
    {
        pAtomDescriptor =
            (PANSC_ATOM_DESCRIPTOR)pAtoIf->AddAtomByName
                (
                    (ANSC_HANDLE)pAtoIf,
                    pIfName,
                    (ULONG)hInterface,
                    hInterface
                );
    }

    if ( !pAtomDescriptor )
    {
        return  ANSC_STATUS_UNAPPLICABLE;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscQioDelIf
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pIfName
            );

    description:

        This function is called to delete an interface mapping.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pIfName
                Specifies the inteface name to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscQioDelIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_QUERY_INTERFACE_OBJECT    pMyObject    = (PANSC_QUERY_INTERFACE_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoIf       = (PANSC_ATOM_TABLE_OBJECT     )pMyObject->hAtoIf;

    returnStatus =
        pAtoIf->DelAtomByName
            (
                (ANSC_HANDLE)pAtoIf,
                pIfName
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscQioQueryIf
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pIfName
            );

    description:

        This function is called to retrieve an interface mapping.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pIfName
                Specifies the inteface name to be matched.

    return:     interface context.

**********************************************************************/

ANSC_HANDLE
AnscQioQueryIf
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pIfName
    )
{
    PANSC_QUERY_INTERFACE_OBJECT    pMyObject       = (PANSC_QUERY_INTERFACE_OBJECT)hThisObject;
    PANSC_ATOM_TABLE_OBJECT         pAtoIf          = (PANSC_ATOM_TABLE_OBJECT     )pMyObject->hAtoIf;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pAtoIf->GetAtomByName
            (
                (ANSC_HANDLE)pAtoIf,
                pIfName
            );

    if ( pAtomDescriptor )
    {
        return  pAtomDescriptor->hContext;
    }

    return  (ANSC_HANDLE)NULL;
}
