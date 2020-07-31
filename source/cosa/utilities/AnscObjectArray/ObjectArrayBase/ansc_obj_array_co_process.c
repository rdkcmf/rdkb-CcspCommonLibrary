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

    module:	ansc_uint_array_co_process.c

        For UInt Array object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Object Array Component Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "ansc_obj_array_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscObjectArrayGetSize
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get the size of array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the number of items.

**********************************************************************/

ULONG
AnscObjectArrayGetSize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    return 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscObjectArraySetSize
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNewSize,
                ULONG                       ulGrowBy
            )

    description:

        This function is called to set array size and growth size.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulNewSize
                The new array size.

                ULONG                       ulGrowBy
                The growth size.

    return:     void.

**********************************************************************/

void
AnscObjectArraySetSize
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNewSize,
        ULONG                       ulGrowBy
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulNewSize);
    UNREFERENCED_PARAMETER(ulGrowBy);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscObjectArrayRemoveAll
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to remove all items.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     void.

**********************************************************************/

void
AnscObjectArrayRemoveAll
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_OBJECT_ARRAY_DATA
        AnscObjectArrayGetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex
            )

    description:

        This function is called to get array item at specified position.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position of array item.

    return:     the array data.

**********************************************************************/

ANSC_OBJECT_ARRAY_DATA
AnscObjectArrayGetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulIndex);
    return (ANSC_OBJECT_ARRAY_DATA)NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscObjectArraySetAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to set the array item at the
        specified position with the given value.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position where to put the data.

                ANSC_OBJECT_ARRAY_DATA      Data
                Array item data.

    return:     void.

**********************************************************************/

void
AnscObjectArraySetAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(Data);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscObjectArrayInsertAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ANSC_OBJECT_ARRAY_DATA      Data,
                ULONG                       ulCount
            )

    description:

        This function is called to insert an array item.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The position to insert array items.

                ANSC_OBJECT_ARRAY_DATA      Data
                Array item data.

                ULONG                       ulCount
                The number of items to be inserted.

    return:     void.

**********************************************************************/

void
AnscObjectArrayInsertAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ANSC_OBJECT_ARRAY_DATA      Data,
        ULONG                       ulCount
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(ulCount);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscObjectArrayRemoveAt
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulIndex,
                ULONG                       ulCount
            )

    description:

        This function is called to remove array items.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulIndex
                The starting position.

                ULONG                       ulCount
                The number of items to be removed.

    return:     void.

**********************************************************************/

void
AnscObjectArrayRemoveAt
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulIndex,
        ULONG                       ulCount
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(ulIndex);
    UNREFERENCED_PARAMETER(ulCount);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        AnscObjectArrayAdd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to add the specified data into array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ANSC_OBJECT_ARRAY_DATA      Data
                New item data.

    return:     the position of the new item.

**********************************************************************/

ULONG
AnscObjectArrayAdd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(Data);
    return 0xFFFFFFFF;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        AnscObjectArraySetGrowth
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulGrowth
            )

    description:

        This function is called to set growth size.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

                ULONG                       ulGrowth
                Growth size.

    return:     void.

**********************************************************************/

void
AnscObjectArraySetGrowth
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulGrowth
    )
{
    PANSC_OBJECT_ARRAY_OBJECT       pObjectArray;

    pObjectArray    = (PANSC_OBJECT_ARRAY_OBJECT)hThisObject;

    if (pObjectArray && ulGrowth != (ULONG)-1)  /* 0xFFFFFFFF stands for unchange */
        pObjectArray->ulGrowth  = ulGrowth;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscObjectArrayFind
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_OBJECT_ARRAY_DATA      Data
            )

    description:

        This function is called to find given data in array.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to the string object.

    return:     the position found, -1 if no match.

**********************************************************************/

LONG
AnscObjectArrayFind
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_OBJECT_ARRAY_DATA      Data
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(Data);
    return (LONG)-1;
}

