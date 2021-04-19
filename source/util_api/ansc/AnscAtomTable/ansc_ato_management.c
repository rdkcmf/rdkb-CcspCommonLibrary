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

    module:	ansc_ato_management.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Ansc Atom Table Object.

        *   AnscAtoAddAtomByName
        *   AnscAtoGetAtomByName
        *   AnscAtoDelAtomByName
        *   AnscAtoDelAllStrAtoms
        *   AnscAtoAddAtomById
        *   AnscAtoGetAtomById
        *   AnscAtoDelAtomById
        *   AnscAtoDelAllIntAtoms

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
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAtoAddAtomByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name,
                ULONG                       id,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to add an atom object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the atom to be added.

                ULONG                       id
                Specifies the id of the atom to be added.

                ANSC_HANDLE                 hContext
                Specifies the context handle associated with the atom.

    return:     atom handle.

**********************************************************************/

ANSC_HANDLE
AnscAtoAddAtomByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name,
        ULONG                       id,
        ANSC_HANDLE                 hContext
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    ULONG                           ulHashIndex     = 0;
    errno_t                         rc              = -1;

    if ( !name || !AnscSizeOfString(name) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        ulHashIndex     = AnscHashString(name, AnscSizeOfString(name), pMyObject->AdoStrTableSize);
        pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR)AnscAllocateMemory(sizeof(ANSC_ATOM_DESCRIPTOR));
    }

    if ( !pAtomDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pAtomDescriptor->HashIndex = ulHashIndex;
        pAtomDescriptor->AtomName  = AnscAllocateMemory(AnscSizeOfString(name) + 2);
        pAtomDescriptor->AtomId    = id;
        pAtomDescriptor->hContext  = hContext;

        rc = strcpy_s(pAtomDescriptor->AtomName, AnscSizeOfString(name) + 2, name);
        ERR_CHK(rc);
    }

    AnscAcquireSpinLock(&pMyObject->AdoStrTableSpinLock);
    AnscSListPushEntry (&pMyObject->AdoStrTable[ulHashIndex], &pAtomDescriptor->Linkage);
    AnscReleaseSpinLock(&pMyObject->AdoStrTableSpinLock);

    return  (ANSC_HANDLE)pAtomDescriptor;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAtoGetAtomByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to retrieve an atom object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the atom to be matched.

    return:     atom handle.

**********************************************************************/

ANSC_HANDLE
AnscAtoGetAtomByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulHashIndex     = 0;

    if ( !name || !AnscSizeOfString(name) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        ulHashIndex = AnscHashString(name, AnscSizeOfString(name), pMyObject->AdoStrTableSize);
    }

    AnscAcquireSpinLock(&pMyObject->AdoStrTableSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->AdoStrTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pAtomDescriptor = ACCESS_ANSC_ATOM_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry     = AnscSListGetNextEntry(pSLinkEntry);

        if ( strcmp(pAtomDescriptor->AtomName, name) == 0 )
        {
            AnscReleaseSpinLock(&pMyObject->AdoStrTableSpinLock);

            return  (ANSC_HANDLE)pAtomDescriptor;
        }
    }

    AnscReleaseSpinLock(&pMyObject->AdoStrTableSpinLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoDelAtomByName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       name
            );

    description:

        This function is called to delete an atom object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       name
                Specifies the name of the atom to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoDelAtomByName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       name
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pMyObject->GetAtomByName
            (
                (ANSC_HANDLE)pMyObject,
                name
            );

    if ( !pAtomDescriptor )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireSpinLock    (&pMyObject->AdoStrTableSpinLock);
    AnscSListPopEntryByLink(&pMyObject->AdoStrTable[pAtomDescriptor->HashIndex], &pAtomDescriptor->Linkage);
    AnscReleaseSpinLock    (&pMyObject->AdoStrTableSpinLock);

    if ( pAtomDescriptor->AtomName )
    {
        AnscFreeMemory(pAtomDescriptor->AtomName);
    }

    AnscFreeMemory(pAtomDescriptor);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoDelAllStrAtoms
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all atom objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoDelAllStrAtoms
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           i               = 0;

    AnscAcquireSpinLock(&pMyObject->AdoStrTableSpinLock);

    for ( i = 0; i < pMyObject->AdoStrTableSize; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->AdoStrTable[i]);

        while ( pSLinkEntry )
        {
            pAtomDescriptor = ACCESS_ANSC_ATOM_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry     = AnscSListPopEntry(&pMyObject->AdoStrTable[i]);

            if ( pAtomDescriptor->AtomName )
            {
                AnscFreeMemory(pAtomDescriptor->AtomName);
            }

            AnscFreeMemory(pAtomDescriptor);
        }
    }

    AnscReleaseSpinLock(&pMyObject->AdoStrTableSpinLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAtoAddAtomById
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       id,
                char*                       name,
                ANSC_HANDLE                 hContext
            );

    description:

        This function is called to add an atom object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       id
                Specifies the id of the atom to be added.

                char*                       name
                Specifies the name of the atom to be added.

                ANSC_HANDLE                 hContext
                Specifies the context handle associated with the atom.

    return:     atom handle.

**********************************************************************/

ANSC_HANDLE
AnscAtoAddAtomById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id,
        char*                       name,
        ANSC_HANDLE                 hContext
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    ULONG                           ulHashIndex     = 0;
    errno_t                         rc              = -1;

    ulHashIndex     = AnscHashUlong(id, pMyObject->AdoIntTableSize);
    pAtomDescriptor = (PANSC_ATOM_DESCRIPTOR)AnscAllocateMemory(sizeof(ANSC_ATOM_DESCRIPTOR));

    if ( !pAtomDescriptor )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pAtomDescriptor->HashIndex = ulHashIndex;
        pAtomDescriptor->AtomName  = (name == NULL)? NULL: AnscAllocateMemory(AnscSizeOfString(name) + 2);
        pAtomDescriptor->AtomId    = id;
        pAtomDescriptor->hContext  = hContext;

        if ( name && pAtomDescriptor->AtomName )
        {
            rc = strcpy_s(pAtomDescriptor->AtomName, AnscSizeOfString(name) + 2, name);
            ERR_CHK(rc);
        }
    }

    AnscAcquireSpinLock(&pMyObject->AdoIntTableSpinLock);
    AnscSListPushEntry (&pMyObject->AdoIntTable[ulHashIndex], &pAtomDescriptor->Linkage);
    AnscReleaseSpinLock(&pMyObject->AdoIntTableSpinLock);

    return  (ANSC_HANDLE)pAtomDescriptor;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        AnscAtoGetAtomById
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       id
            );

    description:

        This function is called to retrieve an atom object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       id
                Specifies the id of the atom to be matched.

    return:     atom handle.

**********************************************************************/

ANSC_HANDLE
AnscAtoGetAtomById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           ulHashIndex     = 0;

    ulHashIndex = AnscHashUlong(id, pMyObject->AdoIntTableSize);

    AnscAcquireSpinLock(&pMyObject->AdoIntTableSpinLock);

    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->AdoIntTable[ulHashIndex]);

    while ( pSLinkEntry )
    {
        pAtomDescriptor = ACCESS_ANSC_ATOM_DESCRIPTOR(pSLinkEntry);
        pSLinkEntry     = AnscSListGetNextEntry(pSLinkEntry);

        if ( pAtomDescriptor->AtomId == id )
        {
            AnscReleaseSpinLock(&pMyObject->AdoIntTableSpinLock);

            return  (ANSC_HANDLE)pAtomDescriptor;
        }
    }

    AnscReleaseSpinLock(&pMyObject->AdoIntTableSpinLock);

    return  (ANSC_HANDLE)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoDelAtomById
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       id
            );

    description:

        This function is called to delete an atom object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       id
                Specifies the id of the atom to be matched.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoDelAtomById
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       id
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;

    pAtomDescriptor =
        (PANSC_ATOM_DESCRIPTOR)pMyObject->GetAtomById
            (
                (ANSC_HANDLE)pMyObject,
                id
            );

    if ( !pAtomDescriptor )
    {
        return  ANSC_STATUS_SUCCESS;
    }

    AnscAcquireSpinLock    (&pMyObject->AdoIntTableSpinLock);
    AnscSListPopEntryByLink(&pMyObject->AdoIntTable[pAtomDescriptor->HashIndex], &pAtomDescriptor->Linkage);
    AnscReleaseSpinLock    (&pMyObject->AdoIntTableSpinLock);

    if ( pAtomDescriptor->AtomName )
    {
        AnscFreeMemory(pAtomDescriptor->AtomName);
    }

    AnscFreeMemory(pAtomDescriptor);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoDelAllIntAtoms
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all atom objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoDelAllIntAtoms
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject       = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    PANSC_ATOM_DESCRIPTOR           pAtomDescriptor = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;
    ULONG                           i               = 0;

    AnscAcquireSpinLock(&pMyObject->AdoIntTableSpinLock);

    for ( i = 0; i < pMyObject->AdoIntTableSize; i++ )
    {
        pSLinkEntry = AnscSListPopEntry(&pMyObject->AdoIntTable[i]);

        while ( pSLinkEntry )
        {
            pAtomDescriptor = ACCESS_ANSC_ATOM_DESCRIPTOR(pSLinkEntry);
            pSLinkEntry     = AnscSListPopEntry(&pMyObject->AdoIntTable[i]);

            if ( pAtomDescriptor->AtomName )
            {
                AnscFreeMemory(pAtomDescriptor->AtomName);
            }

            AnscFreeMemory(pAtomDescriptor);
        }
    }

    AnscReleaseSpinLock(&pMyObject->AdoIntTableSpinLock);

    return  ANSC_STATUS_SUCCESS;
}
