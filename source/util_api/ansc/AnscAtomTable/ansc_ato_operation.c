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

    module:	ansc_ato_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Atom Table Object.

        *   AnscAtoAllocateIntTable
        *   AnscAtoAllocateStrTable
        *   AnscAtoGetAtomCount
        *   AnscAtoGetAtomByIndex

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        12/07/01    initial revision.

**********************************************************************/


#include "ansc_ato_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoAllocateIntTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function allocates resource for the Atom Table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoAllocateIntTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    ULONG                           i            = 0;

    if ( pMyObject->AdoIntTable )
    {
        pMyObject->DelAllIntAtoms((ANSC_HANDLE)pMyObject);

        AnscFreeMemory(pMyObject->AdoIntTable);
    }

    pMyObject->AdoIntTable = (SLIST_HEADER*)AnscAllocateMemory(sizeof(SLIST_HEADER) * pMyObject->AdoIntTableSize);

    if ( !pMyObject->AdoIntTable )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    for ( i = 0; i < pMyObject->AdoIntTableSize; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->AdoIntTable[i]);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoAllocateStrTable
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function allocates resource for the Atom Table.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoAllocateStrTable
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    ULONG                           i            = 0;

    if ( pMyObject->AdoStrTable )
    {
        pMyObject->DelAllStrAtoms((ANSC_HANDLE)pMyObject);

        AnscFreeMemory(pMyObject->AdoStrTable);
    }

    pMyObject->AdoStrTable = (SLIST_HEADER*)AnscAllocateMemory(sizeof(SLIST_HEADER) * pMyObject->AdoStrTableSize);

    if ( !pMyObject->AdoStrTable )
    {
        return  ANSC_STATUS_RESOURCES;
    }

    for ( i = 0; i < pMyObject->AdoStrTableSize; i++ )
    {
        AnscSListInitializeHeader(&pMyObject->AdoStrTable[i]);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscAtoGetAtomCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function retrieves the number of atoms.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     number of atoms.

**********************************************************************/

ULONG
AnscAtoGetAtomCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;

    return  pMyObject->GetIntAtomCount((ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAtoGetAtomByIndex
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       index
            );

    description:

        This function retrieves an atom by matching the index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       index
                Specifies the atom index to be matched.

    return:     atom handle.

**********************************************************************/

ANSC_HANDLE
AnscAtoGetAtomByIndex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulSkipCount     = 0;
    ULONG                           i               = 0;

    AnscAcquireSpinLock(&pMyObject->AdoIntTableSpinLock);

    for ( i = 0; i < pMyObject->AdoIntTableSize; i++ )
    {
        if ( AnscSListQueryDepth(&pMyObject->AdoIntTable[i]) <= (index - ulSkipCount) )
        {
            ulSkipCount += AnscSListQueryDepth(&pMyObject->AdoIntTable[i]);

            continue;
        }
        else
        {
            pSLinkEntry     = AnscSListSearchEntryByIndex(&pMyObject->AdoIntTable[i], index - ulSkipCount);
            pAtomDescriptor = ACCESS_ANSC_ATOM_DESCRIPTOR(pSLinkEntry);
        }

        AnscReleaseSpinLock(&pMyObject->AdoIntTableSpinLock);

        return  (ANSC_HANDLE)pAtomDescriptor;
    }

    AnscReleaseSpinLock(&pMyObject->AdoIntTableSpinLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscAtoGetAtomCount2
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function retrieves the number of atoms.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     number of atoms.

**********************************************************************/

ULONG
AnscAtoGetAtomCount2
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;

    return  pMyObject->GetStrAtomCount((ANSC_HANDLE)pMyObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAtoGetAtomByIndex2
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       index
            );

    description:

        This function retrieves an atom by matching the index.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       index
                Specifies the atom index to be matched.

    return:     atom handle.

**********************************************************************/

ANSC_HANDLE
AnscAtoGetAtomByIndex2
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       index
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulSkipCount     = 0;
    ULONG                           i               = 0;

    AnscAcquireSpinLock(&pMyObject->AdoStrTableSpinLock);

    for ( i = 0; i < pMyObject->AdoStrTableSize; i++ )
    {
        if ( AnscSListQueryDepth(&pMyObject->AdoStrTable[i]) <= (index - ulSkipCount) )
        {
            ulSkipCount += AnscSListQueryDepth(&pMyObject->AdoStrTable[i]);

            continue;
        }
        else
        {
            pSLinkEntry     = AnscSListSearchEntryByIndex(&pMyObject->AdoStrTable[i], index - ulSkipCount);
            pAtomDescriptor = ACCESS_ANSC_ATOM_DESCRIPTOR(pSLinkEntry);
        }

        AnscReleaseSpinLock(&pMyObject->AdoStrTableSpinLock);

        return  (ANSC_HANDLE)pAtomDescriptor;
    }

    AnscReleaseSpinLock(&pMyObject->AdoStrTableSpinLock);

    return  (ANSC_HANDLE)NULL;
}


