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

    module:	ansc_ato_states.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Ansc Atom Table Object.

        *   AnscAtoGetIntAtomCount
        *   AnscAtoGetStrAtomCount
        *   AnscAtoGetIntTableSize
        *   AnscAtoSetIntTableSize
        *   AnscAtoGetStrTableSize
        *   AnscAtoSetStrTableSize
        *   AnscAtoReset

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

        ULONG
        AnscAtoGetIntAtomCount
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

ULONG
AnscAtoGetIntAtomCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    ULONG                           i            = 0;
    ULONG                           ulAdoCount   = 0;

    AnscAcquireSpinLock(&pMyObject->AdoIntTableSpinLock);

    for ( i = 0; i < pMyObject->AdoIntTableSize; i++ )
    {
        ulAdoCount += AnscSListQueryDepth(&pMyObject->AdoIntTable[i]);
    }

    AnscReleaseSpinLock(&pMyObject->AdoIntTableSpinLock);

    return  ulAdoCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscAtoGetStrAtomCount
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

ULONG
AnscAtoGetStrAtomCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;
    ULONG                           i            = 0;
    ULONG                           ulAdoCount   = 0;

    AnscAcquireSpinLock(&pMyObject->AdoStrTableSpinLock);

    for ( i = 0; i < pMyObject->AdoStrTableSize; i++ )
    {
        ulAdoCount += AnscSListQueryDepth(&pMyObject->AdoStrTable[i]);
    }

    AnscReleaseSpinLock(&pMyObject->AdoStrTableSpinLock);

    return  ulAdoCount;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscAtoGetIntTableSize
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

ULONG
AnscAtoGetIntTableSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;

    return  pMyObject->AdoIntTableSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoSetIntTableSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSize
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSize
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoSetIntTableSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;

    pMyObject->AdoIntTableSize = ulSize;

    returnStatus = pMyObject->AllocateIntTable((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscAtoGetStrTableSize
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

ULONG
AnscAtoGetStrTableSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;

    return  pMyObject->AdoStrTableSize;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoSetStrTableSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulSize
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulSize
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoSetStrTableSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;

    pMyObject->AdoStrTableSize = ulSize;

    returnStatus = pMyObject->AllocateStrTable((ANSC_HANDLE)pMyObject);

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAtoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAtoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ATOM_TABLE_OBJECT         pMyObject    = (PANSC_ATOM_TABLE_OBJECT)hThisObject;

    pMyObject->DelAllStrAtoms((ANSC_HANDLE)pMyObject);
    pMyObject->DelAllIntAtoms((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
